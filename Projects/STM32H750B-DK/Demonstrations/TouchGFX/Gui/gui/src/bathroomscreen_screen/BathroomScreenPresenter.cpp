#include <gui/bathroomscreen_screen/BathroomScreenView.hpp>
#include <gui/bathroomscreen_screen/BathroomScreenPresenter.hpp>

BathroomScreenPresenter::BathroomScreenPresenter(BathroomScreenView& v) :
    view(v)
{
}

void BathroomScreenPresenter::activate()
{    
    model->setCurrentView(Model::Bathroom);
    model->setBathroomHomeUI(true);

    view.setBathtubWaterState(model->getBathtubOn());
    view.setBathtubUIState(model->getBathtubUIEntered());
    view.setWasherState(model->getWasherUIEntered());
    view.setHomeUIState(model->getBathroomUIEntered());
}

void BathroomScreenPresenter::deactivate()
{

}

void BathroomScreenPresenter::mcuLoadUpdated(uint8_t load)
{
    view.updateMCULoad(load);
}

void BathroomScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void BathroomScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates BathroomScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void BathroomScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
