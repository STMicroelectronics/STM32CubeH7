#include <gui/homeautomation_screen/PainterRGB565TiledBitmap.hpp>

bool PainterRGB565TiledBitmap::renderInit()
{
    bitmapARGB8888Pointer = 0;
    bitmapRGB565Pointer = 0;
    bitmapAlphaPointer = 0;

    int bitmapX = (currentX % bitmapRectToFrameBuffer.width);
    int bitmapY = (currentY % bitmapRectToFrameBuffer.height);

    if (bitmap.getFormat() == Bitmap::ARGB8888)
    {
        bitmapARGB8888Pointer = (const uint32_t*)bitmap.getData();
        if (!bitmapARGB8888Pointer)
        {
            return false;
        }
        bitmapARGB8888Pointer += bitmapX + bitmapY * bitmapRectToFrameBuffer.width;
    }
    else
    {
        bitmapRGB565Pointer = (const uint16_t*)bitmap.getData();
        if (!bitmapRGB565Pointer)
        {
            return false;
        }
        bitmapRGB565Pointer += bitmapX + bitmapY * bitmapRectToFrameBuffer.width;
        bitmapAlphaPointer = (const uint8_t*)bitmap.getAlphaData();
        if (bitmapAlphaPointer)
        {
            bitmapAlphaPointer += bitmapX + bitmapY * bitmapRectToFrameBuffer.width;
        }
    }

    return true;
}

bool PainterRGB565TiledBitmap::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    if (bitmap.getFormat() == Bitmap::ARGB8888)
    {
        uint32_t argb8888 = *bitmapARGB8888Pointer++;
        alpha = (argb8888 >> 24) & 0xFF;
        red = (argb8888 >> 16) & 0xFF;
        green = (argb8888 >> 8) & 0xFF;
        blue = (argb8888) & 0xFF;
        if ((currentX + 1) % bitmapRectToFrameBuffer.width == 0)
        {
            bitmapARGB8888Pointer -= bitmapRectToFrameBuffer.width;
        }
    }
    else
    {
        uint16_t rgb565 = *bitmapRGB565Pointer++;
        red = (rgb565 & 0xF800) >> 8;
        red |= red >> 5; // To get full range 0-0xFF, not just 0-0xF8
        green = (rgb565 & 0x07E0) >> 3;
        green |= green >> 6; // To get full range 0-0xFF, not just 0-0xFC
        blue = (rgb565 & 0x001F) << 3;
        blue |= (blue >> 5); // To get full range 0-0xFF, not just 0-0xF8
        if (bitmapAlphaPointer)
        {
            alpha = *bitmapAlphaPointer++;
        }
        else
        {
            alpha = 255; // No alpha per pixel in the image, it is solid
        }
        if ((currentX + 1) % bitmapRectToFrameBuffer.width == 0)
        {
            bitmapRGB565Pointer -= bitmapRectToFrameBuffer.width * 3;
        }
    }
    if (painterAlpha < 255)
    {
        // Apply given alpha from setAlpha()
        alpha = (((uint16_t)alpha) * ((uint16_t)painterAlpha)) / 255;
    }
    return true;
}
