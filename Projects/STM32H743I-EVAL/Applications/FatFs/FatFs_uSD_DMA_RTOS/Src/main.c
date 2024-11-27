/**
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_DMA_RTOS/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  *          This sample code shows how to use FatFs with uSD card drive in RTOS
  *          mode.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */
BSP_IO_Init_t init;
typedef enum {
  CARD_CONNECTED, 
  CARD_DISCONNECTED,
  CARD_STATUS_CHANGED,
}SD_ConnectionStateTypeDef;

osMessageQId ConnectionEvent;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void FS_AppThread(void const *argument);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void FS_FileOperations(void);
static void SD_Initialize(void);

static uint8_t isInitialized = 0;
static uint8_t isFsCreated = 0;
static __IO uint8_t statusChanged = 0;

uint8_t workBuffer[2 * _MAX_SS];
/* 
 * ensure that the read buffer 'rtext' is 32-Byte aligned in address and size
 * to guarantee that any other data in the cache won't be affected when the 'rtext'
 * is being invalidated.
 */
ALIGN_32BYTES(uint8_t rtext[64]);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  MPU_Config();
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* stm32h7xx HAL library initialization */
  HAL_Init();
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
  BSP_IO_Init(0, &init);
  /* Configure LED1 and LED3 */
  
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);  
  
  /*##-1- Link the micro SD disk I/O driver And Create RTOS environment ######*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    SD_Initialize();
    osThreadDef(uSDThread, FS_AppThread, osPriorityNormal, 0, 8 * configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(uSDThread), NULL); 
    
    /* Create Storage Message Queue */
    osMessageQDef(osqueue, 10, uint16_t);
    ConnectionEvent = osMessageCreate (osMessageQ(osqueue), NULL);  

  }
  /*##-2- Start scheduler ####################################################*/
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
  * @brief  Start task
  * @param  pvParameters not used
  * @retval None
  */
static void FS_AppThread(void const *argument)
{
  osEvent event;

    if(BSP_SD_IsDetected(0))
    {
      osMessagePut ( ConnectionEvent, CARD_CONNECTED, osWaitForever);
    }
    
  /* Infinite Loop */
  for( ;; )
  {
    event = osMessageGet( ConnectionEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
      case CARD_STATUS_CHANGED:
        if (BSP_SD_IsDetected(0))
        {
          osMessagePut ( ConnectionEvent, CARD_CONNECTED, osWaitForever);
        }
        else
        {
          BSP_LED_Off(LED4);
          osMessagePut ( ConnectionEvent, CARD_DISCONNECTED, osWaitForever);
          
        }
        break;
      case CARD_CONNECTED:
        BSP_LED_Off(LED3);
        SD_Initialize();  
        FS_FileOperations();
        statusChanged = 0;
        
        break;

      case CARD_DISCONNECTED:
        BSP_LED_Off(LED1);
        BSP_LED_On(LED3);

        if(isInitialized == 1)
        {
          isInitialized = 0;
          f_mount(NULL, (TCHAR const*)"", 0);
        }
        statusChanged = 0;
        
        break;
      }
    }
  }
}

/**
  * @brief  Main routine for Mass Storage Class
  * @param  None
  * @retval None
  */
static void FS_FileOperations(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs uSD + FreeRTOS"; /* File write buffer */
  
  /* Register the file system object to the FatFs module */
  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
  {
    /* check whether the FS has been already created */
    if (isFsCreated == 0)
    {
      if(f_mkfs(SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
      {
        BSP_LED_On(LED3);
        return;
      }
      isFsCreated = 1;
    }
    /* Create and Open a new text file object with write access */
    if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) 
    {
      /* Write data to the text file */
      res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
      
      if((byteswritten > 0) && (res == FR_OK))
      {
        /* Close the open text file */
        f_close(&MyFile);
        
        /* Open the text file object with read access */
        if(f_open(&MyFile, "STM32.TXT", FA_READ) == FR_OK)
        {
          /* Read data from the text file */
          res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);
          
          if((bytesread > 0) && (res == FR_OK))
          {
            /* Close the open text file */
            f_close(&MyFile);
            
            /* Compare read data with the expected data */
            if((bytesread == byteswritten))
            {                             
              /* Success of the demo: no error occurrence */
              BSP_LED_On(LED1);
              return;              
            }
          }
        }
      }
    }
  }
  /* Error */
  BSP_LED_On(LED3);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(MFX_IRQOUT_PIN == GPIO_Pin)
  {
    if (statusChanged == 0)
    {
      statusChanged = 1;
      osMessagePut ( ConnectionEvent, CARD_STATUS_CHANGED, osWaitForever);
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
    while(1) { ; }
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
    while(1) { ; }
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
  * @brief  Configure the MPU attributes as Write Through for Internal SRAM1.
  * @note   The Base Address 0x24000000 is the SRAM1 accessible by the SDIO internal DMA.
  *         The Configured Region Size is 512KB because same as SRAM1 size.
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

 /* Configure the MPU attributes as Normal Non Cacheable for SRAM1 */

  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

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
#endif /* USE_FULL_ASSERT */ 


static void SD_Initialize(void)
{
  if (isInitialized == 0)
  {
    BSP_SD_Init(0);
     
    if(BSP_SD_IsDetected(0))
    {
      isInitialized = 1;
    }
  }
}
