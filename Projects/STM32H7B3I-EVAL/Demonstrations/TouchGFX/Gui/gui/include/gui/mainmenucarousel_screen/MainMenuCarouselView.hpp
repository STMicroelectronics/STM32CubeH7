#ifndef MAINMENUCAROUSEL_VIEW_HPP
#define MAINMENUCAROUSEL_VIEW_HPP

#include <gui_generated/mainmenucarousel_screen/MainMenuCarouselViewBase.hpp>
#include <gui/mainmenucarousel_screen/MainMenuCarouselPresenter.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <gui/common/Carousel.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/common/Defines.hpp>

class MainMenuCarouselView : public MainMenuCarouselViewBase
{
public:
    MainMenuCarouselView();
    virtual ~MainMenuCarouselView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleClickEvent(const ClickEvent& evt);

    virtual void screenSaverMinorTick();
    void mcuLoadUpdated(uint8_t newMcuLoad);

protected:
    enum State
    {
        ELEMENT_SELECTED,
        NO_STATE
    };

    State currentState;

    Defines::DemoID demoIDs[Defines::NUMBER_OF_DEMO_SCREENS];
    Defines::DemoID selectedDemoScreen;

    Carousel carousel;

    TEXTS headlines[Defines::NUMBER_OF_DEMO_SCREENS];
    TEXTS texts[Defines::NUMBER_OF_DEMO_SCREENS];

    int pauses;

    void setupCarousel();

    Callback<MainMenuCarouselView, const Carousel&> carouselAnimationEnded;
    void carouselAnimationEndedhandler(const Carousel& c);

    void updateDemoText();

    void setSelectedDemoScreen(Defines::DemoID demoId);
    void gotoSelectedDemoScreen();

    int16_t dragDistance;
    bool dragInsideCarousel;
    bool dragInsideCenterButton;
    bool hasAnimatedDrag;
};

#endif // MAINMENUCAROUSEL_VIEW_HPP
