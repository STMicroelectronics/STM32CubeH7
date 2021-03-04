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

#ifndef TILEDIMAGE_HPP
#define TILEDIMAGE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
* @class TiledImage TiledImage.hpp touchgfx/widgets/TiledImage.hpp
 *
 * @brief Simple widget capable of showing a tiled bitmap.
 *
 *        Simple widget capable of showing a tiled bitmap. This means that when TiledImage is
 *        larger than the provided Bitmap, the Bitmap is repeated over and over horizontally
 *        and vertically. The bitmap can be alpha-blended with the background and have areas
 *        of transparency.
 *
 * @see Image
 */
class TiledImage : public Image
{
public:

    /**
     * @fn TiledImage::TiledImage(const Bitmap& bmp = Bitmap())
     *
     * @brief Default Constructor.
     *
     *        Constructs a new Image with a default alpha value of 255 (solid) and a default
     *        Bitmap if none is specified.
     *
     * @param bmp The bitmap to display.
     */
    TiledImage(const Bitmap& bmp = Bitmap()) : Image(bmp), xOffset(0), yOffset(0)
    {
    }

    /**
     * @fn void TiledImage::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets the bitmap ID for this TiledImage.
     *
     *        Sets the bitmap ID for this TiledImage. Updates the width and height of this widget to
     *        match that of the bitmap.
     *
     * @param bmp The bitmap instance.
     *
     * @see Bitmap
     */
    virtual void setBitmap(const Bitmap& bmp);

    /**
     * @fn virtual void TiledImage::setOffset(int16_t x, int16_t y);
     *
     * @brief Sets an offset into the bitmap where the tile drawing should start.
     *
     *        Sets an offset into the bitmap where the tile drawing should start.
     *
     * @param x The x coordinate offset.
     * @param y The y coordinate offset.
     *
     * @see setXOffset
     * @see setYOffset
     */
    virtual void setOffset(int16_t x, int16_t y);

    /**
     * @fn virtual void TiledImage::setXOffset(int16_t x);
     *
     * @brief Sets x offset into the bitmap where the tile drawing should start.
     *
     *        Sets x offset into the bitmap where the tile drawing should start.
     *
     * @param x The x coordinate offset.
     *
     * @see setYOffset
     * @see setOffset
     */
    virtual void setXOffset(int16_t x);

    /**
     * @fn virtual void TiledImage::setYOffset(int16_t y);
     *
     * @brief Sets y offset into the bitmap where the tile drawing should start.
     *
     *        Sets y offset into the bitmap where the tile drawing should start.
     *
     * @param y The y coordinate offset.
     *
     * @see setXOffset
     * @see setOffset
     */
    virtual void setYOffset(int16_t y);

    /**
     * @fn virtual void TiledImage::getOffset(int16_t& x, int16_t& y);
     *
     * @brief Gets the offset into the bitmap where the tile drawing should start.
     *
     *        Gets the offset into the bitmap where the tile drawing should start. Please note
     *        that the offsets set using setOffset have been normalized.
     *
     * @param [out] x The x coordinate offset.
     * @param [out] y The y coordinate offset.
     *
     * @see getXOffset
     * @see getYOffset
     */
    virtual void getOffset(int16_t& x, int16_t& y);

    /**
     * @fn virtual int16_t TiledImage::getXOffset();
     *
     * @brief Get x coordinate offset.
     *
     * @return The x coordinate offset.
     * @see getYOffset
     * @see getOffset
     */
    virtual int16_t getXOffset();

    /**
     * @fn virtual int16_t TiledImage::getYOffset();
     *
     * @brief Get y coordinate offset.
     *
     * @return The y coordinate offset.
     * @see getXOffset
     * @see getOffset
     */
    virtual int16_t getYOffset();

    /**
     * @fn virtual void TiledImage::draw(const Rect& invalidatedArea) const;
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
     * @fn virtual Rect TiledImage::getSolidRect() const;
     *
     * @brief Gets the largest solid (non-transparent) rectangle.
     *
     *        Gets the largest solid (non-transparent) rectangle. This value is pre-calculated
     *        by the image converter tool.
     *
     * @return The largest solid (non-transparent) rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual uint16_t TiledImage::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_IMAGE.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TILEDIMAGE;
    }
protected:
    int16_t xOffset; ///< The X offset into the bitmap to start drawing
    int16_t yOffset; ///< The Y offset into the bitmap to start drawing
};
} // namespace touchgfx

#endif // TILEDIMAGE_HPP
