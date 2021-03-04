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

#ifndef HAL_HPP
#define HAL_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/hal/BlitOp.hpp>
#include <touchgfx/hal/Gestures.hpp>
#include <touchgfx/hal/DMA.hpp>
#include <touchgfx/hal/FrameBufferAllocator.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/lcd/LCD.hpp>

#include <platform/driver/touch/TouchController.hpp>
#include <platform/driver/button/ButtonController.hpp>
#include <platform/core/MCUInstrumentation.hpp>

namespace touchgfx
{
class UIEventListener;

/**
 * @class HAL HAL.hpp touchgfx/hal/HAL.hpp
 *
 * @brief Hardware Abstraction Layer.
 *
 *        Contains functions that are specific to the hardware platform the code is running on.
 */
class HAL
{
public:

    /**
     * @fn HAL::HAL(DMA_Interface& dmaInterface, LCD& display, TouchController& touchCtrl, uint16_t width, uint16_t height)
     *
     * @brief Creates a HAL instance.
     *
     *        Creates a HAL instance.
     *
     * @param [in] dmaInterface Reference to the DMA interface.
     * @param [in] display      Reference to the LCD.
     * @param [in] touchCtrl    Reference to the touch controller.
     * @param width             The width of the LCD display, in pixels.
     * @param height            The height of the LCD display, in pixels.
     */
    HAL(DMA_Interface& dmaInterface, LCD& display, TouchController& touchCtrl, uint16_t width, uint16_t height) :
        dma(dmaInterface),
        lcdRef(display),
        touchController(touchCtrl),
        mcuInstrumentation(0),
        buttonController(0),
        frameBufferAllocator(0),
        taskDelayFunc(0),
        frameBuffer0(0),
        frameBuffer1(0),
        frameBuffer2(0),
        refreshStrategy(REFRESH_STRATEGY_DEFAULT),
        fingerSize(1),
        lockDMAToPorch(true),
        auxiliaryLCD(0),
        touchSampleRate(1),
        mcuLoadPct(0),
        vSyncCnt(0),
        vSyncForFrame(1),
        vSyncCompensationEnabled(false),
        clientDirty(false),
        swapRequested(false),
        lastTouched(false),
        updateMCULoad(0),
        cc_begin(0),
        displayOrientationChangeRequested(false)
    {
        instance = this;
        DISPLAY_WIDTH = width;
        DISPLAY_HEIGHT = height;
        DISPLAY_ROTATION = rotate0;
        FRAME_BUFFER_WIDTH = DISPLAY_WIDTH;
        FRAME_BUFFER_HEIGHT = DISPLAY_HEIGHT;
        nativeDisplayOrientation = ((width >= height) ? ORIENTATION_LANDSCAPE : ORIENTATION_PORTRAIT);
    }

    /**
     * @fn virtual HAL::~HAL()
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~HAL() { }

    /**
     * @fn static HAL* HAL::getInstance()
     *
     * @brief Gets the HAL instance.
     *
     *        Gets the HAL instance.
     *
     * @return The HAL instance.
     */
    static HAL* getInstance()
    {
        return instance;
    }

    /**
     * @fn virtual void HAL::setDisplayOrientation(DisplayOrientation orientation)
     *
     * @brief Sets the desired display orientation (landscape or portrait).
     *
     *        Sets the desired display orientation (landscape or portrait). If desired
     *        orientation is different from the native orientation of the display, a rotation
     *        is automatically applied. The rotation does not incur any performance cost.
     *
     * @note A screen transition must occur before this takes effect!
     *
     * @param orientation The desired display orientation.
     */
    virtual void setDisplayOrientation(DisplayOrientation orientation)
    {
        requestedOrientation = orientation;
        displayOrientationChangeRequested = true;
    }

    /**
     * @fn DisplayOrientation HAL::getDisplayOrientation() const
     *
     * @brief Gets the current display orientation.
     *
     *        Gets the current display orientation. Will be equal to the native orientation of
     *        the display unless setDisplayOrientation has been explicitly called earlier.
     *
     * @return The current display orientation.
     */
    DisplayOrientation getDisplayOrientation() const
    {
        if (DISPLAY_ROTATION == rotate0)
        {
            return nativeDisplayOrientation;
        }
        else
        {
            return (nativeDisplayOrientation == ORIENTATION_LANDSCAPE ? ORIENTATION_PORTRAIT : ORIENTATION_LANDSCAPE);
        }
    }

    /**
     * @fn void HAL::signalDMAInterrupt()
     *
     * @brief Notify the framework that a DMA interrupt has occurred.
     *
     *        Notify the framework that a DMA interrupt has occurred.
     */
    void signalDMAInterrupt()
    {
        dma.signalDMAInterrupt();
    }

    /**
     * @fn void HAL::initialize();
     *
     * @brief This function is responsible for initializing the entire framework.
     *
     *        This function is responsible for initializing the entire framework.
     */
    void initialize();

    /**
     * @fn virtual void HAL::taskEntry();
     *
     * @brief Main event loop.
     *
     *        Main event loop. Will wait for VSYNC signal, and then process next frame. Call
     *        this function from your GUI task.
     *
     * @note This function never returns!
     */
    virtual void taskEntry();

    /**
     * @fn virtual void HAL::flushFrameBuffer();
     *
     * @brief This function is called whenever the framework has performed a complete draw.
     *
     *         On some platforms, a local frame buffer needs to be pushed to the display
     *         through a SPI channel or similar. Implement that functionality here. This
     *         function is called whenever the framework has performed a complete draw.
     */
    virtual void flushFrameBuffer();

    /**
     * @fn virtual void HAL::flushFrameBuffer(const Rect& rect);
     *
     * @brief This function is called whenever the framework has performed a partial draw.
     *
     *        This function is called whenever the framework has performed a partial draw.
     *
     * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
     *
     * @see flushFrameBuffer().
     */
    virtual void flushFrameBuffer(const Rect& rect);

    /**
     * @fn virtual void HAL::allowDMATransfers();
     *
     * @brief Allow the DMA to start transfers.
     *
     *        Allow the DMA to start transfers. Front Porch Entry is a good place to call this.
     */
    virtual void allowDMATransfers();

    /**
     * @fn void HAL::frontPorchEntered()
     *
     * @brief Has to be called from within the LCD IRQ routine when the Front Porch Entry is
     *        reached.
     *
     *        Has to be called from within the LCD IRQ routine when the Front Porch Entry is
     *        reached.
     */
    void frontPorchEntered()
    {
        allowDMATransfers();
    }

