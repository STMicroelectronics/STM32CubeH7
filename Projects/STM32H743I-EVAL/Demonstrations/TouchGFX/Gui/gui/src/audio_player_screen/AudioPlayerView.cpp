#include <gui/audio_player_screen/AudioPlayerView.hpp>
#include <touchgfx/Color.hpp>
#include <string.h>
#include "BitmapDatabase.hpp"
#include <touchgfx/hal/GPIO.hpp>

AudioPlayerView::AudioPlayerView() :
    playCallback(this, &AudioPlayerView::playButtonClicked),
    playTouchedCallback(this, &AudioPlayerView::playButtonTouched),
    coverImageTouchedCallback(this, &AudioPlayerView::coverWidgetTouched),
    playButtonFadeEndedCallback(this, &AudioPlayerView::playButtonFadeEnded),
    backCallback(this, &AudioPlayerView::backButtonClicked),
    prevNextCallback(this, &AudioPlayerView::prevNextButtonClicked),
    volumeChangedCallback(this, &AudioPlayerView::handleVolumeChange),
    playlistElementSelectedCallback(this, &AudioPlayerView::playlistElementSelected),
    actionCallback(this, &AudioPlayerView::onAction),
    progressSliderCallback(this, &AudioPlayerView::progressSliderChanged),
    playerState(STOPPED),
    duration(0),
    folderWidget(*this),
    seekInProgess(false),
    currProgress(0),
    newProgress(0),
    ticks(0)
{
    strncpy(currentFolder, fileinput::getDrive(), sizeof(currentFolder));
    CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
}

void AudioPlayerView::setupScreen()
{
    HAL::getInstance()->setFrameRateCompensation(false);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_MENU_ICON_ID), Bitmap(BITMAP_MENU_ICON_PRESSED_ID));
    gotoMenuButton.setXY(11, 8);

    mcuLoadTxt.setXY(620 - 160 + 32, 24);
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right() - 6, mcuLoadTxt.getY(), 50, 30);
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    mcuLoadValueTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    bottomBar.setVolumeAction(volumeChangedCallback);
    bottomBar.setActionCallback(actionCallback);

    background.setBitmap(Bitmap(BITMAP_AUDIO_DEFAULT_BACKGROUND_ID));

    blurBackground.setPosition(0, 0, 640, 480);
    blurBackground.setPixelData((uint8_t*)HAL::getInstance()->getAnimationStorage());
    blurBackground.setBitmapFormat(Bitmap::RGB565);
    Bitmap cover(BITMAP_COVER_UNKNOWN_ARTIST_ID);
    coverImage.setBitmap(cover);

    coverImage.setX((HAL::DISPLAY_WIDTH - coverImage.getWidth()) / 2);
    coverImage.setY((HAL::DISPLAY_HEIGHT - coverImage.getHeight()) / 2);
    coverImage.setAction(progressSliderCallback);
    coverImage.setClickAction(coverImageTouchedCallback);
    
    playButton.setBitmaps(Bitmap(BITMAP_PLAY_BUTTON_ID), Bitmap(BITMAP_PLAY_BUTTON_ID));
    playButton.setX((container.getWidth() - playButton.getWidth()) / 2);
    playButton.setY((container.getHeight() - playButton.getHeight()) / 2);
    playButton.setAction(playCallback);
    playButton.setClickAction(playTouchedCallback);
    playButton.setFadeAnimationEndedAction(playButtonFadeEndedCallback);

    prevButton.setBitmaps(Bitmap(BITMAP_PREVIOUS_BUTTON_ID), Bitmap(BITMAP_PREVIOUS_BUTTON_PRESSED_ID));
    prevButton.setXY(46, (container.getHeight() - prevButton.getHeight()) / 2);
    prevButton.setAction(prevNextCallback);

    nextButton.setBitmaps(Bitmap(BITMAP_NEXT_BUTTON_ID), Bitmap(BITMAP_NEXT_BUTTON_PRESSED_ID));
    nextButton.setXY(494, prevButton.getY());
    nextButton.setAction(prevNextCallback);

    add(background);
    add(coverImage);
    add(playButton);
    add(prevButton);
    add(nextButton);

    backButton.setBitmaps(Bitmap(BITMAP_HOME_BACK_BUTTON_ID), Bitmap(BITMAP_HOME_BACK_BUTTON_ID));
    backButton.setXY(6, 6);
    backButton.setAction(backCallback);
    backButton.setVisible(false);

    playlist.setVisible(false);
    playlist.setSelectedCallback(playlistElementSelectedCallback);
    add(playlist);

    equalizerSettingCallback = Callback<AudioPlayer, const AudioPlayer::EqualizerSetting, const int16_t>(presenter->getPlayer(), &AudioPlayer::setEqualizerSetting);
    equalizer.setVisible(false);
    equalizer.setOnChangeCallback(equalizerSettingCallback);
    add(equalizer);

    //requires new playlist element
    folderWidget.setPlaylist(presenter->getPlayList(), PLAYLIST_LENGTH);
    folderWidget.setXY(-430, 70);
    add(folderWidget);

    topBar.setXY(0, -topBar.getHeight());
    add(topBar);
    bottomBar.setY(480);
    add(bottomBar);
    applyCoverBlur(cover);

    showBars(BAR_FADEIN);

    topBar.add(backButton);
    topBar.add(gotoMenuButton);
    topBar.add(mcuLoadTxt);
    topBar.add(mcuLoadValueTxt);
}

