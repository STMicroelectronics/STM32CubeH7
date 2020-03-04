#include <gui/controls_screen/ControlsView.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

ControlsView::ControlsView() :
    swipeEndedCallback(this, &ControlsView::swipeEndedHandler)
{
    CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
}

void ControlsView::setupScreen()
{
    updateBrightnessText(75);

    potentiometerScreen.add(potentiometerValue);

    potentiometerValue.setXY(443, 13);

    swipeContainer.setPosition(0, 84, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT - 84);

    // Remove containers from screen (added from the designer)
    // to add them to swipecontainer instead.
    remove(potentiometerScreen);
    remove(brightnessScreen);
    swipeContainer.add(potentiometerScreen);
    swipeContainer.add(brightnessScreen);
    swipeContainer.setDotIndicatorBitmaps(Bitmap(BITMAP_DOT_INDICATOR__ID), Bitmap(BITMAP_DOT_INDICATOR_HIGHLIGHTED_ID));
    swipeContainer.setDotIndicatorXYWithCenteredX(HAL::DISPLAY_WIDTH / 2, 315);
    swipeContainer.setMenuChangeEndedCallback(swipeEndedCallback);
    add(swipeContainer);

    updateHeadline();
}

void ControlsView::tearDownScreen()
{
}

void ControlsView::mcuLoadUpdated(uint8_t newMcuLoad)
{
    mcuLoad.updateProcessorLoad(newMcuLoad);
}

void ControlsView::setPotentiometerValue(int value)
{
    if (value != potentiometerValue.getValue())
    {
        potentiometerValue.setValue(value);
    }
}

void ControlsView::setBrightnessValue(int value)
{
    slider.setValue(value);
}

void ControlsView::screenSaverMinorTick()
{
    application().gotoMainMenuCarouselScreenNoTransition();
}

void ControlsView::updateBrightnessText(int value)
{
    Unicode::snprintf(brightnessTextBuffer, BRIGHTNESSTEXT_SIZE, "%d", value);
    brightnessText.resizeToCurrentText();
    brightnessText.setX((HAL::DISPLAY_WIDTH - brightnessText.getWidth()) / 2);
    brightnessPercentageText.moveTo(brightnessText.getRect().right() + 6, brightnessPercentageText.getY());
    container.invalidate();
}

void ControlsView::swipeEndedHandler(const SwipeContainer& sc)
{
    updateHeadline();
}

void ControlsView::updateHeadline()
{
    if (swipeContainer.getSelectedScreen() == 0)
    {
        headline.setTypedText(TypedText(T_CONTROLS_HEADLINE0));
    }
    else
    {
        headline.setTypedText(TypedText(T_CONTROLS_HEADLINE1));
    }
    headline.invalidate();
}

void ControlsView::newBrightnessValueHandler(int value)
{
    presenter->newBrightnessValue(value);
    updateBrightnessText(value);
}
