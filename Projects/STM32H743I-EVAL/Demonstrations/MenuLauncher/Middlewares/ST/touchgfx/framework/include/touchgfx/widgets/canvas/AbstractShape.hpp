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

#ifndef ABSTRACTSHAPE_HPP
#define ABSTRACTSHAPE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>

#include <touchgfx/widgets/canvas/CanvasWidget.hpp>

namespace touchgfx
{
/**
 * @class AbstractShape AbstractShape.hpp touchgfx/widgets/canvas/AbstractShape.hpp
 *
 * @brief Simple widget capable of drawing a abstractShape.
 *
 *        Simple widget capable of drawing a abstractShape. The abstractShape can be scaled and
 *        rotated around 0,0. Note that the y axis goes down, so a abstractShape that goes up
 *        must be given negative coordinates.
 *
 * @see CanvasWidget
 *
 * ### tparam T The type of the points used for the abstractShape. Must be int or float.
 */
class AbstractShape : public CanvasWidget
{
public:
    /**
     * @struct ShapePoint AbstractShape.hpp touchgfx/widgets/canvas/AbstractShape.hpp
     *
     * @brief Defines an alias representing the array of points making up the abstract shape.
     *
     *        Defines an alias representing the array of points making up the abstract shape.
     *        This will help setting up the abstractShape very easily using setAbstractShape().
     *
     * @tparam T Generic type parameter, either int or float.
     *
     * @see setAbstractShape()
     */
    template <typename T>
    struct ShapePoint
    {
        T x; ///< The x coordinate of the points.
        T y; ///< The y coordinate of the points.
    };

    /**
     * @fn AbstractShape::AbstractShape();
     *
     * @brief Constructs a new AbstractShape.
     *
     *        Constructs a new AbstractShape.
     */
    AbstractShape();

    /**
     * @fn virtual AbstractShape::~AbstractShape();
     *
     * @brief Virtual Destructor.
     *
     *        Virtual Destructor.
     */
    virtual ~AbstractShape();

    /**
     * @fn virtual int AbstractShape::getNumPoints() const = 0;
     *
     * @brief Gets number points used to make up the shape.
     *
     *        Gets number points used to make up the shape.
     *
     * @return The number points.
     */
    virtual int getNumPoints() const = 0;

    /**
     * @fn virtual void AbstractShape::setCorner(int i, CWRUtil::Q5 x, CWRUtil::Q5 y) = 0;
     *
     * @brief Sets a corner of the shape.
     *
     *        Sets a corner of the shape in Q5 format.
     *
     * @param i Zero-based index of the corner.
     * @param x The x coordinate in Q5 format.
     * @param y The y coordinate in Q5 format.
     *
     * @note Remember to call updateAbstractShapeCache() to make sure that the cached outline of
     *       the shape is correct.
     * @see updateAbstractShapeCache
     */
    virtual void setCorner(int i, CWRUtil::Q5 x, CWRUtil::Q5 y) = 0;

    /**
     * @fn virtual CWRUtil::Q5 AbstractShape::getCornerX(int i) const = 0;
     *
     * @brief Gets the x coordinate of a corner.
     *
     *        Gets the x coordinate of a corner.
     *
     * @param i Zero-based index of the corner.
     *
     * @return The corner x coordinate.
     */
    virtual CWRUtil::Q5 getCornerX(int i) const = 0;

    /**
     * @fn virtual CWRUtil::Q5 AbstractShape::getCornerY(int i) const = 0;
     *
     * @brief Gets the y coordinate of a corner.
     *
     *        Gets the y coordinate of a corner.
     *
     * @param i Zero-based index of the corner.
     *
     * @return The corner y coordinate.
     */
    virtual CWRUtil::Q5 getCornerY(int i) const = 0;

