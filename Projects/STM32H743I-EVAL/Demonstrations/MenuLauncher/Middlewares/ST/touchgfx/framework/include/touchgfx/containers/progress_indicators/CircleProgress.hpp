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

#ifndef CIRCLEPROGRESS_HPP
#define CIRCLEPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterRGB565.hpp>

namespace touchgfx
{
/**
 * @class CircleProgress CircleProgress.hpp touchgfx/containers/progress_indicators/CircleProgress.hpp
 *
 * @brief A circle progress.
 *
 *        A circle progress indicator uses CWR for drawing the arc of a circle to show progress,
 *        and so the user must create a painter for painting the circle. The circle progress is
 *        defined by setting the minimum and maximum angle of the arc.
 *
 * @see Circle
 */
class CircleProgress : public AbstractProgressIndicator
{
public:

    /**
     * @fn CircleProgress::CircleProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    CircleProgress();

    /**
     * @fn virtual CircleProgress::~CircleProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~CircleProgress();

    /**
     * @fn virtual void CircleProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the circle progress indicator.
     *
     *        Sets the position and dimension of the circle progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the circle progress indicator.
     * @param height The height of the circle progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void CircleProgress::setPainter(AbstractPainter& painter);
     *
     * @brief Sets the painter.
     *
     *        Sets the painter to use for drawing.
     *
     * @param [in,out] painter The painter.
     *
     * @see Circle::setPainter
     * @see AbstractPainter
     */
    virtual void setPainter(AbstractPainter& painter);

    /**
     * @fn virtual void CircleProgress::setCenter(int x, int y);
     *
     * @brief Sets the center.
     *
     *        Sets the center of the circle / arc.
     *
     * @param x The int to process.
     * @param y The int to process.
     */
    virtual void setCenter(int x, int y);

    /**
     * @fn virtual void CircleProgress::getCenter(int& x, int& y) const;
     *
     * @brief Gets the center coordinates.
     *
     *        Gets the center coordinates.
     *
     * @param [out] x The x coordinate.
     * @param [out] y The y coordinate.
     */
    virtual void getCenter(int& x, int& y) const;

    /**
     * @fn virtual void CircleProgress::setRadius(int r);
     *
     * @brief Sets the radius.
     *
     *        Sets the radius of the circle.
     *
     * @param r The int to process.
     *
     * @see Circle::setRadius
     */
    virtual void setRadius(int r);

    /**
     * @fn virtual int CircleProgress::getRadius() const;
     *
     * @brief Gets the radius.
     *
     *        Gets the radius.
     *
     * @return The radius.
     */
    virtual int getRadius() const;

    /**
     * @fn virtual void CircleProgress::setLineWidth(int width);
     *
     * @brief Sets line width.
     *
     *        Sets line width of the circle. If a line width of zero is specified, it has a
     *        special meaning of drawing a filled circle instead of just the circle arc.
     *
     * @param width The width.
     *
     * @see Circle::setLineWidth
     */
    virtual void setLineWidth(int width);

    /**
     * @fn virtual int CircleProgress::getLineWidth() const;
     *
     * @brief Gets line width.
     *
     *        Gets line width.
     *
     * @return The line width.
     *
     * @see setLineWidth
     */
    virtual int getLineWidth() const;

    /**
     * @fn virtual void CircleProgress::setCapPrecision(int precision);
     *
     * @brief Sets the cap precision.
     *
     *        Sets the cap precision of the circle arc. This is not used if line width is zero.
     *
     * @param precision The cap precision.
     *
     * @see Circle::setCapPrecision
     */
    virtual void setCapPrecision(int precision);

    /**
     * @fn virtual int CircleProgress::getCapPrecision() const
     *
     * @brief Gets the cap precision.
     *
     *        Gets the cap precision.
     *
     * @return The cap precision.
     */
    virtual int getCapPrecision() const
    {
        return circle.getCapPrecision();
    }

    /**
     * @fn virtual void CircleProgress::setStartEndAngle(int startAngle, int endAngle);
     *
     * @brief Sets start and end angle.
     *
     *        Sets start and end angle. By swapping end and start angles, circles can be drawn
     *        backwards.
     *
     * @param startAngle The start angle.
     * @param endAngle   The end angle.
     */
    virtual void setStartEndAngle(int startAngle, int endAngle);

    /**
     * @fn virtual int CircleProgress::getStartAngle() const;
     *
     * @brief Gets start angle.
     *
     *        Gets start angle.
     *
     * @return The start angle.
     *
     * @see setStartEndAngle
     * @see getEndAngle
     */
    virtual int getStartAngle() const;

    /**
     * @fn virtual int CircleProgress::getEndAngle() const;
     *
     * @brief Gets end angle.
     *
     *        Gets end angle. Beware that the value returned is not related to the current
     *        progress of the circle but rather the end point of the circle when it is at 100%.
     *
     * @return The end angle.
     *
     * @see setStartEndAngle
     * @see setEndAngle
     */
    virtual int getEndAngle() const;

    /**
     * @fn virtual void CircleProgress::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha.
     *
     *        Sets the alpha of the Circle. Please note, that the alpha can also be set on the
     *        Painter, but this can be controlled directly from the user app, setting alpha for
     *        the CircleProgress will set the alpha of the actual circle.
     *
     * @param alpha The alpha.
     *
     * @see getAlpha
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t CircleProgress::getAlpha() const;
     *
     * @brief Gets the alpha.
     *
     *        Gets the alpha of the circle.
     *
     * @return The alpha.
     *
     * @see setAlpha
     */
    virtual uint8_t getAlpha() const;

    virtual void setValue(int value);
protected:
    Circle circle;      ///< The circle
    int circleEndAngle; ///< The circle end angle
};
}

#endif // CIRCLEPROGRESS_HPP
