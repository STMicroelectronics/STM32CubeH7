#include <gui/main_screen/mainView.hpp>
#include <gui/main_screen/mainPresenter.hpp>

mainPresenter::mainPresenter(mainView& v)
    : view(v)
{
}

void mainPresenter::activate()
{
    HAL::getInstance()->setFrameRateCompensation(true);
    model->setFirmwareVersion("V1.0.1");
}

void mainPresenter::deactivate()
{
    HAL::getInstance()->setFrameRateCompensation(false);
}

void mainPresenter::moduleSelected(int moduleIndex)
{
    model->moduleSelected(moduleIndex);
}

void mainPresenter::setFirmwareVersion(const char* fwVersion)
{
    view.setFirmwareVersion(fwVersion);
}
