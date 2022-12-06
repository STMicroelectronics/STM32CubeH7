/**
  ******************************************************************************
  * @file    audioplayer_app.c
  * @author  MCD Application Team
  * @brief   Audio player application functions
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
#include "audio_player_app.h"
#include "rtc_app.h"
#include "Middlewares_Addons/SpiritDSP_Equalizer/spiritEQ.h"
#include "Middlewares_Addons/SpiritDSP_LoudnessControl/spiritLdCtrl.h"
#include "cmsis_os.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FIL wav_file;
static osMessageQId   AudioEvent = 0;
static osThreadId     AudioThreadId = 0;
extern SAI_HandleTypeDef         haudio_out_sai;

#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
long  AUDIO_EqInstance[SPIRIT_EQ_PERSIST_SIZE_IN_BYTES/4]  ;
TSpiritEQ_Band AUDIO_EQ_Bands[SPIRIT_EQ_MAX_BANDS];
__IO int16_t AUDIO_loudness;
__IO uint32_t AUDIO_LdCtrlPersistance[SPIRIT_LDCTRL_PERSIST_SIZE_IN_BYTES/4];
__IO uint32_t AUDIO_LdCtrlScratch[SPIRIT_LDCTRL_SCRATCH_SIZE_IN_BYTES/4];
TSpiritLdCtrl_Prms AUDIO_LdCtrlInstanceParams;
TSpiritEQ_Band *tmpEqBand;

#define CALC_LDNS_DB(x)   (1 << (SPIRIT_LDCTRL_GAIN_Q_BITS - x ))  

#endif

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);



/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);

#ifdef __GNUC__
#define PRAGMA_AUDIO_BUFFERS_LOCATION
#define ATTRIBUTE_AUDIO_BUFFERS_LOCATION __attribute__ ((section ("audio_buffers"))) __attribute__ ((aligned(4)))
#elif defined __ICCARM__
#define PRAGMA_AUDIO_BUFFERS_LOCATION _Pragma("location=\"audio_buffers\"")
#define ATTRIBUTE_AUDIO_BUFFERS_LOCATION
#elif defined(__ARMCC_VERSION)
#define PRAGMA_AUDIO_BUFFERS_LOCATION
#define ATTRIBUTE_AUDIO_BUFFERS_LOCATION __attribute__ ((section ("audio_buffers"))) __attribute__ ((aligned(4)))
#endif

PRAGMA_AUDIO_BUFFERS_LOCATION AUDIO_ProcessTypdef haudio ATTRIBUTE_AUDIO_BUFFERS_LOCATION;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume)
{
#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
  uint32_t index = 0;
  __IO uint32_t ldness_value;
#endif  
BSP_AUDIO_Init_t AudioPlayInit;
  AudioPlayInit.Device = AUDIO_OUT_DEVICE_SPK_HP;
  AudioPlayInit.ChannelsNbr = 2;
  AudioPlayInit.SampleRate = I2S_AUDIOFREQ_48K;
  AudioPlayInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioPlayInit.Volume = volume;
   /* Try to Init Audio interface in different config in case of failure */
  BSP_AUDIO_OUT_Init (0, &AudioPlayInit);


  /* Initialize internal audio structure */
  haudio.state  = AUDIOPLAYER_STOP;
  haudio.mute   = MUTE_OFF;
  haudio.volume = volume;
  haudio.freq   = I2S_AUDIOFREQ_48K;

