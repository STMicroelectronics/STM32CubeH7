#include <gui/settings_screen/SettingsView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

SettingsView::SettingsView() :
    animatingToState(NO_ACTIVE_CONTAINER),
    buttonPressedCallback(this, &SettingsView::buttonPressedHandler),
    selectedClockChangedCallback(this, &SettingsView::selectedClockChangedHandler),
    moveAnimationEndedCallback(this, &SettingsView::moveAnimationEndedHandler)
{

}

SettingsView::~SettingsView()
{
}

void SettingsView::setupScreen()
{
    background.setBitmap(Bitmap(BITMAP_CONTROLS_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_MENU_ICON_ID), Bitmap(BITMAP_MENU_ICON_PRESSED_ID));
    gotoMenuButton.setXY(21, 14);
    add(gotoMenuButton);

    mcuLoadTxt.setXY(620 - 160 + 32, 32);
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right() - 6, mcuLoadTxt.getY(), 50, 30);
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    mcuLoadValueTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    headline.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    headline.setPosition(160, 20, 320, 30);
    add(headline);

    settingsButton.setBitmaps(Bitmap(BITMAP_SETTINGS_BUTTON_ID), Bitmap(BITMAP_SETTINGS_BUTTON_PRESSED_ID));
    settingsButton.setXY((HAL::DISPLAY_WIDTH - settingsButton.getWidth()) / 2, 310 + 84);
    settingsButton.setAction(buttonPressedCallback);
    add(settingsButton);

    containerViewPort.setPosition(0, 84, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT - 84);
    add(containerViewPort);


    // MAIN CONTAINER
    mainScreenContainer.setPosition(0, 0, containerViewPort.getWidth(), containerViewPort.getHeight());
    containerViewPort.add(mainScreenContainer);

    clock.setXY(64, 42);
    mainScreenContainer.add(clock);

    calendar.setBitmap(Bitmap(BITMAP_SETTINGS_CALENDAR_ID));
    calendar.setXY(360, 46);
    mainScreenContainer.add(calendar);

    mainScreenDateYearTextBuffer[0] = 0;
    mainScreenDateYearText.setTypedText(TypedText(T_SETTINGS_MAIN_DATE_YEAR));
    mainScreenDateYearText.setWildcard(mainScreenDateYearTextBuffer);
    mainScreenDateYearText.setColor(Color::getColorFrom24BitRGB(0x1B, 0x38, 0x48));
    mainScreenDateYearText.setXY(calendar.getX(), 0);
    mainScreenDateYearText.setBaselineY(calendar.getY() + 196);
    mainScreenDateYearText.setWidth(calendar.getWidth());
    mainScreenContainer.add(mainScreenDateYearText);

    mainScreenDateMonthTextBuffer[0] = 0;
    mainScreenDateMonthText.setTypedText(TypedText(T_SETTINGS_MAIN_DATE_MONTH));
    mainScreenDateMonthText.setWildcard(mainScreenDateMonthTextBuffer);
    mainScreenDateMonthText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    mainScreenDateMonthText.setXY(calendar.getX(), 0);
    mainScreenDateMonthText.setBaselineY(calendar.getY() + 65);
    mainScreenDateMonthText.setWidth(calendar.getWidth());
    mainScreenContainer.add(mainScreenDateMonthText);

    mainScreenDateDayTextBuffer[0] = 0;
    mainScreenDateDayText.setTypedText(TypedText(T_SETTINGS_MAIN_DATE_DAY));
    mainScreenDateDayText.setWildcard(mainScreenDateDayTextBuffer);
    mainScreenDateDayText.setColor(Color::getColorFrom24BitRGB(0x1B, 0x38, 0x48));
    mainScreenDateDayText.setXY(calendar.getX(), 0);
    mainScreenDateDayText.setBaselineY(calendar.getY() + 157);
    mainScreenDateDayText.setWidth(calendar.getWidth());
    mainScreenContainer.add(mainScreenDateDayText);

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


    dotBackground.setBitmap(Bitmap(BITMAP_CONTROLS_3DOTS_ID));
    containerViewPort.add(dotBackground);

    dot.setBitmaps(Bitmap(BITMAP_DOT_INDICATOR__ID), Bitmap(BITMAP_DOT_INDICATOR_HIGHLIGHTED_ID));
    dot.setNumberOfDots(3);
    add(dot);

    nextDoneButton.setBitmaps(Bitmap(BITMAP_SETTINGS_NEXT_ID), Bitmap(BITMAP_SETTINGS_NEXT_PRESSED_ID));
    nextDoneButton.setAction(buttonPressedCallback);
    containerViewPort.add(nextDoneButton);

    // Set state to main screen
    animatingToState = MAIN_SCREEN;
    moveAnimationEndedHandler(settingsContainer);
}

