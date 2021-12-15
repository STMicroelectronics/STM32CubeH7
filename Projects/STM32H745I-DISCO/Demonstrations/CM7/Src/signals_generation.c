/**
  ******************************************************************************
  * @file    Demonstration/STM32H745-Discovery_Demo/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This module shows how  to use the DAC
  *          peripheral to generate several signals.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main_oscillo_cm7.h"

/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SINE_RES        128                                  // Sine Waveform resolution
#define ESCALATOR_RES   12                                  // Waveform resolution
#define TRINAGLE_RES    120                                 // Waveform resolution
#define SQUARE_RES      2
#define DC_RES          1
#define NOISE_RES       1    
#define INPUT_FREQUENCY 200000000                             // TIM6 counter clock (prescaled APB1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef    DacHandle;
static DAC_ChannelConfTypeDef sConfig;

/* Escalator wave */
const uint16_t EscalatorWaveRef[ESCALATOR_RES] = {
  0x0, 0x333, 0x666, 0x999, 0xCCC, 0xFFF, 0xCCC, 0x999, 0x666, 0x333, 0x0
};

/* Sine wave */
const uint16_t SineWaveRef[SINE_RES] = { 
  2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897, 
  2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640, 
  3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002, 
  4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894, 
  3858, 3817, 3772, 3722, 3669, 3611, 3550, 3485, 3416, 3344, 
  3269, 3191, 3110, 3027, 2941, 2853, 2763, 2671, 2578, 2483, 
  2387, 2291, 2194, 2096, 1999, 1901, 1804, 1708, 1612, 1517, 
  1424, 1332, 1242, 1154, 1068, 985, 904, 826, 751, 679, 
  610, 545, 484, 426, 373, 323, 278, 237, 201, 169, 
  141, 118, 100, 87, 79, 75, 76, 82, 93, 109, 
  129, 154, 184, 218, 257, 300, 347, 399, 455, 514, 
  577, 644, 714, 788, 865, 944, 1026, 1111, 1198, 1287, 
  1378, 1471, 1565, 1660, 1756, 1853, 1950, 2047
};   

/* Triangular wave */
const uint16_t TriangularWaveRef[TRINAGLE_RES] = {
  0x044, 0x088, 0x0cc, 0x110, 0x154, 0x198, 0x1dc, 0x220, 0x264, 0x2a8,
  0x2ec, 0x330, 0x374, 0x3b8, 0x3fc, 0x440, 0x484, 0x4c8, 0x50c, 0x550,
  0x594, 0x5d8, 0x61c, 0x660, 0x6a4, 0x6e8, 0x72c, 0x770, 0x7b4, 0x7f8,
  0x83c, 0x880, 0x8c4, 0x908, 0x94c, 0x990, 0x9d4, 0xa18, 0xa5c, 0xaa0,
  0xae4, 0xb28, 0xb6c, 0xbb0, 0xbf4, 0xc38, 0xc7c, 0xcc0, 0xd04, 0xd48,
  0xd8c, 0xdd0, 0xe14, 0xe58, 0xe9c, 0xee0, 0xf24, 0xf68, 0xfac, 0xff0,
  0xfac, 0xf68, 0xf24, 0xee0, 0xe9c, 0xe58, 0xe14, 0xdd0, 0xd8c, 0xd48,
  0xd04, 0xcc0, 0xc7c, 0xc38, 0xbf4, 0xbb0, 0xb6c, 0xb28, 0xae4, 0xaa0,
  0xa5c, 0xa18, 0x9d4, 0x990, 0x94c, 0x908, 0x8c4, 0x880, 0x83c, 0x7f8,
  0x7b4, 0x770, 0x72c, 0x6e8, 0x6a4, 0x660, 0x61c, 0x5d8, 0x594, 0x550,
  0x50c, 0x4c8, 0x484, 0x440, 0x3fc, 0x3b8, 0x374, 0x330, 0x2ec, 0x2a8,
  0x264, 0x220, 0x1dc, 0x198, 0x154, 0x110, 0x0cc, 0x088, 0x044, 0x000
};

/* Square wave */
const uint16_t SquareWaveRef[SQUARE_RES] = { 
  0x00, 0xFFF
};

/* DC wave */
const uint16_t DC_WaveRef[DC_RES] = { 
  0xFFE
};

/* NOISE wave */
const uint16_t NoiseWaveRef[NOISE_RES] = { 
  0xFFF
};

