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

#ifndef SCROLLLIST_HPP
#define SCROLLLIST_HPP

//#include <touchgfx/Callback.hpp>
//#include <touchgfx/Drawable.hpp>
#include <touchgfx/containers/scrollers/ScrollBase.hpp>

namespace touchgfx
{
/**
 * @class ScrollList ScrollList.hpp touchgfx/containers/scrollers/ScrollList.hpp
 *
 * @brief A scrolling menu of drawables.
 *
 *        A scrolling menu of drawables. To preserve resources, a lot of items can be
 *        displayed using only a few drawables. To achieve this, please see DrawableList.
 */
class ScrollList : public ScrollBase
{
public:
    /**
     * @fn ScrollList::ScrollList();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ScrollList();

    /**
     * @fn virtual ScrollList::~ScrollList()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ScrollList()
    {
    }

    /**
     * @fn virtual void ScrollList::setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback);
     *
     * @brief Sets the drawables parameters.
     *
     *        Sets the drawables parameters. These parameters are \li The access class to the array
     *        of drawables \li Callback to update the contents of a drawable.
     *
     * @param [in,out] drawableListItems      Number of drawables allocated.
     * @param [in]     updateDrawableCallback A callback to update the contents of a drawable.
     *
     * @see DrawableList::setDrawables
     */
    virtual void setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback);

    /**
     * @fn void ScrollList::setWindowSize(int16_t items);
     *
     * @brief Sets window size.
     *
     *        Sets window size. This is the number of items that should always be visible. The
     *        default value is 1.
     *
     * @param items The items.
     *
     * @note This only applies to non-circular lists.
     */
    void setWindowSize(int16_t items);

    /**
     * @fn void ScrollList::setPadding(int16_t distanceBefore, int16_t distanceAfter);
     *
     * @brief Sets distance offset before and after drawables in the ScrollList.
     *
     *        Sets distance offset before and after the "visible" drawables in the ScrollList. This
     *        allows the actual area where widgets are placed to have a little extra area where
     *        parts of drawables can be seen. For example if the ScrollList is 200, each drawable
     *        is 50 and distance before and distance after are 25, then there is room for three
     *        visible drawables inside the ScrollList. When scrolling, part of the scrolled out
     *        drawables can be seen before and after the three drawables. Actually 25/50 = 50% of a
     *        drawable can be seen before and after the three drawables in the ScrollList.
     *
     * @param distanceBefore The distance before the first drawable in the ScrollList.
     * @param distanceAfter  The distance after the last drawable in the ScrollList.
     *
     * @see getPaddingBefore
     * @see getPaddingAfter
     */
    void setPadding(int16_t paddingBefore, int16_t paddingAfter);

    /**
     * @fn int16_t ScrollList::getPaddingBefore() const;
     *
     * @brief Gets distance before first drawable in ScrollList.
     *
     *        Gets distance before first drawable in ScrollList.
     *
     * @return The distance before.
     *
     * @see setPadding
     * @see getPaddingAfter
     */
    int16_t getPaddingBefore() const;

    /**
     * @fn int16_t ScrollList::getPaddingAfter() const;
     *
     * @brief Gets distance after last drawable in ScrollList.
     *
     *        Gets distance after last drawable in ScrollList.
     *
     * @return The distance after the last drawable in the ScrollList.
     *
     * @see setPadding
     * @see getPaddingBefore
     */
    int16_t getPaddingAfter() const;

    /**
     * @fn void ScrollList::setSnapping(bool snap);
     *
     * @brief Sets snapping.
     *
     *        Set snapping. If snapping is false, the items can flow freely. If snapping is true,
     *        the items will snap into place so an item is always in the "selected" spot.
     *
     * @param snap true to snap.
     */
    void setSnapping(bool snap);

    /**
     * @fn bool ScrollList::getSnapping() const;
     *
     * @brief Gets the current snap stetting.
     *
     *        Gets the current snap stetting.
     *
     * @return true if snapping is set, false otherwise.
     */
    bool getSnapping() const;

    /**
     * @fn int16_t ScrollList::getItem(int16_t drawableIndex)
     *
     * @brief Gets an item.
     *
     *        Gets an item.
     *
     * @param drawableIndex Zero-based index of the drawable.
     *
     * @return The item.
     */
    int16_t getItem(int16_t drawableIndex)
    {
        return list.getItemIndex(drawableIndex);
    }

    /**
     * @fn virtual void ScrollList::handleClickEvent(const ClickEvent& evt);
     *
     * @brief Handles the click event described by evt.
     *
     * @param evt The event.
     */
    virtual void handleClickEvent(const ClickEvent& evt);

protected:
    /**
     * @fn virtual int32_t ScrollList::getPositionForItem(int16_t itemIndex);
     *
     * @brief Gets position for an item.
     *
     *        Get the position for an item. The position should ensure that the item is in view as
     *        defined by the semantics of the actual scroll class. If the item is already in view,
     *        the current offset is returned and not the offset of the given item.
     *
     * @param itemIndex Zero-based index of the item.
     *
     * @return The position for item.
     */
    virtual int32_t getPositionForItem(int16_t itemIndex);

    /**
     * @fn int32_t ScrollList::getNearestAlignedOffset(int32_t offset) const;
     *
     * @brief Gets nearest aligned offset.
     *
     * @param offset The offset.
     *
     * @return The nearest aligned offset.
     */
    virtual int32_t getNearestAlignedOffset(int32_t offset) const;

    /**
     * @fn virtual int32_t ScrollList::keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const;
     *
     * @brief Keep offset inside limits.
     *
     * @param newOffset The new offset.
     * @param overShoot The over shoot.
     *
     * @return An int32_t.
     */
    virtual int32_t keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const;

    int16_t paddingAfterLastItem;   ///< The distance after last item
    bool snapping;                   ///< True to snapping
    int windowSize;                  ///< Size of the window
};
} // namespace touchgfx

#endif // SCROLLLIST_HPP
