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

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <touchgfx/Application.hpp>
#include <touchgfx/Utils.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>

namespace touchgfx
{
/**
 * @class Drawable Drawable.hpp touchgfx/Drawable.hpp
 *
 * @brief The Drawable class is an abstract definition of something that can be drawn.
 *
 *        The Drawable class is an abstract definition of something that can be drawn. In the
 *        composite design pattern, the Drawable is the component interface. Drawables can be
 *        added to a screen as a tree structure through the leaf node class Widget and the
 *        Container class. A Drawable contains a pointer to its next sibling and a pointer to
 *        its parent node. These are maintained by the Container to which the Drawable is added.
 *
 *        The Drawable interface contains two pure virtual functions which must be implemented
 *        by widgets, namely draw() and getSolidRect(). In addition it contains general
 *        functionality for receiving events and navigating the tree structure.
 *
 *        The coordinates of a Drawable are always relative to its parent node.
 *
 * @see Widget
 * @see Container
 */
class Drawable
{
public:

    /**
     * @fn Drawable::Drawable()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Drawable() : rect(0, 0, 0, 0),
        parent(0),
        nextSibling(0),
        nextDrawChainElement(0),
        touchable(false),
        visible(true)
    {
    }

    /**
     * @fn virtual Drawable::~Drawable()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Drawable() { }

    /**
     * @fn virtual void Drawable::draw(const Rect& invalidatedArea) const = 0;
     *
     * @brief Pure virtual function for drawing this drawable.
     *
     *        Pure virtual function for drawing this drawable. It is a requirement that the
     *        draw implementation does not draw outside the region specified by invalidatedArea.
     *
     * @param invalidatedArea The subregion of this drawable that needs to be redrawn, expressed in
     *                        coordinates relative to its parent (e.g. for a complete redraw,
     *                        invalidatedArea will be (0, 0, width, height).
     */
    virtual void draw(const Rect& invalidatedArea) const = 0;

    /**
     * @fn virtual Rect Drawable::getSolidRect() const = 0;
     *
     * @brief Pure virtual function for obtaining the largest possible rectangle that is guaranteed
     *        to be solid (non-transparent).
     *
     *        Pure virtual function for obtaining the largest possible rectangle that is
     *        guaranteed to be solid (non-transparent). Used by JSMOC to prune the draw graph.
     *
     * @note The rectangle returned must be relative to (0, 0), meaning that to indicate a
     *       completely solid widget, Rect(0, 0, getWidth(), getHeight()) must be returned.
     *
     * @return The solid rect.
     */
    virtual Rect getSolidRect() const = 0;

    /**
     * @fn virtual void Drawable::invalidateRect(Rect& invalidatedArea) const;
     *
     * @brief Request that a subregion of this drawable is redrawn.
     *
     *        Request that a subregion of this drawable is redrawn. Will recursively traverse
     *        the tree towards the root, and once reached, issue a draw operation. When this
     *        function returns, the specified invalidated area has been redrawn for all
     *        appropriate Drawables covering the region.
     *
     * @param [in] invalidatedArea The area of this drawable to redraw expressed in coordinates
     *                             relative to its parent (e.g. to request a complete redraw,
     *                             invalidatedArea will be (0, 0, width, height).
     */
    virtual void invalidateRect(Rect& invalidatedArea) const;

    /**
     * @fn virtual void Drawable::invalidate() const;
     *
     * @brief Tell the framework that this entire drawable needs to be redrawn.
     *
     *        Tell the framework that this entire drawable needs to be redrawn.
     *
     * @see invalidateRect
     */
    virtual void invalidate() const;

    /**
     * @fn Drawable* Drawable::getNextSibling()
     *
     * @brief Gets the next sibling.
     *
     *        Returns the next sibling node. This will be the next Drawable that has
     *        been added to the same Container as this Drawable.
     *
     * @return The next sibling. If this is the last sibling, the return value is 0.
     */
    Drawable* getNextSibling()
    {
        return nextSibling;
    }

