#include <gui/game2048_screen/Game2048View.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

Game2048View::Game2048View() :
    moveCompletedCallback(this, &Game2048View::moveCompletedHandler),
    gameWonCallback(this, &Game2048View::gameWonHandler),
    gameLostCallback(this, &Game2048View::gameLostHandler)
{
}

void Game2048View::setupScreen()
{
    currentScore = 0;
    updateScoreText();

    gameBoard.setXY(20, 0);
    gameBoard.setMoveCompletedCallback(moveCompletedCallback);
    gameBoard.setGameWonCallback(gameWonCallback);
    gameBoard.setGameLostCallback(gameLostCallback);
    add(gameBoard);

    howToPlayInstruction[0].setBitmap(Bitmap(BITMAP_HOW_TO_PLAY_00_ID));
    howToPlayInstruction[1].setBitmap(Bitmap(BITMAP_HOW_TO_PLAY_01_ID));
    howToPlayInstruction[2].setBitmap(Bitmap(BITMAP_HOW_TO_PLAY_02_ID));

    for (int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++)
    {
        // Enlarge the instruction swipe area by placing it in the howToPlayInstructionExtraSpaceContainer
        howToPlayInstruction[i].setXY(43, 10);
        howToPlayInstructionExtraSpaceContainer[i].add(howToPlayInstruction[i]);
        howToPlayInstructionExtraSpaceContainer[i].setPosition(0, 0, 43 + howToPlayInstruction[i].getWidth() + 44, howToPlayInstruction[i].getHeight() + 20);
        howToPlayInstructionContainer.add(howToPlayInstructionExtraSpaceContainer[i]);
    }
    howToPlayInstructionContainer.setXY(7, 20);
    howToPlayInstructionContainer.setDotIndicatorBitmaps(Bitmap(BITMAP_GAME_2048_DOT_OFF_ID), Bitmap(BITMAP_GAME_2048_DOT_ON_ID));
    howToPlayInstructionContainer.setDotIndicatorXYWithCenteredX(howToPlayInstructionContainer.getWidth() / 2, howToPlayInstructionContainer.getHeight() - 7);
    howToPlayInstructionContainer.setSwipeCutoff(80);
    howToPlayInstructionContainer.setEndSwipeElasticWidth(30);
    howToPlayModalWindow.hide();

    howToPlayModalWindow.remove(closeHowToPlayButton);
    howToPlayModalWindow.add(howToPlayInstructionContainer);
    howToPlayModalWindow.add(closeHowToPlayButton);

    // Make sure that popup elements is added last so they appears on top of all the other graphical elements
    remove(endGameStatus);
    remove(howToPlayModalWindow);
    add(endGameStatus);
    add(howToPlayModalWindow);
}

void Game2048View::tearDownScreen()
{
}

void Game2048View::mcuLoadUpdated(uint8_t newMcuLoad)
{
    mcuLoad.updateProcessorLoad(newMcuLoad);
}

void Game2048View::newGamePressed()
{
    if (!gameBoard.isTouchable())
    {
        endGameStatus.setVisible(false);
        endGameStatus.invalidate();
        gameBoard.setTouchable(true);
        autoPlayButton.setTouchable(true);
        howToPlayButton.setTouchable(true);
        startNewGame();
    }
    else
    {
        startNewGame();
    }
}

void Game2048View::autoPlayPressed()
{
    gameBoard.setAutoPlay(autoPlayButton.getState());
}

void Game2048View::howToPlayPressed()
{
    howToPlayInstructionContainer.setSelectedScreen(0);
    showHowToPlayPopUp(true);
}

void Game2048View::closeHowToPlayPressed()
{
    showHowToPlayPopUp(false);
}

void Game2048View::moveCompletedHandler(const GameBoard& gb)
{
    int scoreDifference = gb.getScore() - currentScore;
    currentScore = gb.getScore();
    updateScoreText();
    showDeltaScoreText(scoreDifference);
}

void Game2048View::gameWonHandler(const GameBoard& gb)
{
    endGameStatus.setBitmap(Bitmap(BITMAP_GAME_2048_YOU_WIN_ID));
    endGameStatus.setVisible(true);
    endGameStatus.invalidate();

    gameBoard.setTouchable(false);
    autoPlayButton.setTouchable(false);
    howToPlayButton.setTouchable(false);
}

void Game2048View::gameLostHandler(const GameBoard& gb)
{
    endGameStatus.setBitmap(Bitmap(BITMAP_GAME_2048_GAME_OVER_ID));
    endGameStatus.setVisible(true);
    endGameStatus.invalidate();

    gameBoard.setTouchable(false);
    autoPlayButton.setTouchable(false);
    howToPlayButton.setTouchable(false);
}

void Game2048View::updateScoreText()
{
    Unicode::snprintf(scoreTxtBuffer, SCORETXT_SIZE, "%d", currentScore);
    scoreTxt.invalidate();
}

void Game2048View::showDeltaScoreText(int deltaScore)
{
    if (deltaScore == 0)
    {
        return;
    }
    Unicode::snprintf(deltaScoreTxtBuffer, DELTASCORETXT_SIZE, "%d", deltaScore);

    deltaScoreTxt.setAlpha(255);
    deltaScoreTxt.setXY(scoreTxt.getX() - 8, scoreTxt.getY() - 4);

    int duration = 26;
    deltaScoreTxt.startMoveAnimation(deltaScoreTxt.getX(), deltaScoreTxt.getY() - 24, duration);
    deltaScoreTxt.startFadeAnimation(0, duration, EasingEquations::cubicEaseIn);
    deltaScoreTxt.invalidate();
}

void Game2048View::showHowToPlayPopUp(bool visible)
{
    if (visible)
    {
        howToPlayModalWindow.show();
    }
    else
    {
        howToPlayModalWindow.hide();
    }
    howToPlayModalWindow.invalidate();
}

void Game2048View::startNewGame()
{
    gameBoard.initiateNewGame();
    currentScore = 0;
    updateScoreText();
}

void Game2048View::screenSaverMinorTick()
{
    if (howToPlayModalWindow.isVisible())
    {
        showHowToPlayPopUp(false);
    }

    if (endGameStatus.isVisible())
    {
        newGamePressed();
    }

    if (!gameBoard.getAutoPlay())
    {
        autoPlayButton.forceState(true);
        autoPlayButton.invalidate();
        gameBoard.setAutoPlay(true);
    }
}
