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

#ifndef BOXWITHBORDER_HPP
#define BOXWITHBORDER_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class BoxWithBorder BoxWithBorder.hpp touchgfx/containers/buttons/BoxWithBorder.hpp
 *
 * @brief A box with border.
 */
class BoxWithBorder : public Widget
{
public:
    /**
     * @fn BoxWithBorder::BoxWithBorder()
     *
     * @brief Default constructor.
     */
    BoxWithBorder() : Widget(), alpha(255), color(0), borderColor(0), borderSize(0)
    {
    }

    /**
     * @fn BoxWithBorder::BoxWithBorder(uint16_t width, uint16_t height, colortype color, colortype borderColor, uint8_t borderSize, uint8_t alpha = 255)
     *
     * @brief Constructor.
     *
     * @param width       The width.
     * @param height      The height.
     * @param color       The color.
     * @param borderColor The border color.
     * @param borderSize  Size of the border.
     * @param alpha       (Optional) The alpha.
     */
    BoxWithBorder(uint16_t width, uint16_t height, colortype color, colortype borderColor, uint8_t borderSize, uint8_t alpha = 255) : Widget(),
        alpha(alpha), color(color)
    {
        rect.width = width;
        rect.height = height;
    }

    /**
     * @fn virtual BoxWithBorder::~BoxWithBorder()
     *
     * @brief Destructor.
     */
    virtual ~BoxWithBorder() {}

    /**
     * @fn virtual Rect BoxWithBorder::getSolidRect() const;
     *
     * @brief Gets solid rectangle.
     *
     * @return The solid rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn void BoxWithBorder::setColor(colortype color)
     *
     * @brief Sets a color.
     *
     * @param color The color.
     */
    void setColor(colortype color)
    {
        this->color = color;
    }

    /**
     * @fn inline colortype BoxWithBorder::getColor() const
     *
     * @brief Gets the color.
     *
     * @return The color.
     */
    inline colortype getColor() const
    {
        return color;
    }

    /**
     * @fn void BoxWithBorder::setBorderColor(colortype color)
     *
     * @brief Sets border color.
     *
     * @param color The color.
     */
    void setBorderColor(colortype color)
    {
        this->borderColor = color;
    }

    /**
     * @fn inline colortype BoxWithBorder::getBorderColor() const
     *
     * @brief Gets border color.
     *
     * @return The border color.
     */
    inline colortype getBorderColor() const
    {
        return borderColor;
    }

    /**
     * @fn void BoxWithBorder::setBorderSize(uint8_t size)
     *
     * @brief Sets border size.
     *
     * @param size The size.
     */
    void setBorderSize(uint8_t size)
    {
        borderSize = size;
    }

    /**
     * @fn inline uint8_t BoxWithBorder::getBorderSize() const
     *
     * @brief Gets border size.
     *
     * @return The border size.
     */
    inline uint8_t getBorderSize() const
    {
        return borderSize;
    }

    /**
     * @fn void BoxWithBorder::setAlpha(uint8_t alpha)
     *
     * @brief Sets an alpha.
     *
     * @param alpha The alpha.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn inline uint8_t BoxWithBorder::getAlpha() const
     *
     * @brief Gets the alpha.
     *
     * @return The alpha.
     */
    inline uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual void BoxWithBorder::draw(const Rect& area) const;
     *
     * @brief Draws the given area.
     *
     * @param area The area.
     */
    virtual void draw(const Rect& area) const;

    /**
     * @fn virtual uint16_t BoxWithBorder::getType() const
     *
     * @brief Gets the type.
     *
     * @return The type.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_BOX;
    }

protected:
    uint8_t     alpha;         ///< The alpha
    colortype   color;         ///< The color
    colortype   borderColor;   ///< The border color
    uint8_t     borderSize;    ///< Size of the border
};
} // namespace touchgfx

#endif // BOXWITHBORDER_HPP
