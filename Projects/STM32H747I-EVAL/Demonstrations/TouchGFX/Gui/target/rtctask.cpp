#include <rtctask.hpp>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "rtc_app.h"

//*******************************************************
//   Define Queue handles
//*******************************************************

xQueueHandle rtc_msg_q = 0;
extern xQueueHandle gui_msg_q;

struct RTCMessage
{
    RTC_TimeTypeDef RTC_Time;
    RTC_DateTypeDef RTC_Date;
};

void RTCTask(void* params)
{
  rtc_msg_q = xQueueCreate( 1, sizeof( struct RTCMessage * ) );
  RTCMessage msg;
  
  for(;;)
  {
    if(xQueueReceive(rtc_msg_q, &msg, portMAX_DELAY) == pdTRUE)
    {
      uint8_t rtcMsg = 0;
        
      RTC_SetTime(&msg.RTC_Time);
      RTC_SetDate(&msg.RTC_Date);
      
      xQueueSend(gui_msg_q, &rtcMsg, 0);
    }
  }
}
