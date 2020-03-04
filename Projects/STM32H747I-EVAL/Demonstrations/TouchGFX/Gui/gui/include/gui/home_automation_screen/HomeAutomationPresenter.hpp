

#ifndef HOME_AUTOMATION_PRESENTER_HPP
#define HOME_AUTOMATION_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class HomeAutomationView;

class HomeAutomationPresenter : public DemoPresenter
{
public:
    HomeAutomationPresenter(HomeAutomationView& v);

    virtual void activate();
    virtual void deactivate();

    virtual ~HomeAutomationPresenter() {};

    virtual void presenterTick();

    virtual void oneSecond();
    virtual void screenSaverMinorTick();

private:
    HomeAutomationPresenter();

    HomeAutomationView& view;

    int16_t counter;

    bool startSlide;
    int16_t x;
    int16_t y;

    enum States
    {
        LIGHT,
        BLINDS,
        SECURITY,
        STATISTICS
    } home_state;
};

#endif // HOME_AUTOMATION_PRESENTER_HPP
