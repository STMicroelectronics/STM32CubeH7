/**
  ******************************************************************************
  * @file    memory_msp.c
  * @author  MCD Application Team
  * @brief   External memory code execution MSP module.
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

/** @addtogroup MEMORY
  * @{
  */

/** @defgroup MEMORY
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup MEMORY_MSP_Private_Functions
  * @{
  */

#if (CODE_AREA == USE_QSPI) || (BINARY_AREA == USE_SPI_NOR)
/**
  * @brief QSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for QSPI interrupt
  * @retval None
  */
void QSPI_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the QuadSPI memory interface clock */
  QSPI_CLK_ENABLE();
  /* Reset the QuadSPI memory interface */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();
  /* Enable GPIO clocks */
  QSPI_CLK_GPIO_CLK_ENABLE();
  QSPI_BK1_D0_GPIO_CLK_ENABLE();
  QSPI_BK1_D1_GPIO_CLK_ENABLE();
/*#if (CODE_AREA == USE_QSPI)  */
  QSPI_BK1_D2_GPIO_CLK_ENABLE();
  QSPI_BK1_D3_GPIO_CLK_ENABLE();
/*#endif*/
  QSPI_BK2_CS_GPIO_CLK_ENABLE();
  QSPI_BK2_D0_GPIO_CLK_ENABLE();
  QSPI_BK2_D1_GPIO_CLK_ENABLE();
/*#if (CODE_AREA == USE_QSPI)*/
  QSPI_BK2_D2_GPIO_CLK_ENABLE();
  QSPI_BK2_D3_GPIO_CLK_ENABLE();
/*#endif*/
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* QSPI CLK GPIO pin configuration  */
  gpio_init_structure.Pin       = QSPI_CLK_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_CLK_GPIO_PORT, &gpio_init_structure);

  /* QSPI CS GPIO pin configuration  */
  gpio_init_structure.Pin       = QSPI_BK1_CS_PIN;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_CS_GPIO_PORT, &gpio_init_structure);

  /* QSPI D0 GPIO pin configuration  */
  gpio_init_structure.Pin       = QSPI_BK1_D0_PIN;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_D0_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin       = QSPI_BK2_D0_PIN;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_D0_GPIO_PORT, &gpio_init_structure);

  /* QSPI D1 GPIO pin configuration  */
  gpio_init_structure.Pin       = QSPI_BK1_D1_PIN;
  gpio_init_structure.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_D1_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin       = QSPI_BK2_D1_PIN;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_D1_GPIO_PORT, &gpio_init_structure);

  /* QSPI D2 GPIO pin configuration  */
  gpio_init_structure.Pin       = QSPI_BK1_D2_PIN;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK1_D2_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin       = QSPI_BK2_D2_PIN;
  HAL_GPIO_Init(QSPI_BK2_D2_GPIO_PORT, &gpio_init_structure);

  /* QSPI D3 GPIO pin configuration  */
  gpio_init_structure.Pin       = QSPI_BK1_D3_PIN;
  HAL_GPIO_Init(QSPI_BK1_D3_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin       = QSPI_BK2_D3_PIN;
  HAL_GPIO_Init(QSPI_BK2_D3_GPIO_PORT, &gpio_init_structure);

  /*##-3- Configure the NVIC for QSPI #########################################*/
  /* NVIC configuration for QSPI interrupt */
  HAL_NVIC_SetPriority(QUADSPI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(QUADSPI_IRQn);

}
#endif

#if (BINARY_AREA == USE_SPI_NOR)
/**
  * @brief QSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @retval None
  */
void QSPI_MspDeInit(void)
{
  /*##-1- Disable the NVIC for QSPI ###########################################*/
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-Configure QSPI pins */
  HAL_GPIO_DeInit(QSPI_CLK_GPIO_PORT, QSPI_CLK_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_CS_GPIO_PORT, QSPI_BK1_CS_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D0_GPIO_PORT, QSPI_BK1_D0_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D1_GPIO_PORT, QSPI_BK1_D1_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D2_GPIO_PORT, QSPI_BK1_D2_PIN);
  HAL_GPIO_DeInit(QSPI_BK1_D3_GPIO_PORT, QSPI_BK1_D3_PIN);

  HAL_GPIO_DeInit(QSPI_BK2_CS_GPIO_PORT, QSPI_BK2_CS_PIN);
  HAL_GPIO_DeInit(QSPI_BK2_D0_GPIO_PORT, QSPI_BK2_D0_PIN);
  HAL_GPIO_DeInit(QSPI_BK2_D1_GPIO_PORT, QSPI_BK2_D1_PIN);
  HAL_GPIO_DeInit(QSPI_BK2_D2_GPIO_PORT, QSPI_BK2_D2_PIN);
  HAL_GPIO_DeInit(QSPI_BK2_D3_GPIO_PORT, QSPI_BK2_D3_PIN);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the QuadSPI memory interface */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();

  /* Disable the QuadSPI memory interface clock */
  QSPI_CLK_DISABLE();
}

#endif


#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
/**
  * @brief  Initializes SDRAM MSP.
  * @retval None
  */
void SDRAM_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  
  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9 | GPIO_PIN_10 |\
                              GPIO_PIN_14 | GPIO_PIN_15;
 
   
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
      
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
    
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure); 
}
#endif /*(DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)*/

#if (BINARY_AREA == USE_MMC)

void MMC_MspInit(void)
{  
  GPIO_InitTypeDef gpio_init_structure;

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
  gpio_init_structure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);


  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SDMMC1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
  
}

void MMC_MspDeInit(void)
{  
  /*##-1- Disable the NVIC for SDMMC1 ########################################*/
  HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
  
  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* SDMMC GPIO CLKIN PB8, D0 PC8, D1 PC9, D2 PC10, D3 PC11, CK PC12, CMD PD2 */
  /* De-Configure SDMMC1 pins */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);
  
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the SDMMC1 memory interface */
  __HAL_RCC_SDMMC1_FORCE_RESET();
  __HAL_RCC_SDMMC1_RELEASE_RESET();

  /* Disable the SDMMC1 memory interface clock */
  __HAL_RCC_SDMMC1_CLK_DISABLE();
  
}
#endif /*(BINARY_AREA == USE_MMC)*/

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
