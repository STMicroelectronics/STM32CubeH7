/**
  ******************************************************************************
  * @file    jpeg_utils.c 
  * @author  MCD Application Team
  * @brief   This driver provides JPEG MCU (Minimum Coded Unit) blocks to RGB and RGB to JPEG MCU conversion functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
    [..]  
      The STM32 HW JPEG decoder/encoder peripheral allows to decode/encode jpeg images.
      In decoding, the STM32 HW JPEG data output are organized in blocks called MCU (Minimum Coded Unit)
      as specified in the jpeg standard. 
      In encoding, the STM32 HW JPEG data input must be organized in MCU blocks.
      Depending of the image color space and chrominance sampling, an MCU is generally organized in :
      N luminance (Y) blocks + a Blue chrominance (Cb) block + a Red chrominance (Cr) block.
      Each block size is 8x8 samples.
      
      The STM32 HW JPEG supports 3 possible color space :
        - YCbCr : an MCU is composed of 3 color components : Y , Cb and Cr 
        - GrayScale : an MCU is composed of 1 single color component : Y
        - CMYK : an MCU is composed of 4 color components : Cyan, Magenta, Yellow, and Key (blacK)
                      
      This utility file should be used on top of the STM32 HAL JPEG driver. 
      In decoding, it allows to convert MCU blocks to RGB888 or ARGB8888 pixels stored to a destination 
      frame buffer that can be displayed.
      In encoding, it allows to convert RGB888 or ARGB8888 pixels  to MCU blocks.
      
      This utility supports following MCU Chroma sampling format and color space :
        - YCbCr 4:2:0 : Each MCU is composed of 4 Y 8x8 blocks + 1 Cb 8x8 block + Cr 8x8 block
        - YCbCr 4:2:2 : Each MCU is composed of 2 Y 8x8 blocks + 1 Cb 8x8 block + Cr 8x8 block
        - YCbCr 4:4:4 : Each MCU is composed of 1 Y 8x8 block + 1 Cb 8x8 block + Cr 8x8 block
        - GrayScale   : Each MCU is composed of 1 Y 8x8 block
        - CMYK        : Each MCU is composed of 1 Cyan 8x8  block + 1 Magenta 8x8 block + 1 Yellow 8x8 block + 1 Key 8x8 block.
  ==============================================================================
                     ##### How to use this driver #####                         
  ==============================================================================
    [..]
      - The configuration file "jpeg_utils_conf_template.h" is used to configure this utility
        providing some useful flexibilities.
        This file should be copied to the application folder and modified as follows:
          - Rename it to "jpeg_utils_conf.h".
          - Update the name of the JPEG driver's header file, depending on the EVAL board you are using.
    
      Example if using the STM32F769I-EVAL board :
        - Copy the file "jpeg_utils_conf_template.h" to the application folder and rename it to "jpeg_utils_conf.h"
        - Edit the "jpeg_utils_conf.h" and change lines 51 and 52 as follow :
           #include "stm32f7xx_hal.h"
           #include "stm32f7xx_hal_jpeg.h"
    
      Using this configuration file, user can change the following settings:
          - Use RGB888 or ARGB8888 or RGB565 by setting the constant JPEG_RGB_FORMAT respectively to JPEG_RGB888, JPEG_ARGB8888 JPEG_RGB565.
          - Swap RED, and Blue offsets if user needs to change the color order to BGR (instead of RGB) by setting:
             #define JPEG_SWAP_RB     1
          - Enable or disable the decoding post-processing functions (YCbCr to RGB conversion functions) by setting the define USE_JPEG_DECODER 
            respectively to 0 or 1.
          - Enable or disable the encoding pre-processing functions (RGB to YCbCr conversion functions) by setting the define USE_JPEG_ENCODER 
            respectively to 0 or 1.
 
    [..]For Decoding:
      - First, function "JPEG_InitColorTables" should be called to initialize the YCbCr to RGB color
        conversion tables. This function needs to be called only one time at the beginning of the
        program whatever the number of jpeg files to be decoded.
    
      - As soon as the JPEG HW peripheral finished parsing the header of the JPEG input file,
        the HAL JPEG callback "HAL_JPEG_InfoReadyCallback" is launched providing the jpeg file
        characteristics found in its header.
        User can then call the utility function "JPEG_GetDecodeColorConvertFunc" with these
        information and retrieve the corresponding color conversion function and number of MCUs.
    
        Then each time an integer number of MCUs are available (from the HW JPEG output), user
        can call the retrieved function to convert these HW JPEG output data to RGB888 or
        ARGB8888 pixel stored to the specified destination buffer.
    
    [..]For Encoding:
      - First, function "JPEG_InitColorTables" should be called to initialize the YCbCr/RGB color
        conversion tables. This function needs to be called only one time at the beginning of the
        program whatever the number of jpeg files to be encoded or decoded.
    
      - First Use the utility function "JPEG_GetEncodeColorConvertFunc" with the input image information
        to retrieve the corresponding color conversion function and number of MCUs.
    
        Then each time an RGB input buffer is available, user can call the retrieved function to convert 
        RGB data to MCU blocks stored to the specified destination buffer.
  @endverbatim
  */

