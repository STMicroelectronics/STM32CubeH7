

#ifndef ROOM_CONTROL_ADJUSTABLE_HPP
#define ROOM_CONTROL_ADJUSTABLE_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <gui/home_automation_screen/RoomControl.hpp>
#include <gui/home_automation_screen/RoomControlWheel.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>

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
    PainterRGB888 circleColorPainter;

    TouchArea cancelSelectionTouchArea;

    virtual void buttonClicked(const AbstractButton& source);

    bool isItemSelected();
    void updateRoomCircle(int index);
    void updateCenterCircle();
};

#endif /* ROOM_CONTROL_ADJUSTABLE_HPP */
