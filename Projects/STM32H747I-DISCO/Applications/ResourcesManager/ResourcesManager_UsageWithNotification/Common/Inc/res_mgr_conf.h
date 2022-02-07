/**
  ******************************************************************************
  * @file    res_mgr_conf.h
  * @author  MCD Application Team
  * @brief   header for resource manager configuration
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
#ifndef  RES_MGR_CONF_H__
#define  RES_MGR_CONF_H__

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>
#include <main.h>
#include "stm32h7xx_hal.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup RES_MGR_TABLE
  * @{
  */
  
/** @defgroup RES_MGR_TABLE
  * @brief 
  * @{
  */ 


/** @defgroup RES_MGR_Exported_Defines
  * @{
  */ 
/* This Part may be filled by CubeMX/User */ 
/******** Resource IDs ********************************************************/
enum
{
  RESMGR_ID_ADC1                            ,
  RESMGR_ID_ADC2                            ,
  RESMGR_ID_USART1                          ,
  RESMGR_ID_USART2                          ,
  RESMGR_ID_RESMGR_TABLE                    ,
};

#define   RESMGR_ENTRY_NBR  ((uint32_t)RESMGR_ID_RESMGR_TABLE + 1UL)  
#define   RESMGR_ID_ALL                    0x0000FFFFU
#define   RESMGR_ID_NONE                   0xFFFFFFFFU

#define RESMGR_USE_DEFAULT_TBL

#define RES_DEFAULT_ASSIGN_NONE   0U
#define RES_DEFAULT_ASSIGN_CPU1   1U
#define RES_DEFAULT_ASSIGN_CPU2   2U

static const uint8_t Default_ResTbl[RESMGR_ENTRY_NBR] = {
  RES_DEFAULT_ASSIGN_NONE,/* RESMGR_ID_ADC1*/
  RES_DEFAULT_ASSIGN_NONE,/* RESMGR_ID_ADC2 */
  RES_DEFAULT_ASSIGN_NONE,/* RESMGR_ID_USART1*/
  RES_DEFAULT_ASSIGN_NONE,/* RESMGR_ID_USART2 */
  RES_DEFAULT_ASSIGN_CPU1,/* RESMGR_ID_RESMGR_TABLE */
};
/* End of CubeMx/User part*/ 

/**
  * @}
  */


/** @defgroup RES_MGR_Lock_Procedure
  * @{
  */ 

/* Customized Lock Procedure definition  begin */ 
#define HSEM_ID_RES_TABLE                      31U

#define RESMGR_TBL_LOCK(lock_id)                          \
{                                                         \
  while (HAL_HSEM_FastTake(HSEM_ID_RES_TABLE) != HAL_OK)  \
  {                                                       \
  }                                                       \
}                                                         \

#define RESMGR_TBL_UNLOCK(lock_id)                        \
{                                                         \
  HAL_HSEM_Release(HSEM_ID_RES_TABLE,0);                  \
}                                                         \


/* Customized Lock Procedure definition  end */ 
/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  
#endif /* RES_MGR_CONF_H__ */

