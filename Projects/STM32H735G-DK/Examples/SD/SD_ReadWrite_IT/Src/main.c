/**
  ******************************************************************************
  * @file    SD/SD_ReadWrite_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use SD through
  *          the STM32H7xx HAL API.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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

/** @addtogroup SD_ReadWrite_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define DATA_SIZE              ((uint32_t)0x06400000U) /* Data Size 100Mo */

/* ------ Buffer Size ------ */
#define BUFFER_SIZE            ((uint32_t)0x00028000U) /* 160Ko */

#define NB_BUFFER              DATA_SIZE / BUFFER_SIZE
#define NB_BLOCK_BUFFER        BUFFER_SIZE / BLOCKSIZE /* Number of Block (512o) by Buffer */
#define BUFFER_WORD_SIZE       (BUFFER_SIZE>>2)        /* Buffer size in Word */


#define SD_TIMEOUT             ((uint32_t)0x00100000U)
#define ADDRESS                ((uint32_t)0x00000400U) /* SD Address to write/read data */
#define DATA_PATTERN           ((uint32_t)0xB5F3A5F3U) /* Data pattern to write */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef SDHandle;
__IO uint8_t RxCplt, TxCplt;

/******** SD Transmission Buffer definition *******/
#if defined ( __ICCARM__ )
#pragma location = 0x24000000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint8_t aTxBuffer[BUFFER_WORD_SIZE*4];
/**************************************************/

/******** SD Receive Buffer definition *******/
#if defined ( __ICCARM__ )
#pragma location = 0x24028000
#elif defined ( __CC_ARM )
__attribute__((section (".RAM_D1")))
#elif defined ( __GNUC__ )
__attribute__((section (".RAM_D1")))
#endif
uint8_t aRxBuffer[BUFFER_WORD_SIZE*4];
/**************************************************/
/* UART handler declaration, used for printf */
UART_HandleTypeDef UartHandle;

__IO uint8_t step = 0;
uint32_t start_time = 0;
uint32_t stop_time = 0;
uint32_t loop_index = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void UART_Config(void);
static uint8_t Wait_SDCARD_Ready(void);
static void Error_Handler(void);

#ifdef __GNUC__ /* __GNUC__ */
 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
 #else
 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
 #endif /* __GNUC__ */


/* Private functions ---------------------------------------------------------*/
void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t index = 0;

  
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  HAL_SD_CardCIDTypedef pCID;
  HAL_SD_CardCSDTypedef pCSD;

 /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  /*##-1- Initialize LEDs mounted on STM32H735G-DK board #####################*/
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /*##-2- Configure USART for printf messages #####################*/
  UART_Config();

  /*##-3- Initialize SD instance #####################*/
  SDHandle.Instance = SDMMC1;
  HAL_SD_DeInit(&SDHandle);

  /* if CLKDIV = 0 then SDMMC Clock frequency = SDMMC Kernel Clock
     else SDMMC Clock frequency = SDMMC Kernel Clock / [2 * CLKDIV].
  SDMMC Kernel Clock = 200MHz, SDMMC Clock frequency = 25MHz  */
  SDHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  SDHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  SDHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
  SDHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  SDHandle.Init.ClockDiv            = 4;

  if(HAL_SD_Init(&SDHandle) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_SD_Erase(&SDHandle, ADDRESS, ADDRESS+BUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  if(Wait_SDCARD_Ready() != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SD_GetCardCID(&SDHandle, &pCID);
  HAL_SD_GetCardCSD(&SDHandle, &pCSD);

  while(1)
  {
    switch(step)
    {
      case 0:
      {
        /*##- 4 - Initialize Transmission buffer #####################*/
        for (index = 0; index < BUFFERSIZE; index++)
        {
          aTxBuffer[index] = DATA_PATTERN + index;
        }
        printf(" ****************** Start Write test ******************* \n");
        printf(" - Buffer size to write: %lu MB   \n", (DATA_SIZE>>20));
        index = 0;
        start_time = HAL_GetTick();
        step++;
      }
      break;
      case 1:
      {
        if(Wait_SDCARD_Ready() != HAL_OK)
        {
          Error_Handler();
        }
        TxCplt = 0;
        /*##- 5 - Start Transmission buffer #####################*/
        if(HAL_SD_WriteBlocks_IT(&SDHandle, aTxBuffer, ADDRESS + (loop_index * NB_BLOCK_BUFFER), NB_BLOCK_BUFFER) != HAL_OK)
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
          /* Toggle LED1 Transfer of Buffer OK */
          BSP_LED_Toggle(LED1);

          /* Transfer of Buffer completed */
          index++;
          if(index<NB_BUFFER)
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
        for (index = 0; index < BUFFERSIZE; index++)
        {
          aRxBuffer[index] = 0;
        }
        printf(" ******************* Start Read test ******************* \n");
        printf(" - Buffer size to read: %lu MB   \n", (DATA_SIZE>>20));
        start_time = HAL_GetTick();
        index = 0;
        step++;
      }
      break;
      case 4:
      {
        if(Wait_SDCARD_Ready() != HAL_OK)
        {
          Error_Handler();
        }
        /*##- 7 - Start Reception buffer #####################*/
        RxCplt = 0;
        if(HAL_SD_ReadBlocks_IT(&SDHandle, aRxBuffer, ADDRESS + (loop_index * NB_BLOCK_BUFFER), NB_BLOCK_BUFFER) != HAL_OK)
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
          /* Toggle LED1, Transfer of Buffer OK */
          BSP_LED_Toggle(LED1);
          /* Transfer of Buffer completed */
          index++;
          if(index<NB_BUFFER)
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
        index=0;
        printf(" ********************* Check data ********************** \n");
        while((index<BUFFERSIZE) && (aRxBuffer[index] == aTxBuffer[index]))
        {
          index++;
        }

        if(index != BUFFERSIZE)
        {
          printf(" - Check data Error !!!!   \n");
          Error_Handler();
        }
        printf(" - Check data OK  \n");
        /* Toggle LED1, Check Transfer OK */
        BSP_LED_On(LED1);
        step = 0;
        loop_index ++;
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
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 104
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 104;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) {};
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
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    while(1) {};
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
  printf("\n\r #        SD Example: Write Read with IT mode          # \r");
  printf("\n\r ####################################################### \n\r");



}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  printf(" - Error \n");
  BSP_LED_Off(LED1);
  while(1)
  {
    /* Toggle LED_RED: Error */
    BSP_LED_Toggle(LED2);
  }
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
  * @brief  Fills the goal 32-bit length buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: length of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset)
{
  uint16_t IndexTmp;

  /* Put in global buffer same values */
  for ( IndexTmp = 0; IndexTmp < BufferLength; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
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

