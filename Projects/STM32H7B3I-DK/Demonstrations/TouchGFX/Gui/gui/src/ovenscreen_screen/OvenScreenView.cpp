#include <gui/ovenscreen_screen/OvenScreenView.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

#include <gui/common/Defines.hpp>

OvenScreenView::OvenScreenView() :
    OvenState(OVEN_MODE),
    splashScreenPressedCallback(this, &OvenScreenView::splashScreenPressedHandler),
    animationEnddedCallback(this, &OvenScreenView::animationEnddedHandler),
    scrollWheelOvenProgramAnimateToCallback(this, &OvenScreenView::scrollWheelOvenProgramAnimateToHandler),
    splashScreenFadeEndedCallback(this, &OvenScreenView::splashScreenFadeEndedHandler),
    buttonFadeEndedCallback(this, &OvenScreenView::buttonFadeEndedHandler),
    colorCnt(0),
    changeColorTimer(false),
    changeColorTemp(false),
    selectedItemDragged(false),
    totalDrag(0),
    autoDemoCounter(0)
{

}

void OvenScreenView::setupScreen()
{
    OvenScreenViewBase::setupScreen();

    // Since the three containers OvenTimerContainer, OvenTemperatureContainer and OvenSplashScreen, is added in code,
    // there position is set and they are added to the screen.
    ovenTimerMenu.setXY(0, 0);
    ovenTimerMenu.setAlpha(0);
    ovenTimerMenu.initialize();
    add(ovenTimerMenu);

    ovenTempMenu.setXY(0, 0);
    ovenTempMenu.setAlpha(0);
    ovenTempMenu.initialize();
    add(ovenTempMenu);

    remove(containerScreenElements);
    add(containerScreenElements);

    ovenSplashScreen.initialize();
    ovenSplashScreen.setXY(0, 0);
    add(ovenSplashScreen);

    ovenSplashScreen.setScreenPressedAction(splashScreenPressedCallback);
    ovenSplashScreen.setFadeAnimationEndedAction(splashScreenFadeEndedCallback);

    scrollWheelOvenProgram.setAnimationEndedCallback(animationEnddedCallback);
    scrollWheelOvenProgram.setAnimateToCallback(scrollWheelOvenProgramAnimateToCallback);
    scrollWheelOvenProgram.setAnimationSteps(20);

    scrollWheelOvenProgramSelectedListItems[0].zoomImage(true);
    scrollWheelOvenProgramSelectedListItems[1].zoomImage(true);

    currentItem = presenter->getSelectedOvenMode();

    fadeOvenModeWheel(0, 0);
    buttonOk.setFadeAnimationEndedAction(buttonFadeEndedCallback);

    ovenTimerMenu.setWheelsValue(presenter->getOvenTimerHour(), presenter->getOvenTimerMinute());
  
    presenter->setOvenTimerHour(ovenTimerMenu.getSelectedHour());
    presenter->setOvenTimerMinute(ovenTimerMenu.getSelectedMinute());

    Unicode::snprintf(textAreaTimerBuffer1, TEXTAREATIMERBUFFER1_SIZE, "%d", presenter->getOvenTimerHour());
    Unicode::snprintf(textAreaTimerBuffer2, TEXTAREATIMERBUFFER2_SIZE, "%d", presenter->getOvenTimerMinute());
    textAreaTimer.invalidate();

    ovenTempMenu.setCurrentTemp(presenter->getOvenTemperature());
    Unicode::snprintf(textAreaTemperatureBuffer, TEXTAREATEMPERATURE_SIZE, "%d", presenter->getOvenTemperature());
    textAreaTimer.invalidate();

    if (presenter->getShowOven())
    {
        buttonStartStop.setBitmaps(Bitmap(BITMAP_STOP_KNAP_ID), Bitmap(BITMAP_STOP_KNAP_PRESSED_ID));
    }
    else
    {
        buttonStartStop.setBitmaps(Bitmap(BITMAP_START_KNAP_ID), Bitmap(BITMAP_START_KNAP_PRESSED_ID));
    }

    scrollWheelOvenProgram.animateToItem(currentItem, 0);

    mcu.initialize();
    mcu.setMCUOnly(true);
    mcu.setTextColor(Color::getColorFrom24BitRGB(255, 255, 255));
    remove(mcu);
    add(mcu);

    if (presenter->getOvenEntered())
    {
        ovenSplashScreen.setVisible(false);
        fadeOvenModeElements(255, 0);

        imageButtomBar.setFadeAnimationDelay(0);
        imageButtomBar.startFadeAnimation(255, OVEN_FADE_DURATION);

        textAreaTemperature.setFadeAnimationDelay(0);
        textAreaTimer.setFadeAnimationDelay(0);
        textAreaTemperature.startFadeAnimation(255, 0);
        textAreaTimer.startFadeAnimation(255, 0);

        buttonKitchen.setFadeAnimationDelay(0);
        buttonKitchen.startFadeAnimation(255, 0);      

        animationEnddedHandler();
    }
}

