#include <gui/aboutscreen_screen/AboutScreenView.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#ifdef SIMULATOR
#ifdef __GNUC__
// Comnpilled with GCC, the simulator looks for the video based on where the simulator is opend from.
// The location below matches, when the simulator is started from the TouchGFX Designer.
#define BEHIND_THE_DEMO_AVI "../../../../../Utilities/Media/Video/BehindTheDemo.avi"
#else
#define BEHIND_THE_DEMO_AVI "../../../../../../../Utilities/Media/Video/BehindTheDemo.avi"
#endif
#else
#include "hwjpeg_decode.h"
extern struct HwJPEG_Context_s HwJPEG_Context;
#define BEHIND_THE_DEMO_AVI "BehindTheDemo.avi"
#endif

AboutScreenView::AboutScreenView() :
    scrollWheelAnimationEnddedCallback(this, &AboutScreenView::scrollWheelAnimationEnddedHandler),
    scrollWheelAnimateToCallback(this, &AboutScreenView::scrollWheelAnimateToHandler),
    textFadeEnddedCallback(this, &AboutScreenView::textFadeEnddedHandler),
    playButtonFadeEnddedCallback(this, &AboutScreenView::playButtonFadeEnddedHandler),
    selectedItemDragged(false),
    totalDrag(0),
    videoScrollYTick(0),
    videoScrollYLast(0),
    videoScrollYDest(0),
    videoScrollDeltaY(0),
    usbVideoState(NO_USB),
    movieEndedCallback(this, &AboutScreenView::onMovieEnded),
    #ifndef SIMULATOR
    Storage_Id(USB_DISK_UNIT),
    #endif
    autoDemoCounter(0),
    autoDemoVideoPresent(0),
    autoDemoScroll(0),
    autoScrollStates(DesignerWheel)
{

}

