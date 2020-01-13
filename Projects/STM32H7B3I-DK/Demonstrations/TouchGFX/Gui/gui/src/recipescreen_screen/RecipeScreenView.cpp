#include <gui/recipescreen_screen/RecipeScreenView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

#include <gui/common/Defines.hpp>

RecipeScreenView::RecipeScreenView() :    
    recipeSelectedCallback(this, &RecipeScreenView::recipeSelectedHandler),
    AnimationEndedCallback(this, &RecipeScreenView::AnimationEndedHandler),
    autoDemoCounter(0)
{

}

void RecipeScreenView::setupScreen()
{
    RecipeScreenViewBase::setupScreen();
    scrollListRecipeSelector.setItemSelectedCallback(recipeSelectedCallback);
    scrollListRecipeSelector.setAnimationEndedCallback(AnimationEndedCallback);

    mcuLoad.initialize();
    mcuLoad.setMCUOnly(true);
    mcuLoad.setTextColor(Color::getColorFrom24BitRGB(255, 255, 255));
}

void RecipeScreenView::tearDownScreen()
{
    RecipeScreenViewBase::tearDownScreen();
}

void RecipeScreenView::scrollListRecipeSelectorUpdateItem(RecipeListElement& item, int16_t itemIndex)
{
    item.updateItem(itemIndex);
}

void RecipeScreenView::recipeSelectedHandler(int16_t item)
{
    if (item == 0)
    {
        application().gotoRecipeSelectedScreenScreenCoverTransitionNorth();
    }
    else
    {
        scrollListRecipeSelector.animateToItem(0, item * 10);
        fadeTopRecipe = true;
    }
}

void RecipeScreenView::AnimationEndedHandler()
{
    if (fadeTopRecipe)
    {
        fadeTopRecipe = false;

        //The for loop runs through the visible items, starts a fade animation it finds item 0.
        for (int i = 0; i < scrollListRecipeSelectorListItems.getNumberOfDrawables(); i++)
        {
            if (scrollListRecipeSelector.getItem(i) == 0)
            {
                scrollListRecipeSelectorListItems[i].startFadeAnimation(RECIPE_FADE_END_ALPHA, RECIPE_FADE_DURATION);
                return;
            }
        }
    }
}

void RecipeScreenView::updateMCULoad(uint8_t load)
{
    mcuLoad.setMcuLoad(load);
}

void RecipeScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::RecipeSelectorRecipe:
        switch (autoDemoCounter)
        {
        case 8:
            scrollListRecipeSelector.animateToItem(6, 200);
            break;

        case 26:
            screenClick(ClickEvent::PRESSED, 200, 100);
            break;

        case 28:
            screenClick(ClickEvent::RELEASED, 200, 100);
            break;

        case 50:
            screenClick(ClickEvent::PRESSED, 200, 100);
            break;

        case 52:
            presenter->setAutoDemoState(Model::RecipeRecipeSelector);
            screenClick(ClickEvent::RELEASED, 200, 100);
            break;

        default:
            break;
        }
        break;

    case Model::RecipeSelectorHomeUI:
        switch (autoDemoCounter)
        {
        case 10:
            screenClick(ClickEvent::PRESSED, 10, 10);
            break;

        case 12:
            presenter->setAutoDemoState(Model::HomeUIKitchen);
            screenClick(ClickEvent::RELEASED, 10, 10);
            break;
        }
        break;

    default:
        break;
    }

    autoDemoCounter++;
}

void RecipeScreenView::handleClickEvent(const ClickEvent& evt)
{
    //The for loop runs through the visible items, starts a fade animation it finds item 0.
    for (int i = 0; i < scrollListRecipeSelectorListItems.getNumberOfDrawables(); i++)
    {
        if (scrollListRecipeSelector.getItem(i) == 0)
        {
            if (scrollListRecipeSelectorListItems[i].isFadeAnimationRunning())
            {
                scrollListRecipeSelectorListItems[i].stopFadeAnimation();
            }
            break;
        }
    }
    RecipeScreenViewBase::handleClickEvent(evt);
}

void RecipeScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    RecipeScreenView::handleClickEvent(evt);
}
