/**
******************************************************************************
* @file    clock_weather_app.c
* @author  MCD Application Team
* @brief   This file provides the clock and weather APIs
******************************************************************************
* @attention
*
* Copyright (c) 2019 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "clock_weather_app.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char METRIC[] = "metric";
const char IMPERIAL[] = "imperial";
const char *open_weather_map_keys[] =   { "5b49eb541b132bf109874f38e3e706eb",
                                          "3893bc654a09861c5af847bee2f70cfc",
                                          "a18cc2b301cf0c47486d01db78b7c9ca",
                                          "1c995fdbfdeea62f4047de1e8ade53d5",
                                          "d8d752c68659f6755b591d585a55b2bb",
                                          "ca2ff40e790dc2b18f6abbce437c893d",
                                          "873c46bdba2f608b76f59b87fdff043c",
                                          "1ecbc3ad4479e5322ea255790ac25205",
                                          "f3b0ce4321dd1c5ed88bdf8a0a5cf155",
					  "2ac66a005a35f3820bbe68365bdba37e",
                                          "bc69d11217caf11d676cceabfd1a046a",
                                          "e04a001c0f78e4fac0f474e91b2936ff",
					  "66b53307a7cbded0e503a14055b5d52e",
                                          "7d20bf295f600f3a6f24ac1175db9431" };

extern osMessageQId GUIEvent;
extern uint8_t password_ascii[64];
extern uint8_t ssid_ascii[32]; 
extern uint8_t encryption_ascii[30];
extern RTC_HandleTypeDef RtcHandle;
extern RNG_HandleTypeDef RngHandle;
extern int32_t es_wifi_driver(net_if_handle_t * pnetif);

char SDPath[4];
FATFS SDFatFs;
cw_config_t default_cfg;

net_if_handle_t netif;
net_event_handler_t net_handler;
net_if_driver_init_func es_wifi_driver_ptr = &es_wifi_driver;
net_wifi_scan_results_t APs[MAX_LISTED_AP];
osMessageQId ClockAndWeatherEvent = 0;


uint8_t HTTP_Response[20 * 1024];
char_t  WEATHER_SERVER [] = "api.openweathermap.org";

static char UTC_Date_Time[64] = "2000-01-01 00:00:00";
static uint32_t osKernelSysTick_Start = 0;

char* jsondata;
char  City_Date_Time_Buff[80];
char day_of_week[2]= {0};
int unixtime = 1546560000; /*2019-01-04 00:00:00*/
uint8_t retry_request = 0;
/* Private function prototypes -----------------------------------------------*/
extern int extractDateTime(char *json_string, char* date_time, char* day_of_week, int *unixtime);
static void Clock_And_Weather_Thread(void const *argument);
static int config_reader(void* user, const char* section, const char* name,
                           const char* value);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes module wifi network interface
  * @param  none
  * @retval none
  */
void WIFI_Init(void)
{
  NET_Init();
  
  /* Clock and Weather Queue */
  osMessageQDef(Clock_And_Weather_Queue, 10, uint16_t);
  ClockAndWeatherEvent = osMessageCreate (osMessageQ(Clock_And_Weather_Queue), NULL);
  
  /* Clock and Weather Thread  definition */
  osThreadDef(CLOCK_AND_WEATHER_THREAD, Clock_And_Weather_Thread, osPriorityNormal, 0, 4*configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(CLOCK_AND_WEATHER_THREAD), NULL);
  
}

/**
  * @brief  Clock and weather thread
  * @param  argument: pointer that is passed to the thread function as start argument
  * @retval none
  */