void AboutScreenView::setupScreen()
{
    AboutScreenViewBase::setupScreen();

#ifndef SIMULATOR
    JPEG_InitDecode_HW(&HwJPEG_Context);
#endif

    currentItem = designerAboutScrollWheel.getSelectedItem();
    for (int cnt = 0; designerAboutScrollWheelSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        designerAboutScrollWheelSelectedListItems[cnt].setLargeText();
    }

    designerAboutScrollWheel.setAnimationEndedCallback(scrollWheelAnimationEnddedCallback);
    designerAboutScrollWheel.setAnimateToCallback(scrollWheelAnimateToCallback);
    textAreaAboutInfo.setFadeAnimationEndedAction(textFadeEnddedCallback);
    buttonPlayVideo.setFadeAnimationEndedAction(playButtonFadeEnddedCallback);
    MissingUSBMessage.setFadeAnimationEndedAction(textFadeEnddedCallback);

    videoScrollYLast = 0;
    videoScrollYTick = 0;
    videoScrollYDest = 0;
    videoScrollDeltaY = 0;

    textAreaGettingStarted.setLinespacing(4);
    textAreaDesigner1.setLinespacing(4);
    textAreaDesigner2.setLinespacing(4);
    textAreaAboutInfo.setLinespacing(4);
    textAreaGetStarted1.setLinespacing(4);
    textAreaGetStarted2.setLinespacing(4);
    textAreaAssistance.setLinespacing(4);

    f = fileinput::openFile(BEHIND_THE_DEMO_AVI);
    
    #ifdef SIMULATOR
    if (!f)
    {
        buttonPlayVideo.setVisible(false);
        MissingUSBMessage.setVisible(true);
        MissingUSBMessage.setTypedText(TypedText(T_ABOUTNOVIDEOSIM));
        usbVideoState = NO_VIDEO;
        autoDemoVideoPresent = false;
    }
    else
    {
        buttonPlayVideo.setVisible(true);
        MissingUSBMessage.setVisible(false);
        usbVideoState = VIDEO_DETECTED;
        autoDemoVideoPresent = true;
    }
    #else
    
    if (Storage_GetStatus(Storage_Id) != STORAGE_NOINIT)
    {       
       if (f)
       {
          buttonPlayVideo.setVisible(true);
          MissingUSBMessage.setVisible(false);
          usbVideoState = VIDEO_DETECTED;
          autoDemoVideoPresent = true;
       }
       else
       {
          buttonPlayVideo.setVisible(false);
          MissingUSBMessage.setVisible(true);
          MissingUSBMessage.setTypedText(TypedText(T_ABOUTNOVIDEOUSB));
          usbVideoState = NO_VIDEO;          
          autoDemoVideoPresent = false;
       }
    }
    else
    {
        buttonPlayVideo.setVisible(false);
        MissingUSBMessage.setVisible(true);
        MissingUSBMessage.setTypedText(TypedText(T_ABOUTINSERTUSB));
        usbVideoState = NO_USB;
        autoDemoVideoPresent = false;
    }
    #endif

    //allocate dynamic bitmap for video
    videoBitmap = Bitmap::dynamicBitmapCreate(480, 272, Bitmap::RGB888);
    temporaryBufferBitmap = Bitmap::dynamicBitmapCreate(480, 272, Bitmap::RGB888);

    //Configure MJPEG widget
    mJPEGReaderMovie.setPixelData(Bitmap::dynamicBitmapGetAddress(videoBitmap));
    mJPEGReaderMovie.setFrameBuffer(Bitmap::dynamicBitmapGetAddress(videoBitmap)); /* also initialize the internal frameBuffer pointer */
    mJPEGReaderMovie.setTemporaryFrameBuffer(Bitmap::dynamicBitmapGetAddress(temporaryBufferBitmap));
    mJPEGReaderMovie.setLineBuffer(lineBuffer);
    mJPEGReaderMovie.setFileBuffer(fileBuffer, sizeof(fileBuffer));
    mJPEGReaderMovie.setTimeBetweenTicks(16);
    mJPEGReaderMovie.setRepeat(false);
    mJPEGReaderMovie.setAlpha(0);
    mJPEGReaderMovie.setMovieEndedAction(movieEndedCallback);
    mJPEGReaderMovie.setVisible(false);

    //PixelDataWidget for movie, fullscreen by default
    mJPEGReaderMovie.setPosition(imageVideoBackground.getX(), imageVideoBackground.getY(), HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    mJPEGReaderMovie.setVisible(false);

    scrollableContainerAbout.add(mJPEGReaderMovie);

    fps_mcu.initialize();
    fps_mcu.setVisible(false);
    fps_mcu.setXY(480 - fps_mcu.getWidth() - 3, imageVideoBackground.getY() + imageVideoBackground.getHeight() - fps_mcu.getHeight());

    //textAreaVideoHeadline.setVisible(false);
    //textAreaVideoSubHeadline.setVisible(false);
    scrollableContainerAbout.add(fps_mcu);
}

void AboutScreenView::tearDownScreen()
{
    AboutScreenViewBase::tearDownScreen();
    if(f)
    {
        mJPEGReaderMovie.closeFile();	
    }
}

void AboutScreenView::designerAboutScrollWheelUpdateItem(AboutWheelElement& item, int16_t itemIndex)
{
    item.setText(itemIndex);
}

void AboutScreenView::designerAboutScrollWheelUpdateCenterItem(AboutWheelElement& item, int16_t itemIndex)
{
    item.setText(itemIndex);
}

void AboutScreenView::scrollWheelAnimationEnddedHandler()
{
    for (int cnt = 0; designerAboutScrollWheelSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
    {
        designerAboutScrollWheelSelectedListItems[cnt].setLargeText();
    }
}

void AboutScreenView::scrollWheelAnimateToHandler(int16_t itemSelected)
{
    // Since the value of itemSelected is the previous item +/- "steps" to the new item,
    // the value of the actual number of items and their index is calculated
    int selectedItem = (itemSelected % 9) < 0 ? 9 + (itemSelected % 9) : (itemSelected % 9);

    if (currentItem != selectedItem)
    {
        for (int cnt = 0; designerAboutScrollWheelSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
        {
            designerAboutScrollWheelSelectedListItems[cnt].setSmallText();
            textAreaAboutInfo.startFadeAnimation(0, 30);
        }
    }
    currentItem = selectedItem;
}

void AboutScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        totalDrag = 0;

        if (selectedItemDragged)
        {
            selectedItemDragged = false;
        }
    }
    
    if (autoDemoScroll)
    {
        autoDemoScroll = false;
    }

    AboutScreenViewBase::handleClickEvent(evt);
}

void AboutScreenView::handleTickEvent()
{
    if (mJPEGReaderMovie.isPlaying())
    {
        if (videoScrollDeltaY != 0)
        {
            videoScrollYTick++;
            videoScrollYDest = EasingEquations::cubicEaseInOut(videoScrollYTick, 0, videoScrollDeltaY, 30);
            scrollableContainerAbout.moveChildrenRelative(0, videoScrollYDest - videoScrollYLast);
            videoScrollYLast = videoScrollYDest;

            if (videoScrollDeltaY == videoScrollYDest)
            {
                videoScrollYLast = 0;
                videoScrollYTick = 0;
                videoScrollYDest = 0;
                videoScrollDeltaY = 0;
            }
        }
        else
        {
            if (!mJPEGReaderMovie.isFadeAnimationRunning())
            {
                mJPEGReaderMovie.tick();
                fps_mcu.update();
            }
        }
    }
    else
    {
        #ifdef SIMULATOR
        switch (usbVideoState)
        {
        case NO_VIDEO:
            if (f)
            {
                buttonPlayVideo.setVisible(true);
                MissingUSBMessage.setVisible(false);
                usbVideoState = VIDEO_DETECTED;

                buttonPlayVideo.invalidate();
                MissingUSBMessage.invalidate();
            }
            break;

        case VIDEO_DETECTED:
            if (!f)
            {
                buttonPlayVideo.setVisible(false);
                MissingUSBMessage.setVisible(true);
                MissingUSBMessage.setTypedText(TypedText(T_ABOUTNOVIDEOSIM));
                usbVideoState = NO_VIDEO;

                buttonPlayVideo.invalidate();
                MissingUSBMessage.invalidate();
            }
            break;

        default:
            break;
        }
        #else
        switch (usbVideoState)
        {
        case NO_USB:
            if (Storage_GetStatus(Storage_Id) != STORAGE_NOINIT)
            {
                f = fileinput::openFile(BEHIND_THE_DEMO_AVI);

                if (f)
                {                  
                    usbVideoState = VIDEO_DETECTED;
                }
                else
                {                    
                    usbVideoState = NO_VIDEO;
                }
                MissingUSBMessage.startFadeAnimation(0, 20);
            }
            break;
            
        case NO_VIDEO:
        case VIDEO_DETECTED:  
            if (Storage_GetStatus(Storage_Id) == STORAGE_NOINIT)
            {
                if (buttonPlayVideo.isVisible())
                {
                    buttonPlayVideo.startFadeAnimation(0, 20);
                    if(f)
                    {
                        mJPEGReaderMovie.closeFile();
                    }
                }
                else
                {
                    MissingUSBMessage.startFadeAnimation(0, 20);
                }
                usbVideoState = NO_USB;
                fileinput::deInit();
            }
            break;
        }
        #endif      
    }

    if (autoDemoScroll)
    {
        switch (autoScrollStates)
        {
        case DesignerWheel:
            if (designerAboutScrollWheel.getY() > 30)
            {
                scrollableContainerAbout.moveChildrenRelative(0, -2);
            }
            else
            {
                autoDemoScroll = false;
            }
            break;

        case Bottom:
            if (imageLaptopMjolner.getY() > 138)
            {
                scrollableContainerAbout.moveChildrenRelative(0, -1);
            }
            else
            {
                autoDemoScroll = false;
            }
            break;

        case Top:
            if (buttonHallway.getY() <= -6)
            {
                scrollableContainerAbout.moveChildrenRelative(0, 4);
            }
            else
            {
                autoDemoScroll = false;
            }
            break;
        }        
    }
}

void AboutScreenView::handleDragEvent(const DragEvent& evt)
{
    totalDrag += evt.getDeltaX();

    if(designerAboutScrollWheel.isAnimating())
    {
        if (totalDrag >  3 || totalDrag < -3)
        {
            for (int cnt = 0; designerAboutScrollWheelSelectedListItems.getNumberOfDrawables() > cnt; cnt++)
            {
                designerAboutScrollWheelSelectedListItems[cnt].setSmallText();
            }
        }
        selectedItemDragged = true;
    }

    if (selectedItemDragged)
    {
        scrollableContainerAbout.enableVerticalScroll(false);
    }
    else
    {
        scrollableContainerAbout.enableVerticalScroll(true);
    }

    if (mJPEGReaderMovie.isPlaying())
    {
        mJPEGReaderMovie.pause();
        onMovieEnded();
        videoScrollYLast = 0;
        videoScrollYTick = 0;
        videoScrollYDest = 0;
        videoScrollDeltaY = 0;
    }

    View::handleDragEvent(evt);
}


void AboutScreenView::textFadeEnddedHandler(const FadeAnimator<TextArea>& src)
{
    if(&src == &textAreaAboutInfo)
    {
        if (textAreaAboutInfo.getAlpha() == 0)
        {
            // The switch uses the value calculated in scrollWheelAnimateToHandler() 
            // to set the new mode text.
            switch (currentItem)
            {
            case 0:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTSTRUCTURETEXT));
                break;

            case 1:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTSKINSTEXT));
                break;

            case 2:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTWIDGETSTESXT));
                break;            

            case 3:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTEXAMPLESTEXT));
                break;

            case 4:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTCUSTOMCONTAINERTEXT));
                break;

            case 5:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTTEXTHANDLINGTEXT));
                break;

            case 6:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTCODEGENERATIONTEXT));
                break;

            case 7:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTINTERACTIONSTEXT));
                break;

            case 8:
                textAreaAboutInfo.setTypedText(TypedText(T_ABOUTRUNTEXT));
                break;
            }

            textAreaAboutInfo.startFadeAnimation(255, 30);
        }
    }
    else if (&src == &MissingUSBMessage)
    {
        if (MissingUSBMessage.getAlpha() == 0)
        {
            switch (usbVideoState)
            {
            case VIDEO_DETECTED:
                buttonPlayVideo.setAlpha(0);
                buttonPlayVideo.setVisible(true);
                buttonPlayVideo.startFadeAnimation(255, 20);

                MissingUSBMessage.setVisible(false);
                break;

            case NO_VIDEO:
                MissingUSBMessage.setTypedText(TypedText(T_ABOUTNOVIDEOUSB));
                MissingUSBMessage.startFadeAnimation(255, 20);
                break;

            case NO_USB:
                MissingUSBMessage.setTypedText(TypedText(T_ABOUTINSERTUSB));
                MissingUSBMessage.startFadeAnimation(255, 20);
                break;
            }
        }
    }
}

