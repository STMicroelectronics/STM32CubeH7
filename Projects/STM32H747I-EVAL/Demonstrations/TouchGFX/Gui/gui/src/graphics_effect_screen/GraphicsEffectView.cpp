#include <gui/graphics_effect_screen/GraphicsEffectView.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Math3D.hpp>
#include <BitmapDatabase.hpp>

GraphicsEffectView::GraphicsEffectView() :
    DemoView(),
    sliderValueChangedCallback(this, &GraphicsEffectView::sliderValueChangedCallbackHandler),
    xAngle3D(0.0f),
    yAngle3D(0.0f),
    zAngle3D(0.0f),
    deltaXangle3D(0.033f),
    deltaYangle3D(0.039f),
    deltaZangle3D(0.029f),
    zAngle2D(0.0f),
    deltaZangle2D(0.019f)
{
    HAL::getInstance()->setFrameRateCompensation(true);
    touchgfx_printf("GraphicsEffectView()\n");

    background.setXY(0, 0);
    background.setBitmap(Bitmap(BITMAP_ANIMATING_BUTTONS_BACKGROUND_ID));

    slider1.setBitmaps(Bitmap(BITMAP_BLUE_SLIDER_VERTICAL_SMALL_SLIDER3_VERTICAL_ROUND_BACK_ID), Bitmap(BITMAP_BLUE_SLIDER_VERTICAL_SMALL_SLIDER3_VERTICAL_ROUND_FILL_ID), Bitmap(BITMAP_BLUE_SLIDER_VERTICAL_SMALL_INDICATORS_SLIDER3_VERTICAL_ROUND_NOB_ID));
    slider1.setXY(24, 480 - 47 - 172);
    slider1.setupVerticalSlider(7, 3, 0, 0, 125);
    slider1.setValueRange(32, 255);
    slider1.setValue(192);
    slider1.setNewValueCallback(sliderValueChangedCallback);

    slider2.setBitmaps(Bitmap(BITMAP_BLUE_SLIDER_VERTICAL_SMALL_SLIDER3_VERTICAL_ROUND_BACK_ID), Bitmap(BITMAP_BLUE_SLIDER_VERTICAL_SMALL_SLIDER3_VERTICAL_ROUND_FILL_ID), Bitmap(BITMAP_BLUE_SLIDER_VERTICAL_SMALL_INDICATORS_SLIDER3_VERTICAL_ROUND_NOB_ID));
    slider2.setXY(800 - 24 - 34, 480 - 47 - 172);
    slider2.setupVerticalSlider(7, 3, 0, 0, 125);
    slider2.setValueRange(20, 90);
    slider2.setValue(80);
    slider2.setNewValueCallback(sliderValueChangedCallback);

    add(background);
    add(slider1);
    add(slider2);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_MENU_ICON_ID), Bitmap(BITMAP_MENU_ICON_PRESSED_ID));
    gotoMenuButton.setXY(21, 14);
    add(gotoMenuButton);

    fpsInfoBuf[0] = 0;

    fpsInfo.setPosition(800 - 100, 20, 100, 70 - 20);
    fpsInfo.setWildcard(fpsInfoBuf);
    fpsInfo.setTypedText(TypedText(T_MOVIE_LOAD_HEADLINE));
    fpsInfo.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(fpsInfo);
}

void GraphicsEffectView::sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value)
{
    if (&src == &slider1)
    {
        sliderValueUpdated(value);
    }
    else if (&src == &slider2)
    {
        slider2ValueUpdated(value);
    }
}

void GraphicsEffectView::setupScreen()
{
    tickCounter = 0;
    gestureYTick = 0;
    gestureZTick = 0;

    // Setup the texture mapper image that does 3D rotation
    textureMapperImage3D[0].setBitmap(Bitmap(BITMAP_BOX00_ID));
    textureMapperImage3D[1].setBitmap(Bitmap(BITMAP_BOX01_ID));
    textureMapperImage3D[2].setBitmap(Bitmap(BITMAP_BOX00_ID));
    textureMapperImage3D[3].setBitmap(Bitmap(BITMAP_BOX01_ID));

    cameraYDelta = -180;

    int imageWidth = textureMapperImage3D[0].getBitmap().getWidth();
    int imageHeight = textureMapperImage3D[0].getBitmap().getHeight();

    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        textureMapperImage3D[i].setXY(0, 0);
        textureMapperImage3D[i].setWidth(HAL::DISPLAY_WIDTH);
        textureMapperImage3D[i].setHeight(HAL::DISPLAY_HEIGHT);
        textureMapperImage3D[i].setBitmapPosition((HAL::DISPLAY_WIDTH - imageWidth) / 2, (HAL::DISPLAY_HEIGHT - imageHeight) / 2);
        textureMapperImage3D[i].setCameraDistance(1000.0f);
        textureMapperImage3D[i].setOrigo(textureMapperImage3D[i].getBitmapPositionX() + (imageWidth / 2), textureMapperImage3D[i].getBitmapPositionY() + (imageHeight / 2), 1000.f + imageWidth / 2);
        textureMapperImage3D[i].setCamera(textureMapperImage3D[i].getBitmapPositionX() + (imageWidth / 2), textureMapperImage3D[i].getBitmapPositionY() + cameraYDelta);
        textureMapperImage3D[i].setRenderingAlgorithm(TextureMapper::NEAREST_NEIGHBOR);
        textureMapperImage3D[i].updateAngles(xAngle3D, yAngle3D, zAngle3D);

        add(textureMapperImage3D[i]);
    }
    sliderValueUpdated(slider1.getValue());
    slider2ValueUpdated(slider2.getValue());
}

