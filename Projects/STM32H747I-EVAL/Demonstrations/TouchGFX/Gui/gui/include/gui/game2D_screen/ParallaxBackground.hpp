


#ifndef PARALLAX_BACKGROUND_HPP
#define PARALLAX_BACKGROUND_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <gui/game2D_screen/CollectibleItem.hpp>

using namespace touchgfx;

class ParallaxBackground : public Container
{
public:
    ParallaxBackground(int16_t width, int16_t height);
    virtual ~ParallaxBackground();

    void startAnimation();
    void stopAnimation();

    void addPlayerCharacter(AnimatedImage& player);
    void addCollectibleItem(CollectibleItem& item);

    void setActiveTickInterval(int interval);

    virtual void handleTickEvent();
private:

    struct Layer 
    {
        Image image0;
        Image image1;
        Image image2;
        int animationUpdateSpeed;
        int animationWidth;
    };

    enum AnimationState
    {
        ANIMATION_RUNNING,
        NO_ANIMATION
    };

    AnimationState currentAnimationState;

    int tickCounter;

    int tickInterval;

    Layer background;
    Layer ground;
    Layer leaves;
    Layer treeTops;


    void initializeLayer(Layer& layer, BitmapId bmp, int animationUpdateSpeed, int animationWidth, int y);
    void moveLayer(Layer& layer, int tickCount);
};

#endif /* PARALLAX_BACKGROUND_HPP */
