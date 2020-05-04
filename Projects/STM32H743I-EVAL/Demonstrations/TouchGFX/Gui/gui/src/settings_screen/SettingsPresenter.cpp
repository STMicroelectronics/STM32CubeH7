#include <gui/settings_screen/SettingsPresenter.hpp>
#include <gui/settings_screen/SettingsView.hpp>
#include <gui/common/Defines.hpp>

SettingsPresenter::SettingsPresenter(SettingsView& v) : 
    DemoPresenter(v),
    view(v),
    counter(0)
{
}

void SettingsPresenter::activate()
{
    if (model->getCounterActive())
    {
        view.setSelectedClock(0);
        view.updateTime(15, 2, 40, false);
        view.updateDate(0, 1, 23);
    }
    else
    {
        view.setSelectedClock(model->getSelectedClock());
        view.updateTime(model->getCurrentTime().hours, model->getCurrentTime().minutes, model->getCurrentTime().seconds, false);
        view.updateDate(model->getDateYear(), model->getDateMonth(), model->getDateDay());
    }
    model->setCurrentScreen(Defines::SETTINGS_SCREEN);
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
    model->setDate(year, month, day + 1);
    view.updateDate(model->getDateYear(), model->getDateMonth(), model->getDateDay());
}

void SettingsPresenter::oneSecond()
{
    counter++;
    switch (counter)
    {
    case 5:
        view.screenClick(ClickEvent::PRESSED, (420 * 640 / 800), 420);
        break;
    case 6:
        view.screenClick(ClickEvent::RELEASED, (420 * 640 / 800), 420);
        break;
    case 13:
        view.screenClick(ClickEvent::PRESSED, (650 * 640 / 800), 250);
        break;
    case 14:
        view.screenClick(ClickEvent::RELEASED, (650 * 640 / 800), 250);
        break;
    case 16:
        view.screenClick(ClickEvent::PRESSED, (420 * 640 / 800), 230);
        break;
    case 17:
        view.screenClick(ClickEvent::RELEASED, (420 * 640 / 800), 230);
        break;
    case 20:
        view.screenClick(ClickEvent::PRESSED, (670 * 640 / 800), 420);
        break;
    case 22:
        view.screenClick(ClickEvent::RELEASED, (670 * 640 / 800), 420);
        break;
    case 26:
        view.screenClick(ClickEvent::PRESSED, (201 * 640 / 800), 313);
        break;
    case 27:
        view.screenClick(ClickEvent::RELEASED, (201 * 640 / 800), 313);
        break;
    case 29:
        view.screenClick(ClickEvent::PRESSED, (230 * 640 / 800), 280);
        break;
    case 30:
        view.screenClick(ClickEvent::RELEASED, (230 * 640 / 800), 280);
        break;
    case 32:
        view.screenClick(ClickEvent::PRESSED, (240 * 640 / 800), 240);
        break;
    case 33:
        view.screenClick(ClickEvent::RELEASED, (201 * 640 / 800), 240);
        break;
    case 35:
        view.screenClick(ClickEvent::PRESSED, (540 * 640 / 800), 240);
        break;
    case 36:
        view.screenClick(ClickEvent::RELEASED, (540 * 640 / 800), 240);
        break;
    case 38:
        view.screenClick(ClickEvent::PRESSED, (100 * 640 / 800), 238);
        break;
    case 39:
        view.screenDrag((100 * 640 / 800), 238, (113 * 640 / 800), 189);
        break;
    case 40:
        view.screenDrag((100 * 640 / 800), 238, (108 * 640 / 800), 200);
        break;
    case 41:
        view.screenDrag((108 * 640 / 800), 200, (113 * 640 / 800), 189);
        break;
    case 42:
        view.screenDrag((113 * 640 / 800), 189, (125 * 640 / 800), 180);
        break;
    case 43:
        view.screenDrag((125 * 640 / 800), 180, (158 * 640 / 800), 179);
        break;
    case 44:
        view.screenClick(ClickEvent::RELEASED, (540 * 640 / 800), 240);
        break;
    case 46:
        view.screenClick(ClickEvent::PRESSED, (700 * 640 / 800), 250);
        break;
    case 47:
        view.screenClick(ClickEvent::RELEASED, (700 * 640 / 800), 250);
        break;
    case 48:
        view.screenClick(ClickEvent::PRESSED, (200 * 640 / 800), 200);
        break;
    case 49:
        view.screenClick(ClickEvent::RELEASED, (200 * 640 / 800), 200);
        break;
    case 51:
        view.screenClick(ClickEvent::PRESSED, (650 * 640 / 800), 450);
        break;
    case 52:
        view.screenClick(ClickEvent::RELEASED, (650 * 640 / 800), 450);
        break;
    case 57:
        view.setDay(10, 22, 6);
        break;
    case 67:
        view.screenClick(ClickEvent::PRESSED, (650 * 640 / 800), 450);
        break;
    case 68:
        view.screenClick(ClickEvent::RELEASED, (650 * 640 / 800), 450);
        break;
    case 72:
        view.screenClick(ClickEvent::PRESSED, (45 * 640 / 800), 45);
        break;
    case 73:
        view.screenClick(ClickEvent::RELEASED, (45 * 640 / 800), 45);
        break;
    }
}

void SettingsPresenter::screenSaverMinorTick()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
}
