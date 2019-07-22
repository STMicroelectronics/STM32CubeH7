/**
  ******************************************************************************
  * @file    jpeg_utils.h  
  * @author  MCD Application Team
  * @brief   Header for jpeg_utils.c module
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
#ifndef __JPEG_UTILS_H
#define __JPEG_UTILS_H

/* Includes ------------------------------------------------------------------*/
#include "jpeg_utils_conf.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup JPEG
  * @{
  */

/** @defgroup JPEG_Exported_Defines JPEG Exported Defines
  * @{
  */
/**
* @}
*/

/** @defgroup JPEG_Exported_Types JPEG Exported Types
  * @{
  */
#if (USE_JPEG_DECODER == 1)
typedef   uint32_t (* JPEG_YCbCrToRGB_Convert_Function)(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer,
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount );
#endif

#if (USE_JPEG_ENCODER == 1)                                      
typedef   uint32_t (* JPEG_RGBToYCbCr_Convert_Function)(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer,
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount );
#endif
/**
* @}
*/

/** @defgroup JPEG_Exported_Macros JPEG Exported Macros
  * @{
  */ 
/**
* @}
*/

/** @defgroup JPEG_Exported_Variables JPEG Exported Variables
  * @{
  */
/**
* @}
*/  

/** @defgroup JPEG_Exported_FunctionsPrototype JPEG Exported FunctionsPrototype
  * @{
  */ 
void JPEG_InitColorTables(void);

#if (USE_JPEG_DECODER == 1)
HAL_StatusTypeDef JPEG_GetDecodeColorConvertFunc(JPEG_ConfTypeDef *pJpegInfo, JPEG_YCbCrToRGB_Convert_Function *pFunction, uint32_t *ImageNbMCUs);
#endif

#if (USE_JPEG_ENCODER == 1)
HAL_StatusTypeDef JPEG_GetEncodeColorConvertFunc(JPEG_ConfTypeDef *pJpegInfo, JPEG_RGBToYCbCr_Convert_Function *pFunction, uint32_t *ImageNbMCUs);
#endif
/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 
#endif /* __JPEG_UTILS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

