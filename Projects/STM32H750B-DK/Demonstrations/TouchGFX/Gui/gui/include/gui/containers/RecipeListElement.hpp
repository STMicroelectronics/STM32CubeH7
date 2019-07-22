#ifndef RECIPELISTELEMENT_HPP
#define RECIPELISTELEMENT_HPP

#include <gui_generated/containers/RecipeListElementBase.hpp>

class RecipeListElement : public RecipeListElementBase
{
public:
    RecipeListElement();
    virtual ~RecipeListElement() {}

    virtual void initialize();

    void updateItem(int16_t item);

    void startFadeAnimation(uint8_t endAlpha, uint16_t duration);
    void stopFadeAnimation();
    bool isFadeAnimationRunning();

protected:
    Callback<RecipeListElement, const FadeAnimator<Image>&> FadeAnimationEndedCallback;
    void FadeAnimationEndedHandler(const FadeAnimator<Image>& image);

};

#endif // RECIPELISTELEMENT_HPP
