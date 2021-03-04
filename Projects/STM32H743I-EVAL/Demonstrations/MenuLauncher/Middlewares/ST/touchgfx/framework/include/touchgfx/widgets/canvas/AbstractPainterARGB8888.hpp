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

#ifndef ABSTRACTPAINTERARGB8888_HPP
#define ABSTRACTPAINTERARGB8888_HPP

#include <assert.h>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class AbstractPainterARGB8888 AbstractPainterARGB8888.hpp touchgfx/widgets/canvas/AbstractPainterARGB8888.hpp
 *
 * @brief A Painter that will paint using a color and an alpha value.
 *
 *        The AbstractPainterARGB8888 class allows a shape to be filled with a given color and
 *        alpha value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class AbstractPainterARGB8888 : public AbstractPainter
{
public:
    AbstractPainterARGB8888()
    {
        assert(compatibleFramebuffer(Bitmap::ARGB8888) && "The chosen painter only works with ARGB8888 displays");
    }

    virtual ~AbstractPainterARGB8888() {}

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:

    /**
     * @fn virtual bool AbstractPainterARGB8888::renderInit()
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
     * @fn virtual bool AbstractPainterARGB8888::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) = 0;
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
     * @fn virtual void AbstractPainterARGB8888::renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue);
     *
     * @brief Renders the pixel.
     *
     *        Renders the pixel into the frame buffer.
     *
     * @param [in] p pointer into the frame buffer where the given color should be written.
     * @param red    The red color.
     * @param green  The green color.
     * @param blue   The blue color.
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @fn virtual void AbstractPainterARGB8888::renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
     *
     * @brief Renders the pixel.
     *
     *        Renders the pixel into the frame buffer.
     *
     * @param [in] p pointer into the frame buffer where the given color should be written.
     * @param red    The red color.
     * @param green  The green color.
     * @param blue   The blue color.
     * @param alpha  The alpha.
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
}; // class AbstractPainterARGB8888
} // namespace touchgfx

#endif // ABSTRACTPAINTERARGB8888_HPP
