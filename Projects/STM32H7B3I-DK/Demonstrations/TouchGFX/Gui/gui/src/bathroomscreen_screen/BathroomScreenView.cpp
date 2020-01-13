#include <gui/bathroomscreen_screen/BathroomScreenView.hpp>
#include "BitmapDatabase.hpp"
#include <gui/common/Defines.hpp>

BathroomScreenView::BathroomScreenView() :
    fadeButtonCallback(this, &BathroomScreenView::fadebuttonHandler),
    clickListnerImageCallback(this, &BathroomScreenView::clickListnerImageHandler),
    fadeCounter(0),
    autoDemoCounter(0)
{

}

void BathroomScreenView::setupScreen()
{
    BathroomScreenViewBase::setupScreen();

    buttonBathtub.setFadeAnimationEndedAction(fadeButtonCallback);
    background.setClickAction(clickListnerImageCallback);

    mcuLoad.initialize();
    mcuLoad.setMCUOnly(true);
    mcuLoad.setTextColor(Color::getColorFrom24BitRGB(85, 85, 85));
}

void BathroomScreenView::tearDownScreen()
{
    BathroomScreenViewBase::tearDownScreen();
}

void BathroomScreenView::setBathtubWaterState(bool on)
{
    imageBathtubWater.setVisible(on);
}

void BathroomScreenView::setWasherState(bool on)
{
    imageWasherActive.setVisible(on);
    imageWasherUI.setVisible(on);
}

void BathroomScreenView::setBathtubUIState(bool on)
{
    imageBathtubUI.setVisible(on);
}

void BathroomScreenView::setHomeUIState(bool on)
{
    imageHomeUI.setVisible(on);
}

void BathroomScreenView::startGlowAnimaton()
{
    buttonWasher.startFadeAnimation(255, FADE_TICKS);
    buttonBathtub.startFadeAnimation(255, FADE_TICKS);
    buttonHomeControl.startFadeAnimation(255, FADE_TICKS);
}

void BathroomScreenView::fadebuttonHandler(const FadeAnimator<Button>& src)
{
    if (fadeCounter < FADE_COUNT)
    {
        if (buttonBathtub.getAlpha() == 255)
        {
            buttonBathtub.startFadeAnimation(0, FADE_TICKS);
            buttonWasher.startFadeAnimation(0, FADE_TICKS);
            buttonHomeControl.startFadeAnimation(0, FADE_TICKS);
        }
        else
        {
            buttonBathtub.startFadeAnimation(255, FADE_TICKS);
            buttonWasher.startFadeAnimation(255, FADE_TICKS);
            buttonHomeControl.startFadeAnimation(255, FADE_TICKS);
        }
        fadeCounter++;
    }
}

void BathroomScreenView::clickListnerImageHandler(const Image& img, const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Starts the glow animation if it is not already running.
        if (!buttonBathtub.isFadeAnimationRunning())
        {           
            startGlowAnimaton();
        }
        fadeCounter = 0;
    }
}

void BathroomScreenView::updateMCULoad(uint8_t load)
{
    mcuLoad.setMcuLoad(load);
}

void BathroomScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::BathroomWasher:
        switch (autoDemoCounter)
        {
        case 60:
            screenClick(ClickEvent::PRESSED, 430, 80);
            break;

        case 64:
            screenClick(ClickEvent::RELEASED, 430, 80);
            presenter->setAutoDemoState(Model::WasherBathroom);
            break;
        }
        break;

    case Model::BathroomBathtub:
        switch (autoDemoCounter)
        {
        case 20:
            screenClick(ClickEvent::PRESSED, 200, 200);
            break;

        case 24:
            screenClick(ClickEvent::RELEASED, 200, 200);
            presenter->setAutoDemoState(Model::BathtubBathroom);
            break;
        }
        break;

    case Model::BathroomHomeUI:
        switch (autoDemoCounter)
        {
        case 20:
            screenClick(ClickEvent::PRESSED, 60, 87);
            break;

        case 24:
            screenClick(ClickEvent::RELEASED, 60, 87);
            presenter->setAutoDemoState(Model::HomeUIBathroom);
            break;
        }
        break;

    case Model::BathroomHallway:
        switch (autoDemoCounter)
        {
        case 10:
            screenClick(ClickEvent::PRESSED, 20, 20);
            break;

        case 12:
            presenter->setAutoDemoState(Model::HallwayAbout);
            screenClick(ClickEvent::RELEASED, 20, 20);
            break;
        }
        break;

    default:
        break;
    }
    autoDemoCounter++;
}

void BathroomScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    View::handleClickEvent(evt);
}