void OvenScreenView::tearDownScreen()
{
    OvenScreenViewBase::tearDownScreen();
}

void OvenScreenView::buttonTurnOnOff()
{
    // When the oven is turned on/off the model is informerd of the state change,
    // and the updated state is fetched to updated the bitmaps for the start/stop button.
    presenter->setShowOven(!presenter->getShowOven());

    if (presenter->getShowOven())
    {
        buttonStartStop.setBitmaps(Bitmap(BITMAP_STOP_KNAP_ID), Bitmap(BITMAP_STOP_KNAP_PRESSED_ID));
    }
    else
    {
        buttonStartStop.setBitmaps(Bitmap(BITMAP_START_KNAP_ID), Bitmap(BITMAP_START_KNAP_PRESSED_ID));
    }
}

void OvenScreenView::tempButtonClicked()
{
    // When the temperature is pressed the scrollWheelRoomSelector fades out and the temperature wheel fades in
    // The text for time is also cahnging color.
    if (!changeColorTimer)
    {
        changeColorTemp = true;
        colorCnt = 1;

        currentColor = textAreaTimer.getColor();
        goalColor = Color::getColorFrom24BitRGB(TEXT_UNFOCUS_R, TEXT_UNFOCUS_G, TEXT_UNFOCUS_B);

        fadeOvenModeElements(0, OVEN_FADE_DURATION);

        scrollWheelOvenProgram.setTouchable(false);
        buttonStartStop.setTouchable(false);
        buttonTemperature.setTouchable(false);
        buttonTimer.setTouchable(false);

        OvenState = OVEN_TEMPERATURE;

        ovenTempMenu.setTouchable(true);
        ovenTempMenu.setFadeAnimationDelay(OVEN_FADE_DURATION + OVEN_FADE_DURATION);
        ovenTempMenu.startFadeAnimation(255, OVEN_FADE_DURATION);
        buttonOk.setFadeAnimationDelay(OVEN_FADE_DURATION + OVEN_FADE_DURATION);
        buttonOk.startFadeAnimation(255, OVEN_FADE_DURATION);
        buttonOk.setVisible(true);
        buttonOk.setTouchable(false);
    }
}

