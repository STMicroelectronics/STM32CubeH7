/**
  ******************************************************************************
  * @file    jpeg_utils_conf_template.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    3-June-2016
  * @brief   jpeg_utils configuration template file.
  *          This file should be copied to the application folder and modified 
  *          as follows:
  *            - Rename it to 'jpeg_utils_conf.h'.
  *            - Update the name of the JPEG driver's header file, depending on
  *               the EVAL board you are using, see lines 51 and 52 below (by default this  
  *               file will generate compile error unless you do this modification).
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

#ifndef  __JPEG_UTILS_CONF_H__
#define  __JPEG_UTILS_CONF_H__


/* Includes ------------------------------------------------------------------*/
/*#include "stm32fXxx_hal.h" */
/* #include "stm32fXxx_hal_jpeg.h" */

/* Private define ------------------------------------------------------------*/
/** @addtogroup JPEG_Private_Defines
  * @{
  */
/* RGB Color format definition for JPEG encoding/Decoding : Should not be modified*/
#define JPEG_ARGB8888            0  /* ARGB8888 Color Format */
#define JPEG_RGB888              1  /* RGB888 Color Format   */
#define JPEG_RGB565              2  /* RGB565 Color Format   */
    
/**
* @}
*/
    
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @addtogroup JPEG_Exported_Defines
  * @{
  */    
#define USE_JPEG_DECODER     1  /* Enable Decoding Post-Processing functions (YCbCr to RGB conversion) */
#define USE_JPEG_ENCODER     1  /* Enable Encoding Pre-Processing functions (RGB to YCbCr conversion)*/

#define JPEG_RGB_FORMAT      JPEG_ARGB8888  /* Select RGB format: ARGB8888, RGB888, RBG_565 */
#define JPEG_SWAP_RB         0  /* Change color order to BGR */

/**
* @}
*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __JPEG_UTILS_CONF_H__ */