void AboutScreenView::playButtonFadeEnddedHandler(const FadeAnimator<Button> &src)
{
    if (buttonPlayVideo.getAlpha() == 0)
    {
        switch (usbVideoState)
        {
        case NO_VIDEO:
            MissingUSBMessage.setTypedText(TypedText(T_ABOUTNOVIDEOUSB));
            break;

        case NO_USB:
            MissingUSBMessage.setTypedText(TypedText(T_ABOUTINSERTUSB));
            break;

        default:
            break;
        }
        MissingUSBMessage.setAlpha(0);
        MissingUSBMessage.setVisible(true);
        MissingUSBMessage.startFadeAnimation(255, 50);

        buttonPlayVideo.setVisible(false);
    }
}

void AboutScreenView::playAboutVideo()
{
    if (!mJPEGReaderMovie.isFadeAnimationRunning())
    {
        if (!mJPEGReaderMovie.isPlaying())
        {
            if (mJPEGReaderMovie.setAVIFile(f))
            {
                mJPEGReaderMovie.showFirstFrame();
                mJPEGReaderMovie.gotoFrame(60);
                mJPEGReaderMovie.setVisible(true);
                fps_mcu.setVisible(true);
                mJPEGReaderMovie.play();
                mJPEGReaderMovie.startFadeAnimation(255, 20);
                int16_t firstChildY = scrollableContainerAbout.getFirstChild()->getY();
                videoScrollDeltaY = abs(firstChildY) - (HAL::DISPLAY_HEIGHT + 1);
            }
        }
    }
}

