/**
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/Inc/flash_if.h
  * @author  MCD Application Team
  * @brief   Header file for flash_if.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
   
/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Define the flash memory start address */
#define USER_FLASH_STARTADDRESS       ((uint32_t)0x08000000) 

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x0801FFFF is reserved for the Firmware upgrade code */
#define APPLICATION_ADDRESS           (uint32_t)0x08020000

/* Last Page Address */
#define USER_FLASH_LAST_PAGE_ADDRESS  (uint32_t)0x081FFFFF

/* Define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_LAST_PAGE_ADDRESS - APPLICATION_ADDRESS + 1)

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0_BANK1     ((uint32_t)0x08000000) /* Base @ of Sector 0, Bank1, 128 Kbyte */
#define ADDR_FLASH_SECTOR_1_BANK1     ((uint32_t)0x08020000) /* Base @ of Sector 1, Bank1, 128 Kbyte */
#define ADDR_FLASH_SECTOR_2_BANK1     ((uint32_t)0x08040000) /* Base @ of Sector 2, Bank1, 128 Kbyte */
#define ADDR_FLASH_SECTOR_3_BANK1     ((uint32_t)0x08060000) /* Base @ of Sector 3, Bank1, 128 Kbyte */
#define ADDR_FLASH_SECTOR_4_BANK1     ((uint32_t)0x08080000) /* Base @ of Sector 4, Bank1, 128 Kbyte */
#define ADDR_FLASH_SECTOR_5_BANK1     ((uint32_t)0x080A0000) /* Base @ of Sector 5, Bank1, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6_BANK1     ((uint32_t)0x080C0000) /* Base @ of Sector 6, Bank1, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7_BANK1     ((uint32_t)0x080E0000) /* Base @ of Sector 7, Bank1, 128 Kbyte */

#define ADDR_FLASH_SECTOR_0_BANK2     ((uint32_t)0x08100000) /* Base @ of Sector 0, Bank2, 128 Kbyte */
#define ADDR_FLASH_SECTOR_1_BANK2     ((uint32_t)0x08120000) /* Base @ of Sector 1, Bank2, 128 Kbyte */
#define ADDR_FLASH_SECTOR_2_BANK2     ((uint32_t)0x08140000) /* Base @ of Sector 2, Bank2, 128 Kbyte */
#define ADDR_FLASH_SECTOR_3_BANK2     ((uint32_t)0x08160000) /* Base @ of Sector 3, Bank2, 128 Kbyte */
#define ADDR_FLASH_SECTOR_4_BANK2     ((uint32_t)0x08180000) /* Base @ of Sector 4, Bank2, 128 Kbyte */
#define ADDR_FLASH_SECTOR_5_BANK2     ((uint32_t)0x081A0000) /* Base @ of Sector 5, Bank2, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6_BANK2     ((uint32_t)0x081C0000) /* Base @ of Sector 6, Bank2, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7_BANK2     ((uint32_t)0x081E0000) /* Base @ of Sector 7, Bank2, 128 Kbyte */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void FLASH_If_FlashUnlock(void);
FlagStatus FLASH_If_ReadOutProtectionStatus(void);
uint32_t FLASH_If_EraseSectors(uint32_t Address);
uint32_t FLASH_If_Write(uint32_t FlashAddress, uint32_t* Data ,uint32_t DataLength);

#ifdef __cplusplus
}
#endif

#endif  /* __FLASH_IF_H */

