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
