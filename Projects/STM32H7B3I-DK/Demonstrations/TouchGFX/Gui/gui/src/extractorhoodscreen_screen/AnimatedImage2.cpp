#include <gui/extractorhoodscreen_screen/AnimatedImage2.hpp>

namespace touchgfx
{
void AnimatedImage2::handleTickEvent()
{
    if (running)
    {
        ++ticksSinceUpdate;
        if (ticksSinceUpdate != updateTicksInterval)
        {
            return;
        }

        ticksSinceUpdate = 0;
        BitmapId currentId = getBitmap();

        currentId = (currentId + (1 * bitmapInterval));

        if (currentId > endId)
        {
            currentId = currentId + startId - endId;
        }
        Image::setBitmap(Bitmap(currentId));
        invalidate();
    }
}

void AnimatedImage2::setBitmapInterval(uint8_t val)
{
    bitmapInterval = val;
}

} // namespace touchgfx
