#ifndef GAME_KNIGHT_VIEW_HPP
#define GAME_KNIGHT_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/gameKnight_screen/GameKnightPresenter.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/widgets/TiledImage.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>
#include <gui/gameKnight_screen/Knight.hpp>
#include <gui/gameKnight_screen/Zombie.hpp>
#include <gui/gameKnight_screen/Head.hpp>
#include <gui/gameKnight_screen/Arrow.hpp>
#include <gui/gameKnight_screen/TiledImageBugFixed.hpp>
#include <gui/gameKnight_screen/GameStatusPopup.hpp>
#include <gui/gameKnight_screen/GameButton.hpp>

class GameKnightView : public DemoView<GameKnightPresenter>
{
public:
    GameKnightView();

    virtual ~GameKnightView()
    {
    }

    virtual void setupScreen();

    virtual void tearDownScreen()
    {
    }

    void handleTickEvent();

    void animateBackground();

    void mcuLoadUpdated(uint8_t mcuLoad);
    virtual void screenSaverMinorTick();

    void screenClick(ClickEvent::ClickEventType type, int x, int y);
    
private:
    static const int NUMBER_OF_BACKGROUND_ITEMS = 2;
    static const int NUMBER_OF_LIVES = 3;
    static const int DEATH_COUNTER_DELAY = 50;
    static const int GAME_PROGRESS_SPEED = 80; // Number of ticks that makes out one percentage of the game progress (~60 = 1 sec)
    int activeScreenWidth;

    int frameSkippedCounter;
    int frames;

    int gameProgressCounter;
    int gameProgressPercentage;
    int respawnCounter;
    int lives;

    int tickCounter;

    int groundLevelY;
    Image background;
    Image touchgfxLogo;
    TiledImageBugFixed backgroundLayer01;
    TiledImageBugFixed backgroundLayer02;

    Image backgroundItems[NUMBER_OF_BACKGROUND_ITEMS];

    TiledImageBugFixed ground;

    int fps;
    int mcu;

    TextArea mcuLoadText;
    TextAreaWithOneWildcard mcuLoadValueText;
    Unicode::UnicodeChar mcuLoadTextValueBuffer[15];

    GameStatusPopup gameStatusPopup;

    ToggleButton chromArtButton;

    Knight knight;
    Zombie zombie;
    Head head;
    Arrow arrow;

    int startY;

    GameButton jumpButton;
    GameButton attackButton;

    Image lifeIndicator[NUMBER_OF_LIVES];
    ImageProgress gameProgressIndicator;

    Callback<GameKnightView, const AbstractButton&> buttonClickedCallback;
    void buttonClicked(const AbstractButton& source);

    Callback<GameKnightView, const AnimatedImage&> animationEndedCallback;
    void animationEnded(const AnimatedImage& source);

    Callback<GameKnightView, const MoveAnimator<AnimatedImage>&> moveAnimationEndedCallback;
    void moveAnimationEnded(const MoveAnimator<AnimatedImage>& source);
    void collisionDetection();

    Callback<GameKnightView, const GameStatusPopup::ReturnState> popupButtonCallback;
    void popupButtonHandler(const GameStatusPopup::ReturnState source);

    bool gameIsRunning;
    int deathTickCounter;
    void gotoStartOfGameState();
    void gotoEndOfGameStatePhase1();
    void gotoEndOfGameStatePhase2();
    void showGameElements(bool visible);

    void killKnight();
    void setupEnemies();
    void respawnKnight();
    void updateLiveIndicator();
    void updateGameStatus();

    void updateFPSAndMCU();
};

#endif 
