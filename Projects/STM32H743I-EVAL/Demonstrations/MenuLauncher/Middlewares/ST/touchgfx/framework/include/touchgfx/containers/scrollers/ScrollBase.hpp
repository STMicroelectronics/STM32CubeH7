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

#ifndef SCROLLBASE_HPP
#define SCROLLBASE_HPP

#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/scrollers/DrawableList.hpp>

namespace touchgfx
{
/**
 * @class ScrollBase ScrollBase.hpp touchgfx/containers/scrollers/ScrollBase.hpp
 *
 * @brief A scroll base class.
 *
 *        A scroll base class with a list of drawables (DrawableList).
 *
 * @see ScrollWheelBase
 * @see ScrollList
 * @see DrawableList
 */
class ScrollBase : public Container
{
public:
    /**
     * @fn ScrollBase::ScrollBase();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ScrollBase();

    /**
     * @fn virtual ScrollBase::~ScrollBase()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ScrollBase()
    {
    }

    /**
     * @fn virtual void ScrollBase::setWidth(int16_t width);
     *
     * @brief Sets width of the ScrollBase.
     *
     *        Sets width of the ScrollBase. The width is propagated to the child(ren).
     *
     * @param width The width.
     */
    virtual void setWidth(int16_t width);

    /**
     * @fn virtual void ScrollBase::setHeight(int16_t height);
     *
     * @brief Sets height of the ScrollBase.
     *
     *        Sets height of the ScrollBase.
     *
     * @param height The height. The height is propagated to the child(ren).
     */
    virtual void setHeight(int16_t height);

    /**
     * @fn virtual void ScrollBase::setHorizontal(bool horizontal)
     *
     * @brief Sets a horizontal layout.
     *
     *        Sets a horizontal layout. If horizontal is set true, all drawables are arranged side
     *        by side. If horizontal is set false, the drawables are arranged above and below each
     *        other (vertically).
     *
     * @param horizontal True to align drawables horizontal, false to align drawables vertically.
     *
     * @note Default value is false, i.e. vertical layout.
     *
     * @see DrawableList::setHorizontal
     * @see getHorizontal
     */
    virtual void setHorizontal(bool horizontal);

    /**
     * @fn virtual bool ScrollBase::getHorizontal() const
     *
     * @brief Gets the orientation of the drawables.
     *
     *        Gets the orientation of the drawables, previously set using setHorizontal.
     *
     * @return True if it horizontal, false if it is vertical.
     *
     * @see DrawableList::getHorizontal
     * @see setHorizontal
     */
    virtual bool getHorizontal() const;

    /**
     * @fn virtual void ScrollBase::setCircular(bool circular);
     *
     * @brief Sets whether the list is circular or not.
     *
     *        Sets whether the list is circular (infinite) or not. A circular list is a list where
     *        the first drawable re-appears after the last item in the list - and the last item in
     *        the list appears before the first item in the list.
     *
     * @param circular True if the list should be circular, false if the list should not be circular.
     *
     * @see DrawableList::setCircular
     * @see getCircular
     */
    virtual void setCircular(bool circular);

    /**
     * @fn virtual bool ScrollBase::getCircular() const;
     *
     * @brief Gets the circular setting.
     *
     *        Gets the circular setting, previously set using setCircular().
     *
     * @return True if the list is circular (infinite), false if the list is not circular (finite).
     *
     * @see DrawableList::getCircular
     * @see setCircular
     */
    virtual bool getCircular() const;

    /**
     * @fn void ScrollBase::setDrawableSize(int16_t drawableSize, int16_t drawableMargin);
     *
     * @brief Sets drawables size.
     *
     *        Sets drawables size. The drawable is is the size of each drawable in the list (this
     *        is enforced by the DrawableList class). The spacing is the amount of blank to add
     *        between each drawable. Half of the space is placed before and half of the space is
     *        placed after the drawable. The entire size of an item is thus size + spacing.
     *
     * @param drawableSize   The size of the drawable.
     * @param drawableMargin The margin around drawables (margin before and margin after).
     */
    void setDrawableSize(int16_t drawableSize, int16_t drawableMargin);

    /**
     * @fn virtual int16_t ScrollBase::getDrawableSize() const
     *
     * @brief Gets drawable size.
     *
     *        Gets drawable size as set through the first argument in setDrawables().
     *
     * @return The drawable size.
     *
     * @see setDrawables
     */
    virtual int16_t getDrawableSize() const;

    /**
     * @fn virtual int16_t ScrollBase::getDrawableMargin() const;
     *
     * @brief Gets drawable margin.
     *
     *        Gets drawable margin as set through the argument in setDrawables().
     *
     * @return The drawable margin.
     */
    virtual int16_t getDrawableMargin() const;

