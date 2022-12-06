/**
  ******************************************************************************
  * @file    audio_equalizer_app.c
  * @author  MCD Application Team
  * @brief   Audio Graphical Equalizer application functions
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
#include "audio_equalizer_app.h"

/** @addtogroup AUDIO_PLAYER_APP
  * @{
  */

/** @defgroup EQUALIZER_APPLICATION
  * @brief Audio Graphical Equalizer and Loudness application routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup EQUALIZER_APPLICATION_Private_Variables
* @{
*/
/* Graphic Equalizer */
static void *pGreqPersistentMem = NULL;
static void *pGreqScratchMem = NULL;
static  greq_static_param_t greq_static_param;
static  greq_dynamic_param_t greq_dynamic_param;
static  buffer_t BufferHandler;
static  buffer_t *pBufferHandler = &BufferHandler;

static uint8_t graphic_eq_used = 0;
static uint8_t loudness_used = 0;

static fnBkupRestoreCallback_TypeDef *pBkupRestoreParameter = NULL;
static fnBkupSaveCallback_TypeDef    *pBkupSaveParameter    = NULL;

static __IO int16_t AUDIO_loudness;
 
/* BiQuad Loudness Filter */
static const int32_t tab_coeff_Loudness[1+NB_BIQUAD_SEC_5*6] = {NB_BIQUAD_SEC_5,
                                                                0,  1201999107,  424590732,      3068307,   344502132,   -1644547, \
                                                                1,  1073741824, -2075485822,  1010128930,  1905720500, -845892267, \
                                                                1,  1073741824, -1348856775,   812477686,  1121328227, -685022681, \
                                                                1,  1073741824, -1791523805,   845106364,  1415657206, -407278556, \
                                                                2,  1134670788,           0,           0,           0,          0};
 
static void *pBIQPersistentMem = NULL;
static void *pBIQScratchMem = NULL;
 
static  biquad_df1_cascade_static_param_t  biquad_df1_cascade_static_param;
static  biquad_df1_cascade_dynamic_param_t biquad_df1_cascade_dynamic_param;