    /**
     * @fn virtual void HAL::flushDMA();
     *
     * @brief This function blocks until the DMA queue (containing BlitOps) is empty.
     *
     *        This function blocks until the DMA queue (containing BlitOps) is empty.
     */
    virtual void flushDMA();

    /**
     * @fn virtual uint16_t* HAL::lockFrameBuffer();
     *
     * @brief Waits for the frame buffer to become available for use.
     *
     *        Waits for the frame buffer to become available for use (i.e. not used by DMA
     *        transfers).
     *
     * @note Function blocks until frame buffer is available. Client code MUST call
     *       unlockFrameBuffer() when frame buffer operation has completed.
     *
     * @return A pointer to the beginning of the currently used frame buffer.
     */
    virtual uint16_t* lockFrameBuffer();

    /**
     * @fn virtual void HAL::unlockFrameBuffer();
     *
     * @brief Unlocks the frame buffer (MUST be called exactly once for each call to
     *        lockFrameBuffer()).
     *
     *        Unlocks the frame buffer (MUST be called exactly once for each call to
     *        lockFrameBuffer()).
     */
    virtual void unlockFrameBuffer();

    /**
    * @fn virtual uint16_t* HAL::getTFTFrameBuffer() const = 0;
    *
    * @brief Gets the frame buffer address used by the TFT controller.
    *
    *        Gets the frame buffer address used by the TFT controller.
    *
    * @return The address of the frame buffer currently being displayed on the TFT.
    */
    virtual uint16_t* getTFTFrameBuffer() const = 0;

    /**
     * @fn static LCD& HAL::lcd()
     *
     * @brief Gets a reference to the LCD.
     *
     *        Gets a reference to the LCD.
     *
     * @return A reference to the LCD.
     */
    static LCD& lcd()
    {
        if (instance->useAuxiliaryLCD && instance->auxiliaryLCD)
        {
            return *instance->auxiliaryLCD;
        }
        else
        {
            return instance->lcdRef;
        }
    }

    /**
     * @fn void HAL::lockDMAToFrontPorch(bool enableLock)
     *
     * @brief Function to set whether the DMA transfers are locked to the TFT update cycle.
     *
     *        Function to set whether the DMA transfers are locked to the TFT update cycle. If
     *        locked, DMA transfer will not begin until the TFT controller has finished
     *        updating the display. If not locked, DMA transfers will begin as soon as
     *        possible. Default is true (DMA is locked with TFT).
     *
     *        Disabling the lock will in most cases significantly increase rendering
     *        performance. It is therefore strongly recommended to disable it. Depending on
     *        platform this may in rare cases cause rendering problems (visible tearing on
     *        display). Please see the chapter "Optimizing DMA During TFT Controller Access"
     *        for details on this setting.
     *
     * @note This setting only has effect when using double buffering.
     *
     * @param enableLock True to lock DMA transfers to the front porch signal. Conservative,
     *                   default setting. False to disable, which will normally yield substantial
     *                   performance improvement.
     */
    void lockDMAToFrontPorch(bool enableLock)
    {
        lockDMAToPorch = enableLock;
    }

    /**
     * @fn virtual void HAL::registerTextCache(Unicode::UnicodeChar* str, uint16_t length);
     *
     * @brief Configures HAL to use the supplied buffer as text string cache.
     *
     *        Configures HAL to use the supplied buffer as text string cache. The buffer must
     *        be large enough to hold the longest string in the system. Setting this buffer is
     *        only required if cacheTextString() is actually used and its implementation
     *        requires a buffer.
     *
     * @param [in] str Pointer to buffer location.
     * @param length   Buffer length (in UnicodeChar's)
     *
     * @see cacheTextString
     */
    virtual void registerTextCache(Unicode::UnicodeChar* str, uint16_t length);

    /**
     * @fn virtual const Unicode::UnicodeChar* HAL::cacheTextString(const Unicode::UnicodeChar* str);
     *
     * @brief This function can be used to cache a given string.
     *
     *        This function can be used to cache a given string in a platform specific way to
     *        e.g. speed up access or in case the string is placed in a memory type that does
     *        not support random access such as NAND flash.
     *
     * @param str A pointer to the string which may be in external memory.
     *
     * @return A pointer to an identical string which is guaranteed to be directly readable (ie.
     *         a copy if the original string was placed in NAND flash).
     */
    virtual const Unicode::UnicodeChar* cacheTextString(const Unicode::UnicodeChar* str);

    /**
     * @fn virtual bool HAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes);
     *
     * @brief This function performs a platform-specific memcpy.
     *
     *        This function performs a platform-specific memcpy, if supported by the hardware.
     *
     * @param [out] dest Pointer to destination memory.
     * @param [in] src   Pointer to source memory.
     * @param numBytes   Number of bytes to copy.
     *
     * @return true if the copy succeeded, false if copy was not performed.
     */
    virtual bool blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes);

    /**
     * @fn virtual BlitOperations HAL::getBlitCaps()
     *
     * @brief Function for obtaining the blit capabilities of the concrete HAL implementation.
     *
     *        Function for obtaining the blit capabilities of the concrete HAL implementation.
     *        As default, will return whatever blitcaps are reported by the associated DMA
     *        object.
     *
     * @return a bitmask of the supported blitcaps.
     */
    virtual BlitOperations getBlitCaps()
    {
        return dma.getBlitCaps();
    };

    /**
     * @fn virtual void HAL::blitSetTransparencyKey(uint16_t key);
     *
     * @brief Deprecated function which can be ignored.
     *
     *        Only present for backwards compatibility in TouchGFX 4.x. Will be removed in
     *        TouchGFX 5.
     *
     * @param key The "transparent" color value.
     * @deprecated
     */
    virtual void blitSetTransparencyKey(uint16_t key);

