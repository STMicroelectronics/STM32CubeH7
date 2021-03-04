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

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>

#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

namespace touchgfx
{
/**
 * @class Circle Circle.hpp touchgfx/widgets/canvas/Circle.hpp
 *
 * @brief Simple widget capable of drawing a circle.
 *
 *        Simple widget capable of drawing a circle. By tweaking the parameters of the circle,
 *        several parameters of the circle can be changed. Center, radius, line width, line cap
 *        and partial circle arc. This opens for creation of fascinating graphics.
 *
 * @see CanvasWidget
 */
class Circle : public CanvasWidget
{
public:

    /**
     * @fn Circle::Circle();
     *
     * @brief Constructs a new Circle.
     *
     *        Constructs a new Circle.
     */
    Circle();

    /**
     * @fn template <typename T> void Circle::setCircle(const T x, const T y, const T r)
     *
     * @brief Sets the center and radius of the Circle.
     *
     *        Sets the center and radius of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of center.
     * @param y The y coordinate of center.
     * @param r The radius.
     *
     * @note The area containing the Circle is not invalidated.
     */
    template <typename T>
    void setCircle(const T x, const T y, const T r)
    {
        setCenter<T>(x, y);
        setRadius<T>(r);
    }

    /**
     * @fn void Circle::setCircle(const int16_t x, const int16_t y, const int16_t r)
     *
     * @brief Sets the center and radius of the Circle.
     *
     *        Sets the center and radius of the Circle.
     *
     * @param x The x coordinate of center.
     * @param y The y coordinate of center.
     * @param r The radius.
     *
     * @note The area containing the Circle is not invalidated.
     */
    void setCircle(const int16_t x, const int16_t y, const int16_t r)
    {
        setCircle<int>(x, y, r);
    }

    /**
     * @fn template <typename T> void Circle::setCenter(const T x, const T y)
     *
     * @brief Sets the center of the Circle.
     *
     *        Sets the center of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of center.
     * @param y The y coordinate of center.
     *
     * @note The area containing the Circle is not invalidated.
     */
    template <typename T>
    void setCenter(const T x, const T y)
    {
        this->circleCenterX = CWRUtil::toQ5<T>(x);
        this->circleCenterY = CWRUtil::toQ5<T>(y);
    }

    /**
     * @fn void Circle::setCenter(const int16_t x, const int16_t y)
     *
     * @brief Sets the center of the Circle.
     *
     *        Sets the center of the Circle.
     *
     * @param x The x coordinate of center.
     * @param y The y coordinate of center.
     *
     * @note The area containing the Circle is not invalidated.
     */
    void setCenter(const int16_t x, const int16_t y)
    {
        setCenter<int>(x, y);
    }

    /**
     * @fn template <typename T> void Circle::getCenter(T& x, T& y) const
     *
     * @brief Gets the center coordinates of the Circle.
     *
     *        Gets the center coordinates of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] x The x coordinate of the center.
     * @param [out] y The y coordinate of the center.
     */
    template <typename T>
    void getCenter(T& x, T& y) const
    {
        x = circleCenterX.to<T>();
        y = circleCenterY.to<T>();
    }

    /**
     * @fn template <typename T> void Circle::setRadius(const T r)
     *
     * @brief Sets the radius of the Circle.
     *
     *        Sets the radius of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param r The radius.
     *
     * @note The area containing the Circle is not invalidated.
     */
    template <typename T>
    void setRadius(const T r)
    {
        this->circleRadius = CWRUtil::toQ5<T>(r);
    }

    /**
     * @fn template <typename T> void Circle::getRadius(T& r) const
     *
     * @brief Gets the radius of the Circle.
     *
     *        Gets the radius of the Circle.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] r The radius.
     */
    template <typename T>
    void getRadius(T& r) const
    {
        r = circleRadius.to<T>();
    }

