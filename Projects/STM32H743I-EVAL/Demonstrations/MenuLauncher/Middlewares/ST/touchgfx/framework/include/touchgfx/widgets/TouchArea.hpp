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

#ifndef TOUCHAREA_HPP
#define TOUCHAREA_HPP

#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class TouchArea TouchArea.hpp touchgfx/widgets/TouchArea.hpp
 *
 * @brief Invisible widget used to capture touch events.
 *
 *        Invisible widget used to capture touch events. The TouchArea consumes drag events
 *        without the widget it self moving.
 *
 * @see AbstractButton
 */
class TouchArea : public AbstractButton
{
public:
    /**
     * @fn TouchArea::TouchArea()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    TouchArea() : AbstractButton(), pressedAction(0)
    {
    }

    /**
     * @fn virtual void TouchArea::draw(const Rect& invalidatedArea) const
     *
     * @brief A TouchArea will not draw anything.
     *
     *        A TouchArea will not draw anything.
     *
     * @param invalidatedArea The region of the toucharea to draw. Ignored.
     */
    virtual void draw(const Rect& invalidatedArea) const
    {
    }

    /**
     * @fn virtual void TouchArea::handleDragEvent(const DragEvent& evt)
     *
     * @brief A TouchArea will not move when dragged.
     *
     *        A TouchArea will not move when dragged.
     *
     * @param evt The event to handle. Ignored.
     */
    virtual void handleDragEvent(const DragEvent& evt)
    {
    }

    /**
     * @fn virtual void TouchArea::handleClickEvent(const ClickEvent& event);
     *
     * @brief A TouchArea will refine the handling of click events.
     *
     *        A TouchArea will refine the handling of click events in order to enable the
     *        callback to the pressedAction.
     *
     * @param event The event to handle.
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * @fn virtual Rect TouchArea::getSolidRect() const
     *
     * @brief A TouchArea has no solid rectangle.
     *
     *        A TouchArea has no solid rectangle.
     *
     * @return an empty rect.
     */
    virtual Rect getSolidRect() const
    {
        return Rect(0, 0, 0, 0);
    }

    /**
     * @fn void TouchArea::setPressedAction(GenericCallback< const AbstractButton& >& callback)
     *
     * @brief Associates an action to be performed when the TouchArea is pressed.
     *
     *        Associates an action to be performed when the TouchArea is pressed.
     *
     * @param callback The callback is given a reference to this touch area.
     */
    void setPressedAction(GenericCallback< const AbstractButton& >& callback)
    {
        pressedAction = &callback;
    }

    /**
     * @fn virtual uint16_t TouchArea::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_TOUCHAREA.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TOUCHAREA;
    }

protected:
    GenericCallback< const AbstractButton& >* pressedAction;    ///< The action to perform when the TouchArea is clicked
};
} // namespace touchgfx

#endif // TOUCHAREA_HPP
