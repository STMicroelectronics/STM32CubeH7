#ifndef BATHROOMSCREEN_VIEW_HPP
#define BATHROOMSCREEN_VIEW_HPP

#include <gui_generated/bathroomscreen_screen/BathroomScreenViewBase.hpp>
#include <gui/bathroomscreen_screen/BathroomScreenPresenter.hpp>

class BathroomScreenView : public BathroomScreenViewBase
{
public:
    BathroomScreenView();
    virtual ~BathroomScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();    

    // Functions called from the presenter, to display the graphics that indicates 
    // if the different UIs is previous entered and the devices is on or off.
    void setBathtubWaterState(bool on);
    void setWasherState(bool on);
    void setBathtubUIState(bool on);
    void setHomeUIState(bool on);

    // startGlowAnimaton() is created with the TouchGFX Designer and starts the animation 
    // that indicates which elements in the bathroom that can be interacted with.
    virtual void startGlowAnimaton();
    
    void updateMCULoad(uint8_t load);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);

protected:

    // Callback registers when the glow animation has reach either a alpha of 255 or 0.
    // Based on the amount of times that a fade animation has been started, 
    // a new fade animation is started or nothing is done.
    Callback< BathroomScreenView, const FadeAnimator<Button>& > fadeButtonCallback;
    void fadebuttonHandler(const FadeAnimator<Button>& src);

    // The Callback registers when the screen is pressed where there are not a element,
    // that can be interacted with.
    Callback<BathroomScreenView, const Image&, const ClickEvent&> clickListnerImageCallback;
    void clickListnerImageHandler(const Image& img, const ClickEvent& evt);

    uint16_t fadeCounter; // Counter used for keeping track of how many times the glow animation has reached alpha of 0 or 255.

    uint16_t autoDemoCounter;
};

#endif // BATHROOMSCREEN_VIEW_HPP