    /**
     * @fn virtual void HAL::blitCopy(const uint16_t* pSrc, const uint8_t* pClut, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha, bool hasTransparentPixels, uint16_t dstWidth, Bitmap::BitmapFormat srcFormat, Bitmap::BitmapFormat dstFormat)
     *
     * @brief Blits a 2D source-array to the frame buffer performing alpha-blending.
     *
     *        Blits a 2D source-array to the frame buffer performing alpha-blending (and
     *        transparency keying) as specified.
     *
     * @note Alpha=255 is assumed "solid" and shall be used if HAL does not support
     *       BLIT_OP_COPY_WITH_ALPHA.
     *
     * @param pSrc                 The source-array pointer (points to first value to copy)
     * @param pClut                The CLUT pointer (points to CLUT header data which include
     *                             the type and size of this CLUT followed by colors data)
     * @param x                    The destination x coordinate on the frame buffer.
     * @param y                    The destination y coordinate on the frame buffer.
     * @param width                The width desired area of the source 2D array.
     * @param height               The height of desired area of the source 2D array.
     * @param srcWidth             The distance (in elements) from first value of first line, to
     *                             first value of second line (the source 2D array width)
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and require
     *                             hardware support for that to be enabled.
     * @param dstWidth             The distance (in elements) from first value of first line, to
     *                             first value of second line (the destination 2D array width)
     * @param srcFormat            The source buffer color format (default is the framebuffer format)
     * @param dstFormat            The destination buffer color format (default is the framebuffer
     *                             format)
     */
    void blitCopy(const uint16_t* pSrc, const uint8_t* pClut, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha, bool hasTransparentPixels, uint16_t dstWidth, Bitmap::BitmapFormat srcFormat, Bitmap::BitmapFormat dstFormat);

    /**
     * @fn virtual void HAL::blitCopy(const uint16_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha, bool hasTransparentPixels, uint16_t dstWidth, Bitmap::BitmapFormat srcFormat, Bitmap::BitmapFormat dstFormat);
     *
     * @brief Blits a 2D source-array to the frame buffer performing alpha-blending.
     *
     *        Blits a 2D source-array to the frame buffer performing alpha-blending (and
     *        transparency keying) as specified.
     *
     * @note Alpha=255 is assumed "solid" and shall be used if HAL does not support
     *       BLIT_OP_COPY_WITH_ALPHA.
     *
     * @param pSrc                 The source-array pointer (points to first value to copy)
     * @param x                    The destination x coordinate on the frame buffer.
     * @param y                    The destination y coordinate on the frame buffer.
     * @param width                The width desired area of the source 2D array.
     * @param height               The height of desired area of the source 2D array.
     * @param srcWidth             The distance (in elements) from first value of first line, to
     *                             first value of second line (the source 2D array width)
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and require
     *                             hardware support for that to be enabled.
     * @param dstWidth             The distance (in elements) from first value of first line, to
     *                             first value of second line (the destination 2D array width)
     * @param srcFormat            The source buffer color format (default is the framebuffer format)
     * @param dstFormat            The destination buffer color format (default is the framebuffer
     *                             format)
     */
    virtual void blitCopy(const uint16_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha, bool hasTransparentPixels, uint16_t dstWidth, Bitmap::BitmapFormat srcFormat, Bitmap::BitmapFormat dstFormat);

    /**
     * @fn virtual void HAL::blitCopy(const uint16_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha, bool hasTransparentPixels)
     *
     * @brief Blits a 2D source-array to the frame buffer performing alpha-blending.
     *
     *        Blits a 2D source-array to the frame buffer performing alpha-blending (and
     *        transparency keying) as specified using the default lcd format.
     *
     * @note Alpha=255 is assumed "solid" and shall be used if HAL does not support
     *       BLIT_OP_COPY_WITH_ALPHA.
     *
     * @param pSrc                 The source-array pointer (points to first value to copy)
     * @param x                    The destination x coordinate on the frame buffer.
     * @param y                    The destination y coordinate on the frame buffer.
     * @param width                The width desired area of the source 2D array.
     * @param height               The height of desired area of the source 2D array.
     * @param srcWidth             The distance (in elements) from first value of first line, to
     *                             first value of second line (the source 2D array width)
     * @param alpha                The alpha value to use for blending (255 = solid, no blending)
     * @param hasTransparentPixels If true, this data copy contains transparent pixels and require
     *                             hardware support for that to be enabled.
     */
    virtual void blitCopy(const uint16_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha, bool hasTransparentPixels);

    /**
     * @fn virtual void HAL::blitCopyARGB8888(const uint16_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha);
     *
     * @brief Blits a 2D source-array to the frame buffer performing per-pixel alpha blending.
     *
     *        Blits a 2D source-array to the frame buffer performing per-pixel alpha blending.
     *
     * @param pSrc     The source-array pointer (points to first value to copy)
     * @param x        The destination x coordinate on the frame buffer.
     * @param y        The destination y coordinate on the frame buffer.
     * @param width    The width desired area of the source 2D array.
     * @param height   The height of desired area of the source 2D array.
     * @param srcWidth The distance (in elements) from first value of first line, to first value
     *                 of second line (the source 2D array width)
     * @param alpha    The alpha value to use for blending. This is applied on every pixel, in
     *                 addition to the per-pixel alpha value (255 = solid, no blending)
     */
    virtual void blitCopyARGB8888(const uint16_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, uint8_t alpha);

    /**
     * @fn virtual void HAL::blitCopyGlyph(const uint8_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, colortype color, uint8_t alpha, BlitOperations operation);
     *
     * @brief Blits a 4bpp or 8bpp glyph - maybe use the same method and supply additional color
     *        mode arg.
     *
     *        Blits a 4bpp or 8bpp glyph - maybe use the same method and supply additional
     *        color mode arg.
     *
     * @param pSrc      The source-array pointer (points to first value to copy)
     * @param x         The destination x coordinate on the frame buffer.
     * @param y         The destination y coordinate on the frame buffer.
     * @param width     The width desired area of the source 2D array.
     * @param height    The height of desired area of the source 2D array.
     * @param srcWidth  The distance (in elements) from first value of first line, to first value
     *                  of second line (the source 2D array width)
     * @param color     Color of the text.
     * @param alpha     The alpha value to use for blending (255 = solid, no blending)
     * @param operation The operation type to use for blit copy.
     */
    virtual void blitCopyGlyph(const uint8_t* pSrc, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t srcWidth, colortype color, uint8_t alpha, BlitOperations operation);

    /**
     * @fn virtual void HAL::blitFill(colortype color, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t alpha, uint16_t dstWidth, Bitmap::BitmapFormat dstFormat);
     *
     * @brief Blits a color value to the frame buffer performing alpha-blending (and transparency
     *        keying) as specified.
     *
     *        Blits a color value to the frame buffer performing alpha-blending (and transparency
     *        keying) as specified.
     *
     * @note Alpha=255 is assumed "solid" and shall be used if HAL does not support
     *       BLIT_OP_FILL_WITH_ALPHA.
     *
     * @param color     The desired fill-color.
     * @param x         The destination x coordinate on the frame buffer.
     * @param y         The destination y coordinate on the frame buffer.
     * @param width     The width desired area of the source 2D array.
     * @param height    The height of desired area of the source 2D array.
     * @param alpha     The alpha value to use for blending (255 = solid, no blending)
     * @param dstWidth  The distance (in elements) from first value of first line, to first value of
     *                  second line (the destination 2D array width)
     * @param dstFormat The destination buffer color format (default is the framebuffer format)
     */
    virtual void blitFill(colortype color, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t alpha, uint16_t dstWidth, Bitmap::BitmapFormat dstFormat);

