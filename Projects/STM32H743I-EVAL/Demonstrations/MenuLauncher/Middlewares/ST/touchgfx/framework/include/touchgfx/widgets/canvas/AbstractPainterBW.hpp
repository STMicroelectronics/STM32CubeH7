/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef ABSTRACTPAINTERBW_HPP
#define ABSTRACTPAINTERBW_HPP

#include <assert.h>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class AbstractPainterBW AbstractPainterBW.hpp touchgfx/widgets/canvas/AbstractPainterBW.hpp
 *
 * @brief A Painter that will paint using a color on a LCD1bpp display.
 *
 *        AbstractPainterBW is used for drawing one 1bpp displays. The color is either on or
 *        off No transparency is supported.
 *
 * @see AbstractPainter
 */
class AbstractPainterBW : public AbstractPainter
{
public:
    AbstractPainterBW()
    {
        assert(compatibleFramebuffer(Bitmap::BW) && "The chosen painter only works with BW displays");
    }

    virtual ~AbstractPainterBW() {}

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    /**
     * @fn virtual bool AbstractPainterBW::renderInit()
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
     * @fn virtual bool AbstractPainterBW::renderNext(uint8_t& color) = 0;
     *
     * @brief Get the color of the next pixel in the scan line.
     *
     *        Get the color of the next pixel in the scan line.
     *
     * @param [out] color Color of the pixel, 0 or 1.
     *
     * @return true if the pixel should be painted, false otherwise.
     */
    virtual bool renderNext(uint8_t& color) = 0;

    uint16_t currentX; ///< Current x coordinate relative to the widget
    uint16_t currentY; ///< Current y coordinate relative to the widget
}; // class AbstractPainterBW
} // namespace touchgfx

#endif // ABSTRACTPAINTERBW_HPP
