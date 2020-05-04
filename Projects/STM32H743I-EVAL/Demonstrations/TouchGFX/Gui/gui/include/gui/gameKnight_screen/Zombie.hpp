#ifndef ZOMBIE_HPP_
#define ZOMBIE_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

class Zombie : public Container
{
public:
    enum AnimationState
    {
        RUN,
        ATTACK,
        STOPPED,
        DEAD
    };

    Zombie();
    virtual ~Zombie();

    void handleTickEvent();

    void setState(AnimationState state);
    AnimationState getState() { return currentAnimationState; }

    void setGroundPosition(int ground);

    void reset();

    Rect getAttackArea();
    Rect getBodyArea();

private:
    AnimationState currentAnimationState;
    int tickCounter;

    int groundY;
    AnimatedImage activeAnimation;
};

#endif
