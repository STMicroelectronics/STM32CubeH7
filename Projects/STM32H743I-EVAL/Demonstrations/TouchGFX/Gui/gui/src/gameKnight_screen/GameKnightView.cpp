#include <gui/gameKnight_screen/GameKnightView.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <gui/common/Utils.hpp>
#include <touchgfx/hal/HAL.hpp>

GameKnightView::GameKnightView() :
    buttonClickedCallback(this, &GameKnightView::buttonClicked),
    animationEndedCallback(this, &GameKnightView::animationEnded),
    moveAnimationEndedCallback(this, &GameKnightView::moveAnimationEnded),
    popupButtonCallback(this, &GameKnightView::popupButtonHandler)
{
    frameSkippedCounter = 0;
    frames = 0;
    gameIsRunning = false;
    deathTickCounter = DEATH_COUNTER_DELAY + 1;

    // Demo supports two widths
    activeScreenWidth = (HAL::DISPLAY_WIDTH == 640) ? 640 : 800;
}

void GameKnightView::setupScreen()
{
    background.setBitmap(Bitmap(BITMAP_BACKGROUND_LAYER_00_ID));
    background.setXY((activeScreenWidth == 640) ? - 80 : 0, 0);
    add(background);

    for (int i = 0; i < NUMBER_OF_LIVES; i++)
    {
        int lifeIndicatorWidth = (3 * Bitmap(BITMAP_LIFE_HEAD_ID).getWidth()) + (2 * (50 - Bitmap(BITMAP_LIFE_HEAD_ID).getWidth()));

        lifeIndicator[i].setBitmap(Bitmap(BITMAP_LIFE_HEAD_ID));
        lifeIndicator[i].setXY( (activeScreenWidth - lifeIndicatorWidth) / 2  + (i * 50), 6);
        add(lifeIndicator[i]);
    }

    gameProgressIndicator.setBackground(Bitmap(BITMAP_KNIGHT_SLIDER_BACKGROUND_ID));
    gameProgressIndicator.setXY(16, 8);
    gameProgressIndicator.setProgressIndicatorPosition(44, 11, Bitmap(BITMAP_KNIGHT_PROGRESS_BAR_ID).getWidth(), Bitmap(BITMAP_KNIGHT_PROGRESS_BAR_ID).getHeight());
    gameProgressIndicator.setRange(0, 100);
    gameProgressIndicator.setBitmap(BITMAP_KNIGHT_PROGRESS_BAR_ID);
    gameProgressIndicator.setDirection(AbstractDirectionProgress::RIGHT);
    gameProgressIndicator.setAnchorAtZero(true);
    add(gameProgressIndicator);

    chromArtButton.setBitmaps(Bitmap(BITMAP_CHROM_ART_ON_ID), Bitmap(BITMAP_CHROM_ART_OFF_ID));
    chromArtButton.setXY(activeScreenWidth - 220, 5);
    chromArtButton.setAction(buttonClickedCallback);
    add(chromArtButton);

    mcuLoadText.setTypedText(TypedText(T_KNIGHT_MCU_LOAD_TEXT));
    mcuLoadText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    Unicode::snprintf(mcuLoadTextValueBuffer, 5, "%d", 0);
    mcuLoadValueText.setWildcard(mcuLoadTextValueBuffer);
    mcuLoadValueText.setTypedText(TypedText(T_KNIGHT_MCU_LOAD_VALUE));
    mcuLoadValueText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    mcuLoadText.setXBaselineY(chromArtButton.getX() + 10, 24);
    mcuLoadText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    mcuLoadValueText.setPosition(mcuLoadText.getRect().right(), mcuLoadText.getY(), 109, mcuLoadText.getHeight());
    mcuLoadValueText.setColor(mcuLoadText.getColor());
    add(mcuLoadText);
    add(mcuLoadValueText);

    fps = 0;
    mcu = 0;
    updateFPSAndMCU();

    backgroundLayer01.setBitmap(Bitmap(BITMAP_BACKGROUND_LAYER_01_ID));
    backgroundLayer01.setPosition(0, 270, activeScreenWidth, backgroundLayer01.getHeight());
    add(backgroundLayer01);

    backgroundLayer02.setBitmap(Bitmap(BITMAP_BACKGROUND_LAYER_02_ID));
    backgroundLayer02.setPosition(0, 296, activeScreenWidth, backgroundLayer02.getHeight());
    add(backgroundLayer02);

    for (int i = 0; i < NUMBER_OF_BACKGROUND_ITEMS; i++)
    {
        backgroundItems[i].setX(801);
        add(backgroundItems[i]);
    }

    ground.setBitmap(Bitmap(BITMAP_TILE_ID));
    ground.setPosition(0, 480 - Bitmap(BITMAP_TILE_ID).getHeight(), activeScreenWidth, Bitmap(BITMAP_TILE_ID).getHeight());
    add(ground);

    touchgfxLogo.setBitmap(Bitmap(BITMAP_TOUCHGFX_LOGO_ID));
    touchgfxLogo.setXY((activeScreenWidth - touchgfxLogo.getWidth()) / 2, 443);
    add(touchgfxLogo);


    groundLevelY = 480 - 79;
    
    knight.setGroundPosition(groundLevelY);

    zombie.setGroundPosition(groundLevelY);
    zombie.reset();
    zombie.setState(Zombie::STOPPED);

    head.setXY(800, 160);
    head.reset();
    head.setState(Head::STOPPED);

    arrow.setXY(800, 360);
    arrow.setState(Arrow::STOPPED);

    add(knight);
    add(zombie);
    add(head);
    add(arrow);

    jumpButton.setBitmaps(Bitmap(BITMAP_JUMP_BUTTON_ID), Bitmap(BITMAP_JUMP_BUTTON_PRESSED_ID));
    jumpButton.setXY(0, 480 - jumpButton.getHeight());
    jumpButton.setAction(buttonClickedCallback);
    add(jumpButton);

    attackButton.setBitmaps(Bitmap(BITMAP_ATTACK_BUTTON_ID), Bitmap(BITMAP_ATTACK_BUTTON_PRESSED_ID));
    attackButton.setXY(activeScreenWidth - attackButton.getWidth(), 480 - attackButton.getHeight());
    attackButton.setAction(buttonClickedCallback);
    add(attackButton);

    gameStatusPopup.setButtonCallback(popupButtonCallback);
    add(gameStatusPopup);


    gotoEndOfGameStatePhase2();
    gameStatusPopup.setupWelcome();

}

