#include <gui/ovenscreen_screen/OvenScreenView.hpp>
#include <gui/ovenscreen_screen/OvenScreenPresenter.hpp>

OvenScreenPresenter::OvenScreenPresenter(OvenScreenView& v) : 
    view(v)
{
}

void OvenScreenPresenter::activate()
{
    model->setCurrentView(Model::Oven);
    model->setOvenEntered(true);
}

void OvenScreenPresenter::deactivate()
{

}

void OvenScreenPresenter::setShowOven(bool show)
{
    model->setShowOven(show);
}

bool OvenScreenPresenter::getShowOven()
{
    return model->getShowOven();
}

bool OvenScreenPresenter::getOvenEntered()
{
    return model->getOvenEntered();
}

void OvenScreenPresenter::setSelectedOvenMode(uint16_t mode)
{
    model->setSelectedOvenMode(mode);
}

uint16_t OvenScreenPresenter::getSelectedOvenMode()
{
    return model->getSelectedOvenMode();
}

void OvenScreenPresenter::setOvenTimerHour(int16_t hour)
{
    model->setOvenTimerHour(hour);
}

int16_t OvenScreenPresenter::getOvenTimerHour()
{
    return model->getOvenTimerHour();
}

void OvenScreenPresenter::setOvenTimerMinute(int16_t minute)
{
    model->setOvenTimerMinute(minute);
}

int16_t OvenScreenPresenter::getOvenTimerMinute()
{
    return model->getOvenTimerMinute();
}

void OvenScreenPresenter::setOvenTemperature(uint16_t temp)
{
    model->setOvenTemperature(temp);
}

uint16_t OvenScreenPresenter::getOvenTemperature()
{
    return model->getOvenTemperature();
}

void OvenScreenPresenter::mcuLoadUpdated(uint8_t load)
{
    view.setMcuLoad(load);
}

void OvenScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void OvenScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates OvenScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void OvenScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
