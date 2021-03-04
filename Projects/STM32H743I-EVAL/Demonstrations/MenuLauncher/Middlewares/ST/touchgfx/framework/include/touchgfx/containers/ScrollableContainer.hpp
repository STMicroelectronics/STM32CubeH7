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

#ifndef SCROLLABLECONTAINER_HPP
#define SCROLLABLECONTAINER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Application.hpp>

namespace touchgfx
{
/**
 * @class ScrollableContainer ScrollableContainer.hpp touchgfx/containers/ScrollableContainer.hpp
 *
 * @brief A ScrollableContainer is a container that allows its contents to be scrolled.
 *
 *        A ScrollableContainer is a container that allows its contents to be scrolled. It will
 *        intercept drag operations and move child nodes accordingly.
 *
 *        The size of the ScrollableContainer should be the visible view port area. If the
 *        container contains drawables that are larger than the ScrollableContainer itself,
 *        scrolling is enabled.
 *
 * @note The ScrollableContainer will consume all DragEvents in the area covered by the container,
 *       and use.
 *
 * @see Container
 */
class ScrollableContainer : public Container
{
public:

    /**
     * @fn ScrollableContainer::ScrollableContainer();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ScrollableContainer();

    /**
     * @fn virtual ScrollableContainer::~ScrollableContainer()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ScrollableContainer() { }

    /**
     * @fn void ScrollableContainer::enableHorizontalScroll(bool enable)
     *
     * @brief Enables horizontal scrolling.
     *
     *        By default, scrolling in either direction is enabled, provided that the content
     *        is larger than the size of the scrollable container. This function can be used to
     *        explicitly (dis)allow scrolling in the horizontal direction, even if the content
     *        is larger than the container.
     *
     * @param enable If true (default), horizontal scrolling is enabled. If false, scrolling is
     *               disabled.
     */
    void enableHorizontalScroll(bool enable)
    {
        scrollableX = enable;
    }

    /**
     * @fn void ScrollableContainer::enableVerticalScroll(bool enable)
     *
     * @brief Enables the vertical scroll.
     *
     *        Enables the vertical scroll. By default, scrolling in either direction is enabled,
     *        provided that the content is larger than the size of the scrollable container.
     *        This function can be used to explicitly (dis)allow scrolling in the vertical
     *        direction, even if the content is larger than the container.
     *
     * @param enable If true (default), vertical scrolling is enabled. If false, scrolling is
     *               disabled.
     */
    void enableVerticalScroll(bool enable)
    {
        scrollableY = enable;
    }

    /**
     * @fn virtual void ScrollableContainer::isScrollableXY(bool& scrollX, bool& scrollY)
     *
     * @brief Is the ClickableContainer scrollable in either direction?
     *
     *        Is the ClickableContainer scrollable in either direction? Takes the width of the
     *        contained elements into account.
     *
     * @param [in,out] scrollX Is the container able to scroll horizontally.
     * @param [in,out] scrollY Is the container able to scroll vertically.
     */
    virtual void isScrollableXY(bool& scrollX, bool& scrollY)
    {
        Rect contained = getContainedArea();
        scrollX = (scrollableX && (rect.width < contained.width));
        scrollY = (scrollableY && (rect.height < contained.height));
    }

    /**
     * @fn void ScrollableContainer::setScrollbarsVisible(bool newVisible);
     *
     * @brief Sets the visibility of the scrollbars, when the scrollable area is pressed.
     *
     *        Sets the visibility of the scrollbars, when the scrollable area is pressed.
     *
     * @param newVisible If true (default), the scrollbars are visible when scrollable area is pressed.
     *                   If false, scrollbars are always hidden.
     */
    void setScrollbarsVisible(bool newVisible);

    /**
     * @fn void ScrollableContainer::setScrollbarsPermanentlyVisible()
     *
     * @brief sets the visibility for the scrollbars to be permanent.
     *
     *        sets the visibility for the scrollbars to be permanent.
     *
     */
    void setScrollbarsPermanentlyVisible();

    /**
     * @fn virtual void ScrollableContainer::add(Drawable& d);
     *
     * @brief Adds a Drawable instance as child to this ScrollableContainer.
     *
     *        Adds a Drawable instance as child to this ScrollableContainer.
     *
     * @param [in] d The drawable.
     */
    virtual void add(Drawable& d);

