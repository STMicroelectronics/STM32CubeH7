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

#ifndef TEXTUREMAPPER_HPP
#define TEXTUREMAPPER_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/Bitmap.hpp>
#include <math.h> //lint !e829

namespace touchgfx
{
/**
 * @class TextureMapper TextureMapper.hpp touchgfx/widgets/TextureMapper.hpp
 *
 * @brief The TextureMapper class is a widget capable of drawing a transformed image
 *
 *        The TextureMapper displays a transformed image. The TextureMapper can be used in
 *        effects where an image should be rotated in two or three dimensions.
 *
 *        The image can be freely scaled and rotated in three dimensions. The scaling and
 *        rotation is done around the adjustable origin. A virtual camera is applied to the
 *        rendered image yielding a perspective impression. The amount of perspective
 *        impression can be adjusted. The transformed image is clipped according to the
 *        dimensions of the TextureMapper. In order to make the image fully visible the
 *        TextureMapper should be large enough to accommodate the transformed image.
 *
 *        Note that the drawing of this widget is not trivial and typically has a significant
 *        effect on the mcu load. The number of pixels drawn, the presence of global alpha or
 *        per pixel alpha inflicts the computation and should be considered.
 *
 *        Note that this widget does not support 1 bit per pixel color depth.
 *
 * @see Widget
 */
class TextureMapper : public Widget
{
public:

    /**
     * @enum RenderingAlgorithm
     *
     * @brief Rendering algorithms of the image.
     *
     *        Rendering algorithms of the image.
     *
     *        NEAREST_NEIGHBOR: Fast algorithm with medium image quality. Good for fast
     *        animations. (Default)
     *
     *        BILINEAR_INTERPOLATION: Slower algorithm but better image quality.
     */
    enum RenderingAlgorithm
    {
        NEAREST_NEIGHBOR,
        BILINEAR_INTERPOLATION
    };

    /**
     * @fn TextureMapper::TextureMapper();
     *
     * @brief Default constructor.
     */
    TextureMapper();

    /**
     * @fn virtual TextureMapper::~TextureMapper();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~TextureMapper();

    /**
     * @fn virtual void TextureMapper::setBitmap(const Bitmap& bmp);
     *
     * @brief Sets the bitmap for the image.
     *
     *        Sets the bitmap for the image.
     *        Note that the width and height of the TextureMapper
     *        is set to the size of the image.
     *
     * @param bmp The bitmap to be used by the widget.
     */
    virtual void setBitmap(const Bitmap& bmp);

    /**
     * @fn Bitmap TextureMapper::getBitmap() const
     *
     * @brief Gets the bitmap for the image.
     *
     *        Gets the bitmap for the image.
     *
     * @return the bitmap.
     */
    Bitmap getBitmap() const
    {
        return bitmap;
    }

    /**
     * @fn virtual void TextureMapper::draw(const Rect& invalidatedArea) const;
     *
     * @brief Draws the given invalidated area.
     *
     *        Draws the given invalidated area. The part of the transformed image inside the
     *        invalidatedArea will be drawn.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @see Drawable::draw()
     */
    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * @fn virtual Rect TextureMapper::getSolidRect() const;
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
     * @fn virtual void TextureMapper::setRenderingAlgorithm(RenderingAlgorithm algorithm)
     *
     * @brief Sets the algorithm to be used.
     *
     *        Sets the algorithm to be used. Default setting is NEAREST_NEIGHBOR.
     *
     * @param algorithm The algorithm to use when rendering.
     */
    virtual void setRenderingAlgorithm(RenderingAlgorithm algorithm)
    {
        currentRenderingAlgorithm = algorithm;
    };

    /**
     * @fn virtual RenderingAlgorithm TextureMapper::getRenderingAlgorithm() const
     *
     * @brief Gets the algorithm used when rendering.
     *
     *        Gets the algorithm used when rendering.
     *
     * @return The algorithm used when rendering.
     */
    virtual RenderingAlgorithm getRenderingAlgorithm() const
    {
        return currentRenderingAlgorithm;
    }

