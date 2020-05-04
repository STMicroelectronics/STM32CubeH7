#ifndef SETTINGS_VIEW_HPP
#define SETTINGS_VIEW_HPP

#include <gui_generated/settings_screen/SettingsViewBase.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>
#include <gui/common/DotIndicator.hpp>
#include <touchgfx/containers/clock/AnalogClock.hpp>
#include <gui/settings_screen/ClockSelectorContainer.hpp>
#include <gui/settings_screen/TimeSettingContainer.hpp>
#include <gui/settings_screen/DateSelectorContainer.hpp>

class SettingsView : public SettingsViewBase
{
public:
    SettingsView();
    virtual ~SettingsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void mcuLoadUpdated(uint8_t newMcuLoad);
    void screenSaverMinorTick();

    void updateTime(uint8_t h, uint8_t m, uint8_t s, bool animate);
    void updateDate(uint16_t year, uint8_t month, uint8_t day);

    void setSelectedClock(int clockIndex);

protected:
    enum ActiveContainer
    {
        MAIN_SCREEN,
        CLOCK_SELECTOR,
        TIME_SETTING,
        DATE_SELECTOR,
        NO_ACTIVE_CONTAINER
    } currentActiveContainer;

    ActiveContainer animatingToState;

    MoveAnimator<DotIndicator> dot;

    AnalogClock clock;

    MoveAnimator<Container> settingsContainer;

    ClockSelectorContainer clockSelectorContainer;
    Callback<SettingsView, const ClockSelectorContainer&, int> selectedClockChangedCallback;
    void selectedClockChangedHandler(const ClockSelectorContainer& clockSelector, int selectedClockIndex);

    TimeSettingContainer timeSettingContainer;
    DateSelectorContainer dateSelectorContainer;

    Callback<SettingsView, const MoveAnimator<Container>& > moveAnimationEndedCallback;
    void moveAnimationEndedHandler(const MoveAnimator<Container>& moveAnimator);

    void nextDoneButtonHandler();
    void settingsButtonHandler();

    void setupClock(AnalogClock* analogClock, int clockId);
    void updateHeadline();
    void animateContainers();
};

#endif // SETTINGS_VIEW_HPP
