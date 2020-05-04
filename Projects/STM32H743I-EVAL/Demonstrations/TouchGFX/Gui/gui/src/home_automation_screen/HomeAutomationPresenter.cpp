#include <gui/home_automation_screen/HomeAutomationPresenter.hpp>
#include <gui/home_automation_screen/HomeAutomationView.hpp>
#include <gui/common/Defines.hpp>

HomeAutomationPresenter::HomeAutomationPresenter(HomeAutomationView& v) : 
    DemoPresenter(v),
    view(v),
    counter(0),
    startSlide(false),
    home_state(LIGHT)
{
}

void HomeAutomationPresenter::activate()
{
    model->setCurrentScreen(Defines::HOME_AUTOMATION_SCREEN);
}

void HomeAutomationPresenter::deactivate()
{

}

void HomeAutomationPresenter::oneSecond()
{
    switch (home_state)
    {
    case HomeAutomationPresenter::LIGHT:
        counter++;
        switch (counter)
        {
        case 2:
            view.screenClick(ClickEvent::PRESSED, (134 * 640/800), 269);
            break;
        case 4:
            view.screenClick(ClickEvent::RELEASED, (134 * 640/800), 269);
            break;
        case 7:
            view.screenClick(ClickEvent::PRESSED, (691 * 640/800), 207);
            break;
        case 9:
            view.screenClick(ClickEvent::RELEASED, (691 * 640/800), 207);
            break;
        case 11:
            view.screenClick(ClickEvent::PRESSED, (278 * 640/800), 414);
            break;
        case 13:
            view.screenClick(ClickEvent::RELEASED, (278 * 640/800), 414);
            break;
        case 15:
            view.screenClick(ClickEvent::PRESSED, (581 * 640/800), 315);
            break;
        case 17:
            view.screenClick(ClickEvent::RELEASED, (581 * 640/800), 315);
            break;
        case 19:
            view.screenClick(ClickEvent::PRESSED, (691 * 640/800), 207);
            break;
        case 21:
            view.screenClick(ClickEvent::RELEASED, (691 * 640/800), 207);
            break;
        case 25:
            view.screenClick(ClickEvent::PRESSED, (42 * 640/800), 42);
            break;
        case 27:
            view.screenClick(ClickEvent::RELEASED, (42 * 640/800), 42);
            home_state = BLINDS;
            counter = 0;
            break;
        }
        break;
    case HomeAutomationPresenter::BLINDS:
        counter++;
        switch (counter)
        {
        case 2:
            view.screenClick(ClickEvent::PRESSED, (302 * 640/800), 269);
            break;
        case 4:
            view.screenClick(ClickEvent::RELEASED, (302 * 640/800), 269);
            break;
        case 8:
            view.screenClick(ClickEvent::PRESSED, (196 * 640/800), 266);
            break;
        case 10:
            view.screenClick(ClickEvent::RELEASED, (196 * 640/800), 266);
            break;
        case 12:
            x = 72;
            startSlide = true;
            break;
        case 20:
            startSlide = false;
            break;
        case 28:
            view.screenClick(ClickEvent::PRESSED, (408 * 640/800), 126);
            view.screenClick(ClickEvent::RELEASED, (408 * 640/800), 126);
            break;
        case 31:
            view.screenClick(ClickEvent::PRESSED, (86 * 640/800), 160);
            break;
        case 33:
            view.screenClick(ClickEvent::RELEASED, (86 * 640/800), 160);
            break;
        case 35:
            view.screenCircleUpdate(180);
            break;
        case 37:
            view.screenClick(ClickEvent::PRESSED, (42 * 640/800), 42);
            break;
        case 39:
            view.screenClick(ClickEvent::RELEASED, (42 * 640/800), 42);
            home_state = SECURITY;
            counter = 0;
            break;
        }
        break;
    case HomeAutomationPresenter::SECURITY:
        counter++;
        switch (counter)
        {
        case 1:
            view.screenClick(ClickEvent::PRESSED, (490 * 640/800), 269);
            break;
        case 3:
            view.screenClick(ClickEvent::RELEASED, (490 * 640/800), 269);
            break;
        case 5:
            view.screenClick(ClickEvent::PRESSED, (506 * 640/800), 175);
            break;
        case 6:
            view.screenDrag((506 * 640/800), 175, (506 * 640/800), 284);
            break;
        case 7:
            view.screenDrag((506 * 640/800), 284, (506 * 640/800), 384);
            break;
        case 8:
            view.screenDrag((506 * 640/800), 384, (600 * 640/800), 384);
            break;
        case 9:
            view.screenClick(ClickEvent::RELEASED, (600 * 640/800), 384);
            break;
        case 15:
            view.screenClick(ClickEvent::PRESSED, (704 * 640/800), 200);
            break;
        case 17:
            view.screenClick(ClickEvent::RELEASED, (704 * 640/800), 200);
            break;
        case 19:
            view.screenClick(ClickEvent::PRESSED, (289 * 640/800), 151);
            break;
        case 21:
            view.screenClick(ClickEvent::RELEASED, (289 * 640/800), 151);
            break;
        case 25:
            view.screenClick(ClickEvent::PRESSED, (42 * 640/800), 42);
            break;
        case 27:
            view.screenClick(ClickEvent::RELEASED, (42 * 640/800), 42);
            home_state = STATISTICS;
            counter = 0;
            break;
        }
        break;
    case HomeAutomationPresenter::STATISTICS:
        counter++;
        switch (counter)
        {
        case 2:
            view.screenClick(ClickEvent::PRESSED, (666 * 640/800), 269);
            break;
        case 4:
            view.screenClick(ClickEvent::RELEASED, (666 * 640/800), 269);
            break;
        case 6:
            view.screenClick(ClickEvent::PRESSED, (621 * 640/800), 175);
            break;
        case 7:
            view.screenDrag((621 * 640/800), 175, (715 * 640/800), 284);
            break;
        case 8:
            view.screenDrag((715 * 640/800), 284, (715 * 640/800), 384);
            break;
        case 9:
            view.screenDrag((715 * 640/800), 384, (600 * 640/800), 384);
            break;
        case 10:
            view.screenClick(ClickEvent::RELEASED, (600 * 640/800), 384);
            break;
        case 20:
            view.screenClick(ClickEvent::PRESSED, (506 * 640/800), 175);
            break;
        case 21:
            view.screenDrag((506 * 640/800), 175, (506 * 640/800), 284);
            break;
        case 22:
            view.screenDrag((506 * 640/800), 284, (506 * 640/800), 384);
            break;
        case 23:
            view.screenDrag((506 * 640/800), 384, (600 * 640/800), 384);
            break;
        case 24:
            view.screenClick(ClickEvent::RELEASED, (600 * 640/800), 384);
            break;
        case 38:
            view.screenClick(ClickEvent::PRESSED, (201 * 640/800), 138);
            break;
        case 39:
            view.screenClick(ClickEvent::RELEASED, (201 * 640/800), 138);
            break;
        case 43:
            view.screenClick(ClickEvent::PRESSED, (680 * 640/800), 200);
            break;
        case 44:
            view.screenClick(ClickEvent::RELEASED, (680 * 640/800), 200);
            break;
        case 49:
            view.screenClick(ClickEvent::PRESSED, (42 * 640/800), 42);
            break;
        case 51:
            view.screenClick(ClickEvent::RELEASED, (42 * 640/800), 42);
            break;
        case 53:
            view.screenClick(ClickEvent::PRESSED, (42 * 640/800), 42);
            break;
        case 56:
            view.screenClick(ClickEvent::RELEASED, (42 * 640/800), 42);
            break;
        }
        break;
    default:
        break;
    }
}

void HomeAutomationPresenter::presenterTick()
{
    if (startSlide)
    {
        view.screenCircleUpdate(x);
        x++;
    }
}

void HomeAutomationPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