    /**
     * @fn template <typename T> void Circle::setArc(const T startAngle, const T endAngle)
     *
     * @brief Sets the start and end angles in degrees of the Circle arc.
     *
     *        Sets the start and end angles in degrees of the Circle arc. 0 degrees is straight up
     *        (12 o'clock) and 90 degrees is to the left (3 o'clock). Any positive or negative
     *        degrees can be used to specify the part of the Circle to draw.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param startAngle The start degrees.
     * @param endAngle   The end degrees.
     *
     * @note The area containing the Circle is not invalidated.
     *
     * @see getArc
     * @see updateArcStart
     * @see updateArcEnd
     * @see updateArc
     */
    template <typename T>
    void setArc(const T startAngle, const T endAngle)
    {
        circleArcAngleStart = CWRUtil::toQ5<T>(startAngle);
        circleArcAngleEnd = CWRUtil::toQ5<T>(endAngle);
    }

    /**
     * @fn void Circle::setArc(const int16_t startAngle, const int16_t endAngle)
     *
     * @brief Sets the start and end angles in degrees of the Circle arc.
     *
     *        Sets the start and end angles in degrees of the Circle arc. 0 degrees is straight up
     *        (12 o'clock) and 90 degrees is to the left (3 o'clock). Any positive or negative
     *        degrees can be used to specify the part of the Circle to draw.
     *
     * @param startAngle The start degrees.
     * @param endAngle   The end degrees.
     *
     * @note The area containing the Circle is not invalidated.
     *
     * @see getArc
     * @see updateArcStart
     * @see updateArcEnd
     * @see updateArc
     */
    void setArc(const int16_t startAngle, const int16_t endAngle)
    {
        setArc<int>(startAngle, endAngle);
    }

    /**
     * @fn template <typename T> void Circle::getArc(T& startAngle, T& endAngle) const
     *
     * @brief Gets the start and end angles in degrees for the circle arc.
     *
     *        Gets the start and end angles in degrees for the circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] startAngle The start.
     * @param [out] endAngle   The end.
     *
     * @see setArc
     */
    template <typename T>
    void getArc(T& startAngle, T& endAngle) const
    {
        startAngle = circleArcAngleStart.to<T>();
        endAngle = circleArcAngleEnd.to<T>();
    }

    /**
     * @fn int16_t Circle::getArcStart() const
     *
     * @brief Gets the start angle in degrees for the arc.
     *
     *        Gets the start angle in degrees for the arc.
     *
     * @return The starting angle for the arc.
     *
     * @see getArc
     * @see setArc
     */
    int16_t getArcStart() const
    {
        return circleArcAngleStart.to<int>();
    }

    /**
     * @fn template <typename T> void Circle::getArcStart(T& angle) const
     *
     * @brief Gets the start angle in degrees for the arc.
     *
     *        Gets the start angle in degrees for the arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [in,out] angle The angle.
     *
     * @see getArc
     * @see setArc
     */
    template <typename T>
    void getArcStart(T& angle) const
    {
        angle = circleArcAngleStart.to<T>();
    }

    /**
     * @fn template <typename T> T Circle::getArcEnd() const
     *
     * @brief Gets the end angle in degrees for the arc.
     *
     *        Gets the end angle in degrees for the arc.
     *
     * @tparam T Generic type parameter, either int (default) or float.
     *
     * @return The finishing angle for the arc.
     *
     * @see getArc
     * @see setArc
     */
    int16_t getArcEnd() const
    {
        return circleArcAngleEnd.to<int>();
    }

    /**
     * @fn template <typename T> void Circle::getArcEnd(T& angle) const
     *
     * @brief Gets the end angle in degrees for the arc.
     *
     *        Gets the end angle in degrees for the arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [in,out] angle The angle.
     */
    template <typename T>
    void getArcEnd(T& angle) const
    {
        angle = circleArcAngleEnd.to<T>();
    }

    /**
     * @fn template <typename T> void Circle::updateArcStart(const T startAngle)
     *
     * @brief Updates the start angle in degrees for this Circle arc.
     *
     *        Updates the start angle in degrees for this Circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param startAngle The start angle in degrees.
     *
     * @note The area containing the updated Circle arc is invalidated.
     *
     * @see setArc
     * @see updateArcEnd
     * @see updateArc
     */
    template <typename T>
    void updateArcStart(const T startAngle)
    {
        CWRUtil::Q5 startAngleQ5 = CWRUtil::toQ5<T>(startAngle);
        if (circleArcAngleStart == startAngleQ5)
        {
            return;
        }

        Rect minimalRect = getMinimalRectForUpdatedStartAngle(startAngleQ5);

        circleArcAngleStart = startAngleQ5;

        invalidateRect(minimalRect);
    }