void GraphicsEffectView::tearDownScreen()
{

}

#ifdef SIMULATOR
static int HAL_fps = 0;
#else
extern "C" int HAL_fps;
#endif

void GraphicsEffectView::handleTickEvent()
{
    tickCounter++;

    if ((tickCounter % 60) == 0) {
        Unicode::snprintf(fpsInfoBuf, FPS_TITLE_LENGTH, "%d", HAL_fps);
        fpsInfo.invalidate();
    }

    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        remove(textureMapperImage3D[i]);
    }

    bool inserted[NUMBER_OF_TM];
    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        inserted[i] = false;
    }

    int index = -1;

    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        float biggest = -10000.0f;
        for (int j = 0; j < NUMBER_OF_TM; j++)
        {
            float zvalue = MAX(MAX(textureMapperImage3D[j].getZ0(), textureMapperImage3D[j].getZ1()), MAX(textureMapperImage3D[j].getZ2(), textureMapperImage3D[j].getZ3()));
            if (!inserted[j] && zvalue > biggest)
            {
                biggest = zvalue;
                index = j;
            }
        }
        add(textureMapperImage3D[index]);
        inserted[index] = true;
    }

    float adjGestyreY = 0.0f;

    if (gestureYTick > 0)
    {
        adjGestyreY = ((400.f - EasingEquations::cubicEaseOut(tickCounter - gestureYTick, 0, 400, 50)) / 1000.0f) * gestureY;

        if (tickCounter - gestureYTick >= 50)
        {
            gestureYTick = 0;
        }
    }
    else
    {
        gestureY = 0.f;
    }

    yAngle3D += deltaYangle3D;
    zAngle3D += deltaZangle3D + adjGestyreY;
    zAngle2D += deltaZangle2D;

    textureMapperImage3D[0].updateAngles(xAngle3D, yAngle3D, zAngle3D);
    textureMapperImage3D[1].updateAngles(xAngle3D, yAngle3D + PI / 2.f, zAngle3D);
    textureMapperImage3D[2].updateAngles(xAngle3D, yAngle3D + PI, zAngle3D);
    textureMapperImage3D[3].updateAngles(xAngle3D, yAngle3D + PI * 3 / 2.f, zAngle3D);
}

void GraphicsEffectView::handleGestureEvent(const GestureEvent& evt)
{
    if (evt.getType() == GestureEvent::SWIPE_HORIZONTAL)
    {
        if (evt.getVelocity() > 0)
        {
            if (evt.getY() < (textureMapperImage3D[0].getX() + textureMapperImage3D[0].getHeight() / 2))
            {
                gestureY = 1.0f;
            }
            else
            {
                gestureY = -1.0f;
            }
        }
        else
        {
            if (evt.getY() < (textureMapperImage3D[0].getX() + textureMapperImage3D[0].getHeight() / 2))
            {
                gestureY = -1.0f;
            }
            else
            {
                gestureY = 1.0f;
            }
        }
        gestureYTick = tickCounter;
    }
}

void GraphicsEffectView::sliderValueUpdated(int value)
{
    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        textureMapperImage3D[i].setAlpha(value);
        textureMapperImage3D[i].invalidate();
    }
}

void GraphicsEffectView::slider2ValueUpdated(int value)
{
    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        textureMapperImage3D[i].setScale(value / 100.f);
        textureMapperImage3D[i].invalidate();
    }
}

void GraphicsEffectView::changeSlider1Value(uint16_t val)
{
    slider1.setValue(val);
}

void GraphicsEffectView::changeSlider2Value(uint16_t val)
{
    slider2.setValue(val);
}

void GraphicsEffectView::screenClick(ClickEvent::ClickEventType type, int x, int y)
{
    ClickEvent temp_evt(type, x, y);
    View::handleClickEvent(temp_evt);
}
