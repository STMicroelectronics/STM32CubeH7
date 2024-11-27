/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AUDIO_BUFFER_SIZE  4096
/* Private macro -------------------------------------------------------------*/
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

/* Private variables ---------------------------------------------------------*/
SAI_HandleTypeDef            haudio_out_sai;
DFSDM_Filter_HandleTypeDef   haudio_in_dfsdm_filter[2];
DFSDM_Channel_HandleTypeDef  haudio_in_dfsdm_channel[2];
AUDIO_Drv_t                  *AudioDrv;
void                         *AudioCompObj;

/*Buffer location and size should aligned to cache line size (32 bytes) */
ALIGN_32BYTES(int32_t        LeftRecBuff[AUDIO_BUFFER_SIZE/2]);
ALIGN_32BYTES(int32_t        RightRecBuff[AUDIO_BUFFER_SIZE/2]);
ALIGN_32BYTES(uint16_t       PlayBuff[AUDIO_BUFFER_SIZE]);
uint32_t                     DmaRecHalfBuffCplt  = 0;
uint32_t                     DmaRecBuffCplt      = 0;
uint32_t                     PlaybackStarted     = 0;
uint32_t                     Index;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Audio_ClockInit(void);
static void Audio_SAI_Init(void);
static void Audio_DFSDM_Init(void);
static HAL_StatusTypeDef MX_DFSDM1_Init(DFSDM_Filter_HandleTypeDef *hDfsdmFilter,\
           DFSDM_Channel_HandleTypeDef *hDfsdmChannel, MX_DFSDM_Config *MXConfig);
