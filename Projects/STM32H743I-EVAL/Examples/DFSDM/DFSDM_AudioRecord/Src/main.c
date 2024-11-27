/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DFSDM HAL API to realize
  *          audio recording.
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

/** @addtogroup DFSDM_AudioRecord
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/* Private variables ---------------------------------------------------------*/
DFSDM_Channel_HandleTypeDef  DfsdmLeftChannelHandle;
DFSDM_Channel_HandleTypeDef  DfsdmRightChannelHandle;
DFSDM_Filter_HandleTypeDef   DfsdmLeftFilterHandle;
DFSDM_Filter_HandleTypeDef   DfsdmRightFilterHandle;
SAI_HandleTypeDef            SaiHandle;
AUDIO_Drv_t                  *Audio_Drv = NULL;
void *Audio_CompObj = NULL;
WM8994_Init_t codec_init;

/*Buffer location and size should aligned to cache line size (32 bytes) */
ALIGN_32BYTES(int32_t                      LeftRecBuff[2048]);
ALIGN_32BYTES(int32_t                      RightRecBuff[2048]);
ALIGN_32BYTES(int16_t                      PlayBuff[4096]);
uint32_t                     DmaLeftRecHalfBuffCplt  = 0;
uint32_t                     DmaLeftRecBuffCplt      = 0;
uint32_t                     DmaRightRecHalfBuffCplt = 0;
uint32_t                     DmaRightRecBuffCplt     = 0;
uint32_t                     PlaybackStarted         = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void DFSDM_Init(void);
static void Playback_Init(void);
static void CPU_CACHE_Enable(void);
static int32_t WM8994_Probe(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t i;
  WM8994_Init_t codec_init; 

  
  /* Configure the MPU attributes */
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

  /* Configure the system clock to have a frequency of 400 MHz */
  SystemClock_Config();

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Initialize DFSDM channels and filter for record */
  DFSDM_Init();

  /* Initialize playback */
  Playback_Init();
  
  /* Start DFSDM conversions */
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmRightFilterHandle, RightRecBuff, 2048))
  {
    Error_Handler();
  }
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmLeftFilterHandle, LeftRecBuff, 2048))
  {
    Error_Handler();
  }
  
  /* Start loopback */
  while(1)
  {
    if((DmaLeftRecHalfBuffCplt == 1) && (DmaRightRecHalfBuffCplt == 1))
    {
      /* Store values on Play buff */
      for(i = 0; i < 1024; i++)
      {
        PlayBuff[2*i]     = SaturaLH((LeftRecBuff[i] >> 8), -32768, 32767);
        PlayBuff[(2*i)+1] = SaturaLH((RightRecBuff[i] >> 8), -32768, 32767);
      }
      
      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)PlayBuff, sizeof(PlayBuff)/2);
      
      if(PlaybackStarted == 0)
      {
        codec_init.Resolution   = 0;
        
        /* Fill codec_init structure */
        codec_init.Frequency    = 16000;
        codec_init.InputDevice  = WM8994_IN_NONE;
        codec_init.OutputDevice = AUDIO_OUT_DEVICE_HEADPHONE;
        
        /* Convert volume before sending to the codec */
        codec_init.Volume       = VOLUME_OUT_CONVERT(100);
        
        /* Initialize the codec internal registers */
        if(Audio_Drv->Init(Audio_CompObj, &codec_init) != 0)
        {
          Error_Handler();
        }
        
        /* Start the playback */
        if(Audio_Drv->Play(Audio_CompObj) < 0)
        {
          Error_Handler();
        }
        if(HAL_OK != HAL_SAI_Transmit_DMA(&SaiHandle, (uint8_t *) &PlayBuff[0], 4096))
        {
          Error_Handler();
        }
        PlaybackStarted = 1;
      }      
      DmaLeftRecHalfBuffCplt  = 0;
      DmaRightRecHalfBuffCplt = 0;
    }
    if((DmaLeftRecBuffCplt == 1) && (DmaRightRecBuffCplt == 1))
    {
      /* Store values on Play buff */
      for(i = 1024; i < 2048; i++)
      {
        PlayBuff[2*i]     = SaturaLH((LeftRecBuff[i] >> 8), -32768, 32767);
        PlayBuff[(2*i)+1] = SaturaLH((RightRecBuff[i] >> 8), -32768, 32767);
      }
      
      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&PlayBuff[2048], sizeof(PlayBuff)/2);
      
      DmaLeftRecBuffCplt  = 0;
      DmaRightRecBuffCplt = 0;
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
}

