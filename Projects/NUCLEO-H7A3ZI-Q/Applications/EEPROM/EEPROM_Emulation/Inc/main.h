/**
  ******************************************************************************
  * @file    EEPROM/EEPROM_Emulation/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c
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
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_nucleo.h"
#include "eeprom.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FLASH_BASE_ADDR      (uint32_t)(FLASH_BASE)
#define FLASH_END_ADDR       (uint32_t)(0x081FFFFF)

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0_BANK1     ((uint32_t)0x08000000) /* Base @ of Sector 0, 8 Kbytes */
#define ADDR_FLASH_SECTOR_1_BANK1     ((uint32_t)0x08002000) /* Base @ of Sector 1, 8 Kbytes */
#define ADDR_FLASH_SECTOR_2_BANK1     ((uint32_t)0x08004000) /* Base @ of Sector 2, 8 Kbytes */
#define ADDR_FLASH_SECTOR_3_BANK1     ((uint32_t)0x08006000) /* Base @ of Sector 3, 8 Kbytes */
#define ADDR_FLASH_SECTOR_4_BANK1     ((uint32_t)0x08008000) /* Base @ of Sector 4, 8 Kbytes */
#define ADDR_FLASH_SECTOR_5_BANK1     ((uint32_t)0x0800A000) /* Base @ of Sector 5, 8 Kbytes */
#define ADDR_FLASH_SECTOR_6_BANK1     ((uint32_t)0x0800C000) /* Base @ of Sector 6, 8 Kbytes */
#define ADDR_FLASH_SECTOR_7_BANK1     ((uint32_t)0x0800E000) /* Base @ of Sector 7, 8 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_120_BANK2     ((uint32_t)0x081F0000) /* Base @ of Sector 120, 8 Kbytes */
#define ADDR_FLASH_SECTOR_121_BANK2     ((uint32_t)0x081F2000) /* Base @ of Sector 121, 8 Kbytes */
#define ADDR_FLASH_SECTOR_122_BANK2     ((uint32_t)0x081F4000) /* Base @ of Sector 122, 8 Kbytes */
#define ADDR_FLASH_SECTOR_123_BANK2     ((uint32_t)0x081F6000) /* Base @ of Sector 123, 8 Kbytes */
#define ADDR_FLASH_SECTOR_124_BANK2     ((uint32_t)0x081F8000) /* Base @ of Sector 124, 8 Kbytes */
#define ADDR_FLASH_SECTOR_125_BANK2     ((uint32_t)0x081FA000) /* Base @ of Sector 125, 8 Kbytes */
#define ADDR_FLASH_SECTOR_126_BANK2     ((uint32_t)0x081FC000) /* Base @ of Sector 126, 8 Kbytes */
#define ADDR_FLASH_SECTOR_127_BANK2     ((uint32_t)0x081FE000) /* Base @ of Sector 127, 8 Kbytes */

#endif /* MAIN_H */

