#include <gui/containers/WasherContainer.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "BitmapDatabase.hpp"

WasherContainer::WasherContainer() :
    animationState(WAITING),
    washerWheelAnimateToCallback(this, &WasherContainer::washerWheelAnimateToHandler),
    fadeAnimatonEndedCallBack(this, &WasherContainer::fadeAnimationEnddedHandler),
    buttonPressedCallBack(this, &WasherContainer::buttonPressedHandler),
    centerAnimatedImageCallback(this, &WasherContainer::centerAnimatedImageDone),
	currentItem(0), paused(false), touchTimer(0), animationTimer(0), currentProgram(0), washingTime(0)
{

}

void WasherContainer::initialize()
{
    WasherContainerBase::initialize();

    washerWheel.setAnimationSteps(40);
    washerWheel.setAnimateToCallback(washerWheelAnimateToCallback);

    textAreaDash.setFadeAnimationEndedAction(fadeAnimatonEndedCallBack);
    buttonStart.setAction(buttonPressedCallBack);

    centerAnimation.setDoneAction(centerAnimatedImageCallback);
    
    currentItem = washerWheel.getSelectedItem();

    updateProgramTexts();

    progressBar.setCapPrecision(15);

    mcuLoad.initialize();
    mcuLoad.setXY(317, 0);
    mcuLoad.setMCUOnly(true);
    mcuLoad.setTextColor(Color::getColorFrom24BitRGB(92, 92, 92));
    add(mcuLoad);
}

void WasherContainer::washerWheelUpdateItem(WasherWheelItem& item, int16_t itemIndex)
{
    item.setIcon(getIconBitmap(itemIndex));
}

void WasherContainer::washerWheelUpdateCenterItem(WasherWheelCenterItem& item, int16_t itemIndex)
{
    item.setIcon(getSelectedIconBitmap(itemIndex));
}

void WasherContainer::rightButtonClicked()
{
    if (!washerWheel.isAnimating())
    {
        int item = washerWheel.getSelectedItem();
        washerWheel.animateToItem(item + 1, 50);
    }
}

void WasherContainer::leftButtonClicked()
{
    if (!washerWheel.isAnimating())
    {
        int item = washerWheel.getSelectedItem();
        washerWheel.animateToItem(item - 1, 50);
    }
}

void WasherContainer::washerWheelAnimateToHandler(int16_t item)
{
    if (currentItem != (item % 7))
    {
        currentItem = item % 7;
        fadeOutProgramTexts();
    }
}

void WasherContainer::fadeAnimationEnddedHandler(const FadeAnimator<TextArea>& txt)
{
    switch (animationState)
    {
    case WAITING:
        if (textAreaDash.getAlpha() == 0)
        {
            updateProgramTexts();
        }
        break;
    case WAITING_START:
        break;
    case STARTING:
        prewashIcon.setVisible(true);
        prewashIcon.invalidate();
        washingIcon.setVisible(true);
        washingIcon.invalidate();
        rinsingIcon.setVisible(true);
        rinsingIcon.invalidate();
        spinCycleIcon.setVisible(true);
        spinCycleIcon.invalidate();
        break;
    case WASHING_PREWASH:
    case WASHING_WASHING:
    case WASHING_RINSING:
    case WASHING_SPINNING:
    case WASHING_DONE:
        break;
    }
}

void WasherContainer::setProgramTextAndTime(int16_t item)
{
    switch (item)
    {
    default:
    case 0:
        washingTime = 160;
        Unicode::strncpy(textAreaProgramNameBuffer, "ACTIVE WEAR", TEXTAREAPROGRAMNAME_SIZE);
        currentProgram = 0;
        break;
    case -6:
    case 1:
        washingTime = 120;
        Unicode::strncpy(textAreaProgramNameBuffer, "DELICATES", TEXTAREAPROGRAMNAME_SIZE);
        currentProgram = 1;
        break;
    case -5:
    case 2:
        washingTime = 190;
        Unicode::strncpy(textAreaProgramNameBuffer, "ECO WASH", TEXTAREAPROGRAMNAME_SIZE);
        currentProgram = 2;
        break;
    case -4:
    case 3:
        washingTime = 165;
        Unicode::strncpy(textAreaProgramNameBuffer, "HEAVY DUTY", TEXTAREAPROGRAMNAME_SIZE);
        currentProgram = 3;
        break;
    case -3:
    case 4:
        washingTime = 145;
        Unicode::strncpy(textAreaProgramNameBuffer, "NORMAL", TEXTAREAPROGRAMNAME_SIZE);
        currentProgram = 4;
        break;
    case -2:
    case 5:
        washingTime = 65;
        Unicode::strncpy(textAreaProgramNameBuffer, "QUICK WASH", TEXTAREAPROGRAMNAME_SIZE);
        currentProgram = 5;
        break;
    case -1:
    case 6:
        washingTime = 135;
        Unicode::strncpy(textAreaProgramNameBuffer, "SMART WASH", TEXTAREAPROGRAMNAME_SIZE);
        currentProgram = 6;
        break;
    }

    Unicode::snprintf(textAreaProgramLengthBuffer, TEXTAREAPROGRAMLENGTH_SIZE, "%d:%02d", washingTime / 60, washingTime % 60);
}

