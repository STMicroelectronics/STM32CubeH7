/**
  ******************************************************************************
  * @file    USB_Device/DFU_Standalone/Src/usbd_dfu_flash.c
  * @author  MCD Application Team
  * @brief   Memory management layer
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

/* Includes ------------------------------------------------------------------ */
#include "usbd_dfu_flash.h"
#include "stm32h7xx_hal_conf.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/01*128Ka,7*128Kg"
#define FLASH_ERASE_TIME    (uint16_t)50
#define FLASH_PROGRAM_TIME  (uint16_t)50

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
static uint32_t GetSector(uint32_t Address);

/* Extern function prototypes ------------------------------------------------ */
uint16_t Flash_If_Init(void);
uint16_t Flash_If_Erase(uint32_t Add);
uint16_t Flash_If_Write(uint8_t * src, uint8_t * dest, uint32_t Len);
uint8_t *Flash_If_Read(uint8_t * src, uint8_t * dest, uint32_t Len);
uint16_t Flash_If_DeInit(void);
uint16_t Flash_If_GetStatus(uint32_t Add, uint8_t Cmd, uint8_t * buffer);

USBD_DFU_MediaTypeDef USBD_DFU_Flash_fops = {
  (uint8_t *) FLASH_DESC_STR,
  Flash_If_Init,
  Flash_If_DeInit,
  Flash_If_Erase,
  Flash_If_Write,
  Flash_If_Read,
  Flash_If_GetStatus,
};

/* Private functions --------------------------------------------------------- */
/**
  * @brief  Initializes Memory.
  * @param  None
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t Flash_If_Init(void)
{
  /* Unlock the internal flash */
  HAL_FLASH_Unlock();
  return 0;
}

/**
  * @brief  De-Initializes Memory.
  * @param  None
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t Flash_If_DeInit(void)
{
  /* Lock the internal flash */
  HAL_FLASH_Lock();
  return 0;
}

/**
  * @brief  Erases sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t Flash_If_Erase(uint32_t Add)
{
  uint32_t startsector = 0, sectorerror = 0;

  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;

  /* Get the number of sector */
  startsector = GetSector(Add);
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  eraseinitstruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  eraseinitstruct.Banks = FLASH_BANK_1;
  eraseinitstruct.Sector = startsector;
  eraseinitstruct.NbSectors = 1;

  status = HAL_FLASHEx_Erase(&eraseinitstruct, &sectorerror);

  if (status != HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Writes Data into Memory.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval 0 if operation is successful, MAL_FAIL else.
  */

uint16_t Flash_If_Write(uint8_t * src, uint8_t * dest, uint32_t Len)
{
  uint32_t i = 0;

  for (i = 0; i < Len; i += 32)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
     * be done by byte */
    if (HAL_FLASH_Program
        (FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t) (dest + i),
         (uint32_t) (src + i)) == HAL_OK)
    {
      /* Check the written value */
      if (*(uint64_t *) (src + i) != *(uint64_t *) (dest + i))
      {
        /* Flash content doesn't match SRAM content */
        return 2;
      }
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return 1;
    }
  }
  return 0;
}

/**
  * @brief  Reads Data into Memory.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *Flash_If_Read(uint8_t * src, uint8_t * dest, uint32_t Len)
{
  uint32_t i = 0;
  uint8_t *psrc = src;

  for (i = 0; i < Len; i++)
  {
    dest[i] = *psrc++;
  }
  /* Return a valid address to avoid HardFault */
  return (uint8_t *) (dest);
}

/**
  * @brief  Gets Memory Status.
  * @param  Add: Address to be read from.
  * @param  Cmd: Number of data to be read (in bytes).
  * @retval 0 if operation is successful
  */
uint16_t Flash_If_GetStatus(uint32_t Add, uint8_t Cmd, uint8_t * buffer)
{
  switch (Cmd)
  {
  case DFU_MEDIA_PROGRAM:
    buffer[1] = (uint8_t) FLASH_PROGRAM_TIME;
    buffer[2] = (uint8_t) (FLASH_PROGRAM_TIME << 8);
    buffer[3] = 0;
    break;

  case DFU_MEDIA_ERASE:
  default:
    buffer[1] = (uint8_t) FLASH_ERASE_TIME;
    buffer[2] = (uint8_t) (FLASH_ERASE_TIME << 8);
    buffer[3] = 0;
    break;
  }
  return 0;
}

/**
  * @brief  Gets the sector of a given address
  * @param  Address Address of the FLASH Memory
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if (((Address < ADDR_FLASH_SECTOR_1_BANK1) &&
       (Address >= ADDR_FLASH_SECTOR_0_BANK1)) ||
      ((Address < ADDR_FLASH_SECTOR_1_BANK2) &&
       (Address >= ADDR_FLASH_SECTOR_0_BANK2)))
  {
    sector = FLASH_SECTOR_0;
  }
  else
    if (((Address < ADDR_FLASH_SECTOR_2_BANK1) &&
         (Address >= ADDR_FLASH_SECTOR_1_BANK1)) ||
        ((Address < ADDR_FLASH_SECTOR_2_BANK2) &&
         (Address >= ADDR_FLASH_SECTOR_1_BANK2)))
  {
    sector = FLASH_SECTOR_1;
  }
  else
    if (((Address < ADDR_FLASH_SECTOR_3_BANK1) &&
         (Address >= ADDR_FLASH_SECTOR_2_BANK1)) ||
        ((Address < ADDR_FLASH_SECTOR_3_BANK2) &&
         (Address >= ADDR_FLASH_SECTOR_2_BANK2)))
  {
    sector = FLASH_SECTOR_2;
  }
  else
    if (((Address < ADDR_FLASH_SECTOR_4_BANK1) &&
         (Address >= ADDR_FLASH_SECTOR_3_BANK1)) ||
        ((Address < ADDR_FLASH_SECTOR_4_BANK2) &&
         (Address >= ADDR_FLASH_SECTOR_3_BANK2)))
  {
    sector = FLASH_SECTOR_3;
  }
  else
    if (((Address < ADDR_FLASH_SECTOR_5_BANK1) &&
         (Address >= ADDR_FLASH_SECTOR_4_BANK1)) ||
        ((Address < ADDR_FLASH_SECTOR_5_BANK2) &&
         (Address >= ADDR_FLASH_SECTOR_4_BANK2)))
  {
    sector = FLASH_SECTOR_4;
  }
  else
    if (((Address < ADDR_FLASH_SECTOR_6_BANK1) &&
         (Address >= ADDR_FLASH_SECTOR_5_BANK1)) ||
        ((Address < ADDR_FLASH_SECTOR_6_BANK2) &&
         (Address >= ADDR_FLASH_SECTOR_5_BANK2)))
  {
    sector = FLASH_SECTOR_5;
  }
  else
    if (((Address < ADDR_FLASH_SECTOR_7_BANK1) &&
         (Address >= ADDR_FLASH_SECTOR_6_BANK1)) ||
        ((Address < ADDR_FLASH_SECTOR_7_BANK2) &&
         (Address >= ADDR_FLASH_SECTOR_6_BANK2)))
  {
    sector = FLASH_SECTOR_6;
  }
  else
    if (((Address < ADDR_FLASH_SECTOR_0_BANK2) &&
         (Address >= ADDR_FLASH_SECTOR_7_BANK1)) || ((Address < FLASH_END_ADDR)
                                                     && (Address >=
                                                         ADDR_FLASH_SECTOR_7_BANK2)))
  {
    sector = FLASH_SECTOR_7;
  }
  else
  {
    sector = FLASH_SECTOR_7;
  }

  return sector;
}