    /**
     * @fn virtual void HAL::blitFill(colortype color, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t alpha);
     *
     * @brief Blits a color value to the frame buffer performing alpha-blending (and transparency
     *        keying) as specified.
     *
     *        Blits a color value to the frame buffer performing alpha-blending (and transparency
     *        keying) as specified.
     *
     * @note Alpha=255 is assumed "solid" and shall be used if HAL does not support
     *       BLIT_OP_FILL_WITH_ALPHA.
     *
     * @param color  The desired fill-color.
     * @param x      The destination x coordinate on the frame buffer.
     * @param y      The destination y coordinate on the frame buffer.
     * @param width  The width desired area of the source 2D array.
     * @param height The height of desired area of the source 2D array.
     * @param alpha  The alpha value to use for blending (255 = solid, no blending)
     */
    virtual void blitFill(colortype color, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t alpha);

    /**
     * @fn virtual void HAL::registerEventListener(UIEventListener& listener);
     *
     * @brief Registers an event handler implementation with the underlying event system.
     *
     *        Registers an event handler implementation with the underlying event system. The
     *        actual HAL implementation decides whether or not multiple UIEventListener
     *        instances are allowed (including execution order).
     *
     * @param [in] listener The listener to register.
     */
    virtual void registerEventListener(UIEventListener& listener);

    /**
     * @fn virtual uint16_t* HAL::copyFBRegionToMemory(Rect meAbs);
     *
     * @brief Copies a region of the currently displayed frame buffer to memory.
     *
     *        Copies a region of the currently displayed frame buffer to memory. Used for e.g.
     *        SlideTransition and for displaying pre-rendered drawables
     *        e.g. in animations where redrawing the drawable is not necessary.
     *
     * @note Requires animation storage to be present.
     *
     * @param meAbs The frame buffer region to copy.
     *
     * @return A pointer to the memory address containing the copy of the frame buffer.
     */
    virtual uint16_t* copyFBRegionToMemory(Rect meAbs);

    /**
     * @fn virtual uint16_t* HAL::copyFBRegionToMemory(Rect meAbs, uint16_t* dst, uint32_t stride);
     *
     * @brief Copies a region of the currently displayed frame buffer to memory.
     *
     *        Copies a region of the currently displayed frame buffer
     *        to a buffer. Used for e.g.  SlideTransition and for
     *        displaying pre-rendered drawables e.g. in animations
     *        where redrawing the drawable is not necessary. The
     *        buffer can e.g. be a dynamic bitmap.
     *
     * @note Requires animation storage to be present.
     *
     * @param meAbs  The frame buffer region to copy.
     * @param dst    Address of the buffer to store the copy in.
     * @param stride The width of the target buffer (row length).
     *
     * @return A pointer to the memory address containing the copy of the frame buffer.
     */
    virtual uint16_t* copyFBRegionToMemory(Rect meAbs, uint16_t* dst, uint32_t stride);

    /**
     * @fn uint16_t HAL::getDisplayWidth() const
     *
     * @brief Gets display width.
     *
     *        Gets display width.
     *
     * @return The display width.
     */
    uint16_t getDisplayWidth() const
    {
        return DISPLAY_WIDTH;
    }

    /**
     * @fn uint16_t HAL::getDisplayHeight() const
     *
     * @brief Gets display height.
     *
     *        Gets display height.
     *
     * @return The display height.
     */
    uint16_t getDisplayHeight() const
    {
        return DISPLAY_HEIGHT;
    }

    /**
     * @fn void HAL::swapFrameBuffers();
     *
     * @brief Swaps the two frame buffers.
     *
     *        Swaps the two frame buffers.
     */
    void swapFrameBuffers();

    /**
     * @fn uint32_t HAL::getLCDRefreshCount()
     *
     * @brief Returns the number of VSync interrupts
     *
     *        Returns the number of VSync interrupts between the current drawing operation and
     *        the last drawing operation, i.e. the number of lost frames.
     *
     * @return Number of VSync since previous draw.
     */
    uint32_t getLCDRefreshCount()
    {
        return vSyncForFrame;
    };

    /**
     * @fn void HAL::setFrameRateCompensation(bool enabled)
     *
     * @brief Enables or disables compensation for lost frames
     *
     *        Enables or disables compensation for lost frames. See knowledge base article.
     *
     * @param enabled true to enable, false to disable.
     */
    void setFrameRateCompensation(bool enabled)
    {
        vSyncCompensationEnabled = enabled;
    };

    /**
     * @fn void HAL::vSync()
     *
     * @brief Called by the VSync interrupt.
     *
     *        Called by the VSync interrupt for counting of LCD refreshes.
     */
    void vSync()
    {
        vSyncCnt++;
    };

    /**
     * @fn virtual void HAL::backPorchExited()
     *
     * @brief Has to be called from within the LCD IRQ rutine when the Back Porch Exit is reached.
     *
     *        Has to be called from within the LCD IRQ rutine when the Back Porch Exit is
     *        reached.
     */
    virtual void backPorchExited()
    {
        swapFrameBuffers();
        tick();
    }

    /**
     * @fn virtual void HAL::configureInterrupts() = 0;
     *
     * @brief Configures the interrupts relevant for TouchGFX.
     *
     *        Configures the interrupts relevant for TouchGFX. This primarily entails setting
     *        the interrupt priorities for the DMA and LCD interrupts.
     */
    virtual void configureInterrupts() = 0;

    /**
     * @fn virtual void HAL::enableInterrupts() = 0;
     *
     * @brief Enables the DMA and LCD interrupts.
     *
     *        Enables the DMA and LCD interrupts.
     */
    virtual void enableInterrupts() = 0;

    /**
     * @fn virtual void HAL::disableInterrupts() = 0;
     *
     * @brief Disables the DMA and LCD interrupts.
     *
     *        Disables the DMA and LCD interrupts.
     */
    virtual void disableInterrupts() = 0;