void AudioPlayerView::handleVolumeChange(int newVal)
{
    if (presenter)
    {
        presenter->setVolume(newVal);
    }
}

void AudioPlayerView::showBackButton(bool show)
{
    gotoMenuButton.setVisible(!show);
    gotoMenuButton.invalidate();

    backButton.setVisible(show);
    backButton.invalidate();
}

void AudioPlayerView::setSongInfo(int playlistIndex, char* name, uint32_t duration)
{
    this->duration = duration;
    topBar.setText(name);
    coverImage.setProgress(0);
    bottomBar.setCurrentTime(0, 0);
    bottomBar.setTotalTime(duration / 1000 / 60, (duration / 1000) % 60);
    playlist.setPlaying(playlistIndex);
}

void AudioPlayerView::setCover(BitmapId cover)
{
    Bitmap c(cover);
    if (coverImage.getBitmap() != cover)
    {
        //applyCoverBlur(c);
        coverImage.setBitmap(c);
        coverImage.invalidate();
    }
}

void AudioPlayerView::addToPlaylist(BitmapId cover, char* artist, char* song)
{
    playlist.addSong(cover, artist, song);
}


void AudioPlayerView::onAction(Action action)
{
    uint32_t n = 0;
    switch (action)
    {
    case EQUALIZER:
        //equalizer.setVisible(true);
        //equalizer.invalidate();
        //topBar.setBackgroundVisible(false);
        //bottomBar.setVisible(false);
        //coverImage.setVisible(false);
        //nextButton.setVisible(false);
        //prevButton.setVisible(false);
        //playButton.setVisible(false);
        //topBar.setText("EQUALIZER");
        //showBackButton(true);
        //showBars(BAR_FADE_BOTTOM_OUT);
        break;
    case PLAYLIST:
        playlist.setVisible(true);
        playlist.invalidate();
        topBar.setBackgroundVisible(false);
        topBar.setText("YOUR PLAYLIST");
        showBars(BAR_BOTTOM_HIDE);
        bottomBar.setVisible(false);
        coverImage.setVisible(false);
        nextButton.setVisible(false);
        prevButton.setVisible(false);
        playButton.setVisible(false);
        showBackButton(true);


        break;
    case FOLDER:
        //dirlist is a temporary one-time setup array - All subsequent updates happen from presenter. Get this initialization from presenter instead.
        n = fileinput::getDirectoryList(currentFolder, ".wav", dirlist, DIRLIST_LENGTH);
        folderWidget.setupDirlist(currentFolder, dirlist, n);
        presenter->resetDirectoryList();

        folderWidget.moveTo(0, 70);
        folderWidget.setVisible(true);
        //showBars(BAR_TOP_ONLY);

        equalizer.setVisible(false);
        equalizer.invalidate();
        topBar.setBackgroundVisible(true);
        bottomBar.setVisible(false);
        coverImage.setVisible(true);
        //coverImage.moveTo((HAL::DISPLAY_WIDTH - folderWidget.getWidth()) + 65, coverImage.getY()); 
        nextButton.setVisible(false);
        prevButton.setVisible(false);
        playButton.setVisible(false);
        showBackButton(true);
        break;
    default:
        assert(!"Unknown action!");
    }
}

