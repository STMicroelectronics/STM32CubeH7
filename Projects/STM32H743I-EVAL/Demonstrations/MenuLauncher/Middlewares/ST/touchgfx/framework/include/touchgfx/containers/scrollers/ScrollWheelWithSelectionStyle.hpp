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

#ifndef SCROLLWHEELWITHSELECTIONSTYLE_HPP
#define SCROLLWHEELWITHSELECTIONSTYLE_HPP

#include <touchgfx/containers/scrollers/DrawableList.hpp>
#include <touchgfx/containers/scrollers/ScrollWheelBase.hpp>

namespace touchgfx
{
/**
 * @class ScrollWheelWithSelectionStyle ScrollWheelWithSelectionStyle.hpp touchgfx/containers/scrollers/ScrollWheelWithSelectionStyle.hpp
 *
 * @brief A scroll wheel with selection style.
 *
 *        A scroll wheel with selection style. Similar to an ordinary ScrollWheel, but with a
 *        different style for the selected item which can thus be bold, have a different color
 *        or similar effect to highlight it.
 *
 * @see DrawableList
 * @see ScrollWheel
 */
class ScrollWheelWithSelectionStyle : public ScrollWheelBase
{
public:
    /**
     * @fn ScrollWheelWithSelectionStyle::ScrollWheelWithSelectionStyle();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ScrollWheelWithSelectionStyle();

    /**
     * @fn virtual ScrollWheelWithSelectionStyle::~ScrollWheelWithSelectionStyle();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ScrollWheelWithSelectionStyle()
    {
    }

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setWidth(int16_t width);
     *
     * @brief Sets the width.
     *
     *        Sets the width. If the scroll wheel is vertical, the width is propagated to all the
     *        drawables.
     *
     * @param width The width.
     */
    virtual void setWidth(int16_t width);

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setHeight(int16_t height);
     *
     * @brief Sets the height.
     *
     *        Sets the height. If the scroll wheel is horizontal, the height is propagated to all
     *        the drawables.
     *
     * @param height The height.
     */
    virtual void setHeight(int16_t height);

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setHorizontal(bool horizontal);
     *
     * @brief Sets whether the scroll wheel is horizontal or vertical.
     *
     *        Sets whether the scroll wheel is horizontal or vertical. If the scroll wheel is
     *        hortizontal, the items are arranged side by side, otherwise they are arranged above
     *        and below each other.
     *
     * @param horizontal True means horizontal, false means vertical.
     * @see getHorizontal
     */
    virtual void setHorizontal(bool horizontal);

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setCircular(bool circular);
     *
     * @brief Sets whether the scroll wheel is circular.
     *
     *        Sets whether the scroll wheel is circular. IF the scroll wheel is circular, it can be
     *        scrolled infinitely so that the last item appears before the first item, just like
     *        the first item appears after the last item in the list.
     *
     * @param circular True to make the scroll wheel circular.
     *
     * @see getCircular
     */
    virtual void setCircular(bool circular);

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setNumberOfItems(int16_t numberOfItems);
     *
     * @brief Sets number of items in the scroll wheel.
     *
     *        Sets number of items in the scroll wheel. The scroll wheel is refreshed to ensure
     *        that everything is displayed properly on the screen.
     *
     * @param numberOfItems Number of items.
     *
     * @see getNumberOfItems
     */
    virtual void setNumberOfItems(int16_t numberOfItems);

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setSelectedItemOffset(int16_t offset)
     *
     * @brief Sets selected item offset.
     *
     *        Sets selected item offset. This is the number of pixels from the start of the widget
     *        where the selected item is placed on screen. The offset is the relative x coordinate
     *        if the ScrollWheel is horizontal, otherwise it is the relative y coordinate. If this
     *        value is zero, the selected item is placed at the very start of the widget.
     *
     * @param offset The offset.
     */
    virtual void setSelectedItemOffset(int16_t offset);

    /**
     * @fn void ScrollWheelWithSelectionStyle::setSelectedItemExtraSize(int16_t extraSizeBefore, int16_t extraSizeAfter);
     *
     * @brief Sets selected item extra size.
     *
     *        Sets selected item extra size to make the size of the area for the center drawables
     *        larger.
     *
     * @param extraSizeBefore The extra size before.
     * @param extraSizeAfter  The extra size after.
     *
     * @see setSelectedItemOffset
     */
    virtual void setSelectedItemExtraSize(int16_t extraSizeBefore, int16_t extraSizeAfter);

    /**
     * @fn int16_t ScrollWheelWithSelectionStyle::getSelectedItemExtraSizeBefore() const
     *
     * @brief Gets selected item extra size before.
     *
     *        Gets selected item extra size before.
     *
     * @return The selected item extra size before.
     *
     * @see setSelectedItemExtraSize
     */
    virtual int16_t getSelectedItemExtraSizeBefore() const;

    /**
     * @fn int16_t ScrollWheelWithSelectionStyle::getSelectedItemExtraSizeAfter() const
     *
     * @brief Gets selected item extra size after.
     *
     *        Gets selected item extra size after.
     *
     * @return The selected item extra size after.
     *
     * @see setSelectedItemExtraSize
     */
    virtual int16_t getSelectedItemExtraSizeAfter() const;

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setSelectedItemMargin(int16_t marginBefore, int16_t marginAfter)
     *
     * @brief Sets margin around selected item.
     *
     *        Sets margin around selected item. This like an invisible area added before and after
     *        the selected item (including extra size).
     *
     * @param marginBefore The margin before.
     * @param marginAfter  The margin after.
     *
     * @see setSelectedItemOffset
     * @see setSelectedItemExtraSize
     */
    virtual void setSelectedItemMargin(int16_t marginBefore, int16_t marginAfter);

