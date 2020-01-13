#include <gui/videoplayer_screen/VideoPlayerView.hpp>
#include "BitmapDatabase.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SIMULATOR
#include "hwjpeg_decode.h"
extern struct HwJPEG_Context_s HwJPEG_Context;
#endif /* !SIMULATOR */

VideoPlayerView::VideoPlayerView() :
    movieEndedCallback(this, &VideoPlayerView::movieEndedHandler),
    sliderIsMoved(false),
    animationDelay(0),
    startAnimationVal(0),
    animate(false)
{

}

extern uint32_t video_chunks;

void VideoPlayerView::setupScreen()
{
#ifndef SIMULATOR
    JPEG_InitDecode_HW(&HwJPEG_Context);
#endif
    VideoPlayerViewBase::setupScreen();

    snprintf(currentFolder, sizeof(currentFolder)-1, "%s", fileinput::getDrive());

    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        playlist[i].bmpId = Bitmap::dynamicBitmapCreate(150, 91, Bitmap::RGB888);
    }

    videoBitmap = Bitmap::dynamicBitmapCreate(480, 272, Bitmap::RGB888);
    temporaryBufferBitmap = Bitmap::dynamicBitmapCreate(480, 272, Bitmap::RGB888);

    mJPEGReaderThumb.setTemporaryFrameBuffer(Bitmap::dynamicBitmapGetAddress(temporaryBufferBitmap));
    mJPEGReaderThumb.setLineBuffer(lineBuffer);
    mJPEGReaderThumb.setFileBuffer(fileBuffer, sizeof(fileBuffer));

    mJPEGReaderMovie.setPixelData(Bitmap::dynamicBitmapGetAddress(videoBitmap));
    mJPEGReaderMovie.setFrameBuffer(Bitmap::dynamicBitmapGetAddress(videoBitmap)); /* also initialize the internal frameBuffer pointer */
    mJPEGReaderMovie.setTemporaryFrameBuffer(Bitmap::dynamicBitmapGetAddress(temporaryBufferBitmap));
    mJPEGReaderMovie.setLineBuffer(lineBuffer);
    mJPEGReaderMovie.setFileBuffer(fileBuffer, sizeof(fileBuffer));
    mJPEGReaderMovie.setTimeBetweenTicks(16);

    mJPEGReaderMovie.setMovieEndedAction(movieEndedCallback);
    mJPEGReaderMovie.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    add(mJPEGReaderMovie);

    setupContent();

    videoDurationSlider.setTouchable(false);

    remove(bottomBar);

    remove(backButton);
    remove(volumeBarContainer);
    remove(videoSelector);
    remove(videoButton);
    remove(cancelButton);

    add(bottomBar);
    add(backButton);
    add(volumeBarContainer);
    add(videoSelector);
    add(videoButton);
    add(cancelButton);
 }

void VideoPlayerView::tearDownScreen()
{
    VideoPlayerViewBase::tearDownScreen();
}

