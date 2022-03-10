/**
  ******************************************************************************
  * @file    lcdconf.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016
  * @brief   This file implements the configuration for the GUI library
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

/* Includes ------------------------------------------------------------------*/
#include "LCDConf.h"
#include "GUI_Private.h"
#include "GUIDRV_Lin.h"

#include "stm32h7xx_hal.h"
#include "../Components/rk043fn48h/rk043fn48h.h"

#include "cmsis_os.h"

extern void Error_Handler(void);

/** @addtogroup LCD CONFIGURATION
* @{
*/

/** @defgroup LCD CONFIGURATION
* @brief This file contains the LCD Configuration
* @{
*/

/** @defgroup LCD CONFIGURATION_Private_TypesDefinitions
* @{
*/
/**
* @}
*/

/** @defgroup LCD CONFIGURATION_Private_Defines
* @{
*/
#define XSIZE_PHYS      480
#define YSIZE_PHYS      272

#define NUM_BUFFERS      3 /* Number of multiple buffers to be used */
#define NUM_VSCREENS     1 /* Number of virtual screens to be used */

#define BK_COLOR GUI_DARKBLUE

#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS        1

#ifndef FB_BYTES_PER_PIXEL
#define FB_BYTES_PER_PIXEL    2                       /* 4 - 3 - 2 */
#endif /* FB_BYTES_PER_PIXEL */

#define DMA2D_ALPHA_BLENDING

#if (FB_BYTES_PER_PIXEL == 2)
#define COLOR_CONVERSION_0    GUICC_M565              /* GUICC_M8888I - GUICC_M888 - GUICC_M565 */
#define DISPLAY_DRIVER_0      GUIDRV_LIN_16           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#if (GUI_NUM_LAYERS > 1)
#define COLOR_CONVERSION_1    GUICC_M4444I            /* GUICC_M8888I - GUICC_M1555I - GUICC_M4444I */
#define DISPLAY_DRIVER_1      GUIDRV_LIN_16           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#endif

#elif (FB_BYTES_PER_PIXEL == 3)
#define COLOR_CONVERSION_0    GUICC_M888              /* GUICC_M8888I - GUICC_M888 - GUICC_M565 */
#define DISPLAY_DRIVER_0      GUIDRV_LIN_24           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#if (GUI_NUM_LAYERS > 1)
#define COLOR_CONVERSION_1    GUICC_M1555I             /* GUICC_M8888I - GUICC_M1555I - GUICC_M4444I */
#define DISPLAY_DRIVER_1      GUIDRV_LIN_24           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#endif

#elif (FB_BYTES_PER_PIXEL == 4)
#define COLOR_CONVERSION_0    GUICC_M8888I            /* GUICC_M8888I - GUICC_M888 - GUICC_M565 */
#define DISPLAY_DRIVER_0      GUIDRV_LIN_32           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#if (GUI_NUM_LAYERS > 1)
#define COLOR_CONVERSION_1    GUICC_M8888I            /* GUICC_M8888I - GUICC_M1555I - GUICC_M4444I */
#define DISPLAY_DRIVER_1      GUIDRV_LIN_32           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#endif /* GUI_NUM_LAYERS */

#else
#error Please set a correct value for FB_BYTES_PER_PIXEL !!
#endif /* FB_BYTES_PER_PIXEL */

#ifndef   XSIZE_PHYS
#error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
#error Physical Y size of display is not defined!
#endif
#ifndef   NUM_VSCREENS
#define NUM_VSCREENS 1
#else
#if (NUM_VSCREENS <= 0)
#error At least one screeen needs to be defined!
#endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
#error Virtual screens and multiple buffers are not allowed!
#endif

#define FRAMEBUFFER_SIZE  (XSIZE_PHYS * YSIZE_PHYS * FB_BYTES_PER_PIXEL)