    /**
     * @fn template <typename T> void AbstractShape::setShape(ShapePoint<T>* points)
     *
     * @brief Sets a shape the struct Points.
     *
     *        Sets a shape the struct Points. The cached outline of the shape is automatically
     *        updated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [in] points The points that make up the shape.
     *
     * @note The area containing the shape is not invalidated.
     */
    template <typename T>
    void setShape(ShapePoint<T>* points)
    {
        int numPoints = getNumPoints();
        for (int i = 0; i < numPoints; i++)
        {
            setCorner(i, CWRUtil::toQ5<T>(points[i].x), CWRUtil::toQ5<T>(points[i].y));
        }
        updateAbstractShapeCache();
    }

    /**
     * @fn template <typename T> void AbstractShape::setShape(const ShapePoint<T>* points)
     *
     * @brief Sets a shape the struct Points.
     *
     *        Sets a shape the struct Points. The cached outline of the shape is automatically
     *        updated.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [in] points The points that make up the shape.
     *
     * @note The area containing the shape is not invalidated.
     */
    template <typename T>
    void setShape(const ShapePoint<T>* points)
    {
        int numPoints = getNumPoints();
        for (int i = 0; i < numPoints; i++)
        {
            setCorner(i, CWRUtil::toQ5<T>(points[i].x), CWRUtil::toQ5<T>(points[i].y));
        }
        updateAbstractShapeCache();
    }

    /**
     * @fn template <typename T> void AbstractShape::setOrigin(T x, T y)
     *
     * @brief Sets the position of (0,0).
     *
     *        Sets the position of (0,0) used when the abstractShape was created. This means
     *        that all coordinates initially used when created the shape are moved relative to
     *        these given offsets. Calling setOrigin() again, will not add to the previous
     *        settings of setOrigin() but will replace the old values for origin.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of the shapes position (0,0).
     * @param y The y coordinate of the shapes position (0,0).
     *
     * @note The area containing the AbstractShape is not invalidated.
     *
     * @see moveOrigin()
     */
    template <typename T>
    void setOrigin(T x, T y)
    {
        CWRUtil::Q5 dxNew = CWRUtil::toQ5<T>(x);
        CWRUtil::Q5 dyNew = CWRUtil::toQ5<T>(y);

        if (dx == dxNew && dy == dyNew)
        {
            return;
        }

        dx = dxNew;
        dy = dyNew;

        updateAbstractShapeCache();
    }

    /**
     * @fn template <typename T> void AbstractShape::moveOrigin(T x, T y)
     *
     * @brief Moves the start point for this AbstractShape.
     *
     *        Moves the start point for this AbstractShape. The rectangle that surrounds the
     *        old and new area covered by the shape will be invalidated.
     *
     *
     * @tparam T Generic type parameter, either int or float.
     * @param x The x coordinate of the shapes position (0,0).
     * @param y The y coordinate of the shapes position (0,0).
     *
     * @note The area containing the AbstractShape is invalidated before and after the change.
     *
     * @see setOrigin()
     */
    template <typename T>
    void moveOrigin(T x, T y)
    {
        CWRUtil::Q5 xNew = CWRUtil::toQ5<T>(x);
        CWRUtil::Q5 yNew = CWRUtil::toQ5<T>(y);

        if (dx == xNew && dy == yNew)
        {
            return;
        }

        Rect rect = getMinimalRect();

        dx = xNew;
        dy = yNew;

        updateAbstractShapeCache();

        rect.expandToFit(getMinimalRect());
        invalidateRect(rect);
    }

    /**
     * @fn template <typename T> void AbstractShape::getOrigin(T& dx, T& dy) const
     *
     * @brief Gets the start coordinates for the line.
     *
     *        Gets the start coordinates for the line.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] dx The x coordinate.
     * @param [out] dy The y coordinate.
     */
    template <typename T>
    void getOrigin(T& dx, T& dy) const
    {
        dx = this->dx.to<T>();
        dy = this->dy.to<T>();
    }

