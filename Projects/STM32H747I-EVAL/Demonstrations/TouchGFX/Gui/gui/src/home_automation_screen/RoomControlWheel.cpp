

#include <gui/home_automation_screen/RoomControlWheel.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

RoomControlWheel::RoomControlWheel()
{
    setTouchable(true);

    setWidth(326);
    setHeight(248);

    circleColorPainter.setColor(Color::getColorFrom24BitRGB(0x0, 0xC0, 0xFF));

    circle.setPosition(0, 0, getWidth(), getHeight());
    circle.setCenter(circle.getWidth() / 2, circle.getHeight() / 2);
    circle.setRadius(71);
    circle.setLineWidth(2);
    circle.setPainter(circleColorPainter);
    circle.setVisible(true);
    add(circle);

    knobColorPainter.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    knob.setPosition(0, 0, getWidth(), getHeight());
    knob.setRadius(8);
    knob.setLineWidth(0);
    knob.setPainter(knobColorPainter);
    knob.setVisible(false);
    add(knob);

    percentBuffer[0] = 0;
    Unicode::snprintf(percentBuffer, 4, "%d", 100);
    percent.setTypedText(TypedText(T_BLINDS_PERCENT));
    percent.setWildcard(percentBuffer);
    percent.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    int w = percent.getTextWidth();
    percent.setPosition(getWidth() / 2 + 74, getHeight() / 2 - 21, w, percent.getTextHeight());
    percentSign.setTypedText(TypedText(T_BLINDS_PERCENT_SIGN));
    percentSign.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    percentSign.setPosition(getWidth() / 2 + 74 + w, getHeight() / 2 - 11, percentSign.getTextWidth(), percentSign.getTextHeight());
    add(percent);
    add(percentSign);
}

RoomControlWheel::~RoomControlWheel()
{
}


void RoomControlWheel::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        setSelectedPosition(evt.getX(), evt.getY());
    }
}

void RoomControlWheel::handleDragEvent(const DragEvent& evt)
{
    setSelectedPosition(evt.getNewX(), evt.getNewY());
}

void RoomControlWheel::updateArcEnd(int newDegreeValue)
{
    circle.updateArcEnd(newDegreeValue);
    showKnob(newDegreeValue);
    percent.invalidate();
    Unicode::snprintf(percentBuffer, 4, "%d", getValue());
    percent.invalidate();
}

int RoomControlWheel::getValue()
{
    return (int)(circle.getArcEnd() / 3.6f);
}

void RoomControlWheel::setSelectedPosition(int newX, int newY)
{
    int centerX;
    int centerY;
    circle.getCenter(centerX, centerY);
    int16_t vx = newX - centerX;
    int16_t vy = newY - centerY;

    int length;
    int angleInDegrees = CWRUtil::angle<int>(vx, vy, length);

    if (length < 30 || length > 120)
    {
        return;
    }
    else if ((circle.getArcEnd() > 270) && (angleInDegrees < 90))
    {
        angleInDegrees = 360;
    }
    else if ((angleInDegrees > 270) && (circle.getArcEnd() < 90))
    {
        angleInDegrees = 0;
    }

    updateArcEnd(angleInDegrees);
}

void RoomControlWheel::showKnob(int newDegreeValue)
{
    float x = getWidth() / 2 + (CWRUtil::toQ5<int>(71) * CWRUtil::sine(newDegreeValue)).to<float>();
    float y = getHeight() / 2 - (CWRUtil::toQ5<int>(71) * CWRUtil::cosine(newDegreeValue)).to<float>();
    knob.invalidate();
    knob.setCenter(x, y);
    knob.setVisible(true);
    knob.invalidate();
}

