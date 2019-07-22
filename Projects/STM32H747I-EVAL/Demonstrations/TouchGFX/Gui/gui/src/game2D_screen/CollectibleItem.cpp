


#include <gui/game2D_screen/CollectibleItem.hpp>
#include <BitmapDatabase.hpp>


CollectibleItem::CollectibleItem() :
    tickCounter(0),
    speed(0)
{
    add(image);

    image.setBitmaps(BITMAP_COLLECTIBLE_ITEM_00_ID, BITMAP_COLLECTIBLE_ITEM_09_ID);
    image.setUpdateTicksInterval(4);

    setWidth(image.getWidth());
    setHeight(image.getHeight());
}

CollectibleItem::~CollectibleItem()
{
}


void CollectibleItem::setBitmaps(BitmapId start, BitmapId end)
{
    image.setBitmaps(start, end);
}

void CollectibleItem::handleTickEvent()
{
    tickCounter++;

    if (speed != 0)
    {
        moveTo(getX() - speed, getY());
    }
}

void CollectibleItem::startAnimation(int16_t newSpeed)
{
    speed = newSpeed;
    running = true;
    Application::getInstance()->registerTimerWidget(this);
    image.startAnimation(false, true, true);
}

void CollectibleItem::stopAnimation()
{
    running = false;
    Application::getInstance()->unregisterTimerWidget(this);
    image.stopAnimation();
}
