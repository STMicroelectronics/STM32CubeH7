#include <gui/gameKnight_screen/Head.hpp>
#include <BitmapDatabase.hpp>

Head::Head() :
    groundY(0)
{
    Application::getInstance()->registerTimerWidget(this);

    add(activeAnimation);

    setState(MOVING);
}

Head::~Head()
{

}

void Head::handleTickEvent()
{
    tickCounter++;

    if (currentAnimationState != STOPPED)
    {
        moveRelative(-4, 0);
    }

    if (currentAnimationState == MOVING)
    {
        if (tickCounter == 1)
        {
            activeAnimation.setBitmaps(BITMAP_HEAD_00_ID, BITMAP_HEAD_09_ID);
            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());

            activeAnimation.setXY(0, 0);
            activeAnimation.setUpdateTicksInterval(3);

            activeAnimation.startAnimation(false, false, true);
        }
    }
    else if (currentAnimationState == DEAD)
    {
        if (tickCounter == 1)
        {
            activeAnimation.startFadeAnimation(0, 14, EasingEquations::cubicEaseOut);
        }
    }

}

void Head::setState(AnimationState state)
{
    currentAnimationState = state;
    tickCounter = 0;    
}

void Head::reset()
{
    moveTo(800, getY());
    activeAnimation.setAlpha(255);
    setState(STOPPED);
}

Rect Head::getAttackArea()
{
    return Rect(getX() + 50, getY(), 20, getHeight());
}

Rect Head::getBodyArea()
{
    return Rect(getX() + 30, getY(), 60, getHeight());
}