/* Includes ------------------------------------------------------------------*/
#include "jpeg_utils.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup JPEG JPEG
* @brief JPEG YCbCr blocks <-> RGB conversion utilities  
* @{
*/

/** @defgroup JPEG_Private_Types JPEG Private Types
* @{
*/ 
/* Private typedef -----------------------------------------------------------*/
typedef struct __JPEG_MCU_RGB_ConvertorTypeDef
{
  uint32_t ColorSpace;
  uint32_t ChromaSubsampling;
  
  uint32_t ImageWidth;
  uint32_t ImageHeight;
  uint32_t ImageSize_Bytes;

  uint32_t LineOffset;
  uint32_t BlockSize;
  
  uint32_t H_factor;
  uint32_t V_factor;

  uint32_t WidthExtend;
  uint32_t ScaledWidth;
  
  uint32_t MCU_Total_Nb;
  
  uint16_t *Y_MCU_LUT;  
  uint16_t *Cb_MCU_LUT;  
  uint16_t *Cr_MCU_LUT;  
  uint16_t *K_MCU_LUT;

}JPEG_MCU_RGB_ConvertorTypeDef;
/**
* @}
*/ 

/** @defgroup JPEG_Private_Defines JPEG Private Defines
* @{
*/ 
/* Private define ------------------------------------------------------------*/
#define YCBCR_420_BLOCK_SIZE       384     /* YCbCr 4:2:0 MCU : 4 8x8 blocks of Y + 1 8x8 block of Cb + 1 8x8 block of Cr   */
#define YCBCR_422_BLOCK_SIZE       256     /* YCbCr 4:2:2 MCU : 2 8x8 blocks of Y + 1 8x8 block of Cb + 1 8x8 block of Cr   */
#define YCBCR_444_BLOCK_SIZE       192     /* YCbCr 4:4:4 MCU : 1 8x8 block of Y + 1 8x8 block of Cb + 1 8x8 block of Cr   */

#define GRAY_444_BLOCK_SIZE        64      /* GrayScale MCU : 1 8x8 block of Y */

#define CMYK_444_BLOCK_SIZE        256     /* CMYK MCU : 1 8x8 blocks of Cyan + 1 8x8 block Magenta + 1 8x8 block of Yellow and 1 8x8 block of BlacK */

#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
  #define JPEG_GREEN_OFFSET        8       /* Offset of the GREEN color in a pixel         */    
  #define JPEG_ALPHA_OFFSET        24      /* Offset of the Transparency Alpha in a pixel  */
  #define JPEG_BYTES_PER_PIXEL     4       /* Number of bytes in a pixel                   */
  #if (JPEG_SWAP_RB == 0)
    #define JPEG_RED_OFFSET        16      /* Offset of the RED color in a pixel           */
    #define JPEG_BLUE_OFFSET       0       /* Offset of the BLUE color in a pixel          */
  #else
    #define JPEG_RED_OFFSET        0       /* Offset of the RED color in a pixel           */
    #define JPEG_BLUE_OFFSET       16      /* Offset of the BLUE color in a pixel          */
  #endif /* JPEG_SWAP_RB */

#elif(JPEG_RGB_FORMAT == JPEG_RGB888)
  #define JPEG_GREEN_OFFSET        8       /* Offset of the GREEN color in a pixel         */    
  #define JPEG_BYTES_PER_PIXEL     3       /* Number of bytes in a pixel                   */

  #if (JPEG_SWAP_RB == 0)
    #define JPEG_RED_OFFSET        16      /* Offset of the RED color in a pixel           */
    #define JPEG_BLUE_OFFSET       0       /* Offset of the BLUE color in a pixel          */
  #else
   #define JPEG_RED_OFFSET         0       /* Offset of the RED color in a pixel           */
   #define JPEG_BLUE_OFFSET        16      /* Offset of the BLUE color in a pixel          */
  #endif  /* JPEG_SWAP_RB */

#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
  #define JPEG_GREEN_OFFSET        5       /* Offset of the GREEN color in a pixel         */    
  #define JPEG_BYTES_PER_PIXEL     2       /* Number of bytes in a pixel                   */
  #define JPEG_RGB565_GREEN_MASK   0x07E0  /* Mask of Green component in RGB565 Format     */

  #if (JPEG_SWAP_RB == 0)
    #define JPEG_RED_OFFSET        11      /* Offset of the RED color in a pixel           */
    #define JPEG_BLUE_OFFSET       0       /* Offset of the BLUE color in a pixel          */
    #define JPEG_RGB565_RED_MASK   0xF800  /* Mask of Red component in RGB565 Format       */
    #define JPEG_RGB565_BLUE_MASK  0x001F  /* Mask of Blue component in RGB565 Format      */
  #else
    #define JPEG_RED_OFFSET        0       /* Offset of the RED color in a pixel           */
    #define JPEG_BLUE_OFFSET       11      /* Offset of the BLUE color in a pixel          */
    #define JPEG_RGB565_RED_MASK   0x001F  /* Mask of Red component in RGB565 Format       */
    #define JPEG_RGB565_BLUE_MASK  0xF800  /* Mask of Blue component in RGB565 Format      */
  #endif /* JPEG_SWAP_RB */

#else

  #error "unknown JPEG_RGB_FORMAT "

#endif /* JPEG_RGB_FORMAT */

/**
* @}
*/ 

/** @defgroup JPEG_Private_Macros JPEG Private Macros
* @{
*/ 
/* Private macro -------------------------------------------------------------*/
#if (USE_JPEG_DECODER == 1)
#define CLAMP(value) CLAMP_LUT[(value) + 0x100] /* Range limiting macro */
#endif
#if (USE_JPEG_ENCODER == 1)
#define MAX(val1,val2) ((val1 > val2) ? val1 : val2)
#endif
/**
* @}
*/ 

/** @defgroup JPEG_Private_Variables JPEG Private Variables
* @{
*/ 
/* Private variables ---------------------------------------------------------*/

static JPEG_MCU_RGB_ConvertorTypeDef JPEG_ConvertorParams;

#if (USE_JPEG_DECODER == 1)
static int32_t CR_RED_LUT[256];           /* Cr to Red color conversion Look Up Table  */
static int32_t CB_BLUE_LUT[256];          /* Cb to Blue color conversion Look Up Table */
static int32_t CR_GREEN_LUT[256];         /* Cr to Green color conversion Look Up Table*/
static int32_t CB_GREEN_LUT[256];         /* Cb to Green color conversion Look Up Table*/
#endif /* USE_JPEG_DECODER == 1 */

#if (USE_JPEG_ENCODER == 1)
static int32_t RED_Y_LUT[256];            /* Red to Y color conversion Look Up Table  */
static int32_t RED_CB_LUT[256];           /* Red to Cb color conversion Look Up Table  */
static int32_t BLUE_CB_RED_CR_LUT[256];   /* Red to Cr and Blue to Cb color conversion Look Up Table  */
static int32_t GREEN_Y_LUT[256];          /* Green to Y color conversion Look Up Table*/
static int32_t GREEN_CR_LUT[256];         /* Green to Cr color conversion Look Up Table*/
static int32_t GREEN_CB_LUT[256];         /* Green to Cb color conversion Look Up Table*/
static int32_t BLUE_Y_LUT[256];           /* Blue to Y color conversion Look Up Table */
static int32_t BLUE_CR_LUT[256];          /* Blue to Cr color conversion Look Up Table */

/* Different MCU Look Up Table */
static uint16_t JPEG_Y_MCU_LUT[256];
static uint16_t JPEG_Y_MCU_444_LUT[64];

static uint16_t JPEG_Cb_MCU_420_LUT[256];
static uint16_t JPEG_Cb_MCU_422_LUT[256];
static uint16_t JPEG_Cb_MCU_444_LUT[64];

static uint16_t JPEG_Cr_MCU_420_LUT[256];
static uint16_t JPEG_Cr_MCU_422_LUT[256];
static uint16_t JPEG_Cr_MCU_444_LUT[64];

static uint16_t JPEG_K_MCU_420_LUT[256];
static uint16_t JPEG_K_MCU_422_LUT[256];
static uint16_t JPEG_K_MCU_444_LUT[64];

/* YCCK format blocks */
uint8_t kBlocks[16][16];
#endif /* USE_JPEG_ENCODER == 1 */

#if (USE_JPEG_DECODER == 1)
/*  color clamp table : used for range limiting */
static const uint8_t CLAMP_LUT[] = {
/* clamp range 0xffffffff to 0xffffff00 */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* clamp range 0x00 to 0xff */
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
/* clamp range 0x100 to 0x1ff */
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
#endif /* USE_JPEG_DECODER == 1 */
/**
* @}
*/

/** @defgroup JPEG_Private_FunctionPrototypes JPEG Private FunctionPrototypes
* @{
*/ 
/* Private function prototypes -----------------------------------------------*/
#if (USE_JPEG_ENCODER == 1)
static uint32_t JPEG_ARGB_MCU_YCbCr420_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);

static uint32_t JPEG_ARGB_MCU_YCbCr422_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);

static uint32_t JPEG_ARGB_MCU_YCbCr444_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);


static uint32_t JPEG_ARGB_MCU_Gray_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);

static uint32_t JPEG_ARGB_MCU_YCCK_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);

static void JPEG_Init_MCU_LUT(void);
static void JPEG_InitPreProcColorTables(void);
static uint8_t *JPEG_Set_K_Blocks(uint8_t *pMCUBuffer, uint8_t pKBlocks[16][16], uint32_t ChromaSampling);

#endif /* USE_JPEG_ENCODER == 1 */

#if (USE_JPEG_DECODER == 1)
static uint32_t JPEG_MCU_YCbCr420_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);

static uint32_t JPEG_MCU_YCbCr422_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);

static uint32_t JPEG_MCU_YCbCr444_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);


static uint32_t JPEG_MCU_Gray_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);

static uint32_t JPEG_MCU_YCCK_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount);
static void JPEG_InitPostProcColorTables(void);
#endif /* USE_JPEG_DECODER == 1 */

/**
* @}
*/

/** @defgroup JPEG_Private_Functions JPEG Private Functions
* @{
*/ 

#if (USE_JPEG_ENCODER == 1)
/**
  * @brief  Convert RGB to YCbCr 4:2:0 blocks pixels  
  * @param  pInBuffer  : pointer to input RGB888/ARGB8888 frame buffer.
  * @param  pOutBuffer : pointer to output YCbCr blocks buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from RGB to YCbCr
  */
static uint32_t JPEG_ARGB_MCU_YCbCr420_ConvertBlocks (uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j, currentMCU, xRef,yRef, colones;

  uint32_t refline;
  int32_t ycomp, crcomp, cbcomp, offset;
  
  uint32_t red, green, blue;
  
  uint8_t *pOutAddr;
  uint8_t *pInAddr;
  

  numberMCU = ((3 * DataCount) / ( 2 * JPEG_BYTES_PER_PIXEL * YCBCR_420_BLOCK_SIZE));

  currentMCU = BlockIndex;
  *ConvertedDataCount = numberMCU * JPEG_ConvertorParams.BlockSize;

  pOutAddr = &pOutBuffer[0];

  while(currentMCU < (numberMCU + BlockIndex))
  {
    xRef = ((currentMCU *JPEG_ConvertorParams.H_factor) / JPEG_ConvertorParams.WidthExtend)*JPEG_ConvertorParams.V_factor;
    
    yRef = ((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend);

    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    if(((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend) == 0)
    {
      colones = JPEG_ConvertorParams.H_factor - JPEG_ConvertorParams.LineOffset;
    }
    else
    {
      colones = JPEG_ConvertorParams.H_factor;
    }    
    offset = 0;

    for(i= 0; i <  JPEG_ConvertorParams.V_factor; i+=2)
    {
      
      pInAddr = &pInBuffer[0] ;
      
      for(j=0; j < colones; j+=2)
      {
        /* First Pixel */
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_BLUE_OFFSET/8)) ;
#endif
        ycomp  = (int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue));
        cbcomp = (int32_t)(*(RED_CB_LUT + red)) + (int32_t)(*(GREEN_CB_LUT + green)) + (int32_t)(*(BLUE_CB_RED_CR_LUT + blue)) + 128;
        crcomp = (int32_t)(*(BLUE_CB_RED_CR_LUT + red)) + (int32_t)(*(GREEN_CR_LUT + green)) + (int32_t)(*(BLUE_CR_LUT + blue)) + 128;
        
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset]))  = (ycomp);
        (*(pOutAddr + JPEG_ConvertorParams.Cb_MCU_LUT[offset])) = (cbcomp);
        (*(pOutAddr + JPEG_ConvertorParams.Cr_MCU_LUT[offset])) = (crcomp);
        
        /* Second Pixel */
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + JPEG_BYTES_PER_PIXEL + refline)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + JPEG_BYTES_PER_PIXEL + refline)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + JPEG_BYTES_PER_PIXEL + refline)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_BYTES_PER_PIXEL + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_BYTES_PER_PIXEL + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_BYTES_PER_PIXEL + JPEG_BLUE_OFFSET/8)) ;
#endif  
        ycomp  = (int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue));
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset + 1]))  = (ycomp);
        
        /* Third Pixel */
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + JPEG_ConvertorParams.ScaledWidth + refline)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + JPEG_ConvertorParams.ScaledWidth + refline)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + JPEG_ConvertorParams.ScaledWidth + refline)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_BLUE_OFFSET/8)) ;
#endif
        ycomp  = (int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue));
        
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset + JPEG_ConvertorParams.H_factor]))  = (ycomp);
        
        /* Fourth Pixel */
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_BYTES_PER_PIXEL)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_BYTES_PER_PIXEL)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_BYTES_PER_PIXEL)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_BYTES_PER_PIXEL + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_BYTES_PER_PIXEL + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_ConvertorParams.ScaledWidth + JPEG_BYTES_PER_PIXEL + JPEG_BLUE_OFFSET/8)) ;
#endif
        ycomp  = (int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue));
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset + JPEG_ConvertorParams.H_factor + 1]))  = (ycomp);
        
        /****************/
        
        pInAddr += JPEG_BYTES_PER_PIXEL * 2;
        offset+=2;
      }
      offset += JPEG_ConvertorParams.H_factor + (JPEG_ConvertorParams.H_factor - colones);
      refline += JPEG_ConvertorParams.ScaledWidth * 2 ;
    }
    pOutAddr +=  JPEG_ConvertorParams.BlockSize ;
  }
  return numberMCU;
}

