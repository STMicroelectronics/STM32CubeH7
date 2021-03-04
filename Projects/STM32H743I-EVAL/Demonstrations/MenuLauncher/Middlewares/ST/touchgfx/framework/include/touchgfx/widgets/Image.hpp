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

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class Image Image.hpp touchgfx/widgets/Image.hpp
 *
 * @brief Simple widget capable of showing a bitmap.
 *
 *        Simple widget capable of showing a bitmap. The bitmap can be alpha-blended with the
 *        background and have areas of transparency.
 *
 * @see Widget
 */
class Image : public Widget
{
public:

    /**
     * @fn Image::Image(const Bitmap& bmp = Bitmap())
     *
     * @brief Default Constructor.
     *
     *        Constructs a new Image with a default alpha value of 255 (solid) and a default
     *        Bitmap if none is specified.
     *
     * @param bmp The bitmap to display.
     */
    Image(const Bitmap& bmp = Bitmap()) : Widget(), alpha(255)
    {
        setBitmap(bmp);
    }

    /**
     * @fn virtual void Image::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets the bitmap ID for this Image.
     *
     *        Sets the bitmap ID for this Image. Updates the width and height of this widget to
     *        match that of the bitmap.
     *
     * @param bmp The bitmap instance.
     *
     * @see Bitmap
     */
    virtual void setBitmap(const Bitmap& bmp);

    /**
     * @fn void Image::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn virtual void Image::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the image.
     *
     *        Draws the image. This class supports partial drawing, so only the area described
     *        by the rectangle will be drawn.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn BitmapId Image::getBitmap() const
     *
     * @brief Gets the BitmapId currently contained by the widget.
     *
     *        Gets the BitmapId currently contained by the widget.
     *
     * @return The current BitmapId of the widget.
     */
    BitmapId getBitmap() const
    {
        return bitmap.getId();
    }

    /**
     * @fn uint8_t Image::getAlpha() const
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
     * @fn virtual Rect Image::getSolidRect() const;
     *
     * @brief Gets the largest solid (non-transparent) rectangle.
     *
     *        Gets the largest solid (non-transparent) rectangle. This value is pre-calculated
     *        by the imageconverter tool.
     *
     * @return The largest solid (non-transparent) rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual uint16_t Image::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_IMAGE.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_IMAGE;
    }
protected:
    Bitmap  bitmap;               ///< The Bitmap to display.
    uint8_t alpha;                ///< The Alpha for this image.
    bool    hasTransparentPixels; ///< true if this object has transparent pixels
};
} // namespace touchgfx

#endif // IMAGE_HPP
