#ifndef HOMEUISPLASHCONTAINER_HPP
#define HOMEUISPLASHCONTAINER_HPP

#include <gui_generated/containers/HomeUISplashContainerBase.hpp>

class HomeUISplashContainer : public HomeUISplashContainerBase
{
public:
    HomeUISplashContainer();
    virtual ~HomeUISplashContainer() {}

    virtual void initialize();

    void startFadeAnimation(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

    void setHeadline(TypedText text);
    void setTemp(uint16_t temp);
    void setTempActionText(TypedText text);

protected:

};

#endif // HOMEUISPLASHCONTAINER_HPP
