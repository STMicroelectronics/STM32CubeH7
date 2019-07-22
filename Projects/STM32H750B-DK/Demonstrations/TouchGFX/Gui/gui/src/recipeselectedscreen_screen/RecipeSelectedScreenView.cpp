#include <gui/recipeselectedscreen_screen/RecipeSelectedScreenView.hpp>


extern "C" {
#ifdef SIMULATOR
    extern const unsigned char receipe_video[];
    extern unsigned int receipe_video_len;
#else
    extern const uint8_t receipe_video __attribute__((section("receipe_video_section")));
    static unsigned int receipe_video_len = 7264518;

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_jpeg.h>

#include "hwjpeg_decode.h"

    extern struct HwJPEG_Context_s HwJPEG_Context;
#endif
}

extern uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
static uint8_t fileBuffer[100 * 1024];

RecipeSelectedScreenView::RecipeSelectedScreenView() : 
    playing(false),
    videoBitmap(BITMAP_INVALID),
    temporaryBufferBitmap(BITMAP_INVALID),
    movieEndedCallback(this, &RecipeSelectedScreenView::onMovieEnded),
    autoDemoCounter(0),
    autoDemoScroll(false)
{

}

void RecipeSelectedScreenView::setupScreen()
{
    RecipeSelectedScreenViewBase::setupScreen();

    HAL::getInstance()->setFrameRateCompensation(false);

#ifndef SIMULATOR
    JPEG_InitDecode_HW(&HwJPEG_Context);
#endif

    //allocate dynamic bitmap for video
    videoBitmap = Bitmap::dynamicBitmapCreate(354, 212, Bitmap::RGB888);
    temporaryBufferBitmap = Bitmap::dynamicBitmapCreate(354, 212, Bitmap::RGB888);

    //Configure MJPEG widget
    mjpgWidget.setPosition(0, 0, 480, 272);
    mjpgWidget.setLineBuffer(lineBuffer);
    mjpgWidget.setFileBuffer(fileBuffer, sizeof(fileBuffer));

#if USE_BPP==24
    mjpgWidget.setBitmapFormat(Bitmap::RGB888);
#else
#error Unsupported operations
#endif
#ifdef SIMULATOR
    mjpgWidget.setMovie(receipe_video, receipe_video_len);
#else
    mjpgWidget.setMovie(&receipe_video, receipe_video_len);
#endif
    mjpgWidget.setVisible(true);
    mjpgWidget.setAlpha(0);
    mjpgWidget.setPixelData(Bitmap::dynamicBitmapGetAddress(videoBitmap));
    mjpgWidget.setFrameBuffer(Bitmap::dynamicBitmapGetAddress(videoBitmap)); 
    mjpgWidget.setTemporaryFrameBuffer(Bitmap::dynamicBitmapGetAddress(temporaryBufferBitmap));
    mjpgWidget.setTimeBetweenTicks(16);
    mjpgWidget.gotoFrame(3);
    mjpgWidget.setRepeat(false);
    mjpgWidget.setMovieEndedAction(movieEndedCallback);
    scrollableContainerRecipe.add(mjpgWidget);

    fps_mcu.initialize();
    fps_mcu.setXY(480 - fps_mcu.getWidth() - 3, 272 - fps_mcu.getHeight());
    fps_mcu.setVisible(false);
    scrollableContainerRecipe.add(fps_mcu);
}

void RecipeSelectedScreenView::tearDownScreen()
{
    Bitmap::dynamicBitmapDelete(videoBitmap);
    Bitmap::dynamicBitmapDelete(temporaryBufferBitmap);

    RecipeSelectedScreenViewBase::tearDownScreen();
}

void RecipeSelectedScreenView::handleDragEvent(const DragEvent& evt)
{
    RecipeSelectedScreenViewBase::handleDragEvent(evt);
    if (playing)
    {
        mjpgWidget.startFadeAnimation(0, 30);
        buttonPlay.setTouchable(true);
        playing = false;
        mjpgWidget.pause();
    }
}

void RecipeSelectedScreenView::handleGestureEvent(const GestureEvent& evt)
{
    RecipeSelectedScreenViewBase::handleGestureEvent(evt);
    updateVideoPlayButton(false);
}

