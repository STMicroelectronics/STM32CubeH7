/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body for Cortex-M7.
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
#include <stdio.h>
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#define UART_HSEM_ID (2U) /* HW semaphore 0*/

#define CM7_FILE   "cm7.log"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS MMCFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char MMCPath[4]; /* SD card logical drive path */
UART_HandleTypeDef     UartHandle;

uint8_t workBuffer[_MAX_SS];
ALIGN_32BYTES(uint8_t rtext[96]);

uint8_t wtext[] = "This is FatFs running on CM7 core"; /* File write buffer */
  
/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void FS_FileOperations(void);
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

static void Error_Handler(void);
static void UART_Config(void);

/* Private functions ---------------------------------------------------------*/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFFF);

  return ch;
}

static void UART_Config(void)
{

  UartHandle.Instance        = USARTx;
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_ODD;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  HAL_UART_DeInit(&UartHandle);
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  int32_t timeout;
   FRESULT res;
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }

  /* STM32H7xx HAL library initialization */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* When system initialization is finished, Cortex-M7 will release Cortex-M4  by means of
  HSEM notification */

  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

  /*Take the Inter-Core sync HSEM */
  LOCK_HSEM(HSEM_ID_0);

  UART_Config();

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  LOCK_HSEM(UART_HSEM_ID);

  /*-1- Link the micro SD disk I/O driver*/
  if(FATFS_LinkDriver(&MMC_Driver, MMCPath) == 0)
  {
    /* Create FAT volume */
     printf("[CM7]:\tCreating FileSystem...\n");
    res = f_mkfs(MMCPath, FM_ANY, 0, workBuffer, sizeof(workBuffer));

    if (res != FR_OK)
    {
      printf("[CM7]:\tCreating FileSystem Failed!...\n");
      Error_Handler();
    }
    printf("[CM7]:\tFileSystem Created successfully!\n");
    /* wakeup the CM4 */
    UNLOCK_HSEM(HSEM_ID_0);
    UNLOCK_HSEM(UART_HSEM_ID);
    /* start the FatFs operations simulaneously with the Core CM4 */
    FS_FileOperations();
  }
}

uint32_t file_size;
static void FS_FileOperations(void)
{
  FRESULT res;
  uint32_t byteswritten, bytesread;

  /* Register the file system object to the FatFs module */
  if(f_mount(&MMCFatFs, (TCHAR const*)MMCPath, 0) != FR_OK)
  {
    goto error;
  }
  /* open the file "CM7.TXT" in write mode */
  res = f_open(&MyFile, CM7_FILE, FA_CREATE_ALWAYS | FA_WRITE);
  if(res != FR_OK)
  {
    goto error;
  }

  LOCK_HSEM(UART_HSEM_ID);
  printf("[CM7]:\topening '%s' for writing...\n", CM7_FILE);
  UNLOCK_HSEM(UART_HSEM_ID);

  /* Write data to the text file */
  res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

  if((byteswritten == 0) || (res != FR_OK))
  {
    f_close(&MyFile);
    goto error;
  }

  LOCK_HSEM(UART_HSEM_ID);
  printf("[CM7]:\t'%s' '%lu' bytes written...\n", CM7_FILE, byteswritten);
  UNLOCK_HSEM(UART_HSEM_ID);

  /* Close the open text file */
  f_close(&MyFile);

  /* Open the text file object with read access */
  if(f_open(&MyFile, CM7_FILE, FA_READ) != FR_OK)
  {
    goto error;
  }

  LOCK_HSEM(UART_HSEM_ID);
  printf("[CM7]:\treading '%s' content...\n", CM7_FILE);
  UNLOCK_HSEM(UART_HSEM_ID);

  /* Read data from the text file */
  res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);
  if((bytesread == 0) || (res != FR_OK))
  {
    f_close(&MyFile);
    goto error;
  }
  /* Close the open text file */
  f_close(&MyFile);

 /* Compare read data with the expected data */
  if(Buffercmp(rtext,  (uint8_t *)wtext,  byteswritten) != 0)
  {
    goto error;
  }
  else
  {
    LOCK_HSEM(UART_HSEM_ID);
    printf("[CM7]:\t%s ==> '>  %s <'\n", CM7_FILE, rtext);
    UNLOCK_HSEM(UART_HSEM_ID);
    while(1)
    {
      BSP_LED_Toggle(LED_RED);
      HAL_Delay(250);
    }
  }


error:
  Error_Handler();
}


/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow :
*            System Clock source            = PLL (HSE)
*            SYSCLK(Hz)                     = 400000000 (Cortex-M7 CPU Clock)
*            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
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
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
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

  /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
  (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
  The I/O Compensation Cell activation  procedure requires :
  - The activation of the CSI clock
  - The activation of the SYSCFG clock
  - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

  To do this please uncomment the following code
  */

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
  */
}

/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  LOCK_HSEM(UART_HSEM_ID);
  printf ("[CM7]:\tError!! \n");
  UNLOCK_HSEM(UART_HSEM_ID);

  while(1)
  {
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

static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}
/**
* @}
*/

/**
* @}
*/

