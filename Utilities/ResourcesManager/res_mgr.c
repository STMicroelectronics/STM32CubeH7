/**
  ******************************************************************************
  * @file    res_mgr.c
  * @author  MCD Application Team
  * @brief   This file provides the resources manager services for dual core
  *          products.
  *
  ******************************************************************************
  *
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the License; You may not use this fileexcept in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include  "res_mgr.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup RES_MGR_Private_Types
* @{
*/
/**
* @}
*/


/** @defgroup RES_MGR_Private_Defines
* @{
*/
#ifndef RESMGR_TABLE_LOCATION
#if defined ( __ICCARM__ )
#define RESMGR_TABLE_LOCATION   0x38000000
#elif defined ( __CC_ARM )
#define RESMGR_TABLE_LOCATION   0x38000000
#elif defined ( __GNUC__ )
#define RESMGR_TABLE_LOCATION   ".RAM_D3"
#endif
#endif
/**
* @}
*/


/** @defgroup RES_MGR_Private_Macros
* @{
*/
#if !defined(RESMGR_TBL_LOCK)
#define RESMGR_TBL_LOCK(lock_id)              \
{                                             \
  while(ResMgr_Tbl[(lock_id)].Spinlock != 0U) \
    {                                         \
    }                                         \
  ResMgr_Tbl[(lock_id)].Spinlock = 1;         \
}
#endif
#if !defined(RESMGR_TBL_UNLOCK)
#define RESMGR_TBL_UNLOCK(lock_id)            \
{                                             \
  ResMgr_Tbl[(lock_id)].Spinlock = 0;         \
}
#endif
/**
* @}
*/


/** @defgroup RES_MGR_Private_Variables
* @{
*/

#if defined ( __ICCARM__ )
#pragma location = RESMGR_TABLE_LOCATION
static ResEntry_t  ResMgr_Tbl[RESMGR_ENTRY_NBR];
#elif defined ( __CC_ARM )
static ResEntry_t  ResMgr_Tbl[RESMGR_ENTRY_NBR] __attribute__((at(RESMGR_TABLE_LOCATION)));

#elif defined ( __GNUC__ )
static ResEntry_t  ResMgr_Tbl[RESMGR_ENTRY_NBR] __attribute__((section(RESMGR_TABLE_LOCATION)));

#endif

static ResMgrSendMsg_t    ResMgrSendMsgFunct;
static ResMgrCallback_t   ResMgrCallbackFunct;
/**
* @}
*/


/** @defgroup RES_MGR_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/


/** @defgroup RES_MGR_Private_Functions
* @{
*/


/**
  * @brief  Initializes the Resource Manager module
  * @param  SendFunct : function pointer used to send message to the other core
  * @param  Callback  : function pointer used to publish the status to user side
  * @retval Return Status
  */
ResMgr_Status_t  ResMgr_Init(ResMgrSendMsg_t SendFunct, ResMgrCallback_t Callback)
{
  ResMgr_Status_t ret = RESMGR_OK;
  uint32_t count = 0;
  /* lock table modification */
  RESMGR_TBL_LOCK(RESMGR_ID_RESMGR_TABLE);

  /* Check whether the resource is free */
  if(ResMgr_Tbl[RESMGR_ID_RESMGR_TABLE].Ctx.State != RESMGR_STATE_TBL_INIT)
  {

#if defined(RESMGR_USE_DEFAULT_TBL)
    for( ; count < RESMGR_ENTRY_NBR ; count++)
    {
      if(Default_ResTbl[count] != RES_DEFAULT_ASSIGN_NONE)
      {
        ResMgr_Tbl[count].Ctx.State = RESMGR_STATE_ASSIGNED;
        if ( Default_ResTbl[count]== RES_DEFAULT_ASSIGN_CPU1)
        {
          ResMgr_Tbl[count].Ctx.Flags = RESMGR_FLAGS_CPU1;
        }
        if ( Default_ResTbl[count]== RES_DEFAULT_ASSIGN_CPU2)
        {
          ResMgr_Tbl[count].Ctx.Flags = RESMGR_FLAGS_CPU2;
        }
      }
      else
      {
        ResMgr_Tbl[count].Ctx.State = RESMGR_STATE_RELEASED;
        ResMgr_Tbl[count].Ctx.Flags = 0;
      }
      ResMgr_Tbl[count].Spinlock = 0;
      ResMgr_Tbl[count].Ctx.pHandle = NULL;
    }
#else /* RESMGR_USE_DEFAULT_TBL */
    for( ; count < RESMGR_ENTRY_NBR ; count++)
    {
      ResMgr_Tbl[count].Spinlock = 0;
      ResMgr_Tbl[count].Ctx.Flags = 0;
      ResMgr_Tbl[count].Ctx.State = RESMGR_STATE_RELEASED;
      ResMgr_Tbl[count].Ctx.pHandle = NULL;
    }
#endif /* RESMGR_USE_DEFAULT_TBL */
    ResMgr_Tbl[RESMGR_ID_RESMGR_TABLE].Ctx.State = RESMGR_STATE_TBL_INIT;
  }

  RESMGR_TBL_UNLOCK(RESMGR_ID_RESMGR_TABLE);

  if((SendFunct != NULL) || (Callback != NULL))
  {
    ResMgrSendMsgFunct = SendFunct;
    ResMgrCallbackFunct = Callback;
    ResMgrCallbackFunct(RESMGR_ID_ALL, RESMGR_MSG_INIT);
  }
  else
  {
    ret = RESMGR_ERROR;
  }

  return ret;
}

