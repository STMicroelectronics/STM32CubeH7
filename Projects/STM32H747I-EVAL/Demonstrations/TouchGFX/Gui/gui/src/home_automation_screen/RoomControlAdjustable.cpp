

#include <gui/home_automation_screen/RoomControlAdjustable.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <gui/common/Utils.hpp>

RoomControlAdjustable::RoomControlAdjustable() :
    RoomControl()
{
    selectedItemIndex = -1;

    circleColorPainter.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        roomValues[i] = Utils::randomNumberBetween(10, 90);

        circle[i].setPosition(rooms[i].getX(), rooms[i].getY(), rooms[i].getWidth(), rooms[i].getHeight());
        circle[i].setCenter(circle[i].getWidth() / 2, circle[i].getHeight() / 2);
        circle[i].setRadius(51);
        circle[i].setLineWidth(2);
        circle[i].setPainter(circleColorPainter);
        add(circle[i]);

        updateRoomCircle(i);
    }

    cancelSelectionTouchArea.setPosition(0, 0, getWidth(), getHeight());
    cancelSelectionTouchArea.setPressedAction(buttonClickedCallback);
    cancelSelectionTouchArea.setTouchable(false);
    add(cancelSelectionTouchArea);

    centerCircleControl.setXY(238, 56);
    centerCircleControl.setVisible(false);
    add(centerCircleControl);

}

RoomControlAdjustable::~RoomControlAdjustable()
{
}

void RoomControlAdjustable::buttonClicked(const AbstractButton& source)
{
    if (&source == &cancelSelectionTouchArea)
    {
        for (int i = 0; i < NUMBER_OF_ROOMS; i++)
        {
            rooms[i].setAlpha(255);
            rooms[i].invalidate();

            circle[i].setAlpha(255);
            circle[i].invalidate();
        }

        roomValues[selectedItemIndex] = centerCircleControl.getValue();
        updateRoomCircle(selectedItemIndex);


        selectedItemIndex = -1;
        updateCenterCircle();
        cancelSelectionTouchArea.setTouchable(false);
    }
    else
    {
        // RoomItems
        if (!isItemSelected())
        {
            for (int i = 0; i < NUMBER_OF_ROOMS; i++)
            {
                if (&source != &(rooms[i]))
                {
                    rooms[i].setAlpha(80);
                    rooms[i].invalidate();

                    circle[i].setAlpha(80);
                    circle[i].invalidate();
                }
                else
                {
                    selectedItemIndex = i;
                }
            }

            centerCircleControl.setVisible(true);
            centerCircleControl.invalidate();
            updateCenterCircle();
            cancelSelectionTouchArea.setTouchable(true);
        }

    }

}

bool RoomControlAdjustable::isItemSelected()
{
    return (selectedItemIndex >= 0) && (selectedItemIndex < NUMBER_OF_ROOMS);
}

void RoomControlAdjustable::updateRoomCircle(int index)
{
    circle[index].updateArcEnd((int)(roomValues[index] * 3.6f));
}

void RoomControlAdjustable::updateCenterCircle()
{
    if (isItemSelected())
    {
        centerCircleControl.updateArcEnd((int)(roomValues[selectedItemIndex] * 3.6f));
    }
    else
    {
        centerCircleControl.setVisible(false);
        centerCircleControl.invalidate();
    }
}

void RoomControlAdjustable::updateControlWheel(int16_t degree)
{
    centerCircleControl.updateArcEnd(degree);
}