    /**
     * @fn virtual Rect Drawable::getSolidRectAbsolute();
     *
     * @brief Helper function for obtaining the largest solid rect.
     *
     *        Helper function for obtaining the largest solid rect (as implemented by
     *        getSolidRect()) expressed in absolute coordinates. Will recursively traverse to
     *        the root of the tree.
     *
     * @return Largest solid rect (as implemented by getSolidRect()) expressed in absolute
     *         coordinates.
     */
    virtual Rect getSolidRectAbsolute();

    /**
     * @fn virtual void Drawable::getLastChild(int16_t x, int16_t y, Drawable** last) = 0;
     *
     * @brief Function for obtaining the the last child of this drawable that intersects with the
     *        specified point.
     *
     *        Function for obtaining the the last child of this drawable that intersects with
     *        the specified point. Used in input event handling for obtaining the appropriate
     *        drawable that should receive the event. Note that input events must be delegated
     *        to the last drawable of the tree (meaning highest z-order / front-most drawable).
     *
     * @param x          The point of intersection expressed in coordinates relative to the parent.
     * @param y          The point of intersection expressed in coordinates relative to the parent.
     * @param [out] last Result will be placed here.
     */
    virtual void getLastChild(int16_t x, int16_t y, Drawable** last) = 0;

    /**
     * @fn virtual void Drawable::getVisibleRect(Rect& rect) const;
     *
     * @brief JSMOC Function for obtaining the visible part of this drawable.
     *
     *        JSMOC Function for obtaining the visible part of this drawable. If the parent
     *        node has a smaller area than this Drawable, the parent will act as a viewport,
     *        cutting off the parts of this Drawable that are outside the region. Traverses the
     *        tree and yields a result expressed in absolute coordinates.
     *
     * @param [out] rect The subregion of the drawable on which to perform the operation.
     */
    virtual void getVisibleRect(Rect& rect) const;

    /**
     * @fn const Rect& Drawable::getRect() const
     *
     * @brief Gets the rectangle this Drawable covers.
     *
     *        Gets the rectangle this Drawable covers, in coordinates relative to its parent.
     *
     * @return The rectangle this Drawable covers expressed in coordinates relative to its
     *         parent.
     */
    const Rect& getRect() const
    {
        return rect;
    }

    /**
     * @fn Rect Drawable::getAbsoluteRect() const;
     *
     * @brief Helper function for obtaining the rectangle this Drawable covers.
     *
     *        Helper function for obtaining the rectangle this Drawable covers, expressed in
     *        absolute coordinates.
     *
     * @return The rectangle this Drawable covers expressed in absolute coordinates.
     */
    Rect getAbsoluteRect() const;

    /**
     * @fn virtual void Drawable::translateRectToAbsolute(Rect& r) const;
     *
     * @brief Helper function for converting a specified subregion of this Drawable to absolute
     *        coordinates.
     *
     *        Helper function for converting a specified subregion of this Drawable to absolute
     *        coordinates.
     *
     * @param [in,out] r The Rect to translate.
     */
    virtual void translateRectToAbsolute(Rect& r) const;

    /**
     * @fn virtual void Drawable::setPosition(int16_t x, int16_t y, int16_t width, int16_t height)
     *
     * @brief Sets the size and position of this Drawable.
     *
     *        Sets the size and position of this Drawable, relative to its parent.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param x      The x coordinate of this Drawable.
     * @param y      The y coordinate of this Drawable.
     * @param width  The width of this Drawable.
     * @param height The height of this Drawable.
     */
    virtual void setPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        setXY(x, y);
        setWidth(width);
        setHeight(height);
    };

    /**
     * @fn int16_t Drawable::getX() const
     *
     * @brief Gets the x coordinate of this drawable.
     *
     *        Gets the x coordinate of this drawable.
     *
     * @return The x value, relative to the parent.
     */
    int16_t getX() const
    {
        return rect.x;
    }

    /**
     * @fn int16_t Drawable::getY() const
     *
     * @brief Gets the y coordinate of this drawable.
     *
     *        Gets the y coordinate of this drawable.
     *
     * @return The y value, relative to the parent.
     */
    int16_t getY() const
    {
        return rect.y;
    }

    /**
     * @fn int16_t Drawable::getWidth() const
     *
     * @brief Gets the width of this drawable.
     *
     *        Gets the width of this drawable.
     *
     * @return The width of this drawable.
     */
    int16_t getWidth() const
    {
        return rect.width;
    }

