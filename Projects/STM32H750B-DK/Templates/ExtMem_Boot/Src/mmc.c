/**
  ******************************************************************************
  * @file    mmc.c
  * @author  MCD Application Team
  * @brief   This file includes the MMC supported STM32H750B-DISCO evaluation
             board.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - ToBeUpdated :  MMC  mounted on STM32H750B Discovery board.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "memory.h"
#include "memory_msp.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* MMC transfer state definition */     
#define MMC_TRANSFER_OK                ((uint8_t)0x00)
#define MMC_TRANSFER_BUSY              ((uint8_t)0x01)
#define MMC_DATATIMEOUT                ((uint32_t)0xFFFFFFFFU)

#if (BINARY_AREA == USE_MMC)

/* Private variables ---------------------------------------------------------*/
MMC_HandleTypeDef uSdHandle;
__IO uint32_t MMCWriteStatus = 0, MMCReadStatus = 0;
/* Private function prototypes -----------------------------------------------*/
uint8_t MMC_ReadBlocks(uint32_t *, uint32_t , uint32_t , uint32_t );
uint8_t MMC_DeInit(void);
uint8_t MMC_GetCardState(void);
uint8_t MMC_Erase(uint32_t , uint32_t );
uint8_t MMC_WriteBlocks(uint32_t *, uint32_t , uint32_t , uint32_t );
void MMC_GetCardInfo(HAL_MMC_CardInfoTypeDef *);

/**
  * @brief  Initialize the SDCARD through the FATFS, copy binary to its execution
  *         area and finaly free no more needed ressources.
  * @param  None
  * @retval None
  */
uint32_t MMC_Startup(void)
{   
 uint8_t MMC_state = MEMORY_OK;
  
  /* uMMC device interface configuration */
  uSdHandle.Instance = SDMMC1;

  uSdHandle.Init.ClockDiv            = 4;
  uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_8B;
  
  /* Msp MMC initialization */
  MMC_MspInit();

  /* HAL MMC initialization */
  if(HAL_MMC_Init(&uSdHandle) != HAL_OK)
  {
    MMC_state = MEMORY_ERROR;
  }
  
  if(MMC_state != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  while(MMC_GetCardState() != MMC_TRANSFER_OK)
  {
  }

  MMC_state = MMC_ReadBlocks((uint32_t *) APPLICATION_ADDRESS , BINARY_ADDRESS , (BINARY_SIZE/512) +1 , MMC_DATATIMEOUT);

  /* Wait until MMC cards are ready to use for new operation */
  while(MMC_GetCardState() != MMC_TRANSFER_OK)
  {
  }
  if(MMC_state != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
  
  return MMC_state;
}

/**
  * @brief  DeInitializes the MMC card device.
  * @param  None
  * @retval MMC status
  */
uint8_t MMC_DeInit(void)
{ 
  uint8_t mmc_state = MEMORY_OK;
 
  uSdHandle.Instance = SDMMC1;
  
  /* HAL MMC deinitialization */
  if(HAL_MMC_DeInit(&uSdHandle) != HAL_OK)
  {
    mmc_state = MEMORY_ERROR;
  }

  /* Msp MMC deinitialization */
  uSdHandle.Instance = SDMMC1;
  MMC_MspDeInit();
  
  return  mmc_state;
}

/**
  * @brief  Reads block(s) from a specified address in an MMC card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  NumOfBlocks: Number of MMC blocks to read
  * @param  Timeout: Timeout for read operation
  * @retval MMC status
  */
uint8_t MMC_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  
  if( HAL_MMC_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) == HAL_OK)
  {
    return MEMORY_OK;
  }
  else
  {
    return MEMORY_ERROR;
  }

}

/**
  * @brief  Writes block(s) to a specified address in an MMC card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of MMC blocks to write
  * @param  Timeout: Timeout for write operation
  * @retval MMC status
  */
uint8_t MMC_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
 
  if( HAL_MMC_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) == HAL_OK)
  {
    return MEMORY_OK;
  }
  else
  {
    return MEMORY_ERROR;
  }
}

/**
  * @brief  Erases the specified memory area of the given MMC card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval MMC status
  */
uint8_t MMC_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  
  if( HAL_MMC_Erase(&uSdHandle, StartAddr, EndAddr) == HAL_OK)
  {
    return MEMORY_OK;
  }
  else
  {
    return MEMORY_ERROR;
  }
}

/**
  * @brief  Gets the current MMC card data status.
  * @param  None
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  MMC_TRANSFER_OK: No data transfer is acting
  *            @arg  MMC_TRANSFER_BUSY: Data transfer is acting
  *            @arg  MMC_TRANSFER_ERROR: Data transfer error 
  */
uint8_t MMC_GetCardState(void)
{
  return((HAL_MMC_GetCardState(&uSdHandle) == HAL_MMC_CARD_TRANSFER ) ? MMC_TRANSFER_OK : MMC_TRANSFER_BUSY);
}


/**
  * @brief  Get MMC information about specific MMC card.
  * @param  CardInfo: Pointer to HAL_MMC_CardInfoTypedef structure
  * @retval None 
  */
void MMC_GetCardInfo(HAL_MMC_CardInfoTypeDef *CardInfo)
{
  HAL_MMC_GetCardInfo(&uSdHandle, CardInfo);
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hmmc: MMC handle
  * @retval None
  */
void HAL_MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc)
{
    MMCWriteStatus = 1;
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hmmc: MMC handle
  * @retval None
  */
void HAL_MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc)
{
  MMCReadStatus = 1;
}


#endif /* (BINARY_AREA == USE_MMC) */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

