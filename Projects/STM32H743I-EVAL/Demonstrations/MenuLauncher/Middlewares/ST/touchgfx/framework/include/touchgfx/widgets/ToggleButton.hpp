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

#ifndef TOGGLEBUTTON_HPP
#define TOGGLEBUTTON_HPP

#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
/**
 * @class ToggleButton ToggleButton.hpp touchgfx/widgets/ToggleButton.hpp
 *
 * @brief A ToggleButton is a Button specialization that swaps the two bitmaps when clicked.
 *
 *        A ToggleButton is a Button specialization that swaps the two bitmaps when clicked,
 *        such that the previous "pressed" bitmap, now becomes the one displayed when button is
 *        not pressed.
 *
 * @see Button
 */
class ToggleButton : public Button
{
public:

    /**
     * @fn ToggleButton::ToggleButton();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ToggleButton();

    /**
     * @fn virtual void ToggleButton::setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
     *
     * @brief Sets the bitmaps.
     *
     *        Sets the bitmaps.
     *
     * @note This specific implementation remembers what bitmap was used as pressed, in order to
     *       support the ability to force the state.
     *
     * @param bmpReleased The bitmap to show in the "normal" state, ie when button is not pressed.
     * @param bmpPressed  The bitmap to show when the button is pressed.
     *
     * @see Button::setBitmaps
     */
    virtual void setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
    {
        originalPressed = bmpPressed;
        Button::setBitmaps(bmpReleased, bmpPressed);
    }

    /**
     * @fn void ToggleButton::forceState(bool activeState);
     *
     * @brief Force the button into a specific state.
     *
     *        Use this function to force the button in one of the two possible states. If
     *        button is forced to the active state, then the pressed bitmap from the last call
     *        to setBitmaps becomes the one displayed when button is not pressed.
     *
     * @param activeState If true, display the bmpPressed bitmap when not pressed. If false display
     *                    the bmpReleased bitmap.
     */
    void forceState(bool activeState);

    /**
     * @fn bool ToggleButton::getState() const
     *
     * @brief Gets the state.
     *
     *        Gets the state.
     *
     * @return true if state is currently active.
     */
    bool getState() const
    {
        return up == originalPressed;
    }

    /**
     * @fn virtual void ToggleButton::handleClickEvent(const ClickEvent& event);
     *
     * @brief Overrides handleClickEvent.
     *
     *        Overrides handleClickEvent in order to swap the bitmaps after being clicked.
     *
     * @param event The event to handle.
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * @fn virtual uint16_t ToggleButton::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_TOGGLEBUTTON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TOGGLEBUTTON;
    }
protected:
    Bitmap originalPressed; ///< Contains the bitmap that was originally being displayed when button is pressed.
};
} // namespace touchgfx

#endif // TOGGLEBUTTON_HPP
