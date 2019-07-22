#include <gui/aboutscreen_screen/AboutScreenView.hpp>
#include <gui/aboutscreen_screen/AboutScreenPresenter.hpp>

#include <widgets/FileInput.hpp>
#include <string.h>

#ifdef _MSC_VER
#define strncpy strncpy_s
#endif

AboutScreenPresenter::AboutScreenPresenter(AboutScreenView& v) :
    view(v)
{
  
}

void AboutScreenPresenter::activate()
{
    model->setCurrentView(Model::About);
    HAL::getInstance()->setFrameRateCompensation(true);
}

void AboutScreenPresenter::deactivate()
{
    HAL::getInstance()->setFrameRateCompensation(false);
}

void AboutScreenPresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
	view.mcuLoadUpdated(mcuLoad);
}

void AboutScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void AboutScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates AboutScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void AboutScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