    /**
     * @fn void TextureMapper::setAlpha(uint8_t a)
     *
     * @brief Sets the global alpha blending value.
     *
     *        Sets the global alpha blending value.
     *
     * @param a new alpha.
     */
    void setAlpha(uint8_t a)
    {
        alpha = a;
    }

    /**
    * @fn uint8_t TextureMapper::getAlpha();
    *
    * @brief Gets the current alpha value.
    *
    *        Gets the current alpha value.
    *
    * @return The current alpha value.
    *
    * @see setAlpha
    */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * @fn virtual void TextureMapper::updateAngles(float xAngle, float yAngle, float zAngle);
     *
     * @brief Updates the angles of the image.
     *
     *        Updates the angles of the image.
     *
     * @param xAngle The new x Angle.
     * @param yAngle The new y Angle.
     * @param zAngle The new x Angle.
     */
    virtual void updateAngles(float xAngle, float yAngle, float zAngle);

    /**
     * @fn virtual void TextureMapper::updateXAngle(float xAngle)
     *
     * @brief Updates the x coordinate angle described by xAngle.
     *
     *        Updates the x coordinate angle described by xAngle.
     *
     * @param xAngle The new x angle.
     */
    virtual void updateXAngle(float xAngle)
    {
        updateAngles(xAngle, yAngle, zAngle);
    }

    /**
     * @fn virtual void TextureMapper::updateYAngle(float yAngle)
     *
     * @brief Updates the y coordinate angle described by yAngle.
     *
     *        Updates the y coordinate angle described by yAngle.
     *
     * @param yAngle The new y angle.
     */
    virtual void updateYAngle(float yAngle)
    {
        updateAngles(xAngle, yAngle, zAngle);
    }

    /**
     * @fn virtual void TextureMapper::updateZAngle(float zAngle)
     *
     * @brief Updates the z coordinate angle described by zAngle.
     *
     *        Updates the z coordinate angle described by zAngle.
     *
     * @param zAngle The new z angle.
     */
    virtual void updateZAngle(float zAngle)
    {
        updateAngles(xAngle, yAngle, zAngle);
    }

    /**
     * @fn virtual float TextureMapper::getXAngle() const
     *
     * @brief Get x angle.
     *
     *        Get x angle.
     *
     * @return The x angle.
     */
    virtual float getXAngle() const
    {
        return xAngle;
    }

    /**
     * @fn virtual float TextureMapper::getYAngle() const
     *
     * @brief Get y angle.
     *
     *        Get y angle.
     *
     * @return The y angle.
     */
    virtual float getYAngle() const
    {
        return yAngle;
    }

    /**
     * @fn virtual float TextureMapper::getZAngle() const
     *
     * @brief Get z angle.
     *
     *        Get z angle.
     *
     * @return The z angle.
     */
    virtual float getZAngle() const
    {
        return zAngle;
    }

    /**
     * @fn virtual void TextureMapper::setScale(float scale);
     *
     * @brief Sets the scale of the image.
     *
     *        Sets the scale of the image.
     *
     * @param scale The new scale value.
     */
    virtual void setScale(float scale);

    /**
     * @fn virtual float TextureMapper::getScale() const
     *
     * @brief Gets the scale.
     *
     *        Gets the scale.
     *
     * @return The scale.
     */
    virtual float getScale() const
    {
        return scale;
    }

    /**
     * @fn virtual void TextureMapper::setOrigo(float x, float y, float z)
     *
     * @brief Sets the transformation origo.
     *
     *        Sets the transformation origo.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     */
    virtual void setOrigo(float x, float y, float z)
    {
        xOrigo = x;
        yOrigo = y;
        zOrigo = z;
        applyTransformation();
    }

