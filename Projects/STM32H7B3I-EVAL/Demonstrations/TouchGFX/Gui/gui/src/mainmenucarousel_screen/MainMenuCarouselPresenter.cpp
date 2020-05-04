#include <gui/mainmenucarousel_screen/MainMenuCarouselView.hpp>
#include <gui/mainmenucarousel_screen/MainMenuCarouselPresenter.hpp>

MainMenuCarouselPresenter::MainMenuCarouselPresenter(MainMenuCarouselView& v)
    : view(v)
{
}

void MainMenuCarouselPresenter::activate()
{
}

void MainMenuCarouselPresenter::deactivate()
{
}

void MainMenuCarouselPresenter::setSelectedDemoScreen(Defines::DemoID elementIndex)
{
    model->setSelectedDemoScreen(elementIndex);
}

void MainMenuCarouselPresenter::setPreviousSelectedMenuType(Defines::MainMenuType menuType)
{
    model->setPreviousSelectedMainMenuType(menuType);
}

Defines::DemoID MainMenuCarouselPresenter::getSelectedDemoScreen()
{
    return model->getSelectedDemoScreen();
}

Defines::MainMenuType MainMenuCarouselPresenter::getPreviousSelectedMenuType()
{
    return model->getPreviousSelectedMainMenuType();
}

void MainMenuCarouselPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}

void MainMenuCarouselPresenter::mcuLoadUpdated(uint8_t newMcuLoad)
{
    view.mcuLoadUpdated(newMcuLoad);
}
