#include <touchgfx/hal/HAL.hpp>
#include <gui/home_automation_screen/RoomControl.hpp>
#include <BitmapDatabase.hpp>

RoomControl::RoomControl() :
    buttonClickedCallback(this, &RoomControl::buttonClicked)
{
    rooms[0].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_00_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_00_ID));
    rooms[1].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_01_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_01_ID));
    rooms[2].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_02_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_02_ID));
    rooms[3].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_03_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_03_ID));
    rooms[4].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_04_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_04_ID));
    rooms[5].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_05_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_05_ID));
    rooms[6].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_06_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_06_ID));
    rooms[7].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_07_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_07_ID));
    rooms[8].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_08_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_08_ID));

    rooms[0].setXY(34, 24);
    rooms[1].setXY(516, 214);
    rooms[2].setXY(116, 136);
    rooms[3].setXY(30, 244);
    rooms[4].setXY(200, 267);
    rooms[5].setXY(374, 277);
    rooms[6].setXY(372, 20);
    rooms[7].setXY(476, 94);
    rooms[8].setXY(186, 14);

    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        rooms[i].setAction(buttonClickedCallback);

        add(rooms[i]);
    }

    setWidth(HAL::DISPLAY_WIDTH);
    setHeight(HAL::DISPLAY_HEIGHT - 84);
}

RoomControl::~RoomControl()
{
}
