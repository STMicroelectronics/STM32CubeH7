#ifndef GAME2D_VIEW_HPP
#define GAME2D_VIEW_HPP

#include <gui_generated/game2d_screen/Game2DViewBase.hpp>
#include <gui/game2d_screen/Game2DPresenter.hpp>
#include <gui/game2D_screen/ParallaxBackground.hpp>
#include <touchgfx/widgets/TouchArea.hpp>

class Game2DView : public Game2DViewBase
{
public:
    Game2DView();
    virtual ~Game2DView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    void mcuLoadUpdated(uint8_t newMcuLoad);

protected:
    static const int NUMBER_OF_COLLECTIBLE_ITEMS = 2;

    ParallaxBackground parallaxBackground;
    TouchArea touchArea;

    int playerCharacterStartX;
    int playerCharacterStartY;
    int playerCharacterMaxY;

    CollectibleItem collectibleItem[NUMBER_OF_COLLECTIBLE_ITEMS];
    CollectibleItem bullet;

    bool chromArtOff;

    int velocity;
    int collectibleItemCatched;
    int collectibleItemMissed;
    int playerDeadCounter;

    void restartGame();
    void updateScoreText();
    void updateChromArtButtonBitmaps();

    void chromArtButtonHandler();
};

#endif // GAME2D_VIEW_HPP
