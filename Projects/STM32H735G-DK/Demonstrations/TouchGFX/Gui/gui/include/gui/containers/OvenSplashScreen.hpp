#ifndef OVENSPLASHSCREEN_HPP
#define OVENSPLASHSCREEN_HPP

#include <gui_generated/containers/OvenSplashScreenBase.hpp>


class OvenSplashScreen : public OvenSplashScreenBase
{
public:
    OvenSplashScreen();
    virtual ~OvenSplashScreen() {}

    virtual void initialize();

    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

    virtual void screenIsPressed();

    void setScreenPressedAction(GenericCallback<>& callback);

protected:
    GenericCallback<>* screenPressed;
};

#endif // OVENSPLASHSCREEN_HPP