void SettingsView::tearDownScreen()
{

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

void SettingsView::buttonPressedHandler(const AbstractButton& button)
{
    if (&button == &settingsButton || &button == &nextDoneButton)
    {
        animateContainers();
    }

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

        dotBackground.cancelMoveAnimation();
        dot.cancelMoveAnimation();
        nextDoneButton.cancelMoveAnimation();

        dotBackground.setXY(313 - 80 + settingsContainer.getWidth(), 315);
        dot.setXY(dotBackground.getX() + 32, containerViewPort.getY() + dotBackground.getY());
        nextDoneButton.setXY(594 - 160 + 13 + settingsContainer.getWidth(), 310);
    }

    settingsButton.setTouchable(true);
    nextDoneButton.setTouchable(true);

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
        dotBackground.startMoveAnimation(dotBackground.getX()  - dateSelectorContainer.getWidth(), dotBackground.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        dot.startMoveAnimation(dot.getX()  - dateSelectorContainer.getWidth(), dot.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        nextDoneButton.startMoveAnimation(nextDoneButton.getX()  - dateSelectorContainer.getWidth(), nextDoneButton.getY(), duration, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);

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

void SettingsView::setupClock(AnalogClock* analogClock, int clockId)
{
    // Reset clock settings
    analogClock->useHourHandMinuteCorrection(false);
    analogClock->useMinuteHandSecondCorrection(false);
    analogClock->useAnimation(0);

    if (clockId == 0)
    {
        analogClock->setBackground(BITMAP_CLOCK_01_BACKGROUND_ID, 116, 116);
        analogClock->setupHourHand(BITMAP_CLOCK_01_HOUR_HAND_ID, 3, 93);
        analogClock->setupSecondHand(BITMAP_CLOCK_01_SECOND_HAND_ID, 3, 84);
        analogClock->setupMinuteHand(BITMAP_CLOCK_01_MINUTE_HAND_ID, 8, 73);
        analogClock->useHourHandMinuteCorrection(true);
        analogClock->useMinuteHandSecondCorrection(true);
    }
    else if (clockId == 1)
    {
        analogClock->setBackground(BITMAP_CLOCK_02_BACKGROUND_ID, 116, 116);
        analogClock->setupHourHand(BITMAP_CLOCK_02_HOUR_HAND_ID, 7, 51);
        analogClock->setupMinuteHand(BITMAP_CLOCK_02_MINUTE_HAND_ID, 7, 67);
        analogClock->setupSecondHand(BITMAP_CLOCK_02_SECOND_HAND_ID, 3, 66);
        analogClock->useHourHandMinuteCorrection(true);
        analogClock->useAnimation(14);
    }
    else
    {
        analogClock->setBackground(BITMAP_CLOCK_00_BACKGROUND_ID, 116, 116);
        analogClock->setupHourHand(BITMAP_CLOCK_00_HOUR_HAND_ID, 2, 44);
        analogClock->setupMinuteHand(BITMAP_CLOCK_00_MINUTE_HAND_ID, 2, 64);
        analogClock->setupSecondHand(BITMAP_CLOCK_00_SECOND_HAND_ID, 4, 79);
        analogClock->useHourHandMinuteCorrection(true);
        analogClock->useMinuteHandSecondCorrection(true);
    }
    analogClock->invalidate();
}

void SettingsView::updateTime(uint8_t h, uint8_t m, uint8_t s, bool animate)
{
    if (animate)
    {
        clock.updateTime(h % 12, m, s);
        clockSelectorContainer.updateTime(h % 12, m, s);
        timeSettingContainer.updateTime(h, m, s);
    }
    else
    {
        clock.initializeTime(h % 12, m, s);
        clockSelectorContainer.initializeTime(h % 12, m, s);
        timeSettingContainer.initializeTime(h, m, s);
    }
}

void SettingsView::setSelectedClock(int clockIndex)
{
    clockSelectorContainer.setSelectedClock(clockIndex);
    setupClock(&clock, clockSelectorContainer.getSelectedClock());
    setupClock(timeSettingContainer.getClock(), clockSelectorContainer.getSelectedClock());
    timeSettingContainer.getClock()->useHourHandMinuteCorrection(false);
    timeSettingContainer.getClock()->useMinuteHandSecondCorrection(false);
}

void SettingsView::updateDate(uint16_t year, uint8_t month, uint8_t day)
{
    Unicode::snprintf(mainScreenDateYearTextBuffer, 5, "%d", year + dateSelectorContainer.getYearOffset());
    mainScreenDateYearText.invalidate();

    Unicode::snprintf(mainScreenDateDayTextBuffer, 5, "%d", day);
    mainScreenDateDayText.invalidate();

    Unicode::UnicodeChar monthText[10];

    if (month < 12)
    {
        const char* months[] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
        Unicode::strncpy(monthText, months[month], 10);
    }
    else
    {
        monthText[0] = 0;
    }

    Unicode::snprintf(mainScreenDateMonthTextBuffer, 10, "%s", monthText);
    mainScreenDateMonthText.invalidate();


    dateSelectorContainer.setDate(year, month, day - 1);
}

void SettingsView::screenClick(ClickEvent::ClickEventType type, int x, int y)
{
    ClickEvent temp_evt(type, x, y);
    View::handleClickEvent(temp_evt);
}

void SettingsView::screenDrag(int16_t x_old, int16_t y_old, int16_t x_new, int16_t y_new)
{
    DragEvent temp_evt(DragEvent::DRAGGED, x_old, y_old, x_new, y_new);
    View::handleDragEvent(temp_evt);
}

void SettingsView::setDay(uint8_t m, uint8_t d, uint16_t y)
{
    dateSelectorContainer.setYear(y, 75);
    dateSelectorContainer.setMonth(m, 160);
    dateSelectorContainer.setDay(d, 110);
}
