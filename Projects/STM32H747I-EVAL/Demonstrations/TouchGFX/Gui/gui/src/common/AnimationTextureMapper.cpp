

#include <gui/common/AnimationTextureMapper.hpp>


namespace touchgfx
{

AnimationTextureMapper::AnimationTextureMapper() :
    textureMapperAnimationStepCallback(0),
    textureMapperAnimationEndedCallback(0)
{
    animationRunning = false;

    for (int i = 0; i < NUMBER_OF_ANIMATION_PARAMETERS; i++)
    {
        animations[i].animationActive = false;
    }
}

void AnimationTextureMapper::setupAnimation(AnimationParameter parameter, float endValue, uint16_t duration, uint16_t delay, EasingEquation progressionEquation /*= &EasingEquations::linearEaseNone*/)
{
    animations[parameter].animationActive = true;
    animations[parameter].animationEnd = endValue;
    animations[parameter].animationDuration = duration;
    animations[parameter].animationDelay = delay;
    animations[parameter].animationProgressionEquation = progressionEquation;
}

void AnimationTextureMapper::startAnimation()
{
    Application::getInstance()->registerTimerWidget(this);

    animationCounter = 0;

    animations[X_ROTATION].animationStart = xAngle;
    animations[Y_ROTATION].animationStart = yAngle;
    animations[Z_ROTATION].animationStart = zAngle;
    animations[SCALE].animationStart = scale;

    animationRunning = true;
}

void AnimationTextureMapper::cancelMoveAnimation()
{
    Application::getInstance()->unregisterTimerWidget(this);
    animationRunning = false;

    for (int i = 0; i < NUMBER_OF_ANIMATION_PARAMETERS; i++)
    {
        animations[i].animationActive = false;
    }
}


void AnimationTextureMapper::handleTickEvent()
{
    if (animationRunning)
    {
        bool activeAnimation = false;

        float newXAngle = xAngle;
        float newYAngle = yAngle;
        float newZAngle = zAngle;
        float newScale = scale;

        for (int i = 0; i < NUMBER_OF_ANIMATION_PARAMETERS; i++)
        {
            if (!(animations[i].animationActive))
            {
                continue;
            }

            if ((animationCounter >= animations[i].animationDelay) && 
                (animationCounter <= (uint32_t)(animations[i].animationDelay + animations[i].animationDuration)))
            {
                activeAnimation = true;

                // Adjust the used animationCounter for the startup delay
                uint32_t actualAnimationCounter = animationCounter - animations[i].animationDelay;

                int directionModifier;
                int16_t distance;

                if (animations[i].animationEnd > animations[i].animationStart)
                {
                    directionModifier = 1;
                    distance = (int16_t) ((animations[i].animationEnd - animations[i].animationStart) * 1000);
                }
                else
                {
                    directionModifier = -1;
                    distance = (int16_t) ((animations[i].animationStart - animations[i].animationEnd) * 1000);
                }

                float delta = directionModifier * animations[i].animationProgressionEquation(actualAnimationCounter, 0, distance, animations[i].animationDuration) / 1000.f;

                switch ((AnimationParameter) i)
                {
                case X_ROTATION:
                    newXAngle = animations[X_ROTATION].animationStart + delta;
                    break;
                case Y_ROTATION:
                    newYAngle = animations[Y_ROTATION].animationStart + delta;
                    break;
                case Z_ROTATION:
                    newZAngle = animations[Z_ROTATION].animationStart + delta;
                    break;
                case SCALE:
                    newScale = animations[SCALE].animationStart + delta;
                    break;
                default:
                    break;
                }
            }
        }

        if (activeAnimation)
        {
            updateAngles(newXAngle, newYAngle, newZAngle);
            setScale(newScale);

            if (textureMapperAnimationStepCallback && textureMapperAnimationStepCallback->isValid())
            {
                textureMapperAnimationStepCallback->execute(*this);
            }

            animationCounter++;
        }
        else 
        {
            // End of animation
            cancelMoveAnimation();

            if (textureMapperAnimationEndedCallback && textureMapperAnimationEndedCallback->isValid())
            {
                textureMapperAnimationEndedCallback->execute(*this);
            }
        }
    }
}

}
