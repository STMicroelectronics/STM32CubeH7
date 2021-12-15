

#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/settings_screen/DatePicker.hpp>
#include <touchgfx/hal/HAL.hpp>

#ifndef SIMULATOR
#include "audio_player_app.h"
#include "storage_app.h"
#include "FreeRTOS.h"
#include "rtc_app.h"
#include "bsp.h"
#include <rtctask.hpp>

xQueueHandle GUITaskQueue = 0;

extern xQueueHandle rtc_msg_q; //From RTCTask
xQueueHandle gui_msg_q;

#define AUDIO_EOF_NOTIFICATION 0x1
#define STORAGE_USB_DISCONNECTED 0x2
#define STORAGE_USB_CONNECTED 0x3
#define STORAGE_MSD_DISCONNECTED 0x4
#define STORAGE_MSD_CONNECTED 0x5
#endif

#ifdef SIMULATOR
#include <ctime>
#ifndef _MSC_VER
#include <sys/time.h>
#endif /* _MSC_VER*/
#else
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;

struct RTCMessage
{
    RTC_TimeTypeDef RTC_Time;
    RTC_DateTypeDef RTC_Date;
};

extern uint32_t SystemCoreClock;
#endif /* SIMULATOR */

Model::Model() :
    modelListener(0),
    mcuLoadActive(true),
    selectedDemoScreen(Defines::AUDIO_PLAYER),
    previousSelectedMenuType(Defines::ANIMATING_BUTTONS_MENU),
#ifdef USE_STM32H747I_DISCO
    scanJunctionTemp(false),
#else
    scanPotentiometer(false),
#endif
    tickCounter(0),
    timeCounter(0),
    volume(40),
    brightnessValue(100),
    lastBrightnessValue(100),
    dateYear(0),
    dateMonth(1),
    dateDay(23),
    updateTimeDate(false),
    counterActive(false),
    currentScreen(Defines::MAIN_MENU_SCREEN),
    autoDemoDone(false),
    firstAutoDemoScreen(selectedDemoScreen),
#ifndef SIMULATOR
    autoDemoTimeOutMs(AUTO_DEMO_TIMEOUT_0)
#else
    autoDemoTimeOutMs(20)
#endif
{
#ifndef SIMULATOR    
    
    gui_msg_q = xQueueCreate( 1, sizeof( struct RTCMessage * ) );
  
    RTC_GetTime(&RTC_Time);
    RTC_GetDate(&RTC_Date);

    dateYear = RTC_Date.Year - ((DatePicker::START_YEAR)%100);
    if (dateYear > 11)
        dateYear = 0;
    dateMonth = RTC_Date.Month - 1;
    dateDay = RTC_Date.Date - 1;

    HAL_PWR_EnableBkUpAccess();
    uint32_t BckRegValue = READ_REG(BKP_REG_SW_CFG);
    autoDemoId = (((BckRegValue & 0x0000FF00) >>  8) & 0xFF);
    autoDemoTimeOutMs = (((BckRegValue & 0xFFFF0000) >> 16) & 0xFFFF);
    if(autoDemoTimeOutMs == 0)
    {
      autoDemoTimeOutMs = AUTO_DEMO_TIMEOUT_0;
    }
    if(autoDemoId)
    {
      /* Set SW Reset value to 2 : Pressing SW Reset button will do SW reset with this value */
      WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x2));
    }
    else
    {
      WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x1));
    }
    HAL_PWR_DisableBkUpAccess();
#endif // SIMULATOR
}

#ifndef SIMULATOR
//extern volatile uint32_t mcu_load_pct;
static uint8_t mcuLoadLast = 0;
#endif // SIMULATOR

void Model::setDemoSoftwareFlags(bool active)
{
#ifndef SIMULATOR
  uint8_t SofwareReset = 0x1;
  if(active)
  {
    SofwareReset = 0x2;
  }
  HAL_PWR_EnableBkUpAccess();
  uint32_t BckRegValue = READ_REG(BKP_REG_SW_CFG);
  WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | SofwareReset));
  HAL_PWR_DisableBkUpAccess();
