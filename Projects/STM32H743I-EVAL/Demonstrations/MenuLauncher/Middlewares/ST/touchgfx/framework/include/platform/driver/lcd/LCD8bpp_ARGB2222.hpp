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

#ifndef LCD8BPP_ARGB2222_HPP
#define LCD8BPP_ARGB2222_HPP

#include <stdarg.h>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/TextureMapTypes.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
#undef LCD

/**
 * @class LCD8bpp_ARGB2222 LCD8bpp_ARGB2222.hpp platform/driver/lcd/LCD8bpp_ARGB2222.hpp
 *
 * @brief This class contains the various low-level drawing routines for drawing bitmaps.
 *
 *        This class contains the various low-level drawing routines for drawing bitmaps, texts
 *        and rectangles on 16 bits per pixel displays.
 *
 * @note All coordinates are expected to be in absolute coordinates!
 *
 * @see LCD
 */
class LCD8bpp_ARGB2222 : public LCD
{
public:

    virtual ~LCD8bpp_ARGB2222() {}

    /**
     * @fn virtual void LCD8bpp_ARGB2222::init();
     *
     * @brief Performs initialization.
     *
     *        Performs initialization.
     */
    virtual void init();

    /**
     * @fn virtual void LCD8bpp_ARGB2222::drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);
     *
     * @brief Draws a portion of a bitmap.
     *
     * @param bitmap       The bitmap to draw.
     * @param x            The absolute x coordinate to place pixel (0, 0) on the screen.
     * @param y            The absolute y coordinate to place pixel (0, 0) on the screen.
     * @param rect         A rectangle describing what region of the bitmap is to be drawn.
     * @param alpha        Optional alpha value. Default is 255 (solid).
     * @param useOptimized if false, do not attempt to substitute (parts of) this bitmap with
     *                     faster fillrects.
     */
    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);

    /**
     * @fn virtual void LCD8bpp_ARGB2222::blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);
     *
     * @brief Blits a 2D source-array to the framebuffer.
     *
     *        Blits a 2D source-array to the framebuffer performing alpha-blending (and
     *        tranparency keying) as specified Performs a software blend if HAL does not
     *        support BLIT_COPY_WITH_ALPHA and alpha != 255.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data).  The sourceData must be stored as 16-bits RGB565
     *                             values.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and
     *                             require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    /**
     * @fn virtual void LCD8bpp_ARGB2222::blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);
     *
     * @brief Blits a 2D source-array to the framebuffer while converting the format.
     *
     *        Blits a 2D source-array to the framebuffer performing alpha-blending (and
     *        tranparency keying) as specified. Performs a software blend if HAL does not
     *        support BLIT_COPY_WITH_ALPHA and alpha != 255. LCD16 supports source data
     *        formats: RGB565 and ARGB8888.
     *
     * @param sourceData           The source-array pointer (points to the beginning of the
     *                             data). The sourceData must be stored in a format suitable for
     *                             the selected display.
     * @param sourceFormat         The bitmap format used in the source data.
     * @param source               The location and dimension of the source.
     * @param blitRect             A rectangle describing what region is to be drawn.
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and
     *                             require hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    /**
     * @fn virtual uint16_t* LCD8bpp_ARGB2222::copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, const BitmapId bitmapId);
     *
     * @brief Copies part of the frame buffer region to memory.
     *
     *        Copies part of the framebuffer region to memory. The memory is given as BitmapId,
     *        which can be BITMAP_ANIMATION_STORAGE. The two regions given are the visible region
     *        and the absolute region on screen. This is used to copy only a part of an area. This
     *        might be the case if a SnapshotWidget is placed inside a Container where parts of the
     *        SnapshowWidget is outside the area defined by the Container. The visible region must
     *        be completely inside the absolute region.
     *
     * @note There is only one instance of animation storage. The content of the animation storage
     *       outside the given region is undefined.
     *
     * @param visRegion The visible region.
     * @param absRegion The absolute region.
     * @param bitmapId  Identifier for the bitmap.
     *
     * @returns Null if it fails, else a pointer to the data in the given bitmap.
     *
     * @see blitCopy
     */
    virtual uint16_t* copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, const BitmapId bitmapId);

    /**
     * @fn virtual void LCD8bpp_ARGB2222::fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);
     *
     * @brief Draws a filled rectangle in the specified color.
     *
     *        Draws a filled rectangle in the specified color.
     *
     * @param rect  The rectangle to draw in absolute coordinates.
     * @param color The rectangle color.
     * @param alpha The rectangle opacity (255=solid)
     */
    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);

    /**
     * @fn virtual uint8_t LCD8bpp_ARGB2222::bitDepth() const
     *
     * @brief Number of bits per pixel used by the display.
     *
     *        Number of bits per pixel used by the display.
     *
     * @return 8.
     */
    virtual uint8_t bitDepth() const
    {
        return 8;
    }

    /**
     * @fn virtual Bitmap::BitmapFormat LCD8bpp_ARGB2222::framebufferFormat() const
     *
     * @brief Framebuffer format used by the display
     *
     *        Framebuffer format used by the display
     *
     * @return Bitmap::ARGB2222.
     */
    virtual Bitmap::BitmapFormat framebufferFormat() const
    {
        return Bitmap::ARGB2222;
    }

    /**
     * @fn virtual uint16_t LCD8bpp_ARGB2222::framebufferStride() const
     *
     * @brief Framebuffer stride in bytes
     *
     *        Framebuffer stride in bytes. The distance (in bytes) from the start of one
     *        framebuffer row, to the next.
     *
     * @return The number of bytes in one framebuffer row.
     */
    virtual uint16_t framebufferStride() const
    {
        return getFramebufferStride();
    }

    /**
     * @fn FORCE_INLINE_FUNCTION static uint16_t LCD8bpp_ARGB2222::getFramebufferStride()
     *
     * @brief Framebuffer stride in bytes
     *
     *        Framebuffer stride in bytes. The distance (in bytes) from the start of one
     *        framebuffer row, to the next.
     *
     * @return The number of bytes in one framebuffer row.
     */
    FORCE_INLINE_FUNCTION static uint16_t getFramebufferStride()
    {
        assert(HAL::FRAME_BUFFER_WIDTH > 0 && "HAL has not been initialized yet");
        return HAL::FRAME_BUFFER_WIDTH;
    }

    /**
     * @fn virtual colortype LCD8bpp_ARGB2222::getColorFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue) const
     *
     * @brief Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     *
     *        Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     *
     * @param red   Value of the red part (0-255).
     * @param green Value of the green part (0-255).
     * @param blue  Value of the blue part (0-255).
     *
     * @return The color representation depending on LCD color format.
     */
    virtual colortype getColorFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue) const
    {
        return getColorFromRGB(red, green, blue);
    }

    /**
     * @fn FORCE_INLINE_FUNCTION static colortype LCD8bpp_ARGB2222::getColorFromRGB(uint8_t red, uint8_t green, uint8_t blue)
     *
     * @brief Gets color from RGB
     *
     * @param red   The red.
     * @param green The green.
     * @param blue  The blue.
     *
     * @return The color from RGB.
     */
    FORCE_INLINE_FUNCTION static colortype getColorFromRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        return ((red & 0xC0) >> 2) | ((green & 0xC0) >> 4) | ((blue & 0xC0) >> 6);
    }

    /**
     * @fn virtual uint8_t LCD8bpp_ARGB2222::getRedColor(colortype color) const
     *
     * @brief Gets the red color part of a color.
     *
     *        Gets the red color part of a color. As this function must work for all color depths,
     *        it can be somewhat slow if used in speed critical sections. Consider finding the
     *        color in another way, if possible.
     *
     * @param color The color value.
     *
     * @return The red part of the color.
     */
    virtual uint8_t getRedColor(colortype color) const
    {
        return getRedFromColor(color);
    }

    /**
     * @fn FORCE_INLINE_FUNCTION static uint8_t LCD8bpp_ARGB2222::getRedFromColor(colortype color)
     *
     * @brief Gets red from color
     *
     * @param color The color.
     *
     * @return The red from color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getRedFromColor(colortype color)
    {
        return ((color & 0x30) >> 4) * 0x55;
    }

    /**
     * @fn virtual uint8_t LCD8bpp_ARGB2222::getGreenColor(colortype color) const
     *
     * @brief Gets the green color part of a color.
     *
     *        Gets the green color part of a color. As this function must work for all color depths,
     *        it can be somewhat slow if used in speed critical sections. Consider finding the
     *        color in another way, if possible.
     *
     * @param color The 16 bit color value.
     *
     * @return The green part of the color.
     */
    virtual uint8_t getGreenColor(colortype color) const
    {
        return getGreenFromColor(color);
    }

    /**
     * @fn FORCE_INLINE_FUNCTION static uint8_t LCD8bpp_ARGB2222::getGreenFromColor(colortype color)
     *
     * @brief Gets green from color
     *
     * @param color The color.
     *
     * @return The green from color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getGreenFromColor(colortype color)
    {
        return ((color & 0x0C) >> 2) * 0x55;
    }

    /**
     * @fn virtual uint8_t LCD8bpp_ARGB2222::getBlueColor(colortype color) const
     *
     * @brief Gets the blue color part of a color.
     *
     *        Gets the blue color part of a color. As this function must work for all color depths,
     *        it can be somewhat slow if used in speed critical sections. Consider finding the
     *        color in another way, if possible.
     *
     * @param color The 16 bit color value.
     *
     * @return The blue part of the color.
     */
    virtual uint8_t getBlueColor(colortype color) const
    {
        return getBlueFromColor(color);
    }

    /**
     * @fn FORCE_INLINE_FUNCTION static uint8_t LCD8bpp_ARGB2222::getBlueFromColor(colortype color)
     *
     * @brief Gets blue from color
     *
     * @param color The color.
     *
     * @return The blue from color.
     */
    FORCE_INLINE_FUNCTION static uint8_t getBlueFromColor(colortype color)
    {
        return (color & 0x03) * 0x55;
    }