static osSemaphoreId  AudioEqSemaphore = 0;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup EQUALIZER_APPLICATION_Private_Functions
* @{
*/
/**
  * @brief  Allocate Memory for Audio Equalizer
  * @param  size: memory size to allocate.
  * @retval Audio state.
  */
static void * EQ_MEM_Malloc(size_t size)
{
#ifdef AUDIO_EQ_USE_RTOS
  return pvPortMalloc(size);
#else /* !AUDIO_EQ_USE_RTOS */
  return malloc(size);
#endif /* AUDIO_EQ_USE_RTOS */
}

/**
  * @brief  Free Allocate Audio Equalizer Memory
  * @param  pMemToFree: pointer to allocated memory.
  * @retval None.
  */
static void EQ_MEM_Free( void *pMemToFree ) 
{
#ifdef AUDIO_EQ_USE_RTOS
  vPortFree(pMemToFree);
#else /* !AUDIO_EQ_USE_RTOS */
  free(pMemToFree);
#endif /* AUDIO_EQ_USE_RTOS */
}

/**
  * @brief  Fill Loudness Dynamic Configuration parameters
  * @param  biquad_df1_cascade_dyn: pointer to dynamic BIQ filtering parameters data structure.
  * @param  BiqEn: Enable/Disable BIQ filtering.
  * @retval None.
  */
static void EQ_FilterLoudnessConfig(biquad_df1_cascade_dynamic_param_t *biquad_df1_cascade_dyn, uint8_t BiqEn)
{
  uint8_t k =0;
  /* content: nb_sos, post-shift, b0, b1, a0, a1, a2*/
  int32_t *tab_coeff = (int32_t*)&tab_coeff_Loudness[1];
 
  /* Update some dynamic parameters values*/
  biquad_df1_cascade_dyn->enable = BiqEn;
  biquad_df1_cascade_dyn->nb_sos = tab_coeff_Loudness[0];   
  for (k=0 ; k<biquad_df1_cascade_dyn->nb_sos ; k++)
  {
    biquad_df1_cascade_dyn->biquad_coeff[k*6]   =*tab_coeff++; /*post-chift*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+1] =*tab_coeff++; /*b0*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+2] =*tab_coeff++; /*b1*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+3] =*tab_coeff++; /*a0*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+4] =*tab_coeff++; /*a1*/
    biquad_df1_cascade_dyn->biquad_coeff[k*6+5] =*tab_coeff++; /*a2*/
  }    
}

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup EQUALIZER_APPLICATION_Exported_Functions
  * @{
  */
/**
  * @brief  Initializes audio graphical equalizer
  * @param  None.
  * @retval Audio error state.
  */
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_Init(uint8_t use_graphic_eq, uint8_t use_loudness,
                                        fnBkupRestoreCallback_TypeDef*  pBkupRestoreCallback,
                                        fnBkupSaveCallback_TypeDef*     pBkupSaveCallback)
{
  int32_t error = GREQ_ERROR_NONE;
  int32_t *tab_coeff;

  if((use_graphic_eq) || (use_loudness))
  {
    /* Create Semaphore lock */
    osSemaphoreDef(AUDIO_EQ_SEM);
    /* Create the Semaphore used by the Audio GrEq processing */
    AudioEqSemaphore = osSemaphoreCreate(osSemaphore(AUDIO_EQ_SEM), 1);
    if(!AudioEqSemaphore)
    {
      return (AUDIO_EQUALIZER_ERROR_MEM);
    }

    /* Enables and resets CRC-32 to Unlock Audio Add-ons */
    __HAL_RCC_CRC_CLK_ENABLE();
    CRC->CR = CRC_CR_RESET;
  }

  pBkupRestoreParameter = pBkupRestoreCallback;
  pBkupSaveParameter    = pBkupSaveCallback;
  
  if(use_graphic_eq)
  {
    /* Effect Initialization and configuration */
    /*-----------------------------------------*/
    /* Allocate mem for GrEq */
    pGreqPersistentMem = EQ_MEM_Malloc(greq_persistent_mem_size); /* greq_persistent_mem_size  0x224 */
    pGreqScratchMem = EQ_MEM_Malloc(greq_scratch_mem_size);       /* greq_scratch_mem_size  0xF00 */
 
    if((pGreqPersistentMem == NULL) || (pGreqScratchMem == NULL))
    {
      return (AUDIO_EQUALIZER_ERROR_MEM);
    }

    /* GREQ effect reset */
    error = greq_reset(pGreqPersistentMem, pGreqScratchMem);
    if (error != GREQ_ERROR_NONE)
    {
      return (AUDIO_EQUALIZER_ERROR_ALGO);
    }
 
    /* GREQ effect static parameters setting */
#ifdef EQ_HIGH_QUALITY
    greq_static_param.nb_bands = GREQ_NB_BANDS_10;  /* 10-bands equalizer */
#else
    greq_static_param.nb_bands = GREQ_NB_BANDS_5;  /* 5-bands equalizer */
#endif
    error = greq_setParam(&greq_static_param, pGreqPersistentMem);
    if (error != GREQ_ERROR_NONE)
    {
      return (AUDIO_EQUALIZER_ERROR_ALGO);
    }
 
    /* GREQ dynamic parameters that can be updated here every frame if required */
    if(pBkupRestoreParameter)
    {
      greq_dynamic_param.enable = 0;            /* Enables the effect */
      greq_dynamic_param.user_gain_per_band_dB[0] = pBkupRestoreParameter(AUDIO_PLAYER_EQU1_BKP);  /* Gain for band 1  */
      greq_dynamic_param.user_gain_per_band_dB[1] = pBkupRestoreParameter(AUDIO_PLAYER_EQU2_BKP);  /* Gain for band 2  */
      greq_dynamic_param.user_gain_per_band_dB[2] = pBkupRestoreParameter(AUDIO_PLAYER_EQU3_BKP);  /* Gain for band 3  */
      greq_dynamic_param.user_gain_per_band_dB[3] = pBkupRestoreParameter(AUDIO_PLAYER_EQU4_BKP);  /* Gain for band 4  */
      greq_dynamic_param.user_gain_per_band_dB[4] = pBkupRestoreParameter(AUDIO_PLAYER_EQU5_BKP);  /* Gain for band 5  */
#ifdef EQ_HIGH_QUALITY
      greq_dynamic_param.user_gain_per_band_dB[5] = pBkupRestoreParameter(AUDIO_PLAYER_EQU6_BKP);  /* Gain for band 6  */
      greq_dynamic_param.user_gain_per_band_dB[6] = pBkupRestoreParameter(AUDIO_PLAYER_EQU7_BKP);  /* Gain for band 7  */
      greq_dynamic_param.user_gain_per_band_dB[7] = pBkupRestoreParameter(AUDIO_PLAYER_EQU8_BKP);  /* Gain for band 8  */
      greq_dynamic_param.user_gain_per_band_dB[8] = pBkupRestoreParameter(AUDIO_PLAYER_EQU9_BKP);  /* Gain for band 9  */
      greq_dynamic_param.user_gain_per_band_dB[9] = pBkupRestoreParameter(AUDIO_PLAYER_EQU10_BKP); /* Gain for band 10 */
#endif
    }

    greq_dynamic_param.gain_preset_idx = GREQ_NO_PRESET;         /* User preset defined above */
    error = greq_setConfig(&greq_dynamic_param, pGreqPersistentMem);
    if (error != GREQ_ERROR_NONE)
    {
        return (AUDIO_EQUALIZER_ERROR_ALGO);
    }
  }
 
  if(use_loudness)
  {
    int32_t k=0;

    /* Allocate mem for Biq */
    pBIQPersistentMem = EQ_MEM_Malloc(biquad_df1_cascade_persistent_mem_size); /* biquad_df1_cascade_persistent_mem_size  0x025C */ 
    pBIQScratchMem = EQ_MEM_Malloc(biquad_df1_cascade_scratch_mem_size);       /* biquad_df1_cascade_scratch_mem_size  0x04 */

    if((pBIQPersistentMem == NULL) || (pBIQScratchMem == NULL))
    {
      return (AUDIO_EQUALIZER_ERROR_MEM);
    }

    /* BIQ effect reset */
    error = biquad_df1_cascade_reset(pBIQPersistentMem, pBIQScratchMem);
    if (error != GREQ_ERROR_NONE)
    {
      return (AUDIO_EQUALIZER_ERROR_ALGO);
    }

    if(pBkupRestoreParameter)
    {
      AUDIO_loudness = pBkupRestoreParameter(AUDIO_PLAYER_LOUD_BKP);
    }
 
    /* BIQ effect static parameters setting */
    biquad_df1_cascade_static_param.nb_sos =tab_coeff_Loudness[0]; /* by default set to:NB_BIQUAD_SEC_5 [5-second order sections] */
    tab_coeff = (int32_t*)&tab_coeff_Loudness[1];
    for (k=0 ; k<biquad_df1_cascade_static_param.nb_sos ; k++)
    {
      biquad_df1_cascade_static_param.biquad_coeff[k*6]   =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+1] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+2] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+3] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+4] =*tab_coeff++;
      biquad_df1_cascade_static_param.biquad_coeff[k*6+5] =*tab_coeff++;                                
    }
    error = biquad_df1_cascade_setParam(&biquad_df1_cascade_static_param, pBIQPersistentMem);
    if (error != GREQ_ERROR_NONE)
    {
      return (AUDIO_EQUALIZER_ERROR_ALGO);
    }
 
    /* BIQ dynamic parameters that can be updated here every frame if required */
    EQ_FilterLoudnessConfig(&biquad_df1_cascade_dynamic_param, 1); /* Enable the effect */
    error = biquad_df1_cascade_setConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem);
    if (error != GREQ_ERROR_NONE)
    {
      return (AUDIO_EQUALIZER_ERROR_ALGO);
    }
  }

  graphic_eq_used = use_graphic_eq;
  loudness_used   = use_loudness;

  return AUDIO_EQUALIZER_ERROR_NONE;
}