static void Clock_And_Weather_Thread(void const *argument)
{
  osEvent event;
  int ret = 0;
  for (;;)
  {
    event = osMessageGet(ClockAndWeatherEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      switch (event.value.v)
      {
        
      case UPDATE_WIFI_AP:
        memset(&APs, 0, sizeof(APs));
        
        if( WIFI_Get_Access_Points(&APs[0]) == NET_OK)
        {
          osMessagePut ( GUIEvent, UPDATE_WIFI_AP_DONE , 0);
        }
        break;
        
      case CONNECTING_TO_WIFI:
         if (WIFI_Connect((char*) ssid_ascii, password_ascii, net_wifi_string_to_security((char*)encryption_ascii)) != NET_OK)
        {
          osMessagePut ( GUIEvent, CONNECTING_TO_WIFI_ERROR , 0);
        }
        else
        {
          osMessagePut ( GUIEvent, CONNECTING_TO_WIFI_OK , 0);
        }
        break;
        
      case GET_DATE_AND_TIME:
        if((ret = Get_Date_Time(UTC_Date_Time)) == 0)
        {
          Set_RTC(UTC_Date_Time);
          osMessagePut ( GUIEvent, GET_DATE_AND_TIME_DONE , 0);
        }
        else
        {
          osMessagePut ( GUIEvent, GET_DATE_AND_TIME_ERROR , 0);         
        }
        break;
        
      case GET_WEATHER_FORECAST_TUNIS_C:
        if((ret = Get_Json_Weather_Forecast("TUNIS", METRIC, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_TUNIS_C_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_TUNIS_C , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_TUNIS_C_ERROR , 0);
          }
        }
        break;
        
      case GET_WEATHER_FORECAST_TUNIS_F:
        if((ret = Get_Json_Weather_Forecast("TUNIS", IMPERIAL, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_TUNIS_F_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_TUNIS_F , 0);
          }
          else
          {
            osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_TUNIS_F_ERROR , 0);
          }
        }
        break;
      case GET_WEATHER_FORECAST_PARIS_C:
        if((ret = Get_Json_Weather_Forecast("PARIS", METRIC, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_PARIS_C_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_PARIS_C , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_PARIS_C_ERROR , 0);
          }
        }
        break;
        
      case GET_WEATHER_FORECAST_PARIS_F:
        if((ret = Get_Json_Weather_Forecast("PARIS", IMPERIAL, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_PARIS_F_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_PARIS_F , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_PARIS_F_ERROR , 0);
          }
        }
        break;
      case GET_WEATHER_FORECAST_NEW_YORK_C:
        if((ret = Get_Json_Weather_Forecast("NEW%20YORK", METRIC, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_NEW_YORK_C_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_NEW_YORK_C , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_NEW_YORK_C_ERROR , 0);
          }
        }
        break;
        
      case GET_WEATHER_FORECAST_NEW_YORK_F:
        if((ret = Get_Json_Weather_Forecast("NEW%20YORK", IMPERIAL, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_NEW_YORK_F_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_NEW_YORK_F , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_NEW_YORK_F_ERROR , 0);
          }
        }
        break;    

      case GET_WEATHER_FORECAST_CUSTOM_CITY_1_C:
        if((ret = Get_Json_Weather_Forecast(default_cfg.location[0], METRIC, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_1_C_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_1_C , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_1_C_ERROR , 0);
          }
        }
        break;
        
      case GET_WEATHER_FORECAST_CUSTOM_CITY_1_F:
        if((ret = Get_Json_Weather_Forecast(default_cfg.location[0], IMPERIAL, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_1_F_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_1_F , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_1_F_ERROR , 0);
          }
        }
        break;

      case GET_WEATHER_FORECAST_CUSTOM_CITY_2_C:
        if((ret = Get_Json_Weather_Forecast(default_cfg.location[1], METRIC, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_2_C_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_2_C , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_2_C_ERROR , 0);
          }
        }
        break;
        
      case GET_WEATHER_FORECAST_CUSTOM_CITY_2_F:
        if((ret = Get_Json_Weather_Forecast(default_cfg.location[1], IMPERIAL, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_2_F_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_2_F , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_2_F_ERROR , 0);
          }
        }
        break;

      case GET_WEATHER_FORECAST_CUSTOM_CITY_3_C:
        if((ret = Get_Json_Weather_Forecast(default_cfg.location[2], METRIC, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_3_C_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_3_C , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_3_C_ERROR , 0);
          }
        }
        break;
        
      case GET_WEATHER_FORECAST_CUSTOM_CITY_3_F:
        if((ret = Get_Json_Weather_Forecast(default_cfg.location[2], IMPERIAL, &jsondata)) == 0)
        {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_3_F_DONE , 0);
        }
        else
        {
          if(ret == JSON_DATA_ERROR)
          {
            osMessagePut ( ClockAndWeatherEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_3_F , 0);
          }
          else
          {
          osMessagePut ( GUIEvent, GET_WEATHER_FORECAST_CUSTOM_CITY_3_F_ERROR , 0);
          }
        }
        break;
      }
    }          
  }
}

