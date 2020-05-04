#ifndef HOME_AUTOMATION_VIEW_HPP
#define HOME_AUTOMATION_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/common/LoginWidget.hpp>
#include <gui/home_automation_screen/HomeAutomationPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/home_automation_screen/RoomControlToggle.hpp>
#include <gui/home_automation_screen/RoomControlAdjustable.hpp>
#include <gui/home_automation_screen/StatisticsContainer.hpp>

using namespace touchgfx;

class HomeAutomationView : public DemoView<HomeAutomationPresenter>
{
public:
    HomeAutomationView();
    virtual ~HomeAutomationView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();

    void loginWidgetAnimEndedCallbackHandler(const MoveAnimator<LoginWidget> &widget);

    void screenClick(ClickEvent::ClickEventType type, int x, int y);
    void screenCircleUpdate(int16_t degree);
    void screenDrag(int16_t x_old, int16_t y_old, int16_t x_new, int16_t y_new);

private:

    static const int CANVAS_BUFFER_SIZE = 1024 * 16;
    static const int NUMBER_OF_MAIN_SCREEN_BUTTONS = 3;

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

    Image background;
    Button backButton;

    TextArea headline;

    MoveAnimator<Container> mainScreenButtonContainer;
    MoveAnimator<FadeAnimator<Button> > mainScreenButton[NUMBER_OF_MAIN_SCREEN_BUTTONS];
    Callback<HomeAutomationView, const MoveAnimator<FadeAnimator<Button> >& > mainScreenButtonMoveAnimationEndedCallback;
    void mainScreenButtonMoveAnimationEndedHandler(const MoveAnimator<FadeAnimator<Button> > &src);

    Callback<HomeAutomationView, const AbstractButton &> buttonClickedCallback;
    void mainScreenButtonClicked(const AbstractButton &source);

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
};

#endif // HOME_AUTOMATION_VIEW_HPP
