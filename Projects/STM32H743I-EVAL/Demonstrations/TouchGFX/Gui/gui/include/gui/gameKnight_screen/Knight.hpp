#ifndef KNIGHT_HPP_
#define KNIGHT_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

class Knight : public MoveAnimator<Container>
{
public:
    enum AnimationState
    {
        ENTERING_GAME,
        RUN,
        JUMP,
        JUMP_ATTACK,
        DEAD
    };

    Knight();
    virtual ~Knight();

    void handleTickEvent();

    void setState(AnimationState state);
    AnimationState getState() { return currentAnimationState; }

    void setGroundPosition(int ground);

    Rect getAttackArea();
    Rect getBodyArea();

    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

    void reset();
    
private:
    AnimationState currentAnimationState;
    int tickCounter;

    int xStartPosition;
    int groundY;

    FadeAnimator<AnimatedImage> activeAnimation;

    int jumpHeight;
    int animationDurationUp;
    int animationDurationDown;
    int animationSpeed;
};



#endif 
