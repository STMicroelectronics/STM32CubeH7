#include <gui/containers/forecastWheelContainer.hpp>
#include <gui/common/ClockAndDate.hpp>
#include "BitmapDatabase.hpp"
#include "texts/TextKeysAndLanguages.hpp"

#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

forecastWheelContainer::forecastWheelContainer() :
    weatherElementsAnimationEnddedCallback(this, &forecastWheelContainer::weatherElementsAnimationEndded)
{

}

void forecastWheelContainer::initialize()
{
    forecastWheelContainerBase::initialize();

    currentWeatherIcon.setFadeAnimationEndedAction(weatherElementsAnimationEnddedCallback);

    forecast5days.setXY(0, 210);
    add(forecast5days);
    
    //currentWeatherData.setXY(488, 140);    
    //add(currentWeatherData);
}

void forecastWheelContainer::convertToUppercase(char *word, int size)
{
    for (int i = 0; i < size; i++)
    {
        word[i] = toupper(word[i]);
    }
}

void forecastWheelContainer::setLocation(char newLocation[])
{
    convertToUppercase(newLocation, 20);
    Unicode::strncpy(locationBuffer, newLocation, LOCATION_SIZE);
    location.invalidate();
}

void forecastWheelContainer::updateCurrentTemp(int temperature, bool metric)
{
    if (metric)
    {
        currentTemp.setTypedText(TypedText(T_MAINTEMPC));
    }
    else
    {
        currentTemp.setTypedText(TypedText(T_MAINTEMPF));
    }
    Unicode::snprintf(currentTempBuffer, CURRENTTEMP_SIZE, "%d", temperature);
    currentTemp.invalidate();
}

void forecastWheelContainer::updateForecast(struct weatherData *data, bool metric)
{
    forecast5days.updateForecast(data, metric);
}

void forecastWheelContainer::setDateAndTime(const char* currentTime, int32_t timzone, bool hour24)
{
    int year, month, day;

    char temp[5];

    const uint16_t WEEKDAYNAME_SIZE = 10;
    Unicode::UnicodeChar weekdayName[WEEKDAYNAME_SIZE];
    const uint16_t MONTHNAME_SIZE = 10;
    Unicode::UnicodeChar monthName[MONTHNAME_SIZE];

    memcpy(temp, &currentTime[0], 4);
    temp[4] = '\0';
    year = atoi(temp);

    memcpy(temp, &currentTime[5], 2);
    temp[2] = '\0';
    month = atoi(temp);

    memcpy(temp, &currentTime[8], 2);
    day = atoi(temp);

    getWeekdayName(getWeekday(year, month, day), weekdayName, WEEKDAYNAME_SIZE);

    getMonthName(month, monthName, MONTHNAME_SIZE);

    int hour, minute;
    const uint16_t CLOCK_SIZE = 10;
    Unicode::UnicodeChar clock[CLOCK_SIZE];

    memcpy(temp, &currentTime[11], 2);
    temp[2] = '\0';
    hour = atoi(temp);

    memcpy(temp, &currentTime[14], 2);
    temp[2] = '\0';
    minute = atoi(temp);

    getClock(hour, minute, timzone, hour24, clock, CLOCK_SIZE);

    Unicode::snprintf(currentDateClockBuffer1, CURRENTDATECLOCKBUFFER1_SIZE, "%s %d %s %d", weekdayName, day, monthName, year);
    Unicode::snprintf(currentDateClockBuffer2, CURRENTDATECLOCKBUFFER2_SIZE, "%s", clock);
    currentDateClock.invalidate();
}

void forecastWheelContainer::updateCurrentWeatherIcon(weatherData *data)
{
    char temp[3];

    memcpy(temp, data->icon[0], 2);
    int x = atoi(temp);

    if (data->icon[0][2] == 'd')
    {
        switch (x)
        {
        case 1:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_CLEAR_SKY_ICON_64_ID));
            break;

        case 2:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_FEW_CLOUDS_ICON_64_ID));
            break;

        case 3:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_SCATTERED_CLOUDS_ICON_64_ID));
            break;

        case 4:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_BROKEN_CLOUDS_ICON_64_ID));
            break;

        case 9:
        case 10:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_RAIN_ICON_64_ID));
            break;

        case 11:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_THUNDER_ICON_64_ID));
            break;

        case 13:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_SNOW_ICON_64_ID));
            break;

        case 50:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_MIST_ICON_64_ID));
            break;

        default:
            break;
        }
    }
    else
    {
        switch (x)
        {
        case 1:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_CLEAR_SKY_NIGHT_ICON_64_ID));
            break;

        case 2:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_FEW_CLOUDS_NIGHT_ICON_64_ID));
            break;

        case 3:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_SCATTERED_CLOUDS_NIGHT_ICON_64_ID));
            break;

        case 4:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_BROKEN_CLOUDS_NIGHT_ICON_64_ID));
            break;

        case 9:
        case 10:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_RAIN_NIGHT_ICON_64_ID));
            break;

        case 11:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_THUNDER_NIGHT_ICON_64_ID));
            break;

        case 13:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_SNOW_NIGHT_ICON_64_ID));
            break;

        case 50:
            currentWeatherIcon.setBitmap(Bitmap(BITMAP_MIST_NIGHT_ICON_64_ID));
            break;

        default:
            break;
        }
    }
    currentWeatherIcon.invalidate();
}

void forecastWheelContainer::updateCurrentWeatherInformation(weatherData *data, bool metric)
{
    currentWeatherData.updateCurrentWeatherInformation(data, metric);
}

void forecastWheelContainer::fadeWeatherElements(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation)
{
    currentWeatherIcon.setFadeAnimationDelay(delay);
    currentDateClock.setFadeAnimationDelay(delay);
    currentTemp.setFadeAnimationDelay(delay);
    location.setFadeAnimationDelay(delay);
    forecast5days.setFadeAnimationDelay(delay);
    currentWeatherData.setFadeAnimationDelay(delay);
    
    currentWeatherIcon.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    currentDateClock.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    currentTemp.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    location.startFadeAnimation(endAlpha, duration, alphaProgressionEquation); 
    forecast5days.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
    currentWeatherData.startFadeAnimation(endAlpha, duration, alphaProgressionEquation);
}

bool forecastWheelContainer::isAnimating()
{
    return currentWeatherData.isRunning();
}

uint8_t forecastWheelContainer::getAlpha()
{
    return currentWeatherData.getAlpha();
}

void forecastWheelContainer::weatherElementsAnimationEndded(const FadeAnimator<Image> & src)
{
    if (animationEnddedCallback && animationEnddedCallback->isValid())
    {
        animationEnddedCallback->execute();
    }
}

void forecastWheelContainer::setAnimationEnddedAction(GenericCallback<>& callback)
{
    animationEnddedCallback = &callback;
}