    /**
     * @fn virtual void HAL::enableLCDControllerInterrupt() = 0;
     *
     * @brief Configure the LCD controller to fire interrupts at VSYNC.
     *
     *        Configure the LCD controller to fire interrupts at VSYNC. Called automatically
     *        once TouchGFX initialization has completed.
     */
    virtual void enableLCDControllerInterrupt() = 0;

    /**
     * @fn virtual bool HAL::sampleKey(uint8_t& key)
     *
     * @brief Sample external key event.
     *
     *        Sample external key event.
     *
     * @param [out] key Output parameter that will be set to the key value if a keypress was
     *                  detected.
     *
     * @return True if a keypress was detected and the "key" parameter is set to a value.
     */
    virtual bool sampleKey(uint8_t& key)
    {
        return false;
    }

    /**
     * @fn void HAL::setDragThreshold(uint8_t value)
     *
     * @brief Configure the threshold for reporting drag events.
     *
     *        Configure the threshold for reporting drag events. A touch input movement must
     *        exceed this value in either axis in order to report a drag. Default value is 0.
     *
     * @note Use if touch controller is not completely accurate to avoid "false" drags.
     *
     * @param value New threshold value.
     */
    void setDragThreshold(uint8_t value)
    {
        gestures.setDragThreshold(value);
    }

    static uint16_t        DISPLAY_WIDTH;         ///< The width of the LCD display in pixels.
    static uint16_t        DISPLAY_HEIGHT;        ///< The height of the LCD display in pixels.
    static DisplayRotation DISPLAY_ROTATION;      ///< The rotation from display to frame buffer.
    static uint16_t        FRAME_BUFFER_WIDTH;    ///< The width of the frame buffer in pixels.
    static uint16_t        FRAME_BUFFER_HEIGHT;   ///< The height of the frame buffer in pixels.
    static bool            USE_DOUBLE_BUFFERING;  ///< Is double buffering enabled?
    static bool            USE_ANIMATION_STORAGE; ///< Is animation storage enabled?

    /**
     * @fn virtual void HAL::setFrameBufferStartAddress(void* adr, uint16_t depth = 16, bool useDoubleBuffering = true, bool useAnimationStorage = true)
     *
     * @brief Sets the address used for frame buffers, usually located in external memory.
     *
     *        Sets the address used for frame buffers, usually located in external memory. Will
     *        reserve memory for one or two frame buffers based on display size. Will optionally
     *        also reserve memory for a third frame buffer used for animationStorage.
     *
     * @param [in] adr                 Starting address to use for frame buffers.
     * @param      depth               (Optional) Depth of each pixel in bits, default is 16.
     * @param      useDoubleBuffering  (Optional) If true, reserve memory for an extra frame buffer.
     * @param      useAnimationStorage (Optional) If true, reserve memory for animation storage.
     *
     * @deprecated Use the setFramaBufferStartAddress with 'format' parameter instead of 'depth'
     */
    virtual void setFrameBufferStartAddress(void* adr, uint16_t depth = 16, bool useDoubleBuffering = true, bool useAnimationStorage = true)
    {
        uint16_t stride = lcd().framebufferStride();
        uint32_t bufferSizeInBytes = stride * FRAME_BUFFER_HEIGHT;
        uint8_t* ptr = (uint8_t*)adr;
        void* double_buf = 0;
        void* anim_store = 0;
        if (useDoubleBuffering)
        {
            ptr += bufferSizeInBytes; // Move past used buffer (framebuffer)
            double_buf = (void*)ptr;
        }
        if (useAnimationStorage)
        {
            ptr += bufferSizeInBytes; // Move past used buffer (framebuffer and possibly double buffer)
            anim_store = (void*)ptr;
        }
        switch (depth)
        {
        case 32:
        case 24:
        case 16:
        case 8:
        case 4:
        case 2:
        case 1:
            setFrameBufferStartAddresses(adr, double_buf, anim_store);
            break;
        default:
            assert(0 && "Unsupported bit depth");
            break;
        }
    }

    /**
     * @fn virtual void HAL::setFrameBufferStartAddresses(void* frameBuffer, void* doubleBuffer, void* animationStorage)
     *
     * @brief Sets frame buffer start addresses.
     *
     *        Sets individual frame buffer start addresses.
     *
     * @param [in] frameBuffer      Buffer for frame buffer data, must be non-null.
     * @param [in] doubleBuffer     If non-null, buffer for double buffer data. If null double buffering is disabled.
     * @param [in] animationStorage If non-null, the animation storage. If null animation storage is disabled.
     */
    virtual void setFrameBufferStartAddresses(void* frameBuffer, void* doubleBuffer, void* animationStorage)
    {
        assert(frameBuffer != 0 && "A framebuffer address must be set");
        frameBuffer0 = reinterpret_cast<uint16_t*>(frameBuffer);
        frameBuffer1 = reinterpret_cast<uint16_t*>(doubleBuffer);
        frameBuffer2 = reinterpret_cast<uint16_t*>(animationStorage);
        USE_DOUBLE_BUFFERING = doubleBuffer != 0;
        USE_ANIMATION_STORAGE = animationStorage != 0;
    }

    /**
     * @fn virtual uint16_t configurePartialFrameBuffer(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height)
     *
     * @brief Configures a partial framebuffer as current framebuffer.
     *
     *        Configures a partial framebuffer as current
     *        framebuffer. This method uses the assigned
     *        FrameBufferAllocator to allocate block of compatible
     *        dimensions. The height of the allocated block is
     *        returned.
     *
     * @param x            The absolute x coordinate of the block on the screen.
     * @param y            The absolute y coordinate of the block on the screen.
     * @param width        The width of the block.
     * @param height       The height of the block requested.
     * @return             The height of the block allocated.
     */
    virtual uint16_t configurePartialFrameBuffer(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height);

    /**
     * @fn void HAL::setTouchSampleRate(int8_t sampleRateInTicks)
     *
     * @brief Sets the number of ticks between each touch screen sample.
     *
     *        Sets the number of ticks between each touch screen sample.
     *
     * @param sampleRateInTicks Sample rate. Default is 1 (every tick).
     */
    void setTouchSampleRate(int8_t sampleRateInTicks)
    {
        if (sampleRateInTicks > 0)
        {
            touchSampleRate = sampleRateInTicks;
        }
    }

    /**
     * @fn int8_t HAL::getTouchSampleRate() const
     *
     * @brief Gets the number of ticks between each touch screen sample.
     *
     *        Gets the number of ticks between each touch screen sample.
     *
     * @return Number of ticks between each touch screen sample.
     */
    int8_t getTouchSampleRate() const
    {
        return touchSampleRate;
    }

