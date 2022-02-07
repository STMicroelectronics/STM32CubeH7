#ifndef RECIPESCREEN_VIEW_HPP
#define RECIPESCREEN_VIEW_HPP

#include <gui_generated/recipescreen_screen/RecipeScreenViewBase.hpp>
#include <gui/recipescreen_screen/RecipeScreenPresenter.hpp>

class RecipeScreenView : public RecipeScreenViewBase
{
public:
    RecipeScreenView();
    virtual ~RecipeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);

    //Updates the texts and images when scrolling the scrollListRecipeSelector
    virtual void scrollListRecipeSelectorUpdateItem(RecipeListElement& item, int16_t itemIndex);

    void updateMCULoad(uint8_t load);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);

protected:
    // Detects when a item in scrollListRecipeSelector is selected.
    // If it is the top item the screen is changed otherwise it moves to the top of the scrollListRecipeSelector
    Callback<RecipeScreenView, int16_t> recipeSelectedCallback;
    void recipeSelectedHandler(int16_t item);

    // Detects when scrollListRecipeSelector is done animating to an item.
    // If it is item 0 the content starts fading.
    Callback<RecipeScreenView, void> AnimationEndedCallback;
    void AnimationEndedHandler(void);

    bool fadeTopRecipe;

    uint16_t autoDemoCounter;
};

#endif // RECIPESCREEN_VIEW_HPP
