#ifndef HEAD_HPP_
#define HEAD_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

class Head : public Container
{
public:
    enum AnimationState
    {
        MOVING,
        STOPPED,
        DEAD
    };

    Head();
    virtual ~Head();

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
    FadeAnimator<AnimatedImage> activeAnimation;
};

#endif