    /**
     * @fn void HAL::setMCUActive(bool active);
     *
     * @brief Register if MCU is active by measuring cpu cycles.
     *
     *        Register if MCU is active by measuring cpu cycles. If user wishes to track MCU
     *        load, this method should be called whenever the OS Idle task is scheduled in or
     *        out. This method makes calls to a concrete implementation of GPIO functionality
     *        and a concrete implementation of cpu cycles.
     *
     * @param active If true, MCU is registered as being active, inactive otherwise.
     */
    void setMCUActive(bool active);

    /**
     * @fn uint32_t HAL::getCPUCycles();
     *
     * @brief Gets the current cycle counter.
     *
     *        Gets the current cycle counter.
     *
     * @return the cycle counter.
     */
    uint32_t getCPUCycles();

    /**
     * @fn void HAL::setMCUInstrumentation(MCUInstrumentation* mcuInstr)
     *
     * @brief Stores a pointer to an instance of an MCU specific instrumentation class.
     *
     *        Stores a pointer to an instance of an MCU specific instrumentation class.
     *
     * @param [in] mcuInstr pointer to MCU instrumentation.
     */
    void setMCUInstrumentation(MCUInstrumentation* mcuInstr)
    {
        mcuInstrumentation = mcuInstr;
    }

    /**
     * @fn void HAL::enableMCULoadCalculation(bool enabled)
     *
     * @brief This method sets a flag that determines if generic HAL should calculate MCU load.
     *
     *        This method sets a flag that determines if generic HAL should calculate MCU load
     *        based on concrete MCU instrumentation.
     *
     * @param enabled If true, set flag to update MCU load.
     */
    void enableMCULoadCalculation(bool enabled)
    {
        updateMCULoad = enabled;
    }

    /**
     * @fn uint8_t HAL::getMCULoadPct() const
     *
     * @brief Gets the current MCU load.
     *
     *        Gets the current MCU load.
     *
     * @return mcuLoadPct the MCU Load in %.
     */
    uint8_t getMCULoadPct() const
    {
        return mcuLoadPct;
    }

    /**
     * @fn void HAL::setButtonController(ButtonController* btnCtrl)
     *
     * @brief Stores a pointer to an instance of a specific implementation of a ButtonController.
     *
     *        Stores a pointer to an instance of a specific implementation of a
     *        ButtonController.
     *
     * @param [in] btnCtrl pointer to button controller.
     */
    void setButtonController(ButtonController* btnCtrl)
    {
        buttonController = btnCtrl;
    }

    /**
     * @fn ButtonController* HAL::getButtonController() const
     *
     * @brief Gets the ButtonController
     *
     *        Gets the associated ButtonController.
     *
     * @return A pointer to the ButtonController, or zero if no ButtonController has been set.
     */
    ButtonController* getButtonController() const
    {
        return buttonController;
    }

    /**
     * @fn void setFrameBufferAllocator(FrameBufferAllocator* allocator)
     *
     * @brief Sets a framebuffer allocator.
     *
     *        Sets a framebuffer allocator. The framebuffer allocator
     *        is only used in partial framebuffer mode.
     *
     * @param [in] allocator pointer to a framebuffer allocator object
     */
    void setFrameBufferAllocator(FrameBufferAllocator* allocator)
    {
        frameBufferAllocator = allocator;
    }

    /**
     * @fn FrameBufferAllocator* getFrameBufferAllocator()
     *
     * @brief Gets the framebuffer allocator.
     *
     *        Gets the framebuffer allocator.
     *
     * @return The framebuffer allocator
     */
    FrameBufferAllocator* getFrameBufferAllocator()
    {
        return frameBufferAllocator;
    }
    /**
     * @fn void HAL::setFingerSize(uint8_t size)
     *
     * @brief Sets the finger size
     *
     *        Sets the finger size in pixels.
     *
     *        Setting the finger size to a size of more than 1 pixel will emulate a finger of
     *        width and height of 2*(fingersize-1)+1. This can be especially useful when trying
     *        to interact with small elements on a high ppi display. The finger size will
     *        influence which element is chosen as the point of interaction, when clicking,
     *        dragging, ... the display. A number of samples will be drawn from within the
     *        finger area and a best matching drawable will be chosen. The best matching
     *        algorithm will consider the size of the drawable and the distance from the touch
     *        point.
     *
     * @param [in] size the size of the finger.
     */
    void setFingerSize(uint8_t size)
    {
        fingerSize = size;
    }

    /**
     * @fn uint8_t HAL::getFingerSize() const
     *
     * @brief Gets the finger size.
     *
     *        Gets the finger size in pixels.
     *
     * @return The size of the finger in pixels, 1 is the default value.
     */
    uint8_t getFingerSize() const
    {
        return fingerSize;
    }

    /**
    * @fn uint16_t* HAL::getAnimationStorage() const
    *
    * @brief Gets the optional frame buffer used for animation storage.
    *
    *        Gets the optional frame buffer used for animation storage.
    *
    * @return The address or 0 if unused.
    */
    uint16_t* getAnimationStorage() const
    {
        return frameBuffer2;
    }

    /**
     * @fn typedef enum FrameRefreshStrategy
     *
     * @brief A list of available frame refresh strategies.
     *
     * @see bool setFrameRefreshStrategy(FrameRefreshStrategy s)
     */
    typedef enum
    {
        REFRESH_STRATEGY_DEFAULT,                      ///< If not explicitly set, this strategy is used.
        REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL, ///< Strategy optimized for single frame buffer on systems with TFT controller.
        REFRESH_STRATEGY_PARTIAL_FRAMEBUFFER           ///< Strategy using less than a full frame buffer.
    } FrameRefreshStrategy;