    /**
     * @fn template <typename T> void AbstractShape::setAngle(T angle)
     *
     * @brief Sets the angle to turn the abstractShape.
     *
     *        Sets the angle to turn the abstractShape. 0 degrees is straight up and 90 degrees
     *        is 3 o'clock.
     *
     * @tparam T Generic type parameter.
     * @param angle The angle to turn the abstractShape to relative to 0 (straight up), not relative
     *              to the previous angle.
     *
     * @note The area containing the AbstractShape is not invalidated.
     *
     * @see updateAngle()
     */
    template <typename T>
    void setAngle(T angle)
    {
        CWRUtil::Q5 angleQ5 = CWRUtil::toQ5<T>(angle);
        if (shapeAngle != angleQ5)
        {
            shapeAngle = angleQ5;
            updateAbstractShapeCache();
        }
    }

    /**
     * @fn template <typename T> void AbstractShape::getAngle(T& angle)
     *
     * @brief Gets the abstractShape's angle.
     *
     *        Gets the abstractShape's angle.
     *
     * @tparam T Generic type parameter.
     * @param [out] angle The current abstractShape angle relative to 0.
     */
    template <typename T>
    void getAngle(T& angle)
    {
        angle = this->shapeAngle.to<T>();
    }

    /**
     * @fn template <typename T> void AbstractShape::updateAngle(T angle)
     *
     * @brief Sets the angle to turn the abstractShape.
     *
     *        Sets the angle to turn the abstractShape. 0 degrees is straight up and 90 degrees
     *        is 3 o'clock.
     *
     * @tparam T Generic type parameter.
     * @param angle The angle to turn the abstractShape.
     *
     * @note The area containing the AbstractShape is invalidated before and after the change.
     *
     * @see setAngle()
     */
    template <typename T>
    void updateAngle(T angle)
    {
        CWRUtil::Q5 angleQ5 = CWRUtil::toQ5<T>(angle);
        if (shapeAngle != angleQ5)
        {
            Rect rectBefore = getMinimalRect();

            shapeAngle = angleQ5;
            updateAbstractShapeCache();

            Rect rectAfter = getMinimalRect();
            rectBefore.expandToFit(rectAfter);
            invalidateRect(rectBefore);
        }
    }

    /**
     * @fn template <typename T> T AbstractShape::getAngle() const
     *
     * @brief Gets the current angle of the abstractShape.
     *
     *        Gets the current angle of the abstractShape.
     *
     * @return The angle of the AbstractShape.
     */
    int getAngle() const
    {
        return shapeAngle.to<int>();
    }

    /**
     * @fn template <typename T> void AbstractShape::setScale(T newXScale, T newYScale)
     *
     * @brief Scale the AbstractShape.
     *
     *        Scale the AbstractShape the given amounts in the x direction and the y direction.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param newXScale The new scale in the x direction.
     * @param newYScale The new scale in the y direction.
     *
     * @note The area containing the AbstractShape is not invalidated.
     *
     * @see getScale, updateScale
     */
    template <typename T>
    void setScale(T newXScale, T newYScale)
    {
        xScale = CWRUtil::toQ10<T>(newXScale);
        yScale = CWRUtil::toQ10<T>(newYScale);
        updateAbstractShapeCache();
    }

    /**
     * @fn template <typename T> void AbstractShape::setScale(T scale)
     *
     * @brief Scale the AbstractShape.
     *
     *        Scale the AbstractShape the given amount in the x direction and the y direction.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param scale The scale in the x direction.
     *
     * @note The area containing the AbstractShape is not invalidated.
     *
     * @see getScale
     */
    template <typename T>
    void setScale(T scale)
    {
        setScale(scale, scale);
    }