void AboutScreenView::onMovieEnded()
{
    mJPEGReaderMovie.startFadeAnimation(0, 20);       
    fps_mcu.setVisible(false);
}

void AboutScreenView::mcuLoadUpdated(uint8_t mcuLoad)
{
    fps_mcu.setMcuLoad(mcuLoad);
}

//Function that handles the auto demo mode
void AboutScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::AboutHallway:
        if (autoDemoVideoPresent)
        {
            switch (autoDemoCounter)
            {

            case 8:
                playAboutVideo();
                break;

            case 680:
                mJPEGReaderMovie.pause();
                onMovieEnded();
                videoScrollYLast = 0;
                videoScrollYTick = 0;
                videoScrollYDest = 0;
                videoScrollDeltaY = 0;

                autoScrollStates = DesignerWheel;
                autoDemoScroll = true;
                break;

            case 695:
                screenClick(ClickEvent::PRESSED, 420, 50);
                break;

            case 697:
                screenClick(ClickEvent::RELEASED, 420, 50);
                break;

            case 704:
                screenClick(ClickEvent::PRESSED, 420, 50);
                break;

            case 706:
                screenClick(ClickEvent::RELEASED, 420, 50);
                break;

            case 712:
                screenClick(ClickEvent::PRESSED, 420, 50);
                break;

            case 714:
                screenClick(ClickEvent::RELEASED, 420, 50);
                break;

            case 722:
                screenClick(ClickEvent::PRESSED, 50, 50);
                break;

            case 724:
                screenClick(ClickEvent::RELEASED, 50, 50);
                break;

            case 734:
                autoScrollStates = Bottom;
                autoDemoScroll = true;
                break;

            case 770:
                autoScrollStates = Top;
                autoDemoScroll = true;
                break;

            case 800:
                screenClick(ClickEvent::PRESSED, 25, 35);
                break;

            case 802:
                presenter->setAutoDemoState(Model::DemoDone);
                screenClick(ClickEvent::RELEASED, 25, 35);
                break;
            }
        }
        else
        {
            switch (autoDemoCounter)
            {
            case 10:
                autoScrollStates = DesignerWheel;
                autoDemoScroll = true;
                break;

            case 25:
                screenClick(ClickEvent::PRESSED, 420, 50);
                break;

            case 27:
                screenClick(ClickEvent::RELEASED, 420, 50);
                break;

            case 34:
                screenClick(ClickEvent::PRESSED, 420, 50);
                break;

            case 36:
                screenClick(ClickEvent::RELEASED, 420, 50);
                break;

            case 42:
                screenClick(ClickEvent::PRESSED, 420, 50);
                break;

            case 44:
                screenClick(ClickEvent::RELEASED, 420, 50);
                break;

            case 52:
                screenClick(ClickEvent::PRESSED, 50, 50);
                break;

            case 54:
                screenClick(ClickEvent::RELEASED, 50, 50);
                break;

            case 64:
                autoScrollStates = Bottom;
                autoDemoScroll = true;
                break;

            case 100:
                autoScrollStates = Top;
                autoDemoScroll = true;
                break;

            case 130:
                screenClick(ClickEvent::PRESSED, 25, 35);
                break;

            case 132:
                presenter->setAutoDemoState(Model::DemoDone);
                screenClick(ClickEvent::RELEASED, 25, 35);
                break;
            }
        }

    default:
        break;
    }
    autoDemoCounter++;
}

void AboutScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    AboutScreenView::handleClickEvent(evt);
}
