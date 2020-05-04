#include <gui/settings_screen/SettingsView.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>

SettingsPresenter::SettingsPresenter(SettingsView& v)
    : view(v)
{
}

void SettingsPresenter::activate()
{
    view.setSelectedClock(model->getSelectedClock());
    view.updateTime(model->getCurrentTime().hours, model->getCurrentTime().minutes, model->getCurrentTime().seconds, false);
    view.updateDate(model->getDateYear(), model->getDateMonth(), model->getDateDay());
}

void SettingsPresenter::deactivate()
{
}
void SettingsPresenter::clockSelected(int clockIndex)
{
    model->setSelectedClock(clockIndex);
}

void SettingsPresenter::updateTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    view.updateTime(hour, minute, second, true);
}

void SettingsPresenter::stateUpdated(int clockIndex, uint8_t hour, uint8_t minute, uint8_t second, uint16_t year, uint8_t month, uint8_t day)
{
    model->setSelectedClock(clockIndex);
    model->setTime(hour, minute, second);
    model->setDate(year, month, day);
    view.updateDate(model->getDateYear(), model->getDateMonth(), model->getDateDay());
}

void SettingsPresenter::gotoMenuSelected()
{
    if (model->getPreviousSelectedMainMenuType() == Defines::CAROUSEL_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreenNoTransition();
    }
    else if (model->getPreviousSelectedMainMenuType() == Defines::ANIMATING_BUTTONS_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuAnimatingButtonsScreenNoTransition();
    }
}

void SettingsPresenter::mcuLoadUpdated(uint8_t newMcuLoad)
{
    view.mcuLoadUpdated(newMcuLoad);
}

void SettingsPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
