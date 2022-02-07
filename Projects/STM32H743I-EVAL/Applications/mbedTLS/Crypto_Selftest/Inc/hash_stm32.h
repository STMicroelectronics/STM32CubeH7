/**
  ******************************************************************************
  * @brief   Header file of mbed TLS HW crypto (HASH) implementation.
  ******************************************************************************
  * @attention
  *
  * Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
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
#ifndef __HASH_H
#define __HASH_H

#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_MD5_ALT)

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* include the appropriate header file */
#include "stm32h7xx_hal.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

/* macros --------------------------------------------------------------------*/
/* constants -----------------------------------------------------------------*/
#define ST_HASH_TIMEOUT ((uint32_t) 1000)  /* TO in ms for the hash processor */

/* defines -------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
#if defined(MBEDTLS_THREADING_C)
extern mbedtls_threading_mutex_t hash_mutex;
extern unsigned char hash_mutex_started;
#endif /* MBEDTLS_THREADING_C */

extern unsigned int hash_context_count;

/* functions prototypes ------------------------------------------------------*/
extern void hash_zeroize(void *v, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT or MBEDTLS_SHA256_ALT or MBEDTLS_MD5_ALT */
#endif /*__HASH_H */
