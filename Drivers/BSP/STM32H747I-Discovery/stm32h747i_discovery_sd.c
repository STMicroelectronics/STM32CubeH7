/**
  ******************************************************************************
  * @file    stm32h747i_discovery_sd.c
  * @author  MCD Application Team
  * @brief   This file includes the uSD card driver mounted on STM32H747I-DISCOVERY
  *          board.
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

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive the micro SD external card mounted on STM32412G-DISCOVERY
     board.
   - This driver does not need a specific component driver for the micro SD device
     to be included with.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the micro SD card using the BSP_SD_Init() function. This
       function includes the MSP layer hardware resources initialization and the
       SDIO interface configuration to interface with the external micro SD. It
       also includes the micro SD initialization sequence.
     o To check the SD card presence you can use the function BSP_SD_IsDetected() which
       returns the detection status
     o If SD presence detection interrupt mode is desired, you must configure the
       SD detection interrupt mode by calling the function BSP_SD_ITConfig(). The interrupt
       is generated as an external interrupt whenever the micro SD card is
       plugged/unplugged in/from the board.
     o The function BSP_SD_GetCardInfo() is used to get the micro SD card information
       which is stored in the structure "HAL_SD_CardInfoTypedef".

  + Micro SD card operations
     o The micro SD card can be accessed with read/write block(s) operations once
       it is ready for access. The access can be performed whether using the polling
       mode by calling the functions BSP_SD_ReadBlocks()/BSP_SD_WriteBlocks(), or by DMA
       transfer using the functions BSP_SD_ReadBlocks_DMA()/BSP_SD_WriteBlocks_DMA()
     o The DMA transfer complete is used with interrupt mode. Once the SD transfer
       is complete, the SD interrupt is handled using the function BSP_SD_IRQHandler(),
       the DMA Tx/Rx transfer complete are handled using the functions
       SD_DMA_Tx_IRQHandler()/SD_DMA_Rx_IRQHandler() that should be defined by user.
       The corresponding user callbacks are implemented by the user at application level.
     o The SD erase block(s) is performed using the function BSP_SD_Erase() with specifying
       the number of blocks to erase.
     o The SD runtime status is returned when calling the function BSP_SD_GetStatus().

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32h747i_discovery_sd.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_DISCOVERY
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY_SD STM32H747I_DISCOVERY_SD
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY_SD_Exported_Variables Exported Variables
  * @{
  */
SD_HandleTypeDef uSdHandle;
/**
  * @}
  */

/** @defgroup STM32H747I_DISCOVERY_SD_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Initializes the SD card device.
  * @retval SD status
  */
uint8_t BSP_SD_Init(void)
{
  uint8_t sd_state = MSD_OK;

  /* uSD device interface configuration */
  uSdHandle.Instance = SDMMC1;

  /* if CLKDIV = 0 then SDMMC Clock frequency = SDMMC Kernel Clock
     else SDMMC Clock frequency = SDMMC Kernel Clock / [2 * CLKDIV].
  */
#if ! defined (BSP_SD_HIGH_PERFORMANCE_CONFIG)
  uSdHandle.Init.ClockDiv            = 4;
#else
  /* Code for high performance */
  uSdHandle.Init.ClockDiv            = 2;
#endif /* BSP_SD_HIGH_PERFORMANCE_CONFIG  */
  uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;

  /* Msp SD initialization */
  BSP_SD_MspInit(&uSdHandle, NULL);

  /* Check if SD card is present */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    BSP_SD_MspDeInit(&uSdHandle, NULL);
    return MSD_ERROR_SD_NOT_PRESENT;
  }

  /* HAL SD initialization */
  if(HAL_SD_Init(&uSdHandle) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return  sd_state;
}

/**
  * @brief  DeInitializes the SD card device.
  * @retval SD status
  */
