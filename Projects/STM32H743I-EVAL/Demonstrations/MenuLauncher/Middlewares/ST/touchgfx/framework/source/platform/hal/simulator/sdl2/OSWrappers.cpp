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

#include <touchgfx/hal/OSWrappers.hpp>
#include <SDL2/SDL.h>

namespace touchgfx
{
static SDL_mutex* s_FrameBufferLock;

void OSWrappers::initialize()
{
    // Setup framebuffer lock
    s_FrameBufferLock = SDL_CreateMutex();
}

void OSWrappers::takeFrameBufferSemaphore()
{
    SDL_LockMutex(s_FrameBufferLock);
}

void OSWrappers::giveFrameBufferSemaphore()
{
    SDL_UnlockMutex(s_FrameBufferLock);
}

void OSWrappers::waitForVSync()
{}

void OSWrappers::tryTakeFrameBufferSemaphore()
{}

void OSWrappers::giveFrameBufferSemaphoreFromISR()
{}
} // namespace touchgfx