void AudioPlayerView::applyCoverBlur(Bitmap& bmp)
{
    HAL::getInstance()->lockFrameBuffer();
    HAL::getInstance()->unlockFrameBuffer();

    //const uint8_t* p = bmp.getData();
    //int x = bmp.getWidth() / 2 - BLUR_REGION_WIDTH / 2;
    //int y = bmp.getHeight() / 2 - BLUR_REGION_HEIGHT / 2;
    //p += (x + y * bmp.getWidth()) * 3;
    //const uint32_t* p32 = (const uint32_t*)p;
    //uint32_t* data = (uint32_t*)blurBuffer;
    //for (int j = 0; j < BLUR_REGION_HEIGHT; j++)
    //{
    //    for (int i = 0; i < BLUR_REGION_WIDTH * 3 / 4; i++)
    //    {
    //        *data++ = *p32++;
    //    }
    //    p32 += ((bmp.getWidth() - BLUR_REGION_WIDTH) * 3) / 4;
    //}


    //gaussBlur(blurBuffer, BLUR_REGION_WIDTH, BLUR_REGION_HEIGHT, 3);
    //upscaleBlurredImage(blurBuffer, BLUR_REGION_WIDTH, BLUR_REGION_HEIGHT);
    //blurBackground.invalidate();
}

void AudioPlayerView::handleTickEvent()
{
    ticks++;
    if (ticks == 20)
    {
      // While screen is displaying "Initializing audio", read contents from
      // sd card.
        presenter->initializeFileSystem();
    }
    if ((playerState == PLAYING) && (seekInProgess == false))
    {
        uint32_t newtime = presenter->getSongProgressMs();
        float f = 360.0f * newtime / (float)duration;
        coverImage.setProgress((int)f);
        bottomBar.setCurrentTime(newtime / 1000 / 60, (newtime / 1000) % 60);

        coverImage.renderVisualization();
        if ((ticks % 1000) == 0)
            coverImage.nextVisualization();
    }
    View::handleTickEvent();
}

