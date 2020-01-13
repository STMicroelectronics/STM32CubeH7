/**
  ******************************************************************************
  * @file    PSSI_Transmit_Receive_DMA/Inc/stm32h7xx_hal_msp.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H7XX_HAL_MSP_H
#define __STM32H7XX_HAL_MSP_H

void HAL_PSSI_MspInit_8B(PSSI_HandleTypeDef *hpssi);
void HAL_PSSI_MspInit_16B(PSSI_HandleTypeDef *hpssi);
void HAL_PSSI_MspInit_32B(PSSI_HandleTypeDef *hpssi);

#endif /* __STM32H7XX_HAL_MSP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/