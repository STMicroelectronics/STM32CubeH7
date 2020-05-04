/**
  ******************************************************************************
  * @file    coders.c
  * @author  MCD Application Team
  * @brief   This file includes Coders layer routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "coders.h"

/** @addtogroup STM32_audio_coders
* @{
*/

/** @defgroup Decoders
* @brief Interface between the player and coders modules
* @{
*/

/** @defgroup Decoders_Exported_Functions
* @{
*/

/**
* @brief  This function initialize decoder struct depending on file extension.
* @param  last char of file extension
* @retval 0 if success, 1 if unhandled format.
*/
int8_t CODERS_SelectDecoder(Decoder_TypeDef* pDecoderStruct, int8_t ch)
{
  switch (ch)
  {
#ifdef __MP3_DECODER__
    /* .mp3 files (.mp'3') */
  case ('3'):
    pDecoderStruct->DecoderInit = Mp3Process_DecoderInit;
    pDecoderStruct->DecoderDeInit = Mp3Process_DecoderDeInit;
    pDecoderStruct->DecoderStopMode = Mp3Process_StopMode;
    pDecoderStruct->Decoder_StreamForward = NULL; /* It is managed by high layer */
    pDecoderStruct->Decoder_StreamBackward = NULL; /* It is managed by high layer */
    pDecoderStruct->Decoder_DecodeData = Mp3Process_DecodeData;
    pDecoderStruct->Decoder_ReadTags = Mp3Process_RetrieveIDTAGS;
    pDecoderStruct->Decoder_GetSamplingRate = Mp3Process_GetSamplingRate;

    pDecoderStruct->Decoder_GetBitRateKbps = Mp3Process_GetBitRateKbps;
    pDecoderStruct->Decoder_GetNbChannels = Mp3Process_GetNbChannels;
    pDecoderStruct->Decoder_GetNbBits = NULL;
    
    pDecoderStruct->Decoder_SetProgressTime = Mp3Process_DecSetProgressTime;
    pDecoderStruct->Decoder_GetStreamLength = Mp3Process_GetStreamLenght;
    pDecoderStruct->Decoder_GetElapsedTime = Mp3Process_GetElapsedTime;
    pDecoderStruct->Decoder_VBRDetected = Mp3Process_VBRDetected;
    pDecoderStruct->PacketSize = MP3_PACKET_SZE;
    return 0;
#endif /* __MP3_DECODER__ */


#ifdef __WMA_DECODER__
    /* .wma files (.wm'{a,A}') */
  case ('a'):
  case ('A'):
    pDecoderStruct->DecoderInit = WmaProcess_DecoderInit;
    pDecoderStruct->DecoderDeInit = WmaProcess_DecoderDeInit;
    pDecoderStruct->DecoderStopMode = WmaProcess_StopMode;
    pDecoderStruct->Decoder_StreamForward = WmaProcess_Forward;
    pDecoderStruct->Decoder_StreamBackward = WmaProcess_Backward;
    pDecoderStruct->Decoder_DecodeData = WmaProcess_DecodeData;
    pDecoderStruct->Decoder_ReadTags = WmaProcess_RetrieveIDTAGS;
    pDecoderStruct->Decoder_GetSamplingRate = WmaProcess_GetSamplingRate;

    pDecoderStruct->Decoder_GetBitRateKbps = WmaProcess_GetBitRateKbps;
    pDecoderStruct->Decoder_GetNbChannels = WmaProcess_GetNbChannels;
    pDecoderStruct->Decoder_GetNbBits = NULL;

    pDecoderStruct->Decoder_GetStreamLength = WmaProcess_GetStreamLenght;
    pDecoderStruct->Decoder_GetElapsedTime = WmaProcess_GetElapsedTime;
    pDecoderStruct->Decoder_VBRDetected = NULL;
    pDecoderStruct->PacketSize = WMA_PACKET_SZE;
    return 0;
#endif /* __WMA_DECODER__ */

#ifdef __WAV_DECODER__
    /* .wav files (.wa'{v,V}') */
  case ('v'):
  case ('V'):
    pDecoderStruct->DecoderInit = WavProcess_DecInit;
    pDecoderStruct->DecoderDeInit = NULL;
    pDecoderStruct->DecoderStopMode = NULL;
    pDecoderStruct->Decoder_StreamForward = NULL; /* It is managed by high layer */
    pDecoderStruct->Decoder_StreamBackward = NULL; /* It is managed by high layer */
    pDecoderStruct->Decoder_DecodeData = WavProcess_DecodeData;
    pDecoderStruct->Decoder_ReadTags = NULL;
    pDecoderStruct->Decoder_GetSamplingRate = WavProcess_DecGetSamplingRate;

    pDecoderStruct->Decoder_GetBitRateKbps = NULL;
    pDecoderStruct->Decoder_GetNbChannels = WavProcess_GetNbChannels;
    pDecoderStruct->Decoder_GetNbBits = WavProcess_GetNbBits;
    
    pDecoderStruct->Decoder_SetProgressTime = WavProcess_DecSetProgressTime;
    pDecoderStruct->Decoder_GetStreamLength = WavProcess_DecGetStreamLenght;
    pDecoderStruct->Decoder_GetElapsedTime = WavProcess_DecGetElapsedTime;
    pDecoderStruct->Decoder_VBRDetected = NULL;
    pDecoderStruct->PacketSize = WAV_PACKET_SZE;
    return 0;
#endif /* __WAV_DECODER__ */

#ifdef __SPEEX_DECODER__
    /* .spx files (.sp'x,X') */
  case ('x'):
  case ('X'):
    pDecoderStruct->DecoderInit = SpeexProcess_DecoderInit;
    pDecoderStruct->DecoderDeInit = SpeexProcess_DecoderDeInit;
    pDecoderStruct->DecoderStopMode = SpeexProcess_StopMode;
    pDecoderStruct->Decoder_StreamForward = SpeexProcess_StreamForward;
    pDecoderStruct->Decoder_StreamBackward = SpeexProcess_StreamBackward;
    pDecoderStruct->Decoder_DecodeData = SpeexProcess_DecodeData;
    pDecoderStruct->Decoder_ReadTags = NULL;
    pDecoderStruct->Decoder_GetSamplingRate = SpeexProcess_GetSamplingRate;

    pDecoderStruct->Decoder_GetBitRateKbps = NULL;
    pDecoderStruct->Decoder_GetNbChannels = NULL;
    pDecoderStruct->Decoder_GetNbBits = NULL;

    pDecoderStruct->Decoder_GetStreamLength = SpeexProcess_GetStreamLenght;
    pDecoderStruct->Decoder_GetElapsedTime = SpeexProcess_GetElapsedTime;
    pDecoderStruct->Decoder_VBRDetected = NULL;
    pDecoderStruct->PacketSize = SPEEX_PACKET_SZE;
    return 0;
#endif /* __SPEEX_DECODER__ */

#ifdef __G711_DECODER__
    /* .g711 files (.g71'1') */
  case ('1'):
    pDecoderStruct->DecoderInit = G711Process_DecoderInit;
    pDecoderStruct->DecoderDeInit = G711Process_DecoderDeInit;
    pDecoderStruct->DecoderStopMode = G711Process_StopMode;
    pDecoderStruct->Decoder_StreamForward = G711Process_StreamForward;
    pDecoderStruct->Decoder_StreamBackward = G711Process_StreamBackward;
    pDecoderStruct->Decoder_DecodeData = G711Process_DecodeData;
    pDecoderStruct->Decoder_ReadTags = NULL;
    pDecoderStruct->Decoder_GetSamplingRate = G711Process_GetSamplingRate;

    pDecoderStruct->Decoder_GetBitRateKbps = NULL;
    pDecoderStruct->Decoder_GetNbChannels = NULL;
    pDecoderStruct->Decoder_GetNbBits = NULL;


    pDecoderStruct->Decoder_GetStreamLength = NULL;
    pDecoderStruct->Decoder_GetElapsedTime = G711Process_GetElapsedTime;
    pDecoderStruct->Decoder_VBRDetected = NULL;
    pDecoderStruct->PacketSize = G711_PACKET_SZE;
    return 0;
#endif /* __G711_DECODER__ */

#ifdef __G726_DECODER__
    /* .g726 files (.g72'6') */
  case ('6'):
    pDecoderStruct->DecoderInit = G726Process_DecoderInit;
    pDecoderStruct->DecoderDeInit = G726Process_DecoderDeInit;
    pDecoderStruct->DecoderStopMode = G726Process_StopMode;
    pDecoderStruct->Decoder_StreamForward = G726Process_StreamForward;
    pDecoderStruct->Decoder_StreamBackward = G726Process_StreamBackward;
    pDecoderStruct->Decoder_DecodeData = G726Process_DecodeData;
    pDecoderStruct->Decoder_ReadTags = NULL;
    pDecoderStruct->Decoder_GetSamplingRate = G726Process_GetSamplingRate;

    pDecoderStruct->Decoder_GetBitRateKbps = NULL;
    pDecoderStruct->Decoder_GetNbChannels = NULL;
    pDecoderStruct->Decoder_GetNbBits = NULL;

    pDecoderStruct->Decoder_GetStreamLength = G726Process_GetStreamLenght;
    pDecoderStruct->Decoder_GetElapsedTime = G726Process_GetElapsedTime;
    pDecoderStruct->Decoder_VBRDetected = NULL;
    pDecoderStruct->PacketSize = G726_PACKET_SZE;
    return 0;
#endif /* __G726_DECODER__ */

#ifdef __ADPCM_DECODER__
    /* .adpcm files (.adpc'm,M') */
    /* --> .adp files (.ad'p,P') */
  case ('p'):
  case ('P'):
    pDecoderStruct->DecoderInit = AdpcmProcess_DecoderInit;
    pDecoderStruct->DecoderDeInit = AdpcmProcess_DecoderDeInit;
    pDecoderStruct->DecoderStopMode = AdpcmProcess_StopMode;
    pDecoderStruct->Decoder_StreamForward = AdpcmProcess_StreamForward;
    pDecoderStruct->Decoder_StreamBackward = AdpcmProcess_StreamBackward;
    pDecoderStruct->Decoder_DecodeData = AdpcmProcess_DecodeData;
    pDecoderStruct->Decoder_ReadTags = NULL;
    pDecoderStruct->Decoder_GetSamplingRate = AdpcmProcess_GetSamplingRate;

    pDecoderStruct->Decoder_GetBitRateKbps = NULL;
    pDecoderStruct->Decoder_GetNbChannels = NULL;
    pDecoderStruct->Decoder_GetNbBits = NULL;

    pDecoderStruct->Decoder_GetStreamLength = AdpcmProcess_GetStreamLenght;
    pDecoderStruct->Decoder_GetElapsedTime = AdpcmProcess_GetElapsedTime;
    pDecoderStruct->Decoder_VBRDetected = NULL;
    pDecoderStruct->PacketSize = ADPCM_PACKET_SZE;
    return 0;
#endif /* __ADPCM_DECODER__ */


    /* Unsupported file format */
  default:
    pDecoderStruct->DecoderInit = NULL;
    pDecoderStruct->DecoderDeInit = NULL;
    pDecoderStruct->DecoderStopMode = NULL;
    pDecoderStruct->Decoder_StreamForward = NULL;
    pDecoderStruct->Decoder_StreamBackward = NULL;
    pDecoderStruct->Decoder_DecodeData = NULL;
    pDecoderStruct->Decoder_ReadTags = NULL;
    pDecoderStruct->Decoder_GetSamplingRate = NULL;

    pDecoderStruct->Decoder_GetBitRateKbps = NULL;
    pDecoderStruct->Decoder_GetNbChannels = NULL;
    pDecoderStruct->Decoder_GetNbBits = NULL;

    pDecoderStruct->Decoder_GetStreamLength = NULL;
    pDecoderStruct->Decoder_GetElapsedTime = NULL;
    pDecoderStruct->Decoder_VBRDetected = NULL;
    pDecoderStruct->PacketSize = MAX_OUT_DEFAULT_PACKET_SZE;
    return 1;
  }
}

