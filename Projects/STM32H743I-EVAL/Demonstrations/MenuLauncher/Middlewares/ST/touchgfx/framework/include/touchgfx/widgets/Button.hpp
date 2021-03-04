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

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class Button Button.hpp touchgfx/widgets/Button.hpp
 *
 * @brief A button with two states.
 *
 *        A button consists of two images, one for its normal state and one when it is pressed
 *        down.
 *
 * @see AbstractButton
 */
class Button : public AbstractButton
{
public:

    /**
     * @fn Button::Button()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Button() : AbstractButton(), up(), down(), alpha(255) { }

    /**
     * @fn virtual Button::~Button()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Button() { }

    /**
     * @fn virtual void Button::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the given invalidated area.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @see Drawable::draw()
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual void Button::setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed);
     *
     * @brief Sets the bitmaps used by this button.
     *
     *        Sets the bitmaps used by this button.
     *
     * @param bmpReleased Bitmap to use when button is released.
     * @param bmpPressed  Bitmap to use when button is pressed.
     */
    virtual void setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed);

    /**
     * @fn virtual Rect Button::getSolidRect() const;
     *
     * @brief Gets solid rectangle.
     *
     *        Gets solid rectangle.
     *
     * @return largest possible solid rect. Delegated to the largest solid rect of the button
     *         bitmap(s).
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn void Button::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha value for the image.
     *
     *        Sets the alpha value for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn uint8_t Button::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn Bitmap Button::getCurrentlyDisplayedBitmap() const
     *
     * @brief Gets currently displayed bitmap.
     *
     *        Function to obtain the currently displayed bitmap, which depends on the button's
     *        pressed state.
     *
     * @return The bitmap currently displayed.
     */
    Bitmap getCurrentlyDisplayedBitmap() const
    {
        return (AbstractButton::pressed ? down : up);
    }

    /**
     * @fn virtual uint16_t Button::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_BUTTON.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_BUTTON;
    }

protected:
    Bitmap  up;    ///< The image to display when button is released.
    Bitmap  down;  ///< The image to display when button is pressed.
    uint8_t alpha; ///< The current alpha value. 255 denotes solid, 0 denotes completely transparent.
};
} // namespace touchgfx

#endif // BUTTON_HPP
