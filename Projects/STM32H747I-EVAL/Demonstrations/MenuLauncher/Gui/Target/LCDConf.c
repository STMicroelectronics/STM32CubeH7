/**
  ******************************************************************************
  * @file    lcdconf.c
  * @author  MCD Application Team
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
#define XSIZE_PHYS      800
#define YSIZE_PHYS      480

#define ZONES           2
#define VSYNC           1
#define VBP             1
#define VFP             1
#define VACT            YSIZE_PHYS
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            XSIZE_PHYS/ZONES      /* !!!! SCREEN DIVIDED INTO 2 AREAS !!!! */


#define NUM_BUFFERS      3 /* Number of multiple buffers to be used */
#define NUM_VSCREENS     1 /* Number of virtual screens to be used */

#define BK_COLOR GUI_DARKBLUE

#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 1

#define FB_BYTES_PER_PIXEL    4                       /* 4 - 3 - 2 */
// #define DMA2D_ALPHA_BLENDING

#if (FB_BYTES_PER_PIXEL == 2)
#define DSI_COLOR_FORMAT      DSI_RGB565              /* DSI_RGB888 - DSI_RGB565 */
#define OTM8009A_COLOR_FORMAT OTM8009A_FORMAT_RBG565  /* OTM8009A_FORMAT_RGB888 - OTM8009A_FORMAT_RBG565 */

#define COLOR_CONVERSION_0    GUICC_M565              /* GUICC_M8888I - GUICC_M888 - GUICC_M565 */
#define DISPLAY_DRIVER_0      GUIDRV_LIN_16           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */

#if (GUI_NUM_LAYERS > 1)
#define COLOR_CONVERSION_1    GUICC_M4444I            /* GUICC_M8888I - GUICC_M1555I - GUICC_M4444I */
#define DISPLAY_DRIVER_1      GUIDRV_LIN_16           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#endif

#elif (FB_BYTES_PER_PIXEL == 3)
#define DSI_COLOR_FORMAT      DSI_RGB888              /* DSI_RGB888 - DSI_RGB565 */
#define OTM8009A_COLOR_FORMAT OTM8009A_FORMAT_RGB888  /* OTM8009A_FORMAT_RGB888 - OTM8009A_FORMAT_RBG565 */

#define COLOR_CONVERSION_0    GUICC_M888              /* GUICC_M8888I - GUICC_M888 - GUICC_M565 */
#define DISPLAY_DRIVER_0      GUIDRV_LIN_24           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */

#if (GUI_NUM_LAYERS > 1)
#define COLOR_CONVERSION_1    GUICC_M1555I             /* GUICC_M8888I - GUICC_M1555I - GUICC_M4444I */
#define DISPLAY_DRIVER_1      GUIDRV_LIN_24           /* GUIDRV_LIN_32 - GUIDRV_LIN_24 - GUIDRV_LIN_16 */
#endif

#elif (FB_BYTES_PER_PIXEL == 4)
#define DSI_COLOR_FORMAT      DSI_RGB888              /* DSI_RGB888 - DSI_RGB565 */
#define OTM8009A_COLOR_FORMAT OTM8009A_FORMAT_RGB888  /* OTM8009A_FORMAT_RGB888 - OTM8009A_FORMAT_RBG565 */

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