protected:
    static const uint16_t TRANSPARENT_COL = 0xABCD; ///< Transparency color. Deprecated, do not use.

    /**
     * @fn virtual void LCD8bpp_ARGB2222::drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionSize);
     *
     * @brief Draw scan line. Draw one horizontal line of the texture map on screen. The scan line
     *        will be drawn using perspective correct texture mapping. The appearance of the
     *        line is determined by the left and right edge and the gradients structure. The
     *        edges contain the information about the x,y,z coordinates of the left and right
     *        side respectively and also information about the u,v coordinates of the texture
     *        map used. The gradients structure contains information about how to interpolate
     *        all the values across the scan line. The data drawn should be present in the
     *        texture argument.
     *
     *        The scan line will be drawn using the additional arguments. The scan line will be
     *        placed and clipped using the absolute and dirty rectangles The alpha will
     *        determine how the scan line should be alpha blended. The subDivisionSize will
     *        determine the size of the piecewise affine texture mapped lines.
     *
     * @param dest              The description of where the texture is drawn - can be used to
     *                          issue a draw off screen.
     * @param gradients         The gradients using in interpolation across the scan line.
     * @param leftEdge          The left edge of the scan line.
     * @param rightEdge         The right edge of the scan line.
     * @param texture           The texture.
     * @param absoluteRect      The containing rectangle in absolute coordinates.
     * @param dirtyAreaAbsolute The dirty area in absolute coordinates.
     * @param renderVariant     The render variant - includes the algorithm and the pixel format.
     * @param alpha             The alpha.
     * @param subDivisionSize   The size of the subdivisions of the scan line. A value of 1 will
     *                          give a completely perspective correct texture mapped scan line. A
     *                          large value will give an affine texture mapped scan line.
     */
    virtual void drawTextureMapScanLine(const DrawingSurface& dest, const Gradients& gradients, const Edge* leftEdge, const Edge* rightEdge, const TextureSurface& texture, const Rect& absoluteRect, const Rect& dirtyAreaAbsolute, RenderingVariant renderVariant, uint8_t alpha, uint16_t subDivisionSize);

    /**
     * @fn static int LCD8bpp_ARGB2222::nextPixel(bool rotatedDisplay, TextRotation textRotation);
     *
     * @brief Find out how much to advance in the display buffer to get to the next pixel.
     *
     *        Find out how much to advance in the display buffer to get to the next pixel.
     *
     * @param rotatedDisplay Is the display running in portrait mode?
     * @param textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next pixel.
     */
    static int nextPixel(bool rotatedDisplay, TextRotation textRotation);

    /**
     * @fn static int LCD8bpp_ARGB2222::nextLine(bool rotatedDisplay, TextRotation textRotation);
     *
     * @brief Find out how much to advance in the display buffer to get to the next line.
     *
     *        Find out how much to advance in the display buffer to get to the next line.
     *
     * @param rotatedDisplay Is the display running in portrait mode?
     * @param textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next line.
     */
    static int nextLine(bool rotatedDisplay, TextRotation textRotation);

    /**
     * @fn virtual void LCD8bpp_ARGB2222::drawGlyph(uint16_t* wbuf8, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, uint8_t dataFormatA4, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation);
     *
     * @brief Private version of draw-glyph with explicit destination buffer pointer argument.
     *
     *        Private version of draw-glyph with explicit destination buffer pointer argument.
     *        For all parameters (except the buffer pointer) see the public function drawString().
     *
     * @param [in] wbuf8      The destination (frame) buffer to draw to.
     * @param widgetArea      The canvas to draw the glyph inside.
     * @param x               Horizontal offset to start drawing the glyph.
     * @param y               Vertical offset to start drawing the glyph.
     * @param offsetX         Horizontal offset in the glyph to start drawing from.
     * @param offsetY         Vertical offset in the glyph to start drawing from.
     * @param invalidatedArea The area to draw within.
     * @param glyph           Specifications of the glyph to draw.
     * @param glyphData       Data containing the actual glyph (dense format)
     * @param dataFormatA4    The glyph is saved using ST A4 format.
     * @param color           The color of the glyph.
     * @param bitsPerPixel    Bit depth of the glyph.
     * @param alpha           The transparency of the glyph.
     * @param rotation        Rotation to do before drawing the glyph.
     */
    virtual void drawGlyph(uint16_t* wbuf8, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, uint8_t dataFormatA4, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation);

    /**
     * @fn static void LCD8bpp_ARGB2222::blitCopyARGB8888(const uint32_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);
     *
     * @brief Blit a 2D source-array to the framebuffer.
     *
     *        Blit a 2D source-array to the framebuffer performing alpha-blending per pixel as
     *        specified if ARGB8888 is not supported by the DMA a software blend is performed.
     *
     * @param sourceData The source-array pointer (points to the beginning of the data). The
     *                   sourceData must be stored as 32- bits ARGB8888 values.
     * @param source     The location and dimension of the source.
     * @param blitRect   A rectangle describing what region is to be drawn.
     * @param alpha      The alpha value to use for blending applied to the whole image (255 =
     *                   solid, no blending)
     */
    static void blitCopyARGB8888(const uint32_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * @fn static void LCD8bpp_ARGB2222::blitCopyAlphaPerPixel(const uint16_t* sourceData16, const Rect& source, const Rect& blitRect, uint8_t alpha);
     *
     * @brief Blit a 2D source-array to the framebuffer.
     *
     *        Blit a 2D source-array to the framebuffer performing alpha-blending per pixel as
     *        specified Performs always a software blend.
     *
     * @param sourceData16 The source-array pointer (points to the beginning of the data). The
     *                     sourceData must be stored as 8-bits ARGB2222 values.
     * @param source       The location and dimension of the source.
     * @param blitRect     A rectangle describing what region is to be drawn.
     * @param alpha        The alpha value to use for blending applied to the whole image (255 =
     *                     solid, no blending)
     */
    static void blitCopyAlphaPerPixel(const uint16_t* sourceData16, const Rect& source, const Rect& blitRect, uint8_t alpha);

private:
    FORCE_INLINE_FUNCTION uint8_t bilinearInterpolation8(uint8_t tl, uint8_t tr, uint8_t bl, uint8_t br, int x, int y) const
    {
        int xy = (x * y) >> 3;
        return (tl * (32 - 2 * y - 2 * x + xy) + tr * (2 * x - xy) + bl * (2 * y - xy) + br * xy) >> 5;
    }

    FORCE_INLINE_FUNCTION float bilinearInterpolate1D(float s, float e, float t) const
    {
        return s + (e - s) * t;
    }

    FORCE_INLINE_FUNCTION uint8_t bilinearInterpolate2D(uint8_t c00, uint8_t c10, uint8_t c01, uint8_t c11, float tx, float ty) const
    {
        return (uint8_t)(bilinearInterpolate1D(bilinearInterpolate1D((float)c00, (float)c10, tx), bilinearInterpolate1D((float)c01, (float)c11, tx), ty) + 0.5f);
    }

    FORCE_INLINE_FUNCTION uint32_t argb2222to32bit(colortype col) const
    {
        return ((((int)col >> 6) * 0x55) << 24) | (LCD8bpp_ARGB2222::getRedFromColor((col)) << 16) | (LCD8bpp_ARGB2222::getGreenFromColor(col) << 8) | LCD8bpp_ARGB2222::getBlueFromColor(col);
    }

};
} // namespace touchgfx
#endif // LCD8BPP_ARGB2222_HPP
