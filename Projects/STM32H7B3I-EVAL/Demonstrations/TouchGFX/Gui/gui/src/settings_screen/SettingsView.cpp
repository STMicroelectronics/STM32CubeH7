#include <gui/settings_screen/SettingsView.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

SettingsView::SettingsView() :
    animatingToState(NO_ACTIVE_CONTAINER),
    selectedClockChangedCallback(this, &SettingsView::selectedClockChangedHandler),
    moveAnimationEndedCallback(this, &SettingsView::moveAnimationEndedHandler)
{
}

void SettingsView::setupScreen()
{
    dot.setBitmaps(Bitmap(BITMAP_DOT_INDICATOR__ID), Bitmap(BITMAP_DOT_INDICATOR_HIGHLIGHTED_ID));
    dot.setNumberOfDots(3);
    add(dot);

    clock.setXY(134, 42);
    mainScreenContainer.add(clock);

    // CLOCK SELECTOR CONTAINER
    clockSelectorContainer.setSelectedClockChangedCallback(selectedClockChangedCallback);
    clockSelectorContainer.setPosition(0, 0, containerViewPort.getWidth(), containerViewPort.getHeight());
    for (int i = 0; i < ClockSelectorContainer::NUMBER_OF_CLOCK_SELECTIONS; i++)
    {
        setupClock(clockSelectorContainer.getClock(i), i);
    }
    settingsContainer.setMoveAnimationEndedAction(moveAnimationEndedCallback);
    settingsContainer.add(clockSelectorContainer);

    // TIME SETTING CONTAINER
    timeSettingContainer.setPosition(clockSelectorContainer.getRect().right(), 0, containerViewPort.getWidth(), containerViewPort.getHeight());
    settingsContainer.add(timeSettingContainer);

    // DATE SELECTOR CONTAINER
    dateSelectorContainer.setPosition(timeSettingContainer.getRect().right(), 0, containerViewPort.getWidth(), containerViewPort.getHeight());
    settingsContainer.add(dateSelectorContainer);

    // SETTINGS CONTAINER
    settingsContainer.setPosition(containerViewPort.getWidth(), 0, dateSelectorContainer.getRect().right(), containerViewPort.getHeight());
    containerViewPort.add(settingsContainer);

    // Set state to main screen
    animatingToState = MAIN_SCREEN;
    moveAnimationEndedHandler(settingsContainer);
}

void SettingsView::tearDownScreen()
{
}

void SettingsView::mcuLoadUpdated(uint8_t newMcuLoad)
{
    mcuLoad.updateProcessorLoad(newMcuLoad);
}

void SettingsView::screenSaverMinorTick()
{
    application().gotoMainMenuCarouselScreenNoTransition();
}

void SettingsView::updateTime(uint8_t h, uint8_t m, uint8_t s, bool animate)
{
    if (animate)
    {
        clock.setTime24Hour(h, m, s);
        clockSelectorContainer.updateTime(h, m, s);
        timeSettingContainer.updateTime(h, m, s);
    }
    else
    {
        clock.initializeTime24Hour(h, m, s);
        clockSelectorContainer.initializeTime(h, m, s);
        timeSettingContainer.initializeTime(h, m, s);
    }
}

void SettingsView::updateDate(uint16_t year, uint8_t month, uint8_t day)
{
    Unicode::snprintf(mainScreenDateYearTextBuffer, MAINSCREENDATEYEARTEXT_SIZE, "%d", year + dateSelectorContainer.getYearOffset());
    mainScreenDateYearText.invalidate();

    Unicode::snprintf(mainScreenDateDayTextBuffer, MAINSCREENDATEDAYTEXT_SIZE, "%d", day + 1);
    mainScreenDateDayText.invalidate();

    Unicode::UnicodeChar monthText[MAINSCREENDATEMONTHTEXT_SIZE];

    if (month < 12)
    {
        const char* months[] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
        Unicode::strncpy(monthText, months[month], MAINSCREENDATEMONTHTEXT_SIZE);
    }
    else
    {
        monthText[0] = 0;
    }

    Unicode::snprintf(mainScreenDateMonthTextBuffer, MAINSCREENDATEMONTHTEXT_SIZE, "%s", monthText);
    mainScreenDateMonthText.invalidate();

    dateSelectorContainer.setDate(year, month, day);
}

