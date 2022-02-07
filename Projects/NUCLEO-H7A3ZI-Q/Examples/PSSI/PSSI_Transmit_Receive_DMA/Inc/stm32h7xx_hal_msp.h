/**
  ******************************************************************************
  * @file    PSSI_Transmit_Receive_DMA/Inc/stm32h7xx_hal_msp.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __STM32H7XX_HAL_MSP_H
#define __STM32H7XX_HAL_MSP_H

void HAL_PSSI_MspInit_8B(PSSI_HandleTypeDef *hpssi);
void HAL_PSSI_MspInit_16B(PSSI_HandleTypeDef *hpssi);
void HAL_PSSI_MspInit_32B(PSSI_HandleTypeDef *hpssi);

#endif /* __STM32H7XX_HAL_MSP_H */

