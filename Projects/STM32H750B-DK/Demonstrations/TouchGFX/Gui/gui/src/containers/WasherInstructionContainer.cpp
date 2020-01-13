#include <gui/containers/WasherInstructionContainer.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern "C" {

#ifdef SIMULATOR
    extern const unsigned char washer_video[];
    extern unsigned int washer_video_len;
#else
    extern const unsigned char washer_video __attribute__((section("washer_video_section")));
    static unsigned int washer_video_len = 5273106;

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_jpeg.h>

#include "hwjpeg_decode.h"

    struct HwJPEG_Context_s HwJPEG_Context;
#endif
}

#if !defined(USE_BPP) || USE_BPP==16
#ifdef SIMULATOR
uint8_t pixeldata[354 * 212 * 2];
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
#else
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
_Pragma("data_alignment = 64")
uint8_t pixeldata[354 * 212 * 2];/*@"NonCacheable"*/;
#endif

#elif USE_BPP==24
#ifdef SIMULATOR
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
uint8_t fileBuffer[100 * 1024];
#else
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
uint8_t fileBuffer[100 * 1024];
#endif
#endif

WasherInstructionContainer::WasherInstructionContainer() :
    boxClickedCallback(this, &WasherInstructionContainer::boxClickedHandler),
    movieEndedCallback(this, &WasherInstructionContainer::movieEndedHandler),
    tickCounter(0), playing(true), movieBufferBitmapId(0), temporaryBufferBitmapId(0)
{

}

WasherInstructionContainer::~WasherInstructionContainer()
{
    Bitmap::dynamicBitmapDelete(movieBufferBitmapId);
    Bitmap::dynamicBitmapDelete(temporaryBufferBitmapId);

#ifndef SIMULATOR
    JPEG_StopDecode_HW(&HwJPEG_Context);
#endif
}

void WasherInstructionContainer::initialize()
{
    WasherInstructionContainerBase::initialize();

    HAL::getInstance()->setFrameRateCompensation(false);

#ifndef SIMULATOR
    JPEG_InitDecode_HW(&HwJPEG_Context);
#endif

    if (!movieBufferBitmapId)
    {
        movieBufferBitmapId = Bitmap::dynamicBitmapCreate(354, 212, Bitmap::RGB888);
    }
    if (!temporaryBufferBitmapId)
    {
        temporaryBufferBitmapId = Bitmap::dynamicBitmapCreate(354, 212, Bitmap::RGB888);
    }

#if 1
    //Configure MJPEG widget
    mjpgWidget.setPosition(0, 0, 354, 212);
    mjpgWidget.setLineBuffer(lineBuffer);
    mjpgWidget.setFileBuffer(fileBuffer, sizeof(fileBuffer));

#if USE_BPP==24
    mjpgWidget.setBitmapFormat(Bitmap::RGB888);
#else
#error Unsupported operations
#endif
    mjpgWidget.setPixelData(Bitmap::dynamicBitmapGetAddress(movieBufferBitmapId));
    mjpgWidget.setFrameBuffer(Bitmap::dynamicBitmapGetAddress(movieBufferBitmapId));
    mjpgWidget.setTemporaryFrameBuffer(Bitmap::dynamicBitmapGetAddress(temporaryBufferBitmapId));
    mjpgWidget.setMovieEndedAction(movieEndedCallback);
#ifdef SIMULATOR
    mjpgWidget.setMovie(washer_video, washer_video_len);
#else
    mjpgWidget.setMovie(&washer_video, washer_video_len);
#endif
    mjpgWidget.setTimeBetweenTicks(16);
    mjpgWidget.setVisible(true);

    mjpgContainer.add(mjpgWidget);
    mjpgContainer.setPosition(147, 54, 300, 180);
#endif

    add(mjpgContainer);

    boxStartStopClick.setClickAction(boxClickedCallback);

    imageIndication.setAlpha(0);
    //move indicator to front
    remove(imageIndication);
    add(imageIndication);

    fpsMcu.initialize();
    fpsMcu.setXY(480 - fpsMcu.getWidth() - 3, 272 - fpsMcu.getHeight());

    add(fpsMcu);
}

void WasherInstructionContainer::tickEvent()
{
    ++tickCounter;
    if (tickCounter && playing)
    {
        TypedText headline, body;
        if (getVideoTexts(tickCounter, headline, body))
        {
            textAreaHeadline.setTypedText(headline);
            textAreaHeadline.invalidate();
            textAreaBodyText.setTypedText(body);
            textAreaBodyText.invalidate();
        }
        fpsMcu.update();
        mjpgWidget.tick();
    }
}


bool WasherInstructionContainer::getVideoTexts(int frameno, TypedText& headline, TypedText& body)
{
    switch (frameno)
    {
    case 1:
        headline = TypedText(T_VIDEOHEADLINE1);
        body = TypedText(T_VIDEOTEXT1);
        return true;
    case 300:
        headline = TypedText(T_VIDEOHEADLINE2);
        body = TypedText(T_VIDEOTEXT2);
        return true;
    case 500:
        headline = TypedText(T_VIDEOHEADLINE3);
        body = TypedText(T_VIDEOTEXT3);
        return true;
    case 740:
        headline = TypedText(T_VIDEOHEADLINE4);
        body = TypedText(T_VIDEOTEXT4);
        return true;
    default:
        return false;
    }
}

void WasherInstructionContainer::boxClickedHandler(const Box& b, const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        if (playing)
        {
            playing = false;
            mjpgWidget.pause();
            imageIndication.setBitmap(Bitmap(BITMAP_VIDEO_PAUSE_BUTTON_ID));
            imageIndication.setAlpha(255);
            imageIndication.invalidate();
            imageIndication.startFadeAnimation(0, 25);
        }
        else
        {
            imageIndication.setBitmap(Bitmap(BITMAP_VIDEO_PLAY_BUTTON_ID));
            imageIndication.setAlpha(255);
            imageIndication.invalidate();
            imageIndication.startFadeAnimation(0, 25);
            mjpgWidget.play();
            playing = true;
        }
    }
}

void WasherInstructionContainer::resetContainer()
{
    tickCounter = 0;
    playing = true;
    mjpgWidget.showFirstFrame();
    mjpgWidget.gotoFrame(1);
    mjpgWidget.play();

    textAreaHeadline.setTypedText(TypedText(T_VIDEOHEADLINE1));
    textAreaHeadline.invalidate();
    textAreaBodyText.setTypedText(TypedText(T_VIDEOTEXT1));
    textAreaBodyText.invalidate();
}

void WasherInstructionContainer::setMcuLoad(uint8_t load)
{
    fpsMcu.setMcuLoad(load);
}

void WasherInstructionContainer::movieEndedHandler()
{
    tickCounter = 0;
    playing = true;
    mjpgWidget.showFirstFrame();
    mjpgWidget.gotoFrame(1);
    mjpgWidget.play();
}
