#include <gui/mainmenuanimatingbuttons_screen/MainMenuAnimatingButtonsView.hpp>
#include <gui/mainmenuanimatingbuttons_screen/MainMenuAnimatingButtonsPresenter.hpp>

MainMenuAnimatingButtonsPresenter::MainMenuAnimatingButtonsPresenter(MainMenuAnimatingButtonsView& v)
    : view(v)
{
}

void MainMenuAnimatingButtonsPresenter::activate()
{
}

void MainMenuAnimatingButtonsPresenter::deactivate()
{
}

void MainMenuAnimatingButtonsPresenter::setSelectedDemoScreen(Defines::DemoID elementIndex)
{
    model->setSelectedDemoScreen(elementIndex);
}

void MainMenuAnimatingButtonsPresenter::setPreviousSelectedMenuType(Defines::MainMenuType menuType)
{
    model->setPreviousSelectedMainMenuType(menuType);
}

Defines::DemoID MainMenuAnimatingButtonsPresenter::getSelectedDemoScreen()
{
    return model->getSelectedDemoScreen();
}

Defines::MainMenuType MainMenuAnimatingButtonsPresenter::getPreviousSelectedMenuType()
{
    return model->getPreviousSelectedMainMenuType();
}

void MainMenuAnimatingButtonsPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}

void MainMenuAnimatingButtonsPresenter::mcuLoadUpdated(uint8_t newMcuLoad)
{
    view.mcuLoadUpdated(newMcuLoad);
}
