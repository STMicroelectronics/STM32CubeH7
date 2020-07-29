#include <gui/homecontrolscreen_screen/HomeControlScreenView.hpp>
#include <gui/homecontrolscreen_screen/HomeControlScreenPresenter.hpp>

HomeControlScreenPresenter::HomeControlScreenPresenter(HomeControlScreenView& v) : 
    view(v)
{
}

void HomeControlScreenPresenter::activate()
{
    model->setCurrentView(Model::HomeUI);
    view.setHomeUIRoom(model->getBathroomHomeUI());
    view.setSplashScreenState(model->getFromRecipeScreen() || (model->getBathroomUIEntered() && model->getBathroomHomeUI()) || (model->getKitchenUIEntered() && !model->getBathroomHomeUI()));
}

void HomeControlScreenPresenter::deactivate()
{
    if (getBathroomHomeUI())
    {
        model->setBathroomUIEntered(true);
    }
    else
    {
        model->setKitchenUIEntered(true);
    }
}

bool HomeControlScreenPresenter::getLightState()
{
    return model->getShowLight();
}

bool HomeControlScreenPresenter::getRecipeState()
{
    return model->getShowPot();
}

bool HomeControlScreenPresenter::getOvenState()
{
    return model->getShowOven();
}

bool HomeControlScreenPresenter::getFanState()
{
    return model->getFanState();
}

uint16_t HomeControlScreenPresenter::getFanSpeed()
{
    return model->getFanSpeed();
}

bool HomeControlScreenPresenter::getBathtubOn()
{
    return model->getBathtubOn();
}

bool HomeControlScreenPresenter::getTempCelcius()
{
    return model->getTempCelcius();
}

bool HomeControlScreenPresenter::getJetOn()
{
    return model->getJetOn();
}

bool HomeControlScreenPresenter::getLightOn()
{
    return model->getLightOn();
}

uint16_t HomeControlScreenPresenter::getTempValue()
{
    return model->getTempValue();
}

uint16_t HomeControlScreenPresenter::getJetLevel()
{
    return model->getJetLevel();
}

uint16_t HomeControlScreenPresenter::getSelectedOvenMode()
{
    return model->getSelectedOvenMode();
}

int16_t HomeControlScreenPresenter::getOvenTimerHour()
{
    return model->getOvenTimerHour();
}
int16_t HomeControlScreenPresenter::getOvenTimerMinute()
{
    return model->getOvenTimerMinute();
}

uint16_t HomeControlScreenPresenter::getOvenTemperature()
{
    return model->getOvenTemperature();
}
uint16_t HomeControlScreenPresenter::getWashingProgram()
{
    return model->getWashingProgram();
}

uint16_t HomeControlScreenPresenter::getWashingTime()
{
    return model->getWashingTime();
}

bool HomeControlScreenPresenter::getBathroomHomeUI()
{
    return model->getBathroomHomeUI();
}

bool HomeControlScreenPresenter::getFromRecipeScreen()
{
    return model->getFromRecipeScreen();
}

bool HomeControlScreenPresenter::getKitchenUIEntered()
{
    return model->getKitchenUIEntered();
}

bool HomeControlScreenPresenter::getBathroomUIEntered()
{
    return model->getBathroomUIEntered();
}

void HomeControlScreenPresenter::setFromRecipeScreen(bool recipeScreen)
{
    model->setFromRecipeScreen(recipeScreen);
}

void HomeControlScreenPresenter::mcuLoadUpdated(uint8_t load)
{
    view.updateMCULoad(load);
}

void HomeControlScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void HomeControlScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates HomeControlScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void HomeControlScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
