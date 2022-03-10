/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
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

#include <platform/driver/touch/SDL2TouchController.hpp>
#include <platform/hal/simulator/sdl2/HALSDL2.hpp>

namespace touchgfx
{
void touchgfx::SDL2TouchController::init()
{
}

bool touchgfx::SDL2TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    return static_cast<HALSDL2*>(HAL::getInstance())->doSampleTouch(x, y); //lint !e1774
}
}