    /**
     * @fn virtual void ScrollableContainer::getLastChild(int16_t x, int16_t y, Drawable** last)
     *
     * @brief Gets the last child in the container.
     *
     *        Gets the last child in the container. The ScrollableContainer needs to intercept
     *        click events, since the scrollbars are displayed upon reception of a PRESSED
     *        ClickEvent. The ScrollableContainer will automatically re-delegate the event to
     *        the appropriate child.
     *
     * @param x          The x coordinate of the (click) event.
     * @param y          The y coordinate of the (click) event.
     * @param [out] last The last child intersecting x,y. ScrollableContainer intercepts these, so
     *                   returns it self.
     */
    virtual void getLastChild(int16_t x, int16_t y, Drawable** last)
    {
        if (isVisible())
        {
            if (isTouchable())
            {
                *last = this;
            }
            else
            {
                Container::getLastChild(x, y, last);
            }
        }
    }

    /**
     * @fn virtual void ScrollableContainer::handleClickEvent(const ClickEvent& evt);
     *
     * @brief Handle the click event.
     *
     *        Handle the click event. Get ready for scrolling, display scrollbars, etc. Send
     *        the click to appropriate child widget.
     *
     * @param evt The ClickEvent.
     */
    virtual void handleClickEvent(const ClickEvent& evt);

    /**
     * @fn virtual void ScrollableContainer::handleDragEvent(const DragEvent& evt);
     *
     * @brief Handle the drag event.
     *
     *        Handle the drag event. Initiate a scrolling of the container. Update scrollbars.
     *
     * @param evt The DragEvent.
     */
    virtual void handleDragEvent(const DragEvent& evt);

    /**
     * @fn virtual void ScrollableContainer::handleGestureEvent(const GestureEvent& evt);
     *
     * @brief Gestures generate a scroll animation so these are intercepted in the same manner as
     *        drag events.
     *
     *        Gestures generate a scroll animation so these are intercepted in the same manner
     *        as drag events.
     *
     * @param evt The GestureEvent.
     */
    virtual void handleGestureEvent(const GestureEvent& evt);

    /**
     * @fn virtual void ScrollableContainer::handleTickEvent();
     *
     * @brief Handle tick events.
     *
     *        Handle tick events. Used in updating the animation of the scroll.
     */
    virtual void handleTickEvent();

    /**
     * @fn virtual Rect ScrollableContainer::getContainedArea() const;
     *
     * @brief Gets contained area.
     *
     *        Gets contained area.
     *
     * @return The contained area.
     */
    virtual Rect getContainedArea() const;

    /**
     * @fn virtual void ScrollableContainer::childGeometryChanged();
     *
     * @brief Used to signal that the size of one or more children have changed.
     *
     *        This function can be called on parent nodes to signal that the size of one or
     *        more of its children have changed. Currently only used in ScrollableContainer to
     *        redraw scrollbars when the size of the scrolling contents changes.
     *
     * @see Drawable::childGeometryChanged
     */
    virtual void childGeometryChanged();

    /**
     * @fn void ScrollableContainer::reset();
     *
     * @brief Resets the x/y coordinates of children.
     *
     *        Resets the x/y coordinates of childrento the position they were in before the
     *        first drag event was received or to the position they were in the last time
     *        reset() was invoked.
     */
    void reset();

    /**
     * @fn virtual void ScrollableContainer::moveChildrenRelative(int16_t deltaX, int16_t deltaY);
     *
     * @brief Moves the scrollable contents relatively.
     *
     *        Moves the scrollable contents relatively.
     *
     * @param deltaX Horizontal displacement.
     * @param deltaY Vertical displacement.
     */
    virtual void moveChildrenRelative(int16_t deltaX, int16_t deltaY);

    /**
     * @fn void ScrollableContainer::setMaxVelocity(uint16_t max)
     *
     * @brief Sets the maximum velocity of a scroll due to a swipe.
     *
     *        Sets the maximum velocity of a scroll due to a swipe.
     *
     * @param max The maximum velocity of the scroll.
     */
    void setMaxVelocity(uint16_t max)
    {
        maxVelocity = max;
    };

