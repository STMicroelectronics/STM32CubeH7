/**
  ******************************************************************************
  * @file    SD/SD_ReadWrite_DMADoubleBuffer/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use SD through
  *          the STM32H7xx HAL API.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup SD_ReadWrite_DMADoubleBuffer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define DATA_SIZE              ((uint32_t)0x06400000U) /* Data Size 100MB */

/* ------ Buffer0 & Buffer1 Size ------ */
#define BUFFER_SIZE            ((uint32_t)0x00001000U) /* 4KB */
#define BUFFER_WORD_SIZE       (BUFFER_SIZE>>2)        /* Buffer size in Word */
#define NB_BLOCK_BUFFER        BUFFER_SIZE / BLOCKSIZE /* Number of Block (512B) by Buffer */

#define LOOP_BUFFER_SIZE       ((uint32_t)0x01900000U) /* 25MB */

#define NB_LOOP                ((uint32_t)DATA_SIZE / LOOP_BUFFER_SIZE)

#define SD_TIMEOUT             ((uint32_t)0x01000000U)
#define DATA_ADDRESS           ((uint32_t)0x00000000U) /* SD Address to write/read data */
#define DATA_PATTERN0          ((uint32_t)0x01000000U) /* Data pattern for buffer0*/
#define DATA_PATTERN1          ((uint32_t)0x02000000U) /* Data pattern for buffer1 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef SDHandle;
__IO uint8_t RxCplt, TxCplt;

/******** SD Buffer0 definition *******/
#if defined ( __ICCARM__ )
#pragma location = 0x24000000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint32_t Buffer0[BUFFER_WORD_SIZE];
/**************************************************/

/******** SD Buffer1 definition *******/
#if defined ( __ICCARM__ )
#pragma location = 0x24010000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint32_t Buffer1[BUFFER_WORD_SIZE];

/**************************************************/
/* UART handler declaration, used for printf */
UART_HandleTypeDef UartHandle;

__IO uint8_t step = 0;
uint32_t start_time = 0;
uint32_t stop_time = 0;
__IO uint32_t RBuff_0=0;
__IO uint32_t RBuff_1=0;
__IO uint32_t WBuff_0=0;
__IO uint32_t WBuff_1=0;
uint32_t address = DATA_ADDRESS;
HAL_StatusTypeDef ReadError= HAL_OK;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void UART_Config(void);
static uint8_t Wait_SDCARD_Ready(void);
static void CPU_CACHE_Enable(void);

#ifdef __GNUC__ /* __GNUC__ */
 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
 #else
 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
 #endif /* __GNUC__ */