/**
  * @brief  Convert RGB to YCbCr 4:2:2 blocks pixels  
  * @param  pInBuffer  : pointer to input RGB888/ARGB8888 frame buffer.
  * @param  pOutBuffer : pointer to output YCbCr blocks buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from RGB to YCbCr
  */
static uint32_t JPEG_ARGB_MCU_YCbCr422_ConvertBlocks (uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j, currentMCU, xRef,yRef, colones;

  uint32_t refline;
  int32_t ycomp, crcomp, cbcomp, offset;
  
  uint32_t red, green, blue;
  
  uint8_t *pOutAddr;
  uint8_t *pInAddr;

  numberMCU = ((2 * DataCount) / (JPEG_BYTES_PER_PIXEL * YCBCR_422_BLOCK_SIZE));

  currentMCU = BlockIndex;
  *ConvertedDataCount = numberMCU * JPEG_ConvertorParams.BlockSize;

  pOutAddr = &pOutBuffer[0];

  while(currentMCU < (numberMCU + BlockIndex))
  {
    xRef = ((currentMCU *JPEG_ConvertorParams.H_factor) / JPEG_ConvertorParams.WidthExtend)*JPEG_ConvertorParams.V_factor;
    
    yRef = ((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend);

    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    if(((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend) == 0)
    {
      colones = JPEG_ConvertorParams.H_factor - JPEG_ConvertorParams.LineOffset;
    }
    else
    {
      colones = JPEG_ConvertorParams.H_factor;
    }    
    offset = 0;

    for(i= 0; i <  JPEG_ConvertorParams.V_factor; i+=1)
    {
      
      pInAddr = &pInBuffer[0] ;
      
      for(j=0; j < colones; j+=2)
      {
        /* First Pixel */
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_BLUE_OFFSET/8)) ;
#endif
        ycomp  = (int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue));
        cbcomp = (int32_t)(*(RED_CB_LUT + red)) + (int32_t)(*(GREEN_CB_LUT + green)) + (int32_t)(*(BLUE_CB_RED_CR_LUT + blue)) + 128;
        crcomp = (int32_t)(*(BLUE_CB_RED_CR_LUT + red)) + (int32_t)(*(GREEN_CR_LUT + green)) + (int32_t)(*(BLUE_CR_LUT + blue)) + 128;
        
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset]))  = ycomp;
        (*(pOutAddr + JPEG_ConvertorParams.Cb_MCU_LUT[offset])) = cbcomp;
        (*(pOutAddr + JPEG_ConvertorParams.Cr_MCU_LUT[offset])) = crcomp;
        
        /* Second Pixel */
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + refline + JPEG_BYTES_PER_PIXEL)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + refline + JPEG_BYTES_PER_PIXEL)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + refline + JPEG_BYTES_PER_PIXEL)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_BYTES_PER_PIXEL + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_BYTES_PER_PIXEL + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_BYTES_PER_PIXEL + JPEG_BLUE_OFFSET/8)) ;
#endif
        ycomp  = (int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue));
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset + 1]))  = ycomp;
        
        /****************/
        
        pInAddr += JPEG_BYTES_PER_PIXEL * 2;
        offset+=2;
      }
      offset += (JPEG_ConvertorParams.H_factor - colones);
      refline += JPEG_ConvertorParams.ScaledWidth ;
      
    }
    pOutAddr +=  JPEG_ConvertorParams.BlockSize;
  }

  return numberMCU;
}

/**
  * @brief  Convert RGB to YCbCr 4:4:4 blocks pixels  
  * @param  pInBuffer  : pointer to input RGB888/ARGB8888 frame buffer.
  * @param  pOutBuffer : pointer to output YCbCr blocks buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from RGB to YCbCr
  */
static uint32_t JPEG_ARGB_MCU_YCbCr444_ConvertBlocks (uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j, currentMCU, xRef,yRef, colones;

  uint32_t refline;
  int32_t ycomp, crcomp, cbcomp, offset;
  
  uint32_t red, green, blue;
  
  uint8_t *pOutAddr;
  uint8_t *pInAddr;

  numberMCU = ((3 * DataCount) / (JPEG_BYTES_PER_PIXEL * YCBCR_444_BLOCK_SIZE));

  currentMCU = BlockIndex;
  *ConvertedDataCount = numberMCU * JPEG_ConvertorParams.BlockSize;

  pOutAddr = &pOutBuffer[0];

  while(currentMCU < (numberMCU + BlockIndex))
  {
    xRef = ((currentMCU *JPEG_ConvertorParams.H_factor) / JPEG_ConvertorParams.WidthExtend)*JPEG_ConvertorParams.V_factor;
    
    yRef = ((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend);
    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    if(((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend) == 0)
    {
      colones = JPEG_ConvertorParams.H_factor - JPEG_ConvertorParams.LineOffset;
    }
    else
    {
      colones = JPEG_ConvertorParams.H_factor;
    }    
    offset = 0;

    for(i= 0; i <  JPEG_ConvertorParams.V_factor; i++)
    {
      pInAddr = &pInBuffer[0] ;
      
      for(j=0; j < colones; j++)
      {
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_BLUE_OFFSET/8)) ;
#endif
        ycomp  = (int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue));
        cbcomp = (int32_t)(*(RED_CB_LUT + red)) + (int32_t)(*(GREEN_CB_LUT + green)) + (int32_t)(*(BLUE_CB_RED_CR_LUT + blue)) + 128;
        crcomp = (int32_t)(*(BLUE_CB_RED_CR_LUT + red)) + (int32_t)(*(GREEN_CR_LUT + green)) + (int32_t)(*(BLUE_CR_LUT + blue)) + 128;
        
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset]))  = (ycomp);
        (*(pOutAddr + JPEG_ConvertorParams.Cb_MCU_LUT[offset])) = (cbcomp);
        (*(pOutAddr + JPEG_ConvertorParams.Cr_MCU_LUT[offset])) = (crcomp);
        
        pInAddr += JPEG_BYTES_PER_PIXEL;
        offset++;
      }
      offset += (JPEG_ConvertorParams.H_factor - colones);
      refline += JPEG_ConvertorParams.ScaledWidth;
    }
    pOutAddr +=  JPEG_ConvertorParams.BlockSize;
  }

  return numberMCU;
}

/**
  * @brief  Convert RGB to Gray blocks pixels  
  * @param  pInBuffer  : pointer to input RGB888/ARGB8888 blocks.
  * @param  pOutBuffer : pointer to output Gray blocks buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from RGB to Gray
  */
static uint32_t JPEG_ARGB_MCU_Gray_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j, currentMCU, xRef,yRef, colones;

  uint32_t refline;
  int32_t offset;
  
  uint32_t red, green, blue;
  
  uint8_t *pOutAddr;
  uint8_t *pInAddr;
  uint8_t ycomp;

  numberMCU = (DataCount / (JPEG_BYTES_PER_PIXEL * GRAY_444_BLOCK_SIZE));

  currentMCU = BlockIndex;
  *ConvertedDataCount = numberMCU * GRAY_444_BLOCK_SIZE;

  pOutAddr = &pOutBuffer[0];

  while(currentMCU < (numberMCU + BlockIndex))
  {
    xRef = ((currentMCU *JPEG_ConvertorParams.H_factor) / JPEG_ConvertorParams.WidthExtend)*JPEG_ConvertorParams.V_factor;
    
    yRef = ((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend);
    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    if(((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend) == 0)
    {
      colones = JPEG_ConvertorParams.H_factor - JPEG_ConvertorParams.LineOffset;
    }
    else
    {
      colones = JPEG_ConvertorParams.H_factor;
    }    
    offset = 0;

    for(i= 0; i <  JPEG_ConvertorParams.V_factor; i++)
    {
      pInAddr = &pInBuffer[0] ;
      
      for(j=0; j < colones; j++)
      {
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + refline + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + refline + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + refline + JPEG_BLUE_OFFSET/8)) ;
#endif
        ycomp  = (uint8_t)((int32_t)(*(RED_Y_LUT + red)) + (int32_t)(*(GREEN_Y_LUT + green)) + (int32_t)(*(BLUE_Y_LUT + blue)));
        
        (*(pOutAddr + offset)) = (ycomp);
        
        pInAddr += JPEG_BYTES_PER_PIXEL;
        offset++;
      }
      offset += (JPEG_ConvertorParams.H_factor - colones);
      refline += JPEG_ConvertorParams.ScaledWidth;
    }
    pOutAddr +=  JPEG_ConvertorParams.BlockSize;
  }

  return numberMCU;
}

