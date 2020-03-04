/**
******************************************************************************
* @file    clock_weather_app.h
* @author  MCD Application Team
* @brief   header for clock_weather_app.c file
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
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
#include "net_conf_es_wifi_spi.h"
#include "es_wifi.h"
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
ES_WIFI_Status_t WIFI_Get_Access_Points(ES_WIFI_APs_t *APs);
int8_t WIFI_Connect(char *ssid, uint8_t *password, uint16_t encryption );
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


