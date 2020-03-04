#ifndef GAME_2D_PRESENTER_HPP
#define GAME_2D_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class Game2DView;

class Game2DPresenter : public DemoPresenter
{
public:
    Game2DPresenter(Game2DView& v);
    virtual ~Game2DPresenter();

    virtual void activate();
    virtual void deactivate();

    virtual void oneSecond();
    virtual void screenSaverMinorTick();

private:
    Game2DPresenter();

    Game2DView& view;

    int16_t counter;
};

#endif // GAME_2D_PRESENTER_HPP