/**
  * @brief  Convert RGB to YCCK blocks pixels  
  * @param  pInBuffer  : pointer to input RGB888/ARGB8888 blocks.
  * @param  pOutBuffer : pointer to output YCCK blocks buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from RGB to YCCK
  */
static uint32_t JPEG_ARGB_MCU_YCCK_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j, currentMCU, xRef,yRef, colones;

  uint32_t refline;
  uint32_t offset;
  
  int32_t red, green,  blue, color_k ;
  
  uint8_t *pOutAddr;
  uint8_t *pInAddr;
  
  numberMCU = ((3 * DataCount) / (JPEG_BYTES_PER_PIXEL * CMYK_444_BLOCK_SIZE));
  *ConvertedDataCount = numberMCU * CMYK_444_BLOCK_SIZE;
  
  currentMCU = BlockIndex;

  pOutAddr = &pOutBuffer[0];

  while(currentMCU < (numberMCU + BlockIndex))
  {        
    xRef = ((currentMCU *JPEG_ConvertorParams.H_factor) / JPEG_ConvertorParams.WidthExtend)*JPEG_ConvertorParams.V_factor;
    
    yRef = ((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend);

    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    if(((currentMCU *JPEG_ConvertorParams.H_factor) % JPEG_ConvertorParams.WidthExtend) == 0)
    {
      colones = JPEG_ConvertorParams.H_factor - JPEG_ConvertorParams.LineOffset;
    }
    else
    {
      colones = JPEG_ConvertorParams.H_factor;
    }    
    offset = 0;
    for(i= 0; i <  JPEG_ConvertorParams.V_factor; i++)
    {  
      pInAddr = &pInBuffer[refline];
      
      for(j=0; j < colones; j++)
      {
#if (JPEG_RGB_FORMAT == JPEG_RGB565)
        red   = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_RED_MASK)   >> JPEG_RED_OFFSET) ;
        green = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_GREEN_MASK) >> JPEG_GREEN_OFFSET) ;
        blue  = (((*(__IO uint16_t *)(pInAddr + refline)) & JPEG_RGB565_BLUE_MASK)  >> JPEG_BLUE_OFFSET) ;
        red   = (red << 3)   | (red >> 2);
        green = (green << 2) | (green >> 4);
        blue  = (blue << 3)  | (blue >> 2);
#else
        red   = (*(pInAddr + JPEG_RED_OFFSET/8)) ;
        green = (*(pInAddr + JPEG_GREEN_OFFSET/8)) ;
        blue  = (*(pInAddr + JPEG_BLUE_OFFSET/8)) ;
#endif
        color_k = MAX(MAX(red,green),blue);
        kBlocks[i][j]=color_k;
        
        (*(pOutAddr + JPEG_ConvertorParams.Y_MCU_LUT[offset]))  = red * 255 / color_k;
        (*(pOutAddr + JPEG_ConvertorParams.Cb_MCU_LUT[offset])) = green * 255 / color_k;
        (*(pOutAddr + JPEG_ConvertorParams.Cr_MCU_LUT[offset])) = blue * 255 / color_k;
        
        pInAddr += JPEG_BYTES_PER_PIXEL;
        offset++;
      }
      offset += (JPEG_ConvertorParams.H_factor - colones);
      refline += JPEG_ConvertorParams.ScaledWidth;
    }
    JPEG_Set_K_Blocks(pOutAddr, kBlocks, JPEG_ConvertorParams.ChromaSubsampling);
    pOutAddr +=  JPEG_ConvertorParams.BlockSize;
  }

  return numberMCU;
}

/**
  * @brief  Retrieve Encoding RGB to YCbCr color conversion function and block number  
  * @param  pJpegInfo  : JPEG_ConfTypeDef that contains the JPEG image information.
  *                      These info are available in the HAL callback "HAL_JPEG_InfoReadyCallback".
  * @param  pFunction  : pointer to JPEG_RGBToYCbCr_Convert_Function , used to Retrieve the color conversion function 
  *                      depending of the jpeg image color space and chroma sampling info. 
  * @param ImageNbMCUs : pointer to uint32_t, used to Retrieve the total number of MCU blocks in the jpeg image.  
  * @retval HAL status : HAL_OK or HAL_ERROR.
  */
HAL_StatusTypeDef JPEG_GetEncodeColorConvertFunc(JPEG_ConfTypeDef *pJpegInfo, JPEG_RGBToYCbCr_Convert_Function *pFunction, uint32_t *ImageNbMCUs)
{
  uint32_t hMCU, vMCU;

  JPEG_ConvertorParams.ColorSpace = pJpegInfo->ColorSpace;
  JPEG_ConvertorParams.ChromaSubsampling = pJpegInfo->ChromaSubsampling;
  
  if(JPEG_ConvertorParams.ColorSpace == JPEG_YCBCR_COLORSPACE) 
  {
    if(JPEG_ConvertorParams.ChromaSubsampling == JPEG_420_SUBSAMPLING)
    {
      *pFunction =  JPEG_ARGB_MCU_YCbCr420_ConvertBlocks;  
    }
    else if (JPEG_ConvertorParams.ChromaSubsampling == JPEG_422_SUBSAMPLING)
    {
      *pFunction = JPEG_ARGB_MCU_YCbCr422_ConvertBlocks;  
    }
    else if (JPEG_ConvertorParams.ChromaSubsampling == JPEG_444_SUBSAMPLING)
    {
      *pFunction = JPEG_ARGB_MCU_YCbCr444_ConvertBlocks;  
    }
    else
    {  
       return HAL_ERROR; /* Chroma SubSampling Not supported*/
    }
  }
  else if(JPEG_ConvertorParams.ColorSpace == JPEG_GRAYSCALE_COLORSPACE)
  {
    *pFunction = JPEG_ARGB_MCU_Gray_ConvertBlocks;  
  }
  else if(JPEG_ConvertorParams.ColorSpace == JPEG_CMYK_COLORSPACE)
  {
    *pFunction = JPEG_ARGB_MCU_YCCK_ConvertBlocks;      
  } 
  else
  {  
     return HAL_ERROR; /* Color space Not supported*/
  }

  JPEG_ConvertorParams.ImageWidth = pJpegInfo->ImageWidth;
  JPEG_ConvertorParams.ImageHeight = pJpegInfo->ImageHeight;
  JPEG_ConvertorParams.ImageSize_Bytes = pJpegInfo->ImageWidth * pJpegInfo->ImageHeight * JPEG_BYTES_PER_PIXEL;

  if((JPEG_ConvertorParams.ChromaSubsampling == JPEG_420_SUBSAMPLING) || (JPEG_ConvertorParams.ChromaSubsampling == JPEG_422_SUBSAMPLING))
  {
    JPEG_ConvertorParams.LineOffset = JPEG_ConvertorParams.ImageWidth % 16;
   
    JPEG_ConvertorParams.Y_MCU_LUT = JPEG_Y_MCU_LUT;
    
    if(JPEG_ConvertorParams.LineOffset != 0)
    {
      JPEG_ConvertorParams.LineOffset = 16 - JPEG_ConvertorParams.LineOffset;  
    }

    JPEG_ConvertorParams.H_factor = 16;
    
    if(JPEG_ConvertorParams.ChromaSubsampling == JPEG_420_SUBSAMPLING)
    {
      JPEG_ConvertorParams.V_factor  = 16;
      
      if(JPEG_ConvertorParams.ColorSpace == JPEG_YCBCR_COLORSPACE)
      {
        JPEG_ConvertorParams.BlockSize =  YCBCR_420_BLOCK_SIZE;
      }

      JPEG_ConvertorParams.Cb_MCU_LUT = JPEG_Cb_MCU_420_LUT;
      JPEG_ConvertorParams.Cr_MCU_LUT = JPEG_Cr_MCU_420_LUT;

      JPEG_ConvertorParams.K_MCU_LUT  = JPEG_K_MCU_420_LUT;      
    }
    else /* 4:2:2*/
    {
      JPEG_ConvertorParams.V_factor = 8;
      
      if(JPEG_ConvertorParams.ColorSpace == JPEG_YCBCR_COLORSPACE)
      {
        JPEG_ConvertorParams.BlockSize =  YCBCR_422_BLOCK_SIZE;
      }     

      JPEG_ConvertorParams.Cb_MCU_LUT = JPEG_Cb_MCU_422_LUT;
      JPEG_ConvertorParams.Cr_MCU_LUT = JPEG_Cr_MCU_422_LUT;
      
      JPEG_ConvertorParams.K_MCU_LUT  = JPEG_K_MCU_422_LUT;        
    }    
  }
  else if(JPEG_ConvertorParams.ChromaSubsampling == JPEG_444_SUBSAMPLING)
  {
    JPEG_ConvertorParams.LineOffset = JPEG_ConvertorParams.ImageWidth % 8;
    
    JPEG_ConvertorParams.Y_MCU_LUT = JPEG_Y_MCU_444_LUT;

    JPEG_ConvertorParams.Cb_MCU_LUT = JPEG_Cb_MCU_444_LUT;
    JPEG_ConvertorParams.Cr_MCU_LUT = JPEG_Cr_MCU_444_LUT;
    
    JPEG_ConvertorParams.K_MCU_LUT  = JPEG_K_MCU_444_LUT;    
        
    if(JPEG_ConvertorParams.LineOffset != 0)
    {
      JPEG_ConvertorParams.LineOffset = 8 - JPEG_ConvertorParams.LineOffset;  
    }
    JPEG_ConvertorParams.H_factor = 8;
    JPEG_ConvertorParams.V_factor = 8;
 
    if(JPEG_ConvertorParams.ColorSpace == JPEG_YCBCR_COLORSPACE)
    {
      JPEG_ConvertorParams.BlockSize = YCBCR_444_BLOCK_SIZE;
    }
    if(JPEG_ConvertorParams.ColorSpace == JPEG_CMYK_COLORSPACE)
    {
      JPEG_ConvertorParams.BlockSize = CMYK_444_BLOCK_SIZE;
    }    
    else if(JPEG_ConvertorParams.ColorSpace == JPEG_GRAYSCALE_COLORSPACE)
    {
      JPEG_ConvertorParams.BlockSize = GRAY_444_BLOCK_SIZE;
    }
   
  }
  else
  {
     return HAL_ERROR; /* Not supported*/
  }
  

  JPEG_ConvertorParams.WidthExtend = JPEG_ConvertorParams.ImageWidth + JPEG_ConvertorParams.LineOffset;
  JPEG_ConvertorParams.ScaledWidth = JPEG_BYTES_PER_PIXEL * JPEG_ConvertorParams.ImageWidth; 
  
  hMCU = (JPEG_ConvertorParams.ImageWidth / JPEG_ConvertorParams.H_factor);
  if((JPEG_ConvertorParams.ImageWidth % JPEG_ConvertorParams.H_factor) != 0)
  {
    hMCU++; /*+1 for horizenatl incomplete MCU */                
  }

  vMCU = (JPEG_ConvertorParams.ImageHeight / JPEG_ConvertorParams.V_factor);
  if((JPEG_ConvertorParams.ImageHeight % JPEG_ConvertorParams.V_factor) != 0)
  {
    vMCU++; /*+1 for vertical incomplete MCU */                
  }
  JPEG_ConvertorParams.MCU_Total_Nb = (hMCU * vMCU);
  *ImageNbMCUs = JPEG_ConvertorParams.MCU_Total_Nb;
  
  return HAL_OK;
}
#endif /* USE_JPEG_ENCODER == 1 */

