#include <gui/homecontrolscreen_screen/HomeControlScreenView.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

#include <gui/common/Defines.hpp>

HomeControlScreenView::HomeControlScreenView() :    
    currentMenu(ROOM_SELECTOR_TEMPERATURE),
    splashContainerClickedCallback(this, &HomeControlScreenView::splashContainerClickedHandler),
    animationEnddedCallback(this, &HomeControlScreenView::animationEnddedHandler),
    scrollWheelRoomSelectorAnimateToCallback(this, &HomeControlScreenView::scrollWheelRoomSelectorAnimateToHandler),
    textFadeEndedCallback(this, &HomeControlScreenView::textFadeEndedHandler),
    imageFadeCallback(this, &HomeControlScreenView::imageFadeHandler),
    lastItemSelected(0),
    selectedItemDragged(false),
    totalDrag(0),
    changeToRecipe(false),
    autoDemoCounter(0)
{
}

void HomeControlScreenView::setupScreen()
{
    HAL::getInstance()->setFrameRateCompensation(true);

    HomeControlScreenViewBase::setupScreen();
    homeUISplashContainer.setClickAction(splashContainerClickedCallback);

    scrollWheelRoomSelector.setAnimationEndedCallback(animationEnddedCallback);
    scrollWheelRoomSelector.setAnimateToCallback(scrollWheelRoomSelectorAnimateToCallback);
    scrollWheelRoomSelector.setAnimationSteps(HOME_UI_SCROLL_WHEEL_STEPS);

    scrollWheelRoomSelector.setAnimationSteps(20);
    scrollWheelRoomSelector.setTouchable(false);
    switchWheelItemsContent(false);

    selectedItemDragged = true;

    glowTemp.setFadeAnimationEndedAction(imageFadeCallback);   

    textAreaHomeUIHeadline.setFadeAnimationEndedAction(textFadeEndedCallback);
    scrollWheelRoomInfo.setTouchable(false);

    lastItemSelected = scrollWheelRoomSelector.getSelectedItem();    

    mcuLoad.initialize();
    mcuLoad.setMCUOnly(true);
    mcuLoad.setTextColor(Color::getColorFrom24BitRGB(52, 75, 101));
}

void HomeControlScreenView::tearDownScreen()
{
    if (!changeToRecipe)
    {
        HAL::getInstance()->setFrameRateCompensation(false);
    }
    HomeControlScreenViewBase::tearDownScreen();
}

void HomeControlScreenView::setSplashScreenState(bool on)
{
    if (on)
    {
        homeUISplashContainer.setVisible(false);

        buttonPreviousRoom.setAlpha(255);
        textAreaHomeUIHeadline.setAlpha(255);
        textAreaDate.setAlpha(255);
        textAreaTime.setAlpha(255);

        fadeBottombar(255, 0, 0);
        fadeScrollWheelRoomSelector(255, 0, 0);
        scrollWheelRoomSelector.setTouchable(true);

        presenter->setFromRecipeScreen(false);
    }
}

void HomeControlScreenView::setHomeUIRoom(bool isBathroom)
{
    if (isBathroom)
    {
        homeUISplashContainer.setHeadline(TypedText(T_HOMEUISPLASHBATHROOM));
        homeUISplashContainer.setTemp(73);
        homeUISplashContainer.setTempActionText(TypedText(T_HOMEUISPLASHBOTTOMLINEHEATINGUP));
        scrollWheelRoomSelector.animateToItem(1, 0);
        buttonMusicRecipes.setBitmaps(Bitmap(BITMAP_MUSIC_ID), Bitmap(BITMAP_MUSIC_ID));
        buttonPreviousRoom.setBitmaps(Bitmap(BITMAP_GO_BACK_BUTTON_BATH_ID), Bitmap(BITMAP_GO_BACK_BUTTON_BATH_ID));
    }
    else
    {
        homeUISplashContainer.setHeadline(TypedText(T_HOMEUISPLASHKITCHEN));
        homeUISplashContainer.setTemp(70);
        homeUISplashContainer.setTempActionText(TypedText(T_HOMEUISPLASHBOTTOMLINEIDLE));
        scrollWheelRoomSelector.animateToItem(0, 0);
        buttonMusicRecipes.setBitmaps(Bitmap(BITMAP_RECIPES_ID), Bitmap(BITMAP_RECIPES_ID));
        buttonPreviousRoom.setBitmaps(Bitmap(BITMAP_GO_BACK_KITCHEN_ID), Bitmap(BITMAP_GO_BACK_KITCHEN_ID));
    }
    animationEnddedHandler();
}

void HomeControlScreenView::switchWheelItemsContent(bool devices)
{
    for (int cnt = 0; scrollWheelRoomSelectorListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        scrollWheelRoomSelectorListItems[cnt].setContent(devices);
    }

    for (int cnt = 0; scrollWheelRoomSelectorSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        scrollWheelRoomSelectorSelectedListItems[cnt].setContent(devices);
    }
}