void OvenScreenView::timerButtonClicked()
{
    // When the timer is pressed the scrollWheelRoomSelector fades out and the timer wheels fades in
    // The text for time is also cahnging color.
    if (!changeColorTemp)
    {
        changeColorTimer = true;
        colorCnt = 1;

        currentColor = textAreaTimer.getColor();
        goalColor = Color::getColorFrom24BitRGB(TEXT_UNFOCUS_R, TEXT_UNFOCUS_G, TEXT_UNFOCUS_B);

        fadeOvenModeElements(0, OVEN_FADE_DURATION);

        scrollWheelOvenProgram.setTouchable(false);
        buttonStartStop.setTouchable(false);
        buttonTemperature.setTouchable(false);
        buttonTimer.setTouchable(false);

        OvenState = OVEN_TIMER;

        ovenTimerMenu.setFadeAnimationDelay(OVEN_FADE_DURATION + OVEN_FADE_DURATION);
        ovenTimerMenu.startFadeAnimation(255, OVEN_FADE_DURATION);
        buttonOk.setFadeAnimationDelay(OVEN_FADE_DURATION + OVEN_FADE_DURATION);
        buttonOk.startFadeAnimation(255, OVEN_FADE_DURATION);
        buttonOk.setVisible(true);
        buttonOk.setTouchable(false);
    }
}

void OvenScreenView::okButtonClicked()
{
    //Presssing ok stores the value selected for either the timer or temperature and returns to
    //the oven mode selector
    switch (OvenState)
    {
    case OVEN_MODE:
        break;

    case OVEN_TEMPERATURE:
        ovenTempMenu.stopWheel();
        presenter->setOvenTemperature(ovenTempMenu.getCurrnetTemp());
        Unicode::snprintf(textAreaTemperatureBuffer, TEXTAREATEMPERATURE_SIZE, "%d", presenter->getOvenTemperature());
        textAreaTemperature.invalidate();

        changeColorTemp = true;
        colorCnt = 1;

        currentColor = textAreaTimer.getColor();
        goalColor = Color::getColorFrom24BitRGB(TEXT_FOCUS_R, TEXT_FOCUS_G, TEXT_FOCUS_B);

        ovenTempMenu.setTouchable(false);
        ovenTempMenu.setFadeAnimationDelay(0);
        ovenTempMenu.startFadeAnimation(0, OVEN_FADE_DURATION);
        buttonOk.setFadeAnimationDelay(0);
        buttonOk.startFadeAnimation(0, OVEN_FADE_DURATION);

        fadeOvenModeElements(255, OVEN_FADE_DURATION, OVEN_FADE_DURATION + OVEN_FADE_DURATION);

        scrollWheelOvenProgram.setTouchable(true);
        buttonTemperature.setTouchable(true);
        buttonTimer.setTouchable(true);

        buttonStartStop.setTouchable(true);

        OvenState = OVEN_MODE;

        break;

    case OVEN_TIMER:
        presenter->setOvenTimerHour(ovenTimerMenu.getSelectedHour());
        presenter->setOvenTimerMinute(ovenTimerMenu.getSelectedMinute());

        Unicode::snprintf(textAreaTimerBuffer1, TEXTAREATIMERBUFFER1_SIZE, "%d", presenter->getOvenTimerHour());
        Unicode::snprintf(textAreaTimerBuffer2, TEXTAREATIMERBUFFER2_SIZE, "%d", presenter->getOvenTimerMinute());
        textAreaTimer.invalidate();

        changeColorTimer = true;
        colorCnt = 1;

        currentColor = textAreaTemperature.getColor();
        goalColor = Color::getColorFrom24BitRGB(TEXT_FOCUS_R, TEXT_FOCUS_G, TEXT_FOCUS_B);

        ovenTimerMenu.setFadeAnimationDelay(0);
        ovenTimerMenu.startFadeAnimation(0, OVEN_FADE_DURATION);
        buttonOk.setFadeAnimationDelay(0);
        buttonOk.startFadeAnimation(0, OVEN_FADE_DURATION);

        fadeOvenModeElements(255, OVEN_FADE_DURATION, OVEN_FADE_DURATION + OVEN_FADE_DURATION);

        scrollWheelOvenProgram.setTouchable(true);
        buttonTemperature.setTouchable(true);
        buttonTimer.setTouchable(true);

        buttonStartStop.setTouchable(true);

        OvenState = OVEN_MODE;
        break;
    }
}

void OvenScreenView::scrollWheelOvenProgramUpdateItem(OvenWheelElement& item, int16_t itemIndex)
{
    item.setImage(itemIndex);
}

