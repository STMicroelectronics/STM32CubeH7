#include <gui/settings_screen/WheelSelector.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

WheelSelector::WheelSelector() :
    currentAnimationState(NO_ANIMATION),
    textsIndex(0),
    animationEndedCallback(this, &WheelSelector::animationEndedHandler),
    elementSelectedAction(0)
{
}

void WheelSelector::setup(BitmapId overlay_, int width, int height, int textHeight_, int textYOffsetInOffset_, int overlayX, int overlayY)
{
    setTouchable(true);

    textYOffsetInOffset = textYOffsetInOffset_;

    centerOverlay.setBitmap(Bitmap(overlay_));
    textsWidth = width;
    textsHeight = textHeight_;
    centerOverlay.setXY(overlayX, overlayY);

    textContainer.setPosition(0, 0, textsWidth, 0);
    textContainer.setMoveAnimationEndedAction(animationEndedCallback);
    add(textContainer);

    add(centerOverlay);

    centerTextViewPort.setPosition(0, centerOverlay.getY(), centerOverlay.getWidth(), textsHeight);
    add(centerTextViewPort);

    centerTextContainerYOffset = centerTextViewPort.getY();

    centerTextContainer.setXY(textContainer.getX(), textContainer.getY() - centerTextContainerYOffset);
    centerTextContainer.setWidth(textsWidth);
    centerTextContainer.setHeight(0);
    centerTextViewPort.add(centerTextContainer);

    hiddenElements = 0;

    setWidth(width);
    setHeight(height);
}

void WheelSelector::addText(TypedTextId textId, TypedTextId selectedTextId, const Unicode::UnicodeChar* newText)
{
    Unicode::snprintf(textbuf[textsIndex], MAX_TEXT_SIZE, "%s", newText);
    texts[textsIndex].setTypedText(TypedText(textId));
    texts[textsIndex].setWildcard(textbuf[textsIndex]);
    texts[textsIndex].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    texts[textsIndex].setXY(0, textsIndex * textsHeight + textYOffsetInOffset);
    texts[textsIndex].setWidth(textsWidth);
    textContainer.add(texts[textsIndex]);
    textContainer.setHeight((textsIndex + 1) * textsHeight);

    centerTexts[textsIndex].setTypedText(TypedText(selectedTextId));
    centerTexts[textsIndex].setWildcard(textbuf[textsIndex]);
    centerTexts[textsIndex].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    centerTexts[textsIndex].setXY(0, textsIndex * textsHeight + textYOffsetInOffset);
    centerTexts[textsIndex].setWidth(textsWidth);
    centerTextContainer.add(centerTexts[textsIndex]);
    centerTextContainer.setHeight((textsIndex + 1) * textsHeight);

    textsIndex++;
}

void WheelSelector::setSelectedItem(int index, int duration, EasingEquation equation)
{
    int centerTextCenter = centerOverlay.getY() + textsHeight / 2;
    int accumHeight = textsHeight * index + textsHeight / 2;
    animateMoveText(calculateSnapPosition(centerTextCenter - accumHeight), duration, EasingEquations::backEaseOut);
}

int WheelSelector::getSelectedItem()
{
    int centerTextCenter = centerOverlay.getY() + textsHeight / 2;
    int foo = centerTextCenter - textContainer.getY();
    return foo / textsHeight;
}

void WheelSelector::reset()
{
    textContainer.setXY(0, 0);
    centerTextContainer.setXY(textContainer.getX(), textContainer.getY() - centerTextContainerYOffset);
    currentAnimationState = NO_ANIMATION;
}

void WheelSelector::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        if (currentAnimationState != NO_ANIMATION)
        {
            textContainer.cancelMoveAnimation();
            centerTextContainer.cancelMoveAnimation();
        }
        xClick = evt.getX();
        yClick = evt.getY();
        currentAnimationState = NO_ANIMATION;
    }
    else if (evt.getType() == ClickEvent::RELEASED)
    {
        if ((currentAnimationState == NO_ANIMATION) ||
                (currentAnimationState == ANIMATING_DRAG && abs(xClick - evt.getX()) < 10 && abs(yClick - evt.getY()) < 10))
        {
            // Click => move to clicked position
            int centerTextCenter = centerOverlay.getY() + textsHeight / 2;
            int newYPosition = calculateSnapPosition(textContainer.getY() - (yClick - centerTextCenter));
            if (newYPosition + getTextContainerVisibleHeight() < centerTextCenter ||
                    newYPosition > centerTextCenter)
            {
                // Avoid reacting on click when already at max position
            }
            else
            {
                int duration = 26;
                animateMoveText(newYPosition, duration, EasingEquations::backEaseOut);

                if (newYPosition == textContainer.getY())
                {
                    // Selected element pressed
                }
            }
        }
        else if (currentAnimationState == ANIMATING_DRAG)
        {
            snap();
        }
    }
}

