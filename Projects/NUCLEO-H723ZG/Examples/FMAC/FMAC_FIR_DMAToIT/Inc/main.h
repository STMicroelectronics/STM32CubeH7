/**
  ******************************************************************************
  * @file    FMAC/FMAC_FIR_DMAToIT/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* Configuration: uncomment in order to use the clipping feature */
#define CLIP_ENABLED

/* Not called/called status */
#define DATA_NOT_READY          0
#define DATA_READY              1

/* Size of the data arrays */
#define ARRAY_SIZE              508

/* Size of the preloaded input data array (size <= INPUT_BUFFER_SIZE) */
#define INPUT_ARRAY_1_SIZE      100

/* Size of the input data arrays written during calculation */
#define INPUT_ARRAY_2_SIZE      120
#define INPUT_ARRAY_3_SIZE      110
#define INPUT_ARRAY_4_SIZE      182

/* Expected number of calls to HAL_FMAC_GetDataCallback */
#define GET_DATA_CALLBACK_COUNT  3

/* Size of the output data arrays read during calculation
   Total expected size: input array size - number of feed-forward taps */
#define OUTPUT_ARRAY_1_SIZE     259
#define OUTPUT_ARRAY_2_SIZE     249

/* Expected number of calls to HAL_FMAC_OutputDataReadyCallback */
#define DATA_RDY_CALLBACK_COUNT 2



/* Filter parameter P: number of feed-forward taps or coefficients in the range [2:127] */
#define COEFF_VECTOR_B_SIZE     5

/* Filter parameter Q: not used */
#define FILTER_PARAM_Q_NOT_USED 0

/* Filter parameter R: gain in the range [0:7] */
#define GAIN                    1



/* Throughput parameter: extra space in the input buffer (minimum: 0) */
#define MEMORY_PARAMETER_D1     122

/* Throughput parameter: extra space in the output buffer (minimum: 1) */
#define MEMORY_PARAMETER_D2     123

/* Throughput parameter: watermark threshold for the input buffer */
#define INPUT_THRESHOLD         FMAC_THRESHOLD_1

/* Throughput parameter: watermark threshold for the output buffer (inferior or equal to MEMORY_PARAMETER_D1) */
#define OUTPUT_THRESHOLD        FMAC_THRESHOLD_1



/* FMAC internal memory configuration: base address of the coefficient buffer */
#define COEFFICIENT_BUFFER_BASE 0

/* FMAC internal memory configuration: size of the coefficient buffer */
#define COEFFICIENT_BUFFER_SIZE COEFF_VECTOR_B_SIZE

/* FMAC internal memory configuration: base address of the input buffer */
#define INPUT_BUFFER_BASE       COEFFICIENT_BUFFER_SIZE

/* FMAC internal memory configuration: size of the input buffer */
#define INPUT_BUFFER_SIZE       COEFF_VECTOR_B_SIZE + MEMORY_PARAMETER_D1

/* FMAC internal memory configuration: base address of the input buffer */
#define OUTPUT_BUFFER_BASE      COEFFICIENT_BUFFER_SIZE + INPUT_BUFFER_SIZE

/* FMAC internal memory configuration: size of the input buffer */
#define OUTPUT_BUFFER_SIZE      MEMORY_PARAMETER_D2


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