void AudioPlayerView::upscaleBlurredImage(uint8_t* src, int width, int height)
{
    // This function will upscale the blurred 200x120 pixel buffer to fill the
    // entire 800x480 frame buffer.
    // The algorithm is a "pseudo-bilinear interpolation", which hits the tradeoff
    // between visual quality and speed.
    // This particular implementation is optimized for exactly these dimensions.
    // Will require tweaking 

    int lineWidthInBytes = BLUR_REGION_WIDTH * 3;

    uint32_t* dest = (uint32_t*)touchgfx::HAL::getInstance()->getAnimationStorage();
    for (int j = 0; j < BLUR_REGION_HEIGHT - 1; j++)
    {
        // Repeat each line from source 4 times.
        for (int k = 0; k < 4; k++)
        {
            for (int i = 0; i < BLUR_REGION_WIDTH - 1; i++)
            {
                int rL = *src + k * (src[lineWidthInBytes] - src[0]) / 4;
                int gL = src[1] + k * (src[lineWidthInBytes + 1] - src[1]) / 4;
                int bL = src[2] + k * (src[lineWidthInBytes + 2] - src[2]) / 4;
                int rR = src[3] + k * (src[lineWidthInBytes + 3] - src[3]) / 4;
                int gR = src[4] + k * (src[lineWidthInBytes + 4] - src[4]) / 4;
                int bR = src[5] + k * (src[lineWidthInBytes + 5] - src[5]) / 4;

                int diffR = (rR - rL) / 4;
                int diffG = (gR - gL) / 4;
                int diffB = (bR - bL) / 4;

                uint32_t pix = ((rL + diffR) << 24) | (bL << 16) | (gL << 8) | rL;
                *dest++ = pix;
                rL += diffR;
                gL += diffG;
                bL += diffB;
                pix = ((gL + diffG) << 24) | ((rL + diffR) << 16) | ((bL) << 8) | (gL);
                *dest++ = pix;
                rL += diffR;
                gL += diffG;
                bL += diffB;
                pix = ((bL + diffB) << 24) | ((gL + diffG) << 16) | ((rL + diffR) << 8) | (bL);
                *dest++ = pix;

                src += 3;
            }
            unsigned int r = *src;
            unsigned int g = src[1];
            unsigned int b = src[2];
            uint32_t pix = (r << 24) | (b << 16) | (g << 8) | r;
            *dest++ = pix;
            pix = (pix >> 8) | (g << 24);
            *dest++ = pix;
            pix = (pix >> 8) | (b << 24);
            *dest++ = pix;
            src += 3;
            src -= lineWidthInBytes;
        }
        src += lineWidthInBytes;
    }
    // Handle last row of pixels in a special way.
    for (int k = 0; k < 4; k++)
    {
        for (int i = 0; i < BLUR_REGION_WIDTH; i++)
        {
            // Read single pixel from source, and write it as 4 pixels:
            // R G B R | G B R G | B R G B
            unsigned int r = *src;
            unsigned int g = src[1];
            unsigned int b = src[2];
            uint32_t pix = (r << 24) | (b << 16) | (g << 8) | r;
            *dest++ = pix;
            pix = (pix >> 8) | (g << 24);
            *dest++ = pix;
            pix = (pix >> 8) | (b << 24);
            *dest++ = pix;
            src += 3;
        }
        src -= lineWidthInBytes;
    }
    src += lineWidthInBytes;
}

void AudioPlayerView::horizontalBlur(uint8_t* src, uint8_t* work, int width, int height, int radius)
{
    int arr = radius + radius + 1;
    for (int i = 0; i < height; i++)
    {
        int ti = i * width * 3;
        int li = 0;
        int ri = radius * 3;
        uint8_t fvr = src[ti + 0];
        uint8_t fvg = src[ti + 1];
        uint8_t fvb = src[ti + 2];
        uint8_t lvr = src[ti + (width - 1) * 3 + 0];
        uint8_t lvg = src[ti + (width - 1) * 3 + 1];
        uint8_t lvb = src[ti + (width - 1) * 3 + 2];
        int valr = (radius + 1) * fvr;
        int valg = (radius + 1) * fvg;
        int valb = (radius + 1) * fvb;

        for (int j = 0; j < width * 3; j++)
        {
            work[j] = src[ti + j];
        }

        for (int j = 0; j < radius; j++)
        {
            valr += work[j * 3 + 0];
            valg += work[j * 3 + 1];
            valb += work[j * 3 + 2];
        }
        for (int j = 0  ; j <= radius ; j++)
        {
            valr += work[ri + 0] - fvr;
            valg += work[ri + 1] - fvg;
            valb += work[ri + 2] - fvb;
            src[ti + 0] = valr / arr;
            src[ti + 1] = valg / arr;
            src[ti + 2] = valb / arr;
            ri += 3;
            ti += 3;
        }
        for (int j = radius + 1; j < width - radius; j++)
        {
            valr += work[ri + 0] - work[li + 0];
            valg += work[ri + 1] - work[li + 1];
            valb += work[ri + 2] - work[li + 2];
            src[ti + 0] = valr / arr;
            src[ti + 1] = valg / arr;
            src[ti + 2] = valb / arr;
            ri += 3;
            li += 3;
            ti += 3;
        }
        for (int j = width - radius; j < width  ; j++)
        {
            valr += lvr - work[li + 0];
            valg += lvg - work[li + 1];
            valb += lvb - work[li + 2];
            src[ti + 0] = valr / arr;
            src[ti + 1] = valg / arr;
            src[ti + 2] = valb / arr;
            li += 3;
            ti += 3;
        }
    }
}

