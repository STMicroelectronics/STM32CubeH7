#ifndef FANKNOB_HPP
#define FANKNOB_HPP

#include <gui_generated/containers/FanKnobBase.hpp>
#include <touchgfx/Callback.hpp>

class FanKnob : public FanKnobBase
{
public:
    FanKnob();
    virtual ~FanKnob() {}

    virtual void initialize();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void setKnobUpdateAction(GenericCallback<uint16_t>& callback);

    void setFanSpeed(uint16_t speed);
    uint16_t getFanSpeed();

protected:
    GenericCallback<uint16_t>* knobUpdate;
    void moveToNearestX(uint16_t x);
    uint16_t fanSpeed;
    uint16_t fixedPos[5];
    bool isDragging;
};

#endif // FANKNOB_HPP