/**
  * @brief  Terminates audio graphical equalizer
  * @param  None.
  * @retval Audio error state.
  */
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_DeInit(void)
{
  if(graphic_eq_used)
  {
    if(pBkupSaveParameter)
    {
      pBkupSaveParameter(AUDIO_PLAYER_EQU1_BKP,  greq_dynamic_param.user_gain_per_band_dB[0]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU2_BKP,  greq_dynamic_param.user_gain_per_band_dB[1]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU3_BKP,  greq_dynamic_param.user_gain_per_band_dB[2]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU4_BKP,  greq_dynamic_param.user_gain_per_band_dB[3]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU5_BKP,  greq_dynamic_param.user_gain_per_band_dB[4]);
#ifdef EQ_HIGH_QUALITY
      pBkupSaveParameter(AUDIO_PLAYER_EQU6_BKP,  greq_dynamic_param.user_gain_per_band_dB[5]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU7_BKP,  greq_dynamic_param.user_gain_per_band_dB[6]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU8_BKP,  greq_dynamic_param.user_gain_per_band_dB[7]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU9_BKP,  greq_dynamic_param.user_gain_per_band_dB[8]);
      pBkupSaveParameter(AUDIO_PLAYER_EQU10_BKP, greq_dynamic_param.user_gain_per_band_dB[9]);
#endif
    }

    /* Enable The Graphic Equalizer */
    greq_getConfig(&greq_dynamic_param, pGreqPersistentMem);
    greq_dynamic_param.enable = 0;            /* Disable the effect */
    if (greq_setConfig(&greq_dynamic_param, pGreqPersistentMem) != GREQ_ERROR_NONE)
    {
      return AUDIO_EQUALIZER_ERROR_ALGO;
    }
 
    /* Free mem used by GrEq */
    if(pGreqPersistentMem)
    {
      EQ_MEM_Free(pGreqPersistentMem);
    }
 
    if(pGreqScratchMem)
    {
      EQ_MEM_Free(pGreqScratchMem);
    }
  }
 
  if(loudness_used)
  {
    if(pBkupSaveParameter)
    {
      pBkupSaveParameter(AUDIO_PLAYER_LOUD_BKP, AUDIO_loudness);
    }

    /* Disable Loudness filtering */
    biquad_df1_cascade_dynamic_param.enable = 0;            /* Disable the effect */
    if (biquad_df1_cascade_setConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem) != BIQ_ERROR_NONE)
    {
        return AUDIO_EQUALIZER_ERROR_ALGO;
    }
 
    /* Free mem used by Biq */
    if(pBIQPersistentMem)
    {
      EQ_MEM_Free(pBIQPersistentMem);
    }
 
    if(pBIQScratchMem)
    {
      EQ_MEM_Free(pBIQScratchMem);
    }
  }

  pBkupSaveParameter    = NULL;
  pBkupRestoreParameter = NULL;

  if(AudioEqSemaphore != 0)
  {
    osSemaphoreDelete(AudioEqSemaphore);
    AudioEqSemaphore = 0;
  }

  graphic_eq_used = 0;
  loudness_used   = 0;

  return AUDIO_EQUALIZER_ERROR_NONE;
}

