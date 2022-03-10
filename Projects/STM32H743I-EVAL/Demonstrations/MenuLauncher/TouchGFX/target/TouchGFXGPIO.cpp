/**
  ******************************************************************************
  * File Name          : TouchGFXGPIO.cpp
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include <touchgfx/hal/GPIO.hpp>

/**
 * GPIO_ID Enum, these are used bt TouchGFX framework to signal events.
 *
 * VSYNC_FREQ,  /// Pin is toggled at each VSYNC
 * RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
 * FRAME_RATE,  /// Pin is toggled when the frame buffers are swapped.
 * MCU_ACTIVE   /// Pin is high when framework is utilizing the MCU.
 *
 */

/* USER CODE BEGIN TouchGFXGPIO.cpp */
#include "stm32h7xx.h"
#include "main.h"

/* USER CODE BEGIN user includes */

/* USER CODE END user includes */

using namespace touchgfx;

static int GPIO_InvertedLevels[4];

#define M_GPIO_PIN_SET(id)    (GPIO_InvertedLevels[id] ? GPIO_PIN_RESET  : GPIO_PIN_SET)
#define M_GPIO_PIN_RESET(id)  (GPIO_InvertedLevels[id] ? GPIO_PIN_SET    : GPIO_PIN_RESET)

/*
 * Perform configuration of IO pins.
 */
void GPIO::init()
{
    for (int id = 0; id <= 4; id++)
    {
        if (GPIO::get(static_cast<GPIO_ID>(id)))
        {
            if (GPIO_InvertedLevels[id] != 1)
            {
                GPIO_InvertedLevels[id] = 1;
            }
        }
    }
}

/*
 * Sets a pin high.
 */
void GPIO::set(GPIO_ID id)
{
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        HAL_GPIO_WritePin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin, M_GPIO_PIN_SET(id));
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        HAL_GPIO_WritePin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin, M_GPIO_PIN_SET(id));
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_Port) && defined(FRAME_RATE_Pin)
        HAL_GPIO_WritePin(FRAME_RATE_Port, FRAME_RATE_Pin, M_GPIO_PIN_SET(id));
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        HAL_GPIO_WritePin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin, M_GPIO_PIN_SET(id));
#endif
        break;
    }
}

/*
 * Sets a pin low.
 */
void GPIO::clear(GPIO_ID id)
{
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        HAL_GPIO_WritePin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin, M_GPIO_PIN_RESET(id));
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        HAL_GPIO_WritePin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin, M_GPIO_PIN_RESET(id));
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_Port) && defined(FRAME_RATE_Pin)
        HAL_GPIO_WritePin(FRAME_RATE_Port, FRAME_RATE_Pin, M_GPIO_PIN_RESET(id));
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        HAL_GPIO_WritePin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin, M_GPIO_PIN_RESET(id));
#endif
        break;
    }
}

/*
 * Toggles a pin.
 */
void GPIO::toggle(GPIO_ID id)
{
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        HAL_GPIO_TogglePin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin);
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        HAL_GPIO_TogglePin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin);
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_Port) && defined(FRAME_RATE_Pin)
        HAL_GPIO_TogglePin(FRAME_RATE_Port, FRAME_RATE_Pin);
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        HAL_GPIO_TogglePin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin);
#endif
        break;
    }
}

/*
 * Gets the state of a pin.
 */
bool GPIO::get(GPIO_ID id)
{
    GPIO_PinState bitstatus = GPIO_PIN_RESET;
    switch (id)
    {
    case GPIO::VSYNC_FREQ:
#if defined(VSYNC_FREQ_GPIO_Port) && defined(VSYNC_FREQ_Pin)
        bitstatus = HAL_GPIO_ReadPin(VSYNC_FREQ_GPIO_Port, VSYNC_FREQ_Pin);
#endif
        break;
    case GPIO::RENDER_TIME:
#if defined(RENDER_TIME_GPIO_Port) && defined(RENDER_TIME_Pin)
        bitstatus = HAL_GPIO_ReadPin(RENDER_TIME_GPIO_Port, RENDER_TIME_Pin);
#endif
        break;
    case GPIO::FRAME_RATE:
#if defined(FRAME_RATE_Port) && defined(FRAME_RATE_Pin)
        bitstatus = HAL_GPIO_ReadPin(FRAME_RATE_Port, FRAME_RATE_Pin);
#endif
        break;
    case GPIO::MCU_ACTIVE:
#if defined(MCU_ACTIVE_GPIO_Port) && defined(MCU_ACTIVE_Pin)
        bitstatus = HAL_GPIO_ReadPin(MCU_ACTIVE_GPIO_Port, MCU_ACTIVE_Pin);
#endif
        break;
    }
    return (bitstatus == GPIO_PIN_SET);
}

/* USER CODE END TouchGFXGPIO.cpp */
