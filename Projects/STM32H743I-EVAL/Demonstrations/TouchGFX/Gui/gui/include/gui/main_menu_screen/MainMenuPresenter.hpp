
#ifndef MAIN_MENU_PRESENTER_HPP
#define MAIN_MENU_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>
#include <gui/common/Defines.hpp>

using namespace touchgfx;

class MainMenuView;

class MainMenuPresenter : public DemoPresenter
{
public:
    MainMenuPresenter(MainMenuView& v);
    virtual ~MainMenuPresenter() {};

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

    void gotoSTMenu();

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

    virtual void oneSecond();

    void setAutoMove(bool move)
    {
        autoMove = move;
    }

private:
    MainMenuPresenter();
    MainMenuView& view;

    int moveCounter;
    bool autoMove;
};

#endif // MAIN_MENU_PRESENTER_HPP
