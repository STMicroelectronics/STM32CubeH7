
#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <gui/settings_screen/DatePicker.hpp>
#include <stdio.h>

#ifndef SIMULATOR
#include "gui/audio_player_screen/audio_player_app.h"
#include "storage_app.h"
#include "FreeRTOS.h"
#include "rtc_app.h"

#ifdef STM32H743xx
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval_lcd.h"
#include "stm32h743i_eval.h"
#endif

xQueueHandle GUITaskQueue = 0;
#define AUDIO_EOF_NOTIFICATION 0x1
#define STORAGE_MSDDISK_DISCONNECTED 0x2
#define STORAGE_MSDDISK_CONNECTED 0x3
#define STORAGE_USBDISK_DISCONNECTED 0x4
#define STORAGE_USBDISK_CONNECTED 0x5
#endif

#define AUTO_DEMO_TIMEOUT_0       20

#ifdef SIMULATOR
#undef USE_RTC_HW
#else
#ifdef USE_RTC_HW
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;
#else
static volatile long lastUs;
extern uint32_t SystemCoreClock;
static int freqMHz;
#endif
#endif /* SIMULATOR */

#ifndef SIMULATOR
int BSP_JunctionTemp_Read();
#endif

#ifndef USE_RTC_HW
static long milliseconds = 0;
#endif //!USE_RTC_HW

Model::Model() :
    modelListener(0),
    mcuLoadActive(true),
    selectedDemoScreen(Defines::AUDIO_PLAYER),
    previousSelectedMenuType(Defines::ANIMATING_BUTTONS_MENU),
    scanJunctionTemp(false),
    tickCounter(0),
    timeCounter(0),
    volume(40),
    brightnessValue(100),
    lastBrightnessValue(100),
    dateYear(0),
    dateMonth(1),
    dateDay(23),
    counterActive(false),
    currentScreen(Defines::MAIN_MENU_SCREEN),
    autoDemoDone(false),
    firstAutoDemoScreen(selectedDemoScreen),
    autoDemoTimeOutMs(AUTO_DEMO_TIMEOUT_0)
{
    if(autoDemoTimeOutMs == 0)
    {
      autoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
    }
    currentTime.hours = 10;
    currentTime.minutes = 15;
    currentTime.seconds = 38;
    currentTime.milliseconds = 0;

#ifdef USE_RTC_HW
    RTC_GetTime(&RTC_Time);
    RTC_GetDate(&RTC_Date);

    currentTime.hours   = RTC_Time.Hours;
    currentTime.minutes = RTC_Time.Minutes;
    currentTime.seconds = RTC_Time.Seconds;

    dateYear = RTC_Date.Year;
    dateYear -= DatePicker::START_YEAR;
    if (dateYear < 0)
        dateYear = 0;
    dateMonth = RTC_Date.Month;
    dateMonth--;
    dateDay = RTC_Date.Date;
#else
#ifndef SIMULATOR
    lastUs = touchgfx::HAL::getInstance()->getCPUCycles();
    freqMHz = SystemCoreClock / 1000000;
#endif
    milliseconds  = currentTime.hours * 1000 * 60 * 60;
    milliseconds += currentTime.minutes * 1000 * 60;
    milliseconds += currentTime.seconds * 1000;
#endif

#ifndef SIMULATOR
    GUITaskQueue = xQueueGenericCreate(1, 1, 0);
    vQueueAddToRegistry(GUITaskQueue, "GUI_Queue");
#endif
}

void Model::setDemoSoftwareFlags(bool active)
{

}

static uint8_t mcuLoadLast = 0;

