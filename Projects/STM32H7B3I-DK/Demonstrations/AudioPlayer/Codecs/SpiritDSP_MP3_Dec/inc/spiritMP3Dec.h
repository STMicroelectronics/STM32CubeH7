/*
  ******************************************************************************
  * @file    spiritMP3Dec.h
  * @author  MCD Application Team
  * @brief   This file is header for spiritMP3Dec MP3 decoder library.
  ******************************************************************************
  * @attention
  *
  * Portions SPIRIT Audio Engine Copyright (c) 1995-2009, SPIRIT
  * Portions Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __SPIRITMP3DEC_H__
#define __SPIRITMP3DEC_H__

/**
*\addtogroup SPIRIT_MP3_DECODER
* @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
    Informational structure to provide information about MP3 stream.
*/
typedef struct
{
    unsigned int nLayer;                         /* MPEG Audio Layer (1-3). Zero indicates that structure is not valid */
    unsigned int nSampleRateHz;                  /* Sample rate, Hz */
    unsigned int nBitrateKbps;                   /* Current bit rate, kilobit per seconds (0 for free-format stream) */
    unsigned int nChannels;                      /* Number of channels (1 or 2) */
    unsigned int IsGoodStream;                   /* Zero indicates that audio stream is not consistent */
    unsigned int anCutOffFrq576[2];              /* Cut-off frequencies for both channels (range 0-576), where 576 = Nyquist frequency */
    unsigned int nBitsReadAfterFrameHeader;      /* Number of bits, read after last frame header (for AV sync) */
    unsigned int nSamplesPerFrame;               /* Number of samples per audio frame (for AV sync) */
    unsigned int nSamplesLeftInFrame;            /* Number of samples, remaining in the internal buffer (for AV sync) */
} TSpiritMP3Info;


/**
    MP3 decoder object (persistent RAM).
*/
typedef struct
{
    int hidden[3086];                            /* Structure contents is hidden for demo version */
} TSpiritMP3Decoder;


/**
    Callback function type to supply decoder with input data.
    return number of MAU's read into buffer.
*/
typedef unsigned int (fnSpiritMP3ReadCallback)(
    void * pMP3CompressedData,                   /* [OUT] Pointer to buffer to fill with coded MP3 data */
    unsigned int nMP3DataSizeInChars,            /* Buffer size in MAU's */
    void * token                                 /* Application-supplied token */
    );


/**
*   Application-supplied MDCT-domain processing function type.
*   MDCT coefficients represented as float or int32 type, depending
*   on build options.
*   This function can be used for equalizer or pitch-shifter
*/
typedef void (fnSpiritMP3ProcessCallback)(
    void * mdct_samples,                /* [IN/OUT] MDCT coefficients  */
    int isShort,                        /* Flag: 1 if short frame */
    int ch,                             /* Channel number (0 or 1). */
    void * token                        /* Application-supplied token */
    );

/**
*  Decoder initialization function
*/
void SpiritMP3DecoderInit(
    TSpiritMP3Decoder *pDecoder,                 /* Decoder structure */
    fnSpiritMP3ReadCallback* pCallbackFn,        /* Data reading callback function */
    fnSpiritMP3ProcessCallback *pProcessFn,      /* Data processing callback function */
    void * token                                 /* Optional parameter for callback function */
    );


/**
    Decoding function
    return number of audio samples decoded.
    NOTE: function always produce stereo data (1 sample = 32 bit = 2ch*16 bit).
*/
unsigned int SpiritMP3Decode (
    TSpiritMP3Decoder *pDecoder,                 /* Decoder structure */
    short *pPCMSamples,                          /* [OUT] Output PCM buffer */
    unsigned int nSamplesRequired,               /* Number of samples to decode (1 sample = 32 bit = 2ch*16 bit) */
    TSpiritMP3Info * pMP3Info                    /* [OUT, opt] Optional informational structure */
    );


/**
*   Return sizeof(TSpiritMP3Decoder)
*/
int SpiritMP3DecoderGetPersistentSize(void);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif

