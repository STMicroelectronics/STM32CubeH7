#include <gui/mainmenuanimatingbuttons_screen/MainMenuAnimatingButtonsView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>

MainMenuAnimatingButtonsView::MainMenuAnimatingButtonsView() :
    currentAnimationState(NO_ANIMATION),
    startUpAnimationBoxDone(this, &MainMenuAnimatingButtonsView::StartupAnimationBoxDoneHandler),
    buttonAnimationDone(this, &MainMenuAnimatingButtonsView::buttonAnimationDoneHandler),
    buttonClicked(this, &MainMenuAnimatingButtonsView::buttonClickedHandler),
    textAnimationDone(this, &MainMenuAnimatingButtonsView::textAnimationDoneHandler)
{
}

void MainMenuAnimatingButtonsView::setupScreen()
{
    startUpAnimationBox.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    startUpAnimationBox.setPosition(HAL::DISPLAY_WIDTH / 2, HAL::DISPLAY_HEIGHT / 2 + BUTTONS_Y_OFFSET, 0, 0);
    startUpAnimationBox.setAnimationEndedCallback(startUpAnimationBoxDone);
    add(startUpAnimationBox);

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        buttonContainers[i].setPosition((HAL::DISPLAY_WIDTH - BUTTONS_WIDTH) / 2, BUTTONS_Y_OFFSET + ((HAL::DISPLAY_HEIGHT - (BUTTONS_HEIGHT * NUMBER_OF_BUTTONS)) / 2) + i * BUTTONS_HEIGHT, BUTTONS_WIDTH, BUTTONS_HEIGHT);
        buttons[i].setBitmaps(Bitmap(BITMAP_ANIMATING_BUTTONS_TABLE_ELEMENT_SHADOW_ID), Bitmap(BITMAP_ANIMATING_BUTTONS_TABLE_ELEMENT_SHADOW_ID));
        buttons[i].setXY(0, 0);
        buttons[i].setVisible(false);
        buttons[i].setAction(buttonClicked);
        buttonContainers[i].add(buttons[i]);
        add(buttonContainers[i]);
    }
    buttonContainers[0].setMoveAnimationEndedAction(buttonAnimationDone);

    demoIDs[0] = Defines::CONTROLS;
    icons[0].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_CONTROLS_TINY_ID));
    texts[0].setTypedText(TypedText(T_ANIMATING_BUTTON_CONTROLS_HEADLINE));

    demoIDs[1] = Defines::GAME2048;
    icons[1].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_2048_TINY_ID));
    texts[1].setTypedText(TypedText(T_ANIMATING_BUTTON_GAME_2048_HEADLINE));

    demoIDs[2] = Defines::GAME2D;
    icons[2].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_GAME_2D_TINY_ID));
    texts[2].setTypedText(TypedText(T_ANIMATING_BUTTON_GAME_2D_HEADLINE));

    demoIDs[3] = Defines::HOME_AUTOMATION;
    icons[3].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_HOME_AUTOMATION_TINY_ID));
    texts[3].setTypedText(TypedText(T_ANIMATING_BUTTON_HOME_AUTOMATION_HEADLINE));

    demoIDs[4] = Defines::SETTINGS;
    icons[4].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_SETTINGS_TINY_ID));
    texts[4].setTypedText(TypedText(T_ANIMATING_BUTTON_SETTINGS_HEADLINE));

    demoIDs[5] = Defines::GRAPHICS_EFFECT;
    icons[5].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_GRAPHICS_EFFECT_TINY_ID));
    texts[5].setTypedText(TypedText(T_ANIMATING_BUTTON_GRAPHICS_EFFECT_HEADLINE));

    texts[NUMBER_OF_BUTTONS - 1].setMoveAnimationEndedAction(textAnimationDone);

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        texts[i].setColor(Color::getColorFrom24BitRGB(0x1B, 0x38, 0x48));
        icons[i].setVisible(false);
        texts[i].setVisible(false);
        buttonContainers[i].add(icons[i]);
        buttonContainers[i].add(texts[i]);
    }

    startUpAnimationBox.startZoomAnimation(BUTTONS_WIDTH, BUTTONS_HEIGHT * NUMBER_OF_BUTTONS, 20, EasingEquations::backEaseOut, EasingEquations::backEaseOut);
    currentAnimationState = ANIMATE_BOX_IN;

    presenter->setPreviousSelectedMenuType(Defines::ANIMATING_BUTTONS_MENU);
}

void MainMenuAnimatingButtonsView::tearDownScreen()
{
}

