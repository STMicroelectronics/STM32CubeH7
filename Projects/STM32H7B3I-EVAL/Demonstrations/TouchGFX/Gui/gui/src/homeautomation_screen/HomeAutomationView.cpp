#include <gui/homeautomation_screen/HomeAutomationView.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <gui/common/Utils.hpp>
#include <texts/TextKeysAndLanguages.hpp>

HomeAutomationView::HomeAutomationView() :
    currentScreenState(MAIN),
    mainScreenButtonMoveAnimationEndedCallback(this, &HomeAutomationView::mainScreenButtonMoveAnimationEndedHandler),
    buttonClickedCallback(this, &HomeAutomationView::mainScreenButtonClicked),
    successCallback(this, &HomeAutomationView::onSuccess),
    failureCallback(this, &HomeAutomationView::onFailure),
    loginWidgetAnimEndedCallback(this, &HomeAutomationView::loginWidgetAnimEndedCallbackHandler)
{
    CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
}

void HomeAutomationView::setupScreen()
{
    //Login widget setup
    solution.add(0);
    solution.add(3);
    solution.add(6);
    solution.add(7);

    loginWidget.setXY(touchgfx::HAL::DISPLAY_WIDTH, 81);
    loginWidget.setSuccessCallback(successCallback);
    loginWidget.setFailureCallback(failureCallback);
    loginWidget.setSolution(solution);
    loginWidget.setBitmapsAndHitBoxes(BITMAP_LOGIN_NODE_00_ID, BITMAP_LOGIN_NODE_28_ID, 80, 80);
    loginWidget.setMoveAnimationEndedAction(loginWidgetAnimEndedCallback);
    loginWidget.invalidate();

    add(loginWidget);

    loginWidgetCloseArea.setPosition(0, 84, HAL::DISPLAY_WIDTH - loginWidget.getWidth(), loginWidget.getHeight());
    loginWidgetCloseArea.setAction(buttonClickedCallback);
    loginWidgetCloseArea.setVisible(false);
    add(loginWidgetCloseArea);

    mainScreenButtonContainer.setPosition(64, 192, HAL::DISPLAY_WIDTH - 120, 200);
    add(mainScreenButtonContainer);
    mainScreenButton[0].setBitmaps(Bitmap(BITMAP_MAIN_BUTTON_LIGHT_ID), Bitmap(BITMAP_MAIN_BUTTON_LIGHT_PRESSED_ID));
    mainScreenButton[1].setBitmaps(Bitmap(BITMAP_MAIN_BUTTON_BLINDS_ID), Bitmap(BITMAP_MAIN_BUTTON_BLINDS_PRESSED_ID));
    mainScreenButton[2].setBitmaps(Bitmap(BITMAP_MAIN_BUTTON_SECURITY_ID), Bitmap(BITMAP_MAIN_BUTTON_SECURITY_PRESSED_ID));
    mainScreenButton[3].setBitmaps(Bitmap(BITMAP_MAIN_BUTTON_STATISTICS_ID), Bitmap(BITMAP_MAIN_BUTTON_STATISTICS_PRESSED_ID));

    for (int i = 0; i < NUMBER_OF_MAIN_SCREEN_BUTTONS; i++)
    {
        mainScreenButton[i].setXY(getMainButtonStartX(i), 0);
        mainScreenButton[i].setAction(buttonClickedCallback);
        mainScreenButton[i].setMoveAnimationEndedAction(mainScreenButtonMoveAnimationEndedCallback);
        mainScreenButtonContainer.add(mainScreenButton[i]);
    }

    roomControlToggle.setXY(0, 84);
    roomControlToggle.setVisible(false);
    add(roomControlToggle);

    roomControlAdjustable.setXY(0, 84);
    roomControlAdjustable.setVisible(false);
    add(roomControlAdjustable);

    statisticsContainer.setXY(0, 80);
    statisticsContainer.setVisible(false);
    add(statisticsContainer);

    for (int i = 0; i < RoomControl::NUMBER_OF_ROOMS; i++)
    {
        lightSelected[i] = (Utils::randomNumberBetween(0, 100) > 50);
        lockSelected[i] = (Utils::randomNumberBetween(0, 100) > 50);
    }

    updateHeadline();

    //Login
    resetLogin();
    failedAttempts = 0;
}

void HomeAutomationView::tearDownScreen()
{
}

