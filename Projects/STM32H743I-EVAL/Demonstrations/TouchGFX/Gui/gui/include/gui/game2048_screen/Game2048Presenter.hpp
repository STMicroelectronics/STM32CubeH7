
#ifndef GAME_2048_PRESENTER_HPP
#define GAME_2048_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class Game2048View;

class Game2048Presenter : public DemoPresenter
{
public:
    Game2048Presenter(Game2048View& v);
    virtual ~Game2048Presenter();

    virtual void activate();
    virtual void deactivate();

    virtual void screenSaverMinorTick();

    virtual void oneSecond();
    
private:
    Game2048Presenter();

    Game2048View& view;

    int16_t counter;
};

#endif // GAME_2048_PRESENTER_HPP
