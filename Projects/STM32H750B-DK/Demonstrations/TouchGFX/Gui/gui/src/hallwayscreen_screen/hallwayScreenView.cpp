#include <gui/hallwayscreen_screen/hallwayScreenView.hpp>
#include <gui/common/Defines.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

HallwayScreenView::HallwayScreenView() :
    circleClickedCallback(this, &HallwayScreenView::circleClickedHandler),
    buttonMoveCallback(this, &HallwayScreenView::buttonMoveHandler),
    buttonFadeCallback(this, &HallwayScreenView::buttonFadeHandler),
    cloudMoveCallback(this, &HallwayScreenView::cloudMoveHandler),
    btnCanceld(false),
    autoDemoCounter(0)
{
}

void HallwayScreenView::setupScreen()
{
    HallwayScreenViewBase::setupScreen();

    circlePressed.setClickAction(circleClickedCallback);
    buttonKitchen.setMoveAnimationEndedAction(buttonMoveCallback);
    buttonKitchen.setFadeAnimationEndedAction(buttonFadeCallback);

    cloudContainer.setX(CLOUDS_START_X);
    cloudContainer.setMoveAnimationEndedAction(cloudMoveCallback);    

    mcu.initialize();
    mcu.setMCUOnly(true);
    mcu.setTextColor(Color::getColorFrom24BitRGB(85, 85, 85));
}

void HallwayScreenView::tearDownScreen()
{
    HallwayScreenViewBase::tearDownScreen();
}

void HallwayScreenView::setIntroState(bool show)
{
    if (show)
    {
        demoIntro.setVisible(true);
    }
    else
    {
        demoIntro.setVisible(false);
        demoIntro.setTouchable(false);

        buttonBathrrom.setVisible(true);
        buttonKitchen.setVisible(true);
        buttonBathrrom.startFadeAnimation(255, HALLWAY_FADE_DURATION);
        buttonKitchen.startFadeAnimation(255, HALLWAY_FADE_DURATION);

        cloudContainer.startMoveAnimation(0, cloudContainer.getY(), CLOUDS_MOVE_SPEED);
    }
}

void HallwayScreenView::circleClickedHandler(const FadeAnimator<Circle>& c, const ClickEvent& evt)
{
	if (demoIntro.getAlpha() == 255 && demoIntro.isVisible())
    {
        if (evt.getType() == ClickEvent::PRESSED)
        {
            circlePressed.setAlpha(155);
            circlePressed.invalidate();
        }
        else if (evt.getType() == ClickEvent::CANCEL)
        {
            circlePressed.setAlpha(0);
            circlePressed.invalidate();
            btnCanceld = true;
        }
        else if (evt.getType() == ClickEvent::RELEASED && !btnCanceld)
        {
            circlePressed.setAlpha(0);
            circlePressed.invalidate();
            circlePressed.startFadeAnimation(0, HALLWAY_FADE_DURATION);
            demoIntro.startFadeAnimation(0, HALLWAY_FADE_DURATION);
            demoIntro.setTouchable(false);
            presenter->setShowIntro(false);

            buttonBathrrom.setVisible(true);
            buttonKitchen.setVisible(true);

            buttonBathrrom.setFadeAnimationDelay(HALLWAY_FADE_DURATION / 2);
            buttonKitchen.setFadeAnimationDelay(HALLWAY_FADE_DURATION / 2);

            buttonBathrrom.startFadeAnimation(255, HALLWAY_FADE_DURATION);
            buttonKitchen.startFadeAnimation(255, HALLWAY_FADE_DURATION);

        }
        else if (evt.getType() == ClickEvent::RELEASED && btnCanceld)
        {
            btnCanceld = false;
        }
    }
}

void HallwayScreenView::buttonMoveHandler(const MoveAnimator<Button>& src)
{
    if (buttonKitchen.getX() == BUTTON_KITCHEN_X_1)
    {
        buttonKitchen.startMoveAnimation(BUTTON_KITCHEN_X_2, buttonKitchen.getY(), BUTTONS_MOVE_SPEED_LONG);
        buttonBathrrom.startMoveAnimation(BUTTON_BATHROOM_X_2, buttonBathrrom.getY(), BUTTONS_MOVE_SPEED_LONG);
    }
    else if (buttonKitchen.getX() == BUTTON_KITCHEN_X_2)
    {
        buttonKitchen.startMoveAnimation(BUTTON_KITCHEN_X_3, buttonKitchen.getY(), BUTTONS_MOVE_SPEED_SHORT);
        buttonBathrrom.startMoveAnimation(BUTTON_BATHROOM_X_3, buttonBathrrom.getY(), BUTTONS_MOVE_SPEED_SHORT);
    }
}

void HallwayScreenView::buttonFadeHandler(const FadeAnimator< MoveAnimator<Button> >& src)
{
    buttonKitchen.startMoveAnimation(BUTTON_KITCHEN_X_1, buttonKitchen.getY(), BUTTONS_MOVE_SPEED_LONG);
    buttonBathrrom.startMoveAnimation(BUTTON_BATHROOM_X_1, buttonBathrrom.getY(), BUTTONS_MOVE_SPEED_LONG);

    cloudContainer.startMoveAnimation(CLOUDS_END_X, cloudContainer.getY(), CLOUDS_MOVE_SPEED);
}

void HallwayScreenView::cloudMoveHandler(const MoveAnimator<Container>& src)
{
    cloudContainer.setX(CLOUDS_START_X);
    cloudContainer.startMoveAnimation(CLOUDS_END_X, cloudContainer.getY(), CLOUDS_MOVE_SPEED);
}

void HallwayScreenView::mcuLoadUpdated(uint8_t mcuLoad)
{
    mcu.setMcuLoad(mcuLoad);
}

void HallwayScreenView::autoDemoTick()
{
    switch (presenter->getAutoDemoState())
    {
    case Model::HallwayKitchen:
        switch (autoDemoCounter)
        {           
        case 6:
            screenClick(ClickEvent::PRESSED, 240, 165);
            break;

        case 10:
            screenClick(ClickEvent::RELEASED, 240, 165);
            break;

        case 60:
            screenClick(ClickEvent::PRESSED, 350, 120);
            break;

        case 62:
            presenter->setAutoDemoState(Model::KitchenExtractor);
            screenClick(ClickEvent::RELEASED, 350, 120);
            break;
        }
        break;

    case Model::HallwayBathroom:
        switch (autoDemoCounter)
        {

        case 44:
            screenClick(ClickEvent::PRESSED, 100, 120);
            break;

        case 46:
            presenter->setAutoDemoState(Model::BathroomWasher);
            screenClick(ClickEvent::RELEASED, 100, 120);
            break;
        }
        break;

    case Model::HallwayAbout:
        switch (autoDemoCounter)
        {

        case 44:
            screenClick(ClickEvent::PRESSED, 240, 110);
            break;

        case 46:
            presenter->setAutoDemoState(Model::AboutHallway);
            screenClick(ClickEvent::RELEASED, 240, 110);
            break;
        }
        break;

    case Model::DemoDone:
        presenter->resetAutoDemo();
        break;

    default:
        break;

    }

    autoDemoCounter++;
}

void HallwayScreenView::screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y)
{
    ClickEvent evt(type, x, y);
    View::handleClickEvent(evt);
}