void WasherContainer::fadeOutProgramTexts()
{
    //only start fade once
    if (!textAreaDash.isRunning())
    {
        const int duration = 10;
        const int delay = 15;
        textAreaProgramName.setFadeAnimationDelay(delay);
        textAreaProgramName.startFadeAnimation(0, duration);
        textAreaDash.setFadeAnimationDelay(delay);
        textAreaDash.startFadeAnimation(0, duration);
        textAreaProgramLength.setFadeAnimationDelay(delay);
        textAreaProgramLength.startFadeAnimation(0, duration);
    }
}

void WasherContainer::updateProgramTexts()
{
    setProgramTextAndTime(currentItem);

    textAreaProgramName.setFadeAnimationDelay(0);
    textAreaProgramName.startFadeAnimation(255, 5);
    textAreaDash.setFadeAnimationDelay(0);
    textAreaDash.startFadeAnimation(255, 5);
    textAreaProgramLength.setFadeAnimationDelay(0);
    textAreaProgramLength.startFadeAnimation(255, 5);
}


void WasherContainer::tickEvent()
{
    touchTimer++;
    switch (animationState)
    {
    case WasherContainer::WAITING:

        if (currentItem != washerWheel.getSelectedItem())
        {
            currentItem = washerWheel.getSelectedItem();
            updateProgramTexts();
        }
        if (touchTimer == TOUCH_START_TIMEOUT && !washerWheel.isAnimating())
        {
            gotoWaitingStart();
        }
        break;
    case WasherContainer::WAITING_START:
        break;
    case WasherContainer::WASHING_PREWASH:
    case WasherContainer::WASHING_WASHING:
    case WasherContainer::WASHING_RINSING:
    case WasherContainer::WASHING_SPINNING:
        {
            if (paused)
            {
                break;
            }
            if (++animationTimer < 30)
            {
                break;
            }
            float angle = (animationTimer - 30) * 0.6f;
            progressBar.setArc(0, static_cast<int16_t>(angle));
            progressBar.invalidate();
            if (angle > 360)
            {
                gotoWashingDone();
            }
            else if (angle > 270)
            {
                gotoWashingSpinning();
            }
            else if (angle > 180)
            {
                gotoWashingRinsing();
            }
            else if (angle > 90)
            {
                gotoWashingWashing();
            }

            break;
        }
    case WasherContainer::WASHING_DONE:
        {
            if (++animationTimer < 30)
            {
                break;
            }

            if (animationTimer == 30)
            {
                progressBar.setVisible(false);
                progressBar.setArc(0, 0);
                progressBar.invalidate();
            }

            if (animationTimer == 60)
            {
                centerAnimation.startAnimation(true, false, false);
            }

            if (animationTimer == 180)
            {

                animationState = WAITING;

                buttonRun.setVisible(false);
                buttonRun.invalidate();

                buttonPause.setVisible(false);
                buttonPause.invalidate();

                washerWheel.setVisible(true);
                washerWheel.invalidate();

                leftShade.setVisible(true);
                leftShade.invalidate();

                rightShade.setVisible(true);
                rightShade.invalidate();

                leftButton.setXY(0, 112);
                leftButton.setAlpha(255);
                leftButton.invalidate();

                rightButton.setXY(440, 112);
                rightButton.setAlpha(255);
                rightButton.invalidate();

                washDoneBackground.setVisible(false);
                washDoneBackground.invalidate();

                icon1.setVisible(false);
                icon1.invalidate();
                icon2.setVisible(false);
                icon2.invalidate();
                icon3.setVisible(false);
                icon3.invalidate();
                icon4.setVisible(false);
                icon4.invalidate();
                icon5.setVisible(false);
                icon5.invalidate();
                icon6.setVisible(false);
                icon6.invalidate();

                centerAnimation.setVisible(false);
                centerAnimation.invalidate();

                imageWhiteCenter.setVisible(true);
                imageWhiteCenter.invalidate();

                imageRPM.setAlpha(255);
                imageHeat.setAlpha(255);
                imageLevel.setAlpha(255);
                imageOption1.setAlpha(255);
                imageOption2.setAlpha(255);
                imageOption3.setAlpha(255);

                imageRPM.invalidate();
                imageHeat.invalidate();
                imageLevel.invalidate();
                imageOption1.invalidate();
                imageOption2.invalidate();
                imageOption3.invalidate();

                prewashIcon.setVisible(false);
                prewashIcon.invalidate();
                washingIcon.setVisible(false);
                washingIcon.invalidate();
                rinsingIcon.setVisible(false);
                rinsingIcon.invalidate();
                spinCycleIcon.setVisible(false);
                spinCycleIcon.invalidate();

                buttonStart.setAlpha(0);
                buttonStart.setVisible(true);
                buttonStart.invalidate();

                updateProgramTexts();
            }
        }
        break;
    default:
        break;
    }

}

