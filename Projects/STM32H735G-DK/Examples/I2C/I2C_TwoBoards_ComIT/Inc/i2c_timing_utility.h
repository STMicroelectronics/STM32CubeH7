/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_ComIT/Inc/i2c_timing_utility.h
  * @author  MCD Application Team
  * @brief   Header of i2c_timing_utility.c
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
#ifndef __I2C_TIMING_UTILITY_H
#define __I2C_TIMING_UTILITY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Exported functions ------------------------------------------------------- */
uint32_t I2C_GetTiming(uint32_t clock_src_freq, uint32_t i2c_freq);

#endif /* __I2C_TIMING_UTILITY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
