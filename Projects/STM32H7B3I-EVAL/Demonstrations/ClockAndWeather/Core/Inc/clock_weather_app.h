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
   
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/ 
#define STATE_TRANSITION_TIMEOUT                10000
#define PORT_NUMBER                             80
#define BSP_SD_INSTANCE                         0U
#define CONFIG_FILE                             "cw_cfg.ini"  
#define RETRY_REQUEST_NUMBER                    10
#define MAX_LISTED_AP                           10
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
    GET_WEATHER_FORECAST_PARIS_C,
    GET_WEATHER_FORECAST_PARIS_C_DONE,
    GET_WEATHER_FORECAST_PARIS_C_ERROR,
    GET_WEATHER_FORECAST_PARIS_F,
    GET_WEATHER_FORECAST_PARIS_F_DONE,
    GET_WEATHER_FORECAST_PARIS_F_ERROR,
    GET_WEATHER_FORECAST_CUSTOM_CITY_1_C,
    GET_WEATHER_FORECAST_CUSTOM_CITY_1_C_DONE,
    GET_WEATHER_FORECAST_CUSTOM_CITY_1_C_ERROR,
    GET_WEATHER_FORECAST_CUSTOM_CITY_1_F,
    GET_WEATHER_FORECAST_CUSTOM_CITY_1_F_DONE,
    GET_WEATHER_FORECAST_CUSTOM_CITY_1_F_ERROR,
    JSON_DATA_ERROR,   
}CLOCK_AND_WEATHER_APP_STATUS;
  
typedef struct cw_config_s {
  char               key[128];
  char               location[3][16];
  int                citynbr;
}cw_config_t;
  
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


