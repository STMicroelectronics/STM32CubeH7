#ifndef ROOM_CONTROL_ADJUSTABLE_HPP
#define ROOM_CONTROL_ADJUSTABLE_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <gui/home_automation_screen/RoomControl.hpp>
#include <gui/home_automation_screen/RoomControlWheel.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#else
#error Unknown USE_BPP
#endif

using namespace touchgfx;

class RoomControlAdjustable : public RoomControl
{
public:
    RoomControlAdjustable();
    virtual ~RoomControlAdjustable();

    void updateControlWheel(int16_t degree);
private:
    int selectedItemIndex;

    uint8_t roomValues[NUMBER_OF_ROOMS];

    RoomControlWheel centerCircleControl;
    Circle circle[NUMBER_OF_ROOMS];
#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 circleColorPainter;
#elif USE_BPP==24
    PainterRGB888 circleColorPainter;
#else
#error Unknown USE_BPP
#endif

    TouchArea cancelSelectionTouchArea;

    virtual void buttonClicked(const AbstractButton& source);

    bool isItemSelected();
    void updateRoomCircle(int index);
    void updateCenterCircle();
};

#endif /* ROOM_CONTROL_ADJUSTABLE_HPP */