    /**
     * @fn virtual void ScrollBase::setNumberOfItems(int16_t items);
     *
     * @brief Sets number of items in the DrawableList.
     *
     *        Sets number of items in the DrawableList. This forces all drawables to be updated to
     *        ensure that the content is correct. For example a minute selector might could have 60
     *        items (only some of which are visible at any given time).
     *
     * @param items Number of items.
     *
     * @note The DrawableList is refreshed to reflect the change.
     */
    virtual void setNumberOfItems(int16_t numberOfItems);

    /**
     * @fn virtual int16_t ScrollBase::getNumberOfItems() const
     *
     * @brief Gets number of items in the DrawableList.
     *
     *        Gets number of items in the DrawableList, as previously set using setNumberOfItems().
     *
     * @return The number of items.
     *
     * @see setNumberOfItems
     * @see DrawableList::getNumberOfItems
     */
    virtual int16_t getNumberOfItems() const;

    /**
     * @fn void ScrollBase::setEasingEquation(EasingEquation equation);
     *
     * @brief Sets easing equation.
     *
     *        Sets easing equation when changing the selected item, for example via swipe or AnimateTo.
     *
     * @param equation The equation.
     *
     * @see setAnimationSteps
     * @see getAnimationSteps
     */
    void setEasingEquation(EasingEquation equation);

    /**
     * @fn void ScrollBase::setAnimationSteps(int16_t steps);
     *
     * @brief Sets animation steps.
     *
     *        Sets animation steps when moving to a new selected item. The default value is 30.
     *
     * @param steps The animation steps.
     *
     * @see setEasingEquation
     * @see getAnimationSteps
     */
    void setAnimationSteps(int16_t steps);

    /**
     * @fn uint16_t ScrollBase::getAnimationSteps() const;
     *
     * @brief Gets animation steps.
     *
     *        Gets animation steps as set in setAnimationSteps.
     *
     * @return The animation steps.
     *
     * @see setAnimationSteps
     * @see setEasingEquation
     */
    uint16_t getAnimationSteps() const;

    /**
     * @fn void ScrollBase::setSwipeAcceleration(uint16_t acceleration);
     *
     * @brief Sets swipe acceleration.
     *
     *        Sets swipe acceleration (times 10).
     *
     * @param acceleration The acceleration times 10, so "60" means "6" and "75" means "7.5".
     *
     * @note The reason for multiplying the acceleration by 10 is to avoid introducing floating point
     *       arithmetic.
     *
     * @see getSwipeAcceleration
     */
    void setSwipeAcceleration(uint16_t acceleration);

    /**
     * @fn uint16_t ScrollBase::getSwipeAcceleration() const;
     *
     * @brief Gets swipe acceleration.
     *
     *        Gets swipe acceleration (times 10).
     *
     * @return The swipe acceleration.
     *
     * @note The reason for multiplying the acceleration by 10 is to avoid introducing floating point
     *       arithmetic.
     *
     * @see setSwipeAcceleration
     */
    uint16_t getSwipeAcceleration() const;

    /**
     * @fn void ScrollBase::setMaxSwipeItems(uint16_t maxItems);
     *
     * @brief Sets maximum swipe items.
     *
     *        Sets maximum swipe items. Often useful when there are five visible items on the
     *        screen and a swipe action should at most swipe the next/previous five items into view
     *        to achieve sort of a paging effect.
     *
     * @param maxItems The maximum items, 0 means "no limit".
     *
     * @see getMaxSwipeItems
     */
    void setMaxSwipeItems(uint16_t maxItems);

    /**
     * @fn uint16_t ScrollBase::getMaxSwipeItems() const;
     *
     * @brief Gets maximum swipe items.
     *
     *        Gets maximum swipe items as set by setMaxSwipeItems.
     *
     * @return The maximum swipe items, 0 means "no limit".
     *
     * @see setMaxSwipeItems
     */
    uint16_t getMaxSwipeItems() const;

    /**
     * @fn void ScrollBase::setDragAcceleration(uint16_t acceleration);
     *
     * @brief Sets drag acceleration.
     *
     *        Sets drag acceleration times 10, so "10" means "1", "15" means "1.5".
     *
     * @param acceleration The drag acceleration. 10 makes the containers follow the finger, higher
     *                     values makes the containers move faster.
     *
     * @note The reason for multiplying the acceleration by 10 is to avoid introducing floating point
     *       arithmetic.
     *
     * @see getDragAcceleration
     */
    void setDragAcceleration(uint16_t acceleration);