#if defined ( __GNUC__ ) /* GCC Compiler */
__attribute__((section(".layer0_fb0"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=32
#pragma location="layer0_fb0"
#elif defined( __CC_ARM ) /* ARM Compiler */
__attribute__((section(".layer0_fb0"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer0_fb0[FRAMEBUFFER_SIZE];

#if (NUM_BUFFERS > 1)
#if defined ( __GNUC__ ) /* GCC Compiler */
__attribute__((section(".layer0_fb1"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=32
#pragma location="layer0_fb1"
#elif defined( __CC_ARM ) /* ARM Compiler */
__attribute__((section(".layer0_fb1"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer0_fb1[FRAMEBUFFER_SIZE];
#endif

#if (NUM_BUFFERS > 2)
#if defined ( __GNUC__ ) /* GCC Compiler */
__attribute__((section(".layer0_fb2"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=32
#pragma location="layer0_fb2"
#elif defined( __CC_ARM ) /* ARM Compiler */
__attribute__((section(".layer0_fb2"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer0_fb2[FRAMEBUFFER_SIZE];
#endif

#if GUI_NUM_LAYERS > 1

#if defined ( __GNUC__ ) /* GCC Compiler */
__attribute__((section(".layer1_fb0"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=32
#pragma location="layer1_fb0"
#elif defined( __CC_ARM ) /* ARM Compiler */
__attribute__((section(".layer1_fb0"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer1_fb0[FRAMEBUFFER_SIZE];

#if (NUM_BUFFERS > 1)
#if defined ( __GNUC__ ) /* GCC Compiler */
__attribute__((section(".layer1_fb1"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=32
#pragma location="layer1_fb1"
#elif defined( __CC_ARM ) /* ARM Compiler */
__attribute__((section(".layer1_fb1"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer1_fb1[FRAMEBUFFER_SIZE];
#endif

#if (NUM_BUFFERS > 2)
#if defined ( __GNUC__ ) /* GCC Compiler */
__attribute__((section(".layer1_fb2"))) __attribute__ ((aligned (32)))
#elif defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=32
#pragma location="layer1_fb2"
#elif defined( __CC_ARM ) /* ARM Compiler */
__attribute__((section(".layer1_fb2"), zero_init)) __attribute__ ((aligned (32)))
#endif
static U8 layer1_fb2[FRAMEBUFFER_SIZE];
#endif

#endif /* GUI_NUM_LAYERS */

static U8 const *Framebuffers[GUI_NUM_LAYERS][NUM_BUFFERS] =
{
  {
    layer0_fb0
#if (NUM_BUFFERS > 1)
  , layer0_fb1
#endif
#if (NUM_BUFFERS > 2)
  , layer0_fb2
#endif
  }
#if (GUI_NUM_LAYERS > 1)
, {
    layer1_fb0
#if (NUM_BUFFERS > 1)
  , layer1_fb1
#endif
#if (NUM_BUFFERS > 2)
  , layer1_fb2
#endif
  }
#endif /* GUI_NUM_LAYERS */
};

/**
* @}
*/

#define __DSI_MASK_TE()
#define __DSI_UNMASK_TE()

/* Redirect bulk conversion to DMA2D routines */
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}
static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
/**
  * @}
  */
/** @defgroup LCD CONFIGURATION_Private_Variables
  * @{
  */
LTDC_HandleTypeDef                   hltdc;
DSI_HandleTypeDef     	             hdsi;
DMA2D_HandleTypeDef                  DMA2D_Handle;

static U8                            RequestDisplayOn = 0;
static LCD_LayerPropTypedef          layer_prop[GUI_NUM_LAYERS];

/* Screen needed definitions */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */

/* ZONES of the screen */
uint8_t pCols[ZONES][4] =
{
#if (ZONES == 4 )
  {0x00, 0x00, 0x00, 0xC7}, /*   0 -> 199 */
  {0x00, 0xC8, 0x01, 0x8F}, /* 200 -> 399 */
  {0x01, 0x90, 0x02, 0x57}, /* 400 -> 599 */
  {0x02, 0x58, 0x03, 0x1F}, /* 600 -> 799 */
#elif (ZONES == 2 )
  {0x00, 0x00, 0x01, 0x8F}, /*   0 -> 399 */
  {0x01, 0x90, 0x03, 0x1F}
#elif (ZONES == 1 )
  {0x00, 0x00, 0x03, 0x1F}, /*   0 -> 799 */
#endif
};

volatile int32_t LCD_ActiveRegion    = 1;
volatile int32_t LCD_ReqRefersh      = 0;

static U32 *LCD_Addr[] = {
  (U32 *)&Framebuffers[0]
#if GUI_NUM_LAYERS > 1
, (U32 *)&Framebuffers[1]
#endif
};

static int _aPendingBuffer[2] = { -1, -1 };
static int _aBufferIndex[GUI_NUM_LAYERS];
static int _axSize[GUI_NUM_LAYERS];
static int _aySize[GUI_NUM_LAYERS];
static int _aBytesPerPixels[GUI_NUM_LAYERS];
static U32 _aBuffer[XSIZE_PHYS * sizeof(U32) * 3];
static U32 * _pBuffer_DMA2D = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 0];

void LCD_LL_DrawBitmap32bpp_filter(U8 const * p);

osSemaphoreId              LcdUpdateSemaphoreId = 0;
osSemaphoreId              osDma2dSemph = 0;
osMutexId                  osDeviceMutex = 0;

void DMA2D_IRQHandler(void);

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

void LCD_SetUpdateRegion(int idx);
void LCD_EnableTE(void);
void DSI_IRQHandler(void);

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

  return (U32 ) Framebuffers[LayerIndex][BufferIndex];
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
  * @brief  DCS or Generic short/long write command
  * @param  NbParams: Number of parameters. It indicates the write command mode:
  *                 If inferior to 2, a long write command is performed else short.
  * @param  pParams: Pointer to parameter values table.
  * @retval None
  */
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
  if(NbrParams <= 1)
  {
    HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
  }
  else
  {
    HAL_DSI_LongWrite(&hdsi,  0, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams);
  }
}

/**
  * @brief  BSP LCD Reset
  *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
  *         and desactivating it later.
  *         This signal is only cabled on eval Rev B and beyond.
  */
static void LCD_LL_Reset(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Configure the GPIO on PF10 */
  gpio_init_structure.Pin   = GPIO_PIN_10;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);

  HAL_Delay(20); /* wait 20 ms */

  /* Deactivate XRES */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);

  /* Wait for 300 ms after releasing XRES before sending commands */
  HAL_Delay(300);
}

/**
  * @brief  Initialize the BSP LCD Msp.
  * Application can surcharge if needed this function implementation
  */
void BSP_LCD_MspInit(void)
{
  /** @brief Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /** @brief Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /** @brief Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();

  /** @brief Soft Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0x5, 0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 0x5, 0);
  HAL_NVIC_EnableIRQ(DSI_IRQn);

  HAL_NVIC_SetPriority(DMA2D_IRQn, 0x5, 0x0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
{
  /* DeInit */
  hltdc.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc);

  /* LTDC Config */
  /* Timing and polarity */
  hltdc.Init.HorizontalSync = HSYNC;
  hltdc.Init.VerticalSync = VSYNC;
  hltdc.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc.Init.TotalWidth = HSYNC+HBP+HACT+HFP;

  /* background value */
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;

  /* Polarity */
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Instance = LTDC;

  HAL_LTDC_Init(&hltdc);
}

/**
  * @brief  Initialize the LCD Controller.
  * @param  None
  * @retval None
  */
static void LCD_LL_Init(void)
{
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  GPIO_InitTypeDef          GPIO_Init_Structure;
  static DSI_CmdCfgTypeDef         CmdCfg;
  static DSI_LPCmdTypeDef          LPCmd;
  static DSI_PLLInitTypeDef        PLLInit;
  static DSI_PHY_TimerTypeDef      PhyTimings;
  DSI_HOST_TimeoutTypeDef          HostTimeouts;
  OTM8009A_IO_t              IOCtx;
  static OTM8009A_Object_t   OTM8009AObj;
  static void                     *Lcd_CompObj = NULL;
  /* Toggle Hardware Reset of the DSI LCD using
  * its XRES signal (active low) */
  LCD_LL_Reset();

  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();

#if (NUM_BUFFERS > 1) || (GUI_NUM_LAYERS > 1)
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 240 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 240/[6|9|12|15] = [40|26.66|20|16] Mhz */
  /* PLLUSBCLK = PLL3_VCO Output/PLL3Q = 240/5 = 48 Mhz */
  /* LTDC clock frequency = [26.66|20] Mhz */
  /* USB clock frequency = 240/5 = 48 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 240;
  PeriphClkInitStruct.PLL3.PLL3P = 1;
  PeriphClkInitStruct.PLL3.PLL3Q = 5;
#if (NUM_BUFFERS > 1) && (GUI_NUM_LAYERS > 1)
#if (FB_BYTES_PER_PIXEL == 4)
  PeriphClkInitStruct.PLL3.PLL3R = 15;  /* 16 MHz */
#elif (FB_BYTES_PER_PIXEL == 3)
  PeriphClkInitStruct.PLL3.PLL3R = 12;  /* 20 MHz */
#else
  PeriphClkInitStruct.PLL3.PLL3R = 9;   /* 26.66 MHz */
#endif /* FB_BYTES_PER_PIXEL */
#else
  PeriphClkInitStruct.PLL3.PLL3R = 6;   /* 40 MHz */
#endif

#else
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 1 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 288 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 240/6 = 40 Mhz */
  /* PLLUSBCLK = PLL3_VCO Output/PLL3Q = 240/5 = 48 Mhz */
  /* LTDC clock frequency = 40 Mhz */
  /* USB clock frequency = 48 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 240;
  PeriphClkInitStruct.PLL3.PLL3P = 1;
  PeriphClkInitStruct.PLL3.PLL3Q = 5;
  PeriphClkInitStruct.PLL3.PLL3R = 6;
#endif /* GUI_NUM_LAYERS */
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi.Instance = DSI;

  HAL_DSI_DeInit(&(hdsi));

  /* We have HSE value at 25MHz and we want data lane at 500Mbps */
  PLLInit.PLLNDIV  = 99;
  PLLInit.PLLIDF   = DSI_PLL_IN_DIV5;
  PLLInit.PLLODF   = DSI_PLL_OUT_DIV1;

  hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi.Init.TXEscapeCkdiv = 0x4;
  HAL_DSI_Init(&(hdsi), &(PLLInit));

  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_COLOR_FORMAT;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_DISABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg);

  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(&hdsi, &LPCmd);

  /* Initialize LTDC */
  LTDC_Init();

  /* Start DSI */
  HAL_DSI_Start(&(hdsi));

  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  *  depending on configuration set in 'hdsivideo_handle'.
  */

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime  = 35; /* Tclk-post + Tclk-trail + Ths-exit = [(60ns + 52xUI) + (60ns) + (300ns)]/16ns */
  PhyTimings.ClockLaneLP2HSTime  = 35; /* Tlpx + (Tclk-prepare + Tclk-zero) + Tclk-pre = [150ns + 300ns + 8xUI]/16ns */
  PhyTimings.DataLaneHS2LPTime   = 35; /* Ths-trail + Ths-exit = [(60ns + 4xUI) + 100ns]/16ns */
  PhyTimings.DataLaneLP2HSTime   = 35; /* Tlpx + (Ths-prepare + Ths-zero) + Ths-sync = [150ns + (145ns + 10xUI) + 8xUI]/16ns */
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime        = 10;
  if(HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK)
  {
    Error_Handler();
  }

  HostTimeouts.TimeoutCkdiv                 = 1;
  HostTimeouts.HighSpeedTransmissionTimeout = 0;
  HostTimeouts.LowPowerReceptionTimeout     = 0;
  HostTimeouts.HighSpeedReadTimeout         = 0;
  HostTimeouts.LowPowerReadTimeout          = 0;
  HostTimeouts.HighSpeedWriteTimeout        = 0;
  HostTimeouts.HighSpeedWritePrespMode      = 0;
  HostTimeouts.LowPowerWriteTimeout         = 0;
  HostTimeouts.BTATimeout                   = 0;
  if(HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK)
  {
    Error_Handler();
  }

   /* Configure the audio driver */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;
  OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx);
  Lcd_CompObj=(&OTM8009AObj);
  OTM8009A_Init(Lcd_CompObj, OTM8009A_FORMAT_RGB888, OTM8009A_ORIENTATION_LANDSCAPE);

  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi, &LPCmd);

  /* Send Display off DCS Command to display */
  HAL_DSI_ShortWrite(&(hdsi),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPOFF,
                     0x00);

  /* Enable GPIOJ clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
  /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
  /* This pin is used only when the LCD and DSI link is configured in command mode    */
  /* Not used in DSI Video mode.                                                      */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);
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
    Layercfg.WindowX1 = HACT;
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = YSIZE_PHYS;
    Layercfg.PixelFormat = _GetPixelformat(LayerIndex);
    Layercfg.FBStartAdress = address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = HACT;
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
  }
}

void DMA2D_IRQHandler(void)
{
  if (DMA2D->ISR & DMA2D_ISR_TEIF) {
    Error_Handler(); /* Should never happen */
  }

  /* Clear the Transfer complete interrupt */
  DMA2D->IFCR = (U32)(DMA2D_IFCR_CTCIF | DMA2D_IFCR_CCTCIF | DMA2D_IFCR_CTEIF);

  /* Signal semaphore */
  osSemaphoreRelease(osDma2dSemph);
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
#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */
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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

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
  return _pBuffer_DMA2D;
}


/**
  * @brief  Set LCD updated region.
  * @param  idx: active region index.
  * @retval None
  */
void LCD_SetUpdateRegion(int idx)
{
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pCols[idx]);
}

/**
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
  uint32_t index = 0;

  __DSI_MASK_TE();

  if (LCD_ReqRefersh)
  {
    for(index = 0; index < GUI_NUM_LAYERS; index++)
    {
      if(_aPendingBuffer[index] >= 0)
      {
        GUI_MULTIBUF_ConfirmEx(index,  _aPendingBuffer[index]);
        _aPendingBuffer[index] = -1;
      }
    }
    HAL_DSI_Refresh(hdsi);
  }
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  uint32_t index = 0;

  if (LCD_ReqRefersh)
  {
    if(LCD_ActiveRegion < ZONES )
    {
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(hdsi);
      for(index = 0; index < GUI_NUM_LAYERS; index ++)
      {
        /* Update LTDC configuration */
        LTDC_LAYER(&hltdc, index)->CFBAR  = (U32 )LCD_Addr[index] + LCD_ActiveRegion  * HACT * _aBytesPerPixels[index];
      }
      __HAL_LTDC_RELOAD_CONFIG(&hltdc);
      __HAL_DSI_WRAPPER_ENABLE(hdsi);
      LCD_SetUpdateRegion(LCD_ActiveRegion++);
      /* Refresh the right part of the display */
      HAL_DSI_Refresh(hdsi);
    }
    else
    {
      __HAL_DSI_WRAPPER_DISABLE(hdsi);
      for(index = 0; index < GUI_NUM_LAYERS; index ++)
      {
        LTDC_LAYER(&hltdc, index)->CFBAR  = (U32 )LCD_Addr[index];
      }
      __HAL_LTDC_RELOAD_CONFIG(&hltdc);
      __HAL_DSI_WRAPPER_ENABLE(hdsi);
      LCD_SetUpdateRegion(0);

      LCD_ActiveRegion = 1;
      LCD_ReqRefersh   = 0;

      if(RequestDisplayOn)
      {
        RequestDisplayOn = 0;
        /* Send Display off DCS Command to display */
        HAL_DSI_ShortWrite(hdsi,
                           0,
                          DSI_DCS_SHORT_PKT_WRITE_P1,
                          OTM8009A_CMD_DISPON,
                          0x00);
      }

      /* Release the semaphore to STemWin to draw next frame */
      osSemaphoreRelease( LcdUpdateSemaphoreId );
    }
  }
}

/**
  * @brief  Enable TE Event at scanline.
  * @param  None
  * @retval None
  */
void LCD_EnableTE(void)
{
  static uint8_t ScanLineParams[2];
#if (ZONES == 4 )
  uint16_t scanline = 283;
#elif (ZONES == 2 )
  uint16_t scanline = 533;
#endif
  ScanLineParams[0] = scanline >> 8;
  ScanLineParams[1] = scanline & 0x00FF;

  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, ScanLineParams);
  /* set_tear_on */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_TEEON, 0x00);
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
      RequestDisplayOn = 0;
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
      LCD_Addr[LayerIndex] = (U32 *)LCD_GetBufferAddress(LayerIndex, p->Index);
      _aPendingBuffer[LayerIndex] = p->Index;

      __HAL_DSI_WRAPPER_DISABLE(&hdsi);
      LTDC_LAYER(&hltdc, LayerIndex)->CFBAR  = (U32 )LCD_Addr[LayerIndex];
      __HAL_LTDC_RELOAD_CONFIG(&hltdc);
      __HAL_DSI_WRAPPER_ENABLE(&hdsi);

      /* Request LCD Refresh during next tearing effect event */
      LCD_ReqRefersh = 1;

      __DSI_UNMASK_TE();

      osSemaphoreWait( LcdUpdateSemaphoreId , 1000 );
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
      RequestDisplayOn = 1;
      /* Required if the display controller should support switching on and off */
      __HAL_LTDC_ENABLE(&hltdc);
      break;
    }
    case LCD_X_OFF:
    {
      RequestDisplayOn = 0;
      /* Send Display off DCS Command to display */
      HAL_DSI_ShortWrite(&(hdsi),
                         0,
                         DSI_DCS_SHORT_PKT_WRITE_P1,
                         OTM8009A_CMD_DISPOFF,
                         0x00);
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
        __HAL_DSI_WRAPPER_DISABLE(&hdsi);
        __HAL_LTDC_LAYER_ENABLE(&hltdc, LayerIndex);
        __HAL_DSI_WRAPPER_ENABLE(&hdsi);
      }
      else
      {
        __HAL_DSI_WRAPPER_DISABLE(&hdsi);
        __HAL_LTDC_LAYER_DISABLE(&hltdc, LayerIndex);
        __HAL_DSI_WRAPPER_ENABLE(&hdsi);
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
#if !defined(MPU_DISABLE_CACHE)
    SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */
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

  /* Complete the initialisation */
  LCD_LL_Init ();

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

  __HAL_DSI_WRAPPER_DISABLE(&hdsi);

  LCD_LL_LayerInit(0, LCD_GetBufferAddress(0, 0));
  HAL_LTDC_SetPitch(&hltdc, XSIZE_PHYS, 0);
  LCD_SetBufferPtrEx(0, (U32 const *)Framebuffers[0]);
#if (GUI_NUM_LAYERS > 1)
  LCD_LL_LayerInit(1, LCD_GetBufferAddress(1, 0));
  HAL_LTDC_SetPitch(&hltdc, XSIZE_PHYS, 1);
  LCD_SetBufferPtrEx(1, (U32 const *)Framebuffers[1]);
#endif

  __HAL_DSI_WRAPPER_ENABLE(&hdsi);

  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pCols[0]);
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);

  HAL_Delay(20);

  LCD_EnableTE();
}

/*********************************************************************
*
*       CopyBuffer
*/
static void DMA2D_CopyBufferWithAlpha_filter(void * pSrc, void * pDst)
{
  U32 PixelFormat;

  osMutexWait(osDeviceMutex, osWaitForever);

  PixelFormat = _GetPixelformat(0);

  DMA2D->CR      = 0x00000000UL | (1 << 9) | (0x2 << 16) | (1 << 8);

  /* Set up pointers */
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->FGOR    = 0;
  DMA2D->OOR     = 0;
  DMA2D->BGOR     = 0;

  /* Set up pixel format */
  DMA2D->FGPFCCR = PixelFormat;
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;

  /*  Set up size */
  DMA2D->NLR     = (U32)(800 << 16) | (U16)480;

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

  /* Start transfer */
  DMA2D->CR |= DMA2D_CR_START;

  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}


void LCD_LL_DrawBitmap32bpp_filter(U8 const * p)
{
  U32 AddrDst;

  AddrDst = LCD_GetBufferAddress(0, _aBufferIndex[0]);
  DMA2D_CopyBufferWithAlpha_filter((void *)AddrDst, (void *)p);
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
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}
/**
  * @brief  DCS or Generic short/long write command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be written
  * @param  pData pointer to a buffer of data to be write
  * @param  Size To precise command to be used (short or long)
  * @retval BSP status
  */
static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Size <= 1U)
  {
    if(HAL_DSI_ShortWrite(&hdsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  else
  {
    if(HAL_DSI_LongWrite(&hdsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  DCS or Generic read command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be read
  * @param  pData pointer to a buffer to store the payload of a read back operation.
  * @param  Size  Data size to be read (in byte).
  * @retval BSP status
  */
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_DSI_Read(&hdsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  return ret;
}

static U32 _GetBitmapPixelformat(const void *pDst)
{
  U32 format;
  const GUI_BITMAP *bitmap = (GUI_BITMAP *)pDst;

  switch(bitmap->BitsPerPixel)
  {
    case 16:
      format = DMA2D_OUTPUT_RGB565;
    break;
    case 24:
      format = DMA2D_OUTPUT_RGB888;
    break;
    default :
    case 32:
      format = DMA2D_OUTPUT_ARGB8888;
    break;
  }

  return format;
}

static U32 _GetBitmapDataAddress(const void *pDst)
{
  const GUI_BITMAP *bitmap = (GUI_BITMAP *)pDst;

  return (U32 )bitmap->pData;
}

/**
  * @brief  DMA2D conversion from YCbCr (video output) to RGB (Display frame buffer)
  * @param  LayerIndex : LCD Layer Index
  * @param  pSrc : Source buffer pointer
  * @param  pDst : Destination buffer pointer
  * @param  xSize: Horizontal size of the frame buffer
  * @param  ySize: Vertical size of the frame buffer
  * @param  aCssMode: CSS Mode
  * @retval None
  */
void DMA2D_CopyBuffer_YCbCr_To_RGB(int LayerIndex, const void * pSrc, const void * pDst, int xSize, int ySize, int aCssMode)
{
  uint32_t inputLineOffset = 0;
  U32 regValue, regMask;
  U32 Destination, PixelFormat;

  if(pDst)
  {
    Destination = _GetBitmapDataAddress(pDst);
    PixelFormat = _GetBitmapPixelformat(pDst);
  }
  else
  {
    Destination = LCD_GetBufferAddress(LayerIndex, _aBufferIndex[LayerIndex]);
    PixelFormat = _GetPixelformat(LayerIndex);
  }

  switch (aCssMode)
  {
    case DMA2D_CSS_420 :
    case DMA2D_CSS_422 :  
      inputLineOffset = xSize % 16;
      if(inputLineOffset != 0)
      {
        inputLineOffset = 16 - inputLineOffset;
      }
    break;
    case DMA2D_NO_CSS :
    default :  
      inputLineOffset = xSize % 8;
      if(inputLineOffset != 0)
      {
        inputLineOffset = 8 - inputLineOffset;
      }
    break;
  }

  regValue = DMA2D_INPUT_YCBCR | (DMA2D_REPLACE_ALPHA << 16) \
           | (DMA2D_REGULAR_ALPHA << 20) | (DMA2D_RB_REGULAR << 21) \
           | (0xFFU << 24) | (aCssMode << 18);

  regMask  = DMA2D_BGPFCCR_CM | DMA2D_BGPFCCR_AM | DMA2D_BGPFCCR_ALPHA | DMA2D_BGPFCCR_AI | DMA2D_BGPFCCR_RBS | DMA2D_FGPFCCR_CSS;

  osMutexWait(osDeviceMutex, osWaitForever);

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00010000UL | (1 << 9) | (1 << 8);
  DMA2D->OPFCCR  = PixelFormat;
  DMA2D->OOR     = 0;
  DMA2D->OPFCCR  |= (DMA2D_REGULAR_ALPHA << 20);
  DMA2D->OPFCCR  |= (DMA2D_RB_REGULAR << 21);
  DMA2D->FGPFCCR = (regMask & regValue);
  DMA2D->FGOR    = inputLineOffset;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;
  DMA2D->OMAR    = Destination;
  DMA2D->FGMAR   = (U32)pSrc;

#if !defined(MPU_DISABLE_CACHE)
  SCB_CleanInvalidateDCache();
#endif /* !MPU_DISABLE_CACHE */

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR |= DMA2D_CR_START;

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();

  osMutexRelease(osDeviceMutex);
}


