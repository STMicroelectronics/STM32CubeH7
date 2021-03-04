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

#ifndef PRERENDERABLE_HPP
#define PRERENDERABLE_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
/**
 * @class PreRenderable PreRenderable.hpp touchgfx/mixins/PreRenderable.hpp
 *
 * @brief This mixin can be used on any Drawable.
 *
 *        This mixin can be used on any Drawable. It provides a preRender function, which will
 *        cache the current visual appearance of the Drawable to be cache in a memory region.
 *        Subsequent calls to draw() on this Drawable will result in a simple memcpy of the
 *        cached memory instead of the normal draw operation. This mixin can therefore be used
 *        on Drawables whose visual appearance is static and the normal draw operation takes a
 *        long time to compute.
 *
 * @note The actual uses of this mixin are rare, and the class is mainly provided for example
 *       purposes.
 *
 * @tparam T The type of Drawable to add this functionality to.
 */
template <class T>
class PreRenderable : public T
{
public:

    /**
     * @fn PreRenderable::PreRenderable()
     *
     * @brief Default constructor.
     *
     *        Default constructor. Initializes the PreRenderable.
     */
    PreRenderable() : preRenderedAddress(0) { }

    /**
     * @fn void PreRenderable::draw(const Rect& invalidatedArea) const
     *
     * @brief Overrides the draw function.
     *
     *        Overrides the draw function. If preRender has been called, perform a memcpy of
     *        the cached version. If not, just call the base class version of draw.
     *
     * @param invalidatedArea The subregion of this Drawable which needs to be redrawn.
     */
    void draw(const Rect& invalidatedArea) const
    {
        if (isPreRendered())
        {
            Rect dirty = invalidatedArea;
            Rect meAbs = T::getRect();

            uint16_t width = T::rect.width;
            int cols = dirty.width;
            int rows = dirty.height;
            int offsetPos = dirty.x + width * dirty.y;
            uint16_t* src = (uint16_t*)preRenderedAddress;

            HAL::getInstance()->blitCopy(src + offsetPos, meAbs.x + dirty.x, meAbs.y + dirty.y, cols, rows, width, 255, false);
        }
        else
        {
            T::draw(invalidatedArea);
        }
    }

    /**
     * @fn virtual void PreRenderable::setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
     *
     * @brief Add to draw chain.
     *
     * @note For TouchGFX internal use only.
     *
     * @param invalidatedArea              Include drawables that intersect with this area only.
     * @param [in,out] nextPreviousElement Modifiable element in linked list.
     */
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
    {
        T::resetDrawChainCache();
        if (isPreRendered())
        {
            if (!T::isVisible())
            {
                return;
            }
            T::nextDrawChainElement = *nextPreviousElement;
            *nextPreviousElement = this;
        }
        else
        {
            T::setupDrawChain(invalidatedArea, nextPreviousElement);
        }
    }

    /**
     * @fn bool PreRenderable::isPreRendered() const
     *
     * @brief Whether or not the snapshot of the widget been taken.
     *
     * @return Is the widget rendered.
     */
    bool isPreRendered() const
    {
        return preRenderedAddress != 0;
    }

    /**
     * @fn void PreRenderable::preRender()
     *
     * @brief Takes a snapshot of the current visual appearance of this widget.
     *
     *        Takes a snapshot of the current visual appearance of this widget. All subsequent
     *        calls to draw on this mixin will result in the snapshot being draw.
     */
    void preRender()
    {
        if (HAL::getInstance()->getBlitCaps() & BLIT_OP_COPY)
        {
            Rect meAbs = T::getRect();
            T::translateRectToAbsolute(meAbs);
            preRenderedAddress = HAL::getInstance()->copyFBRegionToMemory(meAbs);
        }
    }
private:
    uint16_t* preRenderedAddress;
};
} // namespace touchgfx
#endif // PRERENDERABLE_HPP