    /**
     * @fn uint16_t ScrollBase::getDragAcceleration() const;
     *
     * @brief Gets drag acceleration.
     *
     *        Gets drag acceleration (times 10).
     *
     * @return The drag acceleration.
     *
     * @note The reason for multiplying the acceleration by 10 is to avoid introducing floating point
     *       arithmetic.
     *
     * @see setDragAcceleration
     */
    uint16_t getDragAcceleration() const;

    /**
    * @fn void ScrollBase::allowHorizontalDrag(bool enable)
    *
    * @brief Enables horizontal scrolling.
    *
    *        Enables horizontal scrolling to be passed to the children. By default, scrolling
    *        in either direction is enabled. This function can be used to explicitly
    *        (dis)allow scrolling in the horizontal direction.
    *
    * @param enable If true (default), horizontal scrolling is enabled. If false, scrolling is
    *               disabled.
    */
    void allowHorizontalDrag(bool enable);

    /**
    * @fn void ScrollBase::allowVerticalDrag(bool enable)
    *
    * @brief Enables the vertical scroll.
    *
    *        Enables the vertical scroll to be passed to the children. By default, scrolling
    *        in either direction is enabled. This function can be used to explicitly
    *        (dis)allow scrolling in the vertical direction.
    *
    * @param enable If true (default), vertical scrolling is enabled. If false, scrolling is
    *               disabled.
    */
    void allowVerticalDrag(bool enable);

    /**
     * @fn virtual void ScrollBase::animateToItem(int16_t itemIndex, int16_t animationSteps = -1);
     *
     * @brief Go to item.
     *
     *        Go to item, possibly with animation. The given item index is scrolled into view. If
     *        animationSteps is omitted, the default number of animation steps is used. If
     *        animationSteps is 0 no animation will be used, otherwise the number of animation
     *        steps specified is used.
     *
     * @param itemIndex      Zero-based index of the item.
     * @param animationSteps (Optional) The steps to use for the animation. 0 means no animation. If
     *                       omitted, default animation steps are used.
     *
     * @see setAnimationSteps
     */
    virtual void animateToItem(int16_t itemIndex, int16_t animationSteps = -1);

    /**
     * @fn void ScrollBase::setItemSelectedCallback(GenericCallback<int16_t>& callback);
     *
     * @brief Sets Callback which will be called when the selected item is clicked.
     *
     *        Sets Callback which will be called when the selected item is clicked.
     *
     * @param [in] callback The callback.
     */
    void setItemSelectedCallback(GenericCallback<int16_t>& callback);

    /**
     * @fn void ScrollBase::setAnimationEndedCallback(GenericCallback<>& callback);
     *
     * @brief Callback, called when the set animation ended.
     *
     *        Callback, called when the set animation ended.
     *
     * @param [in,out] callback The ended callback.
     */
    void setAnimationEndedCallback(GenericCallback<>& callback);

    /**
     * @fn void ScrollBase::setItemPressedCallback(GenericCallback<int16_t>& callback);
     *
     * @brief Set Callback which will be called when a item is pressed.
     *
     *        Set Callback which will be called when a item is pressed.
     *
     * @param [in] callback The callback.
     */
    void setItemPressedCallback(GenericCallback<int16_t>& callback);

    /**
     * @fn bool ScrollBase::isAnimating() const;
     *
     * @brief Query if this object is animating.
     *
     *        Query if this object is animating. This can be good to know if getSelectedItem()
     *        is called, as the result might not be as expected if isAnimating() returns true.
     *
     * @return true if animating, false if not.
     */
    bool isAnimating() const;

    /**
     * @fn void ScrollBase::stopAnimation();
     *
     * @brief Stops an animation.
     *
     *        Stops an animation if one is ongoing.
     */
    void stopAnimation();

    /**
     * @fn virtual void ScrollBase::handleDragEvent(const DragEvent& evt)
     *
     * @brief Handles the drag event described by evt.
     *
     *        Handles the drag event described by evt.
     *
     * @param evt The event.
     */
    virtual void handleDragEvent(const DragEvent& evt);

    /**
     * @fn virtual void ScrollBase::handleGestureEvent(const GestureEvent& evt);
     *
     * @brief Handles the gesture event described by evt.
     *
     *        Handles the gesture event described by evt.
     *
     * @param evt The event.
     */
    virtual void handleGestureEvent(const GestureEvent& evt);

    /**
     * @fn virtual void ScrollBase::handleTickEvent();
     *
     * @brief Handles the tick event.
     *
     *        Handles the tick event.
     */
    virtual void handleTickEvent();