void GameKnightView::handleTickEvent()
{
    View::handleTickEvent();
    if (HAL::getInstance()->getLCDRefreshCount() > 1)
    {
        frameSkippedCounter++;

    }
    else
    {
        frames++;
    }

    if (frames + frameSkippedCounter >= 60)
    {
        fps = frames;
        updateFPSAndMCU();
        frameSkippedCounter = 0;
        frames = 0;
    }

    tickCounter++;

    animateBackground();

    if (gameIsRunning)
    {
        setupEnemies();
        collisionDetection();
        respawnKnight();

        updateGameStatus();
    }
    else
    {
        deathTickCounter++;
        if (deathTickCounter == DEATH_COUNTER_DELAY)
        {
            gotoEndOfGameStatePhase2();
        }
    }
}

void GameKnightView::animateBackground()
{
    ground.setOffset(ground.getXOffset() + 4, 0);
    ground.invalidate();

    backgroundLayer01.setOffset(backgroundLayer01.getXOffset() + 1, 0);
    backgroundLayer01.invalidate();

    backgroundLayer02.setOffset(backgroundLayer02.getXOffset() + 2, 0);
    backgroundLayer02.invalidate();

    // BackgroundItems
    bool allowedToStartNewBackgroundItem = true;

    for (int j = 0; j < NUMBER_OF_BACKGROUND_ITEMS; j++)
    {
        if (backgroundItems[j].getX() > 200 &&
            backgroundItems[j].getX() <= 800)
        {
            allowedToStartNewBackgroundItem = false;
        }
    }

    if (allowedToStartNewBackgroundItem)
    {
        for (int i = 0; i < NUMBER_OF_BACKGROUND_ITEMS; i++)
        {
            if (backgroundItems[i].getRect().right() < 0 ||
                backgroundItems[i].getX() > 800)
            {
                // Not showing
                if (Utils::randomNumberBetween(0, 1000) < 5)
                {
                    int randomNumber = Utils::randomNumberBetween(0, 100);

//                     BitmapId newItemBmp =
//                         (randomNumber < 15) ? BITMAP_TREE_ID :
//                         (randomNumber < 30) ? BITMAP_SKELETON_ID :
//                         (randomNumber < 50) ? BITMAP_TOMBSTONE_ID :
//                         (randomNumber < 65) ? BITMAP_BUSH_ID :
//                         (randomNumber < 80) ? BITMAP_ARROW_SIGN_ID : BITMAP_DEAD_BUSH_ID;

                    BitmapId newItemBmp =
                        (randomNumber < 30) ? BITMAP_SKELETON_ID :
                        (randomNumber < 60) ? BITMAP_TOMBSTONE_ID : BITMAP_DEAD_BUSH_ID;

                    backgroundItems[i].setBitmap(Bitmap(newItemBmp));
                    backgroundItems[i].setXY(800, groundLevelY - backgroundItems[i].getHeight());
                }
                break;
            }
        }
    }
 
    for (int i = 0; i < NUMBER_OF_BACKGROUND_ITEMS; i++)
    {
        if (backgroundItems[i].getX() <= 800)
        {
            backgroundItems[i].moveRelative(-4, 0);
        }

        if (backgroundItems[i].getRect().right() < 0)
        {
            backgroundItems[i].setX(801);
        }
    }
    
}


