#include <gui/recipescreen_screen/RecipeScreenView.hpp>
#include <gui/recipescreen_screen/RecipeScreenPresenter.hpp>

RecipeScreenPresenter::RecipeScreenPresenter(RecipeScreenView& v) : 
    view(v)
{
}

void RecipeScreenPresenter::activate()
{
    model->setCurrentView(Model::RecipeSelector);
}

void RecipeScreenPresenter::deactivate()
{
    model->setFromRecipeScreen(true);
}

void RecipeScreenPresenter::mcuLoadUpdated(uint8_t load)
{
    view.updateMCULoad(load);
}

void RecipeScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void RecipeScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates RecipeScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void RecipeScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
