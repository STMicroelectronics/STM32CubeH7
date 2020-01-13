#ifndef OVENTEMPERATURECONTAINER_HPP
#define OVENTEMPERATURECONTAINER_HPP

#include <gui_generated/containers/OvenTemperatureContainerBase.hpp>
#include <touchgfx/EasingEquations.hpp>

static const int WHEEL_STEPS = 8;

class OvenTemperatureContainer : public OvenTemperatureContainerBase
{
public:
    OvenTemperatureContainer();
    virtual ~OvenTemperatureContainer() {}

    virtual void initialize();

    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleGestureEvent(const GestureEvent& evt);
    virtual void handleTickEvent();

    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

    void setCurrentTemp(uint16_t temp);
    uint16_t getCurrnetTemp();

    void stopWheel();
    void gestureWheel(int16_t moved);
    int squareRoot(int x);

protected:
    uint16_t currentTemp;

    int16_t lastDragX;
    uint16_t firstImage;
    uint16_t lastImage;
    int16_t currentImage;
    bool wheelClicked;

    uint16_t scrollDuration;
    int16_t beginningValue; 
    int16_t targetValue;   
    uint16_t animationCounter; 
    bool animate;             

    int16_t lastGesture;
    int16_t gestureSteps;
    bool gestureRight;
};

#endif // OVENTEMPERATURECONTAINER_HPP
