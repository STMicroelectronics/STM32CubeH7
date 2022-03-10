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

#ifndef PAINTERBW_HPP
#define PAINTERBW_HPP

#include <touchgfx/widgets/canvas/AbstractPainterBW.hpp>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class PainterBW PainterBW.hpp touchgfx/widgets/canvas/PainterBW.hpp
 *
 * @brief A Painter that will paint using a color on a LCD1bpp display.
 *
 *        PainterBW is used for drawing one 1bpp displays. The color is either on or off No
 *        transparency is supported.
 *
 * @see AbstractPainter
 */
class PainterBW : public AbstractPainterBW
{
public:
    /**
     * @fn static unsigned PainterBW::bw(unsigned red, unsigned green, unsigned blue);
     *
     * @brief Convert color to black/white.
     *
     *        Converts the selected color to either white (1) or black (0) depending on the
     *        converted gray value.
     *
     * @param red   The red color.
     * @param green The green color.
     * @param blue  The blue color.
     *
     * @return 1 (white) if the brightness of the RGB color is above 50% and 0 (black) otherwise.
     */
    static unsigned bw(unsigned red, unsigned green, unsigned blue);

    /**
     * @fn void PainterBW::setColor(colortype color);
     *
     * @brief Sets color to use when drawing the CanvasWidget.
     *
     *        Sets color to use when drawing the CanvasWidget.
     *
     * @param color The color, 0=black, otherwise white.
     */
    void setColor(colortype color);

    /**
     * @fn colortype PainterBW::getColor() const;
     *
     * @brief Gets the current color.
     *
     *        Gets the current color.
     *
     * @return The color.
     */
    colortype getColor() const;

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderNext(uint8_t& color);

    uint8_t painterColor; ///< The color to use when painting
}; // class PainterBW
} // namespace touchgfx

#endif // PAINTERBW_HPP
