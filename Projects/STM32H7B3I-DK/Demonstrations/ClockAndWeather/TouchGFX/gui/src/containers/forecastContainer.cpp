#include <gui/containers/forecastContainer.hpp>
#include "BitmapDatabase.hpp"
#include "texts/TextKeysAndLanguages.hpp"
#include <gui/common/ClockAndDate.hpp>


#ifdef SIMULATOR
#include <time.h>
//#include <chrono>
#endif // SIMULATOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

forecastContainer::forecastContainer()
{

}

void forecastContainer::initialize()
{
    forecastContainerBase::initialize();
}

void forecastContainer::update(char *icon, char *weekday, int temperature, int windspeed, bool metric)
{
    updateIcon(icon);
    updateWeekday(weekday);
    updateTemperature(temperature);
    //updateWindspeed(windspeed, metric);
}

void forecastContainer::updateIcon(char* icon)
{
   char temp[2];
   memcpy(temp, icon, 2);
   int x = atoi(temp);

   if (icon[2] == 'd')
   {
       switch (x)
       {
       case 1:
           weatherIcon.setBitmap(Bitmap(BITMAP_CLEAR_SKY_ICON_24_ID));
           break;

       case 2:
           weatherIcon.setBitmap(Bitmap(BITMAP_FEW_CLOUDS_ICON_24_ID));
           break;

       case 3:
           weatherIcon.setBitmap(Bitmap(BITMAP_SCATTERED_CLOUDS_ICON_24_ID));
           break;

       case 4:
           weatherIcon.setBitmap(Bitmap(BITMAP_BROKEN_CLOUDS_ICON_24_ID));
           break;

       case 9:
       case 10:
           weatherIcon.setBitmap(Bitmap(BITMAP_RAIN_ICON_24_ID));
           break;

       case 11:
           weatherIcon.setBitmap(Bitmap(BITMAP_THUNDER_ICON_24_ID));
           break;

       case 13:
           weatherIcon.setBitmap(Bitmap(BITMAP_SNOW_ICON_24_ID));
           break;

       case 50:
           weatherIcon.setBitmap(Bitmap(BITMAP_MIST_ICON_24_ID));
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
           weatherIcon.setBitmap(Bitmap(BITMAP_CLEAR_SKY_NIGHT_ICON_24_ID));
           break;

       case 2:
           weatherIcon.setBitmap(Bitmap(BITMAP_FEW_CLOUDS_NIGHT_ICON_24_ID));
           break;

       case 3:
           weatherIcon.setBitmap(Bitmap(BITMAP_SCATTERED_CLOUDS_NIGHT_ICON_24_ID));
           break;

       case 4:
           weatherIcon.setBitmap(Bitmap(BITMAP_BROKEN_CLOUDS_NIGHT_ICON_24_ID));
           break;

       case 9:
       case 10:
           weatherIcon.setBitmap(Bitmap(BITMAP_RAIN_NIGHT_ICON_24_ID));
           break;

       case 11:
           weatherIcon.setBitmap(Bitmap(BITMAP_THUNDER_NIGHT_ICON_24_ID));
           break;

       case 13:
           weatherIcon.setBitmap(Bitmap(BITMAP_SNOW_NIGHT_ICON_24_ID));
           break;

       case 50:
           weatherIcon.setBitmap(Bitmap(BITMAP_MIST_NIGHT_ICON_24_ID));
           break;

       default:
           break;
       }
   }
   weatherIcon.invalidate();
}

void forecastContainer::updateWeekday(char* date)
{
    int year, month, day;

    char temp[4];

    memcpy(temp, &date[0], 4);
    year = atoi(temp);

    memcpy(temp, &date[5], 2);
    temp[2] = '\0';
    month = atoi(temp);

    memcpy(temp, &date[8], 2);
    day = atoi(temp);

    switch (getWeekday(year, month, day))
    {
    case 0:
        weekday.setTypedText(TypedText(T_FORECASTSUNDAY));
        break;
    case 1:
        weekday.setTypedText(TypedText(T_FORECASTMONDAY));
        break;
    case 2:
        weekday.setTypedText(TypedText(T_FORECASTTUESDAY));
        break;
    case 3:
        weekday.setTypedText(TypedText(T_FORECASTWEDENSDAY));
        break;
    case 4:
        weekday.setTypedText(TypedText(T_FORECASTTHURSDAY));
        break;
    case 5:
        weekday.setTypedText(TypedText(T_FORECASTFRIDAY));
        break;
    case 6:
        weekday.setTypedText(TypedText(T_FORECASTSATURDAY));
        break;
    }

    weekday.invalidate();
}

void forecastContainer::updateTemperature(int temperature)
{
    Unicode::snprintf(tempBuffer, TEMP_SIZE, "%d", temperature);    
    temp.invalidate();
}

//void forecastContainer::updateWindspeed(int speed, bool metric)
//{
//    if (metric)
//    {
//        wind.setTypedText(touchgfx::TypedText(T_WINDMETRIC));
//    }
//    else
//    {
//        wind.setTypedText(touchgfx::TypedText(T_WINDIMPERIAL));
//    }
//
//    Unicode::snprintf(windBuffer, WIND_SIZE, "%d", speed);
//    wind.invalidate();
//}

void forecastContainer::setAlpha(uint8_t alpha)
{
    //wind.setAlpha(alpha);
    temp.setAlpha(alpha);
    weekday.setAlpha(alpha);
    weatherIcon.setAlpha(alpha);
}

uint8_t forecastContainer::getAlpha()
{
    return weatherIcon.getAlpha();
}
