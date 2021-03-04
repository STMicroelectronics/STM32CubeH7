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

#ifndef PIXELDATAWIDGET_HPP
#define PIXELDATAWIDGET_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class PixelDataWidget PixelDataWidget.hpp touchgfx/widgets/PixelDataWidget.hpp
 *
 * @brief A widget for displaying a buffer of pixel data
 *
 *        The buffer must be of size as widget. If the LCD is 16 bit the buffer must hold 2
 *        bytes for each pixel. If the LCD is 24 bit the buffer must hold 3 bytes for each
 *        pixel.
 *
 * @see touchgfx::Widget
 */
class PixelDataWidget : public touchgfx::Widget
{
public:

    /**
     * @fn PixelDataWidget::PixelDataWidget();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    PixelDataWidget();

    /**
     * @fn virtual void PixelDataWidget::draw(const touchgfx::Rect& invalidatedArea) const;
     *
     * @brief Draw the part of the RAM buffer that is inside the invalidated area.
     *
     *        Draw the part of the RAM buffer that is inside the invalidated area.
     *
     * @param invalidatedArea The region of this drawable that needs to be redrawn.
     *
     * @see touchgfx::Drawable
     */
    virtual void draw(const touchgfx::Rect& invalidatedArea) const;

    /**
     * @fn virtual touchgfx::Rect PixelDataWidget::getSolidRect() const;
     *
     * @brief Report this widget as being completely solid.
     *
     *        Report this widget as being completely solid.
     *
     * @return The solid rect.
     */
    virtual touchgfx::Rect getSolidRect() const;

    /**
     * @fn void PixelDataWidget::setPixelData(uint8_t* const data);
     *
     * @brief Set the pixel data to display.
     *
     *        Set the pixel data to display.
     *
     * @param [in,out] data If non-null, the data.
     */
    void setPixelData(uint8_t* const data);

    /**
     * @fn void PixelDataWidget::setBitmapFormat(Bitmap::BitmapFormat format);
     *
     * @brief Set the format of the pixel data.
     *
     *        Set the format of the pixel data.
     *
     * @param format Describes the format to use.
     */
    void setBitmapFormat(Bitmap::BitmapFormat format);

    /**
     * @fn void PixelDataWidget::setAlpha(uint8_t a);
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param a The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t a);

    /**
     * @fn uint8_t PixelDataWidget::getAlpha();
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     *
     * @see setAlpha
     */
    uint8_t getAlpha() const;

protected:
    uint8_t* buffer;               ///< The buffer where the pixels are copied from
    Bitmap::BitmapFormat format; ///< The pixel format for the data.
    uint8_t alpha;               ///< The Alpha for this widget.
};
} // namespace touchgfx

#endif // PIXELDATAWIDGET_HPP
