/**
  ******************************************************************************
  * @file    OTFDEC_ExecutingAesInstruction/Inc/macronix.h
  * @author  MCD Application Team
  * @brief   Specific part for Macronix memories
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
#ifndef __MACRONIX_H
#define __MACRONIX_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* MX25UM51245G Macronix memory */

/* Size of the flash */
#define OSPI_FLASH_SIZE             26U
#define OSPI_PAGE_SIZE              256U
#define OSPI_SECTOR_SIZE            4096U
#define OSPI_BLOCK_SIZE             65536U

/* Flash commands */
#define OCTAL_IO_DTR_READ_CMD       0xEE11U
#define OCTAL_IO_READ_CMD           0xEC13U
#define OCTAL_PAGE_PROG_CMD         0x12EDU
#define OCTAL_READ_STATUS_REG_CMD   0x05FAU
#define OCTAL_READ_CFG_REG_CMD      0x15EAU
#define OCTAL_READ_CFG_REG_2_CMD    0x718EU
#define OCTAL_SECTOR_ERASE_CMD      0x21DEU
#define OCTAL_BLOCK_ERASE_CMD       0xDC23U
#define OCTAL_WRITE_ENABLE_CMD      0x06F9U
#define READ_STATUS_REG_CMD         0x05U
#define READ_CFG_REG_CMD            0x15U
#define READ_CFG_REG_2_CMD          0x71U
#define WRITE_CFG_REG_2_CMD         0x72U
#define WRITE_ENABLE_CMD            0x06U
#define SECTOR_ERASE_CMD            0x20U
#define PAGE_PROG_CMD               0x02U
#define READ_CMD                    0x03U

/* Dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ_OCTAL   	NB_DUMMY_CYCLE

/* Auto-polling values */
#define WRITE_ENABLE_MATCH_VALUE    0x02U
#define WRITE_ENABLE_MASK_VALUE     0x02U

#define MEMORY_READY_MATCH_VALUE    0x00U
#define MEMORY_READY_MASK_VALUE     0x01U

#define AUTO_POLLING_INTERVAL       0x10U

/* Memory registers address */
#define CONFIG_REG2_ADDR1           0x0000000UL
#define CR2_DTR_OPI_ENABLE          0x02U
#define CR2_STR_OPI_ENABLE          0x01U

#define CONFIG_REG2_ADDR2           0x00000200UL
#define CR2_DQS_PRE_CYCLE			      0x01U

#define CONFIG_REG2_ADDR3           0x00000300UL
#define CR2_DUMMY_CYCLES_66MHZ      0x07U
#define CR2_DUMMY_CYCLES_84MHZ      0x06U
#define CR2_DUMMY_CYCLES_104MHZ     0x05U
#define CR2_DUMMY_CYCLES_133MHZ     0x03U
#define CR2_DUMMY_CYCLES_166MHZ     0x02U
#define CR2_DUMMY_CYCLES_200MHZ     0x00U

/* Memory delay */
#define MEMORY_REG_WRITE_DELAY      40U
#define MEMORY_PAGE_PROG_DELAY      2U

/* Read registers */
#define STATUS_REGISTER							0x00U
#define CFG_REGISTER								0x01U
#define CFG2_REGISTER1							0x02U
#define CFG2_REGISTER2							0x03U
#define CFG2_REGISTER3							0x04U

/* Exported functions ------------------------------------------------------- */

#endif /* __MACRONIX_H */

