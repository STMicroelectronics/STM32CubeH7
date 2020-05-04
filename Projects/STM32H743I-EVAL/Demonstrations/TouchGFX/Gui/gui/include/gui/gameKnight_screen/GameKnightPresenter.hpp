#ifndef GAME_KNIGHT_PRESENTER_HPP
#define GAME_KNIGHT_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/Types.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class GameKnightView;

#if defined(ST) && !defined(SIMULATOR)
extern volatile bool disableChromArt;
#endif

/**
 * The Presenter for the main screen. In the MVP paradigm, the presenter acts upon
 * events from the model and the view. By deriving from ModelListener, the presenter
 * implements the interface which the model uses to inform the current presenter of
 * events and model state changes. The presenter also contains a reference to the
 * associated view, and uses this to update the view accordingly.
 *
 */
class GameKnightPresenter : public DemoPresenter
{
public:
    GameKnightPresenter(GameKnightView& v);

    virtual ~GameKnightPresenter()
    {
    };

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

    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    void setChromArtStatus(bool chromArtStatus);
    virtual void oneSecond();
    virtual void screenSaverMinorTick();
    
private:
    GameKnightPresenter();
    GameKnightView& view;

    int16_t counter;
};

#endif 