    /**
     * @fn template <typename T> void AbstractShape::updateScale(T newXScale, T newYScale)
     *
     * @brief Scale the AbstractShape.
     *
     *        Scale the AbstractShape the given amounts in the x direction and the y direction.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param newXScale The new scale in the x direction.
     * @param newYScale The new scale in the y direction.
     *
     * @note The area containing the AbstractShape is invalidated before and after the change.
     *
     * @see setScale()
     */
    template <typename T>
    void updateScale(T newXScale, T newYScale)
    {
        CWRUtil::Q10 xScaleQ10 = CWRUtil::toQ10<T>(newXScale);
        CWRUtil::Q10 yScaleQ10 = CWRUtil::toQ10<T>(newYScale);

        if (xScale != xScaleQ10 || yScale != yScaleQ10)
        {
            Rect rectBefore = getMinimalRect();

            xScale = xScaleQ10;
            yScale = yScaleQ10;
            updateAbstractShapeCache();

            Rect rectAfter = getMinimalRect();
            rectBefore.expandToFit(rectAfter);
            invalidateRect(rectBefore);
        }
    }

    /**
     * @fn template <typename T> void AbstractShape::getScale(T& x, T& y) const
     *
     * @brief Gets the x scale and y scale.
     *
     *        Gets the x scale and y scale of the shape as previously set using setScale. Default
     *        is 1 for both x scale and y scale.
     *
     * @tparam T Generic type parameter, either int or float.
     * @param [out] x Scaling of x coordinates.
     * @param [out] y Scaling of y coordinates.
     *
     * @see setScale
     */
    template <typename T>
    void getScale(T& x, T& y) const
    {
        x = xScale.to<T>();
        y = yScale.to<T>();
    }

    /**
     * @fn virtual bool AbstractShape::drawCanvasWidget(const Rect& invalidatedArea) const;
     *
     * @brief Draws the AbstractShape.
     *
     *        Draws the AbstractShape. This class supports partial drawing, so only the area
     *        described by the rectangle will be drawn.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @return true if it succeeds, false if it fails.
     */
    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

    /**
     * @fn void AbstractShape::updateAbstractShapeCache();
     *
     * @brief Updates the abstractShape cache.
     *
     *        Updates the abstractShape cache. The cache is used to be able to quickly redraw
     *        the AbstractShape without calculating the points that make up the abstractShape
     *        (with regards to scaling and rotation).
     */
    void updateAbstractShapeCache();

protected:
    /**
    * @fn virtual void AbstractShape::setCache(int i, CWRUtil::Q5 x, CWRUtil::Q5 y) = 0;
    *
    * @brief Sets the cached coordinates of a given corner.
    *
    *        Sets the cached coordinates of a given corner. The coordinates in the cache are
    *        the coordinates from the corners after rotating and scaling the coordinate.
    *
    * @param i Zero-based index of the corner.
    * @param x The x coordinate.
    * @param y The y coordinate.
    */
    virtual void setCache(int i, CWRUtil::Q5 x, CWRUtil::Q5 y) = 0;

    /**
    * @fn virtual CWRUtil::Q5 AbstractShape::getCacheX(int i) const = 0;
    *
    * @brief Gets cached x coordinate of a corner.
    *
    *        Gets cached x coordinate of a corner.
    *
    * @param i Zero-based index of the corner.
    *
    * @return The cached x coordinate.
    */
    virtual CWRUtil::Q5 getCacheX(int i) const = 0;

    /**
    * @fn virtual CWRUtil::Q5 AbstractShape::getCacheY(int i) const = 0;
    *
    * @brief Gets cached y coordinate of a corner.
    *
    *        Gets cached y coordinate of a corner.
    *
    * @param i Zero-based index of the corner.
    *
    * @return The cached y coordinate.
    */
    virtual CWRUtil::Q5 getCacheY(int i) const = 0;

    /**
     * @fn virtual Rect AbstractShape::getMinimalRect() const;
     *
     * @brief Gets minimal rectangle containing the abstractShape.
     *
     *        Gets minimal rectangle containing the abstractShape. Used for invalidating only
     *        the required part of the screen.
     *
     * @return The minimal rectangle.
     */
    virtual Rect getMinimalRect() const;

private:
    CWRUtil::Q5 dx, dy;
    CWRUtil::Q5 shapeAngle;
    CWRUtil::Q10 xScale, yScale;
    Rect minimalRect;
};
} // namespace touchgfx

#endif // ABSTRACTSHAPE_HPP