#if (USE_JPEG_DECODER == 1)
/**
  * @brief  Convert YCbCr 4:2:0 blocks to RGB pixels  
  * @param  pInBuffer  : pointer to input YCbCr blocks buffer.
  * @param  pOutBuffer : pointer to output RGB888/ARGB8888 frame buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from YCbCr to RGB 
  */

static uint32_t JPEG_MCU_YCbCr420_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j,k, currentMCU, xRef,yRef;

  uint32_t refline;
  int32_t ycomp, crcomp, cbcomp;

  int32_t c_red, c_blue, c_green;
  
  uint8_t *pOutAddr, *pOutAddr2;
  uint8_t *pChrom, *pLum;
  
  numberMCU = DataCount / YCBCR_420_BLOCK_SIZE;
  currentMCU = BlockIndex;
  

  while(currentMCU < (numberMCU + BlockIndex))
  {
    xRef = ((currentMCU *16) / JPEG_ConvertorParams.WidthExtend)*16;
    
    yRef = ((currentMCU *16) % JPEG_ConvertorParams.WidthExtend);
    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    pChrom = pInBuffer + 256; /* pChroma = pInBuffer + 4*64 */
    
    pLum = pInBuffer;
    
    for(i= 0; i <  16; i+=2)
    {
      if(i == 8)
      {
        pLum = pInBuffer + 128;
      }
      
      if(refline < JPEG_ConvertorParams.ImageSize_Bytes)
      {
        pOutAddr = pOutBuffer + refline;
        pOutAddr2 = pOutAddr + JPEG_ConvertorParams.ScaledWidth;
        
        for(k= 0; k<2; k++)
        {
          for(j=0; j < 8; j+=2)
          {           
            cbcomp = (int32_t)(*(pChrom));
            c_blue = (int32_t)(*(CB_BLUE_LUT + cbcomp));
            
            crcomp = (int32_t)(*(pChrom + 64));
            c_red = (int32_t)(*(CR_RED_LUT + crcomp));          
            
            c_green = ((int32_t)(*(CR_GREEN_LUT + crcomp)) + (int32_t)(*(CB_GREEN_LUT + cbcomp))) >> 16;      
          
            
            
#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
          
            ycomp = (int32_t)(*(pLum +j));
            
            *(__IO uint32_t *)pOutAddr = 
              (CLAMP(ycomp + c_red) << JPEG_RED_OFFSET)     | \
              (CLAMP( ycomp + c_green) << JPEG_GREEN_OFFSET) | \
              (CLAMP(ycomp + c_blue) << JPEG_BLUE_OFFSET);
            /**********/
            ycomp = (int32_t)(*(pLum +j +1));
            
            *((__IO uint32_t *)(pOutAddr + 4)) = 
              (CLAMP(ycomp + c_red) << JPEG_RED_OFFSET)     | \
              (CLAMP( ycomp + c_green) << JPEG_GREEN_OFFSET) | \
              (CLAMP(ycomp + c_blue) << JPEG_BLUE_OFFSET);
            
            /**********/
            ycomp = (int32_t)(*(pLum +j +8));
            
            *(__IO uint32_t *)pOutAddr2 = 
              (CLAMP(ycomp + c_red) << JPEG_RED_OFFSET)     | \
              (CLAMP( ycomp + c_green) << JPEG_GREEN_OFFSET) | \
              (CLAMP(ycomp + c_blue) << JPEG_BLUE_OFFSET);
            
            /**********/
            ycomp = (int32_t)(*(pLum +j +8 +1));
            
            *((__IO uint32_t *)(pOutAddr2 +4)) = 
              (CLAMP(ycomp + c_red) << JPEG_RED_OFFSET)     | \
              (CLAMP( ycomp + c_green) << JPEG_GREEN_OFFSET) | \
              (CLAMP(ycomp + c_blue) << JPEG_BLUE_OFFSET);

            
#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
          
           ycomp = (int32_t)(*(pLum +j));
          
            pOutAddr[JPEG_RED_OFFSET/8] = CLAMP(ycomp + c_red);
            pOutAddr[JPEG_GREEN_OFFSET/8] = CLAMP(ycomp + c_green);
            pOutAddr[JPEG_BLUE_OFFSET/8] = CLAMP(ycomp + c_blue);
            
            /**********/
            ycomp = (int32_t)(*(pLum +j +1));

            pOutAddr[3 + JPEG_RED_OFFSET/8] = CLAMP(ycomp + c_red);
            pOutAddr[3 + JPEG_GREEN_OFFSET/8] = CLAMP(ycomp + c_green);
            pOutAddr[3 + JPEG_BLUE_OFFSET/8] = CLAMP(ycomp + c_blue);

            /**********/            
            ycomp = (int32_t)(*(pLum +j +8));

            pOutAddr2[JPEG_RED_OFFSET/8] = CLAMP(ycomp + c_red);
            pOutAddr2[JPEG_GREEN_OFFSET/8] = CLAMP(ycomp + c_green);
            pOutAddr2[JPEG_BLUE_OFFSET/8] = CLAMP(ycomp + c_blue);
            
            /**********/
            ycomp = (int32_t)(*(pLum +j +8 +1));            
            
            pOutAddr2[3+ JPEG_RED_OFFSET/8] = CLAMP(ycomp + c_red);
            pOutAddr2[3 + JPEG_GREEN_OFFSET/8] = CLAMP(ycomp + c_green);
            pOutAddr2[3 + JPEG_BLUE_OFFSET/8] = CLAMP(ycomp + c_blue);
 
#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
          
            ycomp = (int32_t)(*(pLum +j));
            
            *(__IO uint16_t *)pOutAddr = 
              ((CLAMP(ycomp + c_red) >> 3) << JPEG_RED_OFFSET)     | \
              ((CLAMP( ycomp + c_green) >> 2) << JPEG_GREEN_OFFSET) | \
              ((CLAMP(ycomp + c_blue) >> 3) << JPEG_BLUE_OFFSET);
            /**********/
            ycomp = (int32_t)(*(pLum +j +1));
            
            *((__IO uint16_t *)(pOutAddr + 2)) = 
              ((CLAMP(ycomp + c_red) >> 3) << JPEG_RED_OFFSET)     | \
              ((CLAMP( ycomp + c_green) >> 2) << JPEG_GREEN_OFFSET) | \
              ((CLAMP(ycomp + c_blue) >> 3) << JPEG_BLUE_OFFSET);
            
            /**********/
            ycomp = (int32_t)(*(pLum +j +8));
            
            *(__IO uint16_t *)pOutAddr2 = 
              ((CLAMP(ycomp + c_red) >> 3) << JPEG_RED_OFFSET)     | \
              ((CLAMP( ycomp + c_green) >> 2) << JPEG_GREEN_OFFSET) | \
              ((CLAMP(ycomp + c_blue) >> 3) << JPEG_BLUE_OFFSET);
            
            /**********/
            ycomp = (int32_t)(*(pLum +j +8 +1));
            
            *((__IO uint16_t *)(pOutAddr2 +2)) = 
              ((CLAMP(ycomp + c_red) >> 3) << JPEG_RED_OFFSET)     | \
              ((CLAMP( ycomp + c_green) >> 2) << JPEG_GREEN_OFFSET) | \
              ((CLAMP(ycomp + c_blue) >> 3) << JPEG_BLUE_OFFSET);         
#endif /* JPEG_RGB_FORMAT */          
          
            pOutAddr += JPEG_BYTES_PER_PIXEL * 2;
            pOutAddr2 += JPEG_BYTES_PER_PIXEL * 2;
          
            pChrom++;
          }
          pLum += 64;                      
        }

        pLum = pLum - 128 + 16;
        
        refline += 2*JPEG_ConvertorParams.ScaledWidth;          
      }
    }   
    
    pInBuffer +=  YCBCR_420_BLOCK_SIZE;
  }
  return numberMCU;
}