static int32_t WM8994_Probe(void);
static int32_t WM8994_PowerUp(void);
void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This project template calls firstly two functions in order to configure MPU feature
     and to enable the CPU Cache, respectively MPU_Config() and CPU_CACHE_Enable().
     These functions are provided as template implementation that User may integrate
     in his application, to enhance the performance in case of use of AXI interface
     with several masters. */

  /* Configure the MPU attributes as Write Through for SRAM */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  /* Add your application code here */

  /* to measure SYSCLK frequency (Divided by RCC_MCODIV_x) on MCO2 pin(PC9). */
  HAL_RCC_MCOConfig(RCC_MCO2,RCC_MCO2SOURCE_SYSCLK,RCC_MCODIV_8);

  /* Configure LED3 */
  BSP_LED_Init(LED1);

  /* Initialize audio clock for recor and play */
  Audio_ClockInit();

  /* Initialize the audio codec */
  if(WM8994_Probe() != 0)
  {
    Error_Handler();
  }
  
  /* Initialize playback */
  Audio_SAI_Init();

  /* Initialize DFSDM channels and filter for record */
  Audio_DFSDM_Init();
  
  if (WM8994_PowerUp() != 0)
  {
    Error_Handler();
  }

  /* Start DFSDM conversions */
 
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&haudio_in_dfsdm_filter[1], RightRecBuff, AUDIO_BUFFER_SIZE/2))
  {
    Error_Handler();
  }
  
  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&haudio_in_dfsdm_filter[0], LeftRecBuff, AUDIO_BUFFER_SIZE/2))
  {
    Error_Handler();
  }  
  
  /* Start loopback */
  while(1)
  {
    if(DmaRecHalfBuffCplt == 1)
    {
      SCB_InvalidateDCache_by_Addr((uint32_t *)&LeftRecBuff[0], AUDIO_BUFFER_SIZE);
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RightRecBuff[0], AUDIO_BUFFER_SIZE);
        /* Store values on Play buff */
      for(Index = 0 ; Index < AUDIO_BUFFER_SIZE/4; Index++)
      {
        PlayBuff[2*Index]     = (uint16_t)(SaturaLH((LeftRecBuff[Index] >> 8), -32768, 32767));
        PlayBuff[(2*Index)+1] = (uint16_t)(SaturaLH((RightRecBuff[Index] >> 8), -32768, 32767));
      }

      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&PlayBuff[0], AUDIO_BUFFER_SIZE);

      if(PlaybackStarted == 0)
      {
        if(HAL_OK != HAL_SAI_Transmit_DMA(&haudio_out_sai, (uint8_t *) &PlayBuff[0], AUDIO_BUFFER_SIZE))
        {
          Error_Handler();
        }
        if(0 != AudioDrv->Play(AudioCompObj))
        {
          Error_Handler();
        }
        PlaybackStarted = 1;
      }
      DmaRecHalfBuffCplt  = 0;
    }
    if(DmaRecBuffCplt == 1)
    {
      SCB_InvalidateDCache_by_Addr((uint32_t *)&LeftRecBuff[AUDIO_BUFFER_SIZE/4], AUDIO_BUFFER_SIZE);
      SCB_InvalidateDCache_by_Addr((uint32_t *)&RightRecBuff[AUDIO_BUFFER_SIZE/4], AUDIO_BUFFER_SIZE);
      /* Store values on Play buff */
      for(Index = AUDIO_BUFFER_SIZE/4 ; Index < AUDIO_BUFFER_SIZE/2; Index++)
      {
        PlayBuff[2*Index]     = (uint16_t)(SaturaLH((LeftRecBuff[Index] >> 8), -32768, 32767));
        PlayBuff[(2*Index)+1] = (uint16_t)(SaturaLH((RightRecBuff[Index] >> 8), -32768, 32767));
      }

      /* Clean Data Cache to update the content of the SRAM */
      SCB_CleanDCache_by_Addr((uint32_t*)&PlayBuff[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2);

      DmaRecBuffCplt  = 0;
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
  * @brief  SAI initialization for audio recording
  * @retval None
  */
static void Audio_DFSDM_Init(void)
{
  DFSDM_Filter_TypeDef* FilterInstnace[2] = {AUDIO_DFSDMx_MIC1_FILTER, AUDIO_DFSDMx_MIC2_FILTER};
  DFSDM_Channel_TypeDef* ChannelInstance[2] = {AUDIO_DFSDMx_MIC1_CHANNEL, AUDIO_DFSDMx_MIC2_CHANNEL};
  uint32_t DigitalMicPins[2] = {DFSDM_CHANNEL_SAME_CHANNEL_PINS, DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS};
  uint32_t DigitalMicType[2] = {DFSDM_CHANNEL_SPI_RISING, DFSDM_CHANNEL_SPI_FALLING};
  uint32_t Channel4Filter[2] = {AUDIO_DFSDMx_MIC1_CHANNEL_FOR_FILTER, AUDIO_DFSDMx_MIC2_CHANNEL_FOR_FILTER};
  MX_DFSDM_Config dfsdm_config;

  for(Index = 0; Index < 2; Index++)
  {
    dfsdm_config.FilterInstance  = FilterInstnace[Index];
    dfsdm_config.ChannelInstance = ChannelInstance[Index];
    dfsdm_config.DigitalMicPins  = DigitalMicPins[Index];
    dfsdm_config.DigitalMicType  = DigitalMicType[Index];
    dfsdm_config.Channel4Filter  = Channel4Filter[Index];
    dfsdm_config.SincOrder       = DFSDM_FILTER_SINC3_ORDER;
    dfsdm_config.Oversampling    = 64U;
    dfsdm_config.ClockDivider    = 4U;
    dfsdm_config.RightBitShift   = 0U;
    dfsdm_config.RegularTrigger  = DFSDM_FILTER_SW_TRIGGER;

    if(MX_DFSDM1_Init(&haudio_in_dfsdm_filter[Index], &haudio_in_dfsdm_channel[Index], &dfsdm_config) != HAL_OK)
    {
      Error_Handler();
    }
  }
}

/**
  * @brief  Initializes the DFSDM Audio instance
  * @param  hDfsdmFilter  DFSDM Filter Handle
  * @param  hDfsdmChannel DFSDM Channel Handle
  * @param  MXConfig      DFSDM configuration structure
  * @note   Channel output Clock Divider and Filter Oversampling are calculated as follow:
  *         - Clock_Divider = CLK(input DFSDM)/CLK(micro) with
  *           1MHZ < CLK(micro) < 3.2MHZ (TYP 2.4MHZ for MP34DT01TR)
  *         - Oversampling = CLK(input DFSDM)/(Clock_Divider * AudioFreq)
  * @retval HAL_status
  */
static HAL_StatusTypeDef MX_DFSDM1_Init(DFSDM_Filter_HandleTypeDef *hDfsdmFilter, DFSDM_Channel_HandleTypeDef *hDfsdmChannel, MX_DFSDM_Config *MXConfig)
{
  /* MIC filters  initialization */
  hDfsdmFilter->Instance                          = MXConfig->FilterInstance;
  hDfsdmFilter->Init.RegularParam.Trigger         = MXConfig->RegularTrigger;
  hDfsdmFilter->Init.RegularParam.FastMode        = ENABLE;
  hDfsdmFilter->Init.RegularParam.DmaMode         = ENABLE;
  hDfsdmFilter->Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
  hDfsdmFilter->Init.InjectedParam.ScanMode       = DISABLE;
  hDfsdmFilter->Init.InjectedParam.DmaMode        = DISABLE;
  hDfsdmFilter->Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM8_TRGO;
  hDfsdmFilter->Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES;
  hDfsdmFilter->Init.FilterParam.SincOrder        = MXConfig->SincOrder;
  hDfsdmFilter->Init.FilterParam.Oversampling     = MXConfig->Oversampling;
  hDfsdmFilter->Init.FilterParam.IntOversampling  = 1;

  if(HAL_DFSDM_FilterInit(hDfsdmFilter) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* MIC channels initialization */
  hDfsdmChannel->Instance                      = MXConfig->ChannelInstance;
  hDfsdmChannel->Init.OutputClock.Activation   = ENABLE;
  hDfsdmChannel->Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hDfsdmChannel->Init.OutputClock.Divider      = MXConfig->ClockDivider;
  hDfsdmChannel->Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hDfsdmChannel->Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
  hDfsdmChannel->Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hDfsdmChannel->Init.Awd.FilterOrder          = DFSDM_CHANNEL_SINC1_ORDER;
  hDfsdmChannel->Init.Awd.Oversampling         = 10;
  hDfsdmChannel->Init.Offset                   = 0;
  hDfsdmChannel->Init.RightBitShift            = MXConfig->RightBitShift;
  hDfsdmChannel->Init.Input.Pins               = MXConfig->DigitalMicPins;
  hDfsdmChannel->Init.SerialInterface.Type     = MXConfig->DigitalMicType;

  if(HAL_OK != HAL_DFSDM_ChannelInit(hDfsdmChannel))
  {
    return HAL_ERROR;
  }

  /* Configure injected channel */
  if(HAL_DFSDM_FilterConfigRegChannel(hDfsdmFilter, MXConfig->Channel4Filter, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}


/**
  * @brief  Audio clock initialization for play and record
  * @retval None
  */
static void Audio_ClockInit(void)
{
  RCC_PeriphCLKInitTypeDef rcc_ex_clk_init_struct;
  /* Configure PLLSAI prescalers */
  /* PLL2_VCO: 271
  SAI_CLK(first level) = PLLSAI_VCO/PLLSAIP = 271/24 = 11.291 Mhz */
  rcc_ex_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  rcc_ex_clk_init_struct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  rcc_ex_clk_init_struct.PLL2.PLL2P = 24;
  rcc_ex_clk_init_struct.PLL2.PLL2Q = 24;
  rcc_ex_clk_init_struct.PLL2.PLL2N = 271;
  rcc_ex_clk_init_struct.PLL2.PLL2R = 1;
  rcc_ex_clk_init_struct.PLL2.PLL2M = 24;
  rcc_ex_clk_init_struct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  rcc_ex_clk_init_struct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  rcc_ex_clk_init_struct.PLL2.PLL2FRACN = 0;

  if(HAL_RCCEx_PeriphCLKConfig(&rcc_ex_clk_init_struct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  SAI initialization for audio play
  * @retval None
  */
static void Audio_SAI_Init(void)
{
  /* Initialize SAI */
  __HAL_SAI_RESET_HANDLE_STATE(&haudio_out_sai);

  haudio_out_sai.Instance                    = SAI1_Block_B;
  haudio_out_sai.Init.AudioMode              = SAI_MODEMASTER_TX;
  haudio_out_sai.Init.Synchro                = SAI_ASYNCHRONOUS;
  haudio_out_sai.Init.SynchroExt             = SAI_SYNCEXT_DISABLE;
  haudio_out_sai.Init.OutputDrive            = SAI_OUTPUTDRIVE_ENABLE;
  haudio_out_sai.Init.NoDivider              = SAI_MASTERDIVIDER_ENABLE;
  haudio_out_sai.Init.FIFOThreshold          = SAI_FIFOTHRESHOLD_1QF;
  haudio_out_sai.Init.AudioFrequency         = SAI_AUDIO_FREQUENCY_44K;
  haudio_out_sai.Init.Mckdiv                 = 0;
  haudio_out_sai.Init.MonoStereoMode         = SAI_STEREOMODE;
  haudio_out_sai.Init.CompandingMode         = SAI_NOCOMPANDING;
  haudio_out_sai.Init.TriState               = SAI_OUTPUT_NOTRELEASED;
  haudio_out_sai.Init.Protocol               = SAI_FREE_PROTOCOL;
  haudio_out_sai.Init.DataSize               = SAI_DATASIZE_16;
  haudio_out_sai.Init.FirstBit               = SAI_FIRSTBIT_MSB;
  haudio_out_sai.Init.ClockStrobing          = SAI_CLOCKSTROBING_FALLINGEDGE;

  haudio_out_sai.FrameInit.FrameLength       = 32;
  haudio_out_sai.FrameInit.ActiveFrameLength = 16;
  haudio_out_sai.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  haudio_out_sai.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  haudio_out_sai.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  haudio_out_sai.SlotInit.FirstBitOffset     = 0;
  haudio_out_sai.SlotInit.SlotSize           = SAI_SLOTSIZE_DATASIZE;
  haudio_out_sai.SlotInit.SlotNumber         = 2;
  haudio_out_sai.SlotInit.SlotActive         = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

  if(HAL_OK != HAL_SAI_Init(&haudio_out_sai))
  {
    Error_Handler();
  }

  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(&haudio_out_sai);

  /* Initialize audio driver */

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
  IOCtx.Init        = BSP_I2C4_Init;
  IOCtx.DeInit      = BSP_I2C4_DeInit;
  IOCtx.ReadReg     = BSP_I2C4_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C4_WriteReg16;
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
      AudioDrv = (AUDIO_Drv_t *) &WM8994_Driver;
      AudioCompObj = &WM8994Obj;
    }
  }
  return ret;
}

/**
  * @brief  Un-reset WM8994.
  * @retval BSP status.
  */
static int32_t WM8994_PowerUp(void)
{  
   int32_t ret = BSP_ERROR_NONE;
   WM8994_Init_t codec_init;
  /* Fill codec_init structure */
  codec_init.InputDevice  = WM8994_IN_LINE1;
  codec_init.OutputDevice = WM8994_OUT_HEADPHONE;
  codec_init.Frequency    = WM8994_FREQUENCY_44K;
  codec_init.Resolution   = WM8994_RESOLUTION_16b; 
  /* Convert volume before sending to the codec */
  codec_init.Volume       = VOLUME_OUT_CONVERT(Audio_Volume);
  /* Initialize the codec internal registers */
  if(AudioDrv->Init(AudioCompObj, &codec_init) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
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
    BSP_LED_Toggle(LED1);
    HAL_Delay(1000);
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for Internal SRAM.
  * @note   The Base Address is 0x24000000
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

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_1MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  Half regular conversion complete callback.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  DmaRecHalfBuffCplt = 1;
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
  DmaRecBuffCplt = 1;
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