void WasherContainer::clickEvent(const ClickEvent& event)
{
    touchTimer = 0;
    switch (animationState)
    {
    case WAITING:
        break;
    case WAITING_START:
        animationState = WAITING;
        buttonStart.startFadeAnimation(0, 10);
        break;
    default:
        break;
    }
}

void WasherContainer::gotoWaitingStart()
{
    animationState = WAITING_START;
    buttonStart.setAlpha(0);
    buttonStart.startFadeAnimation(255, 10);
}

void WasherContainer::buttonPressedHandler(const AbstractButton& src)
{
    if (!washerWheel.isAnimating())
    {
        fadeOutProgramTextsNow();
        animationState = STARTING;

        leftReplacementImage.setX(washerWheel.getX() + washerWheel.getSelectedItemMarginBefore());
        leftReplacementImage.setBitmap(getIconBitmap(washerWheel.getSelectedItem() - 1));
        leftReplacementImage.setVisible(true);
        leftReplacementImage.invalidate();

        rightReplacementImage.setX(5 * washerWheel.getX() + 4 * washerWheel.getDrawableMargin() + washerWheel.getSelectedItemMarginBefore());
        rightReplacementImage.setBitmap(getIconBitmap(washerWheel.getSelectedItem() + 1));
        rightReplacementImage.setVisible(true);
        rightReplacementImage.invalidate();

        washerWheel.invalidate();
        washerWheel.setVisible(false);

        leftShade.setVisible(false);
        leftShade.invalidate();
        rightShade.setVisible(false);
        rightShade.invalidate();

        leftReplacementImage.startMoveAnimation(leftReplacementImage.getX() - START_MOVE_ANIMATION_LENGTH, leftReplacementImage.getY(),
                                                START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);

        rightReplacementImage.startMoveAnimation(rightReplacementImage.getX() + START_MOVE_ANIMATION_LENGTH, rightReplacementImage.getY(),
                                                 START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);

        leftButton.startMoveAnimation(leftButton.getX() - START_MOVE_ANIMATION_LENGTH, leftButton.getY(),
                                      START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);
        rightButton.startMoveAnimation(rightButton.getX() + START_MOVE_ANIMATION_LENGTH, leftButton.getY(),
                                       START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);

        //remove low elements
        imageRPM.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
        imageHeat.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
        imageLevel.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
        imageOption1.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
        imageOption2.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
        imageOption3.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);

        //running & Pause
        buttonRun.setAlpha(0);
        buttonRun.setVisible(true);
        buttonRun.setFadeAnimationDelay(START_MOVE_ANIMATION_DURATION);
        buttonRun.startFadeAnimation(255, START_MOVE_FADE_IN_DURATION);

        buttonPause.setAlpha(0);
        buttonPause.setVisible(true);
        buttonPause.setFadeAnimationDelay(START_MOVE_ANIMATION_DURATION);
        buttonPause.startFadeAnimation(255, START_MOVE_FADE_IN_DURATION);

        //fly in elements
        icon1.setVisible(true);
        icon1.setBitmap(Bitmap(BITMAP_SOIL_LEVEL_MEDIUM_ID));
        icon1.setXY(-120, 70);
        icon1.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY);
        icon1.startMoveAnimation(0, 70, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

        icon2.setVisible(true);
        icon2.setBitmap(Bitmap(BITMAP_HEAT_60_ID));
        icon2.setXY(-120, 106);
        icon2.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL);
        icon2.startMoveAnimation(0, 106, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

        icon3.setVisible(true);
        icon3.setBitmap(Bitmap(BITMAP_SPIN_1600_ID));
        icon3.setXY(-120, 142);
        icon3.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 2);
        icon3.startMoveAnimation(0, 142, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

        icon4.setVisible(true);
        icon4.setBitmap(Bitmap(BITMAP_ALLERGY_ID));
        icon4.setXY(480, 71);
        icon4.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 0);
        icon4.startMoveAnimation(358, 70, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

        icon5.setVisible(true);
        icon5.setBitmap(Bitmap(BITMAP_ECO_WASH_ID));
        icon5.setXY(480, 105);
        icon5.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 1);
        icon5.startMoveAnimation(358, 106, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

        icon6.setVisible(true);
        icon6.setBitmap(Bitmap(BITMAP_PREWASH_ID));
        icon6.setXY(480, 140);
        icon6.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 2);
        icon6.startMoveAnimation(358, 142, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

        centerAnimation.setVisible(true);
        centerAnimation.invalidate();
        centerAnimation.startAnimation(false, true, false);

        imageWhiteCenter.setVisible(false);
        imageWhiteCenter.invalidate();

        buttonStart.setVisible(false);
        buttonStart.invalidate();

    }
}

