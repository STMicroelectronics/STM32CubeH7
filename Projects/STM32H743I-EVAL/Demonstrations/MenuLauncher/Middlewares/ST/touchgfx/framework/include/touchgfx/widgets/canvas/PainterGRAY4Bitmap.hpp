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

#ifndef PAINTERGRAY4BITMAP_HPP
#define PAINTERGRAY4BITMAP_HPP

#include <stdint.h>
#include <touchgfx/widgets/canvas/AbstractPainterGRAY4.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>

namespace touchgfx
{
/**
 * @class PainterGRAY4Bitmap PainterGRAY4Bitmap.hpp touchgfx/widgets/canvas/PainterGRAY4Bitmap.hpp
 *
 * @brief A Painter that will paint using a bitmap.
 *
 *        PainterGRAY4Bitmap will take the color for a given point in the shape from a bitmap.
 *        Please be aware, the the bitmap is used by the CWR (not Shape), so any rotation you
 *        might specify for a Canvas Widget (e.g. Shape) is not applied to the bitmap as CWR is
 *        not aware of this rotation.
 *
 * @see AbstractPainter
 */
class PainterGRAY4Bitmap : public AbstractPainterGRAY4
{
public:

    /**
     * @fn PainterGRAY4Bitmap::PainterGRAY4Bitmap(const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param bmp   The bitmap.
     * @param alpha the alpha.
     */
    PainterGRAY4Bitmap(const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);

    /**
     * @fn void PainterGRAY4Bitmap::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets a bitmap to be used when drawing the CanvasWidget.
     *
     *        Sets a bitmap to be used when drawing the CanvasWidget.
     *
     * @param bmp The bitmap.
     */
    void setBitmap(const Bitmap& bmp);

    /**
     * @fn void PainterGRAY4Bitmap::setAlpha(uint8_t alpha);
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
     * @fn uint8_t PainterGRAY4Bitmap::getAlpha() const;
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

    virtual bool renderNext(uint8_t& gray, uint8_t& alpha);

    const uint8_t* bitmapGRAY4Pointer;   ///< Pointer to the bitmap (GRAY4)
    const uint8_t* bitmapAlphaPointer;     ///< Pointer to the bitmap alpha data for GRAY4

    Bitmap bitmap;                  ///< The bitmap to be used when painting
    Rect   bitmapRectToFrameBuffer; ///< Bitmap rectangle translated to frame buffer coordinates

    uint8_t painterAlpha; ///< The alpha to use if no alpha data is present in the given bitmap
}; // class PainterGRAY4Bitmap
} // namespace touchgfx

#endif // PAINTERGRAY4BITMAP_HPP
