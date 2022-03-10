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

#ifndef SCROLLWHEEL_HPP
#define SCROLLWHEEL_HPP

//#include <touchgfx/Callback.hpp>
//#include <touchgfx/Drawable.hpp>
#include <touchgfx/containers/scrollers/ScrollWheelBase.hpp>
#include <touchgfx/containers/scrollers/DrawableList.hpp>

namespace touchgfx
{
/**
 * @class ScrollWheel ScrollWheel.hpp touchgfx/containers/scrollers/ScrollWheel.hpp
 *
 * @brief A scroll wheel.
 *
 *        A scroll wheel is a list of identically styled drawables which can be scrolled
 *        through. One of the items in the list is the "selected" one, and scrolling through
 *        the list can be done in various ways. The ScrollWheel uses the DrawableList to make
 *        it possible to handle a huge number of items using only a limited number of drawables
 *        by reusing drawables that are no longer in view.
 *
 * @see DrawableList
 * @see ScrollWheelWithSelectionStyle
 */
class ScrollWheel : public ScrollWheelBase
{
public:
    /**
     * @fn ScrollWheel::ScrollWheel();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ScrollWheel();

    /**
     * @fn virtual ScrollWheel::~ScrollWheel();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ScrollWheel();

    /**
     * @fn void ScrollWheel::setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback);
     *
     * @brief Sets the drawables.
     *
     *        Sets the drawables used by the scroll wheel. The drawables are accessed through a
     *        callback that will return the needed drawable and another callback that will put the
     *        right data in the drawable.
     *
     * @param [in,out] drawableListItems      Number of drawables.
     * @param [in,out] updateDrawableCallback The update drawable callback.
     */
    virtual void setDrawables(DrawableListItemsInterface& drawableListItems, GenericCallback<DrawableListItemsInterface*, int16_t, int16_t>& updateDrawableCallback);
};
} // namespace touchgfx

#endif // SCROLLWHEEL_HPP
