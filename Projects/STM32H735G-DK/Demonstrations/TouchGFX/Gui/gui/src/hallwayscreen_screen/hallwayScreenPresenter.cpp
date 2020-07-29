#include <gui/hallwayscreen_screen/hallwayScreenView.hpp>
#include <gui/hallwayscreen_screen/hallwayScreenPresenter.hpp>

HallwayScreenPresenter::HallwayScreenPresenter(HallwayScreenView& v) :
    view(v)
{
}

void HallwayScreenPresenter::activate()
{
    model->setCurrentView(Model::Hallway);
}

void HallwayScreenPresenter::deactivate()
{

}

void HallwayScreenPresenter::setShowIntro(bool show)
{
    model->setShowIntro(show);
}

void HallwayScreenPresenter::mcuLoadUpdated(uint8_t load)
{
    view.mcuLoadUpdated(load);
}

void HallwayScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void HallwayScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates HallwayScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void HallwayScreenPresenter::resetAutoDemo()
{
    model->resetAutoDemo();
}

void HallwayScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
