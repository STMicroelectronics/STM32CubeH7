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

#ifndef ABSTRACTPAINTER_HPP
#define ABSTRACTPAINTER_HPP

#include <stdint.h>

#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class AbstractPainter AbstractPainter.hpp touchgfx/widgets/canvas/AbstractPainter.hpp
 *
 * @brief An abstract class for creating painter classes for drawing canvas widgets.
 *
 *        An abstract class for creating painter classes for drawing canvas widgets.
 */
class AbstractPainter
{
public:

    /**
     * @fn AbstractPainter::AbstractPainter();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    AbstractPainter();

    /**
     * @fn virtual AbstractPainter::~AbstractPainter();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~AbstractPainter();

    /**
     * @fn void AbstractPainter::setOffset(uint16_t offsetX, uint16_t offsetY);
     *
     * @brief Sets the offset of the area being drawn.
     *
     *        Sets the offset of the area being drawn. This allows render() to calculate the x,
     *        y relative to the widget, and not just relative to the invalidated area.
     *
     * @param offsetX The offset x coordinate of the invalidated area relative to the widget.
     * @param offsetY The offset y coordinate of the invalidated area relative to the widget.
     */
    void setOffset(uint16_t offsetX, uint16_t offsetY);

    /**
     * @fn virtual void AbstractPainter::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers) = 0;
     *
     * @brief Paint a designated part of the RenderingBuffer.
     *
     *        Paint a designated part of the RenderingBuffer with respect to the amount of
     *        coverage of each pixel given by the parameter covers.
     *
     * @param [in] ptr Pointer to the row in the RenderingBuffer.
     * @param x        The x coordinate.
     * @param xAdjust  The minor adjustment of x (used when a pixel is smaller than a byte to
     *                 specify that the pointer should have been advanced "xAdjust" pixels
     *                 further).
     * @param y        The y coordinate.
     * @param count    Number of pixels to fill.
     * @param covers   The coverage in of each pixel.
     */
    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers) = 0;

protected:

    /**
     * @fn void AbstractPainter::setWidgetAlpha(uint8_t alpha);
     *
     * @brief Sets widget alpha.
     *
     *        Sets the widget alpha to allow an entire canvas widget to easily be faded without
     *        changing the painter of the widget.
     *
     * @param alpha The alpha.
     *
     * @note Used internally by Canvas.
     */
    void setWidgetAlpha(uint8_t alpha);

    int16_t areaOffsetX; ///< The offset x coordinate of the area being drawn
    int16_t areaOffsetY; ///< The offset y coordinate of the area being drawn
    uint8_t widgetAlpha; ///< The alpha of the widget using the painter

    friend class Canvas;

    /**
     * @fn static inline bool compatibleFramebuffer(Bitmap::BitmapFormat format);
     *
     * @brief Check if the provided bitmap format matches the current framebuffer format.
     *
     *        Helper function to check if the provided bitmap format matches the current framebuffer format.
     *
     * @param format A bitmap format.
     *
     * @return True if the format matches the framebuffer format, false otherwise.
     */
    static inline bool compatibleFramebuffer(Bitmap::BitmapFormat format)
    {
        bool compat = HAL::lcd().framebufferFormat() == format;
        if (HAL::getInstance()->getAuxiliaryLCD())
        {
            compat |= HAL::getInstance()->getAuxiliaryLCD()->framebufferFormat() == format;
        }
        return compat;
    }
}; // class Painter
} // namespace touchgfx

#endif // ABSTRACTPAINTER_HPP
