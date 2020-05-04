#include <gui/graphics_effect_screen/graphics_effectView.hpp>
#include <touchgfx/EasingEquations.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Math3D.hpp>

graphics_effectView::graphics_effectView() :
    xAngle3D(0.0f),
    yAngle3D(0.0f),
    zAngle3D(0.0f),
    deltaXangle3D(0.033f),
    deltaYangle3D(0.039f),
    deltaZangle3D(0.029f),
    zAngle2D(0.0f),
    deltaZangle2D(0.019f)
{

}

void graphics_effectView::setupScreen()
{
    graphics_effectViewBase::setupScreen();

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
    slider1ValueUpdated(slider1.getValue());
    slider2ValueUpdated(slider2.getValue());
}

void graphics_effectView::tearDownScreen()
{
    graphics_effectViewBase::tearDownScreen();
}

void graphics_effectView::handleTickEvent()
{
    tickCounter++;

    /*if ((tickCounter % 60) == 0) {
        Unicode::snprintf(fpsInfoBuf, FPS_TITLE_LENGTH, "%d", HAL_fps);
        fpsInfo.invalidate();
    }*/

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

void graphics_effectView::handleGestureEvent(const GestureEvent& evt)
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

void graphics_effectView::slider1ValueUpdated(int value)
{
    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        textureMapperImage3D[i].setAlpha(value);
        textureMapperImage3D[i].invalidate();
    }
}

void graphics_effectView::slider2ValueUpdated(int value)
{
    for (int i = 0; i < NUMBER_OF_TM; i++)
    {
        textureMapperImage3D[i].setScale(value / 100.f);
        textureMapperImage3D[i].invalidate();
    }
}

void graphics_effectView::changeSlider1Value(uint16_t val)
{
    slider1.setValue(val);
}

void graphics_effectView::changeSlider2Value(uint16_t val)
{
    slider2.setValue(val);
}
