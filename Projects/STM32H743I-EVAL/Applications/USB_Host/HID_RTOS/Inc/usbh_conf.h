/**
  ******************************************************************************
  * @file    USB_Host/HID_RTOS/Inc/USBH_conf.h
  * @author  MCD Application Team
  * @brief   General low level driver configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBH_CONF__H__
#define __USBH_CONF__H__

#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Includes ------------------------------------------------------------------*/

/** @addtogroup USBH_OTG_DRIVER
  * @{
  */
  
/** @defgroup USBH_CONF
  * @brief usb otg low level driver configuration file
  * @{
  */ 

/** @defgroup USBH_CONF_Exported_Defines
  * @{
  */ 

#define USBH_MAX_NUM_ENDPOINTS                2
#define USBH_MAX_NUM_INTERFACES               2
#define USBH_MAX_NUM_CONFIGURATION            1
#define USBH_MAX_NUM_SUPPORTED_CLASS          1
#define USBH_KEEP_CFG_DESCRIPTOR              0
#define USBH_MAX_SIZE_CONFIGURATION           0x200
#define USBH_MAX_DATA_BUFFER                  0x200
#define USBH_DEBUG_LEVEL                      2
#define USBH_USE_OS                           1
    
/** @defgroup USBH_Exported_Macros
  * @{
  */ 
#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define   USBH_PROCESS_PRIO          osPriorityNormal
  #define   USBH_PROCESS_STACK_SIZE    (8 * configMINIMAL_STACK_SIZE)
#endif  

 /* Memory management macros */   
#define USBH_malloc               pvPortMalloc
#define USBH_free                 vPortFree
#define USBH_memset               memset
#define USBH_memcpy               memcpy
    
 /* DEBUG macros */  

  
#if (USBH_DEBUG_LEVEL > 0)
#define  USBH_UsrLog(...)   printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBH_UsrLog(...)   
#endif 
                            
                            
#if (USBH_DEBUG_LEVEL > 1)

#define  USBH_ErrLog(...)   printf("ERROR: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBH_ErrLog(...)   
#endif 
                            
                            
#if (USBH_DEBUG_LEVEL > 2)                         
#define  USBH_DbgLog(...)   printf("DEBUG : ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBH_DbgLog(...)                         
#endif
                            
/**
  * @}
  */ 
                              
                              

    
    
    
/**
  * @}
  */ 


/** @defgroup USBH_CONF_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_CONF_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_CONF_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_CONF_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 


#endif //__USBH_CONF__H__


/**
  * @}
  */ 

/**
  * @}
  */ 

