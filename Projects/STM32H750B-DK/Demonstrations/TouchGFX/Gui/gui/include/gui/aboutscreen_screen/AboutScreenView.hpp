#ifndef ABOUTSCREEN_VIEW_HPP
#define ABOUTSCREEN_VIEW_HPP

#include <gui_generated/aboutscreen_screen/AboutScreenViewBase.hpp>
#include <gui/aboutscreen_screen/AboutScreenPresenter.hpp>
#include <widgets/MJPEGReader.hpp>
#include <gui/containers/FpsMcuLoad.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class AboutScreenView : public AboutScreenViewBase
{
public:
    AboutScreenView();
    virtual ~AboutScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    virtual void designerAboutScrollWheelUpdateItem(AboutWheelElement& item, int16_t itemIndex);
    virtual void designerAboutScrollWheelUpdateCenterItem(AboutWheelElement& item, int16_t itemIndex);
    virtual void playAboutVideo();
    virtual void handleTickEvent();
    void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();

    //Used for emulate a click event on the screen.
    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);
    
protected:    
    Callback<AboutScreenView> scrollWheelAnimationEnddedCallback;
    void scrollWheelAnimationEnddedHandler();

    Callback<AboutScreenView, int16_t> scrollWheelAnimateToCallback;
    void scrollWheelAnimateToHandler(int16_t itemSelected);

    Callback<AboutScreenView, const FadeAnimator<TextArea>&> textFadeEnddedCallback;
    void textFadeEnddedHandler(const FadeAnimator<TextArea>& src);

    Callback<AboutScreenView, const FadeAnimator<Button>&> playButtonFadeEnddedCallback;
    void playButtonFadeEnddedHandler(const FadeAnimator<Button>& src);

    int16_t currentItem;
    bool selectedItemDragged;
    int16_t totalDrag;
    FadeAnimator < MJPEGReader > mJPEGReaderMovie;
    BitmapId videoBitmap;
    BitmapId temporaryBufferBitmap;

    //Buffers in internal RAM
    //Buffer to decode one line of JPEG
    uint8_t lineBuffer[480*1*3 /*Width * Height * Bitdepth*/];
    //RAM for AVI file buffer, 100 Kb, please encode video at max 800 kbits/s
    uint8_t fileBuffer[100 * 1024];
    PixelDataWidget moviePixelWidget;
    BitmapId movieBufferBitmapId;
    BitmapId temporaryBufferBitmapId;
    FpsMcuLoad fps_mcu;
    uint32_t videoScrollYTick;
    int16_t videoScrollYLast;
    int16_t videoScrollYDest;
    int16_t videoScrollDeltaY;
    fileinput::FileHdl f;

private:
    enum USB_STATE
    {
      NO_USB,
      NO_VIDEO,
      VIDEO_DETECTED
    }usbVideoState;
  
    Callback<AboutScreenView> movieEndedCallback;
    void onMovieEnded();
        
    #ifndef SIMULATOR
    STORAGE_IdTypeDef Storage_Id;
    #endif

    uint16_t autoDemoCounter;
    bool autoDemoVideoPresent;
    bool autoDemoScroll;
    enum AutoDemoScrollStates
    {
        DesignerWheel,
        Bottom,
        Top
    } autoScrollStates;
};

#endif // ABOUTSCREEN_VIEW_HPP