    /**
     * @fn int16_t Drawable::getHeight() const
     *
     * @brief Gets the height of this drawable.
     *
     *        Gets the height of this drawable.
     *
     * @return The height of this drawable.
     */
    int16_t getHeight() const
    {
        return rect.height;
    }

    /**
     * @fn virtual void Drawable::setX(int16_t x)
     *
     * @brief Sets the x coordinate of this drawable.
     *
     *        Sets the x coordinate of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param x The new x value, relative to the parent. A negative value is allowed.
     */
    virtual void setX(int16_t x)
    {
        rect.x = x;
    }

    /**
     * @fn virtual void Drawable::setY(int16_t y)
     *
     * @brief Sets the y coordinate of this drawable.
     *
     *        Sets the y coordinate of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param y The new y value, relative to the parent. A negative value is allowed.
     */
    virtual void setY(int16_t y)
    {
        rect.y = y;
    }

    /**
     * @fn virtual void Drawable::setXY(int16_t x, int16_t y)
     *
     * @brief Sets the x and y coordinates of this drawable.
     *
     *        Sets the x and y coordinates of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param x The new x value, relative to the parent. A negative value is allowed.
     * @param y The new y value, relative to the parent. A negative value is allowed.
     */
    virtual void setXY(int16_t x, int16_t y)
    {
        setX(x);
        setY(y);
    }

    /**
     * @fn virtual void Drawable::setWidth(int16_t width)
     *
     * @brief Sets the width of this drawable.
     *
     *        Sets the width of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param width The new width.
     */
    virtual void setWidth(int16_t width)
    {
        rect.width = width;
    }

    /**
     * @fn virtual void Drawable::setHeight(int16_t height)
     *
     * @brief Sets the height of this drawable.
     *
     *        Sets the height of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param height The new height.
     */
    virtual void setHeight(int16_t height)
    {
        rect.height = height;
    }

    /**
     * @fn virtual void Drawable::childGeometryChanged()
     *
     * @brief This function can be called on parent nodes to signal that the size of one or more of
     *        its children has changed.
     *
     *        This function can be called on parent nodes to signal that the size of one or
     *        more of its children has changed. Currently only used in ScrollableContainer to
     *        redraw scrollbars when the size of the scrolling contents changes.
     */
    virtual void childGeometryChanged() { }

    /**
     * @fn virtual void Drawable::handleClickEvent(const ClickEvent& evt)
     *
     * @brief Defines the event handler interface for ClickEvents.
     *
     *        Defines the event handler interface for ClickEvents. The default implementation
     *        ignores the event. The event is only received if the drawable is touchable.
     *
     * @param evt The ClickEvent received from the HAL.
     */
    virtual void handleClickEvent(const ClickEvent& evt) { }

    /**
     * @fn virtual void Drawable::handleGestureEvent(const GestureEvent& evt)
     *
     * @brief Defines the event handler interface for GestureEvents.
     *
     *        Defines the event handler interface for GestureEvents. The default implementation
     *        ignores the event. The event is only received if the drawable is touchable.
     *
     * @param evt The GestureEvent received from the HAL.
     */
    virtual void handleGestureEvent(const GestureEvent& evt) { }

    /**
     * @fn virtual void Drawable::handleDragEvent(const DragEvent& evt)
     *
     * @brief Defines the event handler interface for DragEvents.
     *
     *        Defines the event handler interface for DragEvents. The event is only received if
     *        the drawable is touchable.
     *
     * @param evt The DragEvent received from the HAL.
     */
    virtual void handleDragEvent(const DragEvent& evt) { }

    /**
     * @fn virtual void Drawable::handleTickEvent()
     *
     * @brief Called periodically by the framework if the Drawable instance has subscribed to timer
     *        ticks.
     *
     *        Called periodically by the framework if the Drawable instance has subscribed to
     *        timer ticks.
     *
     * @see Application::registerTimerWidget
     */
    virtual void handleTickEvent() { }

