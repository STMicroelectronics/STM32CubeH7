#ifndef CONTROLS_VIEW_HPP
#define CONTROLS_VIEW_HPP

#include <gui_generated/controls_screen/ControlsViewBase.hpp>
#include <gui/controls_screen/ControlsPresenter.hpp>
#include <gui/controls_screen/CircularValueIndicator.hpp>
#include <gui/common/SwipeContainer.hpp>

class ControlsView : public ControlsViewBase
{
public:
    ControlsView();
    virtual ~ControlsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void mcuLoadUpdated(uint8_t newMcuLoad);

    void setPotentiometerValue(int value);
    void setBrightnessValue(int value);

    void screenSaverMinorTick();
protected:

    static const int CANVAS_BUFFER_SIZE = 1024 * 18;

    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

    CircularValueIndicator potentiometerValue;

    SwipeContainer swipeContainer;
    Callback<ControlsView, const SwipeContainer&> swipeEndedCallback;
    void swipeEndedHandler(const SwipeContainer& sc);

    void updateBrightnessText(int value);
    void updateHeadline();

    virtual void newBrightnessValueHandler(int value);
};

#endif // CONTROLS_VIEW_HPP
