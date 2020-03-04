
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
    rooms[9].setBitmaps(Bitmap(BITMAP_HOME_ROOM_IMAGE_09_ID), Bitmap(BITMAP_HOME_ROOM_IMAGE_09_ID));

    rooms[0].setXY(38, 28);
    rooms[1].setXY(245, 16);
    rooms[2].setXY(144, 128);
    rooms[3].setXY(25, 234);
    rooms[4].setXY(220, 267);
    rooms[5].setXY(404, 277);
    rooms[6].setXY(492, 20);
    rooms[7].setXY(646, 73);
    rooms[8].setXY(536, 188);
    rooms[9].setXY(670, 261);

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
