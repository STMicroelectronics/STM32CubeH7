#include <gui/recipeselectedscreen_screen/RecipeSelectedScreenView.hpp>

RecipeSelectedScreenView::RecipeSelectedScreenView() : 
    autoDemoCounter(0),
    autoDemoScroll(false)
{

}

void RecipeSelectedScreenView::setupScreen()
{
    RecipeSelectedScreenViewBase::setupScreen();

    HAL::getInstance()->setFrameRateCompensation(false);

    fps_mcu.initialize();
    fps_mcu.setXY(480 - fps_mcu.getWidth() - 3, 272 - fps_mcu.getHeight());
    fps_mcu.setVisible(false);
    scrollableContainerRecipe.add(fps_mcu);
}

void RecipeSelectedScreenView::tearDownScreen()
{
    RecipeSelectedScreenViewBase::tearDownScreen();
}

void RecipeSelectedScreenView::handleTickEvent()
{
    if (autoDemoScroll && ((imageDish.getY() + scrollSpeed) <= 0))
    {
        scrollableContainerRecipe.moveChildrenRelative(0, scrollSpeed);
    }
}

void RecipeSelectedScreenView::mcuLoadUpdated(uint8_t mcuLoad)
{
    fps_mcu.setMcuLoad(mcuLoad);
}

void RecipeSelectedScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (autoDemoScroll)
    {
        autoDemoScroll = false;
    }
    RecipeSelectedScreenViewBase::handleClickEvent(evt);
}

void RecipeSelectedScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::RecipeRecipeSelector:
        switch (autoDemoCounter)
        {
        case 8:
            screenClick(ClickEvent::PRESSED, 240, 130);
            break;

        case 10:
            screenClick(ClickEvent::RELEASED, 240, 130);
            break;

        case 260:
            scrollSpeed = -2;
            scrollableContainerRecipe.moveChildrenRelative(0, scrollSpeed);
            autoDemoScroll = true;
            break;

        case 300:
            autoDemoScroll = false;
            break;

        case 304:
            scrollSpeed = 4;
            autoDemoScroll = true;
            break;

        case 324:
            autoDemoScroll = false;
            break;

        case 330:
            screenClick(ClickEvent::PRESSED, 10, 10);
            break;

        case 332:
            presenter->setAutoDemoState(Model::RecipeSelectorHomeUI);
            screenClick(ClickEvent::RELEASED, 10, 10);
            break;        
        }
        
    default:
        break;
    }
    autoDemoCounter++;
}

void RecipeSelectedScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    RecipeSelectedScreenView::handleClickEvent(evt);
}

void RecipeSelectedScreenView::screenDrag(DragEvent::DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY)
{
    DragEvent evt(type, oldX, oldY, newX, newY);
    RecipeSelectedScreenView::handleDragEvent(evt);
}

void RecipeSelectedScreenView::screenGesture(GestureEvent::GestureType type, int16_t v, int16_t x_coord, int16_t y_coord)
{
    GestureEvent evt(type, v, x_coord, y_coord);
    RecipeSelectedScreenView::handleGestureEvent(evt);
}
