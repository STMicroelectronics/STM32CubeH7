#include <gui/game2d_screen/Game2DView.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/Utils.hpp>

#if defined(ST) && !defined(SIMULATOR)
extern volatile bool disableChromArt;
#endif

#if defined(_RENESAS_SYNERGY_) && !defined(SIMULATOR)
extern volatile bool disableHardwareAcceleration;
#endif

Game2DView::Game2DView() :
    parallaxBackground(640, 290),
    chromArtOff(false),
    velocity(-11),
    collectibleItemCatched(0),
    collectibleItemMissed(0),
    playerDeadCounter(0)
{
}

void Game2DView::setupScreen()
{
    mcuLoad.setColor(Color::getColorFrom24BitRGB(0x18, 0x28, 0x28));

    playerCharacter.setBitmaps(BITMAP_SPRITE_00_ID, BITMAP_SPRITE_09_ID);
    playerCharacter.setUpdateTicksInterval(3);

    bullet.setBitmaps(BITMAP_SPHERE1_ID, BITMAP_SPHERE1_ID);
    bullet.stopAnimation();
    parallaxBackground.addCollectibleItem(bullet);

    playerCharacterStartX = parallaxBackground.getX() + 36;
    playerCharacterStartY = parallaxBackground.getRect().bottom() - playerCharacter.getHeight() - 14;
    playerCharacterMaxY = parallaxBackground.getY() + 30;

    parallaxBackground.setXY(0, 0);

    // PlayerCharacter was added in designer code, but we would like to add it
    // to the parallaxBackground instead, therefore remove it first.
    remove(playerCharacter);
    parallaxBackground.addPlayerCharacter(playerCharacter);

    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        collectibleItem[i].setX(parallaxBackground.getRect().right());
        collectibleItem[i].stopAnimation();
        parallaxBackground.addCollectibleItem(collectibleItem[i]);
    }

    touchArea.setPosition(0, 0, parallaxBackground.getWidth(), parallaxBackground.getHeight());

    gameArea.add(parallaxBackground);
    gameArea.add(touchArea);

#if !defined(ST) && !defined(SIMULATOR)
    remove(chromArtOnOffButton);
    remove(chromArtDescription);
    remove(chromArtHeader);
#endif

#if defined(_RENESAS_SYNERGY_) && !defined(SIMULATOR)
    add(chromArtOnOffButton);
    add(chromArtDescription);
    add(chromArtHeader);
    chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_HW_BUTTON_ON_ID), Bitmap(BITMAP_GAME2D_HW_BUTTON_ON_PRESSED_ID));
    chromArtHeader.setTypedText(TypedText(T_GAME_2D_RENESAS_HEADER));
    chromArtDescription.setTypedText(TypedText(T_GAME_2D_RENESAS_DESCRIPTION));
#endif

    updateScoreText();

    playerCharacter.setUpdateTicksInterval(6);
    parallaxBackground.setActiveTickInterval(2);

    parallaxBackground.startAnimation();

#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = chromArtOff;
#endif

#if defined(_RENESAS_SYNERGY_) && !defined(SIMULATOR)
    disableHardwareAcceleration = chromArtOff;
#endif
    restartGame();
}

void Game2DView::tearDownScreen()
{
#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = false;
#endif

#if defined(_RENESAS_SYNERGY_) && !defined(SIMULATOR)
    disableHardwareAcceleration = false;
#endif
}