#if defined ( __GNUC__ )
__attribute__((section(".framebuffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="framebuffer"
#elif defined( __CC_ARM )
__attribute__((section(".framebuffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer0_fb0[FRAMEBUFFER_SIZE];

#if (NUM_BUFFERS > 1)
#if defined ( __GNUC__ )
__attribute__((section(".framebuffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="framebuffer"
#elif defined( __CC_ARM )
__attribute__((section(".framebuffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer0_fb1[FRAMEBUFFER_SIZE];
#endif

#if (NUM_BUFFERS > 2)
#if defined ( __GNUC__ )
__attribute__((section(".framebuffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="framebuffer"
#elif defined( __CC_ARM )
__attribute__((section(".framebuffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer0_fb2[FRAMEBUFFER_SIZE];
#endif

#if GUI_NUM_LAYERS > 1

#if defined ( __GNUC__ )
__attribute__((section(".framebuffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="framebuffer"
#elif defined( __CC_ARM )
__attribute__((section(".framebuffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer1_fb0[FRAMEBUFFER_SIZE];

#if (NUM_BUFFERS > 1)
#if defined ( __GNUC__ )
__attribute__((section(".framebuffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="framebuffer"
#elif defined( __CC_ARM )
__attribute__((section(".framebuffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer1_fb1[FRAMEBUFFER_SIZE];
#endif

#if (NUM_BUFFERS > 2)
#if defined ( __GNUC__ )
__attribute__((section(".framebuffer"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="framebuffer"
#elif defined( __CC_ARM )
__attribute__((section(".framebuffer"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer1_fb2[FRAMEBUFFER_SIZE];
#endif

#endif /* GUI_NUM_LAYERS */

static const U32 *Framebuffers[GUI_NUM_LAYERS][NUM_BUFFERS] =
{
  {
    (U32 *)layer0_fb0
#if (NUM_BUFFERS > 1)
  , (U32 *)layer0_fb1
#endif
#if (NUM_BUFFERS > 2)
  , (U32 *)layer0_fb2
#endif
  }
#if (GUI_NUM_LAYERS > 1)
, {
    (U32 *)layer1_fb0
#if (NUM_BUFFERS > 1)
  , (U32 *)layer1_fb1
#endif
#if (NUM_BUFFERS > 2)
  , (U32 *)layer1_fb2
#endif
  }
#endif /* GUI_NUM_LAYERS */
};

#define LCD_LAYER0_FRAME_BUFFER  ((U32 *)&Framebuffers[0])
#if GUI_NUM_LAYERS > 1
#define LCD_LAYER1_FRAME_BUFFER  ((U32 *)&Framebuffers[1])
#endif

/**
* @}
*/

/* Redirect bulk conversion to DMA2D routines */
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}

/**
  * @}
  */
/** @defgroup LCD CONFIGURATION_Private_Variables
  * @{
  */
DMA2D_HandleTypeDef                  hdma2d;
LTDC_HandleTypeDef                   hltdc;
static LCD_LayerPropTypedef          layer_prop[GUI_NUM_LAYERS];

U32 *LCD_Addr[] = {
  LCD_LAYER0_FRAME_BUFFER
#if GUI_NUM_LAYERS > 1
  , LCD_LAYER1_FRAME_BUFFER
#endif
};

static int _aPendingBuffer[2] = { -1, -1 };
static int _aBufferIndex[GUI_NUM_LAYERS];
static int _axSize[GUI_NUM_LAYERS];
static int _aySize[GUI_NUM_LAYERS];
static int _aBytesPerPixels[GUI_NUM_LAYERS];
#if defined ( __ICCARM__ )
#pragma location="dma2d_palette"
#elif defined(__CC_ARM)
__attribute__((section(".dma2d_palette"), zero_init))
#elif defined(__GNUC__)
__attribute__((section(".dma2d_palette")))
#endif
static U32 DMA2D_palette[XSIZE_PHYS * sizeof(U32) * 3];
static U32 *_pDMA2D_palette = &DMA2D_palette[XSIZE_PHYS * sizeof(U32) * 0];

void LCD_LL_DrawBitmap32bpp_filter(U8 const * p);

osSemaphoreId              LcdUpdateSemaphoreId = 0;
osSemaphoreId              osDma2dSemph = 0;
osMutexId                  osDeviceMutex = 0;

/**
  * @}
  */

/** @defgroup LCD CONFIGURATION_Private_FunctionPrototypes
  * @{
  */
void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine);
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);

static void _DMA_Fill(int LayerIndex, void * pDst, int xSize, int ySize, int OffLine, U32 ColorIndex);

/* Color conversion routines using DMA2D */
DEFINE_DMA2D_COLORCONVERSION(M8888I, LTDC_PIXEL_FORMAT_ARGB8888)
/* Internal pixel format of emWin is 32 bit, because of that ARGB8888 */
DEFINE_DMA2D_COLORCONVERSION(M888,   LTDC_PIXEL_FORMAT_ARGB8888)
DEFINE_DMA2D_COLORCONVERSION(M565,   LTDC_PIXEL_FORMAT_RGB565)
DEFINE_DMA2D_COLORCONVERSION(M1555I, LTDC_PIXEL_FORMAT_ARGB1555)
DEFINE_DMA2D_COLORCONVERSION(M4444I, LTDC_PIXEL_FORMAT_ARGB4444)

/* Array of color conversions for each layer */
static const LCD_API_COLOR_CONV * _apColorConvAPI[] =
{
  COLOR_CONVERSION_0,
#if (GUI_NUM_LAYERS > 1)
  COLOR_CONVERSION_1,
#endif
};

static U32 LCD_GetBufferAddress(int LayerIndex, int BufferIndex)
{
  if (LayerIndex > GUI_NUM_LAYERS)
    return 0;

  return (U32 )Framebuffers[LayerIndex][BufferIndex];
}

/**
* @brief  Return Pixel format for a given layer
* @param  LayerIndex : Layer Index
* @retval Status ( 0 : 0k , 1: error)
*/
static U32 _GetPixelformat(int LayerIndex)
{
  const LCD_API_COLOR_CONV * pColorConvAPI;

  if (LayerIndex >= GUI_COUNTOF(_apColorConvAPI))
  {
    return 0;
  }
  pColorConvAPI = _apColorConvAPI[LayerIndex];
  if(pColorConvAPI == GUICC_M8888I)
  {
    return LTDC_PIXEL_FORMAT_ARGB8888;
  }
  else if (pColorConvAPI == GUICC_M888)
  {
    return LTDC_PIXEL_FORMAT_RGB888;
  }
  else if (pColorConvAPI == GUICC_M565)
  {
    return LTDC_PIXEL_FORMAT_RGB565;
  }
  else if (pColorConvAPI == GUICC_M1555I)
  {
    return LTDC_PIXEL_FORMAT_ARGB1555;
  }
  else if (pColorConvAPI == GUICC_M4444I)
  {
    return LTDC_PIXEL_FORMAT_ARGB4444;
  }
  else if (pColorConvAPI == GUICC_8666)
  {
    return LTDC_PIXEL_FORMAT_L8;
  }
  else if (pColorConvAPI == GUICC_1616I)
  {
    return LTDC_PIXEL_FORMAT_AL44;
  }
  else if (pColorConvAPI == GUICC_88666I)
  {
    return LTDC_PIXEL_FORMAT_AL88;
  }

  /* We'll hang in case of bad configuration */
  Error_Handler();

  return 0;
}

/**
  * @brief LTDC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable the LTDC and DMA2D clocks */
  __HAL_RCC_LTDC_CLK_ENABLE();
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();


  /*** LTDC Pins configuration ***/
  /* GPIOI configuration */
  gpio_init_structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* GPIOJ configuration */
  gpio_init_structure.Pin       = GPIO_PIN_All;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);
  /* GPIOK configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);

  /* LCD_DISP_EN GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_2;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  /* LCD_DISP_CTRL GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_7;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);

  /* LCD_BL_CTRL GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_1;  /* LCD_BL_CTRL pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  /* De-assert display enable LCD_DISP_EN pin */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

  /* Assert display enable LCD_DISP_CTRL pin */
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, GPIO_PIN_RESET);

  /* Assert backlight LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
  
  /* Enable peripheral */
  __HAL_RCC_LTDC_CLK_ENABLE();

  HAL_NVIC_SetPriority(LTDC_IRQn, 0x7, 0x0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);  
}

/**
  * @brief LTDC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)
{
  /* Reset peripherals */
  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();

  /* Release LTDC from reset state */
  __HAL_RCC_LTDC_RELEASE_RESET();
}

void DMA2D_IRQHandler(void)
{
  if (DMA2D->ISR & (DMA2D_ISR_CEIF | DMA2D_ISR_CAEIF | DMA2D_ISR_TEIF)) {
    Error_Handler(); /* Should never happen */
  }

  /* Clear the Transfer complete interrupt */
  DMA2D->IFCR = (U32)(DMA2D_IFCR_CTCIF | DMA2D_IFCR_CCTCIF | DMA2D_IFCR_CTEIF);

  /* Signal semaphore */
  osSemaphoreRelease(osDma2dSemph);
}

/**
  * @brief  Initialize the LCD Controller.
  * @param  LayerIndex : layer Index.
  * @retval None
  */
static void LCD_LL_Init(void)
{
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;

  /* DeInit */
  hltdc.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc);

  /* Set LCD Timings */
  hltdc.Init.HorizontalSync     = 40;
  hltdc.Init.AccumulatedHBP     = 42;
  hltdc.Init.AccumulatedActiveW = 533;
  hltdc.Init.TotalWidth         = 554;
  hltdc.Init.VerticalSync       = 9;
  hltdc.Init.AccumulatedVBP     = 11;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalHeigh         = 285;

  /* background value */
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;

  /*## LTDC Clock Configuration ###########################################*/
  /* RK043FN48H typical PCLK is 9.7 MHz so the PLL3R is configured to provide this clock */
  /* RK043FN48H LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 2 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 2*160 = 320 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 320/33 = 9.69 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 9.69 Mhz */
  periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  periph_clk_init_struct.PLL3.PLL3M = 12;
  periph_clk_init_struct.PLL3.PLL3N = 160;
  periph_clk_init_struct.PLL3.PLL3FRACN = 0;
  periph_clk_init_struct.PLL3.PLL3P = 10;
  periph_clk_init_struct.PLL3.PLL3Q = 10;
  periph_clk_init_struct.PLL3.PLL3R = 33;
  periph_clk_init_struct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  periph_clk_init_struct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
  HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);

  /* Polarity */
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Instance = LTDC;

  HAL_LTDC_Init(&hltdc);
  HAL_LTDC_ProgramLineEvent(&hltdc, 0);

  /* Enable dithering */
  HAL_LTDC_EnableDither(&hltdc);

   /* Configure the DMA2D default mode */
  hdma2d.Init.Mode         = DMA2D_R2M;
  hdma2d.Init.ColorMode    = DMA2D_INPUT_RGB565;
  hdma2d.Init.OutputOffset = 0x0;

  hdma2d.Instance          = DMA2D;

  if(HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    while (1);
  }
}

/**
  * @brief  Initialise the LCD Controller
  * @param  LayerIndex : layer Index.
  * @retval None
  */
static void LCD_LL_LayerInit(U32 LayerIndex, U32 address)
{
  static uint32_t       LUT[256];
  uint32_t              i;

  if (LayerIndex < GUI_NUM_LAYERS)
  {
    LTDC_LayerCfgTypeDef  Layercfg;

    /* Layer Init */
    Layercfg.WindowX0 = 0;
    Layercfg.WindowX1 = XSIZE_PHYS;
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = YSIZE_PHYS;
    Layercfg.PixelFormat = _GetPixelformat(LayerIndex);
    Layercfg.FBStartAdress = address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0xFF;
    Layercfg.Backcolor.Green = 0xFF;
    Layercfg.Backcolor.Red = 0xFF;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = XSIZE_PHYS;
    Layercfg.ImageHeight = YSIZE_PHYS;

    HAL_LTDC_ConfigLayer(&hltdc, &Layercfg, LayerIndex);

    /* Enable LUT on demand */
    if (LCD_GetBitsPerPixelEx(LayerIndex) <= 8)
    {
      /* Enable usage of LUT for all modes with <= 8bpp*/
      HAL_LTDC_EnableCLUT(&hltdc, LayerIndex);
    }
    else
    {
      /* Optional CLUT initialization for AL88 mode (16bpp)*/
      if (layer_prop[LayerIndex].pColorConvAPI == GUICC_88666I)
      {
        HAL_LTDC_EnableCLUT(&hltdc, LayerIndex);

        for (i = 0; i < 256; i++)
        {
          LUT[i] = LCD_API_ColorConv_8666.pfIndex2Color(i);
        }
        HAL_LTDC_ConfigCLUT(&hltdc, LUT, 256, LayerIndex);
      }
    }

    /* Now Disable this layer until next update */
    __HAL_LTDC_LAYER_DISABLE(&hltdc, LayerIndex);
  }
  __HAL_LTDC_RELOAD_CONFIG(&hltdc);
}

/**
  * @brief  DMA2D wait for interrupt
  * @param  None
  * @retval None
  */
static void _DMA2D_ExecOperation(void)
{
  /* Wait until the transfer is done */
  if (osSemaphoreWait(osDma2dSemph, osWaitForever /* 1000 */) == osErrorOS) {
    /* Semaphore timed out */
    Error_Handler();
  }
}

/**
  * @brief  DMA2D Copy buffers
  * @param  LayerIndex: Layer index
  * @param  pSrc      : Source buffer pointer
  * @param  pDst      : Destination buffer pointer
  * @param  xSize     : Horizontal size
  * @param  ySize     : Vertical size
  * @param  OffLineSrc: Source Line offset
  * @param  OffLineDst: Destination Line offset
  * @retval None
  */
static void _DMA_Copy(int LayerIndex, void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst)
{
  U32 PixelFormat;

  /* Get the layer pixel format used */
  PixelFormat    = _GetPixelformat(LayerIndex);

  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00000000UL | (1 << 9) | (1 << 8);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffLineSrc;
  DMA2D->OOR     = OffLineDst;
  DMA2D->FGPFCCR = PixelFormat;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

/**
  * @brief  DMA2D Copy RGB 565 buffer
  * @param  pSrc      : Source buffer pointer
  * @param  pDst      : Destination buffer pointer
  * @param  xSize     : Horizontal size
  * @param  ySize     : Vertical size
  * @param  OffLineSrc: Source Line offset
  * @param  OffLineDst: Destination Line offset
  * @retval None
  */
static void _DMA_CopyRGB565(const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst)
{
  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00000000UL | (1 << 9) | (1 << 8);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffLineSrc;
  DMA2D->OOR     = OffLineDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

/**
  * @brief  DMA2D Fill buffer
  * @param  LayerIndex: Layer index
  * @param  pDst      : Destination buffer pointer
  * @param  xSize     : Horizontal size
  * @param  ySize     : Vertical size
  * @param  OffLineSrc: Source Line offset
  * @param  OffLineDst: Destination Line offset
  * @param  ColorIndex: Color to be used for the Fill operation
  * @retval None
  */
static void _DMA_Fill(int LayerIndex, void * pDst, int xSize, int ySize, int OffLine, U32 ColorIndex)
{
  U32 PixelFormat;

  /* Get the layer pixel format used */
  PixelFormat = _GetPixelformat(LayerIndex);

  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00030000UL | (1 << 9) | (1 << 8);
  DMA2D->OCOLR   = ColorIndex;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->OOR     = OffLine;
  DMA2D->OPFCCR  = PixelFormat;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

#if defined(DMA2D_ALPHA_BLENDING)
/**
  * @brief  DMA2D Alpha blending bulk
  * @param  pColorFG : Foregroung color
  * @param  pColorBG : Background color
  * @param  pColorDst: Destination color
  * @param  NumItems : Number of lines
  * @retval None
  */
static void _DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems)
{
  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00020000UL | (1 << 9) | (1 << 8);
  DMA2D->FGMAR   = (U32)pColorFG;
  DMA2D->BGMAR   = (U32)pColorBG;
  DMA2D->OMAR    = (U32)pColorDst;
  DMA2D->FGOR    = 0;
  DMA2D->BGOR    = 0;
  DMA2D->OOR     = 0;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}
#endif /* DMA2D_ALPHA_BLENDING */

/**
  * @brief  Mixing bulk colors
  * @param  pColorFG : Foregroung color
  * @param  pColorBG : Background color
  * @param  pColorDst: Destination color
  * @param  Intens   : Color intensity
  * @param  NumItems : Number of lines
  * @retval None
  */
static void _DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, U32 NumItems)
{
  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00020000UL | (1 << 9) | (1 << 8);
  DMA2D->FGMAR   = (U32)pColorFG;
  DMA2D->BGMAR   = (U32)pColorBG;
  DMA2D->OMAR    = (U32)pColorDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

/**
  * @brief  Color conversion
  * @param  pSrc          : Source buffer
  * @param  pDst          : Destination buffer
  * @param  PixelFormatSrc: Source pixel format
  * @param  PixelFormatDst: Destination pixel format
  * @param  NumItems      : Number of lines
  * @retval None
  */
static void _DMA_ConvertColor(void * pSrc, void * pDst,  U32 PixelFormatSrc, U32 PixelFormatDst, U32 NumItems)
{
  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00010000UL | (1 << 9) | (1 << 8);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = 0;
  DMA2D->OOR     = 0;
  DMA2D->FGPFCCR = PixelFormatSrc;
  DMA2D->OPFCCR  = PixelFormatDst;
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

/**
  * @brief  Draw L8 picture
  * @param  pSrc          : Source buffer
  * @param  pDst          : Destination buffer
  * @param  OffSrc        : Source Offset
  * @param  OffDst        : Destination Offset
  * @param  PixelFormatDst: Destination pixel format
  * @param  xSize         : Picture horizontal size
  * @param  ySize         : Picture vertical size
  * @retval None
  */
static void _DMA_DrawBitmapL8(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize)
{
  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00010000UL | (1 << 9) | (1 << 8);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffSrc;
  DMA2D->OOR     = OffDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_L8;
  DMA2D->OPFCCR  = PixelFormatDst;
  DMA2D->NLR     = (U32)(xSize << 16) | ySize;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

/**
  * @brief  Draw alpha bitmap
  * @param  pDst       : Destination buffer
  * @param  pSrc       : Source buffer
  * @param  xSize      : Picture horizontal size
  * @param  ySize      : Picture vertical size
  * @param  OffLineSrc : Source line offset
  * @param  OffLineDst : Destination line offset
  * @param  PixelFormat: Pixel format
  * @retval None
  */
static void _DMA_DrawAlphaBitmap(void * pDst, const void * pSrc, int xSize, int ySize, int OffLineSrc, int OffLineDst, int PixelFormat)
{
  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00020000UL | (1 << 9) | (1 << 8);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffLineSrc;
  DMA2D->BGOR    = OffLineDst;
  DMA2D->OOR     = OffLineDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->BGPFCCR = PixelFormat;
  DMA2D->OPFCCR  = PixelFormat;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

/**
  * @brief  Load LUT
  * @param  pColor  : CLUT address
  * @param  NumItems: Number of items to load
  * @retval None
  */
static void _DMA_LoadLUT(LCD_COLOR * pColor, U32 NumItems)
{
  U32 PixelFormat = _GetPixelformat(0);

  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->FGCMAR  = (U32)pColor;
  DMA2D->FGPFCCR  = PixelFormat
                  | ((NumItems - 1) & 0xFF) << 8;

  /* Start loading */
  DMA2D->FGPFCCR |= (1 << 5);

  /* Wait for end of palette transfer */
  while (DMA2D->FGPFCCR & (1 << 5))
  {
  }

  osMutexRelease(osDeviceMutex);
}

#if defined (DMA2D_ALPHA_BLENDING)
/**
  * @brief  DMA2D Alpha blending
  * @param  pColorFG : Foreground color
  * @param  pColorBG : Background color
  * @param  pColorDst: Destination color
  * @param  NumItems : Number of items
  * @retval None
  */
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems)
{
  _DMA_AlphaBlendingBulk(pColorFG, pColorBG, pColorDst, NumItems);
}
#endif /* DMA2D_ALPHA_BLENDING */

/**
  * @brief  Converts the given index values to 32 bit colors.
  * @param  pIndex     : Index value
  * @param  pColor     : Color relative to the index
  * @param  NumItems   : Number of items
  * @param  SizeOfIndex: Size of index color
  * @param  PixelFormat: Pixel format
  * @retval None
  */
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat)
{
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_PIXEL_FORMAT_ARGB8888, NumItems);
}

/**
  * @brief  Converts a 32 bit colors to an index value.
  * @param  pColor     : Color relative to the index
  * @param  pIndex     : Index value
  * @param  NumItems   : Number of items
  * @param  SizeOfIndex: Size of index color
  * @param  PixelFormat: Pixel format
  * @retval None
  */
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat)
{
  _DMA_ConvertColor(pColor, pIndex, LTDC_PIXEL_FORMAT_ARGB8888, PixelFormat, NumItems);
}

/**
  * @brief  LCD Mix color bulk.
  * @param  pFG    : Foreground buffer
  * @param  pBG    : Background buffer
  * @param  pDst   : Destination buffer
  * @param  OffFG  : Foreground offset
  * @param  OffBG  : Background offset
  * @param  OffDest: Destination offset
  * @param  xSize  : Horizontal size
  * @param  ySize  : Vertical size
  * @param  Intens : Color Intensity
  * @retval None
  */
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens)
{
  int y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);

  /* Do the same operation for all the lines */
  for (y = 0; y < ySize; y++)
  {
    /* Use DMA2D for mixing up */
    _DMA_MixColorsBulk(pFG, pBG, pDst, Intens, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
}

/**
  * @brief  LCD Copy buffer
  * @param  LayerIndex: Layer index
  * @param  IndexSrc  : Source index
  * @param  IndexDst  : Destination index
  * @retval None
  */
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst)
{
  U32 AddrSrc, AddrDst;

  AddrSrc    = LCD_GetBufferAddress(LayerIndex, IndexSrc);
  AddrDst    = LCD_GetBufferAddress(LayerIndex, IndexDst);
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, _axSize[LayerIndex], _aySize[LayerIndex], 0, 0);
  /* After this function has been called all drawing operations are routed to Buffer[IndexDst] */
  _aBufferIndex[LayerIndex] = IndexDst;
}

/**
  * @brief  LCD Copy rectangle
  * @param  LayerIndex: Layer index
  * @param  x0        : Horizontal rect origin
  * @param  y0        : Vertical rect origin
  * @param  x1        : Horizontal rect end
  * @param  y1        : Vertical rect end
  * @param  xSize     : Rectangle width
  * @param  ySize     : Rectangle height
  * @retval None
  */
static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize)
{
  U32 AddrSrc, AddrDst;
  int OffLine;

  AddrSrc  = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
  AddrSrc += ((y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex]);
  AddrDst  = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
  AddrDst += ((y1 * _axSize[LayerIndex] + x1) * _aBytesPerPixels[LayerIndex]);

  OffLine = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, OffLine, OffLine);
}

/**
  * @brief  LCD fill rectangle
  * @param  LayerIndex: Layer index
  * @param  x0        : Horizontal rect origin
  * @param  y0        : Vertical rect origin
  * @param  x1        : Horizontal rect end
  * @param  y1        : Vertical rect end
  * @param  PixelIndex: Color to be used for the Fill operation
  * @retval None
  */
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex)
{
  U32 AddrDst;
  int xSize, ySize;

  /* Depending on the draw mode, do it differently */
  if (GUI_GetDrawMode() == GUI_DM_XOR)
  {
    /* Use SW Fill rectangle */
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    /* Then set custom callback function for fillrect operation */
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
  }
  else
  {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    AddrDst  = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
    AddrDst += ((y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex]);
    _DMA_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, _axSize[LayerIndex] - xSize, PixelIndex);
  }
}

/**
  * @brief  Draw 32 bits per pixel bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : start horizontal position on the screen
  * @param  y           : start vertical position on the screen
  * @param  p           : Source buffer
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Number of bytes per Line
  * @retval None
  */
static void _LCD_DrawBitmap32bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine)
{
  U32 AddrDst;
  int OffLineSrc, OffLineDst;

  AddrDst  = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
  AddrDst += ((y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex]);
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  Draw 8 bits per pixel bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : start horizontal position on the screen
  * @param  y           : start vertical position on the screen
  * @param  p           : Source buffer
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Number of bytes per Line
  * @retval None
  */
static void _LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine)
{
  U32 AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  AddrDst  = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
  AddrDst += ((y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex]);
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_DrawBitmapL8((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);
}

/**
  * @brief  Draw 16 bits per pixel memory device
  * @param  pDst           : Destination buffer
  * @param  pSrc           : Source buffer
  * @param  xSize          : Horizontal memory device size
  * @param  ySize          : Vertical memory device size
  * @param  BytesPerLineDst: Destination number of bytes per Line
  * @param  BytesPerLineSrc: Source number of bytes per Line
  * @retval None
  */
static void _LCD_DrawMemdev16bpp(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc)
{
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 2) - xSize;
  OffLineDst = (BytesPerLineDst / 2) - xSize;
  _DMA_CopyRGB565(pSrc, pDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  Draw alpha memory device
  * @param  pDst           : Destination buffer
  * @param  pSrc           : Source buffer
  * @param  xSize          : Horizontal memory device size
  * @param  ySize          : Vertical memory device size
  * @param  BytesPerLineDst: Destination number of bytes per Line
  * @param  BytesPerLineSrc: Source number of bytes per Line
  * @retval None
  */
static void _LCD_DrawMemdevAlpha(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc)
{
  int OffLineSrc, OffLineDst;

  OffLineSrc = (BytesPerLineSrc / 4) - xSize;
  OffLineDst = (BytesPerLineDst / 4) - xSize;
  _DMA_DrawAlphaBitmap(pDst, pSrc, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_ARGB8888);
}

/**
  * @brief  Draw alpha Bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : Horizontal position on the screen
  * @param  y           : vertical position on the screen
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Bytes per Line
  * @retval None
  */
static void _LCD_DrawBitmapAlpha(int LayerIndex, int x, int y, const void * p, int xSize, int ySize, int BytesPerLine)
{
  U32 AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  AddrDst  = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
  AddrDst += ((y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex]);
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_DrawAlphaBitmap((void *)AddrDst, p, xSize, ySize, OffLineSrc, OffLineDst, PixelFormat);
}

/**
  * @brief  Translates the given colors into index values for the display controller
  * @param  pLogPal   : Palette
  * @param  pBitmap   : Bitmap
  * @param  LayerIndex: Layer index
  * @retval LCD_PIXELINDEX
  */
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex)
{
  void (* pFunc)(void);
  int DoDefault = 0;

  /* Check if we have a non transparent device independent bitmap */
  if (pBitmap->BitsPerPixel == 8)
  {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc)
    {
      if (pBitmap->pPal)
      {
        if (pBitmap->pPal->HasTrans)
        {
          DoDefault = 1;
        }
      }
      else
      {
        DoDefault = 1;
      }
    }
    else
    {
      DoDefault = 1;
    }
  }
  else
  {
    DoDefault = 1;
  }

  /* Default palette management for other cases */
  if (DoDefault)
  {
    /* Return a pointer to the index values to be used by the controller */
    return LCD_GetpPalConvTable(pLogPal);
  }

  /* Load LUT using DMA2D */
  _DMA_LoadLUT((U32 *)pLogPal->pPalEntries, pLogPal->NumEntries);

  /* Return something not NULL */
  return _pDMA2D_palette;
}

/**
  * @brief  End of frame interrupt for managing multiple buffering
  * @param  hltdc: ltdc handle 
  * @retval None
  */
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *hltdc) {
  U32 Addr;
  int i;

  for (i = 0; i < GUI_NUM_LAYERS; i++) 
  {
    if (_aPendingBuffer[i] >= 0) 
    {
      /* Calculate address of buffer to be used  as visible frame buffer */
      Addr = LCD_GetBufferAddress(i, _aPendingBuffer[i]);
      
      /* Store address into the LTDC regs */	  
      __HAL_LTDC_LAYER(hltdc, i)->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
      __HAL_LTDC_LAYER(hltdc, i)->CFBAR = Addr;
      __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(hltdc);
      
      /* Tell emWin that buffer is used */
      GUI_MULTIBUF_ConfirmEx(i, _aPendingBuffer[i]);
      
      /* Clear pending buffer flag of layer */
      _aPendingBuffer[i] = -1;
    }
  }
  HAL_LTDC_ProgramLineEvent(hltdc, 0);
  /* Release the semaphore to STemWin to draw next frame */
  osSemaphoreRelease( LcdUpdateSemaphoreId );
}

/**
  * @brief  Function is called by the display driver for several purposes
  * @param  LayerIndex: Index of layer to be configured
  * @param  Cmd       : Commands
  * @param  pData     : Pointer to a LCD_X_DATA structure
  * @retval int
  */
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
  int r = 0;
  U32 addr;

  switch (Cmd)
  {
    case LCD_X_INITCONTROLLER:
    {
      /* Called during the initialization process in order to set up the display controller and put it into operation */
      break;
    }
    case LCD_X_SETORG:
    {
      /* Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p */
      LCD_X_SETORG_INFO * p;

      p = (LCD_X_SETORG_INFO *)pData;
      addr = LCD_GetBufferAddress(LayerIndex, 0) + p->yPos * _axSize[LayerIndex] * _aBytesPerPixels[LayerIndex];
      HAL_LTDC_SetAddress(&hltdc, addr, LayerIndex);
      break;
    }
    case LCD_X_SHOWBUFFER:
    {
      /* Required if multiple buffers are used. The 'Index' element of p contains the buffer index. */
      LCD_X_SHOWBUFFER_INFO * p;

      p = (LCD_X_SHOWBUFFER_INFO *)pData;

      /* Update the index of the buffer to use */
      LCD_Addr[LayerIndex] = (U32 *)(LCD_GetBufferAddress(LayerIndex, p->Index));
      _aPendingBuffer[LayerIndex] = p->Index;

      LTDC_LAYER(&hltdc, LayerIndex)->CFBAR  = (U32 )LCD_Addr[LayerIndex];
      __HAL_LTDC_RELOAD_CONFIG(&hltdc);

      osSemaphoreWait( LcdUpdateSemaphoreId , osWaitForever );
      break;
    }
    case LCD_X_SETLUTENTRY:
    {
      /* Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p */
      LCD_X_SETLUTENTRY_INFO * p;

      p = (LCD_X_SETLUTENTRY_INFO *)pData;
      HAL_LTDC_ConfigCLUT(&hltdc, (uint32_t*)p->Color, p->Pos, LayerIndex);
      break;
    }
    case LCD_X_ON:
    { 
      /* Required if the display controller should support switching on and off */
      __HAL_LTDC_ENABLE(&hltdc);
      break;
    }
    case LCD_X_OFF:
    {
      /* Required if the display controller should support switching on and off */
      __HAL_LTDC_DISABLE(&hltdc);
      break;
    }
    case LCD_X_SETVIS:
    {
      /* Required for setting the layer visibility which is passed in the 'OnOff' element of pData */
      LCD_X_SETVIS_INFO * p;

      p = (LCD_X_SETVIS_INFO *)pData;

      if(p->OnOff  == ENABLE )
      {
        __HAL_LTDC_LAYER_ENABLE(&hltdc, LayerIndex);
      }
      else
      {
        __HAL_LTDC_LAYER_DISABLE(&hltdc, LayerIndex);
      }
      __HAL_LTDC_RELOAD_CONFIG(&hltdc);

      break;
    }
    case LCD_X_SETPOS:
    {
      /* Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData */
      LCD_X_SETPOS_INFO * p;

      p = (LCD_X_SETPOS_INFO *)pData;
      HAL_LTDC_SetWindowPosition(&hltdc, p->xPos, p->yPos, LayerIndex);
      break;
    }
    case LCD_X_SETSIZE:
    {
      /* Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData */
      LCD_X_SETSIZE_INFO * p;
      int xPos, yPos;
      GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
      p = (LCD_X_SETSIZE_INFO *)pData;
      if (LCD_GetSwapXYEx(LayerIndex))
      {
        _axSize[LayerIndex] = p->ySize;
        _aySize[LayerIndex] = p->xSize;
      }
      else
      {
        _axSize[LayerIndex] = p->xSize;
        _aySize[LayerIndex] = p->ySize;
      }
      HAL_LTDC_SetWindowPosition(&hltdc, xPos, yPos, LayerIndex);
      break;
    }
    case LCD_X_SETALPHA:
    {
      /* Required for setting the alpha value which is passed in the 'Alpha' element of pData */
      LCD_X_SETALPHA_INFO * p;

      p = (LCD_X_SETALPHA_INFO *)pData;
      HAL_LTDC_SetAlpha(&hltdc, p->Alpha, LayerIndex);
      break;
    }
    case LCD_X_SETCHROMAMODE:
    {
      /* Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData */
      LCD_X_SETCHROMAMODE_INFO * p;

      p = (LCD_X_SETCHROMAMODE_INFO *)pData;
      if(p->ChromaMode != 0)
      {
        HAL_LTDC_EnableColorKeying(&hltdc, LayerIndex);
      }
      else
      {
        HAL_LTDC_DisableColorKeying(&hltdc, LayerIndex);
      }
      break;
    }
    case LCD_X_SETCHROMA:
    {
      /* Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData */
      LCD_X_SETCHROMA_INFO * p;
      U32 Color;

      p = (LCD_X_SETCHROMA_INFO *)pData;
      Color = ((p->ChromaMin & 0xFF0000) >> 16) | (p->ChromaMin & 0x00FF00) | ((p->ChromaMin & 0x0000FF) << 16);
      HAL_LTDC_ConfigColorKeying(&hltdc, Color, LayerIndex);
      break;
    }
    default:
      r = -1;
      break;
    }

    return r;
}

static void _ClearCacheHook(U32 LayerMask) {
 int i;
 for (i = 0; i < GUI_NUM_LAYERS; i++) {
  if (LayerMask & (1 << i))
  {
    if (_aBufferIndex[i] >= 0)
    {
      SCB_CleanDCache_by_Addr ((uint32_t *)LCD_GetBufferAddress(i, _aBufferIndex[i]), (_aySize[i] * _aySize[i] * _aBytesPerPixels[i]));
    }
  }
 }
}

/**
  * @brief  Called during the initialization process in order to set up the
  *         display driver configuration
  * @param  None
  * @retval None
  */
void LCD_X_Config(void)
{
  int i;
  U32 PixelFormat;

  GUI_DCACHE_SetClearCacheHook(_ClearCacheHook);

  /* At first initialize use of multiple buffers on demand */
#if (NUM_BUFFERS > 1)
  for (i = 0; i < GUI_NUM_LAYERS; i++)
  {
    GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
  }
#endif

  /* Set display driver and color conversion for 1st layer */
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);

  /* Set size of 1st layer */
  if (LCD_GetSwapXYEx(0))
  {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
  }
  else
  {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
  }
#if (GUI_NUM_LAYERS > 1)

  /* Set display driver and color conversion for 2nd layer */
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);

  /* Set size of 2nd layer */
  if (LCD_GetSwapXYEx(1))
  {
    LCD_SetSizeEx (1, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(1, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
  }
  else
  {
    LCD_SetSizeEx (1, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(1, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
  }
#endif
  /* Setting up VRam address and get the pixel size */
  for (i = 0; i < GUI_NUM_LAYERS; i++)
  {
    /* Setting up VRam address */
    LCD_SetVRAMAddrEx(i, (void *)(LCD_GetBufferAddress(i, 0)));
    /* Get the pixel size */
    _aBytesPerPixels[i] = LCD_GetBitsPerPixelEx(i) >> 3;
  }
  /* Setting up custom functions */
  for (i = 0; i < GUI_NUM_LAYERS; i++)
  {
    PixelFormat = _GetPixelformat(i);
    /* Set custom function for copying complete buffers (used by multiple buffering) using DMA2D */
    LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);
    if (PixelFormat <= LTDC_PIXEL_FORMAT_ARGB4444)
    {
      /* Set custom function for filling operations using DMA2D */
      LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
    }
      /* Set custom function for copy recxtangle areas (used by GUI_CopyRect()) using DMA2D */
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT, (void(*)(void))_LCD_CopyRect);

      /* Set functions for direct color mode layers. Won't work with indexed color modes because of missing LUT for DMA2D destination */
      if (PixelFormat <= LTDC_PIXEL_FORMAT_ARGB4444)
      {
        /* Set up custom drawing routine for index based bitmaps using DMA2D */
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))_LCD_DrawBitmap8bpp);
      }

      /* Set up drawing routine for 16bpp bitmap using DMA2D */
      if (PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
      {
        /* Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565 */
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))_LCD_DrawBitmap16bpp);
      }

      /* Set up drawing routine for 32bpp bitmap using DMA2D */
      if (PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
      {
        /* Set up drawing routine for 32bpp bitmap using DMA2D. Makes only sense with ARGB8888 */
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))_LCD_DrawBitmap32bpp);
      }
  }

  /* Set up custom color conversion using DMA2D, works only for direct color modes because of missing LUT for DMA2D destination */

  /* Set up custom bulk color conversion using DMA2D for ARGB1555 */
  GUICC_M1555I_SetCustColorConv(_Color2IndexBulk_M1555I_DMA2D, _Index2ColorBulk_M1555I_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for RGB565 */
  GUICC_M565_SetCustColorConv  (_Color2IndexBulk_M565_DMA2D,   _Index2ColorBulk_M565_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for ARGB4444 */
  GUICC_M4444I_SetCustColorConv(_Color2IndexBulk_M4444I_DMA2D, _Index2ColorBulk_M4444I_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for RGB888 */
  GUICC_M888_SetCustColorConv  (_Color2IndexBulk_M888_DMA2D,   _Index2ColorBulk_M888_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for ARGB8888 */
  GUICC_M8888I_SetCustColorConv(_Color2IndexBulk_M8888I_DMA2D, _Index2ColorBulk_M8888I_DMA2D);

#if defined (DMA2D_ALPHA_BLENDING)
  /* Set up custom alpha blending function using DMA2D */
  GUI_SetFuncAlphaBlending(_DMA_AlphaBlending);
#endif /* DMA2D_ALPHA_BLENDING */

  /* Set up custom function for translating a bitmap palette into index values.
   * Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap
   */
  GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);

  /* Set up custom function for mixing up arrays of colors using DMA2D */
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);

#if GUI_SUPPORT_MEMDEV
  /* Set up custom function for drawing 16bpp memory devices */
  GUI_MEMDEV_SetDrawMemdev16bppFunc(_LCD_DrawMemdev16bpp);

  /* Set up custom function for Alpha drawing operations */
  GUI_SetFuncDrawAlpha(_LCD_DrawMemdevAlpha, _LCD_DrawBitmapAlpha);
#endif

  /* Create the semaphore that will be used for the GUI thread */
  osSemaphoreDef( LcdUpdateSemaphore );
  LcdUpdateSemaphoreId = osSemaphoreCreate( osSemaphore( LcdUpdateSemaphore ), 1 );
  osSemaphoreWait(LcdUpdateSemaphoreId , osWaitForever);

  /* Create the Semaphore used by the two threads */
  /* Create Semaphore lock */
  osSemaphoreDef(Semaphore);
  osDma2dSemph = osSemaphoreCreate(osSemaphore(Semaphore), 1);
  osSemaphoreWait(osDma2dSemph, osWaitForever); /* next call to osSemaphoreWait will block */
  osMutexDef(DeviceMutex);
  osDeviceMutex = osMutexCreate(osMutex(DeviceMutex));

  /* Complete the initialisation */
  LCD_LL_Init ();

  LCD_LL_LayerInit(0, LCD_GetBufferAddress(0, 0));
  HAL_LTDC_SetPitch(&hltdc, XSIZE_PHYS, 0);
  LCD_SetBufferPtrEx(0, (U32 *)Framebuffers[0]);

#if (GUI_NUM_LAYERS > 1)
  LCD_LL_LayerInit(1, LCD_GetBufferAddress(1, 0));
  HAL_LTDC_SetPitch(&hltdc, XSIZE_PHYS, 1);
  LCD_SetBufferPtrEx(1, (U32 *)Framebuffers[1]);
#endif

  HAL_Delay(20);
}

/*********************************************************************
*
*       CopyBuffer
*/
void DMA2D_CopyBufferWithAlpha_filter(const uint16_t xSize, const uint16_t ySize, void * pSrc, void * pDst)
{
  osMutexWait(osDeviceMutex, osWaitForever);

  DMA2D->CR      = 0x00010000UL | (1 << 9) | (1 << 8);

  /* Set up pointers */
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->FGOR    = 0;
  DMA2D->OOR     = 0;
  DMA2D->BGOR    = 0;

  /* Set up pixel format */
  DMA2D->FGPFCCR  = DMA2D_INPUT_ARGB8888;
  DMA2D->FGPFCCR |= DMA2D_FGPFCCR_AM_0;
  DMA2D->OPFCCR   = DMA2D_OUTPUT_ARGB8888;
  DMA2D->OPFCCR  |= DMA2D_OPFCCR_RBS;
  DMA2D->OPFCCR  |= DMA2D_OPFCCR_AI;

  /* Set up size */
  DMA2D->NLR      = (U32)(xSize << 16) | (U16)ySize;

  /* Start transfer */
  DMA2D->CR |= DMA2D_CR_START;

  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}

void LCD_LL_DrawBitmap32bpp_filter(U8 const * p)
{
  U32 AddrDst;

  AddrDst = LCD_GetBufferAddress(0, _aBufferIndex[0]);
  DMA2D_CopyBufferWithAlpha_filter(XSIZE_PHYS, YSIZE_PHYS, (void *)AddrDst, (void *)p);
}

/**
  * @brief  Draw 16 bits per pixel bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : start horizontal position on the screen
  * @param  y           : start vertical position on the screen
  * @param  p           : Source buffer
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Number of bytes per Line
  * @retval None
  */
void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine)
{
  U32 AddrDst;
  int OffLineSrc, OffLineDst;

  AddrDst  = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
  AddrDst += ((y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex]);
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_CopyRGB565((void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