#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
  
    /* Enable CRC to Unlock Audio add-ons */
  __HAL_RCC_CRC_CLK_ENABLE();
  
  /* Retrieve stored settings*/
  AUDIO_EQ_Bands[0].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU1_BKP);
  AUDIO_EQ_Bands[1].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU2_BKP);    
  AUDIO_EQ_Bands[2].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU3_BKP);   
  AUDIO_EQ_Bands[3].gainDb = RTC_BkupRestoreParameter(AUDIOPLAYER_EQU4_BKP);  
  AUDIO_loudness = RTC_BkupRestoreParameter(AUDIOPLAYER_LOUD_BKP);
  
  /* Enable the Eq */
  SpiritEQ_Init(AUDIO_EqInstance, I2S_AUDIOFREQ_48K);
    
  /* set band params */
  SET_BAND_PRMS(&AUDIO_EQ_Bands[0], SPIRIT_EQ_FLT_TYPE_SHELVING_LOWPASS , 100, 100);
  SET_BAND_PRMS(&AUDIO_EQ_Bands[1], SPIRIT_EQ_FLT_TYPE_SHELVING_LOWPASS , 800, 200);
  SET_BAND_PRMS(&AUDIO_EQ_Bands[2], SPIRIT_EQ_FLT_TYPE_SHELVING_HIPASS , 8000, 2000);
  SET_BAND_PRMS(&AUDIO_EQ_Bands[3], SPIRIT_EQ_FLT_TYPE_SHELVING_HIPASS , 15000, 2000);

  for (index = 0; index < SPIRIT_EQ_MAX_BANDS ; index++)
  {
    if((AUDIO_EQ_Bands[index].gainDb < -12) && (AUDIO_EQ_Bands[index].gainDb > 12))
    {
      AUDIO_EQ_Bands[index].gainDb = 0;
    }
    tmpEqBand = &AUDIO_EQ_Bands[index];
    SpiritEQ_FltSet((TSpiritEq *)AUDIO_EqInstance, tmpEqBand, index);
  }
  
  /* Enable Loundness Control */
  if((AUDIO_loudness < -12) && (AUDIO_loudness > 12))
  {
    AUDIO_loudness= 0;
  }
  
  SpiritLdCtrl_Init((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, I2S_AUDIOFREQ_48K);
  SpiritLdCtrl_GetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
  AUDIO_LdCtrlInstanceParams.gainQ8 = CALC_LDNS_DB(AUDIO_loudness);
  SpiritLdCtrl_SetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
#endif
     
  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 10, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudio_Thread, Audio_Thread, osPriorityNormal, 0, 2 * 1024);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);

  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  This function Set the new gain of the equilizer
  * @param  BandNum : equilizer band index
  * @param  NewGainValue : the new band gain -12 dB to 12 dB.
  * @retval None.
*/
void AUDIOPLAYER_SetEqGain(uint8_t BandNum, int16_t NewGainValue)
{
#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
  if((NewGainValue > -12) && (NewGainValue < 12)) 
  {
    SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[BandNum], BandNum );
    AUDIO_EQ_Bands[BandNum].gainDb = NewGainValue;
    SpiritEQ_FltSet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[BandNum], BandNum);
  }
#endif
}

/**
  * @brief  This function return the gain of the equilizer
  * @param  BandNum : equilizer band index
  * @retval band gain -12 dB to +12 dB.
*/
int16_t AUDIOPLAYER_GetEqGain(uint8_t BandNum)
{
#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
  return (AUDIO_EQ_Bands[BandNum].gainDb);
#else
  return 0;
#endif
}

/**
  * @brief  This function Set Loudness Control Gain Value.
  * @param  NewGainValue -12 dB to +12 dB.
  * @retval None.
  */
