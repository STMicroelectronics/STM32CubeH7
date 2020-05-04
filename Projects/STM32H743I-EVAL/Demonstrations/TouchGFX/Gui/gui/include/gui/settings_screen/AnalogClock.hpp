#ifndef ANALOG_CLOCK
#define ANALOG_CLOCK

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <BitmapDatabase.hpp>
#include <gui/common/AnimationTextureMapper.hpp>
#include <touchgfx/EasingEquations.hpp>

using namespace touchgfx;

/**
 * @class AnalogClock
 *
 * @brief An analog clock.
 *
 */
class AnalogClock : public Container
{
public:
    AnalogClock();
    virtual ~AnalogClock();

    void setBackground(const BitmapId backgroundBitmapId);
    void setBackground(const BitmapId backgroundBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
    
    void setupHourHand(const BitmapId hourHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
    void setupMinuteHand(const BitmapId minuteHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
    void setupSecondHand(const BitmapId secondHandBitmapId, int16_t rotationCenterX, int16_t rotationCenterY);

    void useHourHandMinuteCorrection(bool use);
    void useMinuteHandSecondCorrection(bool use);

    void useAnimation(uint16_t duration = 10, EasingEquation animationProgressionEquation = EasingEquations::backEaseInOut);
    uint16_t getAnimationDuration() { return animationDuration; }

    void initializeTime(uint8_t hour, uint8_t minute, uint8_t second);
    void updateTime(uint8_t hour, uint8_t minute, uint8_t second);

    uint8_t getCurrentHour() { return currentHour; }
    uint8_t getCurrentMinute() { return currentMinute; }
    uint8_t getCurrentSecond() { return currentSecond; }

protected:
    Image background;

    AnimationTextureMapper hourHand;
    AnimationTextureMapper minuteHand;
    AnimationTextureMapper secondHand;

    int16_t clockRotationCenterX;
    int16_t clockRotationCenterY;

    uint8_t currentHour;
    uint8_t currentMinute;
    uint8_t currentSecond;

    bool hourHandMinuteCorrectionActive;
    bool minuteHandSecondCorrectionActive;

    uint16_t animationDuration;
    EasingEquation animationEquation;

    void setupHand(TextureMapper& hand, const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY);
    float convertHandValueToAngle(float steps, uint8_t handValue, uint8_t secondHandValue = 0);
    bool animationEnabled();
};

#endif /* ANALOG_CLOCK */
