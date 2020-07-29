#ifndef RECIPESCREEN_PRESENTER_HPP
#define RECIPESCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class RecipeScreenView;

class RecipeScreenPresenter : public Presenter, public ModelListener
{
public:
    RecipeScreenPresenter(RecipeScreenView& v);

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

    virtual ~RecipeScreenPresenter() {};

    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();
    virtual void autoDemoStart();

    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();


private:
    RecipeScreenPresenter();

    RecipeScreenView& view;
};


#endif // RECIPESCREEN_PRESENTER_HPP
