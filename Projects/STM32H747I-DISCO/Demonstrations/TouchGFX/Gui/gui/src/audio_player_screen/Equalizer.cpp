

#include <gui/audio_player_screen/Equalizer.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <math.h>

Equalizer::Equalizer() :
    onDrag(this, &Equalizer::drag),
    onStop(this, &Equalizer::stop),
    onLoudnessChanged(this, &Equalizer::loudnessChanged),
    callback(0)
{
    background.setBitmap(Bitmap(BITMAP_EQUALIZER_BACKGROUND_ID));
    add(background);

    EqualizerSlider* sliders[] = { &slider100Hz, &slider1kHz, &slider10kHz, &slider20kHz };
    for (int i = 0; i < 4; i++)
    {
        EqualizerSlider* slider = sliders[i];
        slider->setXY(49 + i * 139 + ((i > 1) ? i - 1 : 0), 97);
        slider->setBitmaps(Bitmap(BITMAP_EQUALIZER_BAR_UNFILL_ID), Bitmap(BITMAP_EQUALIZER_BAR_FILL_ID), Bitmap(BITMAP_LOUDNESS_KNOP_ID));
        slider->setupVerticalSlider(19, 19, 0, 0, 302);
        slider->setValueRange(-12, 12);
        slider->setNewValueCallback(onDrag);
        slider->setStopValueCallback(onStop);
        add(*slider);
    }

    sliderValue.setBitmap(Bitmap(BITMAP_EQUALIZER_DB_VALUE_ID));
    sliderValue.setVisible(false);
    add(sliderValue);

    sliderValueBuffer[0] = 0;
    sliderValueText.setPosition(0, 0, sliderValue.getWidth(), sliderValue.getHeight());
    sliderValueText.setTypedText(TypedText(T_EQUALIZER_BAND_READOUT));
    sliderValueText.setColor(Color::getColorFrom24BitRGB(29, 29, 35));
    sliderValueText.setWildcard(sliderValueBuffer);
    sliderValueText.setVisible(false);
    add(sliderValueText);

    loudnessSlider.setXY(589, 178);
    loudnessSlider.setLoudnessChangedCallback(onLoudnessChanged);
    //loudnessSlider.setValue(0); //default value
    add(loudnessSlider);

    loudnessValueBuffer[0] = 0;
    loudnessValueText.setPosition(loudnessSlider.getX(), loudnessSlider.getY() + 55, loudnessSlider.getWidth(), 120);
    loudnessValueText.setTypedText(TypedText(T_EQUALIZER_LOUDNESS_READOUT));
    loudnessValueText.setColor(Color::getColorFrom24BitRGB(1, 191, 255));
    loudnessValueText.setWildcard(loudnessValueBuffer);
    add(loudnessValueText);

    setPosition(0, 0, background.getWidth(), background.getHeight());
    //setTouchable(true); not good in combination with fingersize
}

void Equalizer::drag(const Slider& slider, int value)
{
    const EqualizerSlider& equalizerSlider = (const EqualizerSlider&)slider;

    sliderValue.setVisible(true);
    sliderValue.moveTo(slider.getX() - 28, equalizerSlider.getIndicatorY() + 60);

    Unicode::snprintf(sliderValueBuffer, 5, "%d", value);

    sliderValueText.setVisible(true);
    sliderValueText.moveTo(slider.getX() - 70, equalizerSlider.getIndicatorY() + 62);

    if (callback && callback->isValid())
    {
        AudioPlayer::EqualizerSetting setting = AudioPlayer::_100HZ;
        if (&slider == &slider1kHz)
        {
            setting = AudioPlayer::_1KHZ;
        }
        else if (&slider == &slider10kHz)
        {
            setting = AudioPlayer::_10KHZ;
        }
        else if (&slider == &slider20kHz)
        {
            setting = AudioPlayer::_20KHZ;
        }
        callback->execute(setting, value);
    }
}

void Equalizer::stop(const Slider&, int)
{
    sliderValue.setVisible(false);
    sliderValueText.setVisible(false);
    sliderValue.invalidate();
}

void Equalizer::loudnessChanged(const unsigned int percentage)
{
    Unicode::snprintfFloat(loudnessValueBuffer, 6, "%2.1f", (float)(percentage * 24.0f / 100.0f) - 12.0f);
    loudnessValueText.invalidate();
    if (callback && callback->isValid())
    {
        /*MMa : adjust loud from percentage to dB*/
        callback->execute(AudioPlayer::LOUDNESS, ((percentage * 24) / 100 - 12));
    }
}

void Equalizer::setSettings(int16_t _100Hz, int16_t _1kHz, int16_t _10kHz, int16_t _20kHz, int16_t loudness)
{
    /* Values are in db */
    slider100Hz.setValue(_100Hz);
    slider1kHz.setValue(_1kHz);
    slider10kHz.setValue(_10kHz);
    slider20kHz.setValue(_20kHz);
    loudnessSlider.setValue(loudness);
    sliderValue.setVisible(false);
    sliderValueText.setVisible(false);
}

LoudnessSlider::LoudnessSlider() :
    callback(0),
    colorPainter(Color::getColorFrom24BitRGB(1, 191, 255))
{
    dot.setPosition(0, 0, 14, 14);
    dot.setCircle(7, 7, 7);
    dot.setLineWidth(0);
    dot.setPainter(colorPainter);
    add(dot);

    setPosition(0, 0, size, size);
    setTouchable(true);
}

void LoudnessSlider::handleClickEvent(const ClickEvent& evt)
{
    update(evt.getX(), evt.getY());
}

void LoudnessSlider::handleDragEvent(const DragEvent& evt)
{
    update(evt.getNewX(), evt.getNewY());
}

void LoudnessSlider::update(int16_t x, int16_t y)
{
    int16_t vx = x - getWidth() / 2;
    int16_t vy = y - getHeight() / 2;

    int length;
    int angleInDegrees = CWRUtil::angle<int>(vx, vy, length);

    if (length < 30 || length > 130)
    {
        return;
    }

    //clamp angle
    if (angleInDegrees > maxAngle && angleInDegrees <= 180)
    {
        angleInDegrees = maxAngle;
    }
    if (angleInDegrees < 360 - maxAngle && angleInDegrees > 180)
    {
        angleInDegrees = 360 - maxAngle;
    }

    dot.moveTo((int16_t)(getWidth() / 2.0f * (1.0f + 0.8f * cosf((angleInDegrees - 90) * 3.141592f / 180.0f)) - dot.getWidth() / 2),
               (int16_t)(getHeight() / 2.0f * (1.0f + 0.8f * sinf((angleInDegrees - 90) * 3.141592f / 180.0f))) - dot.getHeight() / 2);

    if (callback && callback->isValid())
    {
        int16_t percentage = (((angleInDegrees + maxAngle) % 360) * 100) / (2 * maxAngle);
        callback->execute(percentage);
    }
}

void LoudnessSlider::setValue(int16_t value)
{
    assert(value <= 12 && value >= -12 && "value out of range");
    //-12 to 12
    int percentage = ((value + 12) * 100) / 24;
    int angleInDegrees = percentage * 2 * maxAngle / 100 - maxAngle;

    dot.moveTo((int16_t)(getWidth() / 2.0f * (1.0f + 0.8f * cosf((angleInDegrees - 90) * 3.141592f / 180.0f)) - dot.getWidth() / 2),
               (int16_t)(getHeight() / 2.0f * (1.0f + 0.8f * sinf((angleInDegrees - 90) * 3.141592f / 180.0f))) - dot.getHeight() / 2);

    if (callback && callback->isValid())
    {
        callback->execute(percentage);
    }

}
