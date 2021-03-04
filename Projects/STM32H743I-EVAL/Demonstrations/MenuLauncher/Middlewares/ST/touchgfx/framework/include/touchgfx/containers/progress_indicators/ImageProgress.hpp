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

#ifndef IMAGEPROGRESS_HPP
#define IMAGEPROGRESS_HPP

#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
/**
 * @class ImageProgress ImageProgress.hpp ImageProgress.hpp touchgfx/containers/progress_indicators/ImageProgress.hpp
 *
 * @brief An image progress.
 *
 *        An image progress will show parts of an image as a progress indicator. The image can
 *        progress from the left, the right, the bottom or the top of the given area, and can
 *        visually be fixed with a larger and larger portion of the image showing, or it can be
 *        moved into view.
 */
class ImageProgress : public AbstractDirectionProgress
{
public:

    /**
     * @fn ImageProgress::ImageProgress();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ImageProgress();

    /**
     * @fn virtual ImageProgress::~ImageProgress();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ImageProgress();

    /**
     * @fn virtual void ImageProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);
     *
     * @brief Sets the position and dimension of the image progress indicator.
     *
     *        Sets the position and dimension of the image progress indicator relative to the
     *        background image.
     *
     * @param x      The x coordinate.
     * @param y      The y coordinate.
     * @param width  The width of the image progress indicator.
     * @param height The height of the image progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * @fn virtual void ImageProgress::setAnchorAtZero(const bool anchorAtZero);
     *
     * @brief Sets anchor at zero.
     *
     *        Sets anchor at zero will control whether the image will be placed so that it is
     *        not moving during progress, only more and more of the image will become visible,
     *        or if the image is anchored at the current progress and will appear to slide into
     *        view.
     *
     * @param anchorAtZero true to anchor at zero, false to anchor at current progress.
     *
     * @see getAnchoredAtZero
     */
    virtual void setAnchorAtZero(bool anchorAtZero);

    /**
     * @fn virtual bool ImageProgress::getAnchorAtZero() const;
     *
     * @brief Gets anchor at zero.
     *
     *        Gets anchor at zero.
     *
     * @return true if the image is anchored at zero, false if it is anchored at current
     *         progress.
     *
     * @see setAnchorAtZero
     */
    virtual bool getAnchorAtZero() const;

    /**
     * @fn virtual void ImageProgress::setBitmap(touchgfx::BitmapId bitmapId);
     *
     * @brief Sets the bitmap id.
     *
     *        Sets the bitmap id to use for progress. Please note that the bitmap is tiled
     *        which will allow smaller bitmaps to repeat and save memory.
     *
     * @param bitmapId The bitmap id.
     *
     * @see getBitmap
     * @see TiledImage
     */
    virtual void setBitmap(touchgfx::BitmapId bitmapId);

    /**
     * @fn virtual touchgfx::BitmapId ImageProgress::getBitmap() const;
     *
     * @brief Gets the image.
     *
     *        Gets the image.
     *
     * @return The image.
     *
     * @see setBitmap
     */
    virtual touchgfx::BitmapId getBitmap() const;

    /**
     * @fn virtual void ImageProgress::setAlpha(uint8_t alpha);
     *
     * @brief Sets the alpha.
     *
     *        Sets the alpha of the image.
     *
     * @param alpha The alpha.
     *
     * @see getAlpha
     * @see Image::setAlpha
     */
    virtual void setAlpha(uint8_t alpha);

    /**
     * @fn virtual uint8_t ImageProgress::getAlpha() const;
     *
     * @brief Gets the alpha.
     *
     * @return The the alpha of the image.
     *
     * @see setAlpha
     * @see Image::getAlpha
     */
    virtual uint8_t getAlpha() const;

    virtual void setValue(int value);

protected:
    Container container; ///< The container
    TiledImage image;    ///< The image
    bool fixedPosition;  ///< true if the image should not move during progress
};
}

#endif // IMAGEPROGRESS_HPP
