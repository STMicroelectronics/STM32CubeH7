/**
  ******************************************************************************
  * @file    macronix.h
  * @author  MCD Application Team
  * @brief   Specific part for Macronix memories
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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

/* Read Operations */
#define READ_CMD                                0x03
#define READ_4_BYTE_ADDR_CMD                    0x13

#define FAST_READ_CMD                           0x0B
#define FAST_READ_4_BYTE_ADDR_CMD               0x0C

#define OCTAL_IO_READ_CMD                       0xEC13
#define OCTAL_IO_DTR_READ_CMD                   0xEE11

/* Program Operations */
#define PAGE_PROG_CMD                           0x02
#define PAGE_PROG_4_BYTE_ADDR_CMD               0x12

#define OCTAL_PAGE_PROG_CMD                     0x12ED

/* Erase Operations */
#define SECTOR_ERASE_CMD                        0x20
#define SECTOR_ERASE_4_BYTE_ADDR_CMD            0x21

#define OCTAL_SECTOR_ERASE_CMD                  0x21DE

#define BLOCK_ERASE_CMD                         0xD8
#define BLOCK_ERASE_4_BYTE_ADDR_CMD             0xDC

#define OCTALBLOCK_ERASE_CMD                    0xDC23

#define CHIP_ERASE_CMD                          0xC7
#define CHIP_ERASE_CMD_2                        0x60

#define OCTAL_CHIP_ERASE_CMD                    0x609F
#define OCTAL_CHIP_ERASE_CMD_2                  0xC738

#define PROG_ERASE_RESUME_CMD                   0x30
#define PROG_ERASE_SUSPEND_CMD                  0xB0

#define OCTAL_PROG_ERASE_RESUME_CMD             0x30CF
#define OCTAL_PROG_ERASE_SUSPEND_CMD            0xB04F

/* Write Operations */
#define WRITE_ENABLE_CMD                        0x06
#define WRITE_DISABLE_CMD                       0x04

#define OCTAL_WRITE_ENABLE_CMD                  0x06F9
#define OCTAL_WRITE_DISABLE_CMD                 0x04FB

#define DTR_OCTAL_WRITE_ENABLE_CMD              0xF906

/* Deep power down */
#define ENTER_DEEP_POWER_DOWN_CMD               0xB9
#define RELEASE_DEEP_POWER_DOWN_CMD             0xAB

#define OCTAL_ENTER_DEEP_POWER_DOWN_CMD         0xB946
#define OCTAL_RELEASE_DEEP_POWER_DOWN_CMD       0xAB54

/* Burst length */
#define SET_BURST_LENGTH_CMD                    0xC0

#define OCTAL_SET_BURST_LENGTH_CMD              0xC03F

/* One-Time Programmable Operations */
#define ENTER_SECURED_OTP_CMD                   0xB1
#define EXIT_SECURED_OTP_CMD                    0xC1

#define OCTAL_ENTER_SECURED_OTP_CMD             0xB14E
#define OCTAL_EXIT_SECURED_OTP_CMD              0xC13E

/* Reset Operations */
#define NO_OPERATION_CMD                        0x00
#define RESET_ENABLE_CMD                        0x66
#define RESET_MEMORY_CMD                        0x99

#define OCTAL_NO_OPERATION_CMD                  0x00FF
#define OCTAL_RESET_ENABLE_CMD                  0x6699
#define OCTAL_RESET_MEMORY_CMD                  0x9966

/* Identification Operations */
#define READ_ID_CMD                             0x9F
#define READ_SERIAL_FLASH_DISCO_PARAM_CMD       0x5A

#define OCTAL_READ_ID_CMD                       0x9F60
#define OCTAL_READ_SERIAL_FLASH_DISCO_PARAM_CMD 0x5AA5

/* Register Operations */
#define READ_STATUS_REG_CMD                     0x05
#define READ_CFG_REG_CMD                        0x15
#define WRITE_STATUS_CFG_REG_CMD                0x01

