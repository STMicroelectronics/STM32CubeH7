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

#ifndef BUTTONWITHICON_HPP
#define BUTTONWITHICON_HPP

#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
/**
 * @class ButtonWithIcon ButtonWithIcon.hpp touchgfx/widgets/ButtonWithIcon.hpp
 *
 * @brief A Button specialization that also displays an icon on top of the button bitmap.
 *
 *        A Button specialization that also displays an icon on top of the button bitmap.
 *
 * @see Button
 */
class ButtonWithIcon : public Button
{
public:
    ButtonWithIcon();

#ifdef __IAR_SYSTEMS_ICC__  // Only include in IAR compilation
#pragma diag_suppress=Pe997 // Suppress warning for intentional virtual function override
#elif __ARMCC_VERSION
#pragma diag_suppress 997
#endif

    /**
     * @fn virtual void ButtonWithIcon::setBitmaps(const Bitmap& newBackgroundReleased, const Bitmap& newBackgroundPressed, const Bitmap& newIconReleased, const Bitmap& newIconPressed);
     *
     * @brief Sets the bitmaps used by this button.
     *
     *        Sets the bitmaps used by this button.
     *
     * @param newBackgroundReleased Bitmap to use when button is released.
     * @param newBackgroundPressed  Bitmap to use when button is pressed.
     * @param newIconReleased       The bitmap for the icon in the released/unpressed button
     *                              state.
     * @param newIconPressed        The bitmap for the icon in the pressed button state.
     */
    virtual void setBitmaps(const Bitmap& newBackgroundReleased, const Bitmap& newBackgroundPressed,
                            const Bitmap& newIconReleased, const Bitmap& newIconPressed);
#ifdef __IAR_SYSTEMS_ICC__  // Only include in IAR compilation
#pragma diag_default=Pe997
#endif

    /**
     * @fn void ButtonWithIcon::setIconX(int16_t x)
     *
     * @brief Sets the x coordinate of the icon bitmap.
     *
     *        Sets the x coordinate of the icon bitmap.
     *
     * @note Changing this does not automatically yield a redraw.
     * @note The value will be overwritten by calling.
     *
     * @param x The new x value, relative to the background bitmap. A negative value is allowed.
     */
    void setIconX(int16_t x)
    {
        iconX = x;
    }

    /**
     * @fn void ButtonWithIcon::setIconY(int16_t y)
     *
     * @brief Sets the y coordinate of the icon bitmap.
     *
     *        Sets the y coordinate of the icon bitmap.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param y The new y value, relative to the background bitmap. A negative value is allowed.
     */
    void setIconY(int16_t y)
    {
        iconY = y;
    }

    /**
     * @fn void ButtonWithIcon::setIconXY(int16_t x, int16_t y)
     *
     * @brief Sets the x and y coordinates of the icon bitmap.
     *
     *        Sets the x and y coordinates of the icon bitmap.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param x The new x value, relative to the background bitmap. A negative value is allowed.
     * @param y The new y value, relative to the background bitmap. A negative value is allowed.
     */
    void setIconXY(int16_t x, int16_t y)
    {
        setIconX(x);
        setIconY(y);
    }

    /**
     * @fn Bitmap ButtonWithIcon::getCurrentlyDisplayedIcon() const
     *
     * @brief Function to obtain the currently displayed icon.
     *
     *        Function to obtain the currently displayed icon, which depends on the button's
     *        pressed state.
     *
     * @return The icon currently displayed.
     */
    Bitmap getCurrentlyDisplayedIcon() const
    {
        return (pressed ? iconPressed : iconReleased);
    }

    /**
     * @fn int16_t ButtonWithIcon::getIconX() const
     *
     * @brief Gets the x coordinate of the icon bitmap.
     *
     *        Gets the x coordinate of the icon bitmap.
     *
     * @return The x coordinate of the icon bitmap.
     */
    int16_t getIconX() const
    {
        return iconX;
    }

    /**
     * @fn int16_t ButtonWithIcon::getIconY() const
     *
     * @brief Gets the y coordinate of the icon bitmap.
     *
     *        Gets the y coordinate of the icon bitmap.
     *
     * @return The y coordinate of the icon bitmap.
     */
    int16_t getIconY() const
    {
        return iconY;
    }

    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual uint16_t ButtonWithIcon::getType() const
     *
     * @brief For GUI testing only. Returns type of this drawable.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_BUTTONWITHICON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_BUTTONWITHICON;
    }
protected:
    Bitmap  iconReleased; ///< Icon to display when button is not pressed.
    Bitmap  iconPressed;  ///< Icon to display when button is pressed.
    int16_t iconX;        ///< x coordinate offset for icon.
    int16_t iconY;        ///< y coordinate offset for icon.
};
} // namespace touchgfx

#endif // BUTTONWITHICON_HPP
