

#include <touchgfx/Application.hpp>
#include <touchgfx/Color.hpp>

#include <BitmapDatabase.hpp>
#include <gui/video_player_screen/PlaylistWidget.hpp>

GenericCallback< const uint32_t >* ThumbNail::pressedAction = 0;

ThumbNail::ThumbNail()
    : thumbCallback(this, &ThumbNail::onThumbNailPressed)
{
    setTouchable(true);

    setWidth(190);
    setHeight(114);
    overlay.setBitmap(Bitmap(BITMAP_PLAY_OVERLAY_GRID_ID));

    Bitmap thumbBitmap = Bitmap(BITMAP_EMPTY_SLOT_ID);
    Bitmap playBitmap = Bitmap(BITMAP_PLAY_OVERLAY_GRID_ID);

    const uint32_t play_x = (thumbBitmap.getWidth()-playBitmap.getWidth())/2;
    const uint32_t play_y = (thumbBitmap.getHeight()-playBitmap.getHeight())/2;

    overlay.setXY(play_x, play_y);

    thumbNail.setAction(thumbCallback);

    add(thumbNail);
    add(overlay);
}

void ThumbNail::onThumbNailPressed(const AbstractButton& )
{
    if (pressedAction && pressedAction->isValid())
    {
        pressedAction->execute(index);
    }
}

void ThumbNail::setIndex(uint32_t index)
{
    this->index = index;
}

void ThumbNail::setThumbNail(BitmapId bmpId)
{
    thumbNail.setBitmaps(Bitmap(bmpId), Bitmap(bmpId));
}

void ThumbNail::setPressedAction(GenericCallback< const uint32_t >& callback)
{
    pressedAction = &callback;
}

void PlaylistWidget::setPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    Container::setPosition(x, y, width, height);

    //background
    background.setBitmap(Bitmap(BITMAP_GRID_BACKGROUND_ID));
    background.setPosition(0, 0, width, height);
    
    scroller.setWidth(getWidth());
    scroller.setHeight(getHeight());
}

PlaylistWidget::PlaylistWidget(PlaylistElement* list,
                   GenericCallback< const uint32_t >& movieClickCallback,
                   GenericCallback< const AbstractButton& >& backCallback)
    : playlist(list), backOut(false)
{
    ThumbNail::setPressedAction(movieClickCallback);    
    backButton.setAction(backCallback);

    //thumbnails
    const uint32_t cols = (PLAYLIST_LENGTH+2)/3;
    Bitmap thumbBitmap = Bitmap(BITMAP_EMPTY_SLOT_ID);
    
    //size of scrollable list
    holder.setPosition(0, 20, 80+cols*(thumbBitmap.getWidth() + 20), 3*thumbBitmap.getHeight()+2*20);

    uint32_t thumbInx = 0;

    for (uint32_t i = 0; i<cols; i++)
    {
        const uint32_t x = 80+i*(thumbBitmap.getWidth() + 20);
        for (uint32_t j = 0; j<3 && thumbInx < PLAYLIST_LENGTH; j++)
        {
            const uint32_t y = j*(thumbBitmap.getHeight() + 20);
            thumbNails[thumbInx].setXY(x, y);
            thumbNails[thumbInx].setVisible(false);
            thumbNails[thumbInx].setIndex(thumbInx);

            thumbInx++;
        }
    }

    backContainer.setPosition(0, 0, 80, 480);
    backBackground.setBitmap(Bitmap(BITMAP_BACK_BAR_ID));
    backButton.setBitmaps(Bitmap(BITMAP_BACK_ICON_ID), Bitmap(BITMAP_BACK_ICON_ID));
    backButton.setXY(8, 480/2-70-Bitmap(BITMAP_BACK_ICON_ID).getHeight()/2);

    add(background);
    add(scroller);
    scroller.add(holder);
    add(backContainer);
    backContainer.add(backBackground);
    backContainer.add(backButton);
}

void PlaylistWidget::refreshPlaylist()
{
    uint32_t movieCount = 0;
    scroller.invalidate();
    holder.removeAll();
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (playlist[i].used)
        {
            thumbNails[i].setThumbNail(playlist[i].bmpId);
            thumbNails[i].setVisible(true);
            thumbNails[i].invalidate();
            holder.add(thumbNails[i]);

            movieCount++;
        }
    }
    const uint32_t cols = (movieCount+2)/3;
    const Bitmap thumbBitmap = Bitmap(BITMAP_EMPTY_SLOT_ID);

    holder.setPosition(0, 20, 80+cols*(thumbBitmap.getWidth() + 20), 3*thumbBitmap.getHeight()+2*20);
}

void PlaylistWidget::updateUI()
{
    if (getX() < 0)
        return;

    //current horizontal scroll
    int32_t scrollx = holder.getX();
    //maximum scroll
    int32_t maxx = holder.getWidth() - 800;
    if ((scrollx < -150 || scrollx == -maxx) && !backOut)
    {
        backContainer.startMoveAnimation(-80, 0, 16, &EasingEquations::cubicEaseInOut);
        backOut = true;
    }
    else if (scrollx > -80 && scrollx < 0 && backOut)
    {
        backContainer.moveTo(scrollx, 0);
    }
    else if (scrollx >= 0 && backOut)
    {
        backContainer.moveTo(0, 0);
        backOut = false;
    }
}
