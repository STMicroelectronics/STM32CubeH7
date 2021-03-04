/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef OSWRAPPERS_HPP
#define OSWRAPPERS_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class OSWrappers OSWrappers.hpp touchgfx/hal/OSWrappers.hpp
 *
 * @brief This class specifies OS wrappers for dealing with the frame buffer semaphore and the
 *        VSYNC signal.
 *
 *        This class specifies OS wrappers for dealing with the frame buffer semaphore and the
 *        VSYNC signal.
 */
class OSWrappers
{
public:

    /**
     * @fn static void OSWrappers::initialize();
     *
     * @brief Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
     *
     *        Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
     */
    static void initialize();

    /**
     * @fn static void OSWrappers::signalVSync();
     *
     * @brief Signal that a VSYNC has occurred.
     *
     *        Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
     *
     * @note This function is called from an ISR, and should (depending on OS) trigger a
     *       scheduling.
     */
    static void signalVSync();

    /**
     * @fn static void OSWrappers::waitForVSync();
     *
     * @brief This function blocks until a VSYNC occurs.
     *
     *        This function blocks until a VSYNC occurs.
     *
     * @note This function must first clear the mutex/queue and then wait for the next one to
     *       occur.
     */
    static void waitForVSync();

    /**
     * @fn static void OSWrappers::takeFrameBufferSemaphore();
     *
     * @brief Take the frame buffer semaphore.
     *
     *        Take the frame buffer semaphore. Blocks until semaphore is available.
     */
    static void takeFrameBufferSemaphore();

    /**
     * @fn static void OSWrappers::tryTakeFrameBufferSemaphore();
     *
     * @brief Attempt to obtain the frame buffer semaphore.
     *
     *        Attempt to obtain the frame buffer semaphore. If semaphore is not available, do
     *        nothing.
     *
     * @note must return immediately! This function does not care who has the taken the semaphore,
     *       it only serves to make sure that the semaphore is taken by someone.
     */
    static void tryTakeFrameBufferSemaphore();

    /**
     * @fn static void OSWrappers::giveFrameBufferSemaphore();
     *
     * @brief Release the frame buffer semaphore.
     *
     *        Release the frame buffer semaphore.
     */
    static void giveFrameBufferSemaphore();

    /**
     * @fn static void OSWrappers::giveFrameBufferSemaphoreFromISR();
     *
     * @brief Release the frame buffer semaphore in a way that is safe in interrupt context. Called
     *        from ISR.
     *
     *        Release the frame buffer semaphore in a way that is safe in interrupt context.
     *        Called from ISR.
     */
    static void giveFrameBufferSemaphoreFromISR();

    /**
     * @fn static void taskDelay(uint16_t ms);
     *
     * @brief A function that causes executing task to sleep for a number of milliseconds.
     *
     *        A function that causes executing task to sleep for a number of milliseconds.
     *        This function is OPTIONAL. It is only used by the TouchGFX in the case of
     *        a specific frame refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).
     *        Due to backwards compatibility, in order for this function to be usable by the HAL
     *        the function must be explicitly registered:
     *          hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
     * @param ms The number of milliseconds to sleep
     * @see HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
     * @see HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
     */
    static void taskDelay(uint16_t ms);
};
} // namespace touchgfx
#endif // OSWRAPPERS_HPP