void HomeControlScreenView::radioDevicesSelected()
{
    switch (currentMenu)
    {
    case ROOM_SELECTOR_TEMPERATURE:
        fadeScrollWheelRoomSelector(0, HOME_UI_FADE_DURATION);
        textAreaHomeUIHeadline.setFadeAnimationDelay(0);
        textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);
        currentMenu = ROOM_SELECTOR_DEVICES;
        break;

    case ROOM_INFO:
        scrollWheelRoomSelector.animateToItem(lastItemSelected, 0);
        textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);

        buttonPreviousRoom.setFadeAnimationDelay(HOME_UI_FADE_DELAY);
        buttonPreviousRoom.startFadeAnimation(255, HOME_UI_FADE_DURATION);

        switchRoomLeft.setFadeAnimationDelay(0);
        switchRoomRight.setFadeAnimationDelay(0);
        switchRoomLeft.startFadeAnimation(0, HOME_UI_FADE_DURATION);
        switchRoomRight.startFadeAnimation(0, HOME_UI_FADE_DURATION);

        buttonBack.setFadeAnimationDelay(0);
        buttonBack.startFadeAnimation(0, HOME_UI_FADE_DURATION);

        scrollWheelRoomInfoListItems[0].startFadeAnimation(0, HOME_UI_FADE_DURATION);
        scrollWheelRoomInfoListItems[1].startFadeAnimation(0, HOME_UI_FADE_DURATION);

        currentMenu = ROOM_SELECTOR_DEVICES;
        break;

    default:
        break;
    }
}

void HomeControlScreenView::radioTemperatureSelected()
{
    switch (currentMenu)
    {
    case ROOM_SELECTOR_DEVICES:
        fadeScrollWheelRoomSelector(0, HOME_UI_FADE_DURATION);
        textAreaHomeUIHeadline.setFadeAnimationDelay(0);
        textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);
        currentMenu = ROOM_SELECTOR_TEMPERATURE;

        break;
    default:
        break;
    }
}

void HomeControlScreenView::goToPrevRoom()
{
    if (presenter->getBathroomHomeUI())
    {
        application().gotoBathroomScreenScreenNoTransition();
    }
    else
    {
        application().gotoKitchenScreenScreenNoTransition();
    }
}

void HomeControlScreenView::buttonMuiscRecipesPressed()
{
    if (presenter->getBathroomHomeUI())
    {
        if (!glowTemp.isFadeAnimationRunning())
        {
            glowDevices.startFadeAnimation(255, HOME_UI_GLOW_FADE);
            glowTemp.startFadeAnimation(255, HOME_UI_GLOW_FADE);
        }
        fadeCounter = 0;
    }
    else
    {
        changeToRecipe = true;
        application().gotoRecipeScreenScreenCoverTransitionSouth();
    }
}

void HomeControlScreenView::buttonPersPressed()
{
    if (!glowTemp.isFadeAnimationRunning())
    {
        if (!presenter->getBathroomHomeUI())
        {
            glowRecipe.startFadeAnimation(255, HOME_UI_GLOW_FADE);
        }
        glowDevices.startFadeAnimation(255, HOME_UI_GLOW_FADE);
        glowTemp.startFadeAnimation(255, HOME_UI_GLOW_FADE);
    }
    fadeCounter = 0;
}