    /**
     * @fn void ScrollableContainer::setScrollThreshold(int16_t t)
     *
     * @brief Change the threshold which the first drag event received must exceed before
     *        initiating a scroll.
     *
     *        Change the threshold which the first drag event received must exceed before
     *        initiating a scroll.
     *
     * @note All subsequent scrolls will be processed regardless of threshold value until a
     *       ClickEvent::RELEASED is received.
     *
     * @param t The new threshold value.
     */
    void setScrollThreshold(int16_t t)
    {
        scrollThreshold = t;
    }

    /**
     * @fn void ScrollableContainer::setScrollbarsColor(colortype color);
     *
     * @brief Sets the color of the scroll bars.
     *
     *        Sets the color of the scroll bars.
     *
     * @param color The color of the box.
     */
    void setScrollbarsColor(colortype color);

    /**
     * @fn void ScrollableContainer::setScrollbarsAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha value for the scroll bars.
     *
     *        Sets the alpha value for the scroll bars.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setScrollbarsAlpha(uint8_t alpha);

    /**
     * @fn void ScrollableContainer::setScrollbarPadding(uint8_t padding);
     *
     * @brief Sets the amount of space the scrollbar has to its borders.
     *
     *        Sets the amount of space the scrollbar has to its borders.
     *
     * @param padding The padding.
     */
    void setScrollbarPadding(uint8_t padding);

    /**
     * @fn void ScrollableContainer::setScrollbarWidth(uint8_t width);
     *
     * @brief Sets the width of the scrollbar.
     *
     *        Sets the width of the scrollbar.
     *
     * @param width The width of the scrollbar.
     */
    void setScrollbarWidth(uint8_t width);

    /**
     * @fn int16_t ScrollableContainer::getScrolledX() const;
     *
     * @brief Gets the distance scrolled for the x-axis.
     *
     *        Gets the distance scrolled for the x-axis.
     *
     * @return the distance scrolled for the x-axis.
     */
    int16_t getScrolledX() const;

    /**
     * @fn int16_t ScrollableContainer::getScrolledY() const;
     *
     * @brief Gets the distance scrolled for the y-axis.
     *
     *        Gets the distance scrolled for the y-axis.
     *
     * @return the distance scrolled for the y-axis.
     */
    int16_t getScrolledY() const;

    /**
    * @fn virtual uint16_t ScrollableContainer::getType() const
    *
    * @brief For GUI testing only.
    *
    *        For GUI testing only. Returns type of this drawable.
    *
    * @return TYPE_SCROLLABLECONTAINER.
    */

    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_SCROLLABLECONTAINER;
    }

protected:
    uint8_t   scrollbarPadding;                           ///< The amount of padding. The scrollbar will have a bit of space to the borders of the container.
    uint8_t   scrollbarWidth;                             ///< The width of the scrollbar.
    uint8_t   scrollbarAlpha;                             ///< The scrollbar is semitransparent
    static    const uint8_t SCROLLBAR_LINE = 0;           ///< The scrollbar line.
    colortype scrollbarColor;                             ///< The color of the scrollbar
    static    const uint16_t SCROLLBAR_MIN_VELOCITY = 5;  ///< The minimum velocity of a scroll due to a swipe
    static    const uint16_t SCROLLBAR_MAX_VELOCITY = 17; ///< The (default) maximum velocity of a scroll due to a swipe
    uint16_t  maxVelocity;                                ///< The maximum velocity of a scroll (due to a swipe)

    /**
     * @fn Rect ScrollableContainer::getXScrollbar() const;
     *
     * @brief Gets x coordinate of the scrollbar.
     *
     *        Gets x coordinate of the scrollbar.
     *
     * @return The horizontal scrollbar area.
     */
    Rect getXScrollbar() const;

    /**
     * @fn Rect ScrollableContainer::getYScrollbar() const;
     *
     * @brief Gets y coordinate of the scrollbar.
     *
     *        Gets y coordinate of the scrollbar.
     *
     * @return The vertical scrollbar area.
     */
    Rect getYScrollbar() const;

