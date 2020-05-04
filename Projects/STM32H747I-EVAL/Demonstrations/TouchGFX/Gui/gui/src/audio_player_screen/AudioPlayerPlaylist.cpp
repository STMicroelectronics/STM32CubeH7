

#include <gui/audio_player_screen/AudioPlayerPlaylist.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

AudioPlayerPlaylist::AudioPlayerPlaylist() :
    numberOfElements(0),
    onRemove(this, &AudioPlayerPlaylist::removeElement)
{
    background.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);    
    background.setBitmap(Bitmap(BITMAP_OVERLAY_BACKGROUND_ID));
    add(background);

    setPosition(0, 0, background.getWidth(), background.getHeight());

    scroll.setPosition(0, 70, background.getWidth(), background.getHeight() - 70);
    scroll.setScrollbarsColor(Color::getColorFrom24BitRGB(0xff, 0xff, 0xff));
    scroll.setScrollbarsAlpha(255);
    scroll.setScrollbarWidth(4);
    scroll.setScrollbarPadding(12);
    add(scroll);

    shadow.setBitmap(Bitmap(BITMAP_OVERLAY_BACKGROUND_SHADOW_ID));
    shadow.setY(70);
    add(shadow);
}

void AudioPlayerPlaylist::setSelectedCallback(GenericCallback<int>& callback)
{
    for (int i = 0; i < maxElements; i++)
    {
        elements[i].setSelectedCallback(callback);
    }
}

void AudioPlayerPlaylist::addSong(BitmapId cover, char *artist, char *song)
{
    assert(numberOfElements < maxElements && "no more elements allowed");
    elements[numberOfElements].set(numberOfElements, cover, artist, song);
    elements[numberOfElements].setXY((numberOfElements % 2) == 0 ? 0 : (HAL::DISPLAY_WIDTH / 2), (numberOfElements / 2) * elements[numberOfElements].getHeight());
    elements[numberOfElements].setRemoveCallback(onRemove);
    elements[numberOfElements].setLineVisibility(false);
    elements[numberOfElements].setPlaying(false);

    if (numberOfElements > 1 && (numberOfElements % 2) == 0)
    {
        elements[numberOfElements - 1].setLineVisibility(true);
        elements[numberOfElements - 2].setLineVisibility(true);
    }
    scroll.add(elements[numberOfElements]);

    numberOfElements++;
}

void AudioPlayerPlaylist::setPlaying(int index)
{
    for (int i = 0; i < maxElements; i++)
    {
        elements[i].setPlaying(index == i);
    }
}

void AudioPlayerPlaylist::removeElement(AudioPlayerPlaylistElement &element)
{

}

void AudioPlayerPlaylist::clear()
{
    for (int i = 0; i < numberOfElements; i++)
    {
        scroll.remove(elements[i]);
    }

    numberOfElements = 0;

}

AudioPlayerPlaylistElement::AudioPlayerPlaylistElement() :
    selectedCallback(0),
    removeCallback(0),
    playlistIndex(-1),
    pressed(false)
{
    setWidth(HAL::DISPLAY_WIDTH / 2);
    setHeight(90);

    cover.setXY(60, 15);
    add(cover);

    coverShadow.setBitmap(Bitmap(BITMAP_PLAYLIST_COVER_SHADOW_ID));
    coverShadow.setXY(45, 0);
    add(coverShadow);

    coverOverlay.setBitmap(Bitmap(BITMAP_PLAYLIST_COVER_OVERLAY_ID));
    coverOverlay.setXY(60, 15);
    add(coverOverlay);

    line.setXY(22, getHeight() - 1);
    line.setBitmap(Bitmap(BITMAP_PLAYLIST_UNDERLINE_ID));
    add(line);
    
    numberTextBuffer[0] = 0;
    numberText.setTypedText(TypedText(T_AUDIO_PLAYLIST_LARGE));
    numberText.setColor(Color::getColorFrom24BitRGB(0xff, 0xff, 0xff));
    numberText.setWildcard(numberTextBuffer);
    numberText.setPosition(20, 30, 30, 30);
    add(numberText);

    songTextBuffer[0] = 0;
    songText.setTypedText(TypedText(T_AUDIO_PLAYLIST_LARGE));
    songText.setColor(Color::getColorFrom24BitRGB(0xff, 0xff, 0xff));
    songText.setWildcard(songTextBuffer);
    songText.setPosition(146, 16, 250, 30);
    add(songText);

    artistTextBuffer[0] = 0;
    artistText.setTypedText(TypedText(T_AUDIO_PLAYLIST_SMALL));
    artistText.setColor(Color::getColorFrom24BitRGB(85, 85, 90));
    artistText.setWildcard(artistTextBuffer);
    artistText.setPosition(146, 42, 250, 30);
    add(artistText);

    remove.setXY(325, 25);
    remove.setBitmaps(Bitmap(BITMAP_PLAYLIST_REMOVE_ID), Bitmap(BITMAP_PLAYLIST_REMOVE_PRESSED_ID));
    // add(remove);



    Bitmap alphaMaskBmp(BITMAP_PLAYLIST_COVER_MASK_ID);
    Bitmap dynBmp = Bitmap(Bitmap::dynamicBitmapCreate(alphaMaskBmp.getWidth(), alphaMaskBmp.getHeight(), Bitmap::ARGB8888));
    const uint8_t *srcAlpha = alphaMaskBmp.getData();
    uint8_t *dstAlpha = const_cast<uint8_t *>(dynBmp.getData());
    for (int i = 0; i < alphaMaskBmp.getHeight(); i++)
    {
        for (int j = 0; j < alphaMaskBmp.getWidth(); j++)
        {
            srcAlpha += 3;
            dstAlpha += 3;
            *dstAlpha = *srcAlpha;
            dstAlpha++;
            srcAlpha++;
        }
    }
    cover.setBitmap(dynBmp);
    Bitmap c = Bitmap(BITMAP_COVER_SMALL_ID);
    setCover(c);

    setTouchable(true);
}