void HomeControlScreenView::roomInfoRightClicked()
{
    if (!textAreaHomeUIHeadline.isFadeAnimationRunning())
    {
        switch (currentMenu)
        {
        case ROOM_INFO:
            if (!scrollWheelRoomInfo.isAnimating())
            {
                scrollWheelRoomInfo.animateToItem((lastItemSelected + 1), HOME_UI_ROOMS_MOVE_SPEED);
            }
            break;
        case ROOM_DEVICES:
            if (scrollListKitchenDevices.getX() == containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2)
            {
                scrollListBathroomDevices.setX(HAL::DISPLAY_WIDTH - containerDevicesWheel.getX());

                scrollListKitchenDevices.startMoveAnimation(-scrollListKitchenDevices.getWidth() - containerDevicesWheel.getX(), scrollListKitchenDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
                scrollListBathroomDevices.startMoveAnimation(containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2, scrollListBathroomDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
            }
            else
            {
                scrollListKitchenDevices.setX(HAL::DISPLAY_WIDTH - containerDevicesWheel.getX());

                scrollListBathroomDevices.startMoveAnimation(-scrollListBathroomDevices.getWidth() - containerDevicesWheel.getX(), scrollListBathroomDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
                scrollListKitchenDevices.startMoveAnimation(containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2, scrollListKitchenDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
            }
            break;
        default:
            break;
        }
        lastItemSelected = (lastItemSelected) ? 0 : 1;
        textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);
    }
}

void HomeControlScreenView::roomInfoLeftClicked()
{
    switch (currentMenu)
    {
    case ROOM_INFO:
        if (!scrollWheelRoomInfo.isAnimating())
        {
            scrollWheelRoomInfo.animateToItem((lastItemSelected - 1), HOME_UI_ROOMS_MOVE_SPEED);

            textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);

            lastItemSelected = (lastItemSelected) ? 0 : 1;
            textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);
        }
        break;
    case ROOM_DEVICES:
        if (!scrollListBathroomDevices.isMoveAnimationRunning() && !scrollListKitchenDevices.isMoveAnimationRunning())
        {
            if (scrollListKitchenDevices.getX() == containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2)
            {
                scrollListBathroomDevices.setX(-scrollListBathroomDevices.getWidth() - containerDevicesWheel.getX());

                scrollListKitchenDevices.startMoveAnimation(HAL::DISPLAY_WIDTH - containerDevicesWheel.getX(), scrollListKitchenDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
                scrollListBathroomDevices.startMoveAnimation(containerDevicesWheel.getWidth() / 2 - scrollListBathroomDevices.getWidth() / 2, scrollListBathroomDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
            }
            else
            {
                scrollListKitchenDevices.setX(-scrollListKitchenDevices.getWidth() - containerDevicesWheel.getX());

                scrollListBathroomDevices.startMoveAnimation(HAL::DISPLAY_WIDTH - containerDevicesWheel.getX(), scrollListBathroomDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
                scrollListKitchenDevices.startMoveAnimation(containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2, scrollListKitchenDevices.getY(), HOME_UI_DEVICES_MOVE_SPEED, EasingEquations::cubicEaseOut);
            }

            lastItemSelected = (lastItemSelected) ? 0 : 1;
            textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);
        }
        break;
    default:
        break;
    }
}

void HomeControlScreenView::backButtonPressed()
{
    selectedItemDragged = true;
    scrollWheelRoomSelector.animateToItem(lastItemSelected, 0);
    textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);

    buttonPreviousRoom.setFadeAnimationDelay(HOME_UI_FADE_DELAY);
    buttonPreviousRoom.startFadeAnimation(255, HOME_UI_FADE_DURATION);

    switchRoomLeft.setFadeAnimationDelay(0);
    switchRoomRight.setFadeAnimationDelay(0);
    switchRoomLeft.startFadeAnimation(0, HOME_UI_FADE_DURATION);
    switchRoomRight.startFadeAnimation(0, HOME_UI_FADE_DURATION);

    buttonBack.setFadeAnimationDelay(0);
    buttonBack.startFadeAnimation(0, HOME_UI_FADE_DURATION);

    switch (currentMenu)
    {
    case ROOM_INFO:

        scrollWheelRoomInfoListItems[0].startFadeAnimation(0, HOME_UI_FADE_DURATION);
        scrollWheelRoomInfoListItems[1].startFadeAnimation(0, HOME_UI_FADE_DURATION);

        currentMenu = ROOM_SELECTOR_TEMPERATURE;

        break;
    case ROOM_DEVICES:

        if (scrollListKitchenDevices.getX() == containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2)
        {
            fadeScrollListKitchenDevices(0, HOME_UI_FADE_DURATION);
            setAlphaScrollListBathroomDevices(0);
        }
        else
        {
            fadeScrollListBathroomDevices(0, HOME_UI_FADE_DURATION);
            setAlphaScrollListKitchenDevices(0);
        }

        fadeBottombar(255, HOME_UI_FADE_DURATION, HOME_UI_FADE_DELAY);

        currentMenu = ROOM_SELECTOR_DEVICES;
    default:
        break;
    }
}

void HomeControlScreenView::scrollWheelRoomSelectorUpdateItem(HomeUIWheelElement& item, int16_t itemIndex)
{
    item.setRoomInfo(itemIndex);
}

void HomeControlScreenView::scrollWheelRoomSelectorUpdateCenterItem(HomeUIWheelElement& item, int16_t itemIndex)
{
    item.setRoomInfo(itemIndex);
}

void HomeControlScreenView::scrollWheelRoomInfoUpdateItem(RoomInfoWheelElements& item, int16_t itemIndex)
{
    item.updateValues(itemIndex);
}

void HomeControlScreenView::scrollListBathroomDevicesUpdateItem(DeviceInfoWheelElements& item, int16_t itemIndex)
{
    Unicode::UnicodeChar textBuffer[20];
    Unicode::UnicodeChar textBufferValue[20];

    switch (itemIndex)
    {
    case BATHTUB_UI:
        Unicode::strncpy(textBuffer, "WATER TEMPERATURE", 20);
        if (presenter->getBathtubOn())
        {
            int tempOffset = presenter->getTempCelcius() ? 20 : 70;
            Unicode::snprintf(textBufferValue, 20, "%d °%s", presenter->getTempValue() + tempOffset, presenter->getTempCelcius() ? "C" : "F");            
        }
        else
        {
            Unicode::snprintf(textBufferValue, 20, "OFF");
        }
        item.updateTextLine1(true, textBuffer, textBufferValue);

        Unicode::strncpy(textBuffer, "JET SPEED", 20);
        if (presenter->getBathtubOn())
        {
            Unicode::snprintf(textBufferValue, 20, "LEVEL %d", presenter->getJetOn() ? presenter->getJetLevel() + 1 : 0);
        }
        else
        {
            Unicode::snprintf(textBufferValue, 20, "OFF");
        }
        item.updateTextLine2(true, textBuffer, textBufferValue);

        Unicode::strncpy(textBuffer, "LIGHT CONTROL", 20);
        if (presenter->getBathtubOn())
        {
            Unicode::snprintf(textBufferValue, 20, presenter->getLightOn() ? "ON" : "OFF");
        }
        else
        {
            Unicode::snprintf(textBufferValue, 20, "OFF");
        }        
        item.updateTextLine3(true, textBuffer, textBufferValue);
        break;

    case WASHER_UI:
        Unicode::strncpy(textBuffer, "PROGRAM", 20);

        switch (presenter->getWashingProgram())
        {
        case PROGRAM_ACTIVE_WEAR:
            Unicode::strncpy(textBufferValue, "ACTIVE WEAR", 20);
            break;
        case PROGRAM_DELICATES:
            Unicode::strncpy(textBufferValue, "DELICATES", 20);
            break;
        case PROGRAM_ECO_WASH:
            Unicode::strncpy(textBufferValue, "ECO WASH", 20);
            break;
        case PROGRAM_HEAVY_DUTY:
            Unicode::strncpy(textBufferValue, "HEAVY DUTY", 20);
            break;
        case PROGRAM_NORMAL:
            Unicode::strncpy(textBufferValue, "NORMAL", 20);
            break;
        case PROGRAM_QUICK_WASH:
            Unicode::strncpy(textBufferValue, "QUICK WASH", 20);
            break;
        case PROGRAM_SMART_WASH:
            Unicode::strncpy(textBufferValue, "SMART WASH", 20);
            break;
        default:
            break;
        }
        item.updateTextLine1(true, textBuffer, textBufferValue);

        Unicode::strncpy(textBuffer, "TIMER", 20);
        Unicode::snprintf(textBufferValue, 20, "%d : %02d", presenter->getWashingTime() / 60, presenter->getWashingTime() % 60);
        item.updateTextLine2(true, textBuffer, textBufferValue);
        item.updateTextLine3(false, textBuffer, textBufferValue);
        break;

    case HOME_UI_BATHROOM:
        Unicode::strncpy(textBuffer, "AIRCON TEMPERATURE", 20);
        Unicode::snprintf(textBufferValue, 20, "%d °F", 64);
        item.updateTextLine1(true, textBuffer, textBufferValue);

        Unicode::strncpy(textBuffer, "ROOM TEMPERATURE", 20);
        Unicode::snprintf(textBufferValue, 20, "%d °F", 73);
        item.updateTextLine2(true, textBuffer, textBufferValue);

        Unicode::strncpy(textBuffer, "SCHEDULE", 20);
        Unicode::snprintf(textBufferValue, 20, "6:00 AM TO 10:00 PM");
        item.updateTextLine3(true, textBuffer, textBufferValue);
        break;

    }
    item.updateDeviceInfo(itemIndex, false);
}

void HomeControlScreenView::scrollListKitchenDevicesUpdateItem(DeviceInfoWheelElements& item, int16_t itemIndex)
{
    Unicode::UnicodeChar textBuffer[20];
    Unicode::UnicodeChar textBufferValue[20];

    switch (itemIndex)
    {
    case OVEN_UI:
        Unicode::strncpy(textBuffer, "COOKING MODE", 20);
        if (presenter->getOvenState())
        {
            switch (presenter->getSelectedOvenMode())
            {
            case OVEN_MODE_LIGHTS:
                Unicode::snprintf(textBufferValue, 20, "LIGHTS");
                break;
            case OVEN_MODE_OVEN_TOP:
                Unicode::snprintf(textBufferValue, 20, "TOP HEAT");
                break;
            case OVEN_MODE_OVEN_BUTTOM:
                Unicode::snprintf(textBufferValue, 20, "BUTTOM HEAT");
                break;
            case OVEN_MODE_FAN:
                Unicode::snprintf(textBufferValue, 20, "FAN");
                break;
            case OVEN_MODE_GRILL:
                Unicode::snprintf(textBufferValue, 20, "GRILL");
                break;
            case OVEN_MODE_GRILL_FAN:
                Unicode::snprintf(textBufferValue, 20, "GRILL + FAN");
                break;
            case OVEN_MODE_CLEAN:
                Unicode::snprintf(textBufferValue, 20, "CLEAN");
                break;
            default:
                break;

            }
        }
        else
        {
            Unicode::snprintf(textBufferValue, 20, "OFF");
        }
        item.updateTextLine1(true, textBuffer, textBufferValue);

        Unicode::snprintf(textBuffer, 20, "TEMPERATURE");
        Unicode::snprintf(textBufferValue, 20, "%d °C", presenter->getOvenTemperature());
        item.updateTextLine2(true, textBuffer, textBufferValue);

        Unicode::snprintf(textBuffer, 20, "TIMER");
        Unicode::snprintf(textBufferValue, 20, "%dH %dM", presenter->getOvenTimerHour(), presenter->getOvenTimerMinute());
        item.updateTextLine3(true, textBuffer, textBufferValue);
        break;

    case HOME_TEMPERATURE_UI:
        Unicode::snprintf(textBuffer, 20, "AIRCON TEMPERATURE");
        Unicode::snprintf(textBufferValue, 20, "%d °F", 70);
        item.updateTextLine1(true, textBuffer, textBufferValue);

        Unicode::snprintf(textBuffer, 20, "ROOM TEMPERATURE");
        Unicode::snprintf(textBufferValue, 20, "%d °F", 70);
        item.updateTextLine2(true, textBuffer, textBufferValue);

        Unicode::snprintf(textBuffer, 20, "SCHEDULE");
        Unicode::snprintf(textBufferValue, 20, "6:00 AM TO 10:00 PM");
        item.updateTextLine3(true, textBuffer, textBufferValue);
        break;

    case HOOD_UI:
        Unicode::snprintf(textBuffer, 20, "FAN SPEED");
        if (presenter->getFanState())
        {
            Unicode::snprintf(textBufferValue, 20, "LEVEL %d", (presenter->getFanSpeed() + 1));
        }
        else
        {
            Unicode::snprintf(textBufferValue, 20, "OFF");
        }
        item.updateTextLine1(true, textBuffer, textBufferValue);

        Unicode::snprintf(textBuffer, 20, "LIGHTS");
        Unicode::snprintf(textBufferValue, 20, presenter->getLightState() ? "ON" : "OFF");
        item.updateTextLine2(true, textBuffer, textBufferValue);

        item.updateTextLine3(false, textBuffer, textBuffer);
        break;

    case HOME_RECIPE_UI:
        Unicode::snprintf(textBuffer, 20, "FOOD RECIPES");
        Unicode::snprintf(textBufferValue, 20, presenter->getRecipeState() ? "ONLINE" : "OFFLINE");
        item.updateTextLine1(true, textBuffer, textBufferValue);

        item.updateTextLine2(false, textBuffer, textBuffer);
        item.updateTextLine3(false, textBuffer, textBuffer);
        break;
    default:
        break;

    }
    item.updateDeviceInfo(itemIndex, true);
}

void HomeControlScreenView::splashContainerClickedHandler(const HomeUISplashContainer& src, const ClickEvent& evt)
{
    if (!textAreaDate.isFadeAnimationRunning())
    {
        homeUISplashContainer.startFadeAnimation(0, HOME_UI_FADE_DURATION);
        homeUISplashContainer.setTouchable(false);

        buttonPreviousRoom.setFadeAnimationDelay(HOME_UI_FADE_DELAY);
        textAreaHomeUIHeadline.setFadeAnimationDelay(HOME_UI_FADE_DELAY);
        textAreaDate.setFadeAnimationDelay(HOME_UI_FADE_DELAY);
        textAreaTime.setFadeAnimationDelay(HOME_UI_FADE_DELAY);

        buttonPreviousRoom.startFadeAnimation(255, HOME_UI_FADE_DURATION);
        textAreaHomeUIHeadline.startFadeAnimation(255, HOME_UI_FADE_DURATION);
        textAreaDate.startFadeAnimation(255, HOME_UI_FADE_DURATION);
        textAreaTime.startFadeAnimation(255, HOME_UI_FADE_DURATION);

        fadeBottombar(255, HOME_UI_FADE_DURATION, HOME_UI_FADE_DELAY);
        fadeScrollWheelRoomSelector(255, HOME_UI_FADE_DURATION, HOME_UI_FADE_DELAY);
    }
}

void HomeControlScreenView::animationEnddedHandler()
{
    for (int cnt = 0; scrollWheelRoomSelectorSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        scrollWheelRoomSelectorSelectedListItems[cnt].setCenterItem(true);
    }
}

void HomeControlScreenView::scrollWheelRoomSelectorAnimateToHandler(int16_t itemSelected)
{
    // Since the value of itemSelected is the previous item +/- "steps" to the new item,
    // the value of the actual number of items and their index is calculated
    int selectedItem = (itemSelected % 5) < 0 ? 5 + (itemSelected % 5) : (itemSelected % 5);

    if (lastItemSelected != selectedItem)
    {
        for (int cnt = 0; scrollWheelRoomSelectorSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
        {
            scrollWheelRoomSelectorSelectedListItems[cnt].setCenterItem(false);
        }
    }
    else if ((selectedItem == 0 || selectedItem == 1) && !selectedItemDragged && !scrollWheelRoomSelector.isAnimating())
    {
        // if either the bathroom or the kitchen options is cliked, the
        // ui changes is elements to the devices og temperature info screeens
        textAreaHomeUIHeadline.setFadeAnimationDelay(0);
        textAreaHomeUIHeadline.startFadeAnimation(0, HOME_UI_FADE_DURATION);

        fadeScrollWheelRoomSelector(0, HOME_UI_FADE_DURATION);

        buttonPreviousRoom.setFadeAnimationDelay(0);
        buttonPreviousRoom.startFadeAnimation(0, HOME_UI_FADE_DURATION);

        switchRoomLeft.setAlpha(0);
        switchRoomRight.setAlpha(0);

        switchRoomLeft.setVisible(true);
        switchRoomRight.setVisible(true);

        switchRoomLeft.setFadeAnimationDelay(HOME_UI_FADE_DELAY);
        switchRoomRight.setFadeAnimationDelay(HOME_UI_FADE_DELAY);

        switchRoomLeft.startFadeAnimation(255, HOME_UI_FADE_DURATION);
        switchRoomRight.startFadeAnimation(255, HOME_UI_FADE_DURATION);

        buttonBack.setVisible(true);
        buttonBack.setFadeAnimationDelay(HOME_UI_FADE_DELAY);
        buttonBack.startFadeAnimation(255, HOME_UI_FADE_DURATION);

        scrollWheelRoomSelector.setTouchable(false);

        if (currentMenu == ROOM_SELECTOR_TEMPERATURE)
        {
            currentMenu = ROOM_INFO;

            scrollWheelRoomInfo.animateToItem(selectedItem, 0);

            scrollWheelRoomInfoListItems[0].setAlpha(0);
            scrollWheelRoomInfoListItems[1].setAlpha(0);

            scrollWheelRoomInfo.setVisible(true);

            if (scrollWheelRoomInfoListItems[0].getX() == 0)
            {
                scrollWheelRoomInfoListItems[0].startFadeAnimation(255, HOME_UI_FADE_DURATION, HOME_UI_FADE_DELAY);
                scrollWheelRoomInfoListItems[1].setAlpha(255);
            }
            else
            {
                scrollWheelRoomInfoListItems[0].setAlpha(255);
                scrollWheelRoomInfoListItems[1].startFadeAnimation(255, HOME_UI_FADE_DURATION, HOME_UI_FADE_DELAY);
            }

            switchRoomLeft.setY(66);
            switchRoomRight.setY(66);

        }

        else if (currentMenu == ROOM_SELECTOR_DEVICES)
        {
            currentMenu = ROOM_DEVICES;

            if (selectedItem == 0)
            {
                scrollListKitchenDevices.setX(containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2);
                scrollListBathroomDevices.setX(HAL::DISPLAY_WIDTH - containerDevicesWheel.getX());
            }
            else
            {
                scrollListBathroomDevices.setX(containerDevicesWheel.getWidth() / 2 - scrollListBathroomDevices.getWidth() / 2);
                scrollListKitchenDevices.setX(HAL::DISPLAY_WIDTH - containerDevicesWheel.getX());
            }

            containerDevicesWheel.setVisible(true);

            if (scrollListKitchenDevices.getX() == containerDevicesWheel.getWidth() / 2 - scrollListKitchenDevices.getWidth() / 2)
            {
                setAlphaScrollListBathroomDevices(255);
                setAlphaScrollListKitchenDevices(0);
                fadeScrollListKitchenDevices(255, HOME_UI_FADE_DURATION, HOME_UI_FADE_DELAY);
            }
            else
            {
                setAlphaScrollListKitchenDevices(255);
                setAlphaScrollListBathroomDevices(0);
                fadeScrollListBathroomDevices(255, HOME_UI_FADE_DURATION, HOME_UI_FADE_DELAY);
            }

            if (glowTemp.isFadeAnimationRunning())
            {
                if (!presenter->getBathroomHomeUI())
                {
                    glowRecipe.cancelFadeAnimation();
                    glowRecipe.setAlpha(0);
                    glowRecipe.invalidate();
                }
                glowDevices.cancelFadeAnimation();
                glowTemp.cancelFadeAnimation();
                glowDevices.setAlpha(0);
                glowTemp.setAlpha(0);
                glowDevices.invalidate();
                glowTemp.invalidate();
            }

            switchRoomLeft.setY(81);
            switchRoomRight.setY(81);

            radioButtonDevices.setTouchable(false);
            radioButtonTemperature.setTouchable(false);
            buttonPersonalize.setTouchable(false);
            buttonMusicRecipes.setTouchable(false);
            fadeBottombar(0, HOME_UI_FADE_DURATION);
        }
    }
    else
    {
        lastItemSelected = selectedItem;
    }

    lastItemSelected = selectedItem;
    selectedItemDragged = false;
}

void HomeControlScreenView::textFadeEndedHandler(const FadeAnimator<TextArea>& src)
{
    if (&src == &textAreaHomeUIHeadline)
    {
        if (textAreaHomeUIHeadline.getAlpha() == 0)
        {
            switch (currentMenu)
            {
            case ROOM_INFO:
                switch (lastItemSelected)
                {
                case 0:
                    textAreaHomeUIHeadline.setTypedText(TypedText(T_HOMEUIHEADLINEKITCHEN));
                    break;
                case 1:
                    textAreaHomeUIHeadline.setTypedText(TypedText(T_HOMEUIHEADLINEBATHROOM));
                    break;
                }
                break;

            case ROOM_DEVICES:
                imageTopGradient.setVisible(true);
                switch (lastItemSelected)
                {
                case 0:
                    textAreaHomeUIHeadline.setTypedText(TypedText(T_HOMEUIHEADLINEKITCHENDEVICES));
                    break;
                case 1:
                    textAreaHomeUIHeadline.setTypedText(TypedText(T_HOMEUIHEADLINEBATHROOMDEVICES));
                    break;
                }
                break;

            case ROOM_SELECTOR_TEMPERATURE:
                textAreaHomeUIHeadline.setTypedText(TypedText(T_HOMEUIHEADLINEROOMTEMPERATURES));

                switchRoomLeft.setVisible(false);
                switchRoomRight.setVisible(false);
                scrollWheelRoomInfo.setVisible(false);
                buttonBack.setVisible(false);
                switchWheelItemsContent(false);
                fadeScrollWheelRoomSelector(255, HOME_UI_FADE_DURATION);

                break;

            case ROOM_SELECTOR_DEVICES:
                textAreaHomeUIHeadline.setTypedText(TypedText(T_HOMEUIHEADLINEDEVICES));

                switchRoomLeft.setVisible(false);
                switchRoomRight.setVisible(false);
                containerDevicesWheel.setVisible(false);
                buttonBack.setVisible(false);
                imageTopGradient.setVisible(false);
                switchWheelItemsContent(true);
                fadeScrollWheelRoomSelector(255, HOME_UI_FADE_DURATION);

                break;
            default:
                break;
            }
            textAreaHomeUIHeadline.startFadeAnimation(255, HOME_UI_FADE_DURATION);
        }
        else
        {
            switch (currentMenu)
            {
            case ROOM_SELECTOR_DEVICES:
            case ROOM_SELECTOR_TEMPERATURE:
                scrollWheelRoomSelector.setTouchable(true);
                radioButtonDevices.setTouchable(true);
                radioButtonTemperature.setTouchable(true);
                buttonPersonalize.setTouchable(true);
                buttonMusicRecipes.setTouchable(true);
                break;
            default:
                break;
            }
        }
    }

}

void HomeControlScreenView::handleDragEvent(const DragEvent& evt)
{
    totalDrag += evt.getDeltaX();
    if(scrollWheelRoomSelector.isAnimating())
    {
        if (totalDrag > 6 || totalDrag < -6)
        {
            for (int cnt = 0; scrollWheelRoomSelectorSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
            {
                scrollWheelRoomSelectorSelectedListItems[cnt].setCenterItem(false); 
            }
        }
        HomeControlScreenViewBase::handleDragEvent(evt);
        selectedItemDragged = true;
    }
    else
    {
        HomeControlScreenViewBase::handleDragEvent(evt);
    }    
}

void HomeControlScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        totalDrag = 0;
    }
    HomeControlScreenViewBase::handleClickEvent(evt);
}

void HomeControlScreenView::fadeBottombar(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    radioButtonDevices.setFadeAnimationDelay(delay);
    radioButtonTemperature.setFadeAnimationDelay(delay);
    buttonMusicRecipes.setFadeAnimationDelay(delay);
    buttonPersonalize.setFadeAnimationDelay(delay);

    radioButtonDevices.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    radioButtonTemperature.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    buttonMusicRecipes.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    buttonPersonalize.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

void HomeControlScreenView::fadeScrollWheelRoomSelector(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    for (int cnt = 0; scrollWheelRoomSelectorListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        if (scrollWheelRoomSelectorListItems[cnt].getX() == 15 || scrollWheelRoomSelectorListItems[cnt].getX() == 28)
        {
            scrollWheelRoomSelectorListItems[cnt].fadeWidgets(endAlpha, duration, delay, alphaProgressionEquation);
        }
        else
        {
            scrollWheelRoomSelectorListItems[cnt].setAlpha(endAlpha);
        }
    }
    for (int cnt = 0; scrollWheelRoomSelectorSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        if (scrollWheelRoomSelectorSelectedListItems[cnt].getX() == 15)
        {
            scrollWheelRoomSelectorSelectedListItems[cnt].fadeWidgets(endAlpha, duration, delay, alphaProgressionEquation);
        }
        else
        {
            scrollWheelRoomSelectorSelectedListItems[cnt].setAlpha(endAlpha);
        }
    }
}

void HomeControlScreenView::fadeScrollListBathroomDevices(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    for (int cnt = 0; scrollListBathroomDevicesListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        scrollListBathroomDevicesListItems[cnt].startFadeAnimation(endAlpha, duration, delay, alphaProgressionEquation);
    }
}

void HomeControlScreenView::fadeScrollListKitchenDevices(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    for (int cnt = 0; scrollListKitchenDevicesListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        scrollListKitchenDevicesListItems[cnt].startFadeAnimation(endAlpha, duration, delay, alphaProgressionEquation);
    }
}

void HomeControlScreenView::setAlphaScrollListBathroomDevices(uint8_t alpha)
{
    for (int cnt = 0; scrollListBathroomDevicesListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        scrollListBathroomDevicesListItems[cnt].setAlpha(alpha);
    }
}

void HomeControlScreenView::setAlphaScrollListKitchenDevices(uint8_t alpha)
{
    for (int cnt = 0; scrollListKitchenDevicesListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        scrollListKitchenDevicesListItems[cnt].setAlpha(alpha);
    }
}

void HomeControlScreenView::imageFadeHandler(const FadeAnimator<Image>& src)
{
    if (&src == &glowTemp)
    {
        if ((fadeCounter < FADE_COUNT && !glowTemp.isFadeAnimationRunning()) || glowTemp.getAlpha() == 255)
        {
            if (glowTemp.getAlpha() == 255)
            {
                if (!presenter->getBathroomHomeUI())
                {
                    glowRecipe.startFadeAnimation(0, HOME_UI_GLOW_FADE);
                }
                glowDevices.startFadeAnimation(0, HOME_UI_GLOW_FADE);
                glowTemp.startFadeAnimation(0, HOME_UI_GLOW_FADE);
            }
            else
            {
                if (!presenter->getBathroomHomeUI())
                {
                    glowRecipe.startFadeAnimation(255, HOME_UI_GLOW_FADE);
                }
                glowDevices.startFadeAnimation(255, HOME_UI_GLOW_FADE);
                glowTemp.startFadeAnimation(255, HOME_UI_GLOW_FADE);
            }
            fadeCounter++;
        }
    }
}

void HomeControlScreenView::updateMCULoad(uint8_t load)
{
    mcuLoad.setMcuLoad(load);
}

void HomeControlScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::HomeUIRecipeSelector:
        switch (autoDemoCounter)
        {
        case 8:
            screenClick(ClickEvent::PRESSED, 47, 117);
            break;

        case 10:
            screenClick(ClickEvent::RELEASED, 47, 117);
            break;

        case 18:
            screenClick(ClickEvent::PRESSED, 420, 230);
            break;

        case 20:
            screenClick(ClickEvent::RELEASED, 420, 230);
            break;

        case 30:
            screenClick(ClickEvent::PRESSED, 300, 230);
            break;
        case 32:
            presenter->setAutoDemoState(Model::RecipeSelectorRecipe);
            screenClick(ClickEvent::RELEASED, 300, 230);
            break;
        }
        break;
    case Model::HomeUIKitchen:
        switch (autoDemoCounter)
        {
        case 10:
            screenClick(ClickEvent::PRESSED, 10, 10);
            break;

        case 12:
            presenter->setAutoDemoState(Model::KitchenHallway);
            screenClick(ClickEvent::RELEASED, 10, 10);
            break;
        }
        break;

    case Model::HomeUIBathroom:
        switch (autoDemoCounter)
        {
        case 8:
            screenClick(ClickEvent::PRESSED, 47, 117);
            break;

        case 10:
            screenClick(ClickEvent::RELEASED, 47, 117);
            break;

        case 16:
            scrollWheelRoomSelector.animateToItem(3, 100);
            break;

        case 30:
            scrollWheelRoomSelector.animateToItem(1, 100);

        case 46:
            screenClick(ClickEvent::PRESSED, 240, 140);
            break;

        case 48:
            screenClick(ClickEvent::RELEASED, 240, 140);
            break;

        case 60:
            screenClick(ClickEvent::PRESSED, 450, 140);
            break;

        case 62:
            screenClick(ClickEvent::RELEASED, 450, 140);
            break;

        case 68:
            screenClick(ClickEvent::PRESSED, 450, 140);
            break;

        case 70:
            screenClick(ClickEvent::RELEASED, 450, 140);
            break;

        case 78:
            screenClick(ClickEvent::PRESSED, 22, 140);
            break;

        case 80:
            screenClick(ClickEvent::RELEASED, 22, 140);
            break;

        case 88:
            screenClick(ClickEvent::PRESSED, 22, 22);
            break;

        case 90:
            screenClick(ClickEvent::RELEASED, 22, 22);
            break;

        case 100:
            screenClick(ClickEvent::PRESSED, 183, 232);
            break;

        case 102:
            screenClick(ClickEvent::RELEASED, 183, 232);
            break;

        case 120:
            screenClick(ClickEvent::PRESSED, 450, 140);
            break;

        case 122:
            screenClick(ClickEvent::RELEASED, 450, 140);
            break;

        case 130:
            screenClick(ClickEvent::PRESSED, 240, 140);
            break;

        case 132:
            screenClick(ClickEvent::RELEASED, 240, 140);
            break;

        case 144:
            scrollListBathroomDevices.animateToItem(2, 200);
            break;

        case 180:
            screenClick(ClickEvent::PRESSED, 450, 140);
            break;

        case 182:
            screenClick(ClickEvent::RELEASED, 450, 140);
            break;

        case 194:
            scrollListKitchenDevices.animateToItem(3, 200);
            break;

        case 214:
            screenClick(ClickEvent::PRESSED, 20, 20);
            break;

        case 216:
            screenClick(ClickEvent::RELEASED, 20, 20);
            break;

        case 230:
            screenClick(ClickEvent::PRESSED, 20, 20);
            break;

        case 232:
            presenter->setAutoDemoState(Model::BathroomHallway);
            screenClick(ClickEvent::RELEASED, 20, 20);
            break;
        }

    default:
        break;
    }
    autoDemoCounter++;
}

void HomeControlScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    View::handleClickEvent(evt);
}
