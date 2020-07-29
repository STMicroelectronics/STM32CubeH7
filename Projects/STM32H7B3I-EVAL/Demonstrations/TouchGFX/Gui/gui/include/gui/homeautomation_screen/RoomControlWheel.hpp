#ifndef ROOM_CONTROL_WHEEL_HPP
#define ROOM_CONTROL_WHEEL_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#else
#error Unknown USE_BPP
#endif

using namespace touchgfx;

class RoomControlWheel : public Container
{
public:
    RoomControlWheel();
    virtual ~RoomControlWheel();

    void handleClickEvent(const ClickEvent& evt);
    void handleDragEvent(const DragEvent& evt);

    void updateArcEnd(int newDegreeValue);
    void showKnob(int newDegreeValue);

    int getValue();
protected:
    Circle circle;
    Circle knob;

#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 circleColorPainter;
    PainterRGB565 knobColorPainter;
#elif USE_BPP==24
    PainterRGB888 circleColorPainter;
    PainterRGB888 knobColorPainter;
#else
#error Unknown USE_BPP
#endif

    TextAreaWithOneWildcard percent;
    Unicode::UnicodeChar percentBuffer[4];
    TextArea percentSign;

    void setSelectedPosition(int newX, int newY);
};

#endif /* ROOM_CONTROL_WHEEL_HPP */