    /**
     * @fn template <typename T> void Circle::updateArcEnd(const T endAngle)
     *
     * @brief Updates the end angle in degrees for this Circle arc.
     *
     *        Updates the end angle in degrees for this Circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param endAngle The end angle in degrees.
     *
     * @note The area containing the updated Circle arc is invalidated.
     *
     * @see setArc
     * @see updateArcStart
     * @see updateArc
     */
    template <typename T>
    void updateArcEnd(const T endAngle)
    {
        CWRUtil::Q5 endAngleQ5 = CWRUtil::toQ5(endAngle);
        if (circleArcAngleEnd == endAngleQ5)
        {
            return;
        }

        Rect minimalRect = getMinimalRectForUpdatedEndAngle(endAngleQ5);

        circleArcAngleEnd = endAngleQ5;

        invalidateRect(minimalRect);
    }

    /**
     * @fn template <typename T> void Circle::updateArc(const T startAngle, const T endAngle)
     *
     * @brief Updates the start and end angle in degrees for this Circle arc.
     *
     *        Updates the start and end angle in degrees for this Circle arc.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param startAngle The new start angle in degrees.
     * @param endAngle   The new end angle in degrees.
     *
     * @note The areas containing the updated Circle arcs are invalidated.
     *
     * @see setArc
     * @see getArc
     * @see updateArcStart
     * @see updateArcEnd
     */
    template <typename T>
    void updateArc(const T startAngle, const T endAngle)
    {
        CWRUtil::Q5 startAngleQ5 = CWRUtil::toQ5<T>(startAngle);
        CWRUtil::Q5 endAngleQ5 = CWRUtil::toQ5(endAngle);
        if (circleArcAngleStart == startAngleQ5 && circleArcAngleEnd == endAngleQ5)
        {
            return;
        }

        // Make sure old start < end
        if (circleArcAngleStart > circleArcAngleEnd)
        {
            CWRUtil::Q5 tmp = circleArcAngleStart;
            circleArcAngleStart = circleArcAngleEnd;
            circleArcAngleEnd = tmp;
        }
        // Make sure new start < end
        if (startAngleQ5 > endAngleQ5)
        {
            CWRUtil::Q5 tmp = startAngleQ5;
            startAngleQ5 = endAngleQ5;
            endAngleQ5 = tmp;
        }

        // Nice constant
        const CWRUtil::Q5 _360 = CWRUtil::toQ5<int>(360);

        // Get old circle range start in [0..360[
        if (circleArcAngleStart >= _360)
        {
            int x = (circleArcAngleStart / _360).to<int>();
            circleArcAngleStart = circleArcAngleStart - _360 * x;
            circleArcAngleEnd = circleArcAngleEnd - _360 * x;
        }
        else if (circleArcAngleStart < 0)
        {
            int x = 1 + ((-circleArcAngleStart) / _360).to<int>();
            circleArcAngleStart = circleArcAngleStart + _360 * x;
            circleArcAngleEnd = circleArcAngleEnd + _360 * x;
        }
        // Detect full circle
        if ((circleArcAngleEnd - circleArcAngleStart) > _360)
        {
            circleArcAngleEnd = circleArcAngleStart + _360;
        }

        // Get new circle range start in [0..360[
        if (startAngleQ5 >= _360)
        {
            int x = (startAngleQ5 / _360).to<int>();
            startAngleQ5 = startAngleQ5 - _360 * x;
            endAngleQ5 = endAngleQ5 - _360 * x;
        }
        else if (startAngleQ5 < 0)
        {
            int x = 1 + (-startAngleQ5 / _360).to<int>();
            startAngleQ5 = startAngleQ5 + _360 * x;
            endAngleQ5 = endAngleQ5 + _360 * x;
        }
        // Detect full circle
        if ((endAngleQ5 - startAngleQ5) >= _360)
        {
            // Align full new circle with old start.
            // So old[90..270] -> new[0..360] becomes new[90..450] for smaller invalidated area
            startAngleQ5 = circleArcAngleStart;
            endAngleQ5 = startAngleQ5 + _360;
        }
        else if ((circleArcAngleEnd - circleArcAngleStart) >= _360)
        {
            // New circle is not full, but old is. Align old circle with new.
            // So old[0..360] -> new[90..270] becomes old[90..450] for smaller invalidated area
            circleArcAngleStart = startAngleQ5;
            circleArcAngleEnd = circleArcAngleStart + _360;
        }

        // New start is after old end. Could be overlap
        // if old[10..30]->new[350..380] becomes new[-10..20]
        if (startAngleQ5 > circleArcAngleEnd && endAngleQ5 - _360 >= circleArcAngleStart)
        {
            startAngleQ5 = startAngleQ5 - _360;
            endAngleQ5 = endAngleQ5 - _360;
        }
        // Same as above but for old instead of new
        if (circleArcAngleStart > endAngleQ5 && circleArcAngleEnd - _360 >= startAngleQ5)
        {
            circleArcAngleStart = circleArcAngleStart - _360;
            circleArcAngleEnd = circleArcAngleEnd - _360;
        }

        Rect r;
        if (startAngleQ5 > circleArcAngleEnd || endAngleQ5 < circleArcAngleStart)
        {
            // Arcs do not overlap. Invalidate both arcs.
            r = getMinimalRect(circleArcAngleStart, circleArcAngleEnd);
            invalidateRect(r);

            r = getMinimalRect(startAngleQ5, endAngleQ5);
            invalidateRect(r);
        }
        else
        {
            // Arcs overlap. Invalidate both ends.
            if (circleArcAngleStart != startAngleQ5)
            {
                r = getMinimalRectForUpdatedStartAngle(startAngleQ5);
                invalidateRect(r);
            }
            if (circleArcAngleEnd != endAngleQ5)
            {
                r = getMinimalRectForUpdatedEndAngle(endAngleQ5);
                invalidateRect(r);
            }
        }

        circleArcAngleStart = CWRUtil::toQ5<T>(startAngle);
        circleArcAngleEnd = CWRUtil::toQ5(endAngle);
    }

