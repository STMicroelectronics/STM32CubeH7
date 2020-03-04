#ifndef ARROW_HPP_
#define ARROW_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

class Arrow : public Container
{
public:
    enum AnimationState
    {
        MOVING,
        STOPPED,
        HIT_TARGET
    };

    Arrow();    
    virtual ~Arrow(){}

    void handleTickEvent();

    void setState(AnimationState state);
    AnimationState getState() { return currentAnimationState; }

    void reset();

    Rect getAttackArea();
    Rect getBodyArea();

private:
    AnimationState currentAnimationState;
    int tickCounter;

    int groundY;
    FadeAnimator<Image> activeAnimation;
};

#endif