void Game2DView::handleTickEvent()
{
    // Stop collectible items if they have passed the left border
    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        if (collectibleItem[i].getRect().right() < 0)
        {
            collectibleItem[i].setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
            collectibleItem[i].stopAnimation();

            if (playerDeadCounter == 0)
            {
                collectibleItemMissed++;
                updateScoreText();
            }
        }
    }
    if (bullet.getRect().right() < 0)
    {
        bullet.setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
        bullet.stopAnimation();
    }

    // Start new collectible items
    if (Utils::nextRand(0, 100) < 8)
    {
        for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
        {
            if (!collectibleItem[i].isRunning())
            {
                collectibleItem[i].setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
                collectibleItem[i].startAnimation(Utils::nextRand(2, 7));
                break;
            }
        }
    }

    // Dead time period timeout
    if (playerDeadCounter > 0)
    {
        playerDeadCounter++;

        if (playerDeadCounter > 100)
        {
            restartGame();
        }
        return;
    }

    // Start new bullet
    if (Utils::nextRand(0, 100) < 3)
    {
        if (!bullet.isRunning())
        {
            bullet.setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
            bullet.startAnimation(Utils::nextRand(7, 10));
        }
    }

    // Collect collectible items
    Rect collisionArea(playerCharacter.getRect().x, playerCharacter.getRect().y + 10, playerCharacter.getRect().width - 20, playerCharacter.getRect().height - 10);
    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        if (collectibleItem[i].getRect().intersect(collisionArea))
        {
            collectibleItem[i].setX(parallaxBackground.getRect().right());
            collectibleItem[i].stopAnimation();
            collectibleItemCatched++;
            updateScoreText();
        }
    }

    if (bullet.getRect().intersect(collisionArea))
    {
        // Collision with bullet
        playerCharacter.stopAnimation();
        playerCharacter.startMoveAnimation(playerCharacter.getX() - 160, playerCharacter.getY() + 100, 80, EasingEquations::quadEaseOut, EasingEquations::quadEaseIn);
        playerDeadCounter = 1;
        bullet.setX(-bullet.getWidth());
        bullet.invalidate();
    }
    else
    {
        // The PlayerCharacter can only be controlled when it has arrived at
        // its start position
        if (playerCharacter.getX() == playerCharacterStartX)
        {
            // Control player
            if (flyButton.getPressedState() || touchArea.getPressedState())
            {
                velocity += 1;
                velocity = (velocity > 7) ? 7 : velocity;
            }
            else if (velocity > -10)
            {
                velocity -= 1;
            }

            if (velocity != 0)
            {
                int newY = playerCharacter.getY() - velocity;

                if (newY < playerCharacterMaxY)
                {
                    newY = playerCharacterMaxY;
                }
                else if (newY > playerCharacterStartY)
                {
                    newY = playerCharacterStartY;
                }

                playerCharacter.moveTo(playerCharacter.getX(), newY);
            }
        }
    }
}

void Game2DView::mcuLoadUpdated(uint8_t newMcuLoad)
{
    mcuLoad.updateProcessorLoad(newMcuLoad);
}

void Game2DView::updateScoreText()
{
    Unicode::snprintf(scoreValueBuffer, SCOREVALUE_SIZE, "%i\0", (collectibleItemCatched * 2) - collectibleItemMissed);
    scoreValue.invalidate();
}

void Game2DView::updateChromArtButtonBitmaps()
{
    if (chromArtOff)
    {
#if defined(ST) && !defined(SIMULATOR)
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_ST_BUTTON_OFF_ID), Bitmap(BITMAP_GAME2D_ST_BUTTON_OFF_PRESSED_ID));
#endif
#if defined(_RENESAS_SYNERGY_) && !defined(SIMULATOR)
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_HW_BUTTON_OFF_ID), Bitmap(BITMAP_GAME2D_HW_BUTTON_OFF_PRESSED_ID));
#endif
    }
    else
    {
#if defined(ST) && !defined(SIMULATOR)
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_ST_BUTTON_ON_ID), Bitmap(BITMAP_GAME2D_ST_BUTTON_PRESSED_ID));
#endif
#if defined(_RENESAS_SYNERGY_) && !defined(SIMULATOR)
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_HW_BUTTON_ON_ID), Bitmap(BITMAP_GAME2D_HW_BUTTON_ON_PRESSED_ID));
#endif
    }
    chromArtOnOffButton.invalidate();
}

void Game2DView::chromArtButtonHandler()
{
    chromArtOff = !chromArtOff;
#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = chromArtOff;
#endif

#if defined(_RENESAS_SYNERGY_) && !defined(SIMULATOR)
    disableHardwareAcceleration = chromArtOff;
#endif

    updateChromArtButtonBitmaps();
}

void Game2DView::restartGame()
{
    playerDeadCounter = 0;
    collectibleItemCatched = 0;
    collectibleItemMissed = 0;

    playerCharacter.setXY(-playerCharacter.getWidth(), playerCharacterStartY);
    playerCharacter.startMoveAnimation(playerCharacterStartX, playerCharacterStartY, 64, EasingEquations::cubicEaseOut);
    playerCharacter.startAnimation(false, true, true);
    playerCharacter.invalidate();
    updateScoreText();
}
