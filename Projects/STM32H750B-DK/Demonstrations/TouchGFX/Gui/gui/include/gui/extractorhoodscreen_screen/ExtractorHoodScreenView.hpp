#ifndef EXTRACTORHOODSCREEN_VIEW_HPP
#define EXTRACTORHOODSCREEN_VIEW_HPP

#include <gui_generated/extractorhoodscreen_screen/ExtractorHoodScreenViewBase.hpp>
#include <gui/extractorhoodscreen_screen/ExtractorHoodScreenPresenter.hpp>

#include <gui/extractorhoodscreen_screen/AnimatedImage2.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <gui/containers/FpsMcuLoad.hpp>

class ExtractorHoodScreenView : public ExtractorHoodScreenViewBase
{
public:
    ExtractorHoodScreenView();
    virtual ~ExtractorHoodScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    // virtual functions created with the designer, to turn the fan and light on an off
    virtual void lightButtonClicked();
    virtual void fanButttonClicked();

    // updates the fan and light buttons state
    void setFanButtonState(bool on);
    void setLightButtonState(bool on);

    // Starts, stops and set the fans speed based on the stored states for the fan
    void fanControlUpdate();

    // Updates the MCU load displayed on the screen
    void setMcuLoad(uint8_t load);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);
    void screenDrag(DragEvent::DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY);

protected:
    enum FAN_SPEED
    {
        SPEED_1,
        SPEED_2,
        SPEED_3,
        SPEED_4,
        SPEED_5
    };

    //The Callback is registerd each when a speed is set with the fan knob.
    Callback<ExtractorHoodScreenView, uint16_t> fanKnobUpdateCallback;
    void fanKnobUpdateHandler(uint16_t speed);

    AnimatedImage2 fanAnimation; //An updated version of the AnimatedImage widgets which handles the fan animation

    TextureMapper textureMapperFan;
    float zAngle2D;
    int animationCounter, scrollDuration, beginningValue, targetValue;
    bool animate;
    FpsMcuLoad fps_mcu;

    uint16_t autoDemoCounter;
};

#endif // EXTRACTORHOODSCREEN_VIEW_HPP