    /**
     * @fn void Drawable::setVisible(bool vis)
     *
     * @brief Controls whether this Drawable should be visible.
     *
     *        Controls whether this Drawable should be visible. Only visible Drawables will
     *        have their draw function called. Additionally, invisible drawables will not
     *        receive input events.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param vis true if this Drawable should be visible. By default, drawables are visible unless
     *            this function has been called with false as argument.
     */
    void setVisible(bool vis)
    {
        visible = vis;
    }

    /**
     * @fn void Drawable::setTouchable(bool touch)
     *
     * @brief Controls whether this Drawable receives touch events or not.
     *
     *        Controls whether this Drawable receives touch events or not.
     *
     * @param touch If true  it will receive touch events, if false it will not.
     */
    void setTouchable(bool touch)
    {
        touchable = touch;
    }

    /**
     * @fn bool Drawable::isVisible() const
     *
     * @brief Gets whether this Drawable is visible.
     *
     *        Gets whether this Drawable is visible.
     *
     * @return true if the Drawable is visible.
     * @return True if visible.
     *
     * @see setVisible
     */
    bool isVisible() const
    {
        return visible;
    }

    /**
     * @fn bool Drawable::isTouchable() const
     *
     * @brief Gets whether this Drawable receives touch events or not.
     *
     *        Gets whether this Drawable receives touch events or not.
     *
     * @return True if touch events are received.
     *
     * @see setTouchable
     */
    bool isTouchable() const
    {
        return touchable;
    }

    /**
     * @fn Drawable* Drawable::getParent()
     *
     * @brief Returns the parent node.
     *
     *        Returns the parent node. For the root container, the return value is 0.
     *
     * @return The parent node. For the root container, the return value is 0.
     */
    Drawable* getParent()
    {
        return parent;
    }

    friend class Container;
    friend class Screen;

    /**
     * @fn virtual void Drawable::moveRelative(int16_t x, int16_t y);
     *
     * @brief Moves the drawable.
     *
     *        Moves the drawable.
     *
     * @note Will redraw the appropriate areas of the screen.
     *
     * @param x The relative position to move to.
     * @param y The relative position to move to.
     */
    virtual void moveRelative(int16_t x, int16_t y);

    /**
     * @fn virtual void Drawable::moveTo(int16_t x, int16_t y)
     *
     * @brief Moves the drawable.
     *
     *        Moves the drawable.
     *
     * @note Will redraw the appropriate areas of the screen.
     *
     * @param x The absolute position to move to.
     * @param y The absolute position to move to.
     */
    virtual void moveTo(int16_t x, int16_t y)
    {
        moveRelative(x - rect.x, y - rect.y);
    }

    /**
     * @typedef enum DrawableType
     *
     * @brief Enum defining Drawable types.
     *
     *        Enum defining Drawable types. To be used by automated GUI testing to determine
     *        class type of a Drawable object.
     */
    typedef enum
    {
        TYPE_DRAWABLE,
        TYPE_WIDGET,
        TYPE_ABSTRACTBUTTON,
        TYPE_ANIMATEDIMAGE,
        TYPE_BOX,
        TYPE_BUTTON,
        TYPE_BUTTONWITHICON,
        TYPE_BUTTONWITHLABEL,
        TYPE_IMAGE,
        TYPE_TILEDIMAGE,
        TYPE_KEYBOARD,
        TYPE_SCALABLEIMAGE,
        TYPE_SNAPSHOTWIDGET,
        TYPE_TEXTAREA,
        TYPE_TEXTAREAWITHONEWILDCARD,
        TYPE_TEXTAREAWITHTWOWILDCARDS,
        TYPE_TOGGLEBUTTON,
        TYPE_TOUCHAREA,
        TYPE_CONTAINER,
        TYPE_LISTLAYOUT,
        TYPE_SCROLLABLECONTAINER,
        TYPE_ZOOMANIMATIONIMAGE,
        TYPE_RADIOBUTTON,
        TYPE_TEXTUREMAPPER,
        TYPE_SLIDER,
        TYPE_CUSTOMTYPESBEGIN,
        TYPE_CLICKABLECONTAINER
    } DrawableType;

