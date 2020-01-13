#ifndef OVENSCREEN_VIEW_HPP
#define OVENSCREEN_VIEW_HPP

#include <gui_generated/ovenscreen_screen/OvenScreenViewBase.hpp>
#include <gui/ovenscreen_screen/OvenScreenPresenter.hpp>

#include <gui/containers/OvenTimerContainer.hpp>
#include <gui/containers/OvenTemperatureContainer.hpp>
#include <gui/containers/OvenSplashScreen.hpp>

#include <touchgfx/mixins/ClickListener.hpp>

class OvenScreenView : public OvenScreenViewBase
{
public:
    OvenScreenView();
    virtual ~OvenScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleTickEvent();

    // Updates the elements in the scrollWheelOvenProgram based on the scroll wheel items, index value.
    virtual void scrollWheelOvenProgramUpdateItem(OvenWheelElement& item, int16_t itemIndex);
    virtual void scrollWheelOvenProgramUpdateCenterItem(OvenWheelElement& item, int16_t itemIndex);
    
    // Virtual functions created with the designer
    virtual void buttonTurnOnOff();
    virtual void tempButtonClicked();
    virtual void timerButtonClicked();
    virtual void okButtonClicked();

    // Functions to fade scrollWheelOvenProgram and other elements reading the oven mode selector 
    void fadeOvenModeWheel(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void fadeOvenModeElements(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

    // Updates the MCU load displayed on the screen
    void setMcuLoad(uint8_t load);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);
    void screenDrag(DragEvent::DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY);
    void screenGesture(GestureEvent::GestureType type, int16_t v, int16_t x_coord, int16_t y_coord);

protected:
    enum OvenState
    {
        OVEN_MODE,
        OVEN_TEMPERATURE,
        OVEN_TIMER
    } OvenState;

    // Callback that registers when the splash screen is pressed
    Callback<OvenScreenView> splashScreenPressedCallback;
    void splashScreenPressedHandler();

    // The Callback registers when scrollWheelOvenProgram has animated to a specific item
    // then updates the state for the oven and cahnges the headline.
    Callback<OvenScreenView> animationEnddedCallback;
    void animationEnddedHandler();

    // The Callback registers when the scrollWheelRoomSelector is moving towards a specific item
    Callback<OvenScreenView, int16_t> scrollWheelOvenProgramAnimateToCallback;
    void scrollWheelOvenProgramAnimateToHandler(int16_t itemSelected);

    // The Callback registers when the slpash screen is done fadding
    Callback<OvenScreenView, const FadeAnimator<OvenSplashScreen>&> splashScreenFadeEndedCallback;
    void splashScreenFadeEndedHandler(const FadeAnimator<OvenSplashScreen>& src);

    // The Callback registers when the ok button is done fadding
    Callback<OvenScreenView, const FadeAnimator<Button>&> buttonFadeEndedCallback;
    void buttonFadeEndedHandler(const FadeAnimator<Button>& src);

    // The three custom containers OvenTimerContainer, OvenTemperatureContainer and OvenSplashScreen,
    // created with the touchgfx designer is added in code, to attach
    // the FadeAnimator mixin to them.
    FadeAnimator<OvenTimerContainer> ovenTimerMenu;
    FadeAnimator<OvenTemperatureContainer> ovenTempMenu;
    FadeAnimator<OvenSplashScreen> ovenSplashScreen;

    int16_t currentItem; // holds the value for the last item selected registerd as selected item.
    uint16_t colorCnt;   // Counter used when changing the color of the two text areas textAreaTemperature and textAreaTimer
    colortype currentColor; // the current color of either textAreaTemperature or textAreaTimer
    colortype goalColor; // the goal color for textAreaTemperature or textAreaTimer when changing thier color.
    bool changeColorTimer, changeColorTemp; // tells hanldeTick event which textarea is changing color
    bool selectedItemDragged; // informs if the slecteditem in scrollWheelRoomSelector has been dragged
    int16_t totalDrag; // The total drag amount when dragging scrollWheelRoomSelector.

    uint16_t autoDemoCounter;
};

#endif // OVENSCREEN_VIEW_HPP