    /**
     * @fn virtual void TextureMapper::setOrigo(float x, float y)
     *
     * @brief Sets the transformation origo.
     *
     *        Sets the transformation origo.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    virtual void setOrigo(float x, float y)
    {
        xOrigo = x;
        yOrigo = y;
        applyTransformation();
    }

    /**
     * @fn virtual float TextureMapper::getOrigoX() const
     *
     * @brief Gets transformation origo x coordinate.
     *
     *        Gets transformation origo x coordinate.
     *
     * @return The transformation origo x coordinate.
     */
    virtual float getOrigoX() const
    {
        return xOrigo;
    }

    /**
     * @fn virtual float TextureMapper::getOrigoY() const
     *
     * @brief Gets transformation origo y coordinate.
     *
     *        Gets transformation origo y coordinate.
     *
     * @return The transformation origo y coordinate.
     */
    virtual float getOrigoY() const
    {
        return yOrigo;
    }

    /**
     * @fn virtual float TextureMapper::getOrigoZ() const
     *
     * @brief Gets transformation origo z coordinate.
     *
     *        Gets transformation origo z coordinate.
     *
     * @return The transformation origo z coordinate.
     */
    virtual float getOrigoZ() const
    {
        return zOrigo;
    }

    /**
     * @fn virtual void TextureMapper::setCamera(float x, float y)
     *
     * @brief Sets the camera coordinate.
     *
     *        Sets the camera coordinate.
     *
     * @param x The x coordinate for the camera.
     * @param y The y coordinate for the camera.
     */
    virtual void setCamera(float x, float y)
    {
        xCamera = x;
        yCamera = y;
        applyTransformation();
    }

    /**
     * @fn virtual float TextureMapper::getCameraX() const
     *
     * @brief Gets camera x coordinate.
     *
     *        Gets camera x coordinate.
     *
     * @return The camera x coordinate.
     */
    virtual float getCameraX() const
    {
        return xCamera;
    }

    /**
     * @fn virtual float TextureMapper::getCameraY() const
     *
     * @brief Gets camera y coordinate.
     *
     *        Gets camera y coordinate.
     *
     * @return The camera y coordinate.
     */
    virtual float getCameraY() const
    {
        return yCamera;
    }

    /**
     * @fn virtual void TextureMapper::setCameraDistance(float d)
     *
     * @brief Sets camera distance.
     *
     *        Sets camera distance. Minimal allowed distance is MINIMAL_CAMERA_DISTANCE. Values
     *        below will be set to MINIMAL_CAMERA_DISTANCE.
     *
     * @param d The new camera distance.
     */
    virtual void setCameraDistance(float d)
    {
        cameraDistance = MAX(d, MINIMAL_CAMERA_DISTANCE);
        applyTransformation();
    }

    /**
     * @fn virtual float TextureMapper::getCameraDistance() const
     *
     * @brief Gets camera distance.
     *
     *        Gets camera distance.
     *
     * @return The camera distance.
     */
    virtual float getCameraDistance() const
    {
        return cameraDistance;
    }

    /**
     * @fn virtual void TextureMapper::setBitmapPosition(float x, float y)
     *
     * @brief Sets bitmap position.
     *
     *        Sets the position of the bitmap within the TextureMapper. The bitmap is clipped
     *        against the dimensions of the TextureMapper.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    virtual void setBitmapPosition(float x, float y)
    {
        xBitmapPosition = x;
        yBitmapPosition = y;
        applyTransformation();
    }

    /**
     * @fn virtual void TextureMapper::setBitmapPosition(int x, int y)
     *
     * @brief Sets bitmap position.
     *
     *        Sets the position of the bitmap within the TextureMapper. The bitmap is clipped
     *        against the dimensions of the TextureMapper.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    virtual void setBitmapPosition(int x, int y)
    {
        setBitmapPosition((float)x, (float)y);
    }

    /**
     * @fn virtual float TextureMapper::getBitmapPositionX() const
     *
     * @brief Gets bitmap position x coordinate.
     *
     *        Gets bitmap position x coordinate.
     *
     * @return The bitmap position x coordinate.
     */
    virtual float getBitmapPositionX() const
    {
        return xBitmapPosition;
    }