void Model::tick()
{
    tickCounter++;

    lastSecondValue = currentTime.seconds;

    uint8_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();
    if (mcuLoadLast != /*mcu_load_pct*/ mcuLoadPct)
    {
        mcuLoadLast = mcuLoadPct;
        modelListener->mcuLoadUpdated(mcuLoadLast);
    }

#ifndef SIMULATOR
    uint8_t msg = 0;
    if (xQueueReceive(GUITaskQueue, &msg, 0) == pdTRUE)
    {
        if (msg == AUDIO_EOF_NOTIFICATION)
        {
            if (modelListener)
            {
                modelListener->notifyAudioEndOfFile();
            }
        }
        else if (msg == STORAGE_MSDDISK_DISCONNECTED)
        {
            if (modelListener)
            {
                modelListener->notifyMediaDisconnected();
            }
        }
        else if (msg == STORAGE_MSDDISK_CONNECTED)
        {
            if (modelListener)
            {
                modelListener->notifyMediaConnected();
            }
        }
    }
#endif /* SIMULATOR */

#ifdef USE_RTC_HW
    if ((tickCounter % 20) == 0)
    {
        RTC_GetTime(&RTC_Time);
        RTC_GetDate(&RTC_Date);

        currentTime.hours   = RTC_Time.Hours;
        currentTime.minutes = RTC_Time.Minutes;
        currentTime.seconds = RTC_Time.Seconds;
        modelListener->updateTime(RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);

        dateYear = RTC_Date.Year;
        dateYear -= DatePicker::START_YEAR;
        if (dateYear < 0)
            dateYear = 0;
        dateMonth = RTC_Date.Month;
        dateMonth--;
        dateDay = RTC_Date.Date;
    }
#else
#ifndef SIMULATOR
    long now = touchgfx::HAL::getInstance()->getCPUCycles();
    milliseconds += 2 * (now - lastUs + freqMHz / 2) / freqMHz / 1000;
    lastUs = now;
#else
    milliseconds += 20; /* tick duration should be less than 20 ms */
#endif /* SIMULATOR */
    currentTime.hours = (milliseconds / 1000 / 60 / 60) % 24;
    currentTime.minutes = (milliseconds / 1000 / 60) % 60;
    currentTime.seconds = (milliseconds / 1000) % 60;
    currentTime.milliseconds = milliseconds % 1000;
    if ((tickCounter % 20) == 0)
    {
        modelListener->updateTime(currentTime.hours, currentTime.minutes, currentTime.seconds);
    }
#endif

    if ((tickCounter % 20) == 0)
    {
        if ((modelListener != 0) && (scanPotentiometer))
        {
            modelListener->newPotentiometerValue(getPotentiometerValue());
        }
    }

    if (counterActive)
    {
        timeCounter++;

        if ((timeCounter % TICK_CNT) == 0)
        {
            modelListener->oneSecond();
            timeCounter = 0;
        }
        modelListener->presenterTick();
    }
}

void Model::screenSaverMinorTick()
{
    modelListener->screenSaverMinorTick();
}

void Model::screenSaverMajorTick()
{
    modelListener->screenSaverMajorTick();
}

int Model::getTempValue()
{
#ifndef SIMULATOR
    return 0;
#else
    return currentTime.getValueInSeconds() % 127;
#endif
}

int Model::getPotentiometerValue()
{
#ifndef SIMULATOR
    return BSP_POT_GetLevel(POT1);
#else
    return currentTime.getValueInSeconds() % 100;
#endif
}

void Model::setBrightnessValue(uint8_t value)
{
    uint8_t minCutoff = 20;
    uint8_t newValue = minCutoff + (((100 - minCutoff) * value) / 100);
    if (newValue != lastBrightnessValue)
    {
        lastBrightnessValue = newValue;
#ifndef SIMULATOR
        BSP_LCD_SetBrightness(0, newValue);
#else
        printf("New brightness: %d\r\n", newValue);
#endif
    }
}

uint8_t Model::getBrightnessValue()
{
    return brightnessValue;
}

void Model::notifyAudioEndOfFile()
{
    if (modelListener)
    {
        modelListener->notifyAudioEndOfFile();
    }
}

#ifndef SIMULATOR
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
    AUDIOPLAYER_Stop();
    uint8_t msg = AUDIO_EOF_NOTIFICATION;
    xQueueSend(GUITaskQueue, &msg, 0);
    return AUDIOPLAYER_ERROR_NONE;
}

void  STORAGE_NotifyConnectionChange(uint8_t unit, uint8_t state)
{
    uint8_t msg = STORAGE_MSDDISK_DISCONNECTED;
    if((state == MSDDISK_CONNECTION_EVENT) || (state == USBDISK_CONNECTION_EVENT))
    {
        msg = STORAGE_MSDDISK_CONNECTED;
    }
    xQueueSend(GUITaskQueue, &msg, 0);
}
#endif

void Model::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
#ifdef USE_RTC_HW
    RTC_TimeTypeDef cRTC_Time;

    cRTC_Time.Hours = hour;
    cRTC_Time.Minutes = minute;
    cRTC_Time.Seconds = second;
    RTC_SetTime(&cRTC_Time);
#else
    milliseconds  = hour * 1000 * 60 * 60;
    milliseconds += minute * 1000 * 60;
    milliseconds += second * 1000;
#endif //USE_RTC_HW
}

void Model::setDate(uint16_t year, uint8_t month, uint8_t day)
{
    dateYear = year; 
    dateMonth = month; 
    dateDay = day; 
#ifdef USE_RTC_HW
    RTC_DateTypeDef date;
    date.Year = year + DatePicker::START_YEAR;
    date.Month = month+1;
    date.Date = day;
    date.WeekDay = 1;
    RTC_SetDate(&date);
#endif // USE_RTC_HW
}

void Model::setCounterActive(bool active)
{
    timeCounter = 0;
    counterActive = active;
    if(active)
    {
      firstAutoDemoScreen = selectedDemoScreen;
      autoDemoTimeOutMs = 5;
    }
    else
    {
      autoDemoTimeOutMs = 10;
    }
    setDemoSoftwareFlags(active);
}

bool Model::getCounterActive()
{
    return counterActive;
}

void Model::setCurrentScreen(Defines::Screens screen)
{
     currentScreen = screen;
}

Defines::Screens Model::getCurrentScreen()
{
    return currentScreen;
}