void VideoPlayerView::handleClickEvent(const ClickEvent& evt)
{
    if (videoDurationSlider.getRect().intersect(Rect(evt.getX() - bottomBar.getX(), evt.getY() - bottomBar.getY(), 1, 1)) && evt.getType() == ClickEvent::PRESSED)
    {
        sliderIsMoved = true;
        animate = false;
    }
    else
    {
        sliderIsMoved = false;
    }

    if (bottomBar.getY() == HAL::DISPLAY_HEIGHT)
    {
        bottomBar.startMoveAnimation(0, HAL::DISPLAY_HEIGHT - bottomBar.getHeight(), ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        videoButton.startMoveAnimation(HAL::DISPLAY_WIDTH - videoButton.getWidth(), 0, ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        backButton.startMoveAnimation(0, 0, ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

        if (mJPEGReaderMovie.isPlaying())
        {
            animate = true;
            animationDelay = 0;
            startAnimationVal = ANIMATION_DELAY;
        }
    }
    else if (mJPEGReaderMovie.isPlaying() && videoButton.isVisible())
    {
        animate = true;
        animationDelay = 0;
        startAnimationVal = ANIMATION_DELAY;
    }
    VideoPlayerViewBase::handleClickEvent(evt);
}

void VideoPlayerView::updateVolume(int volume)
{
    volumeSlider.setValue(volume);
    volumeSliderChanged(volume);
    if (volumeBarContainer.getX() != VOLUME_SLIDER_X)
    {
        volumeButton.forceState(false);
        volumeButton.invalidate();
    }
}

void VideoPlayerView::volumeButtonPressed(bool value)
{
    if (value)
    {
        volumeBarContainer.startMoveAnimation(VOLUME_SLIDER_X, volumeBarContainer.getY(), VOLUME_SLIDER_MOVE_DURATION, EasingEquations::cubicEaseOut);
    }
    else
    {
        volumeBarContainer.startMoveAnimation(HAL::DISPLAY_WIDTH, volumeBarContainer.getY(), VOLUME_SLIDER_MOVE_DURATION, EasingEquations::cubicEaseOut);
    }
}

void VideoPlayerView::volumeSliderChanged(uint16_t value)
{
    presenter->setVolume(value);

    if (value == 0)
    {
        volumeButton.setBitmaps(Bitmap(BITMAP_ICON_VOLUME_MUTED_ID), Bitmap(BITMAP_ICON_VOLUME_MUTED_PRESSED_ID));
    }
    else if (value <= 33)
    {
        volumeButton.setBitmaps(Bitmap(BITMAP_ICON_VOLUME_LOW_ID), Bitmap(BITMAP_ICON_VOLUME_LOW_PRESSED_ID));
    }
    else if (value <= 66)
    {
        volumeButton.setBitmaps(Bitmap(BITMAP_ICON_VOLUME_MEDIUM_ID), Bitmap(BITMAP_ICON_VOLUME_MEDIUM_PRESSED_ID));
    }
    else
    {
        volumeButton.setBitmaps(Bitmap(BITMAP_ICON_VOLUME_HIGH_ID), Bitmap(BITMAP_ICON_VOLUME_HIGH_PRESSED_ID));
    }

    volumeButton.forceState(true);

    volumeButton.invalidate();
}

void VideoPlayerView::playPauseButtonPressed(bool value)
{
    presenter->setPlayVideo(value);
    animate = value;
    animationDelay = 0;
    startAnimationVal = ANIMATION_DELAY;

    videoSelector.playPauseSelectedVideo(value);

    if (value)
    {
        mJPEGReaderMovie.play();
    }
    else
    {
        mJPEGReaderMovie.pause();
    }
}

void VideoPlayerView::updateVideoDuration(int totalDuration, int minutes, int seconds)
{
    if (!sliderIsMoved)
    {
        videoDurationSlider.setValue(totalDuration);
        videoDurationSlider.invalidate();
    }

    Unicode::snprintf(durationLeftBuffer, DURATIONLEFT_SIZE, "%.2d:%.2d", minutes, seconds);
    durationLeft.invalidate();
}

void VideoPlayerView::setVideoDuration(int totalLength, int minutes, int seconds)
{
    videoDurationSlider.setValueRange(0, totalLength);
    videoDurationSlider.invalidate();

    Unicode::snprintf(durationTotalBuffer, DURATIONTOTAL_SIZE, "%.2d:%.2d", minutes, seconds);
    durationTotal.invalidate();
}

void VideoPlayerView::videoSliderChanged(uint16_t value)
{
    presenter->updateVideoDuration(value);

    mJPEGReaderMovie.gotoFrame(value * 1000 / mJPEGReaderMovie.getMsPerFrame());
}

void VideoPlayerView::videoListPlayPausePressed(bool value)
{
    playPauseButtonPressed(value);
    playPauseButton.forceState(value);
    playPauseButton.invalidate();
}

void VideoPlayerView::newVideoSelected(int value)
{
    setVideo(value);
    videoSelector.playPauseSelectedVideo(mJPEGReaderMovie.isPlaying());
}

void VideoPlayerView::nextPressed()
{
    presenter->nextPressed();
    videoSelector.playPauseSelectedVideo(mJPEGReaderMovie.isPlaying());
}

void VideoPlayerView::previousPressed()
{
    presenter->previousPressed();
    videoSelector.playPauseSelectedVideo(mJPEGReaderMovie.isPlaying());
}

void VideoPlayerView::shufflePressed(bool value)
{
    presenter->setShuffle(value);

    videoSelector.setRepeatButtonState(false);
    presenter->setRepeat(false);
}

void VideoPlayerView::repeatPressed(bool value)
{
    presenter->setRepeat(value);

    videoSelector.setShuffleButtonState(false);
    presenter->setShuffle(false);
}

void VideoPlayerView::handleTickEvent()
{
    if (presenter->hasVideoEnded())
    {
      movieEndedHandler();
    }

    if (mJPEGReaderMovie.isPlaying())
    {
        mJPEGReaderMovie.tick();
        presenter->updateVideoDuration( presenter->getElapsedTime() / 1000);
    }

    if (animate && animationDelay == startAnimationVal)
    {
        bottomBar.startMoveAnimation(0, HAL::DISPLAY_HEIGHT, ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        videoButton.startMoveAnimation(HAL::DISPLAY_WIDTH + videoButton.getWidth(), -videoButton.getHeight(), ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        backButton.startMoveAnimation(-cancelButton.getWidth(), -cancelButton.getHeight(), ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        animate = false;
    }
    else if (animate)
    {
        animationDelay++;
    }
}

void VideoPlayerView::movieEndedHandler()
{
    nextPressed();

    /*
     * when the repeat and shuffle are not active, pause when reaching back the first video
     */
    if ((presenter->getCurrentVideo() == 0) && (presenter->getRepeat() == 0)  && (presenter->getShuffle() == 0))
    {
      mJPEGReaderMovie.pause();
      presenter->setPlayVideo(false);
      playPauseButton.forceState(false);
      playPauseButton.invalidate();

      videoSelector.playPauseSelectedVideo(mJPEGReaderMovie.isPlaying());
    }
    else
    {
      presenter->setPlayVideo(true);
      mJPEGReaderMovie.play();
    }

    if ((presenter->getCurrentVideo() == 0) && !mJPEGReaderMovie.isPlaying())

    {
        bottomBar.startMoveAnimation(0, HAL::DISPLAY_HEIGHT - bottomBar.getHeight(), ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        videoButton.startMoveAnimation(HAL::DISPLAY_WIDTH - videoButton.getWidth(), 0, ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        backButton.startMoveAnimation(0, 0, ANIMATION_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
    }
}

bool VideoPlayerView::addFile(uint32_t index)
{
    uint32_t pl_index = PLAYLIST_LENGTH;
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (playlist[i].used == false)
        {
            pl_index = i;
            break;
        }
    }
    if (pl_index == PLAYLIST_LENGTH)
    {
        return false; //no space
    }

    memset(playlist[pl_index].filename, '\0', sizeof(playlist[pl_index].filename));
    strcat(playlist[pl_index].filename, dirlist[index].name);

    // forward filename to the audio decoder
    presenter->updateVideoList(playlist[pl_index].filename);

    //load thumbnail from file
    fileinput::FileHdl f = fileinput::openFile(playlist[pl_index].filename);
    if (mJPEGReaderThumb.setAVIFile(f))
    {
        mJPEGReaderThumb.decodeThumbNail(Bitmap(playlist[pl_index].bmpId), 0);
        playlist[pl_index].used = true;
    }
    mJPEGReaderThumb.closeFile();

    return true;
}

void VideoPlayerView::loadRootFiles()
{
    uint32_t n = fileinput::getDirectoryList(currentFolder, ".avi", dirlist, DIRLIST_LENGTH);

    for (unsigned int i = 0; i < n; i++)
    {
        //load thumbnail from file
        if (dirlist[i].isDirectory == false)
        {
            addFile(i);
        }
    }
}

void VideoPlayerView::setupThumbnails()
{
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        if (playlist[i].used)
        {
            videoSelector.setupThumbnail(i, playlist[i].used, Bitmap(playlist[i].bmpId));
            presenter->setNumberOfVideos(i + 1);
        }
    }
}

void VideoPlayerView::setVideo(int id)
{
    if(currentMovie)
    {
      fileinput::closeFile(currentMovie);
    }
    currentMovie = fileinput::openFile(playlist[id].filename);
    mJPEGReaderMovie.setAVIFile(currentMovie);
    mJPEGReaderMovie.showFirstFrame();

    numberOfFrames = mJPEGReaderMovie.getNumberOfFrames();
    msPerFrame = mJPEGReaderMovie.getMsPerFrame();

    totalDuration = numberOfFrames * msPerFrame / 1000;

    presenter->setVideoDuration(totalDuration);
    presenter->updateVideoDuration(0);
    presenter->setCurrentVideo(id);

    videoSelector.setSelectedThumbnail(id);
}

void VideoPlayerView::videoButtonPressed()
{
    animate = false;
}

void VideoPlayerView::cancelButtonPressed()
{
    animate = true;
    animationDelay = 0;
    startAnimationVal = ANIMATION_DELAY;
}

void VideoPlayerView::setupContent()
{
    for (int i = 0; i < PLAYLIST_LENGTH; i++)
    {
        playlist[i].used = false;
    }

    loadRootFiles();
    setupThumbnails();

    if (playlist[0].used)
    {
        mJPEGReaderMovie.setVisible(true);
        bottomBar.setVisible(true);
        setVideo(0);
    }
    else
    {
        mJPEGReaderMovie.setVisible(false);
        bottomBar.setVisible(false);
    }
    mJPEGReaderMovie.invalidate();
    bottomBar.invalidate();
}

void VideoPlayerView::backButtonPressed()
{
    presenter->returnToMenuLauncher();
}
