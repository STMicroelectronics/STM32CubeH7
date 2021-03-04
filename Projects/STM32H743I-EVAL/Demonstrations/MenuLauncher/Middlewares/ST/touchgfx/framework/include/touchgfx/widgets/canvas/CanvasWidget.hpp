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

#ifndef CANVASWIDGET_HPP
#define CANVASWIDGET_HPP

#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>
#include <touchgfx/canvas_widget_renderer/Rasterizer.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
/**
 * @class CanvasWidget CanvasWidget.hpp touchgfx/widgets/canvas/CanvasWidget.hpp
 *
 * @brief Class for drawing complex polygons on the LCD using CanvasWidgetRenderer.
 *
 *        Class for drawing complex polygons on the LCD using CanvasWidgetRenderer.
 *
 * @see Widget
 */
class CanvasWidget : public Widget
{
public:

    /**
     * @fn CanvasWidget::CanvasWidget();
     *
     * @brief Constructor.
     *
     *        Constructor.
     */
    CanvasWidget();

    /**
     * @fn virtual CanvasWidget::~CanvasWidget();
     *
     * @brief Destructor.
     *
     *        Destructor. Declared virtual for sub classing purposes.
     */
    virtual ~CanvasWidget();

    /**
     * @fn virtual void CanvasWidget::setPainter(AbstractPainter& painter);
     *
     * @brief Sets the painter for the CanvasWidget.
     *
     *        Sets the painter for the CanvasWidget.
     *
     * @note The area containing the CanvasWidget is not invalidated.
     *
     * @param [in] painter The painter for the CanvasWidget.
     *
     * @see AbstractPainter
     */
    virtual void setPainter(AbstractPainter& painter);

    /**
     * @fn virtual AbstractPainter& CanvasWidget::getPainter() const;
     *
     * @brief Gets the current painter for the CanvasWidget.
     *
     *        Gets the current painter for the CanvasWidget.
     *
     * @return The painter.
     *
     * @see AbstractPainter
     */
    virtual AbstractPainter& getPainter() const;

    /**
     * @fn virtual void CanvasWidget::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha channel for the CanvasWidget.
     *
     *        Sets the alpha channel for the CanvasWidget.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    virtual void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn virtual uint8_t CanvasWidget::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     */
    virtual uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual void CanvasWidget::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the given invalidated area.
     *
     *        Draws the given invalidated area. If the underlying CanvasWidgetRenderer fail to
     *        render the widget (due to memory limitations), the invalidated area is cut into
     *        smaller slices which are then drawn separately. If drawing a single raster line
     *        fails, that line is skipped (left blank/transparent) and drawing continues on the
     *        next raster line.
     *
     *        If drawing has failed at least once, the number of successfully drawn lines is
     *        remembered for the next time. If a future draw would need to draw more lines, the
     *        area is automatically divided into smaller areas to prevent drawing the canvas
     *        widget in vain.
     *
     * @param invalidatedArea The invalidated area.
     *
     * @see drawCanvasWidget()
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual void CanvasWidget::invalidate() const;
     *
     * @brief Invalidates the area covered by this CanvasWidget.
     *
     *        Invalidates the area covered by this CanvasWidget. Since many widgets are a lot
     *        smaller than the actual size of the canvas widget, each widget must be able to
     *        tell the smallest rectangle completely containing the shape drawn by the widget.
     *        For example a circle arc is typically much smaller than the widget containing the
     *        circle.
     *
     * @see getMinimalRect()
     */
    virtual void invalidate() const;

    /**
     * @fn virtual Rect CanvasWidget::getMinimalRect() const;
     *
     * @brief Gets minimal rectangle containing the shape drawn by this widget.
     *
     *        Gets minimal rectangle containing the shape drawn by this widget. Default
     *        implementation returns the size of the entire widget, but this function should be
     *        overwritten in subclasses and return the minimal rectangle containing the shape.
     *        See classes such as Circle for example implementations.
     *
     * @return The minimal rectangle containing the shape drawn by this widget.
     */
    virtual Rect getMinimalRect() const;

    /**
     * @fn virtual Rect CanvasWidget::getSolidRect() const;
     *
     * @brief Gets the largest solid (non-transparent) rectangle.
     *
     *        Gets the largest solid (non-transparent) rectangle. Since canvas widgets
     *        typically do not have a solid rect, it is recommended to return an empty
     *        rectangle.
     *
     * @return The largest solid (non-transparent) rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual bool CanvasWidget::drawCanvasWidget(const Rect& invalidatedArea) const = 0;
     *
     * @brief Draw canvas widget for the given invalidated area.
     *
     *        Draw canvas widget for the given invalidated area. Similar to draw(), but might
     *        be invoked with several times with smaller areas to due to memory constraints
     *        from the underlying CanvasWidgetRenderer.
     *
     * @param invalidatedArea The invalidated area.
     *
     * @return true the widget was drawn, false if not.
     *
     * @see draw()
     */
    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const = 0;

private:
    AbstractPainter* canvasPainter;
    mutable int16_t maxRenderLines;
    uint8_t alpha;
};
} // namespace touchgfx

#endif // CANVASWIDGET_HPP