void WheelSelector::handleDragEvent(const DragEvent& evt)
{
    currentAnimationState = ANIMATING_DRAG;
    moveText(textContainer.getY() + evt.getDeltaY());
}

void WheelSelector::handleGestureEvent(const GestureEvent& evt)
{
    if (evt.getType() == GestureEvent::SWIPE_VERTICAL)
    {
        currentAnimationState = ANIMATING_GESTURE;

        int delta = evt.getVelocity() * 5;
        int newYPosition = textContainer.getY() + delta;
        adjustForBoundaries(newYPosition);

        int distanceToMove = newYPosition - textContainer.getY();
        int distanceToMoveABS = (distanceToMove > 0) ? distanceToMove : -distanceToMove;
        int duration = distanceToMoveABS / 10;
        duration = (duration < 3) ? 3 : duration;

        animateMoveText(calculateSnapPosition(newYPosition), duration, EasingEquations::backEaseOut);
    }
}

void WheelSelector::hideBottomElements(int numberOfElementsToHide)
{
    for (int i = 0; i < textsIndex - numberOfElementsToHide; i++)
    {
        if (texts[i].getAlpha() != 255)
        {
            texts[i].startFadeAnimation(255, 12);
            centerTexts[i].startFadeAnimation(255, 12);
        }
    }

    for (int i = textsIndex - numberOfElementsToHide; i < textsIndex; i++)
    {
        if (texts[i].getAlpha() == 255)
        {
            texts[i].startFadeAnimation(0, 12);
            centerTexts[i].startFadeAnimation(0, 12);
        }
    }

    hiddenElements = numberOfElementsToHide;
}

void WheelSelector::animationEndedHandler(const MoveAnimator<Container>& source)
{
    currentAnimationState = NO_ANIMATION;

    if (elementSelectedAction != 0 && elementSelectedAction->isValid())
    {
        elementSelectedAction->execute(*this, getSelectedItem());
    }
}

void WheelSelector::moveText(int newYPosition)
{
    adjustForBoundaries(newYPosition);

    textContainer.moveTo(textContainer.getX(), newYPosition);
    centerTextContainer.moveTo(centerTextContainer.getX(), newYPosition - centerTextContainerYOffset);
}

void WheelSelector::adjustForBoundaries(int& newYPosition)
{
    int cutoff = 20;

    if (newYPosition > centerOverlay.getY() + cutoff)
    {
        newYPosition = centerOverlay.getY() + cutoff;
    }
    if (newYPosition < centerOverlay.getY() - (getTextContainerVisibleHeight() + cutoff))
    {
        newYPosition = centerOverlay.getY() - (getTextContainerVisibleHeight() + cutoff);
    }
}

void WheelSelector::animateMoveText(int newYPosition, int duration, EasingEquation equation)
{
    textContainer.startMoveAnimation(textContainer.getX(), newYPosition, duration, equation, equation);
    centerTextContainer.startMoveAnimation(centerTextContainer.getX(), newYPosition - centerTextContainerYOffset, duration, equation, equation);
}

int WheelSelector::calculateSnapPosition(int newYPosition)
{
    int centerTextCenter = centerOverlay.getY() + textsHeight / 2;
    int index = (centerTextCenter - newYPosition) / textsHeight;
    return centerTextCenter - (textsHeight * index + textsHeight / 2);
}

void WheelSelector::snap()
{
    int duration = 24;

    animateMoveText(calculateSnapPosition(textContainer.getY()), duration, EasingEquations::backEaseInOut);
}

int WheelSelector::getTextContainerVisibleHeight()
{
    return (textsHeight * (textsIndex - hiddenElements - 1));
}