void SettingsView::setSelectedClock(int clockIndex)
{
    clockSelectorContainer.setSelectedClock(clockIndex);
    setupClock(&clock, clockSelectorContainer.getSelectedClock());
    setupClock(timeSettingContainer.getClock(), clockSelectorContainer.getSelectedClock());
    timeSettingContainer.getClock()->setHourHandMinuteCorrection(false);
    timeSettingContainer.getClock()->setMinuteHandSecondCorrection(false);
}

void SettingsView::nextDoneButtonHandler()
{
    animateContainers();
}

void SettingsView::settingsButtonHandler()
{
    animateContainers();
}

void SettingsView::selectedClockChangedHandler(const ClockSelectorContainer& clockSelector, int selectedClockIndex)
{
    setSelectedClock(selectedClockIndex);
}

void SettingsView::moveAnimationEndedHandler(const MoveAnimator<Container>& moveAnimator)
{
    currentActiveContainer = animatingToState;
    animatingToState = NO_ACTIVE_CONTAINER;
    updateHeadline();

    if (currentActiveContainer > CLOCK_SELECTOR)
    {
        dot.goRight();
    }
    else if (currentActiveContainer == MAIN_SCREEN)
    {
        // Reset settingsContainer position
        settingsContainer.setXY(containerViewPort.getWidth(), 0);
        dotBackground.setXY(313 + settingsContainer.getWidth(), 315);
        dot.setXY(dotBackground.getX() + 32, containerViewPort.getY() + dotBackground.getY());
        nextDoneButton.setXY(594 + settingsContainer.getWidth(), 310);
    }

    settingsButton.setTouchable(true);
    nextDoneButton.setTouchable(true);
}

void SettingsView::setupClock(AnalogClock* analogClock, int clockId)
{
    // Reset clock settings
    analogClock->setHourHandMinuteCorrection(false);
    analogClock->setMinuteHandSecondCorrection(false);
    analogClock->setAnimation(0);

    if (clockId == 0)
    {
        analogClock->setBackground(BITMAP_CLOCK_00_BACKGROUND_ID, 116, 116);
        analogClock->setupHourHand(BITMAP_CLOCK_00_HOUR_HAND_ID, 2, 44);
        analogClock->setupMinuteHand(BITMAP_CLOCK_00_MINUTE_HAND_ID, 2, 64);
        analogClock->setupSecondHand(BITMAP_CLOCK_00_SECOND_HAND_ID, 4, 79);
        analogClock->setHourHandMinuteCorrection(true);
        analogClock->setMinuteHandSecondCorrection(true);
    }
    else if (clockId == 1)
    {
        analogClock->setBackground(BITMAP_CLOCK_01_BACKGROUND_ID, 116, 116);
        analogClock->setupHourHand(BITMAP_CLOCK_01_HOUR_HAND_ID, 3, 93);
        analogClock->setupSecondHand(BITMAP_CLOCK_01_SECOND_HAND_ID, 3, 84);
        analogClock->setupMinuteHand(BITMAP_CLOCK_01_MINUTE_HAND_ID, 8, 73);
        analogClock->setHourHandMinuteCorrection(true);
        analogClock->setMinuteHandSecondCorrection(true);
    }
    else
    {
        analogClock->setBackground(BITMAP_CLOCK_02_BACKGROUND_ID, 116, 116);
        analogClock->setupHourHand(BITMAP_CLOCK_02_HOUR_HAND_ID, 7, 51);
        analogClock->setupMinuteHand(BITMAP_CLOCK_02_MINUTE_HAND_ID, 7, 67);
        analogClock->setupSecondHand(BITMAP_CLOCK_02_SECOND_HAND_ID, 3, 66);
        analogClock->setHourHandMinuteCorrection(true);
        analogClock->setAnimation(14);
    }
    analogClock->invalidate();
}

void SettingsView::updateHeadline()
{
    switch (currentActiveContainer)
    {
    case SettingsView::MAIN_SCREEN:
        headline.setTypedText(TypedText(T_SETTINGS_HEADLINE0));
        break;
    case SettingsView::CLOCK_SELECTOR:
        headline.setTypedText(TypedText(T_SETTINGS_HEADLINE3));
        break;
    case SettingsView::TIME_SETTING:
        headline.setTypedText(TypedText(T_SETTINGS_HEADLINE1));
        break;
    case SettingsView::DATE_SELECTOR:
        headline.setTypedText(TypedText(T_SETTINGS_HEADLINE2));
        break;
    default:
        break;
    }

    headline.invalidate();
}