/**
  * @brief  Network interface initialization
  * @param  none
  * @retval 0 in case of success, an error code otherwise
  */
int NET_Init(void)
{
  if (net_if_init(&netif, es_wifi_driver_ptr, &net_handler) != NET_OK)
  {
    return -1;
  }
  
  net_if_wait_state(&netif,NET_STATE_INITIALIZED,STATE_TRANSITION_TIMEOUT);
  if ( net_if_start (&netif) != NET_OK )
  {
    return -1;
  }
  
  net_if_wait_state(&netif,NET_STATE_READY,STATE_TRANSITION_TIMEOUT);
  
  return 0;
}

/**
  * @brief  Get available access points
  * @param  APs: pointer Access points structure
  * @retval ES Wifi status
  */
int32_t WIFI_Get_Access_Points(net_wifi_scan_results_t *APs)
{
  int32_t ret;
  ret = net_wifi_scan(&netif,NET_WIFI_SCAN_PASSIVE,NULL);
  if (ret == NET_OK)
  {
    ret = net_wifi_get_scan_results(&netif,APs,MAX_LISTED_AP);
    if (ret > 0) ret= NET_OK;
  }
  return ret;
}

/**

  * @brief  Connect network interface
  * @param  ssid: wifi network name
  * @param  password:  access point password
  * @param  encryption : security mode
  * @retval 0 in case of success, an error code otherwise
  */
int8_t WIFI_Connect(char *ssid, uint8_t *password, int32_t encryption )
{  
  net_wifi_credentials_t  Credentials = 
  {
    (char const*) ssid,
    (char const*)password,
    encryption
  };
  
  if (net_wifi_set_credentials(&netif, &Credentials) != NET_OK)
  {
    return -1;
  }
  
  if (netif.state == NET_STATE_CONNECTING )
  {
    netif.state = NET_STATE_CONNECTED ; 
  }
    
  if (netif.state == NET_STATE_CONNECTED ) 
  {
    if(net_if_disconnect(&netif)!= NET_OK)
    {
      return  -1;
    }
  }
  
  if(net_if_connect (&netif) != NET_OK)
  {
    return -1;
  }
  
  
  return NET_OK;
}

/**

  * @brief  Get weather forecast
  * @param  City: city name
  * @param  metric:  celsius or fahrenheit
  * @param  jsondata : pointer to forecast data
  * @retval 0 in case of success, an error code otherwise
  */