void HomeAutomationView::mainScreenButtonClicked(const AbstractButton& source)
{
    if (&source == &mainScreenButton[0])
    {
        currentScreenState = LIGHT;
        animateMainMenuButtonToCenter(0);
    }
    else if (&source == &mainScreenButton[1])
    {
        currentScreenState = BLINDS;
        animateMainMenuButtonToCenter(1);
    }
    else if (&source == &mainScreenButton[2])
    {
        loginWidgetCloseArea.setVisible(false);
        currentScreenState = LOCK;
        toggleLoginWidget(true);
        activateMainButtons(false);
    }
    else if (&source == &mainScreenButton[3])
    {
        loginWidgetCloseArea.setVisible(false);
        currentScreenState = STATISTICS;
        toggleLoginWidget(true);
        activateMainButtons(false);
    }
    else if (&source == &loginWidgetCloseArea)
    {
        activateMainButtons(false);
        loginWidgetCloseArea.setVisible(false);
        currentScreenState = MAIN;
        toggleLoginWidget(false);
    }
}

void HomeAutomationView::backButtonHandler()
{
    animateToMainMenuButton();
}

void HomeAutomationView::activateMainButtons(bool activate)
{
    for (int i = 0; i < NUMBER_OF_MAIN_SCREEN_BUTTONS; i++)
    {
        mainScreenButton[i].setTouchable(activate);
    }
}

void HomeAutomationView::toggleLoginWidget(bool enable)
{
    if (enable)
    {
        loginSuccess = false;
    }

    int duration = 15;
    int delay = (enable) ? 0 : 30;
    int deltaX = (enable) ? -loginWidget.getWidth() : loginWidget.getWidth();

    mainScreenButtonContainer.setMoveAnimationDelay(delay);
    mainScreenButtonContainer.startMoveAnimation(mainScreenButtonContainer.getX() + deltaX, mainScreenButtonContainer.getY(), duration, EasingEquations::cubicEaseInOut);

    loginWidget.setMoveAnimationDelay(delay);
    loginWidget.startMoveAnimation(loginWidget.getX() + deltaX, loginWidget.getY(), duration, EasingEquations::cubicEaseInOut);

    for (int i = 0; i < NUMBER_OF_MAIN_SCREEN_BUTTONS; i++)
    {
        mainScreenButton[i].setFadeAnimationDelay(delay);
        mainScreenButton[i].startFadeAnimation((enable) ? 100 : 255, duration, EasingEquations::cubicEaseInOut);
    }

    //Clear trace
    if (!enable)
    {
        resetLogin();
    }
}

void HomeAutomationView::mainScreenButtonMoveAnimationEndedHandler(const MoveAnimator<FadeAnimator<Button> >& src)
{
    updateHeadline();

    for (int i = 0; i < NUMBER_OF_MAIN_SCREEN_BUTTONS; i++)
    {
        if (&src == &(mainScreenButton[i]))
        {
            int sizeIncrease = (currentScreenState == MAIN) ? 40 : -40;
            mainScreenButton[i].invalidate();
            mainScreenButton[i].setHeight(mainScreenButton[i].getHeight() + sizeIncrease);
            mainScreenButton[i].invalidate();
            break;
        }
    }

    if (currentScreenState == MAIN)
    {
        // Animated back to MAIN

        activateMainButtons(true);
        showBackButton(false);
    }
    else
    {
        if (currentScreenState == LIGHT)
        {
            roomControlToggle.setup(BITMAP_HOME_ROOM_SELECTED_LIGHT_ID, lightSelected);
            roomControlToggle.setVisible(true);
            roomControlToggle.invalidate();

            showBackButton(true);
        }
        else if (currentScreenState == BLINDS)
        {
            roomControlAdjustable.setVisible(true);
            roomControlAdjustable.invalidate();

            showBackButton(true);
        }
        else if (currentScreenState == LOCK)
        {
            roomControlToggle.setup(BITMAP_HOME_ROOM_SELECTED_LOCK_ID, lockSelected);
            roomControlToggle.setVisible(true);
            roomControlToggle.invalidate();

            showBackButton(true);
        }
        else if (currentScreenState == STATISTICS)
        {
            statisticsContainer.setup();
            statisticsContainer.setVisible(true);
            statisticsContainer.invalidate();

            showBackButton(true);
        }
    }
}

void HomeAutomationView::animateMainMenuButtonToCenter(int index)
{
    for (int i = 0; i < NUMBER_OF_MAIN_SCREEN_BUTTONS; i++)
    {
        if (i == index)
        {
            mainScreenButton[i].setMoveAnimationDelay(12);
            mainScreenButton[i].startMoveAnimation(328 - mainScreenButtonContainer.getX(), mainScreenButton[i].getY(), 12, EasingEquations::cubicEaseInOut);
        }
        else
        {
            mainScreenButton[i].setFadeAnimationDelay(0);
            mainScreenButton[i].startFadeAnimation(0, 14, EasingEquations::cubicEaseIn);
        }
    }
    activateMainButtons(false);
}

