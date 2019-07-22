#include <gui/containers/FanKnob.hpp>

FanKnob::FanKnob() :
    fanSpeed(0),
    isDragging(false)

{    
    fixedPos[0] = 100;
    fixedPos[1] = 170;
    fixedPos[2] = 239;
    fixedPos[3] = 308;
    fixedPos[4] = 379;
}

void FanKnob::initialize()
{
    FanKnobBase::initialize();
    setTouchable(true);
}

void FanKnob::moveToNearestX(uint16_t x)
{
    uint32_t lowestDeltaX = (fixedPos[0] - x) * (fixedPos[0] - x);
    uint16_t newSpeed = 0;

    for (int cnt = 1; cnt < 5; cnt++)
    {
        if (uint32_t((fixedPos[cnt] - x) * (fixedPos[cnt] - x)) < lowestDeltaX)
        {
            lowestDeltaX = ((fixedPos[cnt] - x) * (fixedPos[cnt] - x));
            newSpeed = cnt;
        }
    }
    setFanSpeed(newSpeed);

    if (knobUpdate && knobUpdate->isValid())
    {
        knobUpdate->execute(newSpeed);
    }
}

void FanKnob::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED && isDragging)
    {
        moveToNearestX(containerKnob.getX() + containerKnob.getWidth() / 2);
        isDragging = false;
    }
    if (evt.getType() == ClickEvent::CANCEL)
    {
        moveToNearestX(containerKnob.getX() + containerKnob.getWidth() / 2);
        isDragging = false;
    }
}

void FanKnob::handleDragEvent(const DragEvent& evt)
{
    if (evt.getOldX() >= containerKnob.getX() && evt.getOldX() < (containerKnob.getX() + containerKnob.getWidth()))
    {
        isDragging = true;
        if ((containerKnob.getX() + evt.getDeltaX() > 60) &&
                (containerKnob.getX() + evt.getDeltaX() < 340))
        {
            containerKnob.moveRelative(evt.getDeltaX(), 0);
        }

        if (containerKnob.getX() + evt.getDeltaX() <= 60)
        {
            containerKnob.moveTo(60, containerKnob.getY());
        }

        if (containerKnob.getX() + evt.getDeltaX() >= 340)
        {
            containerKnob.moveTo(340, containerKnob.getY());
        }
    }
}

void FanKnob::setKnobUpdateAction(GenericCallback <uint16_t>& callback)
{
    knobUpdate = &callback;
}

void FanKnob::setFanSpeed(uint16_t speed)
{
    fanSpeed = speed;
    Unicode::snprintf(textAreaKnobBuffer, TEXTAREAKNOB_SIZE, "%d", fanSpeed + 1);
    textAreaKnob.invalidate();
    containerKnob.moveTo(fixedPos[fanSpeed] - (containerKnob.getWidth() / 2), containerKnob.getY());
}

uint16_t FanKnob::getFanSpeed()
{
    return fanSpeed;
}
