#include <gui/gameKnight_screen/Zombie.hpp>
#include <BitmapDatabase.hpp>

Zombie::Zombie() :
    groundY(0)
{
    Application::getInstance()->registerTimerWidget(this);

    activeAnimation.setBitmaps(BITMAP_ZOMBIE_WALK_00_ID, BITMAP_ZOMBIE_WALK_09_ID);
    add(activeAnimation);

    setWidth(activeAnimation.getWidth());
    setHeight(activeAnimation.getHeight());

    setState(RUN);
}

Zombie::~Zombie()
{

}

void Zombie::handleTickEvent()
{
    tickCounter++;

    if (currentAnimationState != STOPPED)
    {
        moveRelative(-2, 0);
    }

    if (currentAnimationState == RUN)
    {
        if (tickCounter == 1)
        {
            activeAnimation.setBitmaps(BITMAP_ZOMBIE_WALK_00_ID, BITMAP_ZOMBIE_WALK_09_ID);
            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());

            activeAnimation.setXY(0, 0);
            activeAnimation.setUpdateTicksInterval(3);

            activeAnimation.startAnimation(false, false, true);
        }

    }
    else if (currentAnimationState == ATTACK)
    {
        int attackAnimationTickInterval = 3;
        int attackAnimationDuration = attackAnimationTickInterval * 8; // frames in animation
        
        if (tickCounter == 1)
        {
            activeAnimation.setBitmaps(BITMAP_ZOMBIE_ATTACK_00_ID, BITMAP_ZOMBIE_ATTACK_07_ID);
            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());

            activeAnimation.setUpdateTicksInterval(attackAnimationTickInterval);
            activeAnimation.startAnimation(false, false, false);
        }
        else if (tickCounter == attackAnimationDuration)
        {
            setState(RUN);
        }
    }
    else if (currentAnimationState == DEAD)
    {
        if (tickCounter == 1)
        {
            activeAnimation.setBitmaps(BITMAP_ZOMBIE_DEAD_00_ID, BITMAP_ZOMBIE_DEAD_07_ID);
            setWidth(activeAnimation.getWidth());
            setHeight(activeAnimation.getHeight());

            moveRelative(-80, 16);

            activeAnimation.setUpdateTicksInterval(7);
            activeAnimation.startAnimation(false, false, false);
        }
    }
}

void Zombie::setState(AnimationState state)
{
    currentAnimationState = state;
    tickCounter = 0;
}

void Zombie::setGroundPosition(int ground)
{
    groundY = ground - getHeight() + 8;
    setY(groundY);
}

void Zombie::reset()
{
    moveTo(800, groundY);
    setState(STOPPED);
}

Rect Zombie::getAttackArea()
{
    return Rect(getX() + 20, getY() + 40, 50, getHeight() - 60);
}

Rect Zombie::getBodyArea()
{
    return Rect(getX() + 40, getY() + 16, 70, getHeight() - 20);
}
