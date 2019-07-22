/**
  ******************************************************************************
  * @file    SPDIFRX/SPDIFRX_AudioPlay/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to play audio by using the SPDIFRX 
  *          interface via the STM32H7xx HAL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup SPDIFRX_AudioPlay
  * @{
  */
  
/* Private define ------------------------------------------------------------*/
#define PLAY_BUFF_SIZE      4096
#define AUDIO_FREQUENCY     SAI_AUDIO_FREQUENCY_16K

/* Private typedef -----------------------------------------------------------*/
typedef enum AUDIO_BufferStatus {
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL
} AUDIO_BufferStatus_e;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SAI_HandleTypeDef           SaiOutputHandle;
SPDIFRX_HandleTypeDef       SpdifrxHandle;
AUDIO_DrvTypeDef            *audio_drv;
__IO uint32_t               spdifrxBuf[PLAY_BUFF_SIZE];
__IO uint16_t               saiOutPlayBuff[PLAY_BUFF_SIZE];
__IO AUDIO_BufferStatus_e   bufferStatus = BUFFER_OFFSET_NONE;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void Playback_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t index;

  /* Configure the MPU attributes as Normal Non Cacheable for D1 AXI SRAM */
  MPU_Config();
  
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

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
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
  /* Configure LED1 (Green)*/
  BSP_LED_Init(LED1);
  
  /* Configure LED3 (Red)*/
  BSP_LED_Init(LED3);

  /* Initialize playback */
  Playback_Init();

  /* Start the playback */
  if(0 != audio_drv->Play(AUDIO_I2C_ADDRESS, NULL, 0))
  {
    Error_Handler();
  }

  /* Start SPDIFRX audio Reception in DMA circular mode */
  HAL_SPDIFRX_ReceiveDataFlow_DMA(&SpdifrxHandle, (uint32_t *)spdifrxBuf, PLAY_BUFF_SIZE);
  
  if(SpdifrxHandle.ErrorCode != HAL_SPDIFRX_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Start SAI audio Transmission in DMA circular mode */  
  if(HAL_OK != HAL_SAI_Transmit_DMA(&SaiOutputHandle, (uint8_t *)saiOutPlayBuff, PLAY_BUFF_SIZE))
  {
    Error_Handler();
  }
  
  /* Infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED1);
    
    /* Initialize buffer offset */
    bufferStatus &= BUFFER_OFFSET_NONE;
    
    /* Update the first part of the buffer */
    while((bufferStatus & BUFFER_OFFSET_HALF) != BUFFER_OFFSET_HALF);
    for(index = 0;  index < PLAY_BUFF_SIZE/2; index++)
    {
      saiOutPlayBuff[index] = (uint16_t) spdifrxBuf[index] ;
    }
    
    /* Initialize buffer offset */
    bufferStatus &= BUFFER_OFFSET_NONE;
    
    /* Update the second part of the buffer */
    while((bufferStatus & BUFFER_OFFSET_FULL) != BUFFER_OFFSET_FULL);
    for(index = PLAY_BUFF_SIZE/2;  index < PLAY_BUFF_SIZE; index++)
    {
      saiOutPlayBuff[index] = (uint16_t) spdifrxBuf[index] ;
    }    
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CM7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (CM4 CPU, AXI and AHBs Clock)
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
  
  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

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
  * @brief  Playback initialization
  * @param  None
  * @retval None
  */
static void Playback_Init(void)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  
  /* Configure PLLSAI prescalers */
  /* PLL2SAI_VCO: VCO_429M
     SAI_CLK(first level) = PLLSAI_VCO/PLLSAIP = 429/38 = 11.289 Mhz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  RCC_PeriphCLKInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2P = 38;
  RCC_PeriphCLKInitStruct.PLL2.PLL2Q = 1;
  RCC_PeriphCLKInitStruct.PLL2.PLL2R = 1;
  RCC_PeriphCLKInitStruct.PLL2.PLL2N = 429;
  RCC_PeriphCLKInitStruct.PLL2.PLL2FRACN = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2M = 25;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure SAI Peripheral */
  __HAL_SAI_RESET_HANDLE_STATE(&SaiOutputHandle);

  SaiOutputHandle.Instance = AUDIO_SAIx;

  __HAL_SAI_DISABLE(&SaiOutputHandle);

  SaiOutputHandle.Init.AudioMode      = SAI_MODEMASTER_TX;
  SaiOutputHandle.Init.Synchro        = SAI_ASYNCHRONOUS;
  SaiOutputHandle.Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
  SaiOutputHandle.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
  SaiOutputHandle.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
  SaiOutputHandle.Init.AudioFrequency = AUDIO_FREQUENCY;
  SaiOutputHandle.Init.Protocol       = SAI_FREE_PROTOCOL;
  SaiOutputHandle.Init.DataSize       = SAI_DATASIZE_16;
  SaiOutputHandle.Init.FirstBit       = SAI_FIRSTBIT_MSB;
  SaiOutputHandle.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;

  SaiOutputHandle.FrameInit.FrameLength       = 64;
  SaiOutputHandle.FrameInit.ActiveFrameLength = 32;
  SaiOutputHandle.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  SaiOutputHandle.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  SaiOutputHandle.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  SaiOutputHandle.SlotInit.FirstBitOffset = 0;
  SaiOutputHandle.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  SaiOutputHandle.SlotInit.SlotNumber     = 2; 
  SaiOutputHandle.SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

  if(HAL_OK != HAL_SAI_Init(&SaiOutputHandle))
  {
    Error_Handler();
  }
  
  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(&SaiOutputHandle);
  
  /* Initialize audio driver */
  if(WM8994_ID != wm8994_drv.ReadID(AUDIO_I2C_ADDRESS))
  {
    Error_Handler();
  }
  
  audio_drv = &wm8994_drv;
  audio_drv->Reset(AUDIO_I2C_ADDRESS);  
  if(0 != audio_drv->Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_HEADPHONE, 40, AUDIO_FREQUENCY))
  {
    Error_Handler();
  }  
  
  /* Configure SPDIFRX Peripheral */
  SpdifrxHandle.Instance = SPDIFRX;
  HAL_SPDIFRX_DeInit(&SpdifrxHandle); 
  
  SpdifrxHandle.Init.InputSelection = SPDIFRX_INPUT_IN0;
  SpdifrxHandle.Init.Retries = SPDIFRX_MAXRETRIES_15;
  SpdifrxHandle.Init.WaitForActivity = SPDIFRX_WAITFORACTIVITY_ON;
  SpdifrxHandle.Init.ChannelSelection = SPDIFRX_CHANNEL_A;
  SpdifrxHandle.Init.DataFormat = SPDIFRX_DATAFORMAT_LSB;
  SpdifrxHandle.Init.StereoMode = SPDIFRX_STEREOMODE_ENABLE;
  SpdifrxHandle.Init.PreambleTypeMask = SPDIFRX_PREAMBLETYPEMASK_ON;
  SpdifrxHandle.Init.ChannelStatusMask = SPDIFRX_CHANNELSTATUS_ON;  
  if(HAL_SPDIFRX_Init(&SpdifrxHandle) != HAL_OK)
  {
    /* Initialization error */
    Error_Handler();
  }
}

/**
  * @brief Rx Transfer (Data flow) half completed callbacks
  * @param hspdif: SPDIFRX handle
  * @retval None
  */
void HAL_SPDIFRX_RxHalfCpltCallback(SPDIFRX_HandleTypeDef *hspdif)
{  
  /* Update buffer status */
  bufferStatus |= BUFFER_OFFSET_HALF;
}

/**
  * @brief Rx Transfer (Data flow) completed callbacks
  * @param hspdif: SPDIFRX handle
  * @retval None
  */
void HAL_SPDIFRX_RxCpltCallback(SPDIFRX_HandleTypeDef *hspdif)
{
  /* Update buffer status */
  bufferStatus |= BUFFER_OFFSET_FULL;
}

/**
  * @brief  Configure the MPU attributes as Normal Non Cacheable for D1 AXI SRAM.
  * @note   The D1SRAM Base Address is 0x24000000 with a size of 512KB.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as as Normal Non Cacheable for D1 AXI SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* LED3 On in error case */
  BSP_LED_On(LED3);
  
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