/**
* @brief  This function initialize Encoder struct depending on file extension.
* @param  last char of file extension
* @retval 0 if success, 1 if unhandled format.
*/
int8_t CODERS_SelectEncoder(Encoder_TypeDef* pEncoderStruct, int8_t ch)
{
  switch (ch)
  {
#ifdef __MP3_ENCODER__
    /* .mp3 files (.mp'3') */
  case ('3'):
    pEncoderStruct->EncoderInit = Mp3Process_EncoderInit;
    pEncoderStruct->EncoderDeInit = Mp3Process_EncoderDeInit;
    pEncoderStruct->EncoderStopMode = Mp3Process_EncoderStopMode;
    pEncoderStruct->Encoder_EncodeData = Mp3Process_EncodeData;
    pEncoderStruct->Encoder_HeaderUpdate =NULL;
    pEncoderStruct->Encoder_SetTags = Mp3Process_SetTags;
    pEncoderStruct->Encoder_GetSampleRate = Mp3Process_EncGetSampleRate;
    pEncoderStruct->Encoder_GetStreamLength = Mp3Process_EncGetStreamLength;
    pEncoderStruct->Encoder_GetElapsedTime = Mp3Process_EncGetElapsedTime;
    pEncoderStruct->PacketSize = MP3IN_PACKET_SZE;
    return 0;
#endif /* __MP3_ENCODER__ */


#ifdef __WAV_ENCODER__
    /* .wav files (.wa'{v,V}') */
  case ('v'):
  case ('V'):
    pEncoderStruct->EncoderInit = WavProcess_EncInit;
    pEncoderStruct->EncoderDeInit = WavProcess_EncDeInit;
    pEncoderStruct->EncoderStopMode = NULL;
    pEncoderStruct->Encoder_EncodeData = WavProcess_EncodeData;
    pEncoderStruct->Encoder_HeaderUpdate =WavProcess_HeaderUpdate;
    pEncoderStruct->Encoder_SetTags = NULL;
    pEncoderStruct->Encoder_GetSampleRate = WavProcess_EncGetSampleRate;
    pEncoderStruct->Encoder_GetStreamLength = WavProcess_EncGetStreamLength;
    pEncoderStruct->Encoder_GetElapsedTime = WavProcess_EncGetElapsedTime;
    pEncoderStruct->PacketSize = WAVIN_PACKET_SZE;
    return 0;
#endif /* __WAV_ENCODER__ */

#ifdef __SPEEX_ENCODER__
    /* .spx files (.sp'{x,X}') */
  case ('x'):
  case ('X'):
    pEncoderStruct->EncoderInit = SpeexProcess_EncoderInit;
    pEncoderStruct->EncoderDeInit = SpeexProcess_EncoderDeInit;
    pEncoderStruct->EncoderStopMode = NULL;
    pEncoderStruct->Encoder_EncodeData = SpeexProcess_EncodeData;
    pEncoderStruct->Encoder_HeaderUpdate =NULL;
    pEncoderStruct->Encoder_SetTags = NULL;
    pEncoderStruct->Encoder_GetSampleRate = SpeexProcess_GetSamplingRate;
    pEncoderStruct->Encoder_GetStreamLength = NULL;
    pEncoderStruct->Encoder_GetElapsedTime = NULL;
    pEncoderStruct->PacketSize = SPEEX_PACKET_SZE;
    return 0;
#endif /* __SPEEX_ENCODER__ */

#ifdef __G711_ENCODER__
    /* .g711 files (.g71'1') */
  case ('1'):
    pEncoderStruct->EncoderInit = G711Process_EncoderInit;
    pEncoderStruct->EncoderDeInit = G711Process_EncoderDeInit;
    pEncoderStruct->EncoderStopMode = NULL;
    pEncoderStruct->Encoder_EncodeData = G711Process_EncodeData;
    pEncoderStruct->Encoder_HeaderUpdate =NULL;
    pEncoderStruct->Encoder_SetTags = NULL;
    pEncoderStruct->Encoder_GetSampleRate = G711Process_GetSamplingRate;
    pEncoderStruct->Encoder_GetStreamLength = G711Process_GetStreamLength;
    pEncoderStruct->Encoder_GetElapsedTime = NULL;
    pEncoderStruct->PacketSize = G711_PACKET_SZE;
    return 0;
#endif /* __G711_ENCODER__ */

#ifdef __G726_ENCODER__
    /* .g726 files (.g72'6') */
  case ('6'):
    pEncoderStruct->EncoderInit = G726Process_EncoderInit;
    pEncoderStruct->EncoderDeInit = G726Process_EncoderDeInit;
    pEncoderStruct->EncoderStopMode = NULL;
    pEncoderStruct->Encoder_EncodeData = G726Process_EncodeData;
    pEncoderStruct->Encoder_HeaderUpdate =NULL;
    pEncoderStruct->Encoder_SetTags = NULL;
    pEncoderStruct->Encoder_GetSampleRate = G726Process_GetSamplingRate;
    pEncoderStruct->Encoder_GetStreamLength = NULL;
    pEncoderStruct->Encoder_GetElapsedTime = NULL;
    pEncoderStruct->PacketSize = G726_PACKET_SZE;
    return 0;
#endif /* __G726_ENCODER__ */

#ifdef __ADPCM_ENCODER__
    /* .adpcm files (.adpc'm,M') */
    /* --> .adp files (.ad'p,P') */
  case ('p'):
  case ('P'):
    pEncoderStruct->EncoderInit = AdpcmProcess_EncoderInit;
    pEncoderStruct->EncoderDeInit = AdpcmProcess_EncoderDeInit;
    pEncoderStruct->EncoderStopMode = NULL;
    pEncoderStruct->Encoder_EncodeData = AdpcmProcess_EncodeData;
    pEncoderStruct->Encoder_HeaderUpdate =NULL;
    pEncoderStruct->Encoder_SetTags = NULL;
    pEncoderStruct->Encoder_GetSampleRate = AdpcmProcess_GetSamplingRate;
    pEncoderStruct->Encoder_GetStreamLength = NULL;
    pEncoderStruct->Encoder_GetElapsedTime = NULL;
    pEncoderStruct->PacketSize = ADPCM_PACKET_SZE;
    return 0;
#endif /* __ADPCM_ENCODER__ */

    /* Unsupported file format */
  default:
    pEncoderStruct->EncoderInit = NULL;
    pEncoderStruct->EncoderDeInit = NULL;
    pEncoderStruct->EncoderStopMode = NULL;
    pEncoderStruct->Encoder_EncodeData = NULL;
    pEncoderStruct->Encoder_HeaderUpdate =NULL;
    pEncoderStruct->Encoder_SetTags = NULL;
    pEncoderStruct->Encoder_GetSampleRate = NULL;
    pEncoderStruct->Encoder_GetStreamLength = NULL;
    pEncoderStruct->Encoder_GetElapsedTime = NULL;
    pEncoderStruct->PacketSize = MAX_OUT_DEFAULT_PACKET_SZE;
    return 1;
  }
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
