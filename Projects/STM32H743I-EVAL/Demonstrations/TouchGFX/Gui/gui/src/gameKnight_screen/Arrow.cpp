#include <gui/gameKnight_screen/Arrow.hpp>
#include <BitmapDatabase.hpp>

Arrow::Arrow() :
    groundY(0)
{
    Application::getInstance()->registerTimerWidget(this);

    activeAnimation.setBitmap(Bitmap(BITMAP_ARROW_IMAGE_ID));
    activeAnimation.setXY(0, 0);
    add(activeAnimation);

    setWidth(activeAnimation.getWidth());
    setHeight(activeAnimation.getHeight());

    setState(MOVING);
}

void Arrow::handleTickEvent()
{
    tickCounter++;

    if (currentAnimationState != STOPPED &&
        currentAnimationState != HIT_TARGET)
    {
        moveRelative(-7, 0);
    }

    if (currentAnimationState == HIT_TARGET)
    {
        int fadeDuration = 30;

        if (tickCounter == 1)
        {
            activeAnimation.startFadeAnimation(0, 30, EasingEquations::cubicEaseIn);
        }
        else if (tickCounter == fadeDuration + 5)
        {
            reset();
        }
    }
}

void Arrow::setState(AnimationState state)
{
    currentAnimationState = state;
    tickCounter = 0;    
}

void Arrow::reset()
{
    moveTo(800, getY());
    activeAnimation.setAlpha(255);
    setState(STOPPED);
}

Rect Arrow::getAttackArea()
{
    return getRect();
}

Rect Arrow::getBodyArea()
{
    return getRect();
}
