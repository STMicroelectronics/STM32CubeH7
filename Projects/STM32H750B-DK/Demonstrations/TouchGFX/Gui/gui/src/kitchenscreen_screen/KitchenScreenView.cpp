#include <gui/kitchenscreen_screen/KitchenScreenView.hpp>
#include <gui/common/Defines.hpp>
#include <touchgfx/Color.hpp>

KitchenScreenView::KitchenScreenView() :
    fadeButtonCallback(this, &KitchenScreenView::fadebuttonHandler),
    clickListnerImageCallback(this, &KitchenScreenView::clickListnerImageHandler),
    fadeCounter(0),
    autoDemoCounter(0)
{

}

void KitchenScreenView::setupScreen()
{
    KitchenScreenViewBase::setupScreen();
    buttonExtractorHood.setFadeAnimationEndedAction(fadeButtonCallback);
    background.setClickAction(clickListnerImageCallback);

    mcuLoad.initialize();
    mcuLoad.setMCUOnly(true);
    mcuLoad.setTextColor(Color::getColorFrom24BitRGB(101, 85, 95));
}

void KitchenScreenView::tearDownScreen()
{
    KitchenScreenViewBase::tearDownScreen();
}

void KitchenScreenView::setLightState(bool on)
{
    imageLight.setVisible(on);
}

void KitchenScreenView::setCookpotState(bool on)
{
    imageCookpot.setVisible(on);
}

void KitchenScreenView::setOvenState(bool on)
{
    imageOvenOn.setVisible(on);
}

void KitchenScreenView::setOvenUIState(bool on)
{
    imageOvenUI.setVisible(on);
}

void KitchenScreenView::setHoodUIState(bool on)
{
    imageHoodUI.setVisible(on);
}

void KitchenScreenView::setHomeUIState(bool on)
{
    imageHomeUI.setVisible(on);
}

void KitchenScreenView::startGlowAnimaton()
{
    buttonExtractorHood.startFadeAnimation(255, FADE_TICKS);
    buttonOven.startFadeAnimation(255, FADE_TICKS);
    buttonThermo.startFadeAnimation(255, FADE_TICKS);
}

void KitchenScreenView::fadebuttonHandler(const FadeAnimator<Button>& src)
{
    if (fadeCounter < FADE_COUNT)
    {
        if (buttonExtractorHood.getAlpha() == 255)
        {
            buttonExtractorHood.startFadeAnimation(0, FADE_TICKS);
            buttonOven.startFadeAnimation(0, FADE_TICKS);
            buttonThermo.startFadeAnimation(0, FADE_TICKS);
        }
        else
        {
            buttonExtractorHood.startFadeAnimation(255, FADE_TICKS);
            buttonOven.startFadeAnimation(255, FADE_TICKS);
            buttonThermo.startFadeAnimation(255, FADE_TICKS);
        }
        fadeCounter++;
    }
}

void KitchenScreenView::clickListnerImageHandler(const Image& img, const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Starts the glow animation if it is not already running.
        if (!buttonExtractorHood.isFadeAnimationRunning())
        {            
            startGlowAnimaton();
        }
        fadeCounter = 0;
    }
}

void KitchenScreenView::updateMCULoad(uint8_t load)
{
    mcuLoad.setMcuLoad(load);
}

void KitchenScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::KitchenExtractor:
        switch (autoDemoCounter)
        {
        case 60:
            screenClick(ClickEvent::PRESSED, 185, 25);
            break;

        case 64:
            screenClick(ClickEvent::RELEASED, 185, 25);
            presenter->setAutoDemoState(Model::ExtratorKitchen);
            break;
        }
        break;

    case Model::KitchenOven:
        switch (autoDemoCounter)
        {
        case 20:
            screenClick(ClickEvent::PRESSED, 400, 200);
            break;

        case 24:
            screenClick(ClickEvent::RELEASED, 400, 200);
            presenter->setAutoDemoState(Model::OvenKitchen);
            break;
        }
        break;

    case Model::KitchenHomeUI:
        switch (autoDemoCounter)
        {
        case 20:
            screenClick(ClickEvent::PRESSED, 61, 114);
            break;

        case 24:
            screenClick(ClickEvent::RELEASED, 61, 114);
            presenter->setAutoDemoState(Model::HomeUIRecipeSelector);
            break;
        }
        break;

    case Model::KitchenHallway:
        switch (autoDemoCounter)
        {
        case 10:
            screenClick(ClickEvent::PRESSED, 10, 10);
            break;

        case 12:
            presenter->setAutoDemoState(Model::HallwayBathroom);
            screenClick(ClickEvent::RELEASED, 10, 10);
            break;
        }
        break;

    default:
        break;
    }
    autoDemoCounter++;
}

void KitchenScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    View::handleClickEvent(evt);
}
