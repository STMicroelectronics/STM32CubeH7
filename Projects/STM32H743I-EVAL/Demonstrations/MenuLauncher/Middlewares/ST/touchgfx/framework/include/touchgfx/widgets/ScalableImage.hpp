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

#ifndef SCALABLEIMAGE_HPP
#define SCALABLEIMAGE_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{
/**
 * @class ScalableImage ScalableImage.hpp touchgfx/widgets/ScalableImage.hpp
 *
 * @brief Widget for representing a scaled version of a bitmap.
 *
 *        Widget for representing a scaled version of a bitmap. Simply change the width/height
 *        of the widget to resize the image. The quality of the scaled image depends of the
 *        rendering algorithm used. The rendering algorithm can be changed dynamically. Please
 *        note that scaling images is done at runtime and requires a lot of calculations.
 *        Therefore use it with some care.
 *
 *        Note that this widget does not support 1 bit per pixel color depth.
 *
 * @see Widget
 */
class ScalableImage : public Widget
{
public:

    /**
     * @enum ScalingAlgorithm
     *
     * @brief Rendering algorithms of the scaled bitmap.
     *
     *        Rendering algorithms of the scaled bitmap.
     *
     *        NEAREST_NEIGHBOR: Fast but not a very good image quality. Good for fast
     *        animations.
     *
     *        BILINEAR_INTERPOLATION: Slow but good image quality. Good for static
     *        representation of a scaled image.
     */
    enum ScalingAlgorithm
    {
        NEAREST_NEIGHBOR,
        BILINEAR_INTERPOLATION
    };

    /**
     * @fn ScalableImage::ScalableImage();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    ScalableImage();

    /**
     * @fn virtual ScalableImage::~ScalableImage();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~ScalableImage();

    /**
     * @fn virtual void ScalableImage::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets the bitmap for the image.
     *
     *        Sets the bitmap for the image.
     *
     * @param bmp The bitmap to be used by the widget.
     */
    virtual void setBitmap(const Bitmap& bmp);

    /**
     * @fn Bitmap ScalableImage::getBitmap() const
     *
     * @brief Gets the bitmap for the image.
     *
     *        Gets the bitmap for the image.
     *
     * @return the small bitmap.
     */
    Bitmap getBitmap() const
    {
        return bitmap;
    }

    /**
     * @fn virtual void ScalableImage::setAlpha(uint8_t alpha)
     *
     * @brief Sets the alpha channel for the image.
     *
     *        Sets the alpha channel for the image.
     *
     * @param alpha The alpha value. 255 = completely solid.
     */
    virtual void setAlpha(uint8_t alpha)
    {
        this->alpha = alpha;
    }

    /**
     * @fn virtual uint8_t ScalableImage::getAlpha() const
     *
     * @brief Gets the current alpha value.
     *
     *        Gets the current alpha value.
     *
     * @return The current alpha value.
     */
    virtual uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual void ScalableImage::setScalingAlgorithm(ScalingAlgorithm algorithm);
     *
     * @brief Sets the algorithm to be used.
     *
     *        Sets the algorithm to be used.
     *
     * @param algorithm The algorithm to use when rendering.
     */
    virtual void setScalingAlgorithm(ScalingAlgorithm algorithm);

    /**
     * @fn virtual ScalingAlgorithm ScalableImage::getScalingAlgorithm();
     *
     * @brief Gets the algorithm used when rendering.
     *
     *        Gets the algorithm used when rendering.
     *
     * @return The algorithm used when rendering.
     */
    virtual ScalingAlgorithm getScalingAlgorithm();

    /**
     * @fn virtual void ScalableImage::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the given invalidated area.
     *
     *        Draws the given invalidated area.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @see Drawable::draw()
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect ScalableImage::getSolidRect() const;
     *
     * @brief Gets solid rectangle.
     *
     *        Gets solid rectangle.
     *
     * @return largest possible solid rect.
     *
     * @see Drawable::getSolidRect()
     */
    virtual Rect getSolidRect() const;

    /**
     * @fn virtual uint16_t ScalableImage::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_SCALABLEIMAGE.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_SCALABLEIMAGE;
    }

protected:
    ScalingAlgorithm currentScalingAlgorithm; ///< The current scaling algorithm.
    Bitmap           bitmap;                  ///< The bitmap to render.
    uint8_t          alpha;                   ///< An alpha value that is applied to the entire image.

    /**
     * @fn void ScalableImage::drawTriangle(const Rect& invalidatedArea, uint16_t* fb, const float* triangleXs, const float* triangleYs, const float* triangleZs, const float* triangleUs, const float* triangleVs) const;
     *
     * @brief Draw a triangle part of the bitmap.
     *
     *        Draw a triangle part of the bitmap.
     *
     * @param invalidatedArea The invalidated area.
     * @param [in,out] fb     If non-null, the fb.
     * @param triangleXs      The triangle xs.
     * @param triangleYs      The triangle ys.
     * @param triangleZs      The triangle zs.
     * @param triangleUs      The triangle us.
     * @param triangleVs      The triangle vs.
     */
    void drawTriangle(const Rect& invalidatedArea, uint16_t* fb, const float* triangleXs, const float* triangleYs, const float* triangleZs, const float* triangleUs, const float* triangleVs) const;

    /**
     * @fn RenderingVariant ScalableImage::lookupRenderVariant() const;
     *
     * @brief Looks up the appropriate render variant based on the bitmap format and scaling
     *        algorithm.
     *
     *        Looks up the appropriate render variant based on the bitmap format and scaling
     *        algorithm.
     *
     * @return A RenderingVariant.
     */
    RenderingVariant lookupRenderVariant() const;
};
} // namespace touchgfx

#endif // SCALABLEIMAGE_HPP
