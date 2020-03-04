#ifndef MAINMENUANIMATINGBUTTONS_VIEW_HPP
#define MAINMENUANIMATINGBUTTONS_VIEW_HPP

#include <gui_generated/mainmenuanimatingbuttons_screen/MainMenuAnimatingButtonsViewBase.hpp>
#include <gui/mainmenuanimatingbuttons_screen/MainMenuAnimatingButtonsPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/common/ZoomAnimationBox.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/common/Defines.hpp>

class MainMenuAnimatingButtonsView : public MainMenuAnimatingButtonsViewBase
{
public:
    MainMenuAnimatingButtonsView();
    virtual ~MainMenuAnimatingButtonsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void screenSaverMinorTick();
    void mcuLoadUpdated(uint8_t newMcuLoad);

protected:
    enum AnimationState
    {
        ANIMATE_BOX_IN,
        ANIMATE_BOX_OUT,
        ANIMATE_BUTTONS_FROM_BOX,
        ANIMATE_BUTTONS_INTO_BOX,
        ANIMATE_ICONS_AND_TEXT_IN,
        NO_ANIMATION
    };

    AnimationState currentAnimationState;

    Defines::DemoID demoIDs[Defines::NUMBER_OF_DEMO_SCREENS];
    Defines::DemoID selectedDemoScreen;

    static const int NUMBER_OF_BUTTONS = Defines::NUMBER_OF_DEMO_SCREENS;

    static const int BUTTONS_WIDTH = 320;
    static const int BUTTONS_HEIGHT = 50;
    static const int BUTTONS_SPACE = 14;

    static const int BUTTONS_Y_OFFSET = 30;

    ZoomAnimationBox startUpAnimationBox;

    MoveAnimator<Container> buttonContainers[NUMBER_OF_BUTTONS];
    Button buttons[NUMBER_OF_BUTTONS];
    FadeAnimator<Image> icons[NUMBER_OF_BUTTONS];
    FadeAnimator<MoveAnimator<TextArea> > texts[NUMBER_OF_BUTTONS];

    Callback<MainMenuAnimatingButtonsView, const ZoomAnimationBox&> startUpAnimationBoxDone;
    void StartupAnimationBoxDoneHandler(const ZoomAnimationBox& box);

    Callback<MainMenuAnimatingButtonsView, const MoveAnimator<Container>&> buttonAnimationDone;
    void buttonAnimationDoneHandler(const MoveAnimator<Container>& box);

    Callback<MainMenuAnimatingButtonsView, const AbstractButton&> buttonClicked;
    void buttonClickedHandler(const AbstractButton& button);

    Callback<MainMenuAnimatingButtonsView, const MoveAnimator<TextArea>&> textAnimationDone;
    void textAnimationDoneHandler(const MoveAnimator<TextArea>& t);

    void setSelectedDemoScreen(Defines::DemoID demoId);
    void gotoSelectedDemoScreen();
};

#endif // MAINMENUANIMATINGBUTTONS_VIEW_HPP