/**
  * @brief  DFSDM channels and filter initialization
  * @param  None
  * @retval None
  */
static void DFSDM_Init(void)
{
  /* Initialize channel 1 (left channel)*/
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmLeftChannelHandle);
  DfsdmLeftChannelHandle.Instance                      = DFSDM1_Channel1;
  DfsdmLeftChannelHandle.Init.OutputClock.Activation   = ENABLE;
  DfsdmLeftChannelHandle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmLeftChannelHandle.Init.OutputClock.Divider      = 4; /* 11.294MHz/4 = 2.82MHz */
  DfsdmLeftChannelHandle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmLeftChannelHandle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmLeftChannelHandle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  DfsdmLeftChannelHandle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
  DfsdmLeftChannelHandle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmLeftChannelHandle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmLeftChannelHandle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmLeftChannelHandle.Init.Offset                   = 0;
  DfsdmLeftChannelHandle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmLeftChannelHandle))
  {
    Error_Handler();
  }

  /* Initialize channel 0 (right channel)*/
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmRightChannelHandle);
  DfsdmRightChannelHandle.Instance                      = DFSDM1_Channel0;
  DfsdmRightChannelHandle.Init.OutputClock.Activation   = ENABLE;
  DfsdmRightChannelHandle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmRightChannelHandle.Init.OutputClock.Divider      = 4; /* 11.294MHz/4 = 2.82MHz */
  DfsdmRightChannelHandle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmRightChannelHandle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmRightChannelHandle.Init.Input.Pins               = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  DfsdmRightChannelHandle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_FALLING;
  DfsdmRightChannelHandle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmRightChannelHandle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmRightChannelHandle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmRightChannelHandle.Init.Offset                   = 0;
  DfsdmRightChannelHandle.Init.RightBitShift            = 2;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmRightChannelHandle))
  {
    Error_Handler();
  }
  
  /* Initialize filter 0 (left channel) */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmLeftFilterHandle);
  DfsdmLeftFilterHandle.Instance                          = DFSDM1_Filter0;
  DfsdmLeftFilterHandle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  DfsdmLeftFilterHandle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmLeftFilterHandle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmLeftFilterHandle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
  DfsdmLeftFilterHandle.Init.InjectedParam.ScanMode       = ENABLE; /* N.U. */
  DfsdmLeftFilterHandle.Init.InjectedParam.DmaMode        = DISABLE; /* N.U. */
  DfsdmLeftFilterHandle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO; /* N.U. */
  DfsdmLeftFilterHandle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_RISING_EDGE; /* N.U. */
  DfsdmLeftFilterHandle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  DfsdmLeftFilterHandle.Init.FilterParam.Oversampling     = 64; /* 11.294MHz/(4*64) = 44.1KHz */
  DfsdmLeftFilterHandle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmLeftFilterHandle))
  {
    Error_Handler();
  }

  /* Initialize filter 1 (right channel) */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmRightFilterHandle);
  DfsdmRightFilterHandle.Instance                          = DFSDM1_Filter1;
  DfsdmRightFilterHandle.Init.RegularParam.Trigger         = DFSDM_FILTER_SYNC_TRIGGER;
  DfsdmRightFilterHandle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmRightFilterHandle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmRightFilterHandle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
  DfsdmRightFilterHandle.Init.InjectedParam.ScanMode       = ENABLE; /* N.U. */
  DfsdmRightFilterHandle.Init.InjectedParam.DmaMode        = DISABLE; /* N.U. */
  DfsdmRightFilterHandle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO; /* N.U. */
  DfsdmRightFilterHandle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_RISING_EDGE; /* N.U. */
  DfsdmRightFilterHandle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
  DfsdmRightFilterHandle.Init.FilterParam.Oversampling     = 64; /* 11.294MHz/(4*64) = 44.1KHz */
  DfsdmRightFilterHandle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmRightFilterHandle))
  {
    Error_Handler();
  }
  
  /* Configure regular channel and continuous mode for filter 0 (left channel) */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmLeftFilterHandle, DFSDM_CHANNEL_1, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  }

  /* Configure regular channel and continuous mode for filter 1 (right channel) */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmRightFilterHandle, DFSDM_CHANNEL_0, DFSDM_CONTINUOUS_CONV_ON))
  {
    Error_Handler();
  }
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
  
  /* Initialize SAI */
  __HAL_SAI_RESET_HANDLE_STATE(&SaiHandle);

  SaiHandle.Instance            = AUDIO_OUT_SAIx;

  SaiHandle.Init.AudioMode      = SAI_MODEMASTER_TX;
  SaiHandle.Init.Synchro        = SAI_ASYNCHRONOUS;
  SaiHandle.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
  SaiHandle.Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
  SaiHandle.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
  SaiHandle.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
  SaiHandle.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
  SaiHandle.Init.Mckdiv         = 0; /* N.U */
  SaiHandle.Init.MonoStereoMode = SAI_STEREOMODE;
  SaiHandle.Init.CompandingMode = SAI_NOCOMPANDING;
  SaiHandle.Init.TriState       = SAI_OUTPUT_NOTRELEASED;
  SaiHandle.Init.Protocol       = SAI_FREE_PROTOCOL;
  SaiHandle.Init.DataSize       = SAI_DATASIZE_16;
  SaiHandle.Init.FirstBit       = SAI_FIRSTBIT_MSB;
  SaiHandle.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;

  SaiHandle.FrameInit.FrameLength       = 32; 
  SaiHandle.FrameInit.ActiveFrameLength = 16;
  SaiHandle.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  SaiHandle.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  SaiHandle.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  SaiHandle.SlotInit.FirstBitOffset = 0;
  SaiHandle.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  SaiHandle.SlotInit.SlotNumber     = 2; 
  SaiHandle.SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

  if(HAL_OK != HAL_SAI_Init(&SaiHandle))
  {
    Error_Handler();
  }
  
  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(&SaiHandle);
  
  if(WM8994_Probe() != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  if(Audio_Drv->Init(Audio_CompObj, &codec_init)!=0)
  {
    Error_Handler();
  }
}