AudioPlayerPlaylistElement::~AudioPlayerPlaylistElement()
{
    Bitmap::dynamicBitmapDelete(cover.getBitmap());
}

void AudioPlayerPlaylistElement::setRemoveCallback(GenericCallback<AudioPlayerPlaylistElement &> &callback)
{
    removeCallback = &callback;
}

void AudioPlayerPlaylistElement::setSelectedCallback(GenericCallback<int>& callback)
{
    selectedCallback = &callback;
}

void AudioPlayerPlaylistElement::setPlaying(bool play)
{
    colortype songColor = play ? Color::getColorFrom24BitRGB(1, 191, 255) : Color::getColorFrom24BitRGB(0xff, 0xff, 0xff);
    colortype artistColor = play ? Color::getColorFrom24BitRGB(29, 117, 154) : Color::getColorFrom24BitRGB(85, 85, 90);

    numberText.setColor(songColor);
    songText.setColor(songColor);
    artistText.setColor(artistColor);

    numberText.invalidate();
    songText.invalidate();
    artistText.invalidate();

    coverOverlay.setVisible(!play);
    coverOverlay.invalidate();

    line.setBitmap(Bitmap(play ? BITMAP_PLAYLIST_UNDERLINE_PLAYING_ID : BITMAP_PLAYLIST_UNDERLINE_ID));
    line.invalidate();
}

void AudioPlayerPlaylistElement::setLineVisibility(bool visible)
{
    line.setVisible(visible);
    line.invalidate();
}

void AudioPlayerPlaylistElement::setCover(Bitmap &bitmap)
{
    assert(bitmap.getWidth() == 256 && bitmap.getHeight() == 256 && "supplied image should be 256x256");
    assert(cover.getWidth() == 60 && cover.getHeight() == 60 && "cover should be 60x60");

    if (bitmap.getFormat() != Bitmap::ARGB8888
        && bitmap.getFormat() != Bitmap::RGB888)
        return;

    int bpp = bitmap.getFormat() == Bitmap::ARGB8888 ? 4 : 3;

    Bitmap coverBmp = Bitmap(cover.getBitmap());

    uint8_t *dst = const_cast<uint8_t *>(Bitmap(cover.getBitmap()).getData());
    for (int i = 0; i < cover.getHeight(); i++)
    {
        const uint8_t *src = bitmap.getData() + i * bpp * bitmap.getWidth() * 3; /* 1:4 rescale, skip 3 lines */
        for (int j = 0; j < cover.getWidth(); j++)
        {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            if (coverBmp.getFormat() == Bitmap::ARGB8888)
                dst++;
            src += bpp * 3; /* 1:4 rescale, skip 3 pixels */
        }
    }
}

void AudioPlayerPlaylistElement::set(int index, BitmapId cover, char* artist, char* song)
{
    playlistIndex = index;
    int number = index + 1;

    Bitmap c = Bitmap(cover);
    setCover(c);
    Unicode::snprintf(numberTextBuffer, 3, "%d%d", number / 10, number % 10);
    Unicode::strncpy(songTextBuffer, song, 50);
    Unicode::strncpy(artistTextBuffer, artist, 50);
}

void AudioPlayerPlaylistElement::handleClickEvent(const ClickEvent& evt)
{
    bool wasPressed = pressed;
    pressed = evt.getType() == ClickEvent::PRESSED;
    setPlaying(pressed);
    if (wasPressed && (evt.getType() == ClickEvent::RELEASED))
    {
        if (selectedCallback && selectedCallback->isValid() && evt.getType() == ClickEvent::RELEASED)
        {
            selectedCallback->execute(playlistIndex);
        }
    }
}