    /**
     * @fn int16_t ScrollWheelWithSelectionStyle::getSelectedItemMarginBefore() const
     *
     * @brief Gets selected item margin before.
     *
     *        Gets selected item margin before.
     *
     * @return The selected item margin before.
     *
     * @see setSelectedItemMargin
     */
    virtual int16_t getSelectedItemMarginBefore() const;

    /**
     * @fn int16_t ScrollWheelWithSelectionStyle::getSelectedItemMarginAfter() const
     *
     * @brief Gets selected item margin after.
     *
     *        Gets selected item margin after.
     *
     * @return The selected item margin after.
     *
     * @see setSelectedItemMargin
     */
    virtual int16_t getSelectedItemMarginAfter() const;

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setSelectedItemPosition(int16_t offset, int16_t extraSizeBefore, int16_t extraSizeAfter, int16_t marginBefore, int16_t marginAfter);
     *
     * @brief Sets the selected item offset.
     *
     *        Sets the selected item offset. This is the distance from the beginning of the
     *        ScrollWheel measured in pixels. The distance before and after that should also be
     *        drawn using the center drawables - for example to extend area of emphasized elements -
     *        can also be specified. Further, if a gap is needed between the "normal" drawables and
     *        the center drawables - for example to give the illusion that that items disappear
     *        under a graphical element, only to appear in the center.
     *
     *        This is a combination of setSelectedItemOffset, setSelectedItemExtraSize and
     *        setSelectedItemMargin.
     *
     * @param offset          The offset of the selected item.
     * @param extraSizeBefore The extra size before the selected item.
     * @param extraSizeAfter  The extra size after the selected item.
     * @param marginBefore    The margin before the selected item.
     * @param marginAfter     The margin after the selected item.
     *
     * @see setSelectedItemOffset
     * @see setSelectedItemExtraSize
     * @see setSelectedItemMargin
     */
    virtual void setSelectedItemPosition(int16_t offset, int16_t extraSizeBefore, int16_t extraSizeAfter, int16_t marginBefore, int16_t marginAfter);

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setDrawableSize(int16_t drawableSize, int16_t drawableMargin);
     *
     * @brief Sets drawable size.
     *
     *        Sets drawable size. Each item in the scroll wheel will have a size of the sum of the
     *        two numbers, where the drawableSize is the size of the drawable in the list and the
     *        drawableMargin is the margin between each drawable (half of which is placed before
     *        the drawable, the rest is placed after the drawable).
     *
     * @param drawableSize   Size of the drawable.
     * @param drawableMargin The drawable margin.
     *
     * @see getDrawableSize
     * @see getDrawableMargin
     * @see getItemSize
     */
    virtual void setDrawableSize(int16_t drawableSize, int16_t drawableMargin);

    /**
     * @fn void ScrollWheelWithSelectionStyle::setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback, DrawableListItemsInterface& centerDrawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateCenterDrawableCallback);
     *
     * @brief Setups the widget.
     *
     *        Setups the widget. Numerous parameters control the position of the widget, the two
     *        scroll lists inside and the values in them.
     *
     * @param [in,out] drawableListItems            Number of drawables in outer array.
     * @param [in]     updateDrawableCallback       The callback to update a drawable.
     * @param [in,out] centerDrawableListItems      Number of drawables in center array.
     * @param [in]     updateCenterDrawableCallback The callback to update a center drawable.
     */
    virtual void setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback,
                              DrawableListItemsInterface& centerDrawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateCenterDrawableCallback);

    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::itemChanged(int itemIndex);
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
     * @fn virtual void ScrollWheelWithSelectionStyle::initialize()
     *
     * @brief Initializes the contents of all drawables.
     *
     *        Initializes the contents of all drawables.
     */
    virtual void initialize()
    {
        ScrollWheelBase::initialize();
        list1.refreshDrawables();
        list2.refreshDrawables();
    }

protected:
    /**
     * @fn virtual void ScrollWheelWithSelectionStyle::setOffset(int32_t offset);
     *
     * @brief Sets offset of item 0 relative to the selected item's position.
     *
     *        Sets offset of item 0 relative to the selected item's position.
     *
     * @param offset The offset.
     *
     * @see getOffset
     */
    virtual void setOffset(int32_t offset);

    /**
     * @fn void ScrollWheelWithSelectionStyle::refreshDrawableListsLayout();
     *
     * @brief Refresh drawable lists layout.
     *
     *        Refresh drawable lists layout. Ensure that the three DrawableLists are places
     *        correctly and setup properly. This is typically done after the
     *        ScrollWheelWithSelectionStyle has been resized or the size of the selected item is
     *        changed.
     */
    void refreshDrawableListsLayout();

    int16_t drawablesInFirstList;        ///< List of drawables in firsts
    DrawableList list1;                  ///< The center list
    DrawableList list2;                  ///< The last list
    int16_t extraSizeBeforeSelectedItem; ///< The distance before selected item
    int16_t extraSizeAfterSelectedItem;  ///< The distance after selected item
    int16_t marginBeforeSelectedItem;    ///< The distance before selected item
    int16_t marginAfterSelectedItem;     ///< The distance after selected item

    DrawableListItemsInterface* drawables;       ///< The drawables at the beginning and end of the scroll wheel
    DrawableListItemsInterface* centerDrawables; ///< The drawables at the center of the scroll wheel

    GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>* originalUpdateDrawableCallback;       ///< The original update drawable callback
    GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>* originalUpdateCenterDrawableCallback; ///< The original update center drawable callback
};
} // namespace touchgfx

#endif // SCROLLWHEELWITHSELECTIONSTYLE_HPP
