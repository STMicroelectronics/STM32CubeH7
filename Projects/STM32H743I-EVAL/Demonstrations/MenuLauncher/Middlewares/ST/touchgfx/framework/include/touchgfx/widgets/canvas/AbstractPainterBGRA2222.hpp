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

#ifndef ABSTRACTPAINTERBGRA2222_HPP
#define ABSTRACTPAINTERBGRA2222_HPP

#include <assert.h>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <platform/driver/lcd/LCD8bpp_BGRA2222.hpp>

namespace touchgfx
{
/**
 * @class AbstractPainterBGRA2222 AbstractPainterBGRA2222.hpp touchgfx/widgets/canvas/AbstractPainterBGRA2222.hpp
 *
 * @brief A Painter that will paint using a color and an alpha value.
 *
 *        The AbstractPainterBGRA2222 class allows a shape to be filled with a given color and
 *        alpha value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class AbstractPainterBGRA2222 : public AbstractPainter
{
public:
    AbstractPainterBGRA2222()
    {
        assert(compatibleFramebuffer(Bitmap::BGRA2222) && "The chosen painter only works with BGRA2222 displays");
    }

    virtual ~AbstractPainterBGRA2222() {}

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

    /**
     * @fn FORCE_INLINE_FUNCTION uint8_t AbstractPainterBGRA2222::mixColors(uint8_t newpix, uint8_t bufpix, uint8_t alpha)
     *
     * @brief Mix colors.
     *
     *        Mix colors from a new pixel and a buffer pixel with the given alpha applied to the
     *        new pixel.
     *
     * @param newpix The newpix value.
     * @param bufpix The bufpix value.
     * @param alpha  The alpha of the newpix.
     *
     * @return The new color to write to the frame buffer.
     */
    FORCE_INLINE_FUNCTION uint8_t mixColors(uint8_t newpix, uint8_t bufpix, uint8_t alpha)
    {
        return mixColors(LCD8bpp_BGRA2222::getRedFromColor((colortype)newpix),
                         LCD8bpp_BGRA2222::getGreenFromColor((colortype)newpix),
                         LCD8bpp_BGRA2222::getBlueFromColor((colortype)newpix), bufpix, alpha);
    }

    /**
     * @fn FORCE_INLINE_FUNCTION uint8_t AbstractPainterBGRA2222::mixColors(uint8_t R, uint8_t G, uint8_t B, uint8_t bufpix, uint8_t alpha)
     *
     * @brief Mix colors.
     *
     *        Mix colors from a new pixel and a buffer pixel with the given alpha applied to the
     *        new pixel.
     *
     * @param R      The red color (0-255).
     * @param G      The green color (0-255).
     * @param B      The blue color (0-255).
     * @param bufpix The bufpix value.
     * @param alpha  The alpha of the newpix.
     *
     * @return The new color to write to the frame buffer.
     */
    FORCE_INLINE_FUNCTION uint8_t mixColors(uint8_t R, uint8_t G, uint8_t B, uint8_t bufpix, uint8_t alpha)
    {
        uint8_t ialpha = 0xFF - alpha;
        uint8_t p_red = LCD8bpp_BGRA2222::getRedFromColor((colortype)bufpix);
        uint8_t p_green = LCD8bpp_BGRA2222::getGreenFromColor((colortype)bufpix);
        uint8_t p_blue = LCD8bpp_BGRA2222::getBlueFromColor((colortype)bufpix);
        uint8_t red = LCD::div255(R * alpha + p_red * ialpha);
        uint8_t green = LCD::div255(G * alpha + p_green * ialpha);
        uint8_t blue = LCD::div255(B * alpha + p_blue * ialpha);
        return static_cast<uint8_t>(LCD8bpp_BGRA2222::getColorFromRGB(red, green, blue));
    }

protected:
    /**
     * @fn virtual bool AbstractPainterBGRA2222::renderInit()
     *
     * @brief Initialize rendering of a single scan line of pixels for the render.
     *
     *        Initialize rendering of a single scan line of pixels for the render.
     *
     * @return true if it succeeds, false if it fails.
     */
    virtual bool renderInit()
    {
        return true;
    }

    /**
     * @fn virtual bool AbstractPainterBGRA2222::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) = 0;
     *
     * @brief Get the color of the next pixel in the scan line.
     *
     *        Get the color of the next pixel in the scan line.
     *
     * @param [out] red   The red.
     * @param [out] green The green.
     * @param [out] blue  The blue.
     * @param [out] alpha The alpha.
     *
     * @return true if the pixel should be painted, false otherwise.
     */
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) = 0;

    /**
     * @fn virtual void AbstractPainterBGRA2222::renderPixel(uint8_t* p, uint8_t red, uint8_t green, uint8_t blue);
     *
     * @brief Renders the pixel.
     *
     *        Renders the pixel into the frame buffer. The colors are reduced from 8,8,8 to 2,2,2.
     *
     * @param [in] p     pointer into the frame buffer where the given color should be written.
     * @param      red   The red color.
     * @param      green The green color.
     * @param      blue  The blue color.
     */
    virtual void renderPixel(uint8_t* p, uint8_t red, uint8_t green, uint8_t blue);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
}; // class AbstractPainterBGRA2222
} // namespace touchgfx

#endif // ABSTRACTPAINTERBGRA2222_HPP
