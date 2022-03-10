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

#ifndef BOX_HPP
#define BOX_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * @class Box Box.hpp touchgfx/widgets/Box.hpp
 *
 * @brief Simple widget capable of showing a rectangle of a specific color and an optional alpha.
 *
 *        Simple widget capable of showing a rectangle of a specific color and an optional
 *        alpha.
 *
 * @see Widget
 */
class Box : public Widget
{
public:

    /**
     * @fn Box::Box()
     *
     * @brief Constructor.
     *
     *        Constructs a new Box with a default alpha value of 255 (solid)
     */
    Box() : Widget(), alpha(255), color(0), reportAsSolid(false)
    {
    }

    /**
     * @fn Box::Box(uint16_t width, uint16_t height, colortype color, uint8_t alpha = 255)
     *
     * @brief Constructor.
     *
     *        Construct a Box.
     *
     * @param width  The width of the box.
     * @param height The height of the box.
     * @param color  The color of the box.
     * @param alpha  The alpha of the box. Default is 255 (solid).
     */
    Box(uint16_t width, uint16_t height, colortype color, uint8_t alpha = 255) : Widget(),
        alpha(alpha), color(color), reportAsSolid(false)
    {
        rect.width = width;
        rect.height = height;
    }

    /**
     * @fn virtual Box::~Box()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Box() {}

    virtual Rect getSolidRect() const;

    /**
     * @fn void Box::setColor(colortype color)
     *
     * @brief Sets the color of the rectangle.
     *
     *        Sets the color of the rectangle.
     *
     * @param color The color of the box.
     */
    void setColor(colortype color)
    {
        this->color = color;
    }

    /**
     * @fn inline colortype Box::getColor() const
     *
     * @brief Gets the current color of the Box.
     *
     *        Gets the current color of the Box.
     *
     * @return The current color.
     */
    inline colortype getColor() const
    {
        return color;
    }

    /**
     * @fn void Box::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha value for this Box.
     *
     *        Sets the alpha value for this Box.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn inline uint8_t Box::getAlpha() const
     *
     * @brief Returns the current alpha value.
     *
     * @return Gets the current alpha value of the Box.
     */
    inline uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual void Box::draw(const Rect& area) const;
     *
     * @brief Draws the box.
     *
     *        Draws the Box.
     *
     * @param area The rectangle to draw, with coordinates relative to the containing layer.
     */
    virtual void draw(const Rect& area) const;

    /**
     * @fn void Box::forceReportAsSolid(bool solid)
     *
     * @brief Override solid area for the Box.
     *
     *        If this is set, getSolidRect() will report the widget as completely solid even if
     *        is (semi-)transparent.
     *
     * @note Very rarely used in practice.
     *
     * @param solid true if this Box should report as solid, even when not.
     */
    void forceReportAsSolid(bool solid)
    {
        reportAsSolid = solid;
    }

    /**
     * @fn virtual uint16_t Box::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_BOX.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_BOX;
    }

protected:
    uint8_t  alpha;         ///< The alpha value used for this Box.
    colortype color;        ///< The fill color for this Box
    bool     reportAsSolid; ///< @see forceReportAsSolid.
};
} // namespace touchgfx

#endif // BOX_HPP