    /**
     * @fn bool HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
     *
     * @brief Set a specific strategy for handling timing and mechanism of frame buffer drawing.
     *
     *        By setting a different frame refresh strategy, the internals of how TouchGFX
     *        interacts with the frame buffer can be modified.
     *
     *        Currently there are two strategies available. This will increase over time.
     *        - REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL: this strategy is available
     *        on targets that use single buffering on a TFT controller based system. It
     *        requires an implementation of the getTFTCurrentLine() function as well as a task
     *        delay function being registered. The implementation of this strategy is that
     *        TouchGFX will carefully track the progress of the TFT controller, and draw parts
     *        of the frame buffer whenever possible. The effect is that the risk of tearing is
     *        much reduced compared to the default single buffer strategy of only drawing in
     *        porch areas. It does have a drawback of slightly increased MCU load. But in many
     *        cases employing this strategy will make it possible to avoid external RAM, by
     *        using just a single frame buffer in internal RAM and still avoid tearing.
     *
     *        - REFRESH_STRATEGY_DEFAULT: This is a general strategy that works for all target
     *        configurations.
     *
     *        Recommendation: Try using REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL if you're
     *        on a TFT controller based system (ie. non-8080) and you have a desire to avoid
     *        external RAM. Otherwise stick to REFRESH_STRATEGY_DEFAULT.
     *
     * @param s The desired strategy to use.
     *
     * @return true if the desired strategy will be used, false otherwise.
     */
    bool setFrameRefreshStrategy(FrameRefreshStrategy s)
    {
        if (s == REFRESH_STRATEGY_DEFAULT || s == REFRESH_STRATEGY_PARTIAL_FRAMEBUFFER)
        {
            refreshStrategy = s;
            return true;
        }
        else if (s == REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL)
        {
            // Perform sanity checks. This strategy requires
            //   - task delay function
            //   - a TFT controller (+ an impl of getTFTCurrentLine())
            //   - single buffering
            if (taskDelayFunc != 0 && getTFTCurrentLine() != 0xFFFF && !USE_DOUBLE_BUFFERING)
            {
                refreshStrategy = s;
                return true;
            }
            return false;
        }
        // Unknown strategy
        return false;
    }

    /**
     * @fn FrameRefreshStrategy HAL::getFrameRefreshStrategy() const
     *
     * @brief Used internally by TouchGFX core to manage the timing and process of drawing into the
     *        frame buffer.
     *
     * @return Current frame refresh strategy.
     *
     * @see bool setFrameRefreshStrategy(FrameRefreshStrategy s)
     */
    FrameRefreshStrategy getFrameRefreshStrategy() const
    {
        return refreshStrategy;
    }

    /**
     * @fn void HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
     *
     * @brief Registers a function capable of delaying GUI task execution
     *
     *        In order to make use of the HAL::taskDelay function, a delay function must be
     *        registered by calling this function. Usually the delay function would be
     *        OSWrappers::taskDelay.
     *
     * @param [in,out] delayF A pointer to a function returning void with an uint16_t parameter
     *                        specifying number of milliseconds to delay.
     *
     * @note The task delay capability is only used when the frame refresh strategy
     *       REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL is selected. Otherwise it is not
     *       necessary to register a delay function.
     */
    void registerTaskDelayFunction(void(*delayF)(uint16_t))
    {
        taskDelayFunc = delayF;
    }

    /**
     * @fn virtual void HAL::taskDelay(uint16_t ms)
     *
     * @brief Delay GUI task execution by number of milliseconds
     *
     *        This function requires the presence of a task delay function. If a task delay
     *        function has not been registered, it returns immediately. Otherwise it returns
     *        when number of milliseconds has passed.
     *
     * @param ms Number of milliseconds to wait.
     *
     * @see void registerTaskDelayFunction(void (*delayF)(uint16_t))
     */
    virtual void taskDelay(uint16_t ms)
    {
        if (taskDelayFunc)
        {
            taskDelayFunc(ms);
        }
    }

    /**
     * @fn virtual uint16_t HAL::getTFTCurrentLine()
     *
     * @brief Get the current line (Y) of the TFT controller
     *
     *        This function is used to obtain the progress of the TFT controller. More
     *        specifically, the line (or Y-value) currently being transferred.
     *
     *        Note: The value must be adjusted to account for vertical back porch before
     *        returning, such that the value is always within the range of 0 &lt;= value &lt;
     *        actual display height in pixels
     *
     *        It is used for the REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL frame refresh
     *        strategy in order to synchronize frame buffer drawing with TFT controller
     *        progress. If this strategy is used, the concrete HAL subclass must provide an
     *        override of this function that returns correct line value. If this strategy is
     *        not used, then the getTFTCurrentLine function is never called and can be
     *        disregarded.
     *
     * @return In this default implementation, 0xFFFF is returned to signify "not implemented".
     */
    virtual uint16_t getTFTCurrentLine()
    {
        return 0xFFFFu;
    }

    /**
     * @fn virtual DMAType HAL::getDMAType()
     *
     * @brief Function for obtaining the DMA type of the concrete DMA implementation.
     *
     *        Function for obtaining the DMA type of the concrete DMA implementation.
     *        As default, will return DMA_TYPE_GENERIC type value.
     *
     * @return a DMAType value of the concrete DMA implementation.
     */
    virtual DMAType getDMAType()
    {
        return dma.getDMAType();
    }

    /**
     * @fn virtual void HAL::drawDrawableInDynamicBitmap(Drawable& drawable, BitmapId bitmapId)
     *
     * @brief Render a Drawable and its widgets into a dynamic bitmap.
     *
     *        Render a Drawable and its widgets into a dynamic bitmap.
     *
     * @param drawable A reference on the Drawable object to render.
     * @param bitmapId Dynamic bitmap to be used as a rendertarget.
     */
    virtual void drawDrawableInDynamicBitmap(Drawable& drawable, BitmapId bitmapId);

    /**
     * @fn virtual void HAL::drawDrawableInDynamicBitmap(Drawable& drawable, BitmapId bitmapId, const Rect& rect)
     *
     * @brief Render a Drawable and its widgets into a dynamic bitmap.
     *
     *        Render a Drawable and its widgets into a dynamic bitmap. Only the specified Rect region is updated.
     *
     * @param drawable A reference on the Drawable object to render.
     * @param bitmapId Dynamic bitmap to be used as a rendertarget.
     * @param rect Region to update.
     */
    virtual void drawDrawableInDynamicBitmap(Drawable& drawable, BitmapId bitmapId, const Rect& rect);

    /**
     * @fn void HAL::setAuxiliaryLCD(LCD* auxLCD)
     *
     * @brief Set an auxiliary LCD class to be used for offscreen rendering.
     *
     *        Set an auxiliary LCD class to be used for offscreen rendering.
     *
     * @param auxLCD A pointer on the axiliary LCD class to use for offscreen rendering.
     */
    void setAuxiliaryLCD(LCD* auxLCD)
    {
        auxiliaryLCD = auxLCD;
    }

