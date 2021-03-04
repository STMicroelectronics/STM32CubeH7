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

#ifndef LINEPROGRESS_HPP
#define LINEPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
/**
 * @class LineProgress LineProgress.hpp touchgfx/containers/progress_indicators/LineProgress.hpp
 *
 * @brief A line progress.
 *
 *        Using Line from canvas widgets, progress will be rendered as a line. The line does
 *        not need to horizontal or vertical, but can start at any coordinate and finish at any
 *        coordinate.
 *
 * @note As LineProgress uses CanvasWidgetRenderer, it is important that a buffer is set up by
 *        calling CanvasWidgetRendere::setBuffer().
 */
class LineProgress : public AbstractProgressIndicator
{
public:

    /**
     * @fn LineProgress::LineProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    LineProgress();

    /**
     * @fn virtual LineProgress::~LineProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~LineProgress();

    /**
     * @fn virtual void LineProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the line progress indicator.
     *
     *        Sets the position and dimension of the line progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the line progress indicator.
     * @param height The height of the line progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void LineProgress::setPainter(AbstractPainter& painter);
     *
     * @brief Sets a painter.
     *
     *        Sets a painter to be used for drawing the line. This can be any Painter, a simple
     *        single color painter, a bitmap painter or a custom painter.
     *
     * @param [in] painter The painter.
     */
    virtual void setPainter(AbstractPainter& painter);

    /**
     * @fn virtual void LineProgress::setStart(int x, int y);
     *
     * @brief Sets a starting point for the line.
     *
     *        Sets a starting point for the line.
     *
     * @param x The x coordinate of the start point.
     * @param y The y coordinate of the start point.
     *
     * @see setEnd
     */
    virtual void setStart(int x, int y);

    /**
     * @fn virtual void LineProgress::getStart(int& x, int& y) const;
     *
     * @brief Gets the coordinates of the starting point of the line.
     *
     *        Gets the coordinates of the starting point of the line.
     *
     * @param [out] x The x coordinate.
     * @param [out] y The y coordinate.
     */
    virtual void getStart(int& x, int& y) const;

    /**
     * @fn virtual void LineProgress::setEnd(int x, int y);
     *
     * @brief Sets an end point for the line.
     *
     *        Sets an end point for the line. When progress is at 100%, the line will go from
     *        the coordinates set by setStart() to these coordinates set by setEnd()
     *
     * @param x The x coordinate of the end point.
     * @param y The y coordinate of the end point.
     *
     * @see setStart
     */
    virtual void setEnd(int x, int y);

    /**
     * @fn virtual void LineProgress::getEnd(int& x, int& y) const;
     *
     * @brief Gets the coordinates of the end point of the line.
     *
     *        Gets the coordinates of the end point of the line. Beware that this is not the
     *        coordinates of the current progress of the line, but the coordinates when the
     *        line is at 100%.
     *
     * @param [out] x The x coordinate.
     * @param [out] y The y coordinate.
     */
    virtual void getEnd(int& x, int& y) const;

    /**
     * @fn virtual void LineProgress::setLineWidth(int width);
     *
     * @brief Sets line width.
     *
     *        Sets line width.
     *
     * @param width The width.
     *
     * @see Line::setLineWidth
     */
    virtual void setLineWidth(int width);

    /**
     * @fn virtual int LineProgress::getLineWidth() const;
     *
     * @brief Gets line width.
     *
     *        Gets line width.
     *
     * @return The line width.
     */
    virtual int getLineWidth() const;

    /**
     * @fn virtual void LineProgress::setLineEndingStyle(Line::LINE_ENDING_STYLE lineEndingStyle);
     *
     * @brief Sets line ending style.
     *
     *        Sets line ending style.
     *
     * @param lineEndingStyle The line ending style.
     *
     * @see Line::setLineEndingStyle
     */
    virtual void setLineEndingStyle(Line::LINE_ENDING_STYLE lineEndingStyle);

    /**
     * @fn virtual Line::LINE_ENDING_STYLE LineProgress::getLineEndingStyle() const
     *
     * @brief Gets line ending style.
     *
     *        Gets line ending style.
     *
     * @return The line ending style.
     */
    virtual Line::LINE_ENDING_STYLE getLineEndingStyle() const;

    /**
     * @fn virtual void LineProgress::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha.
     *
     *        Sets the alpha for the line.
     *
     * @param alpha The alpha.
     *
     * @see Line::setAlpha
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t LineProgress::getAlpha() const;
     *
     * @brief Gets the alpha.
     *
     *        Gets the alpha.
     *
     * @return The alpha.
     *
     * @see Line::getAlpha
     */
    virtual uint8_t getAlpha() const;

    virtual void setValue(int value);

protected:
    Line line;          ///< The line
    CWRUtil::Q5 startX; ///< The start x coordinate
    CWRUtil::Q5 startY; ///< The start y coordinate
    CWRUtil::Q5 endX;   ///< The end x coordinate
    CWRUtil::Q5 endY;   ///< The end y coordinate
};
}

#endif // LINEPROGRESS_HPP
