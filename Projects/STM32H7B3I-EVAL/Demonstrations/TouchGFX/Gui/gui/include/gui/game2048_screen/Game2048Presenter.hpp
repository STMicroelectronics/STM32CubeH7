#ifndef GAME2048_PRESENTER_HPP
#define GAME2048_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Game2048View;

class Game2048Presenter : public Presenter, public ModelListener
{
public:
    Game2048Presenter(Game2048View& v);
    virtual ~Game2048Presenter() {};

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

    void gotoMenuSelected();
    void mcuLoadUpdated(uint8_t newMcuLoad);
    void screenSaverMinorTick();

private:
    Game2048Presenter();

    Game2048View& view;
};

#endif // GAME2048_PRESENTER_HPP