Bitmap WasherContainer::getSelectedIconBitmap(int16_t index)
{
    switch (index)
    {
    case 0:
        return Bitmap(BITMAP_ICON_CENTER_ACTIVEWEAR_MEDIUM_ID);
    case 1:
        return Bitmap(BITMAP_ICON_CENTER_DELICATES_MEDIUM_ID);
    case 2:
        return Bitmap(BITMAP_ICON_CENTER_ECOWASH_MEDIUM_ID);
    case 3:
        return Bitmap(BITMAP_ICON_CENTER_HEAVYDUTY_MEDIUM_ID);
    case 4:
        return Bitmap(BITMAP_ICON_CENTER_NORMAL_MEDIUM_ID);
    case 5:
        return Bitmap(BITMAP_ICON_CENTER_QUICKWASH_MEDIUM_ID);
    case -1:
    case 6:
        return Bitmap(BITMAP_ICON_CENTER_SMARTWASH_MEDIUM_ID);
    default:
        return Bitmap(BITMAP_ICON_CENTER_ACTIVEWEAR_ID);
    }
}

Bitmap WasherContainer::getIconBitmap(int16_t index)
{
    switch (index)
    {
    case 0:
        return Bitmap(BITMAP_ICON_CENTER_ACTIVEWEAR_ID);
    case 1:
        return Bitmap(BITMAP_ICON_CENTER_DELICATES_ID);
    case 2:
        return Bitmap(BITMAP_ICON_CENTER_ECOWASH_ID);
    case 3:
        return Bitmap(BITMAP_ICON_CENTER_HEAVYDUTY_ID);
    case 4:
        return Bitmap(BITMAP_ICON_CENTER_NORMAL_ID);
    case 5:
        return Bitmap(BITMAP_ICON_CENTER_QUICKWASH_ID);
    case -1:
    case 6:
        return Bitmap(BITMAP_ICON_CENTER_SMARTWASH_ID);
    default:
        return Bitmap(BITMAP_ICON_CENTER_ACTIVEWEAR_ID);
    }
}

void WasherContainer::fadeOutProgramTextsNow()
{
    const int duration = 10;
    const int delay = 0;
    textAreaProgramName.setFadeAnimationDelay(delay);
    textAreaProgramName.startFadeAnimation(0, duration);
    textAreaDash.setFadeAnimationDelay(delay);
    textAreaDash.startFadeAnimation(0, duration);
    textAreaProgramLength.setFadeAnimationDelay(delay);
    textAreaProgramLength.startFadeAnimation(0, duration);
}

