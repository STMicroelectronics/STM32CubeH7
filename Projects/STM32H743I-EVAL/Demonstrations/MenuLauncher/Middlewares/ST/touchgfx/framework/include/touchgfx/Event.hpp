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

#ifndef EVENT_HPP
#define EVENT_HPP

namespace touchgfx
{
/**
 * @class Event Event.hpp touchgfx/Event.hpp
 *
 * @brief Simple base class for events.
 *
 *        Simple base class for events.
 */
class Event
{
public:

    /**
     * @typedef enum EventType
     *
     * @brief The events types.
     *
     *        The events types.
     */
    typedef enum
    {
        EVENT_CLICK,  ///< A click
        EVENT_DRAG,   ///< A drag
        EVENT_GESTURE ///< A gesture
    } EventType;

    Event() { }

    /**
     * @fn virtual EventType Event::getEventType() = 0;
     *
     * @brief Gets event type.
     *
     *        Gets event type.
     *
     * @return The type of this event.
     */
    virtual EventType getEventType() = 0;

    /**
     * @fn virtual Event::~Event()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Event() { }
};
} // namespace touchgfx

#endif // EVENT_HPP
