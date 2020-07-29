#ifndef HALLWAYSCREEN_VIEW_HPP
#define HALLWAYSCREEN_VIEW_HPP

#include <gui_generated/hallwayscreen_screen/HallwayScreenViewBase.hpp>
#include <gui/hallwayscreen_screen/hallwayScreenPresenter.hpp>

class HallwayScreenView : public HallwayScreenViewBase
{
public:
    HallwayScreenView();
    virtual ~HallwayScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);    

protected:

    // The callback registers when a move animaiton for bathroom- and kitchenbutton is done.
    Callback<HallwayScreenView, const MoveAnimator<Button>&> buttonMoveCallback;
    void buttonMoveHandler(const MoveAnimator<Button>& src);

    // The callback registers when a fade animaiton for bathroom- and kitchenbutton is done.
    Callback<HallwayScreenView, const FadeAnimator< MoveAnimator<Button> >&> buttonFadeCallback;
    void buttonFadeHandler(const FadeAnimator< MoveAnimator<Button> >& src);

    // The callback registers when the clouds should be restarted.
    Callback<HallwayScreenView, const MoveAnimator<Container>&> cloudMoveCallback;
    void cloudMoveHandler(const MoveAnimator<Container>& src);

    bool btnCanceld;
    uint16_t autoDemoCounter;
};

#endif // HALLWAYSCREEN_VIEW_HPP