#endif // SIMULATOR
}

void Model::setSelectedDemoScreen(Defines::DemoID demoId)
{
  selectedDemoScreen = demoId;
#ifndef SIMULATOR
  if ( counterActive && autoDemoDone )
  {
    /* Set SW Reset to 1 */
    HAL_PWR_EnableBkUpAccess();
    uint32_t BckRegValue = READ_REG(BKP_REG_SW_CFG);
    WRITE_REG(BKP_REG_SW_CFG, ((BckRegValue & 0xFFFFFF00) | 0x1));
    HAL_PWR_DisableBkUpAccess();
    /* Return back to ML */
    NVIC_SystemReset();
  }
#endif /* SIMULATOR */
}

void Model::tick()
{
    tickCounter++;

    lastSecondValue = currentTime.seconds;
#ifdef SIMULATOR
#ifdef _MSC_VER
    time_t rawtime;
    struct tm timenow;
    time(&rawtime);
    localtime_s(&timenow, &rawtime);

    currentTime.hours =   timenow.tm_hour;
    currentTime.minutes = timenow.tm_min;
    currentTime.seconds = timenow.tm_sec;
    currentTime.milliseconds = 0;

#else
    timeval timenow;
    gettimeofday(&timenow, NULL);

    currentTime.hours = (timenow.tv_sec / 60 / 60) % 24;
    currentTime.minutes = (timenow.tv_sec / 60) % 60;
    currentTime.seconds = timenow.tv_sec % 60;
    currentTime.milliseconds = timenow.tv_usec / 1000;
#endif  /*_MSC_VER*/
#else

    uint8_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();
    if (mcuLoadLast != /*mcu_load_pct*/ mcuLoadPct)
    {
        mcuLoadLast = mcuLoadPct;
        modelListener->mcuLoadUpdated(mcuLoadLast);
    }

    uint8_t msg = 0;
    if (xQueueReceive(GUITaskQueue, &msg, 0) == pdPASS)
    {
        if (msg == AUDIO_EOF_NOTIFICATION)
        {
            if (modelListener)
            {
                modelListener->notifyAudioEndOfFile();
            }
        }

        if ((msg == STORAGE_USB_DISCONNECTED)
         || (msg == STORAGE_MSD_DISCONNECTED))
        {
            if (modelListener)
            {
                modelListener->notifyMediaDisconnected();
            }
        }

        if ((msg == STORAGE_USB_CONNECTED)
         || (msg == STORAGE_MSD_CONNECTED))
        {
            if (modelListener)
            {
                modelListener->notifyMediaConnected();
            }
        }
    }

    if ((tickCounter % TICK_CNT) == 0)
    {
      static uint8_t prev_usb_disk_status = 0;
      uint8_t has_sd_card = STORAGE_GetStatus(MSD_DISK_UNIT);
      if(prev_usb_disk_status != has_sd_card)
      {
        prev_usb_disk_status = has_sd_card;
        if(prev_usb_disk_status == 0)
        {
          modelListener->notifyMediaDisconnected();
        }
        else
        {
          modelListener->notifyMediaConnected();
        }
      }
    }
#endif /* SIMULATOR */    

#ifdef USE_STM32H747I_EVAL
    if ((modelListener != 0) && (scanPotentiometer))
    {
        modelListener->newPotentiometerValue(getPotentiometerValue());
    }
#endif

    if ((tickCounter % TICK_CNT) == 0)
    {
#ifdef USE_STM32H747I_DISCO
      if ((modelListener != 0) && (scanJunctionTemp))
      {
        modelListener->newJunctionTempValue(getTempValue());
      }
#endif

#ifndef SIMULATOR

      uint8_t rtcMsg = 0;
      if (xQueueReceive(gui_msg_q, &rtcMsg, 0) == pdPASS)
      {
        updateTimeDate = rtcMsg;
      }

      if(updateTimeDate)
      {
        RTC_GetTime(&RTC_Time);
        RTC_GetDate(&RTC_Date);

        modelListener->updateTime(RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);

        dateYear = RTC_Date.Year - ((DatePicker::START_YEAR)%100);
        dateMonth = RTC_Date.Month - 1;
        dateDay = RTC_Date.Date - 1;
        if (dateYear > 11)
          dateYear = 0;
      }
#else
       modelListener->updateTime(currentTime.hours, currentTime.minutes, currentTime.seconds);
#endif
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

#ifndef SIMULATOR
#include "stm32h7xx_hal.h"
#ifdef USE_STM32H747I_DISCO
int BSP_JunctionTemp_Read();
#include "stm32h747i_discovery.h"
#else
#include "stm32h747i_eval.h"
#endif
#else
#include <stdio.h>
#endif

#ifdef USE_STM32H747I_DISCO
#ifndef SIMULATOR
extern uint32_t TEMP_SENSOR_Start(void);
extern uint32_t TEMP_SENSOR_Stop(void);
extern uint32_t TEMP_SENSOR_GetValue(void);
#endif

void Model::setScanJunctionTemp(bool scanEnabled)
{
#ifndef SIMULATOR
  uint32_t ret = HAL_OK;

  if(scanJunctionTemp != scanEnabled)
  {
    if(scanEnabled)
    {
      ret = TEMP_SENSOR_Start();
    }
    else
    {
      ret = TEMP_SENSOR_Stop();
    }

    if(ret == HAL_OK)
    {
      scanJunctionTemp = scanEnabled;
    }
  }
#else
  if (scanJunctionTemp != scanEnabled)
  {
	scanJunctionTemp = scanEnabled;
  }
#endif
}

int Model::getTempValue()
{
#ifndef SIMULATOR
    return TEMP_SENSOR_GetValue();
#else
    return currentTime.getValueInSeconds() % 127;
#endif
}
#endif

#ifdef USE_STM32H747I_EVAL
int Model::getPotentiometerValue()
{
#ifndef SIMULATOR
    return BSP_POT_GetLevel(POT1) >> 5;
#else
    return currentTime.getValueInSeconds() % 127;
#endif
}
#endif


#ifndef SIMULATOR
extern "C" void LCD_SetBrightness(int value);
#endif

void Model::setBrightnessValue(uint8_t value)
{
    uint8_t minCutoff = 20;
    uint8_t newValue = minCutoff + (((100 - minCutoff) * value) / 100);
    if (newValue != lastBrightnessValue)
    {
        lastBrightnessValue = newValue;
#ifndef SIMULATOR
        LCD_SetBrightness(newValue);
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

/**
  * @brief  Notify Storage Unit connection state.
  * @retval None
  */
void  STORAGE_NotifyConnectionChange(uint8_t unit, uint8_t state)
{
    uint8_t msg = 0;
    if (unit == USB_DISK_UNIT)
    {
        msg = state == 1 ? STORAGE_USB_CONNECTED : STORAGE_USB_DISCONNECTED;
    }
    else
    {
        msg = state == 1 ? STORAGE_MSD_CONNECTED : STORAGE_MSD_DISCONNECTED;
    }
    xQueueSend(GUITaskQueue, &msg, 0);
}
#endif

void Model::setTimeDate(uint8_t hour, uint8_t minute, uint8_t second, uint16_t year, uint8_t month, uint8_t day)
{
  dateYear = year;
  dateMonth = month; 
  dateDay = day; 
#ifndef SIMULATOR
  
  updateTimeDate = false;
  
  RTCMessage rtcMsg;
  RTC_TimeTypeDef cRTC_Time;
  RTC_DateTypeDef date;
  
  cRTC_Time.Hours = hour;
  cRTC_Time.Minutes = minute;
  cRTC_Time.Seconds = second; 
  
  date.Year = year;
  date.Month = month+1;
  date.Date = day+1;
  date.WeekDay = 1;
  
  rtcMsg.RTC_Time = cRTC_Time;
  rtcMsg.RTC_Date = date;
  
  xQueueSend(rtc_msg_q, &rtcMsg, 0);
#endif  
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
