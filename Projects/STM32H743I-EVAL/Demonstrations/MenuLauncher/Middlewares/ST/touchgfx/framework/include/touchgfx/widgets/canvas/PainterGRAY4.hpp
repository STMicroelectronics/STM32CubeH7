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

#ifndef PAINTERGRAY4_HPP
#define PAINTERGRAY4_HPP

#include <stdint.h>
#include <touchgfx/widgets/canvas/AbstractPainterGRAY4.hpp>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * @class PainterGRAY4 PainterGRAY4.hpp touchgfx/widgets/canvas/PainterGRAY4.hpp
 *
 * @brief A Painter that will paint using a color and an alpha value.
 *
 *        The PainterGRAY4 class allows a shape to be filled with a given color and alpha
 *        value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterGRAY4 : public AbstractPainterGRAY4
{
public:

    /**
     * @fn PainterGRAY4::PainterGRAY4(colortype color = 0, uint8_t alpha = 255);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param color the color.
     * @param alpha the alpha.
     */
    PainterGRAY4(colortype color = 0, uint8_t alpha = 255);

    /**
     * @fn void PainterGRAY4::setColor(colortype color, uint8_t alpha = 255);
     *
     * @brief Sets color and alpha to use when drawing the CanvasWidget.
     *
     *        Sets color and alpha to use when drawing the CanvasWidget.
     *
     * @param color The color.
     * @param alpha The alpha.
     */
    void setColor(colortype color, uint8_t alpha = 255);

    /**
     * @fn colortype PainterGRAY4::getColor() const;
     *
     * @brief Gets the current color.
     *
     *        Gets the current color.
     *
     * @return The color.
     */
    colortype getColor() const;

    /**
     * @fn void PainterGRAY4::setAlpha(uint8_t alpha);
     *
     * @brief Sets an alpha value for the painter.
     *
     *        Sets an alpha value for the painter.
     *
     * @param alpha The alpha value to use.
     */
    void setAlpha(uint8_t alpha);

    /**
     * @fn uint8_t PainterGRAY4::getAlpha() const;
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
    virtual bool renderNext(uint8_t& gray, uint8_t& alpha);

    uint8_t painterGray;  ///< The grey color
    uint8_t painterAlpha; ///< The alpha value
}; // class PainterGRAY4
} // namespace touchgfx

#endif // PAINTERGRAY4_HPP
