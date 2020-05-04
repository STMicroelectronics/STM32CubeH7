

#include <gui/common/BottomBar.hpp>
#include <texts/TextKeysAndLanguages.hpp>

BottomBar::BottomBar()
    : buttonCallback(this, &BottomBar::buttonClicked),
      actionCallback(0)
{

    background.setBitmap(Bitmap(BITMAP_COMMON_PLAYER_BAR_ID));

    //size matches movie player bar bitmap
    setWidth(background.getWidth());
    setHeight(background.getHeight());

    playlistButton.setBitmaps(Bitmap(BITMAP_GRID_ID), Bitmap(BITMAP_GRID_ID));
    folderButton.setBitmaps(Bitmap(BITMAP_FOLDER_ID), Bitmap(BITMAP_FOLDER_ID));

    playlistButton.setAction(buttonCallback);
    folderButton.setAction(buttonCallback);

    folderButton.setXY(getWidth() - (20 + 64), 0);
    playlistButton.setXY(getWidth() - (20 + 64 * 2), 0);

    add(background);
    add(playlistButton);
    add(folderButton);
}

void BottomBar::buttonClicked(const AbstractButton& btn)
{
    if (&btn == &playlistButton)
    {
        if (actionCallback) //call external callback
        {
            actionCallback->execute(PLAYLIST);
        }
    }
    else if (&btn == &folderButton)
    {
        if (actionCallback) //call external callback
        {
            actionCallback->execute(FOLDER);
        }
    }
}

void BottomBar::setActionCallback(GenericCallback<Action>& callback)
{
    actionCallback = &callback;
}