/**
  * @brief  Convert YCbCr 4:2:2 blocks to RGB pixels  
  * @param  pInBuffer  : pointer to input YCbCr blocks buffer.
  * @param  pOutBuffer : pointer to output RGB888/ARGB8888 frame buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from YCbCr to RGB 
  */
static uint32_t JPEG_MCU_YCbCr422_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer,
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j,k, currentMCU, xRef,yRef;

  uint32_t refline;
  int32_t ycomp, crcomp, cbcomp;

  int32_t c_red, c_blue, c_green;
  
  uint8_t *pOutAddr;
  uint8_t *pChrom, *pLum;
  
  numberMCU = DataCount / YCBCR_422_BLOCK_SIZE;
  currentMCU = BlockIndex;
  

  while(currentMCU < (numberMCU + BlockIndex))
  {
    xRef = ((currentMCU *16) / JPEG_ConvertorParams.WidthExtend)*8;
    
    yRef = ((currentMCU *16) % JPEG_ConvertorParams.WidthExtend);
    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    pChrom = pInBuffer + 128; /* pChroma = pInBuffer + 2*64 */
    
    pLum = pInBuffer;
    
    for(i= 0; i <  8; i++)
    {
       if(refline < JPEG_ConvertorParams.ImageSize_Bytes)
      {
        pOutAddr = pOutBuffer + refline;
        
        for(k= 0; k<2; k++)
        {
          for(j=0; j < 8; j+=2)
          {           
            cbcomp = (int32_t)(*(pChrom));
            c_blue = (int32_t)(*(CB_BLUE_LUT + cbcomp));
            
            crcomp = (int32_t)(*(pChrom + 64));
            c_red = (int32_t)(*(CR_RED_LUT + crcomp));          
            
            c_green = ((int32_t)(*(CR_GREEN_LUT + crcomp)) + (int32_t)(*(CB_GREEN_LUT + cbcomp))) >> 16;      
          
            
            
#if (JPEG_RGB_FORMAT == JPEG_ARGB8888) 
          
            ycomp = (int32_t)(*(pLum +j));
            
            *(__IO uint32_t *)pOutAddr = 
              (CLAMP(ycomp + c_red) << JPEG_RED_OFFSET)     | \
              (CLAMP( ycomp + c_green) << JPEG_GREEN_OFFSET) | \
              (CLAMP(ycomp + c_blue) << JPEG_BLUE_OFFSET);
            /**********/
            ycomp = (int32_t)(*(pLum +j +1));
            
            *((__IO uint32_t *)(pOutAddr + 4)) = 
              (CLAMP(ycomp + c_red) << JPEG_RED_OFFSET)     | \
              (CLAMP( ycomp + c_green) << JPEG_GREEN_OFFSET) | \
              (CLAMP(ycomp + c_blue) << JPEG_BLUE_OFFSET);         
            
#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
          
           ycomp = (int32_t)(*(pLum +j));
          
            pOutAddr[JPEG_RED_OFFSET/8] = CLAMP(ycomp + c_red);
            pOutAddr[JPEG_GREEN_OFFSET/8] = CLAMP(ycomp + c_green);
            pOutAddr[JPEG_BLUE_OFFSET/8] = CLAMP(ycomp + c_blue);
            
            /**********/
            ycomp = (int32_t)(*(pLum +j +1));

            pOutAddr[3 + JPEG_RED_OFFSET/8] = CLAMP(ycomp + c_red);
            pOutAddr[3 + JPEG_GREEN_OFFSET/8] = CLAMP(ycomp + c_green);
            pOutAddr[3 + JPEG_BLUE_OFFSET/8] = CLAMP(ycomp + c_blue);

#elif (JPEG_RGB_FORMAT == JPEG_RGB565)  
          
            ycomp = (int32_t)(*(pLum +j));
            
            *(__IO uint16_t *)pOutAddr = 
              ((CLAMP(ycomp + c_red) >> 3) << JPEG_RED_OFFSET)     | \
              ((CLAMP( ycomp + c_green) >> 2) << JPEG_GREEN_OFFSET) | \
              ((CLAMP(ycomp + c_blue) >> 3) << JPEG_BLUE_OFFSET);
            /**********/
            ycomp = (int32_t)(*(pLum +j +1));
            
            *((__IO uint16_t *)(pOutAddr + 2)) = 
              ((CLAMP(ycomp + c_red) >> 3) << JPEG_RED_OFFSET)     | \
              ((CLAMP( ycomp + c_green) >> 2) << JPEG_GREEN_OFFSET) | \
              ((CLAMP(ycomp + c_blue) >> 3) << JPEG_BLUE_OFFSET);         
          
#endif /* JPEG_RGB_FORMAT*/          
          
            pOutAddr += JPEG_BYTES_PER_PIXEL * 2;
          
            pChrom++;
          }
          pLum += 64;                      
        }
        
        pLum = pLum - 128 + 8;
        
        refline += JPEG_ConvertorParams.ScaledWidth;          
      }
    }   
    
    pInBuffer +=  YCBCR_422_BLOCK_SIZE;
  }
  return numberMCU;
}

/**
  * @brief  Convert YCbCr 4:4:4 blocks to RGB pixels  
  * @param  pInBuffer  : pointer to input YCbCr blocks buffer.
  * @param  pOutBuffer : pointer to output RGB888/ARGB8888 frame buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from YCbCr to RGB 
  */
static uint32_t JPEG_MCU_YCbCr444_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer,
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j, currentMCU, xRef,yRef;

  uint32_t refline;
  int32_t ycomp, crcomp, cbcomp;
  
  int32_t c_red, c_blue, c_green;
  
  uint8_t *pOutAddr;
  uint8_t *pChrom, *pLum;
  
  numberMCU = DataCount / YCBCR_444_BLOCK_SIZE;
  currentMCU = BlockIndex;
  

  while(currentMCU < (numberMCU + BlockIndex))
  {

    xRef = ((currentMCU *8) / JPEG_ConvertorParams.WidthExtend)*8;
    
    yRef = ((currentMCU *8) % JPEG_ConvertorParams.WidthExtend);
    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;   
    
    pChrom = pInBuffer + 64; /* pChroma = pInBuffer + 4*64 */
    
    pLum = pInBuffer;
    
    for(i= 0; i <  8; i++)
    {
      if(refline < JPEG_ConvertorParams.ImageSize_Bytes)
      {
        pOutAddr = pOutBuffer+ refline;
        

          for(j=0; j < 8; j++)
          {           
            cbcomp = (int32_t)(*pChrom);
            c_blue = (int32_t)(*(CB_BLUE_LUT + cbcomp));
            
            crcomp = (int32_t)(*(pChrom + 64));
            c_red = (int32_t)(*(CR_RED_LUT + crcomp));          
            
            c_green = ((int32_t)(*(CR_GREEN_LUT + crcomp)) + (int32_t)(*(CB_GREEN_LUT + cbcomp))) >> 16;      
                      
#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
          
            ycomp = (int32_t)(*(pLum +j));
            
            *(__IO uint32_t *)pOutAddr = 
              (CLAMP(ycomp + c_red) << JPEG_RED_OFFSET)     | \
              (CLAMP( ycomp + c_green) << JPEG_GREEN_OFFSET) | \
              (CLAMP(ycomp + c_blue) << JPEG_BLUE_OFFSET);       
            
#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
            
            ycomp = (int32_t)(*(pLum +j));
          
            pOutAddr[JPEG_RED_OFFSET/8] = CLAMP(ycomp + c_red);
            pOutAddr[JPEG_GREEN_OFFSET/8] = CLAMP(ycomp + c_green);
            pOutAddr[JPEG_BLUE_OFFSET/8] = CLAMP(ycomp + c_blue);        

#elif (JPEG_RGB_FORMAT == JPEG_RGB565)  
            
            ycomp = (int32_t)(*(pLum +j));
          
            *(__IO uint16_t *)pOutAddr = 
              ((CLAMP(ycomp + c_red) >> 3) << JPEG_RED_OFFSET)     | \
              ((CLAMP( ycomp + c_green) >> 2) << JPEG_GREEN_OFFSET) | \
              ((CLAMP(ycomp + c_blue) >> 3) << JPEG_BLUE_OFFSET);      
            
#endif /* JPEG_RGB_FORMAT */          
          
            pOutAddr += JPEG_BYTES_PER_PIXEL;
          
            pChrom++;
          }
          pLum += 8;

        refline += JPEG_ConvertorParams.ScaledWidth;          
      }
    }   
    
    pInBuffer +=  YCBCR_444_BLOCK_SIZE;
  }
  return numberMCU;
}