    /**
     * @fn template <typename T> void Circle::setLineWidth(const T width)
     *
     * @brief Sets the line width for this Circle.
     *
     *        Sets the line width for this Circle. If the line width is set to zero, the circle
     *        will be filled.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param width The width of the line measured in pixels.
     *
     * @note The area containing the Circle is not invalidated.
     * @note if the new line with is smaller than the old width, the circle should be invalidated
     *       before updating the width to ensure that the old circle is completely erased.
     */
    template <typename T>
    void setLineWidth(const T width)
    {
        this->circleLineWidth = CWRUtil::toQ5<T>(width);
    }

    /**
     * @fn template <typename T> void Circle::getLineWidth(T& width) const
     *
     * @brief Gets line width.
     *
     *        Gets line width.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] width The width.
     *
     * @see setLineWidth
     */
    template <typename T>
    void getLineWidth(T& width) const
    {
        width = circleLineWidth.to<T>();
    }

    /**
     * @fn void Circle::setPrecision(const int precision);
     *
     * @brief Sets a precision of the Circle drawing function.
     *
     *        Sets a precision of the Circle drawing function. The number given as precision is the
     *        number of degrees used as step counter when drawing smaller line fragments around the
     *        circumference of the circle, five being a sensible value. Higher values results in
     *        less nice circles but faster rendering. Large circles might need a precision smaller
     *        than five.
     *
     * @param precision The precision measured in degrees.
     *
     * @note The circle is not invalidated.
     */
    void setPrecision(const int precision);

    /**
     * @fn int Circle::getPrecision() const;
     *
     * @brief Gets the precision of the circle drawing function.
     *
     *        Gets the precision of the circle drawing function. The precision is the
     *        number of degrees used as step counter when drawing smaller line fragments around the
     *        circumference of the circle, the default being being 5.
     *
     * @return The precision.
     *
     * @see setPrecision
     */
    int getPrecision() const;