void OvenScreenView::scrollWheelOvenProgramUpdateCenterItem(OvenWheelElement& item, int16_t itemIndex)
{
    item.setImage(itemIndex);
}

void OvenScreenView::animationEnddedHandler()
{
    scrollWheelOvenProgramSelectedListItems[0].zoomImage(true);
    scrollWheelOvenProgramSelectedListItems[1].zoomImage(true);

    // The switch uses the value calculated in scrollWheelOvenProgramAnimateToHandler() 
    // to set the new mode text.
    switch (currentItem)
    {
    case 0:
        textAreaOvenMode.setTypedText(TypedText(T_OVENMODELIGHTS));
        presenter->setSelectedOvenMode(0);
        break;
    case 1:
        textAreaOvenMode.setTypedText(TypedText(T_OVENMODETOP));
        presenter->setSelectedOvenMode(1);
        break;
    case 2:
        textAreaOvenMode.setTypedText(TypedText(T_OVENMODEBUTTOM));
        presenter->setSelectedOvenMode(2);
        break;
    case 3:
        textAreaOvenMode.setTypedText(TypedText(T_OVENMODEFAN));
        presenter->setSelectedOvenMode(3);
        break;
    case 4:
        textAreaOvenMode.setTypedText(TypedText(T_OVENMODEGRILL));
        presenter->setSelectedOvenMode(4);
        break;
    case 5:
        textAreaOvenMode.setTypedText(TypedText(T_OVENMODEGRILLFAN));
        presenter->setSelectedOvenMode(5);
        break;
    case 6:
        textAreaOvenMode.setTypedText(TypedText(T_OVENMODECLEAN));
        presenter->setSelectedOvenMode(6);
        break;
    }
    textAreaOvenMode.setFadeAnimationDelay(0);
    textAreaOvenMode.startFadeAnimation(255, OVEN_MODE_TEXT_FADE_DURATION);
}

void OvenScreenView::scrollWheelOvenProgramAnimateToHandler(int16_t itemSelected)
{
    // Since the value of itemSelected is the previous item +/- "steps" to the new item,
    // the value of the actual number of items and their index is calculated
    int selectedItem = (itemSelected % 7) < 0 ? 7 + (itemSelected % 7) : (itemSelected % 7);

    if (currentItem != selectedItem)
    {
        scrollWheelOvenProgramSelectedListItems[0].zoomImage(false);
        scrollWheelOvenProgramSelectedListItems[1].zoomImage(false);
        textAreaOvenMode.setFadeAnimationDelay(0);
        textAreaOvenMode.startFadeAnimation(0, OVEN_MODE_TEXT_FADE_DURATION);
    }
    currentItem = selectedItem;
}

void OvenScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        totalDrag = 0;

        if (selectedItemDragged)
        {
            selectedItemDragged = false;
        }
    }
    OvenScreenViewBase::handleClickEvent(evt);
}

void OvenScreenView::handleDragEvent(const DragEvent& evt)
{
    totalDrag += evt.getDeltaX();

    if(scrollWheelOvenProgram.isAnimating())
    {
        if (totalDrag > 6 || totalDrag < -6)
        {
            scrollWheelOvenProgramSelectedListItems[0].zoomImage(false);
            scrollWheelOvenProgramSelectedListItems[1].zoomImage(false);
            if (!textAreaOvenMode.isFadeAnimationRunning() && textAreaOvenMode.getAlpha() != 0)
            {
                textAreaOvenMode.setFadeAnimationDelay(0);
                textAreaOvenMode.startFadeAnimation(0, OVEN_MODE_TEXT_FADE_DURATION);                
            }
            View::handleDragEvent(evt);
        }        
        selectedItemDragged = true;
    }
    else
    {
        View::handleDragEvent(evt);
    }    
}

