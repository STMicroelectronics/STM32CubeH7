/**
  ******************************************************************************
  * @file    jpeg_utils.h  
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    3-June-2016
  * @brief   Header for jpeg_utils.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