/* Private functions ---------------------------------------------------------*/
static void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset);
static HAL_StatusTypeDef Buffercmp(uint32_t* pBuffer, uint16_t BufferLength, uint32_t Offset);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t index=0;
  BSP_IO_Init_t io_init_structure;

  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /*##-1- Initialize LEDs mounted on STM32H743I-EVAL board #####################*/
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_ORANGE);
  BSP_LED_Init(LED_RED);
  
  /*##-2- Configure USART for printf messages #####################*/
  UART_Config();
  
  /* Initialise Transciver MFXPIN to enable 1.8V Switch mode    */
  io_init_structure.Pin  = IO_PIN_13;
  io_init_structure.Pull = IO_PULLDOWN;
  io_init_structure.Mode = IO_MODE_OUTPUT_PP;
  
  BSP_IO_Init(0, &io_init_structure);
  
  /*##-3- Initialize SD instance #####################*/
  SDHandle.Instance = SDMMC1;
  HAL_SD_DeInit(&SDHandle);
  
  /* if CLKDIV = 0 then SDMMC Clock frequency = SDMMC Kernel Clock
     else SDMMC Clock frequency = SDMMC Kernel Clock / [2 * CLKDIV]. 
     SDMMC Kernel Clock = 200MHz, DMMC Clock frequency = 100MHz  */
  SDHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_FALLING;
  SDHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  SDHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
  SDHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
  SDHandle.Init.ClockDiv            = 2;
  
  if(HAL_SD_Init(&SDHandle) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_SD_ConfigSpeedBusOperation(&SDHandle,SDMMC_SPEED_MODE_ULTRA) != HAL_OK)
  {
    Error_Handler();
  }  

  while(1)
  {
    switch(step)
    {
      case 0:
      {
        /*##- 4 - Initialize Transmission buffers #####################*/
        Fill_Buffer(Buffer0, BUFFERSIZE, DATA_PATTERN0); 
        Fill_Buffer(Buffer1, BUFFERSIZE, DATA_PATTERN1);
        
        address = DATA_ADDRESS;
        index=0;
        RBuff_0=0;
        RBuff_1=0;
        WBuff_0=0;
        WBuff_1=0;

        /* Configure Buffers Address and Buffer Size */
        if(HAL_SDEx_ConfigDMAMultiBuffer(&SDHandle, Buffer0, Buffer1, BUFFER_SIZE/BLOCKSIZE) != HAL_OK)
        {
          Error_Handler();
        }
        printf(" ****************** Start Write test ******************* \n");
        printf(" - Buffer size to write: %lu MB   \n", (DATA_SIZE>>20));

        start_time = HAL_GetTick();
        step++;
      }
      break;
      case 1:
      {
        TxCplt = 0;
        if(Wait_SDCARD_Ready() != HAL_OK)
        {
          Error_Handler();
        }
        /*##- 5 - Start Transmission buffer #####################*/
        if(HAL_SDEx_WriteBlocksDMAMultiBuffer(&SDHandle, address, LOOP_BUFFER_SIZE/BLOCKSIZE) != HAL_OK)
        {
          Error_Handler();
        }
        
        step++;
      }
      break;
      case 2:
      {
        if(TxCplt != 0)
        {
          /* Toggle Led Orange, Transfer of Buffer OK */
          BSP_LED_Toggle(LED_ORANGE);
          
          index++;
          address += LOOP_BUFFER_SIZE/BLOCKSIZE;
          
          if(index < NB_LOOP)
          {
            /* More data need to be transferred */
            step--;
          }
          else
          {
            stop_time = HAL_GetTick();
            printf(" - Write Time(ms): %lu  -  Write Speed: %02.2f MB/s  \n", stop_time - start_time, (float)((float)(DATA_SIZE>>10)/(float)(stop_time - start_time)));
            /* All data are transferred */
            step++;
          }
          
        }
      }
      break;
      case 3:
      {
        /*##- 6 - Initialize Reception buffer #####################*/
        Fill_Buffer(Buffer0, BUFFERSIZE, 0); 
        Fill_Buffer(Buffer1, BUFFERSIZE, 0); 
        address = DATA_ADDRESS;
        index=0;
        
        printf(" ******************* Start Read test ******************* \n");
        printf(" - Buffer size to read: %lu MB   \n", (DATA_SIZE>>20));
        start_time = HAL_GetTick();

        step++;
      }
      break;
      case 4:
      {
        /*##- 7 - Initialize Reception buffer #####################*/
        RxCplt = 0;
        if(Wait_SDCARD_Ready() != HAL_OK)
        {
          Error_Handler();
        }
        if(HAL_SDEx_ReadBlocksDMAMultiBuffer(&SDHandle, address, LOOP_BUFFER_SIZE/BLOCKSIZE) != HAL_OK)
        {
          Error_Handler();
        }

        step++;
      }
      break;
      case 5:
      {
        if(RxCplt != 0)
        {
          /* Toggle Led Orange, Transfer of Buffer OK */
          BSP_LED_Toggle(LED_ORANGE);
          
          
          /* Transfer of Buffer completed */
          address += LOOP_BUFFER_SIZE/BLOCKSIZE;
          index++;
          if(index<NB_LOOP)
          {
            /* More data need to be transferred */
            step--;
          }
          else
          {
            stop_time = HAL_GetTick();
            printf(" - Read Time(ms): %lu  -  Read Speed: %02.2f MB/s  \n", stop_time - start_time, (float)((float)(DATA_SIZE>>10)/(float)(stop_time - start_time)));
            /* All data are transferred */
            step++;
          }

        }
      }
      break;
      case 6:
      {
        /*##- 8 - Check Reception buffer #####################*/
        
        printf(" ********************* Check data ********************** \n");
        
        if(ReadError != HAL_OK)
        {
          printf(" - Check data Error !!!!   \n");
          Error_Handler();
        }
        else
        {
          printf(" - Check data OK  \n");
          /* Toggle Green LED, Check Transfer OK */
          BSP_LED_Toggle(LED_GREEN);
          step=0;
        }
      }
      break;
      default :
        Error_Handler();
    }
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef   RCC_ClkInitStruct;
  RCC_OscInitTypeDef   RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  /* PLL_VCO Input = HSE_VALUE/PLL_M = 5 Mhz */
  /* PLL_VCO Output = PLL_VCO Input * PLL_N = 800 Mhz */
  /* SDMMC Kernel Clock = PLL1_VCO Output/PLL_Q = 800/4 = 200 Mhz */
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
  
/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                  RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);
  
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
  
    /*activate CSI clock mondatory for I/O Compensation Cell*/
  __HAL_RCC_CSI_ENABLE() ;

  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  /* Enables the I/O Compensation Cell */
  HAL_EnableCompensationCell();
}

