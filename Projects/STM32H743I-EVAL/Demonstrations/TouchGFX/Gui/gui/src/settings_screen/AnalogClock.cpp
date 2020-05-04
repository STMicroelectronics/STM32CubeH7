#include <gui/settings_screen/AnalogClock.hpp>
#include <touchgfx/Math3D.hpp>

AnalogClock::AnalogClock()
{
    add(background);

    hourHandMinuteCorrectionActive = false;
    minuteHandSecondCorrectionActive = false;

    animationDuration = 0;

    currentHour = 0;
    currentMinute = 0;
    currentSecond = 0;

    hourHand.updateZAngle(0.f);
    minuteHand.updateZAngle(0.f);
    secondHand.updateZAngle(0.f);

    hourHand.setVisible(false);
    minuteHand.setVisible(false);
    secondHand.setVisible(false);
}

AnalogClock::~AnalogClock()
{
}

void AnalogClock::setBackground(const BitmapId backgroundBitmapId)
{
    setBackground(backgroundBitmapId, Bitmap(backgroundBitmapId).getWidth() / 2, Bitmap(backgroundBitmapId).getHeight() / 2);
}

void AnalogClock::setBackground(const BitmapId backgroundBitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    background.setBitmap(Bitmap(backgroundBitmapId));

    clockRotationCenterX = rotationCenterX;
    clockRotationCenterY = rotationCenterY;

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

void AnalogClock::setupHourHand(const BitmapId hourHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    setupHand(hourHand, hourHandBitmapId, rotationCenterX, rotationCenterY);
}

void AnalogClock::setupMinuteHand(const BitmapId minuteHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    setupHand(minuteHand, minuteHandBitmapId, rotationCenterX, rotationCenterY);
}

void AnalogClock::setupSecondHand(const BitmapId secondHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    setupHand(secondHand, secondHandBitmapId, rotationCenterX, rotationCenterY);
}

void AnalogClock::setupHand(TextureMapper& hand, const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    remove(hand);

    hand.setBitmap(Bitmap(bitmapId));
    hand.setWidth(background.getWidth());
    hand.setHeight(background.getHeight());
    hand.setXY(0, 0);
    hand.setBitmapPosition(clockRotationCenterX - rotationCenterX, clockRotationCenterY - rotationCenterY);
    hand.setCameraDistance(300.0f);
    hand.setOrigo((float) clockRotationCenterX, (float) clockRotationCenterY, hand.getCameraDistance());
    hand.setCamera(hand.getOrigoX(), hand.getOrigoY());
    hand.setRenderingAlgorithm(TextureMapper::BILINEAR_INTERPOLATION);

    add(hand);
    hand.setVisible(true);
}

void AnalogClock::initializeTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    currentHour = 255;
    currentMinute = 255;
    currentSecond = 255;

    uint16_t tempAnimationDuration = animationDuration;
    animationDuration = 1;
    updateTime(hour, minute, second);

    animationDuration = tempAnimationDuration;
}

void AnalogClock::updateTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    hour %= 12;
    minute %= 60;
    second %= 60;

    // Make sure that animating to 0 will move from left to right
    if (currentHour != 0 && hour == 0)
    {
        hourHand.updateZAngle(hourHand.getZAngle() - (2 * PI));
    }
    if (currentMinute != 0 && minute == 0)
    {
        minuteHand.updateZAngle(minuteHand.getZAngle() - (2 * PI));
    }
    if (currentSecond != 0 && second == 0)
    {
        secondHand.updateZAngle(secondHand.getZAngle() - (2 * PI));
    }

    float newHandAngle;

    // Move hour hand
    if (hourHand.isVisible() && ((hour != currentHour) || (hourHandMinuteCorrectionActive && (minute != currentMinute))))
    {
        newHandAngle = convertHandValueToAngle(12.f, hour, hourHandMinuteCorrectionActive ? minute : 0);
        if (animationEnabled())
        {
            hourHand.setupAnimation(AnimationTextureMapper::Z_ROTATION, newHandAngle, animationDuration, 0, animationEquation);
            hourHand.startAnimation();
        }
        else
        {
            hourHand.updateZAngle(newHandAngle);
        }
    }

    // Move minute hand
    if (minuteHand.isVisible() && ((minute != currentMinute) || (minuteHandSecondCorrectionActive && (second != currentSecond))))
    {
        newHandAngle = convertHandValueToAngle(60.f, minute, minuteHandSecondCorrectionActive ? second : 0);
        if (animationEnabled())
        {
            minuteHand.setupAnimation(AnimationTextureMapper::Z_ROTATION, newHandAngle, animationDuration, 0, animationEquation);
            minuteHand.startAnimation();
        }
        else
        {
            minuteHand.updateZAngle(newHandAngle);
        }
    }

    // Move second hand
    if (secondHand.isVisible() && (second != currentSecond))
    {
        newHandAngle = convertHandValueToAngle(60.f, second);
        if (animationEnabled() && 0)
        {
            secondHand.setupAnimation(AnimationTextureMapper::Z_ROTATION, newHandAngle, animationDuration, 0, animationEquation);
            secondHand.startAnimation();
        }
        else
        {
            secondHand.updateZAngle(newHandAngle);
        }
    }

    currentHour = hour;
    currentMinute = minute;
    currentSecond = second;
}

float AnalogClock::convertHandValueToAngle(float steps, uint8_t primaryHandValue, uint8_t secondaryHandValue)
{
    return ((primaryHandValue / steps) + (secondaryHandValue / (steps * 60.f))) * 2 * PI;
}

void AnalogClock::useHourHandMinuteCorrection(bool use)
{
    hourHandMinuteCorrectionActive = use;
    updateTime(getCurrentHour(), getCurrentMinute(), getCurrentSecond());
}

void AnalogClock::useMinuteHandSecondCorrection(bool use)
{
    minuteHandSecondCorrectionActive = use;
    updateTime(getCurrentHour(), getCurrentMinute(), getCurrentSecond());
}

bool AnalogClock::animationEnabled()
{
    return animationDuration > 1;
}

void AnalogClock::useAnimation(uint16_t duration, EasingEquation animationProgressionEquation)
{
    animationDuration = duration;
    animationEquation = animationProgressionEquation;
}