void GameKnightView::setupEnemies()
{
    if (zombie.getRect().right() < 0)
    {
        zombie.reset();
        zombie.setState(Zombie::STOPPED);
    }

    if (head.getRect().right() < 0)
    {
        head.reset();
        head.setState(Head::STOPPED);
    }
    
    if (arrow.getRect().right() < 0)
    {
        arrow.reset();
        arrow.setState(Arrow::STOPPED);
    }

    if (knight.getState() == Knight::DEAD)
    {
        return;
    }

    if (zombie.getState() == Zombie::STOPPED &&
        head.getState() == Head::STOPPED &&
        arrow.getState() == Arrow::STOPPED)
    {
        int randomNumber = Utils::randomNumberBetween(0, 100);
        if (randomNumber < 10)
        {
            zombie.setState(Zombie::RUN);
        }
        else if (randomNumber < 15)
        {
            head.setState(Head::MOVING);
        }
        else if (randomNumber < 20)
        {
            arrow.setState(Arrow::MOVING);
        }
    }

    if (head.getState() == Head::MOVING &&
        head.getRect().right() < 600)
    {
        int randomNumber = Utils::randomNumberBetween(0, 1000);
        if (randomNumber < 10)
        {
            zombie.setState(Zombie::RUN);
        }
    }

    if (zombie.getState() == Zombie::RUN &&
        zombie.getRect().right() < 500)
    {
        int randomNumber = Utils::randomNumberBetween(0, 1000);

        if (arrow.getState() == Arrow::STOPPED)
        {
            if (randomNumber < 10)
            {
                head.setState(Head::MOVING);
            }
        }

        if (head.getState() == Head::STOPPED)
        {
            if (randomNumber < 10)
            {
                arrow.setState(Arrow::MOVING);
            }
        }
    }

}


void GameKnightView::respawnKnight()
{
    if (knight.getState() == Knight::DEAD)
    {
        respawnCounter++;
        if (respawnCounter > 200 &&
            zombie.getX() > 600 &&
            head.getX() > 600 &&
            arrow.getX() > 700)
        {
            knight.setState(Knight::ENTERING_GAME);
        }
    }
}

void GameKnightView::mcuLoadUpdated(uint8_t mcuLoad)
{
    mcu = mcuLoad;
    updateFPSAndMCU();
}

void GameKnightView::buttonClicked(const AbstractButton& source)
{
    tickCounter = 0;

    if (&source == &jumpButton)
    {
        knight.setState(Knight::JUMP);
    }
    else if (&source == &attackButton)
    {
        knight.setState(Knight::JUMP_ATTACK);
    }
    else if (&source == &chromArtButton)
    {
        presenter->setChromArtStatus(chromArtButton.getState());
    }

}

void GameKnightView::animationEnded(const AnimatedImage& source)
{
}

void GameKnightView::moveAnimationEnded(const MoveAnimator<AnimatedImage>& source)
{

}