#define OCTAL_READ_STATUS_REG_CMD               0x05FA
#define OCTAL_READ_CFG_REG_CMD                  0x15EA
#define OCTAL_WRITE_STATUS_CFG_REG_CMD          0x01FE

#define DTR_OCTAL_READ_STATUS_REG_CMD           0xFA05

#define READ_CFG_REG_2_CMD                      0x71
#define WRITE_CFG_REG_2_CMD                     0x72

#define OCTAL_READ_CFG_REG_2_CMD                0x718E
#define OCTAL_WRITE_CFG_REG_2_CMD               0x728D

#define READ_FAST_BOOT_REG_CMD                  0x16
#define WRITE_FAST_BOOT_REG                     0x17
#define ERASE_FAST_BOOT_REG                     0x18

#define OCTAL_READ_FAST_BOOT_REG_CMD            0x16E9
#define OCTAL_WRITE_FAST_BOOT_REG               0x17E8
#define OCTAL_ERASE_FAST_BOOT_REG               0x18E7

#define READ_SECURITY_REG_CMD                   0x2B
#define WRITE_SECURITY_REG_CMD                  0x2F

#define OCTAL_READ_SECURITY_REG_CMD             0x2BD4
#define OCTAL_WRITE_SECURITY_REG_CMD            0x2FD0

#define READ_LOCK_REG_CMD                       0x2D
#define WRITE_LOCK_REG_CMD                      0x2C

#define OCTAL_READ_LOCK_REG_CMD                 0x2DD2
#define OCTAL_WRITE_LOCK_REG_CMD                0x2CD3

#define READ_SPB_STATUS_CMD                     0xE2
#define PROG_SPB_BIT_CMD                        0xE3
#define ERASE_ALL_SPB_BIT_CMD                   0xE4

#define OCTAL_READ_SPB_STATUS_CMD               0xE21D
#define OCTAL_PROG_SPB_BIT_CMD                  0xE31C
#define OCTAL_ERASE_ALL_SPB_BIT_CMD             0xE41B

#define READ_DPB_REG_CMD                        0xE0
#define WRITE_DPB_REG_CMD                       0xE1

#define OCTAL_READ_DPB_REG_CMD                  0xE01F
#define OCTAL_WRITE_DPB_REG_CMD                 0xE11E

/* Protection Operations */
#define WRITE_PROTECT_SEL_CMD                   0x68

#define OCTAL_WRITE_PROTECT_SEL_CMD             0x6897

#define GANG_BLOCK_LOCK_CMD                     0x7E
#define GANG_BLOCK_UNLOCK_CMD                   0x98

#define OCTAL_GANG_BLOCK_LOCK_CMD               0x7E81
#define OCTAL_GANG_BLOCK_UNLOCK_CMD             0x9867

/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ                 8
#define DUMMY_CLOCK_CYCLES_READ_OCTAL           20
#define DUMMY_CLOCK_CYCLES_READ_OCTAL_DTR       10

/* Values for the memory ready register */
#define MEMORY_READY_MATCH_VALUE                0x00
#define MEMORY_READY_MASK_VALUE                 0x01
#define MEMORY_READY_INSTRUCTION                READ_STATUS_REG_CMD

#define MEMORY_READY_ALTERNATE_MATCH_VALUE      MEMORY_READY_MATCH_VALUE
#define MEMORY_READY_ALTERNATE_MASK_VALUE       MEMORY_READY_MASK_VALUE
#define MEMORY_READY_ALTERNATE_INSTRUCTION      MEMORY_READY_INSTRUCTION

#define MEMORY_READY_DUAL_FLASH_MATCH_VALUE     0x0000
#define MEMORY_READY_DUAL_FLASH_MASK_VALUE      0x0101
#define MEMORY_READY_DUAL_FLASH_INSTRUCTION     READ_STATUS_REG_CMD

#define OSPI_ALTERNATE_BYTE_PATTERN             0x00

/* Exported functions ------------------------------------------------------- */

#endif /* __MACRONIX_H */

