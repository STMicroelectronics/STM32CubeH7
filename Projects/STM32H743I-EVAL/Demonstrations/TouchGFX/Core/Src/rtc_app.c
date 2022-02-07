/**
  ******************************************************************************
  * @file    rtc_app.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    13-November-2015
  * @brief   This file provides the kernel rtc functions 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc_app.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_RTC
  * @brief Kernel rtc routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define DEMO_TGFX_SIGNATURE 0x5AA55AAA

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle;

/* To be used to WA RTC GetTime issue */
RTC_TimeTypeDef   RTC_TimeStructure = {0};
RTC_DateTypeDef   RTC_DateStructure = {0};

/* Private functions ---------------------------------------------------------*/
static void Set_DemoSignature( void )
{
  /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();

  RTC->BKP1R = DEMO_TGFX_SIGNATURE;

  /* Disable RTC back-up registers access */
  HAL_PWR_DisableBkUpAccess();
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_CalendarBkupInit(void)
{
  __HAL_RCC_RTC_CLK_ENABLE();

  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */ 
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    while(1);
  }

  /**
   * Bypass the shadow register
   */
  HAL_RTCEx_EnableBypassShadow(&RtcHandle);

  /* Disable Write Protection */
  __HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle) ;

#ifdef RTC_CLOCK_SOURCE_LSI
  /* Wait for LSI to be ready */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == 0);
#else
  /* Wait for LSE to be ready */
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == 0);
#endif

  Set_DemoSignature();
}

/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  * @param  hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Enables access to the backup domain ###################################*/
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTc clock source */
   /* Enable RTC back-up registers access */
  HAL_PWR_EnableBkUpAccess();

  /**
   *  Write twice the value to flush the APB-AHB bridge
   *  This bit shall be written in the register before writing the next one
   */
  HAL_PWR_EnableBkUpAccess();

  /*##-2- Configure LSI/LSE as RTC clock source ###################################*/
#ifdef RTC_CLOCK_SOURCE_LSI
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
#else
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
#endif
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    while(1);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
#ifdef RTC_CLOCK_SOURCE_LSI
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
#else
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
#endif
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    while(1);
  }

#ifndef RTC_CLOCK_SOURCE_LSI
  /* Configures the External Low Speed oscillator (LSE) drive capability */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
#endif
  
  /*##-2- Enable RTC peripheral Clocks #######################################*/ 
  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE(); 
}

/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param  hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct = {0};

  /*##-1- Configure LSI/LSE as RTC clock source ###################################*/
#ifdef RTC_CLOCK_SOURCE_LSI
  /* -a- Disable LSI Oscillator */
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSI_OFF;
#else
  /* -a- Disable LSE Oscillator */
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
#endif
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }

  __HAL_RCC_RTC_DISABLE();
}


/**
  * @brief  Backup save parameter 
  * @param  address: RTC Backup data Register number.
  *                  This parameter can be: RTC_BKP_DRx where x can be from 0 to 19 to 
  *                                         specify the register.
  * @param  Data:    Data to be written in the specified RTC Backup data register.
  * @retval None
  */
void RTC_BkupSaveParameter(uint32_t address, uint32_t data)
{
  HAL_RTCEx_BKUPWrite(&RtcHandle,address,data);  
}

/**
  * @brief  Backup restore parameter. 
  * @param  address: RTC Backup data Register number.
  *                  This parameter can be: RTC_BKP_DRx where x can be from 0 to 19 to 
  *                                         specify the register. 
  * @retval None
  */
uint32_t RTC_BkupRestoreParameter(uint32_t address)
{
   return HAL_RTCEx_BKUPRead(&RtcHandle,address);  
}

/**
  * @brief  RTC Get time. 
  * @param  Time: Pointer to Time structure
  * @retval None
  */
void RTC_GetTime(  RTC_TimeTypeDef *Time)
{
   HAL_RTC_GetTime(&RtcHandle, &RTC_TimeStructure, RTC_FORMAT_BIN);
   HAL_RTC_GetDate(&RtcHandle, &RTC_DateStructure, RTC_FORMAT_BIN);

   *Time = RTC_TimeStructure;
}

/**
  * @brief  RTC Set time. 
  * @param  Time: Pointer to Time structure
  * @retval None
  */
void RTC_SetTime(  RTC_TimeTypeDef *Time)
{
   RTC_TimeStructure = *Time;

   RTC_TimeStructure.StoreOperation = 0;
   RTC_TimeStructure.SubSeconds = 0;
   RTC_TimeStructure.DayLightSaving = 0;

   HAL_RTC_SetTime(&RtcHandle, &RTC_TimeStructure, RTC_FORMAT_BIN);
   HAL_RTC_SetDate(&RtcHandle, &RTC_DateStructure, RTC_FORMAT_BIN);
}

/**
  * @brief  RTC Get date
  * @param  Date: Pointer to Date structure
  * @retval None
  */
void RTC_GetDate(  RTC_DateTypeDef *Date)
{
   HAL_RTC_GetTime(&RtcHandle, &RTC_TimeStructure, RTC_FORMAT_BIN);
   HAL_RTC_GetDate(&RtcHandle, &RTC_DateStructure, RTC_FORMAT_BIN);
   
   if((RTC_DateStructure.Date == 0) || (RTC_DateStructure.Month == 0))
   {
     RTC_DateStructure.Date = RTC_DateStructure.Month = 1;
   }

   *Date = RTC_DateStructure;
}

/**
  * @brief  RTC Set date
  * @param  Date: Pointer to Date structure
  * @retval None
  */
void RTC_SetDate(  RTC_DateTypeDef *Date)
{
   /* Set Time first */
   RTC_DateStructure = *Date;

   HAL_RTC_SetTime(&RtcHandle, &RTC_TimeStructure, RTC_FORMAT_BIN);
   HAL_RTC_SetDate(&RtcHandle, &RTC_DateStructure, RTC_FORMAT_BIN);
}

/**
  * @}
  */

/**
  * @}
  */