    /**
     * @fn virtual uint16_t Drawable::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Call this virtual function to determine the class type of
     *        this Drawable object. Can be used in automated GUI testing. Otherwise this
     *        function is unused.
     *
     * @note If creating custom drawables that need to be manipulated by a test framework,
     *       override this function and return a custom value higher than or equal to
     *       TYPE_CUSTOMTYPESBEGIN.
     *
     * @return An integer describing the class type of this object, corresponding to the
     *         DrawableType enum for built-in Drawables.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_DRAWABLE;
    }

    /**
     * @fn void Drawable::drawToDynamicBitmap(BitmapId id)
     *
     * @brief Render the Drawable object into a dynamic bitmap.
     *
     *        Render the Drawable object into a dynamic bitmap.
     *
     * @param id The target dynamic bitmap to use for rendering.
     */
    void drawToDynamicBitmap(BitmapId id);

protected:
    Rect      rect;                 ///< The coordinates of this drawable, relative to its parent.
    Rect cachedVisibleRect;         ///< Cached representation of currently visible area. For TouchGFX internal use.
    Drawable* parent;               ///< Pointer to this drawable's parent.
    Drawable* nextSibling;          ///< Pointer to the next drawable. Maintained by containers.
    Drawable* nextDrawChainElement; ///< Next in draw chain. For TouchGFX internal use.
    int16_t cachedAbsX;             ///< Cached value of absolute X-coord. For TouchGFX internal use.
    int16_t cachedAbsY;             ///< Cached value of absolute Y-coord. For TouchGFX internal use.
    bool      touchable;            ///< True if this drawable should receive touch events.
    bool      visible;              ///< True if this drawable should be drawn.

    static const int16_t UNCACHED_INDICATOR = -1; ///< Constant representing uncached value. For TouchGFX internal use.

    /**
     * @fn void Drawable::resetDrawChainCache()
     *
     * @brief For TouchGFX internal use only.
     *
     *        Reset cached coordinate data.
     *
     * @note For TouchGFX internal use only.
     */
    void resetDrawChainCache()
    {
        // Resetting the cached indicators
        cachedVisibleRect.x = UNCACHED_INDICATOR;
        cachedAbsX = UNCACHED_INDICATOR;
        cachedAbsY = UNCACHED_INDICATOR;
    }

    /**
     * @fn Rect& Drawable::getCachedVisibleRect()
     *
     * @brief For TouchGFX internal use only.
     *
     *        Obtain cached version of visible rect.
     *
     * @note For TouchGFX internal use only.
     *
     * @return The Visible rect for this drawable. Only calculated once.
     */
    Rect& getCachedVisibleRect()
    {
        if (cachedVisibleRect.x == UNCACHED_INDICATOR)
        {
            Rect visibleRect(0, 0, getWidth(), getHeight());
            getVisibleRect(visibleRect);
            cachedVisibleRect = visibleRect;
        }
        return cachedVisibleRect;
    }

    /**
     * @fn int16_t Drawable::getCachedAbsX()
     *
     * @brief For TouchGFX internal use only.
     *
     *        Obtain cached version of absolute X-coord.
     *
     * @note For TouchGFX internal use only.
     *
     * @return The absolute x coordinate for this drawable. Only calculated once.
     */
    int16_t getCachedAbsX()
    {
        if (cachedAbsX == UNCACHED_INDICATOR)
        {
            Rect absRect = getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsX;
    }

    /**
     * @fn int16_t Drawable::getCachedAbsY()
     *
     * @brief For TouchGFX internal use only.
     *
     *        Obtain cached version of absolute Y-coord.
     *
     * @note For TouchGFX internal use only.
     *
     * @return The absolute y coordinate for this drawable. Only calculated once.
     */
    int16_t getCachedAbsY()
    {
        if (cachedAbsY == UNCACHED_INDICATOR)
        {
            Rect absRect = getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsY;
    }

    /**
     * @fn virtual void Drawable::setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
     *
     * @brief For TouchGFX internal use only.
     *
     *        Configure linked list for draw chain.
     *
     * @note For TouchGFX internal use only.
     *
     * @param invalidatedArea              Include drawables that intersect with this area only.
     * @param [in,out] nextPreviousElement Modifiable element in linked list.
     */
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
    {
        resetDrawChainCache();
        nextDrawChainElement = *nextPreviousElement;
        *nextPreviousElement = this;
    }
};
} // namespace touchgfx
#endif // DRAWABLE_HPP