/**
  * @brief  Terminates audio graphical equalizer
  * @param  None.
  * @retval Audio error state.
  */
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_SetupBuffer(buffer_t* pBufferSetup)
{
  if((graphic_eq_used) || (loudness_used))
  {
    if(pBufferSetup)
    {
      /* I/O buffers settings */
      BufferHandler.nb_bytes_per_Sample = pBufferSetup->nb_bytes_per_Sample;
      BufferHandler.nb_channels         = pBufferSetup->nb_channels;
      BufferHandler.buffer_size         = pBufferSetup->buffer_size;
      BufferHandler.mode                = INTERLEAVED;
    }
    else
    {
      return AUDIO_EQUALIZER_ERROR_ALGO;
    }
  }

  return AUDIO_EQUALIZER_ERROR_NONE;
}

/**
  * @brief  This function Set the new gain of the equilizer
  * @param  BandNum : equilizer band index
  * @param  NewGainValue : the new band gain -12 dB to 12 dB.
  * @retval None.
*/
AUDIO_EQUALIZER_ErrorTypDef AUDIOEQ_SetEqGain(uint8_t BandNum, int16_t NewGainValue)
{
  AUDIO_EQUALIZER_ErrorTypDef ErrorCode = AUDIO_EQUALIZER_ERROR_NONE;

  if(graphic_eq_used)
  {
    if((NewGainValue < -12) || (NewGainValue > 12))
    {
      ErrorCode = AUDIO_EQUALIZER_ERROR_INVALID_VALUE;
    }
    else
    {
      /* Lock access */
      osSemaphoreWait(AudioEqSemaphore, osWaitForever);

      greq_dynamic_param.enable = 1; /* Enable the effect */
      greq_dynamic_param.user_gain_per_band_dB[BandNum] = NewGainValue;
      if (greq_setConfig(&greq_dynamic_param, pGreqPersistentMem) != GREQ_ERROR_NONE)
      {
        ErrorCode = AUDIO_EQUALIZER_ERROR_ALGO;
      }

      /* Release the Lock */
      osSemaphoreRelease(AudioEqSemaphore);
    }
  }

  return ErrorCode;
}
 