void MainMenuAnimatingButtonsView::StartupAnimationBoxDoneHandler(const ZoomAnimationBox& box)
{
    if (currentAnimationState == ANIMATE_BOX_IN)
    {
        startUpAnimationBox.setVisible(false);
        startUpAnimationBox.invalidate();

        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            buttons[i].setVisible(true);
            buttons[i].invalidate();

            int numberOfBoxesInRowOne = 3;
            int duration = 20;
            if (i < numberOfBoxesInRowOne)
            {
                buttonContainers[i].startMoveAnimation(64, 160 + (buttonContainers[i].getHeight() * i) + (i)*BUTTONS_SPACE, duration, EasingEquations::backEaseOut, EasingEquations::backEaseOut);
            }
            else
            {
                buttonContainers[i].startMoveAnimation(HAL::DISPLAY_WIDTH - buttonContainers[i].getWidth() - 64, 160 + (buttonContainers[i].getHeight() * (i - numberOfBoxesInRowOne)) + (i - numberOfBoxesInRowOne)*BUTTONS_SPACE, duration, EasingEquations::backEaseOut, EasingEquations::backEaseOut);
            }

            buttonContainers[i].setMoveAnimationDelay(i + 3);
        }
        currentAnimationState = ANIMATE_BUTTONS_FROM_BOX;
    }
    else if (currentAnimationState == ANIMATE_BOX_OUT)
    {
        gotoSelectedDemoScreen();
    }
}

void MainMenuAnimatingButtonsView::buttonClickedHandler(const AbstractButton& button)
{
    if (currentAnimationState != NO_ANIMATION)
    {
        return;
    }

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        if (&button == &buttons[i])
        {
            setSelectedDemoScreen(demoIDs[i]);
        }

        int duration = 12;
        int delayMax = NUMBER_OF_BUTTONS - 1;
        icons[i].setFadeAnimationDelay(delayMax - i);
        icons[i].startFadeAnimation(0, duration);
        texts[i].setFadeAnimationDelay(delayMax - i);
        texts[i].startFadeAnimation(0, duration);

        buttonContainers[i].setMoveAnimationDelay(duration + delayMax + i);
        buttonContainers[i].startMoveAnimation((HAL::DISPLAY_WIDTH - BUTTONS_WIDTH) / 2, BUTTONS_Y_OFFSET + ((HAL::DISPLAY_HEIGHT - (BUTTONS_HEIGHT * NUMBER_OF_BUTTONS)) / 2) + i * BUTTONS_HEIGHT, 20, EasingEquations::linearEaseNone, EasingEquations::backEaseOut);
    }
    currentAnimationState = ANIMATE_BUTTONS_INTO_BOX;
}

void MainMenuAnimatingButtonsView::buttonAnimationDoneHandler(const MoveAnimator<Container>& box)
{
    if (currentAnimationState == ANIMATE_BUTTONS_FROM_BOX)
    {
        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            texts[i].setVisible(true);
            texts[i].setXY(buttons[i].getRect().right(), buttons[i].getY() + 14);
            texts[i].setMoveAnimationDelay(i * 2);
            texts[i].startMoveAnimation(buttons[i].getX() + 60, texts[i].getY(), 20, EasingEquations::cubicEaseOut);

            icons[i].setVisible(true);
            icons[i].setXY(buttons[i].getX() + 10, buttons[i].getY() + 5);
            icons[i].setAlpha(0);
            icons[i].startFadeAnimation(255, 16);
        }
        currentAnimationState = ANIMATE_ICONS_AND_TEXT_IN;
    }
    else if (currentAnimationState == ANIMATE_BUTTONS_INTO_BOX)
    {
        for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
        {
            buttons[i].setVisible(false);
            buttons[i].invalidate();
        }

        startUpAnimationBox.setVisible(true);
        startUpAnimationBox.invalidate();

        startUpAnimationBox.startZoomAnimation(0, 0, 18, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
        currentAnimationState = ANIMATE_BOX_OUT;
    }
}

void MainMenuAnimatingButtonsView::textAnimationDoneHandler(const MoveAnimator<TextArea>& t)
{
    if (currentAnimationState == ANIMATE_ICONS_AND_TEXT_IN)
    {
        currentAnimationState = NO_ANIMATION;
    }
}

void MainMenuAnimatingButtonsView::screenSaverMinorTick()
{
    application().gotoMainMenuCarouselScreenNoTransition();
}

void MainMenuAnimatingButtonsView::setSelectedDemoScreen(Defines::DemoID demoId)
{
    selectedDemoScreen = demoId;
}

void MainMenuAnimatingButtonsView::gotoSelectedDemoScreen()
{
    presenter->setSelectedDemoScreen(selectedDemoScreen);

    switch (selectedDemoScreen)
    {
    case Defines::GAME2048:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2048ScreenNoTransition();
        break;
    case Defines::GAME2D:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2DScreenNoTransition();
        break;
    case Defines::CONTROLS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoControlsScreenNoTransition();
        break;
    case Defines::HOME_AUTOMATION:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoHomeAutomationScreenNoTransition();
        break;
    case Defines::SETTINGS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoSettingsScreenNoTransition();
        break;
    case Defines::GRAPHICS_EFFECT:
        static_cast<FrontendApplication*>(Application::getInstance())->gotographics_effectScreenNoTransition();
        break;
    case Defines::NUMBER_OF_DEMO_SCREENS:
    case Defines::NO_DEMO_SCREEN:
    default:
        break;
    }
}

void MainMenuAnimatingButtonsView::mcuLoadUpdated(uint8_t newMcuLoad)
{
    mcuLoad.updateProcessorLoad(newMcuLoad);
}
