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

#ifndef PAINTERRGBA2222BITMAP_HPP
#define PAINTERRGBA2222BITMAP_HPP

#include <stdint.h>
#include <touchgfx/widgets/canvas/AbstractPainterRGBA2222.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>

namespace touchgfx
{
/**
 * @class PainterRGBA2222Bitmap PainterRGBA2222Bitmap.hpp touchgfx/widgets/canvas/PainterRGBA2222Bitmap.hpp
 *
 * @brief A Painter that will paint using a bitmap.
 *
 *        PainterRGBA2222Bitmap will take the color for a given point in the shape from a bitmap.
 *        Please be aware, the the bitmap is used by the CWR (not Shape), so any rotation you
 *        might specify for a Canvas Widget (e.g. Shape) is not applied to the bitmap as CWR is
 *        not aware of this rotation.
 *
 * @see AbstractPainter
 */
class PainterRGBA2222Bitmap : public AbstractPainterRGBA2222
{
public:
    /**
     * @fn PainterRGBA2222Bitmap::PainterRGBA2222Bitmap(const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param bmp   The bitmap.
     * @param alpha the alpha.
     */
    PainterRGBA2222Bitmap(const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);

    /**
     * @fn void PainterRGBA2222Bitmap::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets a bitmap to be used when drawing the CanvasWidget.
     *
     *        Sets a bitmap to be used when drawing the CanvasWidget.
     *
     * @param bmp The bitmap.
     */
    void setBitmap(const Bitmap& bmp);

    /**
     * @fn void PainterRGBA2222Bitmap::setAlpha(uint8_t alpha);
     *
     * @brief Sets an alpha value for the bitmap.
     *
     *        Sets an alpha value for the bitmap. If the image contains an alpha channel, this
     *        alpha value is combined with the alpha in the bitmap to produce the final alpha
     *        value.
     *
     * @param alpha The alpha value to use if there is no alpha channel in the bitmap.
     */
    void setAlpha(uint8_t alpha);

    /**
     * @fn uint8_t PainterRGBA2222Bitmap::getAlpha() const;
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

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderInit();

    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha);

    const uint8_t* bitmapRGBA2222Pointer;   ///< Pointer to the bitmap (RGBA2222)

    Bitmap bitmap;                  ///< The bitmap to be used when painting
    Rect   bitmapRectToFrameBuffer; ///< Bitmap rectangle translated to frame buffer coordinates

    uint8_t painterAlpha; ///< The alpha to use if no alpha data is present in the given bitmap
}; // class PainterRGBA2222Bitmap
} // namespace touchgfx

#endif // PAINTERRGBA2222BITMAP_HPP
