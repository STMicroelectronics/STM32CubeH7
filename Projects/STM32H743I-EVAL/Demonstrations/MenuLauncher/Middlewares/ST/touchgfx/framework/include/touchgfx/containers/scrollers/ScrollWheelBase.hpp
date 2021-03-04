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

#ifndef SCROLLWHEELBASE_HPP
#define SCROLLWHEELBASE_HPP

#include <touchgfx/Callback.hpp>
#include <touchgfx/containers/scrollers/ScrollBase.hpp>

namespace touchgfx
{
/**
 * @class ScrollWheelBase ScrollWheelBase.hpp touchgfx/containers/scrollers/ScrollWheelBase.hpp
 *
 * @brief A scroll wheel base class.
 *
 *        A scroll wheel base class. Used by ScrollWheel and ScrollWheelWithHighlight.
 *
 * @see ScrollWheel
 * @see ScrollWheelWithHighlight
 */
class ScrollWheelBase : public ScrollBase
{
public:
    /**
     * @fn ScrollWheelBase::ScrollWheelBase();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ScrollWheelBase();

    /**
     * @fn virtual ScrollWheelBase::~ScrollWheelBase();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ScrollWheelBase()
    {
    }

    /**
     * @fn virtual void ScrollWheelBase::setSelectedItemOffset(int16_t offset);
     *
     * @brief Sets selected item offset.
     *
     *        Sets selected item offset. This is the number of pixels from the start of the widget
     *        where the selected item is placed on screen. The offset is the relative x coordinate
     *        if the ScrollWheel is horizontal, otherwise it is the relative y coordinate. If this
     *        value is zero, the selected item is placed at the very start of the widget.
     *
     * @param offset The offset.
     */
    virtual void setSelectedItemOffset(int16_t offset);

    /**
     * @fn virtual int16_t ScrollWheelBase::getSelectedItemOffset() const;
     *
     * @brief Gets selected item offset.
     *
     *        Gets offset of selected item measured in pixels relative to the start of the widget.
     *
     * @return The selected item offset.
     *
     * @see setSelectedItemOffset
     */
    virtual int16_t getSelectedItemOffset() const;

    /**
     * @fn int ScrollWheelBase::getSelectedItem() const;
     *
     * @brief Gets selected item.
     *
     *        Gets selected item. If an animation is in progress, the item that is being scrolled
     *        to is returned, not the item that happens to be flying by at the time.
     *
     * @return The selected item.
     */
    int getSelectedItem() const;

    /**
     * @fn virtual int32_t ScrollWheelBase::keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const
     *
     * @brief Keep offset inside limits.
     *
     * @param newOffset The new offset.
     * @param overShoot The over shoot.
     *
     * @return An int32_t.
     */
    virtual int32_t keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const;

    /**
     * @fn virtual void ScrollWheelBase::handleClickEvent(const ClickEvent& evt);
     *
     * @brief Handles the click event described by evt.
     *
     *        Handles the click event described by evt.
     *
     * @param evt The event.
     */
    virtual void handleClickEvent(const ClickEvent& evt);

    /**
     * @fn virtual void ScrollWheelBase::handleDragEvent(const DragEvent& evt);
     *
     * @brief Handles the drag event described by evt.
     *
     *        Handles the drag event described by evt.
     *
     * @param evt The event.
     */
    virtual void handleDragEvent(const DragEvent& evt);

    /**
     * @fn virtual void ScrollWheelBase::handleGestureEvent(const GestureEvent& evt);
     *
     * @brief Handles the gesture event described by evt.
     *
     *        Handles the gesture event described by evt.
     *
     * @param evt The event.
     */
    virtual void handleGestureEvent(const GestureEvent& evt);

    /**
     * @fn void ScrollWheelBase::setAnimateToCallback(GenericCallback<int16_t>& callback);
     *
     * @brief Sets Callback which will be called when the ScrollWheel animates to a new item.
     *
     *        Sets Callback which will be called when the ScrollWheel animates to a new item.
     *
     * @param [in] callback The callback.
     */
    void setAnimateToCallback(GenericCallback<int16_t>& callback);

protected:
    /**
     * @fn virtual int32_t ScrollWheelBase::getPositionForItem(int16_t itemIndex);
     *
     * @brief Gets position for an item.
     *
     *        Get the position for an item. The position should ensure that the item is in view as
     *        defined by the semantics of the actual scroll class.
     *
     * @param itemIndex Zero-based index of the item.
     *
     * @return The position for item.
     */
    virtual int32_t getPositionForItem(int16_t itemIndex);

    /**
     * @fn virtual void ScrollWheelBase::animateToPosition(int32_t position, int16_t steps = -1);
     *
     * @brief Animate to a new position/offset using the given number of steps.
     *
     *        Animate to a new position/offset using the given number of steps.
     *
     * @param position The new position.
     * @param steps    (Optional) The number of steps. If steps is &lt;0, the default number of
     *                 steps is used (this is the default). If steps ==0, there will be no animation,
     *                 simply a direct skip to the given position.
     */
    virtual void animateToPosition(int32_t position, int16_t steps = -1);

    GenericCallback<int16_t>* animateToCallback; ///< The animate to callback
};
} // namespace touchgfx

#endif // SCROLLWHEELBASE_HPP
