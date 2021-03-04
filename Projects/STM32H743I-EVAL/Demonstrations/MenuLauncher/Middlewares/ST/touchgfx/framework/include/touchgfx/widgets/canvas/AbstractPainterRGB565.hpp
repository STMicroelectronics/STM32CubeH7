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

#ifndef ABSTRACTPAINTERRGB565_HPP
#define ABSTRACTPAINTERRGB565_HPP

#include <assert.h>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class AbstractPainterRGB565 AbstractPainterRGB565.hpp touchgfx/widgets/canvas/AbstractPainterRGB565.hpp
 *
 * @brief A Painter that will paint using a color and an alpha value.
 *
 *        The AbstractPainterRGB565 class allows a shape to be filled with a given color and
 *        alpha value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class AbstractPainterRGB565 : public AbstractPainter
{
public:
    static const uint16_t RMASK = 0xF800; ///< Mask for red   (1111100000000000)
    static const uint16_t GMASK = 0x07E0; ///< Mask for green (0000011111100000)
    static const uint16_t BMASK = 0x001F; ///< Mask for blue  (0000000000011111)

    AbstractPainterRGB565()
    {
        assert(compatibleFramebuffer(Bitmap::RGB565) && "The chosen painter only works with RGB565 displays");
    }

    virtual ~AbstractPainterRGB565() {}

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

    /**
     * @fn FORCE_INLINE_FUNCTION uint16_t AbstractPainterRGB565::mixColors(uint16_t newpix, uint16_t bufpix, uint8_t alpha)
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
    FORCE_INLINE_FUNCTION uint16_t mixColors(uint16_t newpix, uint16_t bufpix, uint8_t alpha)
    {
        return mixColors(newpix & RMASK, newpix & GMASK, newpix & BMASK, bufpix, alpha);
    }

    /**
     * @fn FORCE_INLINE_FUNCTION uint16_t AbstractPainterRGB565::mixColors(uint16_t R, uint16_t G, uint16_t B, uint16_t bufpix, uint8_t alpha)
     *
     * @brief Mix colors.
     *
     *        Mix colors from a new pixel and a buffer pixel with the given alpha applied to the
     *        new pixel.
     *
     * @param R      The red color (placed in 0xF800).
     * @param G      The green color (placed in 0x03E0).
     * @param B      The blue color (placed in 0x001F).
     * @param bufpix The bufpix value.
     * @param alpha  The alpha of the newpix.
     *
     * @return The new color to write to the frame buffer.
     */
    FORCE_INLINE_FUNCTION uint16_t mixColors(uint16_t R, uint16_t G, uint16_t B, uint16_t bufpix, uint8_t alpha)
    {
        uint8_t ialpha = 0xFF - alpha;
        return (((R * alpha + (bufpix & RMASK) * ialpha) / 255) & RMASK) |
               (((G * alpha + (bufpix & GMASK) * ialpha) / 255) & GMASK) |
               (((B * alpha + (bufpix & BMASK) * ialpha) / 255) & BMASK);
    }

protected:
    /**
     * @fn virtual bool AbstractPainterRGB565::renderInit()
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
     * @fn virtual bool AbstractPainterRGB565::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) = 0;
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
     * @fn virtual void AbstractPainterRGB565::renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue);
     *
     * @brief Renders the pixel.
     *
     *        Renders the pixel into the frame buffer. The colors are reduced from 8,8,8 to 5,6,
     *        5.
     *
     * @param [in] p pointer into the frame buffer where the given color should be written.
     * @param red    The red color.
     * @param green  The green color.
     * @param blue   The blue color.
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
}; // class AbstractPainterRGB565
} // namespace touchgfx

#endif // ABSTRACTPAINTERRGB565_HPP