void HomeAutomationView::animateToMainMenuButton()
{
    int index = -1;

    switch (currentScreenState)
    {
    case HomeAutomationView::MAIN:
        break;
    case HomeAutomationView::LIGHT:
        index = 0;
        roomControlToggle.setVisible(false);
        roomControlToggle.invalidate();
        break;
    case HomeAutomationView::BLINDS:
        index = 1;
        roomControlAdjustable.setVisible(false);
        roomControlAdjustable.invalidate();
        break;
    case HomeAutomationView::LOCK:
        index = 2;
        roomControlToggle.setVisible(false);
        roomControlToggle.invalidate();
        break;
    case HomeAutomationView::STATISTICS:
        index = 3;
        statisticsContainer.setVisible(false);
        statisticsContainer.invalidate();
        break;
    case HomeAutomationView::NO_STATE:
        break;
    default:
        break;
    }

    for (int i = 0; i < NUMBER_OF_MAIN_SCREEN_BUTTONS; i++)
    {
        if (i == index)
        {
            mainScreenButton[i].setMoveAnimationDelay(0);
            mainScreenButton[i].startMoveAnimation(getMainButtonStartX(i), mainScreenButton[i].getY(), 12, EasingEquations::cubicEaseInOut);
        }
        else
        {
            mainScreenButton[i].setFadeAnimationDelay(0);
            mainScreenButton[i].startFadeAnimation(255, 14, EasingEquations::cubicEaseIn);
        }
    }

    currentScreenState = MAIN;
}

void HomeAutomationView::showBackButton(bool show)
{
    gotoMenuButton.setVisible(!show);
    gotoMenuButton.invalidate();

    backButton.setVisible(show);
    backButton.invalidate();
}

int HomeAutomationView::getMainButtonStartX(int index)
{
    return (mainScreenButton[0].getWidth() + 30) * index;
}

void HomeAutomationView::onSuccess()
{
    loginSuccess = true;

    resetLogin();

    toggleLoginWidget(false);
}

void HomeAutomationView::onFailure()
{
    loginSuccess = false;

    //If failed attempts = X, display part of the solution. Keep adding more for every failure)
    if (counter < solution.size())
    {
        trace.add(solution[counter]);
        counter++;
    }

    //Always show trace
    loginWidget.showTrace(trace);
}

void HomeAutomationView::loginWidgetAnimEndedCallbackHandler(const MoveAnimator<LoginWidget>& widget)
{
    bool loginWidgetVisible = (loginWidget.getX() < HAL::DISPLAY_WIDTH);

    //reset all input
    loginWidget.reset();

    loginWidgetCloseArea.setVisible(loginWidgetVisible);
    activateMainButtons(!loginWidgetVisible);

    if (loginSuccess)
    {
        if (currentScreenState == LOCK)
        {
            currentScreenState = LOCK;
            animateMainMenuButtonToCenter(2);
        }
        else
        {
            currentScreenState = STATISTICS;
            animateMainMenuButtonToCenter(3);
        }
    }
}

void HomeAutomationView::updateHeadline()
{
    switch (currentScreenState)
    {
    case HomeAutomationView::MAIN:
        headline.setTypedText(TypedText(T_HOME_HEADLINE_00));
        break;
    case HomeAutomationView::LIGHT:
        headline.setTypedText(TypedText(T_HOME_HEADLINE_01));
        break;
    case HomeAutomationView::BLINDS:
        headline.setTypedText(TypedText(T_HOME_HEADLINE_02));
        break;
    case HomeAutomationView::LOCK:
        headline.setTypedText(TypedText(T_HOME_HEADLINE_03));
        break;
    case HomeAutomationView::STATISTICS:
        headline.setTypedText(TypedText(T_HOME_HEADLINE_04));
        break;
    case HomeAutomationView::NO_STATE:
        break;
    default:
        break;
    }

    headline.invalidate();
}

void HomeAutomationView::resetLogin()
{
    failedAttempts = 0;
    counter = 1;
    trace.clear();
    trace.add(solution[0]); //Always show the first element
}

void HomeAutomationView::mcuLoadUpdated(uint8_t newMcuLoad)
{
    mcuLoad.updateProcessorLoad(newMcuLoad);
}

void HomeAutomationView::screenSaverMinorTick()
{
    application().gotoMainMenuCarouselScreenNoTransition();
}
