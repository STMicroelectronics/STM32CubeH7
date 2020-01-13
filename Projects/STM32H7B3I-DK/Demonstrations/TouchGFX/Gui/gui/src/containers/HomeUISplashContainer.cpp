#include <gui/containers/HomeUISplashContainer.hpp>

HomeUISplashContainer::HomeUISplashContainer()
{

}

void HomeUISplashContainer::initialize()
{
    HomeUISplashContainerBase::initialize();
}

void HomeUISplashContainer::startFadeAnimation(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    background.setFadeAnimationDelay(delay);
    textAreaTempUnit.setFadeAnimationDelay(delay);
    textAreaTemp.setFadeAnimationDelay(delay);
    textAreaBottomline.setFadeAnimationDelay(delay);
    textAreaHeadline.setFadeAnimationDelay(delay);
    textAreaWakeUp.setFadeAnimationDelay(delay);

    background.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    textAreaTempUnit.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    textAreaTemp.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    textAreaBottomline.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    textAreaHeadline.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    textAreaWakeUp.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

void HomeUISplashContainer::setHeadline(TypedText text)
{
    textAreaHeadline.setTypedText(text);
}

void HomeUISplashContainer::setTemp(uint16_t temp)
{
    Unicode::snprintf(textAreaTempBuffer, TEXTAREATEMP_SIZE, "%d", temp);
}

void HomeUISplashContainer::setTempActionText(TypedText text)
{
    textAreaBottomline.setTypedText(text);
}