int Get_Json_Weather_Forecast(char* City, const char* metric, char** jsondata)
{
  int32_t sock = 0;
  sockaddr_in_t ServerIp;
  int length = 0, recv = 1;
  char* json_data_end;
  uint8_t FORECAST_Request[200] = {0};
  char ch = '{';
  int32_t ret = 0;
  
  /* Used for storing Random 32bit Number */
  uint32_t aRandom32bit[1];
  
  int lower = 0, upper = 13;
  HAL_RNG_GenerateRandomNumber(&RngHandle, &aRandom32bit[0]);
  
  int num = (aRandom32bit[0] % (upper - lower + 1)) + lower;
  
  memset((char *)&HTTP_Response, 0, sizeof(HTTP_Response));
  
  /*Create the HTTP request for weather Forecast*/  
   sprintf((char*)FORECAST_Request,"GET /data/2.5/forecast?q=%s&units=%s&APPID=%s HTTP/1.1\r\nHost: %s\r\n\r\n", City, 
          metric, open_weather_map_keys[num], WEATHER_SERVER);
  
  ServerIp.sin_len = sizeof(sockaddr_in_t);
  ret = net_if_gethostbyname(&netif, (sockaddr_t *)&ServerIp, WEATHER_SERVER);
  if(ret != NET_OK)
  {
    net_closesocket(sock);
    return ret;
  }
  
  ServerIp.sin_port = NET_HTONS(PORT_NUMBER); /* short, network byte order */
  
  if( (sock = net_socket(NET_AF_INET, NET_SOCK_STREAM ,NET_IPPROTO_TCP )) < 0)
  {
    net_closesocket(sock);
    return ret;
  }
  if( (ret = net_connect( sock, (sockaddr_t *)&ServerIp, sizeof(ServerIp) )) != NET_OK )
  {
    net_closesocket(sock);
    return ret;
  }
  
  if ((ret = net_send( sock, FORECAST_Request, sizeof( FORECAST_Request), 0)) < 0)
  {
    net_closesocket(sock);
    return ret;
  }
 
  while (recv > 0 )
  {
    recv = net_recv( sock, HTTP_Response + length , 1200, 0);
    length += recv;
  }
  net_closesocket(sock);
  
  /* Parse Forecast */
  *jsondata = strchr((char*)HTTP_Response, ch);
  /*Parse length content*/
  json_data_end = strstr((char*)HTTP_Response, "}}");
  if (json_data_end == 0x0)
  {
    return JSON_DATA_ERROR;
  }
  *(json_data_end + 2) = '\0';  
  return 0;
  
}

/**

  * @brief  Get date and time from server
  * @param  date_time: pointer to date and time data
  * @retval 0 in case of success, an error code otherwise
  */
int Get_Date_Time(char* date_time)
{
  int32_t sock = 0;
  sockaddr_in_t ServerIp; 
  char  *json_data;
  char ch = '{';
  char* tmp ;
  char curr_time[24];
  char current_date_time[64]={0};  int32_t ret = 0;
    
  uint8_t DATE_Request[] = "GET /api/timezone/EST HTTP/1.1\r\nHost: worldtimeapi.org\r\n\r\n";
  char  DATE_SERVER [] = "worldtimeapi.org";
  
  memset((char *)&HTTP_Response, 0, 800);
  
  /* Connect to server & send GET request */
  memset((char *)&ServerIp, 0, sizeof(ServerIp));
  
  if( (sock = net_socket(NET_AF_INET, NET_SOCK_STREAM ,NET_IPPROTO_TCP )) < 0)
  {
    net_closesocket(sock);
    return sock;
  }
  
  ServerIp.sin_len = sizeof(sockaddr_in_t);
  ret = net_if_gethostbyname(&netif, (sockaddr_t *)&ServerIp, (char_t*)DATE_SERVER);
  if(ret != NET_OK)
  {
    net_closesocket(sock);
    return ret;
  }
  ServerIp.sin_port = NET_HTONS(PORT_NUMBER);      /* short, network byte order */
  
  if( (ret = net_connect( sock, (sockaddr_t *)&ServerIp, sizeof(ServerIp) )) != NET_OK )
  {
    net_closesocket(sock);
    return ret;
  }
  
  if ((ret = net_send( sock, DATE_Request, sizeof( DATE_Request), 0)) < 0)
  {
    net_closesocket(sock);
    return ret;
  }
  
  if ((ret = net_recv( sock, HTTP_Response, sizeof( HTTP_Response ), 0)) < 0)
  {
    net_closesocket(sock);
    return ret;
  }
  /* JSON Parser DATE + TIME */  
  
  json_data = strchr((char*)HTTP_Response, ch);
  
  /*Extract date and curr_time*/
  if (extractDateTime(json_data, current_date_time, day_of_week, &unixtime) != 0)
  {
    net_closesocket(sock);
    return 1;
  }
  
  sprintf(date_time, "%s", current_date_time);
  
  osKernelSysTick_Start = osKernelSysTick();
  
  tmp = current_date_time;
  strcpy(curr_time, strchr(tmp, 'T') + 1);
  current_date_time[strlen(current_date_time) - strlen(curr_time) - 1] = '\0';
  
  date_time[10] = ' ';
  date_time[19] = '\0';
  
  net_closesocket(sock);
  return 0;
}