/**
  * @brief  Convert Y Gray blocks to RGB pixels  
  * @param  pInBuffer  : pointer to input Luminance Y blocks buffer.
  * @param  pOutBuffer : pointer to output RGB888/ARGB8888 frame buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from YCbCr to RGB 
  */
static uint32_t JPEG_MCU_Gray_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer,
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{
  uint32_t numberMCU;
  uint32_t  currentMCU, xRef,yRef;
  uint32_t refline;
  

  uint32_t i,j, ySample;
  uint8_t *pOutAddr,  *pLum;

  
  numberMCU = DataCount / GRAY_444_BLOCK_SIZE;
  currentMCU = BlockIndex;
  
  while(currentMCU < (numberMCU + BlockIndex))
  {
    xRef = ((currentMCU *8) / JPEG_ConvertorParams.WidthExtend)*8;
    
    yRef = ((currentMCU *8) % JPEG_ConvertorParams.WidthExtend);
    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);
    
    currentMCU++;
  
    pLum = pInBuffer;
    
    for(i= 0; i <  8; i++)
    { 
      pOutAddr = pOutBuffer + refline;
      if(refline < JPEG_ConvertorParams.ImageSize_Bytes)
      {  
        for(j=0; j < 8; j++)
        { 
          ySample =   (uint32_t)(*pLum);

#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
          
          *(__IO uint32_t *)pOutAddr = ySample |  (ySample << 8) | (ySample << 16);
          
#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
          
          pOutAddr[0] =  ySample;
          pOutAddr[1] =  ySample;
          pOutAddr[2] =  ySample;     
 
#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
            
          *(__IO uint16_t *)pOutAddr = ((ySample >> 3) << 11) |  ((ySample >> 2) << 5) | (ySample >> 3);     
          
#endif /* JPEG_RGB_FORMAT */          
          
          pOutAddr += JPEG_BYTES_PER_PIXEL;
          pLum++;
        }

        refline += JPEG_ConvertorParams.ScaledWidth;        
      }
    }
    
    pInBuffer +=  GRAY_444_BLOCK_SIZE;    
  }
  return numberMCU;
}

/**
  * @brief  Convert CMYK blocks to RGB pixels  
  * @param  pInBuffer  : pointer to input CMYK blocks buffer.
  * @param  pOutBuffer : pointer to output RGB888/ARGB8888 frame buffer.
  * @param  BlockIndex : index of the input buffer first block in the final image.
  * @param  DataCount  : number of bytes in the input buffer .
  * @param  ConvertedDataCount  : number of converted bytes from input buffer.  
  * @retval Number of blocks converted from CMYK to RGB 
  */
static uint32_t JPEG_MCU_YCCK_ARGB_ConvertBlocks(uint8_t *pInBuffer, 
                                      uint8_t *pOutBuffer, 
                                      uint32_t BlockIndex,
                                      uint32_t DataCount,
                                      uint32_t *ConvertedDataCount)
{  
  uint32_t numberMCU;
  uint32_t i,j, currentMCU, xRef,yRef;

  uint32_t refline;
  int32_t color_k;
  
  int32_t c_red, c_blue, c_green;
  
  uint8_t *pOutAddr, *pChrom;
  
  numberMCU = DataCount / CMYK_444_BLOCK_SIZE;
  currentMCU = BlockIndex;
  

  while(currentMCU < (numberMCU + BlockIndex))
  {

    xRef = ((currentMCU *8) / JPEG_ConvertorParams.WidthExtend)*8;
    
    yRef = ((currentMCU *8) % JPEG_ConvertorParams.WidthExtend);
    
    refline = JPEG_ConvertorParams.ScaledWidth * xRef + (JPEG_BYTES_PER_PIXEL*yRef);

    currentMCU++;
    
    pChrom = pInBuffer;
    
    for(i= 0; i <  8; i++)
    {
      if(refline < JPEG_ConvertorParams.ImageSize_Bytes)
      {
        pOutAddr = pOutBuffer+ refline;        

          for(j=0; j < 8; j++)
          {           
            color_k = (int32_t)(*(pChrom + 192));
            c_red = (color_k * ((int32_t)(*pChrom)))/255;
            
            c_green = (color_k * (int32_t)(*(pChrom + 64)))/255;
            
            c_blue = (color_k * (int32_t)(*(pChrom + 128)))/255;
                      
#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
          
          *(__IO uint32_t *)pOutAddr = 
            (c_red << JPEG_RED_OFFSET) | \
            (c_green << JPEG_GREEN_OFFSET) | \
            (c_blue << JPEG_BLUE_OFFSET);     
            
#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
            
          pOutAddr[JPEG_RED_OFFSET/8]   =  c_red;
          pOutAddr[JPEG_GREEN_OFFSET/8] =  c_green;
          pOutAddr[JPEG_BLUE_OFFSET/8]  =  c_blue;
          
#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
            
          *(__IO uint16_t *)pOutAddr = 
            ((c_red >> 3) << JPEG_RED_OFFSET)     | \
            ((c_green >> 2) << JPEG_GREEN_OFFSET) | \
            ((c_blue >> 3) << JPEG_BLUE_OFFSET);         
            
#endif /* JPEG_RGB_FORMAT */          
          
            pOutAddr += JPEG_BYTES_PER_PIXEL;
          
            pChrom++;
          }

        refline += JPEG_ConvertorParams.ScaledWidth;          
      }
    }   
    
    pInBuffer +=  CMYK_444_BLOCK_SIZE;
  }
  return numberMCU;
}

/**
  * @brief  Retrieve Decoding YCbCr to RGB color conversion function and block number  
  * @param  pJpegInfo  : JPEG_ConfTypeDef that contains the JPEG image information.
  *                      These info are available in the HAL callback "HAL_JPEG_InfoReadyCallback".
  * @param  pFunction  : pointer to JPEG_YCbCrToRGB_Convert_Function , used to Retrieve the color conversion function 
  *                      depending of the jpeg image color space and chroma sampling info. 
  * @param ImageNbMCUs : pointer to uint32_t, used to Retrieve the total number of MCU blocks in the jpeg image.  
  * @retval HAL status : HAL_OK or HAL_ERROR.
  */