void AudioPlayerView::verticalBlur(uint8_t* src, uint8_t* work, int width, int height, int radius)
{
    int arr = radius + radius + 1;
    for (int i = 0; i < width; i++)
    {
        int ti = i * 3;
        int li = 0;
        int ri = radius * 3;
        uint8_t fvr = src[ti + 0];
        uint8_t fvg = src[ti + 1];
        uint8_t fvb = src[ti + 2];
        uint8_t lvr = src[ti + width * (height - 1) * 3 + 0];
        uint8_t lvg = src[ti + width * (height - 1) * 3 + 1];
        uint8_t lvb = src[ti + width * (height - 1) * 3 + 2];
        int valr = (radius + 1) * fvr;
        int valg = (radius + 1) * fvg;
        int valb = (radius + 1) * fvb;

        for (int j = 0; j < height; j++)
        {
            work[j * 3 + 0] = src[ti + j * width * 3 + 0];
            work[j * 3 + 1] = src[ti + j * width * 3 + 1];
            work[j * 3 + 2] = src[ti + j * width * 3 + 2];
        }

        for (int j = 0; j < radius; j++)
        {
            valr += work[j * 3 + 0];
            valg += work[j * 3 + 1];
            valb += work[j * 3 + 2];
        }
        for (int j = 0  ; j <= radius ; j++)
        {
            valr += work[ri + 0] - fvr;
            valg += work[ri + 1] - fvg;
            valb += work[ri + 2] - fvb;
            src[ti + 0] = valr / arr;
            src[ti + 1] = valg / arr;
            src[ti + 2] = valb / arr;
            ri += 3;
            ti += width * 3;
        }
        for (int j = radius + 1; j < height - radius; j++)
        {
            valr += work[ri + 0] - work[li + 0];
            valg += work[ri + 1] - work[li + 1];
            valb += work[ri + 2] - work[li + 2];
            src[ti + 0] = valr / arr;
            src[ti + 1] = valg / arr;
            src[ti + 2] = valb / arr;
            li += 3;
            ri += 3;
            ti += width * 3;
        }
        for (int j = height - radius; j < height  ; j++)
        {
            valr += lvr - work[li + 0];
            valg += lvg - work[li + 1];
            valb += lvb - work[li + 2];
            src[ti + 0] = valr / arr;
            src[ti + 1] = valg / arr;
            src[ti + 2] = valb / arr;
            li += 3;
            ti += width * 3;
        }
    }
}

void AudioPlayerView::boxBlur(uint8_t* src, uint8_t* work, int width, int height, int radius)
{
    horizontalBlur(src, work, width, height, radius);
    verticalBlur(src, work, width, height, radius);
}

void AudioPlayerView::gaussBlur(uint8_t* src, int width, int height, int radius)
{
    uint8_t work[BLUR_REGION_WIDTH * 3];
    boxBlur(src, work, width, height, 10);
    boxBlur(src, work, width, height, 4);
    boxBlur(src, work, width, height, 3);
}

void AudioPlayerView::tearDownScreen()
{

}

void AudioPlayerView::playButtonClicked(const AbstractButton& btn)
{
    presenter->playPauseClicked();
    playerState = presenter->isAudioPlaying() ? PLAYING : PAUSED;
    if (playerState == PLAYING)
    {
        playButton.setBitmaps(Bitmap(BITMAP_PAUSE_BUTTON_ID), Bitmap(BITMAP_PAUSE_BUTTON_ID));
        playButton.startFadeAnimation(0, 150, EasingEquations::cubicEaseIn);
    }
    else
    {
        playButton.setBitmaps(Bitmap(BITMAP_PLAY_BUTTON_ID), Bitmap(BITMAP_PLAY_BUTTON_ID));
    }

    playButton.invalidate();
    coverImage.setPlayState(playerState);
    bottomBar.setPlayState(playerState);
}