void WasherContainer::centerAnimatedImageDone(const AnimatedImage&)
{
    if (animationState == STARTING)
    {
        gotoWashingPrewash();
    }

    if (animationState == WASHING_DONE)
    {
        washDoneBackground.setVisible(true);
        washDoneBackground.invalidate();

        washAnimation.stopAnimation();
        washAnimation.startFadeAnimation(0, 30);
    }
}

void WasherContainer::gotoWashingPrewash()
{
    animationState = WASHING_PREWASH;
    animationTimer = 0;

    washAnimation.setVisible(true);
    washAnimation.startFadeAnimation(255, 30);
    washAnimation.startAnimation(false, true, true);

    prewashIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_PREWASH_ACTIVE_ID));
    prewashIcon.invalidate();

    progressBar.setAlpha(255);
    progressBar.setVisible(true);
    progressBar.invalidate();
}

void WasherContainer::gotoWashingDone()
{
    animationState = WASHING_DONE;
    spinCycleIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_SPIN_CYCLE_DONE_ID));
    spinCycleIcon.invalidate();

    progressBar.startFadeAnimation(0, 30);

    animationTimer = 0;
}

void WasherContainer::gotoWashingSpinning()
{
    animationState = WASHING_SPINNING;
    rinsingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_RINSING_DONE_ID));
    rinsingIcon.invalidate();
    spinCycleIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_SPIN_CYCLE_ACTIVE_ID));
    spinCycleIcon.invalidate();
}

void WasherContainer::gotoWashingRinsing()
{
    animationState = WASHING_RINSING;
    washingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_WASHING_DONE_ID));
    washingIcon.invalidate();
    rinsingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_RINSING_ACTIVE_ID));
    rinsingIcon.invalidate();
}

void WasherContainer::gotoWashingWashing()
{
    animationState = WASHING_WASHING;
    prewashIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_PREWASH_DONE_ID));
    prewashIcon.invalidate();
    washingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_WASHING_ACTIVE_ID));
    washingIcon.invalidate();
}

void WasherContainer::buttonRunClicked()
{
    paused = false;
    washAnimation.startAnimation(false, false, true);
}

void WasherContainer::buttonPausedClicked()
{
    paused = true;
    washAnimation.stopAnimation();
}

uint16_t WasherContainer::getWashingProgram()
{
    return currentProgram;
}

uint16_t WasherContainer::getWashingTime()
{
    return washingTime;
}

void WasherContainer::setWashingProgram(uint16_t program)
{
    washerWheel.animateToItem(program - 2, 0);
    washerWheel.animateToItem(program, 50);    
    setProgramTextAndTime(program);
}

void WasherContainer::resetContainer()
{
    animationState = WAITING;

    buttonRun.setVisible(false);
    buttonPause.setVisible(false);
    washerWheel.setVisible(true);
    leftShade.setVisible(true);
    rightShade.setVisible(true);
    leftButton.setXY(0, 112);
    leftButton.setAlpha(255);
    rightButton.setXY(440, 112);
    rightButton.setAlpha(255);
    washDoneBackground.setVisible(false);

    progressBar.setVisible(false);
    progressBar.setArc(0, 0);

    washAnimation.setVisible(false);

    icon1.setVisible(false);
    icon2.setVisible(false);
    icon3.setVisible(false);
    icon4.setVisible(false);
    icon5.setVisible(false);
    icon6.setVisible(false);

    centerAnimation.setVisible(false);

    imageWhiteCenter.setVisible(true);

    imageRPM.setAlpha(255);
    imageHeat.setAlpha(255);
    imageLevel.setAlpha(255);
    imageOption1.setAlpha(255);
    imageOption2.setAlpha(255);
    imageOption3.setAlpha(255);

    prewashIcon.setVisible(false);
    washingIcon.setVisible(false);
    rinsingIcon.setVisible(false);
    spinCycleIcon.setVisible(false);

    buttonStart.setAlpha(0);
    buttonStart.setVisible(true);

    currentItem = washerWheel.getSelectedItem();

    updateProgramTexts();
}

void WasherContainer::updateMcuLoad(uint8_t load)
{
    mcuLoad.setMcuLoad(load);
}