HAL_StatusTypeDef JPEG_GetDecodeColorConvertFunc(JPEG_ConfTypeDef *pJpegInfo, JPEG_YCbCrToRGB_Convert_Function *pFunction, uint32_t *ImageNbMCUs)
{
  uint32_t hMCU, vMCU;

  JPEG_ConvertorParams.ColorSpace = pJpegInfo->ColorSpace;
  JPEG_ConvertorParams.ImageWidth = pJpegInfo->ImageWidth;
  JPEG_ConvertorParams.ImageHeight = pJpegInfo->ImageHeight;
  JPEG_ConvertorParams.ImageSize_Bytes = pJpegInfo->ImageWidth * pJpegInfo->ImageHeight * JPEG_BYTES_PER_PIXEL;
  
  JPEG_ConvertorParams.ChromaSubsampling = pJpegInfo->ChromaSubsampling;  
  if(JPEG_ConvertorParams.ColorSpace == JPEG_YCBCR_COLORSPACE)
  {
    if(JPEG_ConvertorParams.ChromaSubsampling == JPEG_420_SUBSAMPLING)
    {
      *pFunction = JPEG_MCU_YCbCr420_ARGB_ConvertBlocks;

      JPEG_ConvertorParams.LineOffset = JPEG_ConvertorParams.ImageWidth % 16;
      
      if(JPEG_ConvertorParams.LineOffset != 0)
      {
        JPEG_ConvertorParams.LineOffset = 16 - JPEG_ConvertorParams.LineOffset;  
      }
      JPEG_ConvertorParams.H_factor = 16;
      JPEG_ConvertorParams.V_factor  = 16;
    }
    else if(JPEG_ConvertorParams.ChromaSubsampling == JPEG_422_SUBSAMPLING)
    {
      *pFunction =  JPEG_MCU_YCbCr422_ARGB_ConvertBlocks;

      JPEG_ConvertorParams.LineOffset = JPEG_ConvertorParams.ImageWidth % 16;
      
      if(JPEG_ConvertorParams.LineOffset != 0)
      {
        JPEG_ConvertorParams.LineOffset = 16 - JPEG_ConvertorParams.LineOffset;  
      }
      JPEG_ConvertorParams.H_factor = 16;
      JPEG_ConvertorParams.V_factor  = 8;
    }
    else /*4:4:4*/
    {
      *pFunction =  JPEG_MCU_YCbCr444_ARGB_ConvertBlocks;

      JPEG_ConvertorParams.LineOffset = JPEG_ConvertorParams.ImageWidth % 8;
      
      if(JPEG_ConvertorParams.LineOffset != 0)
      {
        JPEG_ConvertorParams.LineOffset = 8 - JPEG_ConvertorParams.LineOffset;  
      }
      JPEG_ConvertorParams.H_factor = 8;
      JPEG_ConvertorParams.V_factor  = 8;
    }   
  }
  else if(JPEG_ConvertorParams.ColorSpace == JPEG_GRAYSCALE_COLORSPACE)
  {
    *pFunction =  JPEG_MCU_Gray_ARGB_ConvertBlocks;

    JPEG_ConvertorParams.LineOffset = JPEG_ConvertorParams.ImageWidth % 8;
      
    if(JPEG_ConvertorParams.LineOffset != 0)
    {
      JPEG_ConvertorParams.LineOffset = 8 - JPEG_ConvertorParams.LineOffset;  
    }
    JPEG_ConvertorParams.H_factor = 8;
    JPEG_ConvertorParams.V_factor  = 8;
  }
  else if(JPEG_ConvertorParams.ColorSpace == JPEG_CMYK_COLORSPACE)
  {
    *pFunction =  JPEG_MCU_YCCK_ARGB_ConvertBlocks;

    JPEG_ConvertorParams.LineOffset = JPEG_ConvertorParams.ImageWidth % 8;
      
    if(JPEG_ConvertorParams.LineOffset != 0)
    {
      JPEG_ConvertorParams.LineOffset = 8 - JPEG_ConvertorParams.LineOffset;  
    }
    JPEG_ConvertorParams.H_factor = 8;
    JPEG_ConvertorParams.V_factor  = 8;
  } 
  else
  {  
    return HAL_ERROR; /* Color space Not supported*/
  }
 
  JPEG_ConvertorParams.WidthExtend = JPEG_ConvertorParams.ImageWidth + JPEG_ConvertorParams.LineOffset;
  JPEG_ConvertorParams.ScaledWidth = JPEG_BYTES_PER_PIXEL * JPEG_ConvertorParams.ImageWidth; 
  
  hMCU = (JPEG_ConvertorParams.ImageWidth / JPEG_ConvertorParams.H_factor);
  if((JPEG_ConvertorParams.ImageWidth % JPEG_ConvertorParams.H_factor) != 0)
  {
    hMCU++; /*+1 for horizenatl incomplete MCU */                
  }

  vMCU = (JPEG_ConvertorParams.ImageHeight / JPEG_ConvertorParams.V_factor);
  if((JPEG_ConvertorParams.ImageHeight % JPEG_ConvertorParams.V_factor) != 0)
  {
    vMCU++; /*+1 for vertical incomplete MCU */                
  }
  JPEG_ConvertorParams.MCU_Total_Nb = (hMCU * vMCU);
  *ImageNbMCUs = JPEG_ConvertorParams.MCU_Total_Nb;

  return HAL_OK;
}

/**
  * @brief  Initializes the YCbCr -> RGB colors conversion Look Up Tables  
  * @param  None
  * @retval None
  */
void JPEG_InitPostProcColorTables(void)
{
  int32_t index, i; 
    
  for (i = 0; i <= 255; i++) 
  {
    index = (i * 2) - 256;
    CR_RED_LUT[i] = ( (((int32_t) ((1.40200 / 2) * (1L << 16)))  * index) + ((int32_t) 1 << (16 - 1))) >> 16;
    
    CB_BLUE_LUT[i] = ( (((int32_t) ((1.77200 / 2) * (1L << 16)))  * index) + ((int32_t) 1 << (16 - 1))) >> 16;
    
    CR_GREEN_LUT[i] = (-((int32_t) ((0.71414 / 2) * (1L << 16)))) * index;
    CB_GREEN_LUT[i] = (-((int32_t) ((0.34414 / 2) * (1L << 16)))) * index;
  }  
}
#endif /* USE_JPEG_DECODER == 1 */

/**
  * @brief  Initializes the colors conversion Look Up Tables  
  * @param  None
  * @retval None
  */
void JPEG_InitColorTables(void)
{

#if (USE_JPEG_ENCODER == 1)
  JPEG_InitPreProcColorTables();
  JPEG_Init_MCU_LUT();
#endif

#if (USE_JPEG_DECODER == 1)
  JPEG_InitPostProcColorTables();
#endif

}

#if (USE_JPEG_ENCODER == 1)
/**
  * @brief  Initializes the RGB -> YCbCr colors conversion Look Up Tables  
  * @param  None
  * @retval None
  */
void JPEG_InitPreProcColorTables(void)
{
  int32_t i; 

  for (i = 0; i <= 255; i++) 
  {
    RED_Y_LUT[i]           = ((  ((int32_t) ((0.299 )  * (1L << 16)))  * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;
    GREEN_Y_LUT[i]         = ((  ((int32_t) ((0.587 )  * (1L << 16)))  * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;
    BLUE_Y_LUT[i]          = ((  ((int32_t) ((0.114 )  * (1L << 16)))  * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;
                          
    RED_CB_LUT[i]          = (((-((int32_t) ((0.1687 ) * (1L << 16)))) * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;
    GREEN_CB_LUT[i]        = (((-((int32_t) ((0.3313 ) * (1L << 16)))) * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;

    /* BLUE_CB_LUT and RED_CR_LUT are identical */
    BLUE_CB_RED_CR_LUT[i]  = ((  ((int32_t) ((0.5 )    * (1L << 16)))  * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;

    GREEN_CR_LUT[i]        = (((-((int32_t) ((0.4187 ) * (1L << 16)))) * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;
    BLUE_CR_LUT[i]         = (((-((int32_t) ((0.0813 ) * (1L << 16)))) * i) + ((int32_t) 1 << (16 - 1))) >> 16 ;
  }  
}

/**
  * @brief  Initializes the MCU Look Up Tables  
  * @param  None
  * @retval None
  */
void JPEG_Init_MCU_LUT(void)
{
  uint32_t i, j, offset;
  
  /*Y LUT */
  for(i = 0; i < 16; i++)
  {
    for(j = 0; j < 16; j++)
    {
      offset =  j + (i*8);  
      if((j>=8) && (i>=8)) offset+= 120;
      else  if((j>=8) && (i<8)) offset+= 56;
      else  if((j<8) && (i>=8)) offset+= 64;
      
      JPEG_Y_MCU_LUT[i*16 + j] = offset;
    }
  }
  
  /*Cb Cr K LUT*/
  for(i = 0; i < 16; i++)
  {
    for(j = 0; j < 16; j++)
    {      
      offset = i*16 + j;
      
      JPEG_Cb_MCU_420_LUT[offset] = (j/2) + ((i/2)*8) + 256;
      JPEG_Cb_MCU_422_LUT[offset] = (j/2) + (i*8) + 128;
      
      JPEG_Cr_MCU_420_LUT[offset] = (j/2) + ((i/2)*8) + 320;
      JPEG_Cr_MCU_422_LUT[offset] = (j/2) + (i*8) + 192;
      
      JPEG_K_MCU_420_LUT[offset] = (j/2) + ((i/2)*8) + 384;
      JPEG_K_MCU_422_LUT[offset] = (j/2) + ((i/2)*8) + 256;
    }
  }

  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
    {      
      offset = i*8 + j;
      
      JPEG_Y_MCU_444_LUT[offset]  = offset;
      JPEG_Cb_MCU_444_LUT[offset] = offset + 64 ;
      JPEG_Cr_MCU_444_LUT[offset] = offset + 128 ;
      JPEG_K_MCU_444_LUT[offset]  = offset + 192 ;
    }
  }
}

/**
  * @brief  Initializes the YCCK format K Blocks  
  * @param  ChromaSampling
  * @retval pMCUBuffer
  */
static uint8_t *JPEG_Set_K_Blocks(uint8_t *pMCUBuffer, uint8_t pKBlocks[16][16], uint32_t ChromaSampling)
{
  uint32_t i,j;
  
  if(ChromaSampling == JPEG_420_SUBSAMPLING)
  {    
    /* 4:2:0 4Y + Cb + Cr + K*/
    
    pMCUBuffer = pMCUBuffer + 384; 
   /* K block */ 
    for(i=0;i<16;i+=2)
    {
      for(j=0;j<16;j+=2)
      {
        *pMCUBuffer = pKBlocks[i][j];
        pMCUBuffer++;
      }
    }
  }
  else if(ChromaSampling == JPEG_422_SUBSAMPLING)
  {
    /* 4:2:2 2Y horizental + Cb + Cr + K*/
    pMCUBuffer = pMCUBuffer + 256;

    /* K block */
    for(i=0;i<8;i++)
    {
      for(j=0;j<16;j+=2)
      {
        *pMCUBuffer = pKBlocks[i][j];
        pMCUBuffer++;
      }
    }      
  }  
  else if(ChromaSampling == JPEG_444_SUBSAMPLING)
  {
    /* 4:4:4 Y + Cb + Cr + K*/
    
    /* K block */
    pMCUBuffer = pMCUBuffer + 192;
    for(i=0;i<8;i++)
    {
      for(j=0;j<8;j++)
      {
        *pMCUBuffer = pKBlocks[i][j];
        pMCUBuffer++;
      }
    }      
  }
  return pMCUBuffer;
}
#endif /* USE_JPEG_ENCODER == 1 */
