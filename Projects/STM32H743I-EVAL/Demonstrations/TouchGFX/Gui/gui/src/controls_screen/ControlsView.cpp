#include <gui/controls_screen/ControlsView.hpp>
#include <touchgfx/Color.hpp>


ControlsView::ControlsView() :
    swipeEndedCallback(this, &ControlsView::swipeEndedHandler),
    newValueCallback(this, &ControlsView::newValueHandler)
{
    CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
}

void ControlsView::setupScreen()
{
    background.setBitmap(Bitmap(BITMAP_CONTROLS_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_MENU_ICON_ID), Bitmap(BITMAP_MENU_ICON_PRESSED_ID));
    gotoMenuButton.setXY(21, 14);
    add(gotoMenuButton);

    mcuLoadTxt.setXY(620 - 160 + 32, 32);
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right()-6, mcuLoadTxt.getY(), 50, 30);
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    mcuLoadValueTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    headline.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    headline.setPosition(160, 20, 320, 30);
    add(headline);

    dots.setBitmap(Bitmap(BITMAP_CONTROLS_2DOTS_ID));
    dots.setXY(332 - 80, 399);
    add(dots);

    int xConvertionOffset = 134; // Convert layout from 800x480 demo to 640x480
    board.setBitmap(Bitmap(BITMAP_BOARD_IMAGE_ID));
    board.setXY(-xConvertionOffset, 34);

    potentiometerValue.setXY(443 - xConvertionOffset, 13);

    line.setBitmap(Bitmap(BITMAP_CONTROL_LINE_INDICATOR_ID));
    line.setXY(303 - xConvertionOffset, 134);

    brightnessSmall.setBitmap(Bitmap(BITMAP_ICON_BRIGHTNESS_SMALL_ID));
    brightnessSmall.setXY(55, 71);
    brightnessScreen.add(brightnessSmall);

    brightnessLarge.setBitmap(Bitmap(BITMAP_ICON_BRIGHTNESS_LARGE_ID));
    brightnessLarge.setXY(667 - 160, 55);
    brightnessScreen.add(brightnessLarge);

    slider.setBitmaps(Bitmap(BITMAP_SLIDER_ID), Bitmap(BITMAP_SLIDER_FULL_ID), Bitmap(BITMAP_SLIDER_KNOP_ID));
    slider.setXY(17, 172);
    slider.setupHorizontalSlider(13, 24, 0, 0, 646 - 160);
    slider.setValueRange(0, 100);
    slider.setNewValueCallback(newValueCallback);
    brightnessScreen.add(slider);

    brightnessText.setWildcard(brightnessTextBuffer);
    brightnessText.setTypedText(TypedText(T_CONTROLS_BRIGHTNESS_READOUT));
    brightnessText.setXY((HAL::DISPLAY_WIDTH - 200) / 2, 26);
    brightnessText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    brightnessScreen.add(brightnessText);
    updateBrightnessText(75);

    brightnessPercentageText.setTypedText(TypedText(T_CONTROLS_BRIGHTNESS_PERCENTAGE));
    brightnessPercentageText.moveTo(brightnessText.getRect().right() + 6, brightnessText.getY() + 50);
    brightnessPercentageText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    brightnessScreen.add(brightnessPercentageText);

    swipeContainer.setPosition(0, 84, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT - 84);
    potentiometerScreen.setWidth(swipeContainer.getWidth());
    potentiometerScreen.setHeight(swipeContainer.getHeight());
    brightnessScreen.setWidth(swipeContainer.getWidth());
    brightnessScreen.setHeight(swipeContainer.getHeight());
    potentiometerScreen.add(board);
    potentiometerScreen.add(potentiometerValue);
    potentiometerScreen.add(line);
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

void ControlsView::updateBrightnessText(int value)
{
    Unicode::snprintf(brightnessTextBuffer, 5, "%d", value);
    brightnessText.resizeToCurrentText();
    brightnessText.setX((HAL::DISPLAY_WIDTH - brightnessText.getWidth()) / 2);
    brightnessPercentageText.moveTo(brightnessText.getRect().right() + 6, brightnessPercentageText.getY());
    container.invalidate();
}

void ControlsView::newValueHandler(const Slider& slider, int value)
{
    presenter->newBrightnessValue(value);
    updateBrightnessText(value);
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

void ControlsView::screenClick(ClickEvent::ClickEventType type, int x, int y)
{
    ClickEvent temp_evt(type, x, y);
    View::handleClickEvent(temp_evt);
}

void ControlsView::screenDrag(int16_t x_old, int16_t y_old, int16_t x_new, int16_t y_new)
{
    DragEvent temp_evt(DragEvent::DRAGGED, x_old, y_old, x_new, y_new);
    View::handleDragEvent(temp_evt);
}