void GameKnightView::collisionDetection()
{
    if (knight.getState() == Knight::JUMP_ATTACK &&
        zombie.getState() != Zombie::DEAD &&
        knight.getAttackArea().intersect(zombie.getBodyArea()))
    {
        zombie.setState(Zombie::DEAD);
    }

    if (zombie.getState() != Zombie::DEAD &&
        knight.getState() != Knight::DEAD &&
        zombie.getAttackArea().intersect(knight.getBodyArea()))
    {
        zombie.setState(Zombie::ATTACK);
        killKnight();
    }

    if (knight.getState() != Knight::DEAD &&
        arrow.getAttackArea().intersect(knight.getBodyArea()))
    {
        killKnight();
        arrow.setState(Arrow::HIT_TARGET);
    }

    if (knight.getState() == Knight::JUMP_ATTACK &&
        knight.getAttackArea().intersect(head.getBodyArea()))
    {
        head.setState(Head::DEAD);
    }

    if (knight.getState() != Knight::DEAD &&
        head.getState() != Head::DEAD &&
        head.getAttackArea().intersect(knight.getBodyArea()))
    {
        killKnight();
        head.setState(Head::DEAD);
    }
}

void GameKnightView::popupButtonHandler(const GameStatusPopup::ReturnState source)
{
    switch (source)
    {
    case GameStatusPopup::GOTO_MENU:
        gotoMenuClickedHandler(gotoMenuButton);
        break;
    case GameStatusPopup::GOTO_PLAY:
        gotoStartOfGameState();
        break;
    default:
        break;
    }
}

void GameKnightView::gotoStartOfGameState()
{
    gameIsRunning = true;

    gameStatusPopup.hide();
    gameProgressCounter = 0;
    gameProgressPercentage = 0;
    lives = NUMBER_OF_LIVES;
    updateLiveIndicator();

    showGameElements(true);

    knight.reset();
    zombie.reset();
    head.reset();
    arrow.reset();
}

void GameKnightView::gotoEndOfGameStatePhase1()
{
    gameIsRunning = false;
    deathTickCounter = 0;
}

void GameKnightView::gotoEndOfGameStatePhase2()
{
    gameIsRunning = false;
    showGameElements(false);

    if (lives != 0)
    {
        gameStatusPopup.setupGameWon();
    }
    else
    {
        gameStatusPopup.setupGameLost(gameProgressPercentage);
    }
    gameStatusPopup.show();
}

void GameKnightView::showGameElements(bool visible)
{
    knight.setVisible(visible);
    zombie.setVisible(visible);
    head.setVisible(visible);
    arrow.setVisible(visible);

    knight.invalidate();
    zombie.invalidate();
    head.invalidate();
    arrow.invalidate();
}

void GameKnightView::killKnight()
{
    knight.setState(Knight::DEAD);
    respawnCounter = 1;
    lives--;

    updateLiveIndicator();
}

void GameKnightView::updateLiveIndicator()
{
    for (int i = 0; i < lives; i++)
    {
        lifeIndicator[i].setBitmap(Bitmap(BITMAP_LIFE_HEAD_ID));
        lifeIndicator[i].invalidate();
    }
    for (int i = MAX(lives, 0); i < NUMBER_OF_LIVES; i++)
    {
        lifeIndicator[i].setBitmap(Bitmap(BITMAP_LIFE_HEAD_OFF_ID));
        lifeIndicator[i].invalidate();
    }
}

void GameKnightView::updateGameStatus()
{
    if (knight.getState() != Knight::DEAD)
    {
        gameProgressCounter++;
    }

    gameProgressPercentage = gameProgressCounter / GAME_PROGRESS_SPEED;
    if (gameProgressIndicator.getValue() != gameProgressPercentage)
    {
        gameProgressIndicator.setValue(gameProgressPercentage);
        gameProgressIndicator.invalidate();
    }

    if (gameProgressPercentage == 100)
    {
        gotoEndOfGameStatePhase2();
    }

    if (lives == 0)
    {
        gotoEndOfGameStatePhase1();
    }
}

void GameKnightView::updateFPSAndMCU()
{
    Unicode::snprintf(mcuLoadTextValueBuffer, 15, "%d / %d", fps, mcu);
    mcuLoadValueText.invalidate();
}

void GameKnightView::screenSaverMinorTick()
{

}

void GameKnightView::screenClick(ClickEvent::ClickEventType type, int x, int y)
{
    ClickEvent temp_evt(type, x, y);
    View::handleClickEvent(temp_evt);
}
