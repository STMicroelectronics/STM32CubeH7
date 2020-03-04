#ifndef MAINMENUCAROUSEL_PRESENTER_HPP
#define MAINMENUCAROUSEL_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainMenuCarouselView;

class MainMenuCarouselPresenter : public Presenter, public ModelListener
{
public:
    MainMenuCarouselPresenter(MainMenuCarouselView& v);
    virtual ~MainMenuCarouselPresenter() {};

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    void setSelectedDemoScreen(Defines::DemoID demoId);
    void setPreviousSelectedMenuType(Defines::MainMenuType menuType);

    Defines::DemoID getSelectedDemoScreen();

    /**
    * @fn MainMenuType MainMenuPresenter::getPreviousSelectedMenuType();
    *
    * @brief Gets the previous selected menu type.
    *        Menu types are saved so that you can return to the same menu type
    *        when leaving a demo screen. NO_MENU is used when toggling between
    *        menus.
    *
    * @return The previous selected menu type.
    */
    Defines::MainMenuType getPreviousSelectedMenuType();

    virtual void screenSaverMinorTick();
    virtual void mcuLoadUpdated(uint8_t newMcuLoad);

private:
    MainMenuCarouselPresenter();

    MainMenuCarouselView& view;
};

#endif // MAINMENUCAROUSEL_PRESENTER_HPP
