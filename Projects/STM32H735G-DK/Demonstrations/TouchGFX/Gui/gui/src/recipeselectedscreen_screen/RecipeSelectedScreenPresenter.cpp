#include <gui/recipeselectedscreen_screen/RecipeSelectedScreenView.hpp>
#include <gui/recipeselectedscreen_screen/RecipeSelectedScreenPresenter.hpp>

RecipeSelectedScreenPresenter::RecipeSelectedScreenPresenter(RecipeSelectedScreenView& v)
    : view(v)
{
}

void RecipeSelectedScreenPresenter::activate()
{
    model->setCurrentView(Model::Recipe);
    model->setShowPot(true);
}

void RecipeSelectedScreenPresenter::deactivate()
{

}

void RecipeSelectedScreenPresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
    view.mcuLoadUpdated(mcuLoad);
}

void RecipeSelectedScreenPresenter::autoDemoTick()
{
    view.autoDemoTick();
}

void RecipeSelectedScreenPresenter::setAutoDemoState(Model::AutoDemoStates state)
{
    model->setAutoDemoState(state);
}

Model::AutoDemoStates RecipeSelectedScreenPresenter::getAutoDemoState()
{
    return model->getAutoDemoState();
}

void RecipeSelectedScreenPresenter::autoDemoStart()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHallwayScreenScreenNoTransition();
}