    /**
     * @fn virtual float TextureMapper::getBitmapPositionY() const
     *
     * @brief Gets bitmap position y coordinate.
     *
     *        Gets bitmap position y coordinate.
     *
     * @return The bitmap position y coordinate.
     */
    virtual float getBitmapPositionY() const
    {
        return yBitmapPosition;
    }

    /**
     * @fn virtual float TextureMapper::getX0() const
     *
     * @brief Get X0 coordinate.
     *
     *        Get the x coordinate of the top left corner of the transformed bitmap.
     *
     * @return The X0 coordinate.
     */
    virtual float getX0() const
    {
        return imageX0;
    }

    /**
     * @fn virtual float TextureMapper::getX1() const
     *
     * @brief Get X1 coordinate.
     *
     *        Get the x coordinate of the top right corner of the transformed bitmap.
     *
     * @return The X1 coordinate.
     */
    virtual float getX1() const
    {
        return imageX1;
    }

    /**
     * @fn virtual float TextureMapper::getX2() const
     *
     * @brief Get X2 coordinate.
     *
     *        Get the x coordinate of the bottom right of the transformed bitmap.
     *
     * @return The X2 coordinate.
     */
    virtual float getX2() const
    {
        return imageX2;
    }

    /**
     * @fn virtual float TextureMapper::getX3() const
     *
     * @brief Get X3 coordinate.
     *
     *        Get the x coordinate of the bottom left corner of the transformed bitmap.
     *
     * @return The X3 coordinate.
     */
    virtual float getX3() const
    {
        return imageX3;
    }

    /**
     * @fn virtual float TextureMapper::getY0() const
     *
     * @brief Get Y0 coordinate.
     *
     *        Get the y coordinate of the top left corner of the transformed bitmap.
     *
     * @return The Y0 coordinate.
     */
    virtual float getY0() const
    {
        return imageY0;
    }

    /**
     * @fn virtual float TextureMapper::getY1() const
     *
     * @brief Get Y1 coordinate.
     *
     *        Get the y coordinate of the top right corner of the transformed bitmap.
     *
     * @return The Y1 coordinate.
     */
    virtual float getY1() const
    {
        return imageY1;
    }

    /**
     * @fn virtual float TextureMapper::getY2() const
     *
     * @brief Get Y2 coordinate.
     *
     *        Get the y coordinate of the bottom right corner of the transformed bitmap.
     *
     * @return The Y2 coordinate.
     */
    virtual float getY2() const
    {
        return imageY2;
    }

    /**
     * @fn virtual float TextureMapper::getY3() const
     *
     * @brief Get Y3 coordinate.
     *
     *        Get the y coordinate of the bottom left corner of the transformed bitmap.
     *
     * @return The Y3 coordinate.
     */
    virtual float getY3() const
    {
        return imageY3;
    }

    /**
     * @fn virtual float TextureMapper::getZ0() const
     *
     * @brief Get Z0 coordinate.
     *
     *        Get the z coordinate of the top left corner of the transformed bitmap.
     *
     * @return The Z0 coordinate.
     */
    virtual float getZ0() const
    {
        return imageZ0;
    }

    /**
     * @fn virtual float TextureMapper::getZ1() const
     *
     * @brief Get Z1 coordinate.
     *
     *        Get the z coordinate of the top right corner of the transformed bitmap.
     *
     * @return The Z1 coordinate.
     */
    virtual float getZ1() const
    {
        return imageZ1;
    }

    /**
     * @fn virtual float TextureMapper::getZ2() const
     *
     * @brief Get Z2 coordinate.
     *
     *        Get the z coordinate of the bottom right corner of the transformed bitmap.
     *
     * @return The Z2 coordinate.
     */
    virtual float getZ2() const
    {
        return imageZ2;
    }