/**

  * @brief  Set RTC current date and time
  * @param  currentTime: pointer to current date and time
  * @retval 0 in case of success, an error code otherwise
  */
int Set_RTC(char* currentTime)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  
  int year, month, day, week_day;
  
  char temp[4];
  
  memcpy(temp, &currentTime[0], 4);
  temp[3] = '\0';                     
  year = strtol(temp, NULL, 16);
  
  memcpy(temp, &currentTime[5], 2);
  temp[2] = '\0';
  month = strtol(temp, NULL, 16);
  
  memcpy(temp, &currentTime[8], 2);
  day = strtol(temp, NULL, 16);
  
  week_day = strtol(day_of_week, NULL, 16);
  
  
  int hour, minute, seconds;
  
  memcpy(temp, &currentTime[11], 2);
  temp[2] = '\0';
  hour = strtol(temp, NULL, 16);
  
  memcpy(temp, &currentTime[14], 2);
  temp[2] = '\0';
  minute = strtol(temp, NULL, 16);
  
  memcpy(temp, &currentTime[17], 2);
  temp[2] = '\0';
  seconds = strtol(temp, NULL, 16);
  
  /* 1- Configure the Date */
  sdatestructure.Year    = year;
  sdatestructure.Month   = month;
  sdatestructure.Date    = day;
  sdatestructure.WeekDay = week_day;
  
  if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    return 1;
  }
  /* 2- Configure the Time */
  stimestructure.Hours          = hour;
  stimestructure.Minutes        = minute;
  stimestructure.Seconds        = seconds;
  stimestructure.SubSeconds     = 0x0;

  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    return 1;
  } 

  osKernelSysTick_Start =  HAL_GetTick();
  
  return 0;
}

/**
* @brief  load custom cities from SD card
* @param  none
* @retval 0 in case of success, an error code otherwise
*/
int SD_Load_Custom_Cities(void)
{
  int error;
  
  error = ini_parse(CONFIG_FILE, config_reader, NULL);
  
  if (error < 0)
  {
    return 1;
  } 
  return 0;
}

/**
* @brief  callabck when name found in config file
* @param  user: not used
* @param  section: not used
* @param  name: text string to locate from config file 
* @param  value: name value in config file
* @retval 0 in case of success, an error code otherwise
*/
static int config_reader(void* user, const char* section, const char* name,
                         const char* value)
{
  static int i = 0;
  
  if (i < 3)
  {
    if (strcmp(name, "key") == 0)
    {
      strcpy(default_cfg.key, value);
      return 1;
    }
    
    if (strcmp(name, "location") == 0)
    {
      strcpy(default_cfg.location[i], value);
      i++;
      default_cfg.citynbr = i;
      return 1;
    }
    
  }
  
  return 0;
}

/**
* @brief  Get custom cities number from config file
* @param  none
* @retval custom cities number
*/
int custom_cities_nbr(void)
{
  return default_cfg.citynbr;
}

/**
* @brief  Get City name from config file
* @param  City name
* @retval City name
*/
char* CityName(int City)
{
  return default_cfg.location[City];
}

/**
* @brief  Get Date and time from unixtime
* @param  timezone: time zone
* @retval City name
*/
char* getCity_Date_Time(int32_t timezone)
{
   time_t rawtime;
   struct tm  ts;
   
   rawtime = unixtime + timezone + ((HAL_GetTick() - osKernelSysTick_Start) / 1000 );
   
#if defined(__CC_ARM) /* !< ARM Compiler */
  localtime_r(&rawtime, &ts);
#else
  ts = *gmtime(&rawtime);
#endif
   
   strftime(City_Date_Time_Buff, sizeof(City_Date_Time_Buff), "%Y-%m-%d %H:%M:%S", &ts);
   
   return City_Date_Time_Buff;
}
