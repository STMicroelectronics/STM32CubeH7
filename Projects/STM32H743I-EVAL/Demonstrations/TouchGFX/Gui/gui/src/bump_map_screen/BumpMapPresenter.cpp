#include <gui/bump_map_screen/BumpMapPresenter.hpp>
#include <gui/bump_map_screen/BumpMapView.hpp>

BumpMapPresenter::BumpMapPresenter(BumpMapView& v) : 
    DemoPresenter(v), 
    view(v),
    counter(0)
{
}

void BumpMapPresenter::activate()
{

}

void BumpMapPresenter::deactivate()
{

}

void BumpMapPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 10:
        view.screenClick(ClickEvent::PRESSED, 608, 445);
        break;
    case 11:
        view.screenClick(ClickEvent::RELEASED, 608, 445);
        break;
    case 40:
        view.screenClick(ClickEvent::PRESSED, 608, 445);
        break;
    case 41:
        view.screenClick(ClickEvent::RELEASED, 608, 445);
        break;
    case 60:
        view.screenClick(ClickEvent::PRESSED, 608, 445);
        break;
    case 61:
        view.screenClick(ClickEvent::RELEASED, 608, 445);
        break;
    case 80:
        view.screenClick(ClickEvent::PRESSED, 40, 45);
        break;
    case 81:
        view.screenClick(ClickEvent::RELEASED, 40, 45);
        break;
    }
}

void BumpMapPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