/**
  * @brief  DeInitializes the Resource Manager module
  * @param  None
  * @retval Return Status
  */
ResMgr_Status_t ResMgr_DeInit(void)
{
  uint32_t count = 0;

  /* lock table modification */

  RESMGR_TBL_LOCK(RESMGR_ID_RESMGR_TABLE);

  for( ; count < RESMGR_ENTRY_NBR ; count++)
  {
    if(ResMgr_Tbl[count].Ctx.State == RESMGR_STATE_ASSIGNED)
    {
      ResMgrSendMsgFunct(count, RESMGR_MSG_RELEASE);
    }

    ResMgrCallbackFunct(RESMGR_ID_ALL, RESMGR_MSG_DEINIT);

    ResMgr_Tbl[count].Spinlock = 0;
    ResMgr_Tbl[count].Ctx.Flags = 0;
    ResMgr_Tbl[count].Ctx.State = RESMGR_STATE_RELEASED;
    ResMgr_Tbl[count].Ctx.pHandle = NULL;
  }

  ResMgr_Tbl[RESMGR_ID_RESMGR_TABLE].Ctx.State = RESMGR_STATE_RELEASED;

  RESMGR_TBL_UNLOCK(RESMGR_ID_RESMGR_TABLE);

  ResMgrSendMsgFunct = NULL;
  ResMgrCallbackFunct = NULL;

  return RESMGR_OK;
}

/**
  * @brief  Request a Resource
  * @param  id: Resource identifier
  * @param  flags: Request options
  * @param  prio: Request priority
  * @param  phandle: resource driver handle
  * @retval Status
  */
ResMgr_Status_t  ResMgr_Request ( uint32_t id, uint32_t flags, uint32_t prio, void *phandle)
{
  ResMgr_Status_t ret = RESMGR_OK;

  if(id < RESMGR_ENTRY_NBR)
  {
    /* lock table modification*/

    RESMGR_TBL_LOCK(id);

    /* Check whether the resource is free */
    /* Or the resource is assigned by default to the current Core*/
    if((ResMgr_Tbl[id].Ctx.State == RESMGR_STATE_RELEASED) ||
       ((ResMgr_Tbl[id].Ctx.State == RESMGR_STATE_ASSIGNED) &&
        (ResMgr_Tbl[id].Ctx.Flags & RESMGR_FLAGS_OWNER_MSK) == (flags & RESMGR_FLAGS_OWNER_MSK)))
    {

      if(phandle != NULL)
      {
        if((flags & RESMGR_FLAGS_INHERIT_MSK) == RESMGR_FLAGS_INHERIT_HANDLE)
        {
          if (ResMgr_Tbl[id].Ctx.pHandle != NULL)
          {
            *((uint32_t *)phandle) = (uint32_t ) (uint32_t *)ResMgr_Tbl[id].Ctx.pHandle;
          }
          else
          {
            RESMGR_TBL_UNLOCK(id);
            return  RESMGR_ERROR;
          }
        }
        else
        {
          ResMgr_Tbl[id].Ctx.pHandle = phandle;
        }
      }
      /* Resource free, update the table and send a message */
      ResMgr_Tbl[id].Ctx.Flags = (flags & ~RESMGR_FLAGS_ACCESS_MSK) | \
        RESMGR_FLAGS_ACCESS_NORMAL;

      ResMgr_Tbl[id].Ctx.State = RESMGR_STATE_ASSIGNED;
      RESMGR_TBL_UNLOCK(id);
      ResMgrSendMsgFunct(id, RESMGR_MSG_ASSIGN);
      ResMgrCallbackFunct(id, RESMGR_MSG_ASSIGNED);
    }
    else  /* Resource already used, update the table and send a message */
    {
      ret = RESMGR_BUSY;

      /* is the request for normal access, reject*/
      if((flags & RESMGR_FLAGS_ACCESS_MSK) == RESMGR_FLAGS_ACCESS_NORMAL)
      {
        RESMGR_TBL_UNLOCK(id);
        ResMgrSendMsgFunct(id, RESMGR_MSG_REJECT);
        ResMgrCallbackFunct(id, RESMGR_MSG_REJECTED);
      }

      /* it is a pend request? */
      else if((flags & RESMGR_FLAGS_ACCESS_MSK) == RESMGR_FLAGS_ACCESS_PEND)
      {
        ResMgr_Tbl[id].Ctx.Flags = (ResMgr_Tbl[id].Ctx.Flags & RESMGR_FLAGS_OWNER_MSK) | \
          (flags & (~RESMGR_FLAGS_ACCESS_MSK)) | RESMGR_FLAGS_ACCESS_PEND;

        if(phandle != NULL)
        {
          if((flags & RESMGR_FLAGS_INHERIT_MSK) == RESMGR_FLAGS_INHERIT_HANDLE)
          {
            if (ResMgr_Tbl[id].Ctx.pHandle != NULL)
            {
              *((uint32_t *)phandle) = (uint32_t )(uint32_t *) ResMgr_Tbl[id].Ctx.pHandle;
            }
            else
            {
              return  RESMGR_ERROR;
            }
          }
          else
          {
            ResMgr_Tbl[id].Ctx.pHandle = phandle;
          }
        }
        RESMGR_TBL_UNLOCK(id);
        ResMgrSendMsgFunct(id, RESMGR_MSG_PEND);
        ResMgrCallbackFunct(id, RESMGR_MSG_PENDED);
      }
      else
      {
        ret = RESMGR_ERROR;
      }

    }

  }
  else
  {
    ret = RESMGR_ERROR;
  }
  return ret;
}

