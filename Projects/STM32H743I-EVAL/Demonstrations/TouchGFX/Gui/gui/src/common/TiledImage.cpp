#include <touchgfx/hal/HAL.hpp>
#include <gui/common/TiledImage.hpp>

void TiledImage::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    setWidth(bitmap.getWidth());
    setHeight(bitmap.getHeight());
}

void TiledImage::draw(const Rect& invalidatedArea) const
{
    Rect meAbs;
    translateRectToAbsolute(meAbs);

    uint16_t bitmapWidth = bitmap.getWidth();
    uint16_t bitmapHeight = bitmap.getHeight();

    const int16_t minX = (invalidatedArea.x / bitmapWidth) * bitmapWidth;
    const int16_t maxX = ((invalidatedArea.right() - 1) / bitmapWidth) * bitmapWidth;
    const int16_t minY = (invalidatedArea.y / bitmapHeight) * bitmapHeight;
    const int16_t maxY = ((invalidatedArea.bottom() - 1) / bitmapHeight) * bitmapHeight;
    
    for (int16_t x = minX; x <= maxX; x += bitmapWidth)
    {
        for (int16_t y = minY; y <= maxY; y += bitmapHeight)
        {
            Rect dirty = Rect(x, y, bitmapWidth, bitmapHeight) & invalidatedArea;
            dirty.x -= x;
            dirty.y -= y;
            HAL::lcd().drawPartialBitmap(bitmap, meAbs.x + x, meAbs.y + y, dirty, alpha);
        }
    }
}

Rect TiledImage::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }
    Rect solidRect = bitmap.getSolidRect();
    if (solidRect.width == bitmap.getWidth())
    {
        solidRect.width = getWidth();
    }
    if (solidRect.height == bitmap.getHeight())
    {
        solidRect.height = getHeight();
    }
    return rect;
}