/**
  * @brief  Enable the SD Transciver 1.8V Mode Callback.
  * @param  None
  * @retval None
  */
void HAL_SD_DriveTransceiver_1_8V_Callback(FlagStatus status)
{
  if(status == SET)
  {
    BSP_IO_WritePin(0, IO_PIN_13, IO_PIN_SET);  
  }
  else
  {
    BSP_IO_WritePin(0, IO_PIN_13, IO_PIN_RESET);
  }
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  RxCplt=1;
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  TxCplt=1;
}

/**
  * @brief SD error callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  Error_Handler();
}

/**
  * @brief Read DMA Buffer 0 Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Read_DMADoubleBuffer0CpltCallback(SD_HandleTypeDef *hsd)
{
  SCB_InvalidateDCache_by_Addr(Buffer0, BUFFER_WORD_SIZE*4);
  ReadError += Buffercmp(Buffer0, BUFFERSIZE, DATA_PATTERN0 + (RBuff_0 * (uint32_t)0x00010000));
  RBuff_0++;
  
}

/**
  * @brief Read DMA Buffer 1 Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Read_DMADoubleBuffer1CpltCallback(SD_HandleTypeDef *hsd)
{
  SCB_InvalidateDCache_by_Addr(Buffer1, BUFFER_WORD_SIZE*4);
  ReadError += Buffercmp(Buffer1, BUFFERSIZE, DATA_PATTERN1 + (RBuff_1 * (uint32_t)0x00010000));
  RBuff_1++;
}

/**
  * @brief Write DMA Buffer 0 Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Write_DMADoubleBuffer0CpltCallback(SD_HandleTypeDef *hsd)
{
  WBuff_0++;
  Fill_Buffer(Buffer0, BUFFERSIZE, DATA_PATTERN0 + (WBuff_0 * (uint32_t)0x00010000)); 
  
}

/**
  * @brief Write DMA Buffer 1 Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Write_DMADoubleBuffer1CpltCallback(SD_HandleTypeDef *hsd)
{
  WBuff_1++;
  Fill_Buffer(Buffer1, BUFFERSIZE, DATA_PATTERN1 + (WBuff_1 * (uint32_t)0x00010000)); 
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  printf(" - Error \n");
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_ORANGE);
  while(1)
  {
    /* Toggle LED_RED: Error */
    BSP_LED_Toggle(LED_RED);
  }
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}


/**
  * @brief  This function is executed to configure in case of error occurrence.
  * @param  None
  * @retval None
  */
static void UART_Config(void)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : 
                      BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Output a message on Hyperterminal using printf function */
  printf("\n\r ####################################################### \r");
  printf("\n\r #   SD Example: Write Read with MultiBuffer DMA mode  # \r");
  printf("\n\r ####################################################### \n\r");

  

}

/**
  * @brief  Fills the goal 32-bit length buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: length of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset)
{
  uint16_t IndexTmp;

  /* Put in global buffer same values */
  for ( IndexTmp = 0; IndexTmp <= BufferLength; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
  SCB_CleanDCache_by_Addr(pBuffer, BufferLength*4);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
static HAL_StatusTypeDef Buffercmp(uint32_t* pBuffer, uint16_t BufferLength, uint32_t Offset)
{
  uint32_t counter = 0;
  while(BufferLength--)
  {
    if(pBuffer[counter] != counter + Offset)
    {
      return HAL_ERROR;
    }
    counter++;
  }
  return HAL_OK;
}
/**
  * @brief  Wait SD Card ready status
  * @param  None
  * @retval None
  */
static uint8_t Wait_SDCARD_Ready(void)
{
  uint32_t loop = SD_TIMEOUT;
  
  /* Wait for the Erasing process is completed */
  /* Verify that SD card is ready to use after the Erase */
  while(loop > 0)
  {
    loop--;
    if(HAL_SD_GetCardState(&SDHandle) == HAL_SD_CARD_TRANSFER)
    {
        return HAL_OK;
    }
  }
  return HAL_ERROR;
}
/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}


/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