    /**
     * @fn virtual void ScrollBase::itemChanged(int itemIndex)
     *
     * @brief Item changed.
     *
     *        Inform that an item has change and force all drawables with the given item index to
     *        be updated via the callback provided.
     *
     * @param itemIndex Zero-based index of the changed item.
     */
    virtual void itemChanged(int itemIndex);

    /**
     * @fn virtual void ScrollBase::initialize()
     *
     * @brief Initializes the contents of all drawables.
     *
     *        Initializes the contents of all drawables.
     */
    virtual void initialize()
    {
        list.refreshDrawables();
    }

protected:
    /**
     * @fn void ScrollBase::setOffset(int32_t offset)
     *
     * @brief Sets display offset of first item.
     *
     *        Sets display offset of first item.
     *
     * @param offset The offset.
     */
    virtual void setOffset(int32_t offset);

    /**
     * @fn int32_t ScrollBase::getOffset() const
     *
     * @brief Gets display offset of first item.
     *
     *        Gets display offset of first item.
     *
     * @return The offset.
     */
    virtual int32_t getOffset() const;

    /**
     * @fn virtual int32_t ScrollBase::getPositionForItem(int16_t itemIndex) = 0;
     *
     * @brief Gets position for an item.
     *
     *        Get the position for an item. The position should ensure that the item is in view as
     *        defined by the semantics of the actual scroll class.
     *
     * @param itemIndex Zero-based index of the item.
     *
     * @return The position for item.
     */
    virtual int32_t getPositionForItem(int16_t itemIndex) = 0;

    /**
     * @fn int ScrollBase::getNormalizedOffset(int offset) const;
     *
     * @brief Gets normalized offset from a given offset.
     *
     *        Gets normalized offset from a given offset from 0 down to -numItems*itemSize.
     *
     * @param offset The offset.
     *
     * @return The normalized offset.
     */
    int getNormalizedOffset(int offset) const;

    /**
     * @fn virtual int32_t ScrollBase::keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const = 0;
     *
     * @brief Keep offset inside limits.
     *
     * @param newOffset The new offset.
     * @param overShoot The over shoot.
     *
     * @return An int32_t.
     */
    virtual int32_t keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const = 0;

    /**
     * @fn virtual int32_t ScrollBase::getNearestAlignedOffset(int32_t offset) const;
     *
     * @brief Gets nearest offset aligned to a multiple of itemSize.
     *
     *        Gets nearest offset aligned to a multiple of itemSize.
     *
     * @param offset The offset.
     *
     * @return The nearest aligned offset.
     */
    virtual int32_t getNearestAlignedOffset(int32_t offset) const;

    /**
     * @fn void ScrollBase::animateToPosition(int32_t position, int16_t steps);
     *
     * @brief Animate to a new position/offset using the given number of steps.
     *
     *        Animate to a new position/offset using the given number of steps.
     *
     * @param position The new position.
     * @param steps    The steps.
     */
    virtual void animateToPosition(int32_t position, int16_t steps = -1);

    /**
     * @enum AnimationState
     *
     * @brief Values that represent animation states.
     */
    enum AnimationState
    {
        NO_ANIMATION,      ///< No animation
        ANIMATING_GESTURE, ///< Animating a gesture
        ANIMATING_DRAG     ///< Animating a click+drag
    };

    DrawableList list;                 ///< The list
    int16_t numberOfDrawables;         ///< Number of drawables
    int16_t distanceBeforeAlignedItem; ///< The distance before aligned item

    int16_t itemSize;               ///< Size of the item
    uint16_t swipeAcceleration;     ///< The swipe acceleration
    uint16_t dragAcceleration;      ///< The drag acceleration
    uint16_t maxSwipeItems;         ///< The maximum swipe items
    EasingEquation easingEquation;  ///< The easing equation
    uint16_t defaultAnimationSteps; ///< The animation steps

    GenericCallback<int16_t>* itemSelectedCallback; ///< The item selected callback
    GenericCallback<>* itemLockedInCallback;        ///< The item locked in callback
    GenericCallback<>* animationEndedCallback;      ///< The animation ended callback
    GenericCallback<int16_t>* itemPressedCallback;   ///< The item pressed callback

    AnimationState currentAnimationState; ///< The current animation state
    int gestureStep;                      ///< The gesture step
    int gestureStepsTotal;                ///< The gesture steps total
    int gestureStart;                     ///< The gesture start
    int gestureEnd;                       ///< The gesture end

    int16_t xClick;             ///< The click
    int16_t yClick;             ///< The click
    int32_t initialSwipeOffset; ///< The initial swipe offset

    bool draggableX; ///< Is the container draggable in the horizontal direction.
    bool draggableY; ///< Is the container draggable in the vertical direction.
};
} // namespace touchgfx

#endif // SCROLLBASE_HPP