void RecipeSelectedScreenView::updateVideoPlayButton(bool movingUp)
{
    if (movingUp && buttonPlay.getAlpha() != 0 && buttonPlay.getY() < -buttonPlay.getHeight())
    {
        if (buttonPlay.isFadeAnimationRunning())
        {
            buttonPlay.cancelFadeAnimation();
        }
        buttonPlay.setAlpha(0);
    }
    else if (movingUp && buttonPlay.getAlpha() != 0 && !buttonPlay.isFadeAnimationRunning())
    {
        buttonPlay.startFadeAnimation(0, 15);
    }
    else if (!movingUp && buttonPlay.getAlpha() == 0 && !buttonPlay.isFadeAnimationRunning())
    {
        buttonPlay.startFadeAnimation(255, 15);
    }
}

void RecipeSelectedScreenView::handleTickEvent()
{
    if (playing)
    {
        fps_mcu.setVisible(true);
        fps_mcu.update();
        if (!mjpgWidget.isFadeAnimationRunning())
        {
            mjpgWidget.tick();
        }
    }
    else
    {
        fps_mcu.setVisible(false);
        updateVideoPlayButton(imageDish.getY() != 0);
    }
    if (autoDemoScroll && ((imageDish.getY() + scrollSpeed) <= 0))
    {
        scrollableContainerRecipe.moveChildrenRelative(0, scrollSpeed);
    }
}

void RecipeSelectedScreenView::mcuLoadUpdated(uint8_t mcuLoad)
{
    fps_mcu.setMcuLoad(mcuLoad);
}

void RecipeSelectedScreenView::onMovieEnded()
{
    mjpgWidget.startFadeAnimation(0, 30);
    buttonPlay.setTouchable(true);
    playing = false;
}

void RecipeSelectedScreenView::playClicked()
{
    if (!playing && imageDish.getY() == 0)
    {
        mjpgWidget.startFadeAnimation(255, 30);
        buttonPlay.startFadeAnimation(0, 30);
        buttonPlay.setTouchable(false);
        mjpgWidget.gotoFrame(3);
        playing = true;
        mjpgWidget.showFirstFrame();
        mjpgWidget.play();
    }
}

void RecipeSelectedScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (playing && evt.getType() == ClickEvent::RELEASED)
    {
        mjpgWidget.startFadeAnimation(0, 30);
        buttonPlay.setTouchable(true);

        playing = false;
        mjpgWidget.pause();
    }
    if (autoDemoScroll)
    {
        autoDemoScroll = false;
    }
    RecipeSelectedScreenViewBase::handleClickEvent(evt);
}

void RecipeSelectedScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::RecipeRecipeSelector:
        switch (autoDemoCounter)
        {
        case 8:
            screenClick(ClickEvent::PRESSED, 240, 130);
            break;

        case 10:
            screenClick(ClickEvent::RELEASED, 240, 130);
            break;

        case 260:
            scrollSpeed = -2;
            scrollableContainerRecipe.moveChildrenRelative(0, scrollSpeed);
            autoDemoScroll = true;
            break;

        case 300:
            autoDemoScroll = false;
            break;

        case 304:
            scrollSpeed = 4;
            autoDemoScroll = true;
            break;

        case 324:
            autoDemoScroll = false;
            break;

        case 330:
            screenClick(ClickEvent::PRESSED, 10, 10);
            break;

        case 332:
            presenter->setAutoDemoState(Model::RecipeSelectorHomeUI);
            screenClick(ClickEvent::RELEASED, 10, 10);
            break;        
        }
        
    default:
        break;
    }
    autoDemoCounter++;
}

void RecipeSelectedScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    RecipeSelectedScreenView::handleClickEvent(evt);
}

void RecipeSelectedScreenView::screenDrag(DragEvent::DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY)
{
    DragEvent evt(type, oldX, oldY, newX, newY);
    RecipeSelectedScreenView::handleDragEvent(evt);
}

void RecipeSelectedScreenView::screenGesture(GestureEvent::GestureType type, int16_t v, int16_t x_coord, int16_t y_coord)
{
    GestureEvent evt(type, v, x_coord, y_coord);
    RecipeSelectedScreenView::handleGestureEvent(evt);
}
