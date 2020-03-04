#include <gui/homeautomation_screen/RoomControlToggle.hpp>

RoomControlToggle::RoomControlToggle() :
    RoomControl()
{
    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        roomsSelected[i].setXY(rooms[i].getX(), rooms[i].getY());
        roomsSelected[i].setVisible(false);

        add(roomsSelected[i]);
    }
}

RoomControlToggle::~RoomControlToggle()
{
}

void RoomControlToggle::setup(BitmapId selected, bool* state)
{
    roomValues = state;

    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        roomsSelected[i].setBitmap(Bitmap(selected));
        roomsSelected[i].setVisible(roomValues[i]);
    }
}

void RoomControlToggle::buttonClicked(const AbstractButton& source)
{
    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        if (&source == &(rooms[i]))
        {
            roomValues[i] = !roomValues[i];
            roomsSelected[i].setVisible(roomValues[i]);
            roomsSelected[i].invalidate();
        }
    }
}