    /**
     * @fn void Circle::setCapPrecision(const int precision);
     *
     * @brief Sets the precision of the ends of the Circle arc.
     *
     *        Sets a precision of the ends of the Circle arc. The precision is given in degrees
     *        where 180 is the default which results in a square ended arc (aka "butt cap"). 90
     *        will draw "an arrow head" and smaller values gives a round cap. Larger values of
     *        precision results in faster rendering of the circle.
     *
     * @param precision The new cap precision.
     *
     * @note The circle is not invalidated.
     */
    void setCapPrecision(const int precision);

    /**
     * @fn int Circle::getCapPrecision() const;
     *
     * @brief Sets the precision of the ends of the Circle arc.
     *
     *        Gets the precision of the ends of the Circle arc.
     *
     * @return The cap precision in degrees.
     *
     * @see getCapPrecision
     */
    int getCapPrecision() const;

    /**
     * @fn virtual bool Circle::drawCanvasWidget(const Rect& invalidatedArea) const;
     *
     * @brief Draws the Circle.
     *
     *        Draws the Circle. This class supports partial drawing, so only the area described by
     *        the rectangle will be drawn.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @return true if it succeeds, false if it fails.
     */
    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect Circle::getMinimalRect() const;
     *
     * @brief Gets minimal rectangle for the current shape of the circle.
     *
     *        Gets minimal rectangle for the current shape of the circle.
     *
     * @return The minimal rectangle.
     */
    virtual Rect getMinimalRect() const;

    /**
     * @fn Rect Circle::getMinimalRect(int16_t arcStart, int16_t arcEnd) const;
     *
     * @brief Gets minimal rectangle containing a given circle arc.
     *
     *        Gets minimal rectangle containing a given circle arc.
     *
     * @param arcStart The arc start.
     * @param arcEnd   The arc end.
     *
     * @return The minimal rectangle.
     */
    Rect getMinimalRect(int16_t arcStart, int16_t arcEnd) const;

    /**
     * @fn Rect Circle::getMinimalRect(CWRUtil::Q5 arcStart, CWRUtil::Q5 arcEnd) const;
     *
     * @brief Gets minimal rectangle containing a given circle arc.
     *
     *        Gets minimal rectangle containing a given circle arc.
     *
     * @param arcStart The arc start.
     * @param arcEnd   The arc end.
     *
     * @return The minimal rectangle.
     */
    Rect getMinimalRect(CWRUtil::Q5 arcStart, CWRUtil::Q5 arcEnd) const;

private:
    CWRUtil::Q5 circleCenterX;       ///< The circle center x coordinate
    CWRUtil::Q5 circleCenterY;       ///< The circle center y coordinate
    CWRUtil::Q5 circleRadius;        ///< The circle radius
    CWRUtil::Q5 circleArcAngleStart; ///< The circle arc angle start
    CWRUtil::Q5 circleArcAngleEnd;   ///< The circle arc angle end
    CWRUtil::Q5 circleLineWidth;     ///< Width of the circle line
    uint8_t circleArcIncrement;      ///< The circle arc increment
    uint8_t circleCapArcIncrement;   ///< The circle cap arc increment

    void moveToAR2(Canvas& canvas, const CWRUtil::Q5& angle, const CWRUtil::Q5& r2) const;
    void lineToAR2(Canvas& canvas, const CWRUtil::Q5& angle, const CWRUtil::Q5& r2) const;
    void lineToXYAR2(Canvas& canvas, const CWRUtil::Q5& x, const CWRUtil::Q5& y, const CWRUtil::Q5& angle, const CWRUtil::Q5& r2) const;
    void updateMinMaxAR(const CWRUtil::Q5& a, const CWRUtil::Q5& r2, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const;
    void updateMinMaxXY(const CWRUtil::Q5& xNew, const CWRUtil::Q5& yNew, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const;
    void calculateMinimalRect(CWRUtil::Q5 arcStart, CWRUtil::Q5 arcEnd, CWRUtil::Q5& xMin, CWRUtil::Q5& xMax, CWRUtil::Q5& yMin, CWRUtil::Q5& yMax) const;
    Rect getMinimalRectForUpdatedStartAngle(const CWRUtil::Q5& startAngleQ5) const;
    Rect getMinimalRectForUpdatedEndAngle(const CWRUtil::Q5& endAngleQ5) const;
};
} // namespace touchgfx

#endif // CIRCLE_HPP