/**
  * @brief  Release the resource
  * @param  id: Resource identifier
  * @retval Status
  */
ResMgr_Status_t  ResMgr_Release ( uint32_t id, uint32_t flags)
{
  ResMgr_Status_t ret = RESMGR_OK;

  if(id < RESMGR_ENTRY_NBR)
  {
    /* lock table modification */
    RESMGR_TBL_LOCK(id);
    /* Check whether the resource is free */
    if((ResMgr_Tbl[id].Ctx.State == RESMGR_STATE_ASSIGNED) && \
      ((ResMgr_Tbl[id].Ctx.Flags & RESMGR_FLAGS_OWNER_MSK) & (flags & RESMGR_FLAGS_OWNER_MSK)) != 0)
    {
      if((ResMgr_Tbl[id].Ctx.Flags & RESMGR_FLAGS_ACCESS_MSK) == RESMGR_FLAGS_ACCESS_PEND)
      {
        /* Another task is waiting for the resource ?*/
        ResMgr_Tbl[id].Ctx.State = RESMGR_STATE_ASSIGNED;
        ResMgr_Tbl[id].Ctx.Flags &=~RESMGR_FLAGS_ACCESS_MSK;
        ResMgr_Tbl[id].Ctx.Flags |= RESMGR_FLAGS_ACCESS_NORMAL;
        RESMGR_TBL_UNLOCK(id);
        ResMgrSendMsgFunct(id, RESMGR_MSG_PEND_ASSIGN);
        ResMgrCallbackFunct(id, RESMGR_MSG_PEND_ASSIGNED);
      }
      else if((ResMgr_Tbl[id].Ctx.Flags & RESMGR_FLAGS_ACCESS_MSK) == RESMGR_FLAGS_ACCESS_NORMAL)
      {
        ResMgr_Tbl[id].Ctx.State = RESMGR_STATE_RELEASED;
        RESMGR_TBL_UNLOCK(id);
        ResMgrSendMsgFunct(id, RESMGR_MSG_RELEASE);
        ResMgrCallbackFunct(id, RESMGR_MSG_RELEASED);
      }
      else
      {
        return  RESMGR_ERROR;
      }
    }
    ResMgr_Tbl[id].Spinlock = 0;
  }
  else
  {
    ret = RESMGR_ERROR;
  }
  return ret;
}

/**
  * @brief  Get reource context
  * @param  id: Resource identifier
  * @param  ctx: Resource context pointer
  * @retval Status
  */
ResMgr_Status_t  ResMgr_GetResContext( uint32_t id, ResMgr_Ctx_t  *ctx)
{
  ResMgr_Status_t ret = RESMGR_OK;

  if(id < RESMGR_ENTRY_NBR)
  {

    RESMGR_TBL_LOCK(id);
    ctx->Flags = ResMgr_Tbl[id].Ctx.Flags;
    ctx->State = ResMgr_Tbl[id].Ctx.State;
    ctx->pHandle = ResMgr_Tbl[id].Ctx.pHandle;
    RESMGR_TBL_UNLOCK(id);
  }
  else
  {
    ret = RESMGR_ERROR;
  }
  return ret;
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