/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t WM8994_Probe(void)
{
  int32_t ret = BSP_ERROR_NONE;
  WM8994_IO_t              IOCtx;
  static WM8994_Object_t   WM8994Obj;
  uint32_t id;

  /* Configure the audio driver */
  IOCtx.Address     = AUDIO_I2C_ADDRESS;
  IOCtx.Init        = BSP_I2C1_Init;
  IOCtx.DeInit      = BSP_I2C1_DeInit;
  IOCtx.ReadReg     = BSP_I2C1_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C1_WriteReg16;
  IOCtx.GetTick     = BSP_GetTick;

  if(WM8994_RegisterBusIO (&WM8994Obj, &IOCtx) != WM8994_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    /* Reset the codec */
    if(WM8994_Reset(&WM8994Obj) != WM8994_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if(WM8994_ReadID(&WM8994Obj, &id) != WM8994_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if(id != WM8994_ID)
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
    else
    {
      Audio_Drv = (AUDIO_Drv_t *) &WM8994_Driver;
      Audio_CompObj = &WM8994Obj;
    }
  }
  return ret;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while (1)
  {
    /* Toggle LED3 with a period of one second */
    BSP_LED_Toggle(LED3);
    HAL_Delay(1000);
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
  * @brief  Half regular conversion complete callback. 
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &DfsdmLeftFilterHandle)
  {
    DmaLeftRecHalfBuffCplt = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&LeftRecBuff[0],sizeof(LeftRecBuff)/2);
  }
  else
  {
    DmaRightRecHalfBuffCplt = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&RightRecBuff[0],sizeof(RightRecBuff)/2);
  }
}

/**
  * @brief  Regular conversion complete callback. 
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_DFSDM_FilterGetRegularValue.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &DfsdmLeftFilterHandle)
  {
    DmaLeftRecBuffCplt = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&LeftRecBuff[1024],sizeof(LeftRecBuff)/2);
  }
  else
  {
    DmaRightRecBuffCplt = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&RightRecBuff[1024],sizeof(RightRecBuff)/2);
  }
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

