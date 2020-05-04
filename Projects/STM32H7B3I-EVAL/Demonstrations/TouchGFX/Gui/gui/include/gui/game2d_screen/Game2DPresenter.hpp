#ifndef GAME2D_PRESENTER_HPP
#define GAME2D_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Game2DView;

class Game2DPresenter : public Presenter, public ModelListener
{
public:
    Game2DPresenter(Game2DView& v);

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

    virtual ~Game2DPresenter() {};

    void gotoMenuSelected();
    virtual void mcuLoadUpdated(uint8_t newMcuLoad);
private:
    Game2DPresenter();

    Game2DView& view;
};

#endif // GAME2D_PRESENTER_HPP
