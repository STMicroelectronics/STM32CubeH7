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

#ifndef ABSTRACTCLOCK_HPP
#define ABSTRACTCLOCK_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>

namespace touchgfx
{
/**
 * @class AbstractClock AbstractClock.hpp touchgfx/containers/clock/AbstractClock.hpp
 *
 * @brief Superclass of clock widgets.
 */
class AbstractClock : public Container
{
public:

    /**
     * @fn AbstractClock::AbstractClock();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    AbstractClock();

    /**
     * @fn virtual AbstractClock::~AbstractClock()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~AbstractClock() {}

    /**
     * @fn virtual void AbstractClock::setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second)
     *
     * @brief Sets the time with input format as 24H.
     *
     *        Sets the time with input format as 24H. Note that this does not affect any selected
     *        presentation formats.
     *
     * @param hour   The hours (in 24H format).
     * @param minute The minutes (in 24H format).
     * @param second The seconds (in 24H format).
     */
    virtual void setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * @fn virtual void AbstractClock::setTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am)
     *
     * @brief Sets the time with input format as 12H.
     *
     *        Sets the time with input format as 12H. Note that this does not affect any selected
     *        presentation formats.
     *
     * @param hour   The hours (in 12H format).
     * @param minute The minutes (in 12H format).
     * @param second The seconds (in 12H format).
     * @param am     AM/PM setting. True = AM, false = PM.
     */
    virtual void setTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am);

    /**
     * @fn uint8_t AbstractClock::getCurrentHour() const;
     *
     * @brief Gets the current hour.
     *
     *        Gets the current hour.
     *
     * @return The current hour.
     */
    uint8_t getCurrentHour() const;

    /**
     * @fn uint8_t AbstractClock::getCurrentMinute() const;
     *
     * @brief Gets the current minute.
     *
     *        Gets the current minute.
     *
     * @return The current minute.
     */
    uint8_t getCurrentMinute() const;

    /**
     * @fn uint8_t AbstractClock::getCurrentSecond() const;
     *
     * @brief Gets the current second.
     *
     *        Gets the current second.
     *
     * @return The current second.
     */
    uint8_t getCurrentSecond() const;

protected:
    uint8_t currentHour;    ///< Local copy of the current hour
    uint8_t currentMinute;  ///< Local copy of the current minute
    uint8_t currentSecond;  ///< Local copy of the current second

    /**
     * @fn virtual void AbstractClock::updateClock() = 0;
     *
     * @brief Updates the visual representation of the clock.
     *
     *        Updates the visual representation of the clock.
     */
    virtual void updateClock() = 0;
};
}

#endif // ABSTRACTCLOCK_HPP
