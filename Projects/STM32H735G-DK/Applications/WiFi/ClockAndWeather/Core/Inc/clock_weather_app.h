/**
******************************************************************************
* @file    clock_weather_app.h
* @author  MCD Application Team
* @brief   header for clock_weather_app.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLOCK_WEATHER_APP_H
#define __CLOCK_WEATHER_APP_H
#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "net_connect.h"
#include "ini.h"
#include "net_internals.h"
#include <stdlib.h>
#include <time.h>
  
/* Exported types ------------------------------------------------------------*/
typedef enum {    
  UPDATE_WIFI_AP = 0,
  UPDATE_WIFI_AP_DONE,
  CONNECTING_TO_WIFI,
  CONNECTING_TO_WIFI_OK,
  CONNECTING_TO_WIFI_ERROR,
  GET_DATE_AND_TIME,
  GET_DATE_AND_TIME_DONE,
  GET_DATE_AND_TIME_ERROR,
  GET_DATE_AND_TIME_TIMEOUT,
  GET_DATE_AND_TIME_ONLY,
  GET_DATE_AND_TIME_ONLY_DONE,
  GET_WEATHER_FORECAST_TUNIS_C,
  GET_WEATHER_FORECAST_TUNIS_C_DONE,
  GET_WEATHER_FORECAST_TUNIS_C_ERROR,
  GET_WEATHER_FORECAST_TUNIS_F,
  GET_WEATHER_FORECAST_TUNIS_F_DONE,
  GET_WEATHER_FORECAST_TUNIS_F_ERROR,
  GET_WEATHER_FORECAST_PARIS_C,
  GET_WEATHER_FORECAST_PARIS_C_DONE,
  GET_WEATHER_FORECAST_PARIS_C_ERROR,
  GET_WEATHER_FORECAST_PARIS_F,
  GET_WEATHER_FORECAST_PARIS_F_DONE,
  GET_WEATHER_FORECAST_PARIS_F_ERROR,
  GET_WEATHER_FORECAST_NEW_YORK_C,
  GET_WEATHER_FORECAST_NEW_YORK_C_DONE,
  GET_WEATHER_FORECAST_NEW_YORK_C_ERROR,
  GET_WEATHER_FORECAST_NEW_YORK_F,
  GET_WEATHER_FORECAST_NEW_YORK_F_DONE,
  GET_WEATHER_FORECAST_NEW_YORK_F_ERROR,    
  GET_WEATHER_FORECAST_CUSTOM_CITY_1_C,
  GET_WEATHER_FORECAST_CUSTOM_CITY_1_C_DONE,
  GET_WEATHER_FORECAST_CUSTOM_CITY_1_C_ERROR,
  GET_WEATHER_FORECAST_CUSTOM_CITY_1_F,
  GET_WEATHER_FORECAST_CUSTOM_CITY_1_F_DONE,
  GET_WEATHER_FORECAST_CUSTOM_CITY_1_F_ERROR,    
  GET_WEATHER_FORECAST_CUSTOM_CITY_2_C,
  GET_WEATHER_FORECAST_CUSTOM_CITY_2_C_DONE,
  GET_WEATHER_FORECAST_CUSTOM_CITY_2_C_ERROR,
  GET_WEATHER_FORECAST_CUSTOM_CITY_2_F,
  GET_WEATHER_FORECAST_CUSTOM_CITY_2_F_DONE,
  GET_WEATHER_FORECAST_CUSTOM_CITY_2_F_ERROR,    
  GET_WEATHER_FORECAST_CUSTOM_CITY_3_C,
  GET_WEATHER_FORECAST_CUSTOM_CITY_3_C_DONE,
  GET_WEATHER_FORECAST_CUSTOM_CITY_3_C_ERROR,
  GET_WEATHER_FORECAST_CUSTOM_CITY_3_F,
  GET_WEATHER_FORECAST_CUSTOM_CITY_3_F_DONE,
  GET_WEATHER_FORECAST_CUSTOM_CITY_3_F_ERROR,    
  JSON_DATA_ERROR,    
}CLOCK_AND_WEATHER_APP_STATUS;
   
typedef struct cw_config_s {
  char               key[128];
  char               location[3][16];
  int                citynbr;
}cw_config_t;
  
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/ 
#define STATE_TRANSITION_TIMEOUT                10000
#define SSID1                                   "TUN_MCD_MW_01"
#define PASSWORD                                "DA531GKL+43RV"
#define PORT_NUMBER                             80
#define BSP_SD_INSTANCE                         0U
#define CONFIG_FILE                             "cw_cfg.ini"
#define MAX_LISTED_AP                           10

#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV                       0x7F
#define RTC_SYNCH_PREDIV                        0xF9
#endif
  
#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV                       0x7F
#define RTC_SYNCH_PREDIV                        0x00FF
#endif 
  
#define RETRY_REQUEST_NUMBER                    10

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
  
void WIFI_Init(void);
int NET_Init(void);
int32_t  WIFI_Get_Access_Points(net_wifi_scan_results_t *APs);
int8_t WIFI_Connect(char *ssid, uint8_t *password, int32_t encryption );
char* get_location(uint8_t location);
int Get_Json_Weather_Forecast(char* City, const char* metric, char** jsondata);
int Get_Date_Time(char* date_time);
int RTC_Init(void);
int Set_RTC(char* currentTime);
int SD_Load_Custom_Cities(void);
void CalendarUpdate(char* currentTime);
int custom_cities_nbr(void);
char* CityName(int City);
char* getUTC_Date_Time(void);
char* getCity_Date_Time(int32_t timezone);
  
#ifdef __cplusplus
}
#endif
#endif /* __CLOCK_WEATHER_APP_H */