void AUDIOPLAYER_SetLoudnessGain(int16_t NewGainValue)
{
#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
  if((NewGainValue > -12) && (NewGainValue < 12)) 
  {  
    SpiritLdCtrl_GetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
    AUDIO_LdCtrlInstanceParams.gainQ8 = CALC_LDNS_DB (NewGainValue);
    AUDIO_loudness = NewGainValue;
    SpiritLdCtrl_Reset  ((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance);
    SpiritLdCtrl_SetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
  }
#endif
}

/**
  * @brief  This function Set Loudness Control Gain Value.
  * @param  NewGainValue -12 dB to 12 dB.
  * @retval None.
  */
int16_t AUDIOPLAYER_GetLoudnessGain(void)
{
#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
  return (AUDIO_loudness);
#else
  return 0;
#endif
}

/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void)
{
  return haudio.state;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AUDIOPLAYER_GetVolume(void)
{
  return haudio.volume;
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume)
{
  if(BSP_AUDIO_OUT_SetVolume(0,volume) == BSP_ERROR_NONE)
  {
    haudio.volume = volume;
    return AUDIOPLAYER_ERROR_NONE;    
  }
  else
  {
   return AUDIOPLAYER_ERROR_HW;
  }
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency used to play the audio stream.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Play(uint32_t frequency)
{
  uint32_t numOfReadBytes;
  haudio.state = AUDIOPLAYER_PLAY;
  
  /* Fill whole buffer @ first time */
  if(f_read(&wav_file, 
            &haudio.buff[0], 
            AUDIO_OUT_BUFFER_SIZE, 
            (void *)&numOfReadBytes) == FR_OK)
  { 
    if(numOfReadBytes != 0)
    {
      BSP_AUDIO_OUT_Stop(0);
      memset(haudio.buff, 0, AUDIO_OUT_BUFFER_SIZE);

      if(haudio.freq != frequency )
      {
        BSP_AUDIO_OUT_SetSampleRate(0,frequency);
        haudio.freq = frequency;
      }

      BSP_AUDIO_OUT_Play(0, (uint8_t *)&haudio.buff[0], AUDIO_OUT_BUFFER_SIZE);
      return AUDIOPLAYER_ERROR_NONE;
    }
  }
  return AUDIOPLAYER_ERROR_IO;
}

/**
  * @brief  Audio player DeInit
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_DeInit(void)
{
  haudio.state = AUDIOPLAYER_STOP; 
  
  BSP_AUDIO_OUT_Stop(0);
  BSP_AUDIO_OUT_DeInit(0);

  f_close(&wav_file); 
  
  if(AudioEvent != 0)
  {
    vQueueDelete(AudioEvent); 
    AudioEvent = 0;
  }
  if(AudioThreadId != 0)
  {
    osThreadTerminate(AudioThreadId);
    AudioThreadId = 0;
  }

#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU1_BKP, AUDIO_EQ_Bands[0].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU2_BKP, AUDIO_EQ_Bands[1].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU3_BKP, AUDIO_EQ_Bands[2].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_EQU4_BKP, AUDIO_EQ_Bands[3].gainDb);
  RTC_BkupSaveParameter(AUDIOPLAYER_LOUD_BKP, AUDIO_loudness);
#endif

  return AUDIOPLAYER_ERROR_NONE;  
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void)
{
  haudio.state = AUDIOPLAYER_STOP;
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW); 
  f_close(&wav_file);

  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void)
{  
  haudio.state = AUDIOPLAYER_PAUSE;
  BSP_AUDIO_OUT_Pause(0);
  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void)
{
  haudio.state = AUDIOPLAYER_PLAY;
  BSP_AUDIO_OUT_Resume(0); 
  return AUDIOPLAYER_ERROR_NONE;
}
/**
  * @brief  Sets audio stream position
  * @param  position: stream position.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetPosition(uint32_t position)
{
  uint64_t file_pos;

  file_pos = (f_size(&wav_file) / 128 / 100);
  file_pos *= (position * 128); 
  f_lseek(&wav_file, file_pos);

  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Sets the volume at mute
  * @param  state: could be MUTE_ON to mute sound or MUTE_OFF to unmute 
  *                the codec and restore previous volume level.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Mute(uint8_t state)
{
   BSP_AUDIO_OUT_Mute(0);
   
   return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Get the wav file information.
  * @param  file: wav file.
  * @param  info: pointer to wav file structure
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_GetFileInfo(uint8_t* file, WAV_InfoTypedef* info)
{
  uint32_t numOfReadBytes;
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;

  
  if( f_open(&wav_file, (char *)file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    /* Fill the buffer to Send */
    if(f_read(&wav_file, info, sizeof(WAV_InfoTypedef), (void *)&numOfReadBytes) == FR_OK)
    {
      if((info->ChunkID == 0x46464952) && (info->AudioFormat == 1))
      {
        ret = AUDIOPLAYER_ERROR_NONE;
      }
    }
    f_close(&wav_file);      
  }
  return ret;
}

