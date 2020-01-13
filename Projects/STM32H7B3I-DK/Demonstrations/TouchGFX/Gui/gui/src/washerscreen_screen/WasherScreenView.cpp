#include <gui/washerscreen_screen/WasherScreenView.hpp>

WasherScreenView::WasherScreenView() :
    autoDemoCounter(0)
{

}

void WasherScreenView::setupScreen()
{
    WasherScreenViewBase::setupScreen();
}

void WasherScreenView::tearDownScreen()
{
    WasherScreenViewBase::tearDownScreen();
}

void WasherScreenView::handleClickEvent(const ClickEvent& event)
{
    View::handleClickEvent(event);

    if (washerContainer.isVisible())
    {
        washerContainer.clickEvent(event);
    }
}

void WasherScreenView::handleTickEvent()
{
    washerContainer.tickEvent();
}

uint16_t WasherScreenView::getSelectedProgram()
{
    return washerContainer.getWashingProgram();
}

uint16_t WasherScreenView::getWashingTime()
{
    return washerContainer.getWashingTime();
}

void WasherScreenView::setSelectedProgram(uint16_t program)
{
    washerContainer.setWashingProgram(program);
}

void WasherScreenView::mcuLoadUpdated(uint8_t mcuLoad)
{
    washerContainer.updateMcuLoad(mcuLoad);
}

void WasherScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::WasherBathroom:
        switch (autoDemoCounter)
        {
        case 10:
            screenClick(ClickEvent::PRESSED, 452, 130);
            break;

        case 12:
            screenClick(ClickEvent::RELEASED, 452, 130);
            break;

        case 20:
            screenClick(ClickEvent::PRESSED, 452, 130);
            break;

        case 22:
            screenClick(ClickEvent::RELEASED, 452, 130);
            break;

        case 30:
            screenClick(ClickEvent::PRESSED, 452, 130);
            break;

        case 32:
            screenClick(ClickEvent::RELEASED, 452, 130);
            break;

        case 40:
            screenClick(ClickEvent::PRESSED, 20, 130);
            break;

        case 42:
            screenClick(ClickEvent::RELEASED, 20, 130);
            break;

        case 50:
            screenClick(ClickEvent::PRESSED, 240, 140);
            break;

        case 52:
            screenClick(ClickEvent::RELEASED, 240, 140);
            break;

        case 180:
            screenClick(ClickEvent::PRESSED, 458, 25);
            break;

        case 182:
            screenClick(ClickEvent::RELEASED, 458, 25);
            break;

        case 400:
            screenClick(ClickEvent::PRESSED, 25, 25);
            break;

        case 402:
            screenClick(ClickEvent::RELEASED, 25, 25);
            break;

        case 420:
            screenClick(ClickEvent::PRESSED, 25, 25);
            break;

        case 422:
            presenter->setAutoDemoState(Model::BathroomBathtub);
            screenClick(ClickEvent::RELEASED, 25, 25);
            break;
        }
        break;

    default:
        break;

    }
    autoDemoCounter++;
}

void WasherScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    WasherScreenView::handleClickEvent(evt);
}
