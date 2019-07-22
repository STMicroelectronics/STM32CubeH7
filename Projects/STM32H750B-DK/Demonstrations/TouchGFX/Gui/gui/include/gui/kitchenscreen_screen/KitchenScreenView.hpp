#ifndef KITCHENSCREEN_VIEW_HPP
#define KITCHENSCREEN_VIEW_HPP

#include <gui_generated/kitchenscreen_screen/KitchenScreenViewBase.hpp>
#include <gui/kitchenscreen_screen/KitchenScreenPresenter.hpp>



class KitchenScreenView : public KitchenScreenViewBase
{
public:
    KitchenScreenView();
    virtual ~KitchenScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // startGlowAnimaton() is created with the TouchGFX Designer and starts the animation 
    // that indicates which elements in the bathroom that can be interacted with.
    virtual void startGlowAnimaton();

    // Functions called from the presenter, to display the graphics that indicates 
    // if the different UIs is previous entered and the devices is on or off.
    void setLightState(bool on);
    void setCookpotState(bool on);
    void setOvenState(bool on);
    void setOvenUIState(bool on);
    void setHoodUIState(bool on);
    void setHomeUIState(bool on);

    void updateMCULoad(uint8_t load);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);

protected:
    // Callback registers when the glow animation has reach either a alpha of 255 or 0.
    // Based on the amount of times that a fade animation has been started, 
    // a new fade animation is started or nothing is done.
    Callback< KitchenScreenView, const FadeAnimator<Button>& > fadeButtonCallback;
    void fadebuttonHandler(const FadeAnimator<Button>& src);

    // The Callback registers when the screen is pressed where there are not a element,
    // that can be interacted with.
    Callback<KitchenScreenView, const Image&, const ClickEvent&> clickListnerImageCallback;
    void clickListnerImageHandler(const Image& img, const ClickEvent& evt);

    uint16_t fadeCounter; // Counter used for keeping track of how many times the clow animation has reached alpha of 0 or 255.

    uint16_t autoDemoCounter;
};

#endif // KITCHENSCREEN_VIEW_HPP
