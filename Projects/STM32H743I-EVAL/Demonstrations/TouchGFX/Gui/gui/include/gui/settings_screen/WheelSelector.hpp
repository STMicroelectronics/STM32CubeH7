#ifndef WHEEL_SELECTOR_HPP
#define WHEEL_SELECTOR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

/**
 * The WheelSelector class has the functionality of a spinning wheel that can be
 * drag, swiped and clicked. The Wheel contains texts and the vertical spacing 
 * between the text entities can be adjusted.
 * The Wheel has an overlay area where the text changes to a different typography.
 * The setup method handles the setup of all of the graphical elements and must
 * be called prior to usage.
 * Any background and shadowing must be applied outside the WheelSelector.
 */
class WheelSelector : public Container
{
public:
    WheelSelector();

    /**
     * @fn void WheelSelector::setup(BitmapId overlay_, int width, int height, int textHeight_, int textYOffsetInOffset_, int overlayX, int overlayY);
     *
     * @brief Setup the wheel selector 
     *
     * @param overlay_ The background image for the selected area
     * @param width The width of the wheel selector. 
     * @param height The height of the wheel selector.
     * @param textHeight_ The height of each text entity in the wheel. Includes top/bottom margin and the actual text.
     * @param textYOffsetInOffset_ The y coordinate of the text inside each text entity. Use this to place the text relative to the textHeight_.
     * @param overlayX Placement (x position) of the selected overlay area relative to the wheel selector.
     * @param overlayY Placement (y position) of the selected overlay area relative to the wheel selector.
     *
     */
    void setup(BitmapId overlay_, int width, int height, int textHeight_, int textYOffsetInOffset_, int overlayX, int overlayY);
    
    /**
     * @fn void WheelSelector::addText(TypedTextId textId, TypedTextId selectedTextId, const Unicode::UnicodeChar* newText);
     *
     * @brief Adds a new text to the wheel selector 
     *
     * @param textId The wildcard text to be used for the text when outside the selected/overlay area
     * @param selectedTextId The wildcard text to be used for the text when inside the selected/overlay area
     * @param newText The actual text of the text entry
     *
     */
    void addText(TypedTextId textId, TypedTextId selectedTextId, const Unicode::UnicodeChar* newText);

    void setSelectedItem(int index, int duration, EasingEquation equation = EasingEquations::backEaseOut);
    int getSelectedItem();

    void reset();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleGestureEvent(const GestureEvent& evt);

    void setElementSelectedCallback(touchgfx::GenericCallback<const WheelSelector&, const int&>& callback)
    {
        elementSelectedAction = &callback;
    }

    void hideBottomElements(int numberOfElementsToHide);

private:
    static const int MAX_NUMBER_OF_TEXTS = 60;

    enum AnimationState
    {
        ANIMATING_GESTURE, ///< Animating a gesture
        ANIMATING_SNAP,    ///< Animating a 'snap' after a drag/gesture to align index
        ANIMATING_DRAG,    ///< Animating a click+drag
        NO_ANIMATION       ///< No animation
    };

    AnimationState currentAnimationState;

    Image centerOverlay;

    MoveAnimator<Container> textContainer;
    FadeAnimator<TextAreaWithOneWildcard> texts[MAX_NUMBER_OF_TEXTS];
    Unicode::UnicodeChar textbuf[MAX_NUMBER_OF_TEXTS][15];

    MoveAnimator<Container> centerTextContainer;
    FadeAnimator<TextAreaWithOneWildcard> centerTexts[MAX_NUMBER_OF_TEXTS];
    Unicode::UnicodeChar centerTextbuf[MAX_NUMBER_OF_TEXTS][15];

    Container centerTextViewPort;
    int centerTextContainerYOffset;

    int textYOffsetInOffset;

    int overlayCenter;

    int textsIndex;
    int textsWidth;
    int textsHeight;

    int hiddenElements;

    Callback<WheelSelector, const MoveAnimator<Container>&> animationEndedCallback;
    void animationEndedHandler(const MoveAnimator<Container>& source);

    GenericCallback<const WheelSelector&, const int&>* elementSelectedAction;

    int16_t xClick;
    int16_t yClick;
    void moveText(int newYPosition);

    void adjustForBoundaries(int& newYPosition);

    void animateMoveText(int newYPosition, int duration, EasingEquation equation);
    int calculateSnapPosition(int newYPosition);
    void snap();

    int getTextContainerVisibleHeight();
};


#endif /* WHEEL_SELECTOR_HPP */