    /**
     * @fn Rect ScrollableContainer::getXBorder(const Rect& xBar, const Rect& yBar) const;
     *
     * @brief Gets the area where the horizontal scrollbar can move.
     *
     *        Gets the area where the horizontal scrollbar can move.
     *
     * @param xBar The current horizontal scrollbar, supplied for caching reasons.
     * @param yBar The current vertical scrollbar, supplied for caching reasons.
     *
     * @return The area.
     */
    Rect getXBorder(const Rect& xBar, const Rect& yBar) const;

    /**
     * @fn Rect ScrollableContainer::getYBorder(const Rect& xBar, const Rect& yBar) const;
     *
     * @brief Gets the area where the vertical scrollbar can move.
     *
     *        Gets the area where the vertical scrollbar can move.
     *
     * @param xBar The current horizontal scrollbar, supplied for caching reasons.
     * @param yBar The current vertical scrollbar, supplied for caching reasons.
     *
     * @return The area.
     */
    Rect getYBorder(const Rect& xBar, const Rect& yBar) const;

    /**
     * @fn void ScrollableContainer::invalidateScrollbars();
     *
     * @brief Invalidate the scrollbars.
     *
     *        Invalidate the scrollbars.
     */
    void invalidateScrollbars();

    /**
     * @fn virtual bool ScrollableContainer::doScroll(int16_t deltaX, int16_t deltaY);
     *
     * @brief Method to actually scroll the container.
     *
     *        Method to actually scroll the container. Passing negative values will scroll the
     *        items in the ScrollableContainer up / left, whereas positive values will scroll
     *        items down / right.
     *
     *        If the distance is larger than allowed, the deltas are adjusted down to make sure
     *        the contained items stay inside view.
     *
     * @param deltaX The horizontal amount to scroll.
     * @param deltaY The vertical amount to scroll.
     *
     * @return did the container actually scroll. The call doScroll(0,0) will always return
     *         false.
     */
    virtual bool doScroll(int16_t deltaX, int16_t deltaY);

    GestureEvent::GestureType accelDirection; ///< The current direction (horizontal or vertical) of scroll

    Box xSlider; ///< The horizontal scrollbar drawable
    Box ySlider; ///< The vertical scrollbar drawable

    Drawable* pressedDrawable;    ///< The drawable child of this container which received the last ClickEvent::PRESSED notification. When scrolling, send this drawable a CANCEL event if the new x/y coords no longer matches this drawable.
    Drawable* lastDraggableChild; ///< The drawable child of this container which should receive drag events. Note that only drag events in directions which cannot be scrolled by this ScrollableContainer will be forwarded to children.

    int16_t scrolledXDistance; ///< The scrolled horizontal distance
    int16_t scrolledYDistance; ///< The scrolled vertical distance
    int16_t scrollThreshold;   ///< The threshold which the first drag event received must exceed before scrolling. Default is 5.

    int16_t pressedX; ///< The x coordinate where the last ClickEvent::PRESSED was received.
    int16_t pressedY; ///< The y coordinate where the last ClickEvent::PRESSED was received.

    bool isPressed;   ///< Is the container currently pressed (maybe show scrollbars)
    bool isScrolling; ///< Is the container scrolling (i.e. has overcome the initial larger drag that is required to initiate a scroll).

    bool scrollableX; ///< Is the container scrollable in the horizontal direction.
    bool scrollableY; ///< Is the container scrollable in the vertical direction.

    bool scrollbarsVisible; ///< Are scrollbars always visible.
    bool scrollbarsPermanentlyVisible; ///< Are scrollbars always visible.

    uint16_t scrollDuration; ///< Number of ticks the scroll animation should use.

    int16_t beginningValue; ///< Initial X or Y for calculated values in scroll animation.
    int16_t targetValue;    ///< Target X or Y value for scroll animation

    uint16_t animationCounter; ///< Current step/tick in scroll animation.
    bool animate;              ///< Is scroll animation currently active

    int16_t fingerAdjustmentX; ///< How much should the finger be adjusted horizontally
    int16_t fingerAdjustmentY; ///< and how much vertically

    bool hasIssuedCancelEvent; ///< true if the pressed drawable has received cancel event
};
} // namespace touchgfx

#endif // SCROLLABLECONTAINER_HPP