void OvenScreenView::handleTickEvent()
{
    //when a text is set to change color EasingEquation is used to calcualte the new RGB value which is assigned to the text area
    if (changeColorTemp && colorCnt <= OVEN_FADE_DURATION)
    {
        int tempR = EasingEquations::cubicEaseOut(colorCnt, Color::getRedColor(currentColor.getColor32()), -(Color::getRedColor(currentColor.getColor32()) - Color::getRedColor(goalColor.getColor32())), OVEN_FADE_DURATION);
        int tempG = EasingEquations::cubicEaseOut(colorCnt, Color::getGreenColor(currentColor.getColor32()), -(Color::getGreenColor(currentColor.getColor32()) - Color::getGreenColor(goalColor.getColor32())), OVEN_FADE_DURATION);
        int tempB = EasingEquations::cubicEaseOut(colorCnt, Color::getBlueColor(currentColor.getColor32()), -(Color::getBlueColor(currentColor.getColor32()) - Color::getBlueColor(goalColor.getColor32())), OVEN_FADE_DURATION);

        textAreaTimer.setColor(Color::getColorFrom24BitRGB(tempR, tempG, tempB));
        textAreaTimer.invalidate();

        changeColorTemp = colorCnt >= OVEN_FADE_DURATION ? false : true;
        colorCnt++;
    }
    else if (changeColorTimer && colorCnt <= OVEN_FADE_DURATION)
    {
        int tempR = EasingEquations::cubicEaseOut(colorCnt, Color::getRedColor(currentColor.getColor32()), -(Color::getRedColor(currentColor.getColor32()) - Color::getRedColor(goalColor.getColor32())), OVEN_FADE_DURATION);
        int tempG = EasingEquations::cubicEaseOut(colorCnt, Color::getGreenColor(currentColor.getColor32()), -(Color::getGreenColor(currentColor.getColor32()) - Color::getGreenColor(goalColor.getColor32())), OVEN_FADE_DURATION);
        int tempB = EasingEquations::cubicEaseOut(colorCnt, Color::getBlueColor(currentColor.getColor32()), -(Color::getBlueColor(currentColor.getColor32()) - Color::getBlueColor(goalColor.getColor32())), OVEN_FADE_DURATION);

        textAreaTemperature.setColor(Color::getColorFrom24BitRGB(tempR, tempG, tempB));
        textAreaTemperature.invalidate();

        changeColorTimer = colorCnt >= OVEN_FADE_DURATION ? false : true;
        colorCnt++;
    }
}

void OvenScreenView::fadeOvenModeElements(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    textAreaOvenMode.setFadeAnimationDelay(delay);
    buttonStartStop.setFadeAnimationDelay(delay);
    imageButtomBarWheel.setFadeAnimationDelay(delay);

    textAreaOvenMode.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    buttonStartStop.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    imageButtomBarWheel.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);

    fadeOvenModeWheel(endAlpha, duration, delay, alphaProgressionEquation);
}

void OvenScreenView::fadeOvenModeWheel(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    for (int cnt = 0; cnt < scrollWheelOvenProgramSelectedListItems.getNumberOfDrawables(); cnt++)
    {
        scrollWheelOvenProgramSelectedListItems[cnt].startFade(endAlpha, duration, delay, alphaProgressionEquation);
    }

    for (int cnt = 0; cnt < scrollWheelOvenProgramListItems.getNumberOfDrawables(); cnt++)
    {
        scrollWheelOvenProgramListItems[cnt].startFade(endAlpha, duration, delay, alphaProgressionEquation);
    }
}

void OvenScreenView::splashScreenPressedHandler()
{
    ovenSplashScreen.startFadeAnimation(0, OVEN_FADE_DURATION);
}