ALIGN_32BYTES (uint16_t NoiseWave[NOISE_RES]);
ALIGN_32BYTES (uint16_t TriangularWave[TRINAGLE_RES]);
ALIGN_32BYTES (uint16_t DC_Wave[DC_RES]);
ALIGN_32BYTES (uint16_t SquareWave[SQUARE_RES]);
ALIGN_32BYTES (uint16_t SineWave[SINE_RES]);
ALIGN_32BYTES (uint16_t EscalatorWave[ESCALATOR_RES]);

/* Private function prototypes -----------------------------------------------*/
static uint32_t DAC_NoiseWaveConfig(uint16_t outputAmplitude);
static uint32_t DAC_SquareWaveConfig(uint16_t outputAmplitude);
static uint32_t DAC_DCWaveConfig(uint16_t outputAmplitude);
static uint32_t DAC_EscalatorWaveConfig(uint16_t outputAmplitude);
static void TIM6_Config(uint32_t resolution, uint32_t frequency);
static uint32_t DAC_SineWaveConfig(uint16_t outputAmplitude);
static uint32_t DAC_TrianglarWaveConfig(uint16_t outputAmplitude);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Generate different waves.
  * @param  None
  * @retval None
  */
uint32_t SG_GenerateWave(SG_WaveButtonIndex_e waveButtonIndex, uint32_t waveFrequency, uint16_t outputAmplitude)
{
  uint32_t ret = DEMO_OK;
  
  /*##-1- Configure the DAC peripheral */
  DacHandle.Instance = DACx;
  HAL_DAC_DeInit(&DacHandle);
  
  switch (waveButtonIndex)
  {
    case SG_SQUARE_WAVE_BUTTON_INDEX:
      TIM6_Config(SQUARE_RES, waveFrequency);
      ret = DAC_SquareWaveConfig(outputAmplitude);
      break;
    case SG_SINE_WAVE_BUTTON_INDEX:
      TIM6_Config(SINE_RES, waveFrequency);
      ret = DAC_SineWaveConfig(outputAmplitude);
      break;
    case SG_TRIANGLE_WAVE_BUTTON_INDEX:
      TIM6_Config(TRINAGLE_RES, waveFrequency);
      ret = DAC_TrianglarWaveConfig(outputAmplitude);
      break;
    case SG_NOISE_WAVE_BUTTON_INDEX:
      TIM6_Config(11, waveFrequency);
      DAC_NoiseWaveConfig(outputAmplitude);
      break;
    case SG_ESCALATOR_WAVE_BUTTON_INDEX:
      TIM6_Config(ESCALATOR_RES, waveFrequency);
      ret = DAC_EscalatorWaveConfig(outputAmplitude);
      break;
    case SG_DC_WAVE_BUTTON_INDEX:
      TIM6_Config(DC_RES, waveFrequency);
      ret = DAC_DCWaveConfig(outputAmplitude);
      break;
    default:
    	break;
  }
  
  return ret;
}

/**
  * @brief  Stop wave generation.
  * @param  None
  * @retval None
  */
uint32_t SG_StopWaveGeneration(void)
{
  uint32_t ret = DEMO_OK;
  
  if (HAL_DAC_Stop(&DacHandle, DACx_CHANNEL) != HAL_OK)
  {
    /* Start DMA Error */
    ret = DEMO_ERROR;
  }
  
  return ret;
}

/**
  * @brief  DAC Channel1 noise wave Configuration
  * @param  None
  * @retval None
  */
static uint32_t DAC_NoiseWaveConfig(uint16_t outputAmplitude)
{
  uint32_t i;

  /* Calculate new amplitude values */
  for(i = 0; i < NOISE_RES; i++)
  {
    NoiseWave[i] = ((NoiseWaveRef[i]*outputAmplitude)/DAC_MAX_AMPLITUDE);
  }
  
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)NoiseWave, NOISE_RES*2);
  
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    return DEMO_ERROR;
  }

  /* DAC channel1 Configuration */
  sConfig.DAC_Trigger = DACx_TRIGGER;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  /* Start DAC Channel1 */
  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    return DEMO_ERROR;
  }
  
  /* Set DAC  DHR register */
  HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL, DAC_ALIGN_12B_R, NoiseWave[NOISE_RES]);
  
  /* Enable DAC Channel1 and channel 2 */
  if (HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    return DEMO_ERROR;
  }
  /* Activate the noise generation */
  HAL_DACEx_NoiseWaveGenerate(&DacHandle, DACx_CHANNEL, DAC_LFSRUNMASK_BITS11_0);
  
  return DEMO_OK;
}

/**
  * @brief  DAC Channel1 DC wave Configuration
  * @param  None
  * @retval None
  */
