/**
******************************************************************************
* @file    OCTOSPI/OCTOSPI_Flash_MemoryMapped_DTR/Src/stm32h7xx_hal_msp.c
* @author  MCD Application Team
* @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Examples
* @{
*/

/** @addtogroup Templates
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
* @{
*/

/**
* @brief OSPI MSP Initialization
*        This function configures the hardware resources used in this example:
*           - Peripheral's clock enable
*           - Peripheral's GPIO Configuration
*           - NVIC configuration for OSPI interrupts
* @param hospi: OSPI handle pointer
* @retval None
*/
void HAL_MspInit(void)
{
GPIO_InitTypeDef GPIO_InitStruct;


GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
GPIO_InitStruct.Pull      = GPIO_NOPULL;

/* Enable the OctoSPI memory interface clock */
OSPI1_CLK_ENABLE();

/* Reset the OctoSPI memory interface */
__HAL_RCC_OSPI1_FORCE_RESET();
__HAL_RCC_OSPI1_RELEASE_RESET();

/* Enable the GPIO clocks */
OSPI1_CS_GPIO_CLK_ENABLE();
OSPI1_CLK_P_GPIO_CLK_ENABLE();
OSPI1_D0_GPIO_CLK_ENABLE();
OSPI1_D1_GPIO_CLK_ENABLE();
OSPI1_D2_GPIO_CLK_ENABLE();
OSPI1_D3_GPIO_CLK_ENABLE();
OSPI1_D4_GPIO_CLK_ENABLE();
OSPI1_D5_GPIO_CLK_ENABLE();
OSPI1_D6_GPIO_CLK_ENABLE();
OSPI1_D7_GPIO_CLK_ENABLE();
OSPI1_DQS_GPIO_CLK_ENABLE();

/* OSPI CS GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_CS_PIN;
GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
GPIO_InitStruct.Pull      = GPIO_PULLUP;
GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_CS_GPIO_PORT, &GPIO_InitStruct);

/* OSPI CLK GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_CLK_PIN;
GPIO_InitStruct.Pull      = GPIO_NOPULL;
GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_CLK_GPIO_PORT, &GPIO_InitStruct);

/* OSPI D0 GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_D0_PIN;
GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_D0_GPIO_PORT, &GPIO_InitStruct);

/* OSPI D1 GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_D1_PIN;
GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_D1_GPIO_PORT, &GPIO_InitStruct);

/* OSPI1 D2 GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_D2_PIN;
GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_D2_GPIO_PORT, &GPIO_InitStruct);

/* OSPI1 D3 GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_D3_PIN;
GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_D3_GPIO_PORT, &GPIO_InitStruct);

/* OSPI1 D4 GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_D4_PIN;
GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_D4_GPIO_PORT, &GPIO_InitStruct);

/* OSPI1 D5 GPIO pin configuration  */
GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
GPIO_InitStruct.Pin       = OSPI1_D5_PIN;
HAL_GPIO_Init(OSPI1_D5_GPIO_PORT, &GPIO_InitStruct);

/* OSPI1 D6 GPIO pin configuration  */
GPIO_InitStruct.Pin       = OSPI1_D6_PIN;
GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
HAL_GPIO_Init(OSPI1_D6_GPIO_PORT, &GPIO_InitStruct);

/* OSPI1 D7 GPIO pin configuration  */
GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
GPIO_InitStruct.Pin       = OSPI1_D7_PIN;
HAL_GPIO_Init(OSPI1_D7_GPIO_PORT, &GPIO_InitStruct);

/* OSPI1 DQS GPIO pin configuration  */
GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
GPIO_InitStruct.Pin       = OSPI1_DQS_PIN;
HAL_GPIO_Init(OSPI1_DQS_GPIO_PORT, &GPIO_InitStruct);

/* Enable and set OctoSPI interrupt to the lowest priority */
HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0x0F, 0);
HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);
}

/**
* @brief  DeInitializes the Global MSP.
* @param  None
* @retval None
*/
void HAL_MspDeInit(void)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* Configure MPC_WM2 to have memory non-secure */
SET_BIT(GTZC_TZSC_S->MP2WM1, GTZC_TZSC_MP2WM1_LGTH_Msk);
SET_BIT(GTZC_TZSC_S->MP2WM2, GTZC_TZSC_MP2WM2_LGTH_Msk);
__HAL_RCC_GTZC_CLK_DISABLE();
#endif

/* Disable OctoSPI interrupts */
HAL_NVIC_DisableIRQ(OCTOSPI1_IRQn);

/* Reset the OctoSPI memory interface */
__HAL_RCC_OSPI1_FORCE_RESET();
__HAL_RCC_OSPI1_RELEASE_RESET();

/* Disable the OctoSPI memory interface clock */
__HAL_RCC_OSPI1_CLK_DISABLE();

#if (USE_HAL_OSPI_REGISTER_CALLBACKS == 1)
MspDeInit++;
#endif
}


/**
* @brief  Initializes the PPP MSP.
* @param  None
* @retval None
*/
/*void HAL_PPP_MspInit(void)
{*/
/*}*/

/**
* @brief  DeInitializes the PPP MSP.
* @param  None
* @retval None
*/
/*void HAL_PPP_MspDeInit(void)
{*/
/*}*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