/**
  * @brief  This function return the gain of the equilizer
  * @param  BandNum : equilizer band index
  * @retval band gain -12 dB to +12 dB.
*/
int16_t AUDIOEQ_GetEqGain(uint8_t BandNum)
{
  return (greq_dynamic_param.user_gain_per_band_dB[BandNum]);
}

/**
  * @brief  This function return the loudness of the equilizer
  * @retval band gain -12 dB to +12 dB.
*/
int16_t AUDIOEQ_GetLoudness(void)
{
  return (AUDIO_loudness);
}

/**
  * @brief  This function Set Loudness Control Gain Value.
  * @param  NewLoudnessValue -12 dB to +12 dB.
  * @retval None.
  */
AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_SetLoudness(int16_t NewLoudnessValue)
{
  AUDIO_EQUALIZER_ErrorTypDef ErrorCode = AUDIO_EQUALIZER_ERROR_NONE;

  if(loudness_used)
  {
    if((NewLoudnessValue < -12) || (NewLoudnessValue > 12))
    {
      ErrorCode = AUDIO_EQUALIZER_ERROR_INVALID_VALUE;
    }
    else
    {
      /* Lock access */
      osSemaphoreWait(AudioEqSemaphore, osWaitForever);

      /* Update BIQ dynamic parameters for the next frame */
      EQ_FilterLoudnessConfig(&biquad_df1_cascade_dynamic_param, ((NewLoudnessValue == 0) ? 0 : 1)); /* Enable the effect */

      if (biquad_df1_cascade_setConfig(&biquad_df1_cascade_dynamic_param, pBIQPersistentMem) != BIQ_ERROR_NONE)
      {
        ErrorCode = AUDIO_EQUALIZER_ERROR_ALGO;
      }
      else
      {
        AUDIO_loudness = NewLoudnessValue;
      }

      /* Release the Lock */
      osSemaphoreRelease(AudioEqSemaphore);
    }
  }

  return ErrorCode;
}

AUDIO_EQUALIZER_ErrorTypDef  AUDIOEQ_Apply(uint8_t* pBuff, uint32_t Size)
{
  AUDIO_EQUALIZER_ErrorTypDef ErrorCode = AUDIO_EQUALIZER_ERROR_NONE;

  /* Lock access */
  osSemaphoreWait(AudioEqSemaphore, osWaitForever);

  if((graphic_eq_used) || (loudness_used))
  {
    pBufferHandler->data_ptr    = pBuff;
    pBufferHandler->buffer_size = Size/(pBufferHandler->nb_bytes_per_Sample * pBufferHandler->nb_channels); /* just half buffer is process (size per channel) */
  }

  if(graphic_eq_used)
  {
    /* Apply GrEq */
    if (greq_process(pBufferHandler, pBufferHandler, pGreqPersistentMem) != GREQ_ERROR_NONE)
    {
      ErrorCode = AUDIO_EQUALIZER_ERROR_ALGO;
    }
  }

  if(loudness_used)
  {
    /* Apply Loudness settings */
    if (biquad_df1_cascade_process(pBufferHandler, pBufferHandler, pBIQPersistentMem) != BIQ_ERROR_NONE)
    {
      ErrorCode = AUDIO_EQUALIZER_ERROR_ALGO;
    }
  }

  /* Release the Lock */
  osSemaphoreRelease(AudioEqSemaphore);

  return ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

