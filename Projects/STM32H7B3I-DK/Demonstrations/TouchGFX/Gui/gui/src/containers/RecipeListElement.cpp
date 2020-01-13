#include <gui/containers/RecipeListElement.hpp>
#include "BitmapDatabase.hpp"
#include "texts/TextKeysAndLanguages.hpp"

RecipeListElement::RecipeListElement() :
    FadeAnimationEndedCallback(this, &RecipeListElement::FadeAnimationEndedHandler)
{

}

void RecipeListElement::initialize()
{
    RecipeListElementBase::initialize();
    imageRecipe.setFadeAnimationEndedAction(FadeAnimationEndedCallback);
}

void RecipeListElement::updateItem(int16_t item)
{
    switch (item)
    {
    case 0:
        imageRecipe.setBitmap(Bitmap(BITMAP_PASTA_01_ID));
        textAreaRecipeHeadline.setTypedText(TypedText(T_RECIPEHEADLINE1));
        textAreaRecipeText.setTypedText(TypedText(T_RECIPETEXT1));
        boxLine.setVisible(false);
        break;

    case 1:
        imageRecipe.setBitmap(Bitmap(BITMAP_PASTA_02_ID));
        textAreaRecipeHeadline.setTypedText(TypedText(T_RECIPEHEADLINE2));
        textAreaRecipeText.setTypedText(TypedText(T_RECIPETEXT2));
        boxLine.setVisible(true);
        break;

    case 2:
        imageRecipe.setBitmap(Bitmap(BITMAP_PASTA_03_ID));
        textAreaRecipeHeadline.setTypedText(TypedText(T_RECIPEHEADLINE3));
        textAreaRecipeText.setTypedText(TypedText(T_RECIPETEXT3));
        boxLine.setVisible(true);
        break;

    case 3:
        imageRecipe.setBitmap(Bitmap(BITMAP_PASTA_04_ID));
        textAreaRecipeHeadline.setTypedText(TypedText(T_RECIPEHEADLINE4));
        textAreaRecipeText.setTypedText(TypedText(T_RECIPETEXT4));
        boxLine.setVisible(true);
        break;

    case 4:
        imageRecipe.setBitmap(Bitmap(BITMAP_PASTA_05_ID));
        textAreaRecipeHeadline.setTypedText(TypedText(T_RECIPEHEADLINE5));
        textAreaRecipeText.setTypedText(TypedText(T_RECIPETEXT5));
        boxLine.setVisible(true);
        break;

    case 5:
        imageRecipe.setBitmap(Bitmap(BITMAP_PASTA_06_ID));
        textAreaRecipeHeadline.setTypedText(TypedText(T_RECIPEHEADLINE6));
        textAreaRecipeText.setTypedText(TypedText(T_RECIPETEXT6));
        boxLine.setVisible(true);
        break;

    case 6:
        imageRecipe.setBitmap(Bitmap(BITMAP_PASTA_07_ID));
        textAreaRecipeHeadline.setTypedText(TypedText(T_RECIPEHEADLINE7));
        textAreaRecipeText.setTypedText(TypedText(T_RECIPETEXT7));
        boxLine.setVisible(true);
        break;
    }
}

void RecipeListElement::startFadeAnimation(uint8_t endAlpha, uint16_t duration)
{
    imageRecipe.startFadeAnimation(endAlpha, duration);
    textAreaRecipeHeadline.startFadeAnimation(endAlpha, duration);
    textAreaRecipeText.startFadeAnimation(endAlpha, duration);
}

void RecipeListElement::FadeAnimationEndedHandler(const FadeAnimator<Image>& image)
{
    if (image.getAlpha() == 55)
    {
        startFadeAnimation(255, 45);
    }
}


void RecipeListElement::stopFadeAnimation()
{
    imageRecipe.cancelFadeAnimation();
    textAreaRecipeHeadline.cancelFadeAnimation();
    textAreaRecipeText.cancelFadeAnimation();

    imageRecipe.setAlpha(255);
    textAreaRecipeHeadline.setAlpha(255);
    textAreaRecipeText.setAlpha(255);

    imageRecipe.invalidate();
    textAreaRecipeHeadline.invalidate();
    textAreaRecipeText.invalidate();
}

bool RecipeListElement::isFadeAnimationRunning()
{
    return imageRecipe.isFadeAnimationRunning();
}