void AudioPlayerView::setView(enum CoverWidget::COVER_VIEW view)
{
    switch (view) {
    case CoverWidget::NO_MEDIA_VIEW:
        coverImage.setView(CoverWidget::NO_MEDIA_VIEW);
        break;
    case CoverWidget::PROGRESS_VIEW:
        playButton.setVisible(true);
        playButton.setAlpha(255);
        playButton.invalidate();
        coverImage.setView(CoverWidget::PROGRESS_VIEW);
        break;
    case CoverWidget::VISUALIZATION_VIEW:
        playButton.setVisible(false);
        playButton.invalidate();
        coverImage.setView(CoverWidget::VISUALIZATION_VIEW);
        break;
    }
}

void AudioPlayerView::playButtonTouched(const Button&, const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
        playButton.cancelFadeAnimation();
    coverImage.setTouched(evt.getType() == ClickEvent::PRESSED);
}

void AudioPlayerView::coverWidgetTouched(const CoverWidget&, const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        playButton.cancelFadeAnimation();
        setView(CoverWidget::PROGRESS_VIEW);
        playButton.startFadeAnimation(0, 300, EasingEquations::cubicEaseIn);
    }

    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Apply ne Progress
        if(seekInProgess)
        {
            playButton.startFadeAnimation(0, 300, EasingEquations::cubicEaseIn);
            seekInProgess = false;
            //touchgfx_printf("%s : apply new progress %d\r\n", __PRETTY_FUNCTION__, newProgress);
            currProgress = newProgress;
            presenter->setSongProgressPercentage(newProgress);
        }
     }
}

void AudioPlayerView::playButtonFadeEnded(const FadeAnimator<ClickListener<Button> >&)
{
    setView(CoverWidget::VISUALIZATION_VIEW);
}

void AudioPlayerView::prevNextButtonClicked(const AbstractButton& btn)
{
    playButton.cancelFadeAnimation();
    setView(CoverWidget::PROGRESS_VIEW);
    if (playerState == PLAYING)
        playButton.startFadeAnimation(0, 300, EasingEquations::cubicEaseIn);

    if (&btn == &prevButton)
    {
        presenter->selectPreviousSong();
    }
    else if (&btn == &nextButton)
    {
        presenter->selectNextSong();
    }
    bottomBar.setCurrentTime(0, 0);
}

void AudioPlayerView::setEqualizerSettings(int16_t _100Hz, int16_t _1kHz, int16_t _10kHz, int16_t _20kHz, int16_t loudness)
{
    equalizer.setSettings(_100Hz, _1kHz, _10kHz, _20kHz, loudness);
}

void AudioPlayerView::showNoMedia()
{
    setCover(BITMAP_COVER_NO_MEDIA_ID);
    topBar.setText("NO MEDIA FOUND");
    bottomBar.setVisible(false);
    nextButton.setVisible(false);
    prevButton.setVisible(false);
    playButton.setVisible(false);
    setView(CoverWidget::NO_MEDIA_VIEW);
}

void AudioPlayerView::showInitializing()
{
    showNoMedia();
    topBar.setText("INITIALIZING AUDIO...");
}

bool AudioPlayerView::addFile(uint32_t index)
{
    presenter->fileAdded(index);

    return true;
}

void AudioPlayerView::removeFile(uint32_t index)
{
    presenter->removeFile(index);
}

//Keep this one in view, since we just need to change folders.
void AudioPlayerView::folderSelected(uint32_t index)
{
    presenter->folderSelected(index);
}

void AudioPlayerView::upPressed()
{
    presenter->upPressed();
}

void AudioPlayerView::donePressed()
{
    showMainScreen();
    presenter->donePressed();
    showBars(BAR_BOTTOM_HIDE);
    showBars(BAR_FADE_BOTTOM_IN);
}

void AudioPlayerView::backButtonClicked(const AbstractButton& source)
{
    presenter->backPressed();
    showMainScreen();
    showBars(BAR_BOTTOM_HIDE);
    showBars(BAR_FADEIN);
}

void AudioPlayerView::playlistElementSelected(int index)
{
    showMainScreen();
    showBars(BAR_FADEIN);
    presenter->selectSong(index);
    playButton.cancelFadeAnimation();
    setView(CoverWidget::PROGRESS_VIEW);
    playButton.startFadeAnimation(0, 300, EasingEquations::cubicEaseIn);
}

