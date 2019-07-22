


#ifndef COLLECTIBLE_ITEM_HPP
#define COLLECTIBLE_ITEM_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>

using namespace touchgfx;

class CollectibleItem : public Container
{
public:
    CollectibleItem();
    virtual ~CollectibleItem();

    void startAnimation(int16_t newSpeed);
    void stopAnimation();

    void setBitmaps(BitmapId start, BitmapId end);

    bool isRunning() { return running; }

    virtual void handleTickEvent();
private:

    int tickCounter;
    int speed;
    bool running;

    AnimatedImage image;
};

#endif /* COLLECTIBLE_ITEM_HPP */
