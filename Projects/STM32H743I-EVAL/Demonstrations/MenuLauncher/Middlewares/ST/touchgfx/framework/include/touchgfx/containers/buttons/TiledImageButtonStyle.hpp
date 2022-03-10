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

#ifndef TILEDIMAGEBUTTONSTYLE_HPP
#define TILEDIMAGEBUTTONSTYLE_HPP

#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
template<class T>

/**
 * @class TiledImageButtonStyle TiledImageButtonStyle.hpp touchgfx/containers/buttons/TiledImageButtonStyle.hpp
 *
 * @brief A tiled image button style.
 *
 *        An tiled image button style. This class is supposed to be
 *        used with one of the ButtonTrigger classes to create a
 *        functional button. This class will show one of two tiled
 *        images depending on the state of the button (pressed or
 *        released).
 *
 *        The TiledImageButtonStyle does not set the size of the enclosing
 *        container (normally AbstractButtonContainer) to the size of
 *        the pressed Bitmap. This can be overridden by calling
 *        setWidth/setHeight after setting the bitmaps.
 *
 * @tparam T Generic type parameter. Typically a AbstractButtonContainer subclass.
 *
 * @see AbstractButtonContainer
 */
class TiledImageButtonStyle : public T
{
public:
    /**
     * @fn TiledImageButtonStyle::TiledImageButtonStyle()
     *
     * @brief Default constructor.
     */
    TiledImageButtonStyle() : T()
    {
        tiledImage.setXY(0, 0);
        T::add(tiledImage);
    }

    /**
     * @fn virtual TiledImageButtonStyle::~TiledImageButtonStyle()
     *
     * @brief Destructor.
     */
    virtual ~TiledImageButtonStyle() { }

    /**
     * @fn virtual void TiledImageButtonStyle::setWidth(int16_t width)
     *
     * @brief Sets a width.
     *
     * @param width The width.
     */
    virtual void setWidth(int16_t width)
    {
        tiledImage.setWidth(width);
        T::setWidth(width);
    }

    /**
     * @fn virtual void TiledImageButtonStyle::setHeight(int16_t height)
     *
     * @brief Sets a height.
     *
     * @param height The height.
     */
    virtual void setHeight(int16_t height)
    {
        tiledImage.setHeight(height);
        T::setHeight(height);
    }

    /**
     * @fn virtual void TiledImageButtonStyle::setTileBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
     *
     * @brief Sets tile bitmaps.
     *
     * @param bmpReleased The bitmap released.
     * @param bmpPressed  The bitmap pressed.
     */
    virtual void setTileBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
    {
        upTile = bmpReleased;
        downTile = bmpPressed;
        AbstractButtonContainer::setWidth(downTile.getWidth());
        AbstractButtonContainer::setHeight(downTile.getHeight());

        handlePressedUpdated();
    }

    /**
     * @fn virtual void TiledImageButtonStyle::setTileOffset(int16_t x, int16_t y);
     *
     * @brief Sets an offset into the bitmap where the tile drawing should start.
     *
     *        Sets an offset into the bitmap where the tile drawing should start.
     *
     * @param x The x coordinate offset.
     * @param y The y coordinate offset.
     */
    virtual void setTileOffset(int16_t x, int16_t y)
    {
        tiledImage.setOffset(x, y);
    }

protected:
    TiledImage tiledImage; ///< The tiled image
    Bitmap  upTile;        ///< The image to display when button is released.
    Bitmap  downTile;      ///< The image to display when button is pressed.

    /**
     * @fn virtual void TiledImageButtonStyle::handlePressedUpdated()
     *
     * @brief Handles the pressed updated.
     */
    virtual void handlePressedUpdated()
    {
        int16_t buttonWidth = AbstractButtonContainer::getWidth();
        int16_t buttonHeight = AbstractButtonContainer::getHeight();

        tiledImage.setBitmap(T::getPressed() ? downTile : upTile);
        tiledImage.setWidth(buttonWidth);
        tiledImage.setHeight(buttonHeight);
        T::handlePressedUpdated();
    }

    /**
     * @fn virtual void TiledImageButtonStyle::handleAlphaUpdated()
     *
     * @brief Handles the alpha updated.
     */
    virtual void handleAlphaUpdated()
    {
        tiledImage.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};
} // namespace touchgfx

#endif // TILEDIMAGEBUTTONSTYLE_HPP
