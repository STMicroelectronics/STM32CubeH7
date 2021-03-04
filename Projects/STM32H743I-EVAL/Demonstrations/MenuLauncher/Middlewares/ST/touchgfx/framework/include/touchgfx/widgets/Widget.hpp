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

#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <assert.h>
#include <touchgfx/Drawable.hpp>
namespace touchgfx
{
/**
 * @class Widget Widget.hpp touchgfx/widgets/Widget.hpp
 *
 * @brief A Widget is a Drawable leaf (i.e. not a container).
 *
 *        A Widget is a Drawable leaf (i.e. not a container). It does not currently contain any
 *        implementation code, since the Drawable base class handles everything related to leaf
 *        nodes. Extend this when implementing custom widgets.
 *
 * @see Drawable
 */
class Widget : public Drawable
{
public:
    /**
     * @fn Widget::Widget()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Widget() : Drawable() { }

    /**
     * @fn virtual Widget::~Widget()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Widget() { }

    /**
     * @fn virtual void Widget::getLastChild(int16_t x, int16_t y, Drawable** last)
     *
     * @brief Function for obtaining the the last child of this widget that intersects with the
     *        specified point.
     *
     *        Function for obtaining the the last child of this widget that intersects with the
     *        specified point. Used in input event handling for obtaining the appropriate drawable
     *        that should receive the event. Note that input events must be delegated to the last
     *        drawable of the tree (meaning highest z-order / front-most drawable).
     *
     *        Only containers can have children, so this implementation simply yields itself as
     *        result. The container implementation will filter children that do not intersect with
     *        the point or are not visible/enabled, so performing those checks are unnecessary.
     *
     * @param x          The point of intersection expressed in coordinates relative to the parent.
     * @param y          The y coordinate.
     * @param [out] last Result will be placed here.
     */
    virtual void getLastChild(int16_t x, int16_t y, Drawable** last)
    {
        if (isVisible() && isTouchable())
        {
            *last = this;
        }
    }

    /**
     * @fn virtual uint16_t Widget::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_WIDGET.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_WIDGET;
    }
};
} // namespace touchgfx

#endif // WIDGET_HPP
