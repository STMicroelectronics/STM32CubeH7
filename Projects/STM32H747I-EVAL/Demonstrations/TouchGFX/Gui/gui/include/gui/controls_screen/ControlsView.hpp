

#ifndef CONTROLS_VIEW_HPP
#define CONTROLS_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/controls_screen/ControlsPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/controls_screen/CircularValueIndicator.hpp>
#include <touchgfx/containers/Container.hpp>
#include <gui/common/SwipeContainer.hpp>
#include <touchgfx/containers/Slider.hpp>

using namespace touchgfx;

class ControlsView : public DemoView<ControlsPresenter>
{
public:
    ControlsView();
    virtual ~ControlsView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();

#ifdef USE_STM32H747I_DISCO
    void setJunctionTempValue(int value);
#else
    void setPotentiometerValue(int value);
#endif

    void setBrightnessValue(int value);

    void screenClick(ClickEvent::ClickEventType type, int x, int y);
    void screenDrag(int16_t x_old, int16_t y_old, int16_t x_new, int16_t y_new);

private:
    static const int CANVAS_BUFFER_SIZE = 1024 * 18;

    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

    Image background;
    Image dots;

    TextArea headline;

#ifdef USE_STM32H747I_DISCO
    Container JunctionTempScreen;
    Image board;
    Image line;
    CircularValueIndicator JunctionTempValue;
#else
    Container potentiometerScreen;
    Image board;
    Image line;
    CircularValueIndicator potentiometerValue;
#endif

    Container brightnessScreen;
    Image brightnessSmall;
    Image brightnessLarge;
    Slider slider;

    TextAreaWithOneWildcard brightnessText;
    Unicode::UnicodeChar brightnessTextBuffer[5];
    TextArea brightnessPercentageText;

    SwipeContainer swipeContainer;
    Callback<ControlsView, const SwipeContainer&> swipeEndedCallback;
    void swipeEndedHandler(const SwipeContainer& sc);
    
    Callback<ControlsView, const Slider&, int> newValueCallback;
    void newValueHandler(const Slider& slider, int value);
    
    void updateBrightnessText(int value);
    void updateHeadline();

};

#endif // CONTROLS_VIEW_HPP
