#ifndef TILEDIMAGE_HPP
#define TILEDIMAGE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>

/**
 * @class TiledImage TiledImage.hpp touchgfx/widgets/TiledImage.hpp
 *
 * @brief Simple widget capable of showing a bitmap.
 *
 *        Simple widget capable of showing a bitmap. The bitmap can be alpha-blended with the
 *        background and have areas of transparency (LCD::TRANSPARENT_COL).
 *
 * @see Widget
 */

using namespace touchgfx;

class TiledImage : public Widget
{
public:

    /**
     * @fn TiledImage::TiledImage(const Bitmap& bmp = Bitmap())
     *
     * @brief Default Constructor.
     *
     *        Constructs a new TiledImage with a default alpha value of 255 (solid) and a default
     *        Bitmap if none is specified.
     *
     * @param bmp The bitmap to display.
     */
    TiledImage(const Bitmap& bmp = Bitmap()) : Widget(), alpha(255)
    {
        setBitmap(bmp);
    }

    /**
     * @fn void TiledImage::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets the bitmap ID for this TiledImage.
     *
     *        Sets the bitmap ID for this TiledImage. Updates the width and height of this widget to
     *        match that of the bitmap.
     *
     * @param bmp The bitmap instance.
     *
     * @see Bitmap
     */
    void setBitmap(const Bitmap& bmp);

    /**
     * @fn void TiledImage::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn virtual void TiledImage::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the image.
     *
     *        Draws the image. This class supports partial drawing, so only the area described
     *        by the rectangle will be drawn.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn BitmapId TiledImage::getBitmap() const
     *
     * @brief Gets the BitmapId currently contained by the widget.
     *
     *        Gets the BitmapId currently contained by the widget.
     *
     * @return The current BitmapId of the widget.
     */
    BitmapId getBitmap() const
    {
        return bitmap.getId();
    }

    /**
     * @fn uint8_t TiledImage::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual Rect TiledImage::getSolidRect() const;
     *
     * @brief Gets the largest solid (non-transparent) rectangle.
     *
     *        Gets the largest solid (non-transparent) rectangle. This value is pre-calculated
     *        by the imageconverter tool.
     *
     * @return The largest solid (non-transparent) rectangle.
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual uint16_t TiledImage::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_IMAGE.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_IMAGE;
    }
protected:
    Bitmap  bitmap;               ///< The Bitmap to display.
    uint8_t alpha;                ///< The Alpha for this image.
};

#endif // TILEDIMAGE_HPP