static uint32_t DAC_DCWaveConfig(uint16_t outputAmplitude)
{
  uint32_t i;

  /* Calculate new amplitude values */
  for(i = 0; i < DC_RES; i++)
  {
    DC_Wave[i] = ((DC_WaveRef[i]*outputAmplitude)/DAC_MAX_AMPLITUDE);
  }
  
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)DC_Wave, DC_RES*2);
  
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    return DEMO_ERROR;
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  sConfig.DAC_Trigger = DACx_TRIGGER;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    return DEMO_ERROR;
  }

  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)DC_Wave, DC_RES, DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    return DEMO_ERROR;
  }
  
  return DEMO_OK;
}

/**
  * @brief  DAC Channel1 square wave Configuration
  * @param  None
  * @retval None
  */
static uint32_t DAC_SquareWaveConfig(uint16_t outputAmplitude)
{
  uint32_t i;

  /* Calculate new amplitude values */
  for(i = 0; i < SQUARE_RES; i++)
  {
    SquareWave[i] = ((SquareWaveRef[i]*outputAmplitude)/DAC_MAX_AMPLITUDE);
  }
  
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)SquareWave, SQUARE_RES*2);
  
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)SquareWave, SQUARE_RES*2);
  
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    return DEMO_ERROR;
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  sConfig.DAC_Trigger = DACx_TRIGGER;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    return DEMO_ERROR;
  }  
  
  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)SquareWave, SQUARE_RES, DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    return DEMO_ERROR;
  }
  
  return DEMO_OK;
}

/**
  * @brief  DAC Channel1 escalator wave Configuration
  * @param  None
  * @retval None
  */
static uint32_t DAC_EscalatorWaveConfig(uint16_t outputAmplitude)
{
  uint32_t i;

  /* Calculate new amplitude values */
  for(i = 0; i < ESCALATOR_RES; i++)
  {
    EscalatorWave[i] = ((EscalatorWaveRef[i]*outputAmplitude)/DAC_MAX_AMPLITUDE);
  }
  
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)EscalatorWave, ESCALATOR_RES*2);
  
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    return DEMO_ERROR;
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  sConfig.DAC_Trigger = DACx_TRIGGER;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    return DEMO_ERROR;
  }

  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)EscalatorWave, ESCALATOR_RES, DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    return DEMO_ERROR;
  }
  
  return DEMO_OK;
}

/**
  * @brief  DAC Channel1 sine wave Configuration
  * @param  None
  * @retval None
  */
static uint32_t DAC_SineWaveConfig(uint16_t outputAmplitude)
{
  uint32_t i;

  /* Calculate new amplitude values */
  for(i = 0; i < SINE_RES; i++)
  {
    SineWave[i] = ((SineWaveRef[i]*outputAmplitude)/DAC_MAX_AMPLITUDE);
  }
  
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)SineWave, SINE_RES*2);
  
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    return DEMO_ERROR;
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  sConfig.DAC_Trigger = DACx_TRIGGER;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    return DEMO_ERROR;
  }

  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)SineWave, SINE_RES, DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    return DEMO_ERROR;
  }
  
  return DEMO_OK;
}

/**
  * @brief  DAC Channel1 Trianglar wave Configuration
  * @param  None
  * @retval None
  */
static uint32_t DAC_TrianglarWaveConfig(uint16_t outputAmplitude)
{
  uint32_t i;

  /* Calculate new amplitude values */
  for(i = 0; i < TRINAGLE_RES; i++)
  {
    TriangularWave[i] = ((TriangularWaveRef[i]*outputAmplitude)/DAC_MAX_AMPLITUDE);
  }
  
  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)TriangularWave, TRINAGLE_RES*2);
  
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    return DEMO_ERROR;
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  sConfig.DAC_Trigger = DACx_TRIGGER;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    return DEMO_ERROR;
  }

  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)TriangularWave, TRINAGLE_RES, DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    return DEMO_ERROR;
  }
  
  return DEMO_OK;
}

/**
  * @brief  TIM6 Configuration
  * @note   TIM6 configuration is based on APB1 frequency
  * @note   TIM6 Update event occurs each TIM6CLK/256
  * @param  None
  * @retval None
  */
static void TIM6_Config(uint32_t resolution, uint32_t frequency)
{
  static TIM_HandleTypeDef  htim;
  TIM_MasterConfigTypeDef sMasterConfig;
  uint16_t periode;
  
  periode = (INPUT_FREQUENCY/(resolution * frequency));
  
  /* Configure the TIM peripheral */
  htim.Instance               = TIMx;
  htim.Init.Period            = periode;
  htim.Init.Prescaler         = 0;
  htim.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.RepetitionCounter = 0;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim);

  /* TIM6 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);
}

/**
  * @}
  */

/**
  * @}
  */

