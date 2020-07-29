#ifndef WASHERCONTAINER_HPP
#define WASHERCONTAINER_HPP

#include <gui_generated/containers/WasherContainerBase.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <gui/containers/FpsMcuLoad.hpp>

class WasherContainer : public WasherContainerBase
{
public:
    WasherContainer();
    virtual ~WasherContainer() {}

    virtual void initialize();

    virtual void washerWheelUpdateItem(WasherWheelItem& item, int16_t itemIndex);
    virtual void washerWheelUpdateCenterItem(WasherWheelCenterItem& item, int16_t itemIndex);

    void tickEvent();
    void clickEvent(const ClickEvent& event);

    virtual void rightButtonClicked();
    virtual void leftButtonClicked();
    virtual void buttonRunClicked();
    virtual void buttonPausedClicked();

    void setProgramTextAndTime(int16_t item);
    void fadeOutProgramTexts();
    void fadeOutProgramTextsNow();
    void updateProgramTexts();
    void gotoWaitingStart();

    Bitmap getSelectedIconBitmap(int16_t index);
    Bitmap getIconBitmap(int16_t index);

    void gotoWashingPrewash();
    void gotoWashingDone();
    void gotoWashingSpinning();
    void gotoWashingRinsing();
    void gotoWashingWashing();

    uint16_t getWashingProgram();
    uint16_t getWashingTime();
    void setWashingProgram(uint16_t program);

    void resetContainer();

    void updateMcuLoad(uint8_t load);

protected:
    enum
    {
        NUM_ICONS_IN_MENU = 5,
        TOUCH_START_TIMEOUT = 120,
        START_MOVE_ANIMATION_DURATION = 45,
        START_MOVE_ANIMATION_LENGTH = 440,
        START_MOVE_FADE_OUT_DURATION = 10,
        START_MOVE_FADE_IN_DURATION = 20,
        START_MOVE_FLYIN_DURATION = 12,
        START_MOVE_FLYIN_INTERVAL = 12,
        START_MOVE_FLYIN_DELAY = 60
    };

    enum AnimationState
    {
        WAITING, //default
        WAITING_START,
        STARTING,
        WASHING_PREWASH,
        WASHING_WASHING,
        WASHING_RINSING,
        WASHING_SPINNING,
        WASHING_DONE
    } animationState;

    Callback<WasherContainer, int16_t> washerWheelAnimateToCallback;
    void washerWheelAnimateToHandler(int16_t item);

    Callback<WasherContainer, const FadeAnimator<TextArea>& > fadeAnimatonEndedCallBack;
    void fadeAnimationEnddedHandler(const FadeAnimator<TextArea>& txt);

    Callback<WasherContainer, const AbstractButton& > buttonPressedCallBack;
    void buttonPressedHandler(const AbstractButton& src);

    Callback<WasherContainer, const AnimatedImage&> centerAnimatedImageCallback;
    void centerAnimatedImageDone(const AnimatedImage&);

    int16_t currentItem;
    bool paused;
    uint16_t touchTimer;
    uint32_t animationTimer;
    uint16_t currentProgram;
    uint16_t washingTime;

    FpsMcuLoad mcuLoad;
};

#endif // WASHERCONTAINER_HPP
