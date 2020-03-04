#ifndef BUMP_MAP_VIEW_HPP
#define BUMP_MAP_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/bump_map_screen/BumpMapPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <gui/bump_map_screen/BumpMapImage.hpp>

using namespace touchgfx;

class BumpMapView : public DemoView<BumpMapPresenter>
{
public:
    BumpMapView();
    virtual ~BumpMapView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void screenSaverMinorTick();

    void screenClick(ClickEvent::ClickEventType type, int x, int y);
    
private:
    enum States
    {
        ANIMATE_TO_NEXT_IMAGE,
        ANIMATE_SHADE_UP,
        NO_ANIMATION
    } currentState;

    static const int NUMBER_OF_BUMP_MAPS = 2;
    struct
    {
        BitmapId image;
        const unsigned int* bump_map;
    } bumpMapInfo[NUMBER_OF_BUMP_MAPS];

    uint32_t animationCounter;
    uint8_t currentBumpMap;

    Box shade;
    BumpMapImage bumpMapImage;
    Button nextButton;

    Callback<BumpMapView, const AbstractButton&> onButtonPressed;

    void buttonPressedhandler(const AbstractButton& button);
    void updateBumpMapImage();
    void animateToNextImage();
    void animateShadeUp();

    int8_t originalTouchSampleRate;
};

#endif
