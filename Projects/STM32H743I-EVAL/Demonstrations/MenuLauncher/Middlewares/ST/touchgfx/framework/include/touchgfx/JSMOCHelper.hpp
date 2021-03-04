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

#ifndef JSMOCHELPER_HPP
#define JSMOCHELPER_HPP

#include <touchgfx/Drawable.hpp>

namespace touchgfx
{
/**
 * @class JSMOCHelper JSMOCHelper.hpp touchgfx/JSMOCHelper.hpp
 *
 * @brief Helper class providing caching of certain information while the JSMOC algorithm runs
 *        during draw operations.
 *
 *         Helper class providing caching of certain information while the JSMOC algorithm runs
 *         during draw operations. Not intended for application-level use.
 */
class JSMOCHelper
{
public:
    /**
     * @fn JSMOCHelper::JSMOCHelper()
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    JSMOCHelper()
    {
    }

    /**
     * @fn void JSMOCHelper::setWidget(Drawable* newWidget)
     *
     * @brief Sets a widget.
     *
     *        Sets a widget.
     *
     * @param [in] newWidget The widget to operate on.
     */
    void setWidget(Drawable* newWidget)
    {
        widget = newWidget;

        // Resetting the cached indicators
        cachedVisibleRect.x = CACHED_INDICATOR;
        cachedAbsX = CACHED_INDICATOR;
        cachedAbsY = CACHED_INDICATOR;
    }

    /**
     * @fn Drawable* JSMOCHelper::getWidget()
     *
     * @brief Gets the widget.
     *
     *        Gets the widget.
     *
     * @return The widget this helper operates on.
     */
    Drawable* getWidget()
    {
        return widget;
    }

    /**
     * @fn Rect& JSMOCHelper::getCachedVisibleRect()
     *
     * @brief Gets the visible rect for the widget of this helper.
     *
     *        Gets the visible rect for the widget of this helper.
     *
     * @return The visible rect for the widget of this helper. Only calculated once.
     */
    Rect& getCachedVisibleRect()
    {
        assert(widget != 0);

        if (!hasCachedVisibleRect())
        {
            Rect visibleRect(0, 0, widget->getWidth(), widget->getHeight());
            widget->getVisibleRect(visibleRect);
            cachedVisibleRect = visibleRect;
        }
        return cachedVisibleRect;
    }

    /**
     * @fn int16_t JSMOCHelper::getCachedAbsX()
     *
     * @brief Gets the absolute x coordinate for the widget of this helper.
     *
     *        Gets the absolute x coordinate for the widget of this helper.
     *
     * @return The absolute x coordinate for the widget of this helper. Only calculated once.
     */
    int16_t getCachedAbsX()
    {
        if (!hasCachedAbsX())
        {
            Rect absRect = widget->getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsX;
    }

    /**
     * @fn int16_t JSMOCHelper::getCachedAbsY()
     *
     * @brief Gets the absolute y coordinate for the widget of this helper.
     *
     *        Gets the absolute y coordinate for the widget of this helper.
     *
     * @return The absolute y coordinate for the widget of this helper. Only calculated once.
     */
    int16_t getCachedAbsY()
    {
        if (!hasCachedAbsY())
        {
            Rect absRect = widget->getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsY;
    }

    /**
     * @fn int16_t JSMOCHelper::getWidth()
     *
     * @brief Gets the width of the widget of this helper.
     *
     *        Gets the width of the widget of this helper.
     *
     * @return The width of the widget of this helper.
     */
    int16_t getWidth()
    {
        return widget->getWidth();
    }

    /**
     * @fn int16_t JSMOCHelper::getHeight()
     *
     * @brief Gets the height of the widget of this helper.
     *
     *        Gets the height of the widget of this helper.
     *
     * @return The height of the widget of this helper.
     */
    int16_t getHeight()
    {
        return widget->getHeight();
    }

    /**
     * @fn void JSMOCHelper::draw(const Rect& invalidatedArea)
     *
     * @brief Draws the widget of this helper.
     *
     *        Draws the widget of this helper.
     *
     * @param invalidatedArea The area of the widget to draw.
     */
    void draw(const Rect& invalidatedArea)
    {
        widget->draw(invalidatedArea);
    }

private:
    static const int16_t CACHED_INDICATOR = -1;

    Drawable* widget;

    Rect cachedVisibleRect;
    bool hasCachedVisibleRect()
    {
        return cachedVisibleRect.x != CACHED_INDICATOR;
    }

    int16_t cachedAbsX;
    int16_t cachedAbsY;
    bool hasCachedAbsX()
    {
        return cachedAbsX != CACHED_INDICATOR;
    }
    bool hasCachedAbsY()
    {
        return cachedAbsY != CACHED_INDICATOR;
    }
};
} // namespace touchgfx

#endif // JSMOCHELPER_HPP
