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

#ifndef ABSTRACTPAINTERGRAY2_HPP
#define ABSTRACTPAINTERGRAY2_HPP

#include <assert.h>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class AbstractPainterGRAY2 AbstractPainterGRAY2.hpp touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp
 *
 * @brief A Painter that will paint using a color and an alpha value.
 *
 *        The AbstractPainterGRAY2 class allows a shape to be filled with a given color and
 *        alpha value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class AbstractPainterGRAY2 : public AbstractPainter
{
public:
    AbstractPainterGRAY2()
    {
        assert(compatibleFramebuffer(Bitmap::GRAY2) && "The chosen painter only works with GRAY2 displays");
    }

    virtual ~AbstractPainterGRAY2() {}

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:

    /**
     * @fn virtual bool AbstractPainterGRAY2::renderInit()
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
     * @fn virtual bool AbstractPainterGRAY2::renderNext(uint8_t& gray, uint8_t& alpha) = 0;
     *
     * @brief Get the color of the next pixel in the scan line.
     *
     *        Get the color of the next pixel in the scan line.
     *
     * @param [out] gray  The gray (0-15).
     * @param [out] alpha The alpha.
     *
     * @return true if the pixel should be painted, false otherwise.
     */
    virtual bool renderNext(uint8_t& gray, uint8_t& alpha) = 0;

    /**
     * @fn virtual void AbstractPainterGRAY2::renderPixel(uint8_t* p, uint16_t offset, uint8_t gray);
     *
     * @brief Renders the pixel.
     *
     *        Renders the pixel into the frame buffer.
     *
     * @param [in] p pointer into the frame buffer line where the given pixel should be written.
     * @param offset The offset to the pixel from the given pointer.
     * @param gray   The green color.
     *
     * ### param blue The blue color.
     */
    virtual void renderPixel(uint8_t* p, uint16_t offset, uint8_t gray);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
}; // class AbstractPainterGRAY2
} // namespace touchgfx

#endif // ABSTRACTPAINTERGRAY2_HPP
