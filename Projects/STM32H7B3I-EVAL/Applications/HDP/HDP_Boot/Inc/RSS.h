/**
  ******************************************************************************
  * @file    HDP_Boot/Inc/RSS.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef RSS_H
#define RSS_H
#include "inttypes.h"

typedef uint32_t RSS_Status_t ;
#define RSS_ERROR        ((uint32_t)0)
#define RSS_DISABLED    ((uint32_t)0xEE000091)
#define RSS_OK          ((uint32_t)0xEE000092)

typedef uint32_t RSS_FlashBank_et ;
#define RSS_PCROP_FLASH_BANK_1 ((uint32_t)1)
#define RSS_PCROP_FLASH_BANK_2 ((uint32_t)2)

#define RSS_ENABLE_JTAG_AT_EXIT    ((unsigned int) 0x97A0FA17)
#define RSS_KEEP_JTAG_DISABLED_AT_EXIT 	((unsigned int) 0)

#define RSS_FWKEY_SIZE  16

#define RSS_MODULE_SIZE_MAX (512*1024) /* 512K max */

/* the following allows to enable RSS application services (SMI, destroy PCROP area) at runtime
the magic value must be present at the right secure user vector location */
#define RSS_ENABLE_RUNTIME_SERVICE_VECTOR ((uint32_t)0x7)
#define RSS_ENABLE_RUNTIME_SERVICE_MAGIC ((uint32_t)0x45535352)

/** RSS version
*/
typedef struct
{
    uint8_t patch;/**< bug fixes */
    uint8_t minor;/**< backward compatible changes : public API deprecated, new API functions, improvements, ... */
    uint8_t major;/**< backward incompatible changes : public API modification */
    uint8_t unused; /**< reserved */
}RSS_Version_t;

/** Firmware key information.
* Contains information about firmware key.
*/
typedef struct
{
    uint32_t    iv[4]; /**< base initialization vector to be used with key */
    uint8_t    value[RSS_FWKEY_SIZE]; /**< key value (max 64 bytes) */
}RSS_FWKey_t;

/** Firmware license.
* Contains information for RSS to unwrap license and retrieve FW key(s).
*/
typedef struct
{
    struct {
        uint32_t        type;
        uint32_t        sessionID;                /**< unused for static licenses. Must correspond to chip sessionID for live licenses */
        uint32_t        version;                  /**< license protocol version */
        uint32_t        secExtensionsMinVersion;/**< minimal version of upgradable security extensions - unused on H7 */
        uint8_t         rfu[8];
        uint8_t        providerPublicKey[64];  /**< public key for ECIES algorithm */
    }header;                                   /**< authenticated_header */
    RSS_FWKey_t	wrappedKey;                 /**< encrypted firmware key */
    uint8_t        tag[16];                /**< integrity tag of authenticated header and wrapped key */
}RSS_FWLicense_t;

/** Encrypted module image header.
* Contains information for RSS to unwrap FW key(s).
*/
typedef struct
{
    uint8_t     magic[8]; /**< magic value to represent image type (here SMI) */
    uint32_t    dest; /**< address where to flash decrypted module */
    uint32_t    moduleSize; /**< encrypted module size (in bytes)*/
    uint8_t     tag[16];  /**< integrity tag  */
    uint32_t    securityVersion[4]; /**< level of security / RSS version requested (RFU) */
}RSS_ModuleHeader_t;

/** Secure Area.
* Contains secure area parameters.
*/
typedef struct
{
    uint32_t sizeInBytes; /**< pass 0 for an empty secure area */
    uint32_t startAddress; /**< pass NULL for an empty secure area */
    uint32_t removeDuringBankErase; /**< if 0, keep area during bank/mass erase. else area will be removed*/
}RSS_SecureArea_t;

/** Get RSS status.
*
* \return version RSS version. Format is 0x00JJNNPP, with major number (JJ), minor number (NN) and patch number (PP)
*/
RSS_Version_t RSS_getVersion(void);

/** Get RSS status.
*
* \return status
* \retval RSS_OK          Last operation succeeded\n
*         RSS_ERROR       Last operation failed \n
*         RSS_DISABLED    RSS/Security not activated
*/
RSS_Status_t                RSS_getStatus(void);

/** Get Chip Unique Key certificate.
*
* Return address of certificate (works even if security is disabled)
*
* \return certificate address
*/
uint8_t*    RSS_getCertificate(void);

/** Get Chip Unique Key certificate size.
*
* Return size of certificate (works even if security is disabled)
*
* \return certificate size
*/
uint32_t    RSS_getCertificateSize(void);

/**
* \brief set interrupt vectors address
*
* if vector remapping is disabled, this is the only way to set/change vectors
*
* \param[in] address of vectors
*
*/
void        RSS_SetVectors(unsigned int vectors);

/** Initialize secure area(s).
*
* reset into RSS secure zone to setup secure area(s).
*
* \param[in] nbareas number of areas, 1 for one area in any bank, or 2 for 2 areas, one in each bank
* \param[in] area pointer on array to to one or two valid areas.
*
* \warning this function fails if there is already a secure area
* \warning this function does not return
*/
void RSS_resetAndInitializeSecureAreas(uint32_t nbAreas, RSS_SecureArea_t* areas);

/** securely exit from secure area.
*
* after exit, secure areas will not be accessible anymore
*
* \param[in] address of application vectors where to jump after exit
* \param[in] state of JTAG when exiting the secure area : RSS_ENABLE_JTAG_AT_EXIT when JTAG must be enabled, RSS_KEEP_JTAG_DISABLED_AT_EXIT when JTAG must be kept disabled
*
*/
void RSS_exitSecureArea(unsigned int vectors, unsigned int jtagState);

/** Destroy PCROP area.
*
* reset into RSS secure zone to overprogram contents of PCROP area and disable this area.
*
* \param[in] bank flash bank containing the area to destroy.
*
* \warning this function does not return
*/
void RSS_resetAndDestroyPCROPArea(RSS_FlashBank_et bank);

/** secure modules installation.
*
* reset into RSS secure zone to install modules into PCROP zone.
*
* \param[in] license FW license containing the encrypted module key
* \param[in] moduleHeader pointer to module header
* \param[in] encrypted_modules pointer on encrypted modules code
*
* \warning [\e dest,\e encrypted_modules_length] zone should be already erased
* \warning if [\e dest,\e encrypted_modules_length] zone lies outside current PCROP zone, PCROP zone should also be already erased
* \warning this function does not return
*/
void RSS_SMI_resetAndInstallModules(RSS_FWLicense_t* license,
RSS_ModuleHeader_t* moduleHeader,
unsigned char* encryptedModules);

#ifdef HW_IP_ENABLEMENT
/** IP enablement.
*
* reset into RSS secure zone to enable IPs.
*
* \param[in] activation token
*
*/
void RSS_resetAndActivateIPs(uint32_t address);
#endif /* HW_IP_ENABLEMENT */

typedef struct
{
  RSS_Version_t        (*getVersion)(void);
  uint32_t             reserved1 ;
  uint32_t             reserved2;
  uint32_t             reserved3;
  uint32_t             reserved4;
  void                 (*exitSecureArea)(unsigned int vectors, unsigned int jtagState);
  void                 (*resetAndInitializeSecureAreas)(uint32_t nbAreas, RSS_SecureArea_t* areas);
}RSS_API_Table_t;

#define RSS ((RSS_API_Table_t*)0x1FF09500)

/**
* @}
*/

#endif /* RSS_H */
