/**
  ******************************************************************************
  * @file    OTFDEC_EncryptionDecryption/Inc/RSS_OTFD.h
  * @author  MCD Application Team
  * @brief   otfd.c module
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

#ifndef RSS_OTFD_H
#define RSS_OTFD_H
#include "RSS.h"

/** \defgroup rss_otfd OTFD
OTFD support functions
*/

/** OTFD encryption parameters
* Contains parameters for OTFD encryption
*/
typedef struct
{
	struct
	{
		uint32_t dest; /**< Octo-SPI memory address to store the encrypted code */
		uint8_t* buffer; /**< RAM address of buffer containing data to encrypt. Will contain encrypted data on next boot */
		uint32_t size; /**< size of the data in RAM buffer */
	}data;
	struct
	{
		uint32_t number; /**< OTFD region number. Value between 0..7
													0: OTFDEC1 region 1
													1: OTFDEC1 region 2
													2: OTFDEC1 region 3
													4: OTFDEC1 region 4
													5: OTFDEC2 region 1
													6: OTFDEC2 region 2
													7: OTFDEC2 region 3
													8: OTFDEC2 region 4
											*/
		uint32_t mode; /**< OTFD region mode. 0 = instruction only AES-CTR, 1 = data only AES-CTR, 2 = instruction or data AES-CTR, 3 = instruction only Enhanced Cipher */
		uint32_t version; /**< region version */
		void* nonce; /**< address of 64 bits nonce */
		void* key; /**< address of 128 bits region key */
	}region; /**< info about OTFD region */
}RSS_OTFD_ENC_Params_t;

/**
*\ingroup rss_OTFD
* \brief perform encryption using OTFD IP
*
* Reset into RSS secure zone to encrypt in place a data buffer stored in RAM,
*  using the On-The Fly Decryption IP.
*
* Option bytes requirements to call this function:
*  - Security ON
*
* Other requirements:\n
*  - During reboot, RSS uses 18KB DTCM from 0x2000 0800 to 0x2000 5000.
*
* This function should not be called directly, as its address is subject to change. Use the \ref RSS_OTFD table instead. (RSS_OTFD->resetAndEncrypt)
*
* This function configures the secure API and reset to launch the corresponding RSS secure service on next boot (\ref SECAPI_OTFD_installFW)
*
* \param[in] OTFD encryption parameters pointer
*
* \warning
*/
void RSS_OTFD_resetAndEncrypt(RSS_OTFD_ENC_Params_t* otfd_session_params);

/* RSS API table for OTFD services */
/**
*\ingroup rss_otfd
* @brief holds pointers to OTFD API functions
*
* for example, to call RSS_OTFD_resetAndEncrypt using this table, call RSS_OTFD->resetAndEncrypt
*/
typedef struct
{
	void				(*resetAndEncrypt) (RSS_OTFD_ENC_Params_t* otfd_session_params); /**< pointer to \ref RSS_OTFD_resetAndEncrypt API function */
}RSS_API_OTFD_Table_t;

/**
*\ingroup rss_otfd
* @brief fixed location of the table */
#define RSS_OTFD ((RSS_API_OTFD_Table_t*)0x1FF09560)

#endif // RSS_OTFD_H
