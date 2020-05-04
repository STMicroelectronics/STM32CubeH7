#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>


#include "stm32h7b3i_eval.h"
#include "bsp.h"

Model::Model() :
    modelListener(0),
    mcuLoadActive(true),
    selectedDemoScreen(Defines::GAME2D),
    previousSelectedMenuType(Defines::ANIMATING_BUTTONS_MENU),
    scanPotentiometer(false),
    tickCounter(0),
    brightnessValue(100),
    lastBrightnessValue(100),
    dateYear(4),
    dateMonth(0),
    dateDay(0)

{
}
static uint8_t mcuLoadLast = 0;

void Model::tick()
{
    tickCounter++;

    uint8_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();
    if (mcuLoadLast != mcuLoadPct)
    {
        mcuLoadLast = mcuLoadPct;
        modelListener->mcuLoadUpdated(mcuLoadLast);
    }

    if ((modelListener != 0) && (scanPotentiometer))
    {
        modelListener->newPotentiometerValue(getPotentiometerValue());
    }

    if ((tickCounter % 60) == 0)
    {   currentTime.seconds++;
        currentTime.minutes += currentTime.seconds/60;
        currentTime.hours+= currentTime.minutes/60;
        currentTime.seconds%= 60;
        currentTime.minutes%= 60;
        currentTime.hours%= 24;
        modelListener->updateTime(currentTime.hours, currentTime.minutes, currentTime.seconds);
        tickCounter = 0;
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

int Model::getPotentiometerValue()
{
    return  BSP_POT_GetLevel(POT1);
}

void Model::setBrightnessValue(uint8_t value)
{
  if (value != lastBrightnessValue)
  {
    lastBrightnessValue = value;
    if(lastBrightnessValue < 5)
    {
      lastBrightnessValue = 5;
    }
    BSP_SetBrightness(lastBrightnessValue);
    touchgfx_printf("New brightness: %d\r\n", lastBrightnessValue);
  }
}

uint8_t Model::getBrightnessValue()
{
    return brightnessValue;
}

void Model::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{

    currentTime.hours = hour;
    currentTime.minutes = minute;
    currentTime.seconds = second;
}

void Model::setDate(uint16_t year, uint8_t month, uint8_t day)
{
    dateYear = year;
    dateMonth = month;
    dateDay = day;
}
