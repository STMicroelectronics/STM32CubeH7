#include <gui/bathtubscreen_screen/BathtubScreenView.hpp>
#include <gui/bathtubscreen_screen/BathtubScreenPresenter.hpp>

BathtubScreenPresenter::BathtubScreenPresenter(BathtubScreenView& v) :
    view(v)
{
  
}

void BathtubScreenPresenter::activate()
{
    model->setCurrentView(Model::Bathtub);
    model->setBathtubUIEntered(true);
}

void BathtubScreenPresenter::deactivate()
{

}

void BathtubScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void BathtubScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}

void BathtubScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates BathtubScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}


void BathtubScreenPresenter::setBathtubOn(bool on)
{
    model->setBathtubOn(on);
}

bool BathtubScreenPresenter::getBathtubOn()
{
    return model->getBathtubOn();
}

void BathtubScreenPresenter::setTempCelcius(bool temp)
{
    model->setTempCelcius(temp);
}

bool BathtubScreenPresenter::getTempCelcius()
{
    return model->getTempCelcius();
}

void BathtubScreenPresenter::setJetOn(bool on)
{
    model->setJetOn(on);
}

bool BathtubScreenPresenter::getJetOn()
{
    return model->getJetOn();
}

void BathtubScreenPresenter::setLightOn(bool on)
{
    model->setLightOn(on);
}

bool BathtubScreenPresenter::getLightOn()
{
    return model->getLightOn();
}

void BathtubScreenPresenter::setTempValue(uint16_t temp)
{
    model->setTempValue(temp);
}

uint16_t BathtubScreenPresenter::getTempValue()
{
    return model->getTempValue();
}

void BathtubScreenPresenter::setJetLevel(uint16_t level)
{
    model->setJetLevel(level);
}

uint16_t BathtubScreenPresenter::getJetLevel()
{
    return model->getJetLevel();
}

void BathtubScreenPresenter::setColorCoordinates(uint32_t x, uint32_t y)
{
    model->setColorCoordinates(x, y);
}

void BathtubScreenPresenter::getColorCoordinates(uint32_t& x, uint32_t& y)
{
    model->getColorCoordinates(x, y);
}

void BathtubScreenPresenter::mcuLoadUpdated(uint8_t load)
{
    view.updateMCULoad(load);
}
