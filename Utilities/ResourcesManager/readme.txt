/**
  @page Resource manager configuration

  @verbati
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   How to configure resource manage
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
  @endverbatim

@par How to configure resource manager

The configuration file "res_mgr_conf.h" is used to configure this utility providing some useful
flexibilities:
- It defines the resources (RESMGR_ID_xxx) controlled by the resource manager
- It defines an optional default assignment table
- It offers the possibility to customize the Lock implementation used by the resource manager
- It also offers the possibility to use the RPMSG/OpenAMP-based extension

"res_mgr_conf.h" shall be built from the corresponding series template (example:
res_mgr_conf_stm32h7xx_template.h).
Copy that template to the application folder and rename that file in "res_mgr_conf.h". Then:
- In the enum list, keep only the resources that are expected to be controlled by the resource
  manager and remove the other ones.
- To define a default assignment table, define RESMGR_USE_DEFAULT_TBL and fill in the
  Default_ResTbl[RESMGR_ENTRY_NBR] table. This is optional (when not used, the resources are
  "Not assigned" by default)
- Optionnaly, implement the ResMgr_Tbl_LockInit(), ResMgr_Tbl_Lock() and ResMgr_Tbl_UnLock()
  functions. These functions are used by the resource manager to avoid concurrent access
  conflict between the two Cores.
- Optionnaly, define RESMGR_WITH_RPMSG to use the RPMSG/OpenAMP-based extension (see resmgr.c
  for more information)

Example:
    ...
    enum
    {
      RESMGR_ID_ADC1                            ,
      RESMGR_ID_ADC2                            ,
      RESMGR_ID_USART1                          ,
      RESMGR_ID_USART2                          ,
      RESMGR_ID_RESMGR_TABLE                    ,
    };
    ...
    #define RESMGR_USE_DEFAULT_TBL
    static const uint8_t Default_ResTbl[RESMGR_ENTRY_NBR] = {
    /* 0:Not assigned | 1:Assigned to core1 | 2:Assigned to core2 */
       1, /* RESMGR_ID_ADC1 */
       2, /* RESMGR_ID_ADC2 */
       2, /* RESMGR_ID_USART1 */
       1, /* RESMGR_ID_USART2 */
       0, /* RESMGR_ID_RESMGR_TABLE */
    };
    ...

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