    /**
     * @fn void HAL::getAuxiliaryLCD(LCD* auxLCD)
     *
     * @brief Get the auxiliary LCD class attached to the HAL instance if any.
     *
     *        Get the auxiliary LCD class attached to the HAL instance if any.
     *
     * @return A pointer on the axiliary LCD class attached to the HAL instance.
     */
    LCD* getAuxiliaryLCD()
    {
        return auxiliaryLCD;
    }

protected:

    /**
     * @fn virtual void HAL::tick();
     *
     * @brief This function is called at each timer tick, depending on platform implementation.
     *
     *        This function is called at each timer tick, depending on platform implementation.
     */
    virtual void tick();

    /**
     * @fn virtual bool HAL::beginFrame();
     *
     * @brief Called when beginning to rendering a frame.
     *
     *        Called when beginning to rendering a frame.
     *
     * @return true if rendering can begin, false otherwise.
     */
    virtual bool beginFrame();

    /**
     * @fn virtual void HAL::endFrame();
     *
     * @brief Called when a rendering pass is completed.
     *
     *        Called when a rendering pass is completed.
     */
    virtual void endFrame();

    /**
     * @fn virtual void HAL::setTFTFrameBuffer(uint16_t* address) = 0;
     *
     * @brief Sets the frame buffer address used by the TFT controller.
     *
     *        Sets the frame buffer address used by the TFT controller.
     *
     * @param [in] address New frame buffer address.
     */
    virtual void setTFTFrameBuffer(uint16_t* address) = 0;

    /**
     * @fn uint16_t* HAL::getClientFrameBuffer()
     *
     * @brief Gets client frame buffer.
     *
     *        Gets client frame buffer.
     *
     * @return The address of the framebuffer currently used by the framework to draw in.
     */
    uint16_t* getClientFrameBuffer()
    {
        if (USE_DOUBLE_BUFFERING && getTFTFrameBuffer() == frameBuffer0)
        {
            return frameBuffer1;
        }
        return frameBuffer0;
    }

    /**
     * @fn virtual void HAL::touch(int32_t x, int32_t y);
     *
     * @brief Called by the touch driver to indicate a touch.
     *
     *        Called by the touch driver to indicate a touch.
     *
     * @param x The x coordinate of the touch.
     * @param y The y coordinate of the touch.
     */
    virtual void touch(int32_t x, int32_t y);

    /**
     * @fn virtual void HAL::noTouch();
     *
     * @brief Called by the touch driver to indicate that no touch is currently detected.
     *
     *        Called by the touch driver to indicate that no touch is currently detected.
     */
    virtual void noTouch();

    /**
     * @fn virtual void HAL::performDisplayOrientationChange()
     *
     * @brief Perform the actual display orientation change.
     *
     *        Perform the actual display orientation change.
     */
    virtual void performDisplayOrientationChange()
    {
        if (requestedOrientation != nativeDisplayOrientation)
        {
            if (DISPLAY_ROTATION == rotate0)
            {
                FRAME_BUFFER_WIDTH = DISPLAY_WIDTH;
                FRAME_BUFFER_HEIGHT = DISPLAY_HEIGHT;
                DISPLAY_HEIGHT = FRAME_BUFFER_WIDTH;
                DISPLAY_WIDTH = FRAME_BUFFER_HEIGHT;
                DISPLAY_ROTATION = rotate90;
            }
        }
        else if (DISPLAY_ROTATION != rotate0)
        {
            FRAME_BUFFER_WIDTH = DISPLAY_HEIGHT;
            FRAME_BUFFER_HEIGHT = DISPLAY_WIDTH;
            DISPLAY_HEIGHT = FRAME_BUFFER_HEIGHT;
            DISPLAY_WIDTH = FRAME_BUFFER_WIDTH;
            DISPLAY_ROTATION = rotate0;
        }
    }

    DMA_Interface&         dma;                         ///< A reference to the DMA interface.
    LCD&                   lcdRef;                      ///< A reference to the LCD.
    TouchController&       touchController;             ///< A reference to the touch controller.
    MCUInstrumentation*    mcuInstrumentation;          ///< A reference to an optional MCU instrumentation.
    ButtonController*      buttonController;            ///< A reference to an optional ButtonController.
    FrameBufferAllocator*  frameBufferAllocator;        ///< A reference to an optional FrameBufferAllocator.
    static                 bool isDrawing;              ///< True if currently in the process of rendering a screen.
    Gestures               gestures;                    ///< Class for low-level interpretation of touch events.
    DisplayOrientation     nativeDisplayOrientation;    ///< Contains the native display orientation. If desired orientation is different, apply rotation.
    void(*taskDelayFunc)(uint16_t);                    ///< Pointer to a function that can delay GUI task for a number of milliseconds.
    uint16_t*              frameBuffer0;                ///< Pointer to the first frame buffer.
    uint16_t*              frameBuffer1;                ///< Pointer to the second frame buffer.
    uint16_t*              frameBuffer2;                ///< Pointer to the optional third frame buffer used for animation storage.
    FrameRefreshStrategy   refreshStrategy;             ///< The selected display refresh strategy.
    uint8_t                fingerSize;                  ///< The radius of the finger in pixels
    bool                   lockDMAToPorch;              ///< Whether or not to lock DMA transfers with TFT porch signal.
    bool                   frameBufferUpdatedThisFrame; ///< True if something was drawn in the current frame.
    LCD*                   auxiliaryLCD;                ///< Auxiliary LCD class used to render Drawables into dynamic bitmaps.
    Rect                   partialFrameBufferRect;      ///< The region of the screen covered by the partial framebuffer.

private:
    UIEventListener*   listener;
    static HAL*        instance;
    int32_t            lastX;
    int32_t            lastY;
    int8_t             touchSampleRate;
    uint8_t            mcuLoadPct;
    uint8_t            vSyncCnt;
    uint8_t            vSyncForFrame;
    bool               vSyncCompensationEnabled;
    bool               clientDirty;
    bool               swapRequested;
    bool               lastTouched;
    bool               updateMCULoad;
    uint32_t           cc_begin;
    DisplayOrientation requestedOrientation;
    bool               displayOrientationChangeRequested;
    bool               useAuxiliaryLCD;

    uint16_t* getDstAddress(uint16_t x, uint16_t y, uint16_t* startAddress, uint16_t dstWidth, Bitmap::BitmapFormat dstFormat) const;
    uint16_t* getDstAddress(uint16_t x, uint16_t y, uint16_t* startAddress) const;
    uint8_t   getBitDepth(Bitmap::BitmapFormat format) const;
};
} // namespace touchgfx
#endif // HAL_HPP
