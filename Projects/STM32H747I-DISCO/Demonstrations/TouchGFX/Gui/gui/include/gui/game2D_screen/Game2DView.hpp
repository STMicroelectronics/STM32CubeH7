
#ifndef GAME_2D_VIEW_HPP
#define GAME_2D_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/game2D_screen/Game2DPresenter.hpp>
#include <gui/game2D_screen/CollectibleItem.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <gui/game2D_screen/ParallaxBackground.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/widgets/TouchArea.hpp>

class Game2DView : public DemoView<Game2DPresenter>
{
public:
    Game2DView();
    virtual ~Game2DView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    void screenClick(ClickEvent::ClickEventType type, int x, int y);
private:
    static const int NUMBER_OF_COLLECTIBLE_ITEMS = 2;

    Image background;

    Container gameArea;
    ParallaxBackground parallaxBackground;
    TouchArea touchArea; 

    MoveAnimator<AnimatedImage> playerCharacter;
    int playerCharacterStartX;
    int playerCharacterStartY;
    int playerCharacterMaxY;

    CollectibleItem collectibleItem[NUMBER_OF_COLLECTIBLE_ITEMS];
    CollectibleItem bullet;

    bool chromArtOff;

    Button flyButton;
    Button chromArtOnOffButton;

    TextArea chromArtDescription;
    TextArea chromArtHeader;

    TextArea scoreText;
    TextAreaWithOneWildcard scoreValue;
    Unicode::UnicodeChar scoreValueBuf[16];

    int velocity;
    int collectibleItemCatched;
    int collectibleItemMissed;
    int playerDeadCounter;

    Callback<Game2DView, const AbstractButton&> onButtonPressed;
    void buttonPressedhandler(const AbstractButton& button);

    void restartGame();
    void updateScoreText();
    void updateChromArtButtonBitmaps();
};

#endif // GAME_2D_VIEW_HPP
