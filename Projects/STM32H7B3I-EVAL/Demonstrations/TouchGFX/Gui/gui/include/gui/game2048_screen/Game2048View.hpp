#ifndef GAME2048_VIEW_HPP
#define GAME2048_VIEW_HPP

#include <gui_generated/game2048_screen/Game2048ViewBase.hpp>
#include <gui/game2048_screen/Game2048Presenter.hpp>
#include <gui/game2048_screen/GameBoard.hpp>
#include <gui/common/SwipeContainer.hpp>

class Game2048View : public Game2048ViewBase
{
public:
    Game2048View();
    virtual ~Game2048View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void mcuLoadUpdated(uint8_t newMcuLoad);
    virtual void screenSaverMinorTick();

protected:
    static const int NUMBER_OF_INSTRUCTIONS = 3;

    GameBoard gameBoard;
    int currentScore;

    SwipeContainer howToPlayInstructionContainer;
    Container howToPlayInstructionExtraSpaceContainer[NUMBER_OF_INSTRUCTIONS];
    Image howToPlayInstruction[NUMBER_OF_INSTRUCTIONS];

    Callback<Game2048View, const GameBoard& > moveCompletedCallback;
    Callback<Game2048View, const GameBoard& > gameWonCallback;
    Callback<Game2048View, const GameBoard& > gameLostCallback;

    virtual void newGamePressed();
    virtual void autoPlayPressed();
    virtual void howToPlayPressed();
    virtual void closeHowToPlayPressed();

    void moveCompletedHandler(const GameBoard& gb);
    void gameWonHandler(const GameBoard& gb);
    void gameLostHandler(const GameBoard& gb);

    void updateScoreText();
    void showDeltaScoreText(int deltaScore);
    void showHowToPlayPopUp(bool visible);

    void startNewGame();
};

#endif // GAME2048_VIEW_HPP
