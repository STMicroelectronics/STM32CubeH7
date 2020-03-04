#ifndef HOMEAUTOMATION_VIEW_HPP
#define HOMEAUTOMATION_VIEW_HPP

#include <gui_generated/homeautomation_screen/HomeAutomationViewBase.hpp>
#include <gui/homeautomation_screen/HomeAutomationPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/homeautomation_screen/RoomControlToggle.hpp>
#include <gui/homeautomation_screen/RoomControlAdjustable.hpp>
#include <gui/homeautomation_screen/StatisticsContainer.hpp>
#include <gui/homeautomation_screen/LoginWidget.hpp>

class HomeAutomationView : public HomeAutomationViewBase
{
public:
    HomeAutomationView();
    virtual ~HomeAutomationView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void loginWidgetAnimEndedCallbackHandler(const MoveAnimator<LoginWidget>& widget);

    void mcuLoadUpdated(uint8_t newMcuLoad);
    void screenSaverMinorTick();
protected:

    static const int CANVAS_BUFFER_SIZE = 1024 * 16;
    static const int NUMBER_OF_MAIN_SCREEN_BUTTONS = 4;

    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

    enum ScreenState
    {
        MAIN,
        LIGHT,
        BLINDS,
        LOCK,
        STATISTICS,
        NO_STATE
    };

    ScreenState currentScreenState;
    bool loginSuccess;

    MoveAnimator<Container> mainScreenButtonContainer;
    MoveAnimator<FadeAnimator<Button> > mainScreenButton[NUMBER_OF_MAIN_SCREEN_BUTTONS];
    Callback<HomeAutomationView, const MoveAnimator<FadeAnimator<Button> >& > mainScreenButtonMoveAnimationEndedCallback;
    void mainScreenButtonMoveAnimationEndedHandler(const MoveAnimator<FadeAnimator<Button> >& src);

    Callback<HomeAutomationView, const AbstractButton&> buttonClickedCallback;
    void mainScreenButtonClicked(const AbstractButton& source);

    // State for the light/lock selector. Placed here for demo purpose
    // in real application they should be placed in the model and communicated
    // to the control system
    bool lightSelected[RoomControl::NUMBER_OF_ROOMS];
    bool lockSelected[RoomControl::NUMBER_OF_ROOMS];

    RoomControlToggle roomControlToggle;
    RoomControlAdjustable roomControlAdjustable;
    StatisticsContainer statisticsContainer;

    void updateHeadline();

    void activateMainButtons(bool activate);
    void toggleLoginWidget(bool enable);

    void animateMainMenuButtonToCenter(int index);
    void animateToMainMenuButton();

    void showBackButton(bool show);

    int getMainButtonStartX(int index);

    //Login stuff
    MoveAnimator<LoginWidget> loginWidget;
    TouchArea loginWidgetCloseArea;
    Callback<HomeAutomationView> successCallback;
    Callback<HomeAutomationView> failureCallback;
    Callback<HomeAutomationView, const MoveAnimator<LoginWidget>& > loginWidgetAnimEndedCallback;

    Vector<uint8_t, 9> trace;
    Vector<uint8_t, 9> solution;
    uint8_t counter;
    uint8_t failedAttempts;

    //Local callback functions
    void onSuccess();
    void onFailure();
    void resetLogin();

    virtual void backButtonHandler();
};

#endif // HOMEAUTOMATION_VIEW_HPP
