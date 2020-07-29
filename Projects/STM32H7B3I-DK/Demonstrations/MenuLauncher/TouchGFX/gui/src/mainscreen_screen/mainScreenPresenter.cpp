#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>

mainScreenPresenter::mainScreenPresenter(mainScreenView& v)
    : view(v)
{
}

void mainScreenPresenter::activate()
{
  model->setFirmwareVersion("V1.0.1"); 
}

void mainScreenPresenter::deactivate()
{

}

void mainScreenPresenter::moduleSelected(int moduleIndex)
{
    model->moduleSelected(moduleIndex);
}

void mainScreenPresenter::setFirmwareVersion(const char* fwVersion)
{
    view.setFirmwareVersion(fwVersion);
}
