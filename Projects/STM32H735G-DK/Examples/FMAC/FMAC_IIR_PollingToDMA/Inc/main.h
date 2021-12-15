/**
  ******************************************************************************
  * @file    FMAC/FMAC_IIR_PollingToDMA/Inc/main.h
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
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
/* Not called/called status */
#define CALLBACK_NOT_CALLED     0
#define CALLBACK_CALLED         1

/* Size of the data arrays */
#define ARRAY_SIZE              256



/* Filter parameter P: number of feed-forward taps or coefficients in the range [2:64] */
#define COEFF_VECTOR_B_SIZE     7

/* Filter parameter Q: number of feedback coefficients in the range [1:COEFF_VECTOR_B_SIZE-1] */
#define COEFF_VECTOR_A_SIZE     6

/* Filter parameter R: gain in the range [0:7] */
#define GAIN                    0



/* Throughput parameter: extra space in the input buffer (minimum: 0) */
#define MEMORY_PARAMETER_D1     114

/* Throughput parameter: extra space in the output buffer (minimum: 1) */
#define MEMORY_PARAMETER_D2     115

/* Throughput parameter: watermark threshold for the input buffer */
#define INPUT_THRESHOLD         FMAC_THRESHOLD_1

/* Throughput parameter: watermark threshold for the output buffer (inferior or equal to MEMORY_PARAMETER_D1) */
#define OUTPUT_THRESHOLD        FMAC_THRESHOLD_1



/* FMAC internal memory configuration: base address of the coefficient buffer */
#define COEFFICIENT_BUFFER_BASE 0

/* FMAC internal memory configuration: size of the two coefficient buffers */
#define COEFFICIENT_BUFFER_SIZE COEFF_VECTOR_B_SIZE + COEFF_VECTOR_A_SIZE

/* FMAC internal memory configuration: base address of the input buffer */
#define INPUT_BUFFER_BASE       COEFFICIENT_BUFFER_SIZE

/* FMAC internal memory configuration: size of the input buffer */
#define INPUT_BUFFER_SIZE       COEFF_VECTOR_B_SIZE + MEMORY_PARAMETER_D1

/* FMAC internal memory configuration: base address of the input buffer */
#define OUTPUT_BUFFER_BASE      COEFFICIENT_BUFFER_SIZE + INPUT_BUFFER_SIZE

/* FMAC internal memory configuration: size of the input buffer */
#define OUTPUT_BUFFER_SIZE      COEFF_VECTOR_A_SIZE + MEMORY_PARAMETER_D2

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