void OvenScreenView::splashScreenFadeEndedHandler(const FadeAnimator<OvenSplashScreen>& src)
{
    if (ovenSplashScreen.getAlpha() == 0)
    {
        ovenSplashScreen.setVisible(false);
        fadeOvenModeElements(255, OVEN_FADE_DELAY);

        imageButtomBar.setFadeAnimationDelay(0);
        imageButtomBar.startFadeAnimation(255, OVEN_FADE_DURATION);

        textAreaTemperature.setFadeAnimationDelay(0);
        textAreaTimer.setFadeAnimationDelay(0);
        textAreaTemperature.startFadeAnimation(255, OVEN_FADE_DURATION);
        textAreaTimer.startFadeAnimation(255, OVEN_FADE_DURATION);

        buttonKitchen.setFadeAnimationDelay(0);
        buttonKitchen.startFadeAnimation(255, OVEN_FADE_DURATION);

        animationEnddedHandler();
    }
}

void OvenScreenView::buttonFadeEndedHandler(const FadeAnimator<Button>& src)
{
    if (&src == &buttonOk)
    {
        if (buttonOk.getAlpha() == 0)
        {
            buttonOk.setVisible(false);
        }
        else if (buttonOk.getAlpha() == 255)
        {
            buttonOk.setTouchable(true);
        }
    }
}

void OvenScreenView::setMcuLoad(uint8_t load)
{
    mcu.setMcuLoad(load);
}

void OvenScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::OvenKitchen:
        switch (autoDemoCounter)
        {
        case 4:
            screenClick(ClickEvent::PRESSED, 47, 117);
            break;

        case 6:
            screenClick(ClickEvent::RELEASED, 47, 117);
            break;

        case 10:
            scrollWheelOvenProgram.animateToItem(5, 100);
            break;

        case 20:
            scrollWheelOvenProgram.animateToItem(1, 150);
            break;

        case 46:
            screenClick(ClickEvent::PRESSED, 400, 230);
            break;

        case 48:
            screenClick(ClickEvent::RELEASED, 400, 230);
            break;

        case 56:
            ovenTimerMenu.animateHourWheelToItem(13, 200);
            break;

        case 64:
            ovenTimerMenu.animateMinuteWheelToItem(12, 160);
            break;

        case 80:
            screenClick(ClickEvent::PRESSED, 240, 240);
            break;

        case 82:
            screenClick(ClickEvent::RELEASED, 240, 240);
            break;

        case 90:
            screenClick(ClickEvent::PRESSED, 70, 230);
            break;

        case 92:
            screenClick(ClickEvent::RELEASED, 70, 230);
            break;

        case 100:
            screenClick(ClickEvent::PRESSED, 30, 145);
            break;

        case 102:
            screenGesture(GestureEvent::SWIPE_VERTICAL, 20, 100, 145);
            break;

        case 106:
            screenClick(ClickEvent::RELEASED, 300, 145);
            break;

        case 120:
            screenClick(ClickEvent::PRESSED, 240, 240);
            break;

        case 122:
            screenClick(ClickEvent::RELEASED, 240, 240);
            break;

        case 130:
            screenClick(ClickEvent::PRESSED, 240, 240);
            break;

        case 132:
            screenClick(ClickEvent::RELEASED, 240, 240);
            break;

        case 138:
            screenClick(ClickEvent::PRESSED, 30, 30);
            break;

        case 140:
            presenter->setAutoDemoState(Model::KitchenHomeUI);
            screenClick(ClickEvent::RELEASED, 30, 30);
            break;
        }
        break;

    default:
        break;
    }
    autoDemoCounter++;
}

void OvenScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    OvenScreenView::handleClickEvent(evt);
}

void OvenScreenView::screenDrag(DragEvent::DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY)
{
    DragEvent evt(type, oldX, oldY, newX, newY);
    OvenScreenView::handleDragEvent(evt);
}

void OvenScreenView::screenGesture(GestureEvent::GestureType type, int16_t v, int16_t x_coord, int16_t y_coord)
{
    GestureEvent evt(type, v, x_coord, y_coord);
    OvenScreenView::handleGestureEvent(evt);
}