/**
  * @brief  Select wav file.
  * @param  file: wav file.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SelectFile(uint8_t * file)
{
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;
  if( f_open(&wav_file, (char *)file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    f_lseek(&wav_file, sizeof(WAV_InfoTypedef));
    ret = AUDIOPLAYER_ERROR_NONE;
  }
  return ret;  
}

/**
  * @brief  Get wav file progress.
  * @param  None
  * @retval file progress.
  */
uint32_t AUDIOPLAYER_GetProgress(void)
{
  return f_tell(&wav_file);
}

/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  if(haudio.state == AUDIOPLAYER_PLAY)
  {
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_FULL, 0);    
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{ 
  if(haudio.state == AUDIOPLAYER_PLAY)
  {
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_HALF, 0);    
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(uint32_t Instance)
{
  haudio.state = AUDIOPLAYER_ERROR;
}

/**
  * @brief  Audio task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Audio_Thread(void const * argument)
{
  uint32_t numOfReadBytes;    
  osEvent event;  
  for(;;)
  {
    event = osMessageGet(AudioEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      if(haudio.state == AUDIOPLAYER_PLAY)
      {
        switch(event.value.v)
        {
        case PLAY_BUFFER_OFFSET_HALF:
          if(f_read(&wav_file, 
                    &haudio.buff[0], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            {  
              AUDIOPLAYER_NotifyEndOfFile();
            } 
            
#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
            SpiritEQ_Apply((void *)AUDIO_EqInstance,
                           /* NB_Channel */2, 
                           (int16_t *)&haudio.buff[0], 
                           numOfReadBytes  / 4);
            
            /* Apply Loudness settings */
            SpiritLdCtrl_Apply((TSpiritLdCtrl *)AUDIO_LdCtrlPersistance,
                               /* NB_Channel */ 2, 
                               (int16_t *)&haudio.buff[0], 
                               numOfReadBytes / 4, 
                               (void *)AUDIO_LdCtrlScratch);            
            
#endif
          }
          else
          {
              AUDIOPLAYER_NotifyError();    
          }
          break;  
          
        case PLAY_BUFFER_OFFSET_FULL:
          if(f_read(&wav_file, 
                    &haudio.buff[AUDIO_OUT_BUFFER_SIZE/2], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            { 
              AUDIOPLAYER_NotifyEndOfFile();                     
            } 
#if !defined  ( __GNUC__ ) && !defined ( __ICCARM__ ) && !defined (__ARMCC_VERSION)
            SpiritEQ_Apply((void *)AUDIO_EqInstance,
                           /* NB_Channel */2, 
                           (int16_t *)&haudio.buff[AUDIO_OUT_BUFFER_SIZE /2], 
                           numOfReadBytes  / 4);
            
            
            /* Apply Loudness settings */
            SpiritLdCtrl_Apply((TSpiritLdCtrl *)AUDIO_LdCtrlPersistance,
                               /* NB_Channel */ 2, 
                               (int16_t *)&haudio.buff[AUDIO_OUT_BUFFER_SIZE /2], 
                               numOfReadBytes / 4, 
                               (void *)AUDIO_LdCtrlScratch);                
#endif
          }
          else
          {
              AUDIOPLAYER_NotifyError();   
          }
          break;   
          
        default:
          break;
        }
      }
      
    }

  }
}

/**
  * @brief  Notify end of playing.
  * @param  None
  * @retval None
  */
__weak AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Notify audio error.
  * @param  None
  * @retval None
  */
__weak AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyError(void)
{
  return AUDIOPLAYER_Stop();
}

/**
  * @}
  */

/**
  * @}
  */

