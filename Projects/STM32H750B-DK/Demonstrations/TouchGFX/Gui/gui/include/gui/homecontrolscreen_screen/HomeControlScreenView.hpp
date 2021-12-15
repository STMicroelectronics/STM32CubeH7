#ifndef HOMECONTROLSCREEN_VIEW_HPP
#define HOMECONTROLSCREEN_VIEW_HPP

#include <gui_generated/homecontrolscreen_screen/HomeControlScreenViewBase.hpp>
#include <gui/homecontrolscreen_screen/HomeControlScreenPresenter.hpp>

class HomeControlScreenView : public HomeControlScreenViewBase
{
public:
    HomeControlScreenView();
    virtual ~HomeControlScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleClickEvent(const ClickEvent& evt);

    // Updates the elements in the scrollWheelRoomSelector based on the scroll wheel items, index value.
    virtual void scrollWheelRoomSelectorUpdateItem(HomeUIWheelElement& item, int16_t itemIndex);
    virtual void scrollWheelRoomSelectorUpdateCenterItem(HomeUIWheelElement& item, int16_t itemIndex);

    // 
    virtual void scrollWheelRoomInfoUpdateItem(RoomInfoWheelElements& item, int16_t itemIndex);

    // Updates the elements in the two device lists based on the items, index value.
    // The stored states is displayed for each corresponding item.
    virtual void scrollListBathroomDevicesUpdateItem(DeviceInfoWheelElements& item, int16_t itemIndex);
    virtual void scrollListKitchenDevicesUpdateItem(DeviceInfoWheelElements& item, int16_t itemIndex);

    void setAlphaScrollListBathroomDevices(uint8_t alpha);
    void setAlphaScrollListKitchenDevices(uint8_t alpha);


    // Virtual functions created with the designer
    virtual void radioDevicesSelected();
    virtual void radioTemperatureSelected();
    virtual void roomInfoLeftClicked();
    virtual void roomInfoRightClicked();
    virtual void backButtonPressed();
    virtual void goToPrevRoom();
    virtual void buttonMuiscRecipesPressed();
    virtual void buttonPersPressed();

    // Switches the content scrollWheelRoomSelector to display device info or temperature
    void switchWheelItemsContent(bool devices);

    // Functions to fade the different scroll whells/list and the bottombar
    void fadeBottombar(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void fadeScrollWheelRoomSelector(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void fadeScrollListBathroomDevices(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    void fadeScrollListKitchenDevices(uint8_t endAlpha, uint16_t duration, uint16_t delay = 0, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);

    void setSplashScreenState(bool show);
    void setHomeUIRoom(bool isBathroom);

    void updateMCULoad(uint8_t load);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);

protected:
    enum HOMEUI_MENU
    {
        ROOM_SELECTOR_TEMPERATURE,
        ROOM_SELECTOR_DEVICES,
        ROOM_INFO,
        ROOM_DEVICES
    } currentMenu;

    enum KITCHEN_DEVICES
    {
        OVEN_UI,
        HOME_TEMPERATURE_UI,
        HOOD_UI,
        HOME_RECIPE_UI
    };

    enum BATHROOM_DEVICES
    {
        BATHTUB_UI,
        WASHER_UI,
        HOME_UI_BATHROOM
    };

    enum WASHER_PROGRAM
    {
        PROGRAM_ACTIVE_WEAR,
        PROGRAM_DELICATES,
        PROGRAM_ECO_WASH,
        PROGRAM_HEAVY_DUTY,
        PROGRAM_NORMAL,
        PROGRAM_QUICK_WASH,
        PROGRAM_SMART_WASH
    };

    enum OVEN_MODE
    {
        OVEN_MODE_LIGHTS,
        OVEN_MODE_OVEN_TOP,
        OVEN_MODE_OVEN_BUTTOM,
        OVEN_MODE_FAN,
        OVEN_MODE_GRILL,
        OVEN_MODE_GRILL_FAN,
        OVEN_MODE_CLEAN
    };

    // The Callback registers when the splash container is clikced to fade it out
    void splashContainerClickedHandler(const HomeUISplashContainer& src, const ClickEvent& evt);
    Callback<HomeControlScreenView, const HomeUISplashContainer&, const ClickEvent&> splashContainerClickedCallback;

    // The Callback registers a animation for the scrollWheelRoomSelector is ended
    Callback<HomeControlScreenView> animationEnddedCallback;
    void animationEnddedHandler();

    // The Callback registers when the scrollWheelRoomSelector is moving to a specific item
    Callback<HomeControlScreenView, int16_t> scrollWheelRoomSelectorAnimateToCallback;
    void scrollWheelRoomSelectorAnimateToHandler(int16_t itemSelected);

    // The Callback registers when the headline is done fading
    Callback<HomeControlScreenView, const FadeAnimator<TextArea>&> textFadeEndedCallback;
    void textFadeEndedHandler(const FadeAnimator<TextArea>& src);

    // The Callback registers when the showing which buttons can be pressed is done
    Callback<HomeControlScreenView, const FadeAnimator<Image>&> imageFadeCallback;
    void imageFadeHandler(const FadeAnimator<Image>& src);

    int16_t lastItemSelected; // The previous item that is selected in scrollWheelRoomSelector
    bool selectedItemDragged; // Used to tell if the current selected item is dragged.
    int16_t totalDrag; // The total amount the scrollWheelRoomSelector has been dragged
    uint16_t fadeCounter; // Counter used for keeping track of how many times the clow animation has reached alpha of 0 or 255.
    bool changeToRecipe;

    uint16_t autoDemoCounter;
};

#endif // HOMECONTROLSCREEN_VIEW_HPP