uint8_t BSP_SD_DeInit(void)
{
  uint8_t sd_state = MSD_OK;

  uSdHandle.Instance = SDMMC1;

  /* HAL SD deinitialization */
  if(HAL_SD_DeInit(&uSdHandle) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  /* Msp SD deinitialization */
  uSdHandle.Instance = SDMMC1;
  BSP_SD_MspDeInit(&uSdHandle, NULL);

  return  sd_state;
}

/**
  * @brief  Configures Interrupt mode for SD1 detection pin.
  * @retval Returns 0
  */
uint8_t BSP_SD_ITConfig(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Configure Interrupt mode for SD detection pin */
  gpio_init_structure.Pin = SD_DETECT_PIN;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Mode = GPIO_MODE_IT_RISING_FALLING;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpio_init_structure);

  /* Enable and set SD detect EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)(SD_DETECT_EXTI_IRQn), 0x0F, 0x00);
  HAL_NVIC_EnableIRQ((IRQn_Type)(SD_DETECT_EXTI_IRQn));

  return MSD_OK;
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_SD_IsDetected(void)
{
  __IO uint8_t status = SD_PRESENT;


  /* Check SD card detect pin */
  if (HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) == GPIO_PIN_SET)
  {
    status = SD_NOT_PRESENT;
  }

  return status;
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  NumOfBlocks: Number of SD blocks to read
  * @param  Timeout: Timeout for read operation
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{

  if( HAL_SD_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }

}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write
  * @param  Timeout: Timeout for write operation
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{

  if( HAL_SD_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  NumOfBlocks: Number of SD blocks to read
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{

  if( HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks) == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{

  if( HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks) == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }

}

/**
  * @brief  Erases the specified memory area of the given SD card.
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{

  if( HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) == HAL_OK)
  {
    return MSD_OK;
  }
  else
  {
    return MSD_ERROR;
  }
}

/**
  * @brief  Initializes the SD MSP.
  * @param  hsd SD handle
  * @param  Params User parameters
  * @retval None
  */
__weak void BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params)
{
  /* __weak function can be modified by the application */

  GPIO_InitTypeDef gpio_init_structure;

  /* SD pins are in conflict with Camera pins on the Disco board
	   therefore Camera must be power down before using the BSP SD
	   To power down the camera , Set GPIOJ pin 14 to high
	*/

  /* Enable GPIO J clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  gpio_init_structure.Pin       = GPIO_PIN_14;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

  /* Set the camera POWER_DOWN pin (active high) */
  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14, GPIO_PIN_SET);

  /* Enable SDIO clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();


  /* Enable GPIOs clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_SDIO1;

  /* SDMMC GPIO CLKIN PB8, D0 PC8, D1 PC9, D2 PC10, D3 PC11, CK PC12, CMD PD2 */
  /* GPIOC configuration */
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* Configure Input mode for SD detection pin */
  SD_DETECT_GPIO_CLK_ENABLE();
  gpio_init_structure.Pin = SD_DETECT_PIN;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpio_init_structure);

  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SDMMC1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDMMC1_IRQn);

}

/**
  * @brief  DeInitializes the SD MSP.
  * @param  hsd SD handle
  * @param  Params User parameters
  * @retval None
  */
__weak void BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params)
{
    /* Disable NVIC for SDIO interrupts */
    HAL_NVIC_DisableIRQ(SDMMC1_IRQn);

    /* DeInit GPIO pins can be done in the application
       (by surcharging this __weak function) */

    /* Disable SDMMC1 clock */
    __HAL_RCC_SDMMC1_CLK_DISABLE();

    /* GPIO pins clock and DMA clocks can be shut down in the application
       by surcharging this __weak function */
}

/**
  * @brief  Handles SD card interrupt request.
  * @retval None
  */
void BSP_SD_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle);
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  *            @arg  SD_TRANSFER_ERROR: Data transfer error
  */
uint8_t BSP_SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  * @retval None
  */
void BSP_SD_GetCardInfo(BSP_SD_CardInfo *CardInfo)
{
  HAL_SD_GetCardInfo(&uSdHandle, CardInfo);
}

/**
  * @brief SD Abort callbacks
  * @param hsd SD handle
  * @retval None
  */
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_AbortCallback();
}


/**
  * @brief Tx Transfer completed callbacks
  * @param hsd SD handle
  * @retval None
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_WriteCpltCallback();
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd SD handle
  * @retval None
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_ReadCpltCallback();
}

/**
  * @brief BSP SD Abort callbacks
  * @retval None
  */
__weak void BSP_SD_AbortCallback(void)
{

}

/**
  * @brief BSP Tx Transfer completed callbacks
  * @retval None
  */
__weak void BSP_SD_WriteCpltCallback(void)
{

}

/**
  * @brief BSP Rx Transfer completed callbacks
  * @retval None
  */
__weak void BSP_SD_ReadCpltCallback(void)
{

}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