void AudioPlayerView::showMainScreen()
{
    bool empty = presenter->isPlaylistEmpty();

    folderWidget.moveTo(-430, 70);
    topBar.setBackgroundVisible(true);
    bottomBar.setVisible(true);
    coverImage.setVisible(true);
    coverImage.setTouchable(!empty);
    coverImage.moveTo((HAL::DISPLAY_WIDTH - coverImage.getWidth()) / 2, (HAL::DISPLAY_HEIGHT - coverImage.getHeight()) / 2);
    nextButton.setVisible(!presenter->isPlaylistEmpty());
    prevButton.setVisible(!empty);
    playButton.moveTo((HAL::DISPLAY_WIDTH - playButton.getWidth()) / 2, playButton.getY());
    playButton.setVisible(!empty);
    playlist.setVisible(false);
    playlist.invalidate();
    equalizer.setVisible(false);
    showBackButton(false);
}

void AudioPlayerView::showBars(BAR_STATE show)
{
    switch (show)
    {
    case BAR_TOP_HIDE:
        topBar.moveTo(0, 0 - topBar.getHeight());
        break;
    case BAR_FADEIN:
        topBar.startMoveAnimation(0, 0, 16, &EasingEquations::cubicEaseInOut);
        bottomBar.startMoveAnimation(0, 480 - bottomBar.getHeight(), 16, &EasingEquations::cubicEaseInOut);
        break;
    case BAR_FADEOUT:
        topBar.startMoveAnimation(0, 0 - topBar.getHeight(), 16, &EasingEquations::cubicEaseInOut);
        bottomBar.startMoveAnimation(0, 480, 16, &EasingEquations::cubicEaseInOut);
        break;
    case BAR_FADE_BOTTOM_IN:
        bottomBar.startMoveAnimation(0, 480 - bottomBar.getHeight(), 16, &EasingEquations::cubicEaseInOut);
        break;
    case BAR_FADE_BOTTOM_OUT:
        bottomBar.startMoveAnimation(0, 480, 16, &EasingEquations::cubicEaseInOut);
        break;
    case BAR_BOTTOM_HIDE:
        bottomBar.moveTo(0, 480);
        break;
    case BAR_SHOW_IMMEDIATE:
        topBar.moveTo(0, 0);
        bottomBar.moveTo(0, 480 - bottomBar.getHeight());
        break;
    case BAR_HIDE:
        topBar.moveTo(0, 0 - topBar.getHeight());
        bottomBar.moveTo(0, 480);
        break;
    case BAR_TOP_ONLY:
        topBar.moveTo(0, 0);
        bottomBar.moveTo(0, 480);
        break;
    }
}

void AudioPlayerView::setupDirlist(const char* const foldername, fileinput::DirEntry* dirlist, uint32_t length)
{
    folderWidget.setupDirlist(foldername, dirlist, length);
}


void AudioPlayerView::clearPlaylist()
{
    playlist.clear();
}

void AudioPlayerView::progressSliderChanged(uint32_t newValue)
{
    //newValue is in degrees (0-359). Convert to pct.
    newProgress = (uint32_t)(100*newValue/360.0f);

    if(currProgress != newProgress)
    {
        uint32_t newtime = newProgress * duration / 100;
        playButton.cancelFadeAnimation();
        setView(CoverWidget::PROGRESS_VIEW);
        seekInProgess = true;
        currProgress = newProgress;
        bottomBar.setCurrentTime(newtime / 1000 / 60, (newtime / 1000) % 60);
        //touchgfx_printf("%s : set current time %d  --> (%02d:%02d)\r\n", __PRETTY_FUNCTION__, newtime, newtime / 1000 / 60, (newtime / 1000) % 60);
    }
}

void AudioPlayerView::screenClick(ClickEvent::ClickEventType type, int x, int y)
{
    ClickEvent temp_evt(type, x, y);
    View::handleClickEvent(temp_evt);
}

void AudioPlayerView::screenDrag(int16_t x_old, int16_t y_old, int16_t x_new, int16_t y_new)
{
    DragEvent temp_evt(DragEvent::DRAGGED, x_old, y_old, x_new, y_new);
    View::handleDragEvent(temp_evt);
}
