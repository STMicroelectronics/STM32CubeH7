


#include <gui/main_menu_screen/MainMenuAnimatingButtonsView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

MainMenuAnimatingButtonsView::MainMenuAnimatingButtonsView() :
    MainMenuView(),
    currentAnimationState(NO_ANIMATION),
    startUpAnimationBoxDone(this, &MainMenuAnimatingButtonsView::StartupAnimationBoxDoneHandler),
    buttonAnimationDone(this, &MainMenuAnimatingButtonsView::buttonAnimationDoneHandler),
    buttonClicked(this, &MainMenuAnimatingButtonsView::buttonClickedHandler),
    textAnimationDone(this, &MainMenuAnimatingButtonsView::textAnimationDoneHandler)
{
}

MainMenuAnimatingButtonsView::~MainMenuAnimatingButtonsView()
{
}

void MainMenuAnimatingButtonsView::setupScreen()
{
    background.setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    if (EMBED_IN_ST_DEMO)
    {
        mcuLoadTxt.setXY(624, 416);
        mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right() - 6, mcuLoadTxt.getY(), 50, 30);
        toggleMenuButton.setXY(710, 32);
        add(gotoSTMenuButton);
    }

    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    add(toggleMenuButton);

    startUpAnimationBox.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    startUpAnimationBox.setPosition(HAL::DISPLAY_WIDTH/2, HAL::DISPLAY_HEIGHT/2 + BUTTONS_Y_OFFSET, 0, 0);
    startUpAnimationBox.setAnimationEndedCallback(startUpAnimationBoxDone);
    add(startUpAnimationBox);

    for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
    {
        buttonContainers[i].setPosition((HAL::DISPLAY_WIDTH - BUTTONS_WIDTH) / 2, BUTTONS_Y_OFFSET + ((HAL::DISPLAY_HEIGHT - (BUTTONS_HEIGHT * NUMBER_OF_BUTTONS)) / 2) + i * BUTTONS_HEIGHT, BUTTONS_WIDTH, BUTTONS_HEIGHT );
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

    demoIDs[3] = Defines::AUDIO_PLAYER;
    icons[3].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_AUDIO_PLAYER_TINY_ID));
    texts[3].setTypedText(TypedText(T_ANIMATING_BUTTON_AUDIO_PLAYER_HEADLINE));

    demoIDs[4] = Defines::HOME_AUTOMATION;
    icons[4].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_HOME_AUTOMATION_TINY_ID));
    texts[4].setTypedText(TypedText(T_ANIMATING_BUTTON_HOME_AUTOMATION_HEADLINE));

    demoIDs[5] = Defines::SETTINGS;
    icons[5].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_SETTINGS_TINY_ID));
    texts[5].setTypedText(TypedText(T_ANIMATING_BUTTON_SETTINGS_HEADLINE));

    demoIDs[6] = Defines::VIDEO_PLAYER;
    icons[6].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_VIDEO_PLAYER_TINY_ID));
    texts[6].setTypedText(TypedText(T_ANIMATING_BUTTON_VIDEO_PLAYER_HEADLINE));

    demoIDs[7] = Defines::GRAPHICS_EFFECT;
    icons[7].setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_MENU_ICONS_GRAPHICS_EFFECT_TINY_ID));
    texts[7].setTypedText(TypedText(T_ANIMATING_BUTTON_GRAPHICS_EFFECT_HEADLINE));

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

            int numbersToTheLeft = 4;
            int duration = 20;
            if (i < numbersToTheLeft)
            {
                buttonContainers[i].startMoveAnimation(64, 160 + (buttonContainers[i].getHeight() * i) + (i)*BUTTONS_SPACE, duration, EasingEquations::backEaseOut, EasingEquations::backEaseOut);
            }
            else
            {
                buttonContainers[i].startMoveAnimation(HAL::DISPLAY_WIDTH - buttonContainers[i].getWidth() - 64, 160 + (buttonContainers[i].getHeight() * (i-numbersToTheLeft)) + (i-numbersToTheLeft)*BUTTONS_SPACE, duration, EasingEquations::backEaseOut, EasingEquations::backEaseOut);
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
        icons[i].setFadeAnimationDelay(delayMax - i );
        icons[i].startFadeAnimation(0, duration);
        texts[i].setFadeAnimationDelay(delayMax - i );
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
            texts[i].setMoveAnimationDelay(i*2);
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

void MainMenuAnimatingButtonsView::toggleMenu()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
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
    toggleMenu();
}