    /**
     * @fn virtual float TextureMapper::getZ3() const
     *
     * @brief Get Z3 coordinate.
     *
     *        Get the z coordinate of the bottom left corner of the transformed bitmap.
     *
     * @return The Z3 coordinate.
     */
    virtual float getZ3() const
    {
        return imageZ3;
    }

    /**
     * @fn virtual uint16_t TextureMapper::getType() const
     *
     * @brief For GUI testing only.
     *
     *        For GUI testing only. Returns type of this drawable.
     *
     * @return TYPE_TEXTUREMAPPER.
     */
    virtual uint16_t getType() const
    {
        return (uint16_t)TYPE_TEXTUREMAPPER;
    }

protected:

    /**
     * @fn void TextureMapper::applyTransformation();
     *
     * @brief Applies the transformation.
     *
     *        Transform the bitmap using the supplied origo, scale, rotation and camera. This
     *        method is called by all the methods that manipulate origo, scale, rotation and
     *        camera.
     */
    void applyTransformation();

    /**
     * @fn Rect TextureMapper::getBoundingRect() const;
     *
     * @brief Gets bounding rectangle.
     *
     *        Gets bounding rectangle of the transformed bitmap.
     *
     * @return The bounding rectangle.
     */
    Rect getBoundingRect() const;

    /**
     * @fn void TextureMapper::drawTriangle(const Rect& invalidatedArea, uint16_t* fb, const float* triangleXs, const float* triangleYs, const float* triangleZs, const float* triangleUs, const float* triangleVs) const;
     *
     * @brief Draw triangle.
     *
     *        The TextureMapper will draw the transformed bitmap by drawing two triangles. One
     *        triangle is created from the points 0,1,2 and the other triangle from the points
     *        1,2,3. The triangle is drawn using the x,y,z values from each point along with
     *        the u,v coordinates in the bitmap associated with each point.
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
     * @fn RenderingVariant TextureMapper::lookupRenderVariant() const;
     *
     * @brief Returns the rendering variant based on the bitmap format, alpha value and rendering
     *        algorithm.
     *
     *        Returns the rendering variant based on the bitmap format, alpha value and
     *        rendering algorithm.
     *
     * @return The RenderingVariant.
     */
    RenderingVariant lookupRenderVariant() const;

    RenderingAlgorithm currentRenderingAlgorithm;   ///< The current rendering algorithm.
    Bitmap             bitmap;                      ///< The bitmap to render.
    uint8_t            alpha;                       ///< An alpha value that is applied to the entire image.

    static const int MINIMAL_CAMERA_DISTANCE = 1;   ///< The minimal camera distance

    float xBitmapPosition;  ///< The bitmap position x
    float yBitmapPosition;  ///< The bitmap position y

    float xAngle;   ///< The angle x
    float yAngle;   ///< The angle y
    float zAngle;   ///< The angle z
    float scale;    ///< The scale

    float xOrigo;   ///< The origo x coordinate
    float yOrigo;   ///< The origo y coordinate
    float zOrigo;   ///< The origo z coordinate

    float xCamera;          ///< The camera x coordinate
    float yCamera;          ///< The camera y coordinate
    float cameraDistance;   ///< The camera distance

    float imageX0;    ///< The coordinate for the image points
    float imageY0;    ///< The coordinate for the image points
    float imageZ0;    ///< The coordinate for the image points
    float imageX1;    ///< The coordinate for the image points
    float imageY1;    ///< The coordinate for the image points
    float imageZ1;    ///< The coordinate for the image points
    float imageX2;    ///< The coordinate for the image points
    float imageY2;    ///< The coordinate for the image points
    float imageZ2;    ///< The coordinate for the image points
    float imageX3;    ///< The coordinate for the image points
    float imageY3;    ///< The coordinate for the image points
    float imageZ3;    ///< The coordinate for the image points

    uint16_t subDivisionSize;       ///< The size of the affine sub divisions
};
} // namespace touchgfx

#endif // TEXTUREMAPPER_HPP
