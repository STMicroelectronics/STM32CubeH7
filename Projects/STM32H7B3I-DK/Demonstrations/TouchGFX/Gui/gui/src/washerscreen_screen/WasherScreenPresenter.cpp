#include <gui/washerscreen_screen/WasherScreenView.hpp>
#include <gui/washerscreen_screen/WasherScreenPresenter.hpp>

WasherScreenPresenter::WasherScreenPresenter(WasherScreenView& v) : 
    view(v)
{
}

void WasherScreenPresenter::activate()
{
    model->setCurrentView(Model::Washer);
    model->setWasherUIEntered(true);
    view.setSelectedProgram(model->getWashingProgram());
}

void WasherScreenPresenter::deactivate()
{
    model->setWashingProgram(view.getSelectedProgram());
    model->setWashingTime(view.getWashingTime());
}

void WasherScreenPresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
    view.mcuLoadUpdated(mcuLoad);
}

void WasherScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void WasherScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates WasherScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void WasherScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