void SettingsView::animateContainers()
{
    int duration = 14;
    int delay = 6;

    mainScreenContainer.setMoveAnimationDelay(0);
    settingsButton.setFadeAnimationDelay(0);
    settingsContainer.setMoveAnimationDelay(0);
    dotBackground.setMoveAnimationDelay(0);
    dot.setMoveAnimationDelay(0);
    nextDoneButton.setMoveAnimationDelay(0);

    settingsButton.setTouchable(false);
    nextDoneButton.setTouchable(false);

    switch (currentActiveContainer)
    {
    case SettingsView::MAIN_SCREEN:
        settingsContainer.setXY(containerViewPort.getWidth(), 0);
        dotBackground.setXY(313 + settingsContainer.getWidth(), 315);
        dot.setXY(dotBackground.getX() + 32, containerViewPort.getY() + dotBackground.getY());
        nextDoneButton.setXY(594 + settingsContainer.getWidth(), 310);

        animatingToState = CLOCK_SELECTOR;

        nextDoneButton.setBitmaps(Bitmap(BITMAP_SETTINGS_NEXT_ID), Bitmap(BITMAP_SETTINGS_NEXT_PRESSED_ID));
        nextDoneButton.invalidate();

        mainScreenContainer.startMoveAnimation(mainScreenContainer.getX(), -mainScreenContainer.getHeight(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);

        settingsContainer.setMoveAnimationDelay(delay);
        dotBackground.setMoveAnimationDelay(delay);
        dot.setMoveAnimationDelay(delay);
        nextDoneButton.setMoveAnimationDelay(delay);

        settingsContainer.startMoveAnimation(0, 0, duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        dotBackground.startMoveAnimation(dotBackground.getX() - settingsContainer.getWidth(), dotBackground.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        dot.startMoveAnimation(dot.getX() - settingsContainer.getWidth(), dot.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        nextDoneButton.startMoveAnimation(nextDoneButton.getX() - settingsContainer.getWidth(), nextDoneButton.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);

        settingsButton.startFadeAnimation(0, duration - delay, EasingEquations::cubicEaseIn);

        dot.setHighlightPosition(0);
        break;
    case SettingsView::CLOCK_SELECTOR:
        animatingToState = TIME_SETTING;

        settingsContainer.startMoveAnimation(settingsContainer.getX() - clockSelectorContainer.getWidth(), 0, duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        timeSettingContainer.setActive(true);

        break;
    case SettingsView::TIME_SETTING:
        animatingToState = DATE_SELECTOR;

        settingsContainer.startMoveAnimation(settingsContainer.getX() - timeSettingContainer.getWidth(), 0, duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);

        nextDoneButton.setBitmaps(Bitmap(BITMAP_SETTINGS_DONE_ID), Bitmap(BITMAP_SETTINGS_DONE_PRESSED_ID));
        nextDoneButton.invalidate();

        break;
    case SettingsView::DATE_SELECTOR:
        animatingToState = MAIN_SCREEN;

        mainScreenContainer.setMoveAnimationDelay(delay);
        mainScreenContainer.startMoveAnimation(mainScreenContainer.getX(), 0, duration - delay, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        settingsContainer.startMoveAnimation(settingsContainer.getX() - dateSelectorContainer.getWidth(), 0, duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        dotBackground.startMoveAnimation(dotBackground.getX() - dateSelectorContainer.getWidth(), dotBackground.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        dot.startMoveAnimation(dot.getX() - dateSelectorContainer.getWidth(), dot.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        nextDoneButton.startMoveAnimation(nextDoneButton.getX() - dateSelectorContainer.getWidth(), nextDoneButton.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);

        settingsButton.setFadeAnimationDelay(delay);
        settingsButton.startFadeAnimation(255, duration - delay, EasingEquations::cubicEaseIn);

        // Communicate the selected values to the model
        uint8_t hour, minute, second;
        timeSettingContainer.getTime(hour, minute, second);
        presenter->stateUpdated(clockSelectorContainer.getSelectedClock(), hour, minute, second, dateSelectorContainer.getYear(), dateSelectorContainer.getMonth(), dateSelectorContainer.getDay());
        timeSettingContainer.setActive(false);
        break;
    default:
        break;
    }
}
