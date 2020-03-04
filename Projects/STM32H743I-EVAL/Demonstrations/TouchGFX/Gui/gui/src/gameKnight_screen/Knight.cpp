#include <gui/gameKnight_screen/Knight.hpp>
#include <BitmapDatabase.hpp>

Knight::Knight() :
    groundY(0)
{
    Application::getInstance()->registerTimerWidget(this);

    activeAnimation.setBitmaps(BITMAP_RUN_00_ID, BITMAP_RUN_09_ID);
    add(activeAnimation);

    setWidth(activeAnimation.getWidth());
    setHeight(activeAnimation.getHeight());

    xStartPosition = 100;
}

Knight::~Knight()
{

}

void Knight::handleTickEvent()
{
    MoveAnimator<Container>::handleTickEvent();

    tickCounter++;

    if (tickCounter == 1)
    {
        activeAnimation.setAlpha(255);
    }

    if (currentAnimationState == RUN || currentAnimationState == ENTERING_GAME)
    {
        int enteringGameAnimationDuration = 50;

        if (tickCounter == 1)
        {
            activeAnimation.setBitmaps(BITMAP_RUN_00_ID, BITMAP_RUN_09_ID);
            activeAnimation.setXY(0, 0);
            activeAnimation.setUpdateTicksInterval(3);

            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());

            if (currentAnimationState == ENTERING_GAME)
            {
                moveTo(-300, groundY);
                setMoveAnimationDelay(0);
                startMoveAnimation(xStartPosition, groundY, enteringGameAnimationDuration, EasingEquations::circEaseOut);
            }

            activeAnimation.startAnimation(false, false, true);
        }
        else if (currentAnimationState == ENTERING_GAME && tickCounter == enteringGameAnimationDuration)
        {
            currentAnimationState = RUN;
        }
    } 
    else if (currentAnimationState == JUMP || currentAnimationState == JUMP_ATTACK)
    {
        if (tickCounter == 1)
        {
            if (currentAnimationState == JUMP)
            {
                activeAnimation.setBitmaps(BITMAP_JUMP_00_ID, BITMAP_JUMP_04_ID);
            }
            else if (currentAnimationState == JUMP_ATTACK)
            {
                activeAnimation.setBitmaps(BITMAP_JUMPATTACK_00_ID, BITMAP_JUMPATTACK_09_ID);
            }
            activeAnimation.startAnimation(false, false, false);
            activeAnimation.setUpdateTicksInterval(animationSpeed);
            activeAnimation.invalidate();

            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());

            setMoveAnimationDelay(0);
            startMoveAnimation(getX(), getY() - jumpHeight, animationDurationUp, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseOut);
        }
        else if (tickCounter == animationDurationUp)
        {
            setMoveAnimationDelay(0);
            startMoveAnimation(getX(), groundY, animationDurationDown, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseIn);
        }
        else if (tickCounter == (animationDurationUp + animationDurationDown) - 10)
        {
            if (currentAnimationState == JUMP)
            {
                activeAnimation.setBitmaps(BITMAP_JUMP_05_ID, BITMAP_JUMP_09_ID);
                activeAnimation.startAnimation(false, false, false);
                activeAnimation.setUpdateTicksInterval(animationSpeed);
                activeAnimation.invalidate();

                setWidth(activeAnimation.getWidth());
                setHeight(activeAnimation.getHeight());
            }
        }
        else if (tickCounter == (animationDurationUp + animationDurationDown) - 5)
        {
            activeAnimation.setBitmaps(BITMAP_RUN_00_ID, BITMAP_RUN_09_ID);
            activeAnimation.startAnimation(false, false, true);
            activeAnimation.setUpdateTicksInterval(3);
            activeAnimation.invalidate();

            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());
        }
        else if (tickCounter == (animationDurationUp + animationDurationDown))
        {
            setState(RUN);
        }
    }
    else if (currentAnimationState == DEAD)
    {
        if (tickCounter == 1)
        {
            activeAnimation.setBitmaps(BITMAP_DEAD_01_ID, BITMAP_DEAD_09_ID);

            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());

            moveRelative(-118, 6);

            if (getY() != groundY)
            {
                setMoveAnimationDelay(0);
                startMoveAnimation(getX(), groundY + 4, 20, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseIn);
            }

            activeAnimation.setFadeAnimationDelay(40);
            activeAnimation.startFadeAnimation(0, 20, EasingEquations::cubicEaseIn);

            activeAnimation.setUpdateTicksInterval(3);
            activeAnimation.startAnimation(false, false, false);
        }
    }
}

void Knight::setState(AnimationState newState)
{
    if (! (currentAnimationState == RUN || 
           newState == RUN || newState == DEAD ||
          (currentAnimationState == DEAD && newState == ENTERING_GAME) ||
          (currentAnimationState == JUMP && newState == JUMP_ATTACK) ) )
    {
        return;
    }

    if (newState == JUMP)
    {
        jumpHeight = 70;
        animationDurationUp = 26;
        animationDurationDown = 26;
        animationSpeed = 3;
        tickCounter = 0;
    }
    else if (newState == JUMP_ATTACK)
    {
        const int jumpAttackAnimationSpeed = 3;
        if (currentAnimationState == JUMP)
        {
            // Jump is already being performed - so just change animation
            activeAnimation.setBitmaps(BITMAP_JUMPATTACK_00_ID, BITMAP_JUMPATTACK_09_ID);
            activeAnimation.setUpdateTicksInterval(jumpAttackAnimationSpeed);
            activeAnimation.startAnimation(false, false, false);

            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());
        }
        else
        {
            jumpHeight = 20;
            animationDurationUp = 16;
            animationDurationDown = 16;
            animationSpeed = jumpAttackAnimationSpeed;
            tickCounter = 0;
        }
    }
    else
    {
        tickCounter = 0;
    }

    currentAnimationState = newState;
}

void Knight::setGroundPosition(int ground)
{
    groundY = ground - getHeight() + 10;
    setY(groundY);
}

Rect Knight::getAttackArea()
{
    return Rect(getRect().right() - 40, getY() + 80, 40, getHeight() - 100);
}

Rect Knight::getBodyArea()
{
    return Rect(getX() + 30, getY() + 20, 70, getHeight() - 30);
}

void Knight::reset()
{
    currentAnimationState = DEAD;
    moveTo(-300, getY());
    setState(ENTERING_GAME);
    invalidate();
}

