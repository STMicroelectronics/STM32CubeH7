/**
  ******************************************************************************
  * @file    memory_msp.c
  * @author  MCD Application Team
  * @brief   External memory code execution MSP module.
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
#if (CODE_AREA == USE_QSPI)  
  QSPI_BK1_D2_GPIO_CLK_ENABLE();
  QSPI_BK1_D3_GPIO_CLK_ENABLE();
#endif
  QSPI_BK2_CS_GPIO_CLK_ENABLE();
  QSPI_BK2_D0_GPIO_CLK_ENABLE();
  QSPI_BK2_D1_GPIO_CLK_ENABLE();
#if (CODE_AREA == USE_QSPI)
  QSPI_BK2_D2_GPIO_CLK_ENABLE();
  QSPI_BK2_D3_GPIO_CLK_ENABLE();
#endif
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

  gpio_init_structure.Pin       = QSPI_BK2_CS_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_CS_GPIO_PORT, &gpio_init_structure);

  /* QSPI D0 GPIO pin configuration  */
  gpio_init_structure.Pin       = QSPI_BK1_D0_PIN;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Alternate = GPIO_AF10_QUADSPI;
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
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(QSPI_BK2_D3_GPIO_PORT, &gpio_init_structure);

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

#if (DATA_AREA == USE_EXTERNAL_SRAM) || (CODE_AREA == USE_EXTERNAL_SRAM)
/**
  * @brief  Initializes SRAM MSP.
  * @retval None
  */
void SRAM_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;

  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |\
                              GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |\
                              GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3| GPIO_PIN_4 | GPIO_PIN_7     |\
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |\
                              GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* GPIOF configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* GPIOG configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |\
                              GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
}
#endif /*(DATA_AREA == USE_EXTERNAL_SRAM) || (CODE_AREA == USE_EXTERNAL_SRAM)*/


#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
/**
  * @brief  Initializes SDRAM MSP.
  * @retval None
  */
void SDRAM_MspInit(void)
{
  GPIO_InitTypeDef  GPIO_Init_Structure;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clocks */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
   /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

  /* GPIOD configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9 | GPIO_PIN_10 |\
                              GPIO_PIN_14 | GPIO_PIN_15;


  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* GPIOE configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;

  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* GPIOF configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;

  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* GPIOG configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |\
                              GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* GPIOH configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /* GPIOI configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOI, &GPIO_Init_Structure);
}
#endif /*(DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)*/

#if (CODE_AREA == USE_NOR)
/**
  * @brief NOR MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @retval None
  */
void NOR_MspInit(void)
{
   GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();  
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
    /* GPIOC configuration */
  gpio_init_structure.Alternate = GPIO_AF9_FMC;
  gpio_init_structure.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);
  
  /* GPIOD configuration */
  gpio_init_structure.Alternate = GPIO_AF12_FMC;  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7 |\
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                              GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 ;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
    
  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6   |\
                              GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                              GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4     |\
                              GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
}
#endif /* (CODE_AREA == USE_NOR) */

#if (BINARY_AREA == USE_SDCARD)
/**
  * @brief  Initializes the SD (&i2c mxp) MSP.
  * @param  hsd: SD handle
  * @param  Params  
  * @retval None
  */
void SDCARD_MspInit(void)
{

   GPIO_InitTypeDef gpio_init_structure;
  
/* GPIO setting for SDCARD */
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
  
  /* GPIOC configuration */
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
   
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* D0DIR(PC6), D123DIR(PC7) */
  gpio_init_structure.Alternate = GPIO_AF8_SDIO1;
  /* GPIOC configuration */
  gpio_init_structure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);
  
  /* CKIN(PB8), CDIR(PB9) */
  gpio_init_structure.Alternate = GPIO_AF7_SDIO1;
  /* GPIOB configuration */
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

/* GPIO setting for I2C */
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configure I2C Tx as alternate function */
  gpio_init_structure.Pin = GPIO_PIN_6;
  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);
  
  /* Configure I2C Rx as alternate function */
  gpio_init_structure.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);
  
  /*** Configure the I2C peripheral ***/ 
  /* Enable I2C clock */
  __HAL_RCC_I2C1_CLK_ENABLE();
  
  /* Force the I2C peripheral clock reset */  
  __HAL_RCC_I2C1_FORCE_RESET(); 
  
  /* Release the I2C peripheral clock reset */  
  __HAL_RCC_I2C1_RELEASE_RESET();
  
  __HAL_RCC_SDMMC1_FORCE_RESET();
  __HAL_RCC_SDMMC1_RELEASE_RESET();
}

/**
  * @brief  De-Initializes the SD (&i2c mxp) MSP.
  * @param  hsd: SD handle
  * @param  Params  
  * @retval None
  */
void SDCARD_MspDeInit(void)
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
#endif /* (BINARY_AREA == USE_SDCARD) */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


