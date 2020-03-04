#include <gui/bump_map_screen/BumpMapView.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>
#include <gui/bump_map_screen/BumpMap.hpp>
#include <touchgfx/EasingEquations.hpp>

#ifndef SIMULATOR
#include <stm32h7xx_hal.h>
#endif

BumpMapView::BumpMapView() :
    currentState(NO_ANIMATION),
    animationCounter(0),
    currentBumpMap(0),
    bumpMapImage(BITMAP_TOUCHGFX_LIGHT_EFFECT_ID, _bump_map_touchgfx_logo, _light_source),
    onButtonPressed(this, &BumpMapView::buttonPressedhandler)
{
    bumpMapInfo[0].image = BITMAP_TOUCHGFX_LIGHT_EFFECT_ID;
    bumpMapInfo[0].bump_map = _bump_map_touchgfx_logo;
    bumpMapInfo[1].image = BITMAP_NEW_LIGHT_EFFECT_IMAGE_ID;
    bumpMapInfo[1].bump_map = _bump_map_touchgfx_logo_2;
    updateBumpMapImage();
}

BumpMapView::~BumpMapView()
{
}

void BumpMapView::setupScreen()
{
    shade.setColor(0x0);
    shade.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    currentState = ANIMATE_SHADE_UP;

    bumpMapImage.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    bumpMapImage.setVisible(true);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_MENU_ICON_ID), Bitmap(BITMAP_MENU_ICON_PRESSED_ID));
    gotoMenuButton.setXY(16, 16);

    nextButton.setBitmaps(Bitmap(BITMAP_BUMP_MAP_NEXT_BUTTON_ID), Bitmap(BITMAP_BUMP_MAP_NEXT_BUTTON_PRESSED_ID));
    nextButton.setXY(HAL::DISPLAY_WIDTH - nextButton.getWidth() - 16, HAL::DISPLAY_HEIGHT - nextButton.getHeight() - 16);
    nextButton.setAction(onButtonPressed);
    nextButton.setTouchable(false);

    add(bumpMapImage);
    add(shade);
    add(nextButton);
    add(gotoMenuButton);
}

void BumpMapView::tearDownScreen()
{
  
}

void BumpMapView::handleTickEvent()
{
    if (currentState == ANIMATE_TO_NEXT_IMAGE)
    {
        animateToNextImage();
    }
    else if (currentState == ANIMATE_SHADE_UP)
    {
        animateShadeUp();
    }
}

void BumpMapView::handleDragEvent(const DragEvent& evt)
{
    bumpMapImage.setLightPosition(evt.getNewX(), evt.getNewY());
}

void BumpMapView::buttonPressedhandler(const AbstractButton& button)
{
    if (&button == &nextButton)
    {
        currentState = ANIMATE_TO_NEXT_IMAGE;
        animationCounter = 0;
        nextButton.setTouchable(false);
    }
}

void BumpMapView::updateBumpMapImage()
{
    bumpMapImage.setImage(Bitmap(bumpMapInfo[currentBumpMap].image));
    bumpMapImage.setBumpMap(bumpMapInfo[currentBumpMap].bump_map);
    bumpMapImage.invalidate();
}

void BumpMapView::animateToNextImage()
{
    uint8_t animateShadeDownDuration = 18;
    uint8_t changeImageDuration = 1;

    if (animationCounter <= animateShadeDownDuration)
    {
        int16_t deltaBackground = EasingEquations::quadEaseOut(animationCounter, 0, HAL::DISPLAY_HEIGHT, animateShadeDownDuration);
        shade.moveTo(0, -HAL::DISPLAY_HEIGHT + deltaBackground);

    }
    else if (animationCounter == animateShadeDownDuration + changeImageDuration)
    {
        currentBumpMap++;
        if (currentBumpMap == NUMBER_OF_BUMP_MAPS)
        {
            currentBumpMap = 0;
        }
        updateBumpMapImage();
    }
    else
    {
        animationCounter = 0;
        currentState = ANIMATE_SHADE_UP;
    }

    animationCounter++;
}

void BumpMapView::animateShadeUp()
{
    uint8_t animateShadeUpDuration = 18;

    if (animationCounter <= animateShadeUpDuration)
    {
        int16_t deltaBackground = EasingEquations::quadEaseIn(animationCounter, 0, HAL::DISPLAY_HEIGHT, animateShadeUpDuration);
        shade.moveTo(0, -deltaBackground);
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        nextButton.setTouchable(true);
    }

    animationCounter++;
}


void BumpMapView::screenSaverMinorTick()
{

}

void BumpMapView::screenClick(ClickEvent::ClickEventType type, int x, int y)
{
    ClickEvent temp_evt(type, x, y);
    View::handleClickEvent(temp_evt);
}
