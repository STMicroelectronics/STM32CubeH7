/**
  ******************************************************************************
  * @file    BSP/Src/camera.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the camera features of the
  *          stm32h7b3i_eval_camera driver
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
#include "main.h"
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WVGA_RES_X      800
#define WVGA_RES_Y      480

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t   LcdResX    = WVGA_RES_X; /* WVGA landscape 800x480 */
static uint32_t   LcdResY    = WVGA_RES_Y;

/* Camera resolutions */
uint32_t CameraResolution[5] = {CAMERA_R160x120, CAMERA_R320x240, CAMERA_R480x272, CAMERA_R640x480, CAMERA_R800x480};
uint8_t  CameraLightMode[5] = {CAMERA_LIGHT_AUTO, CAMERA_LIGHT_SUNNY, CAMERA_LIGHT_OFFICE, CAMERA_LIGHT_CLOUDY, CAMERA_LIGHT_HOME};
uint32_t CameraPixelFormat[3] = {CAMERA_PF_RGB565, CAMERA_PF_RGB888, CAMERA_PF_YUV422};

uint8_t  CameraEffect[7] =
{
  CAMERA_COLOR_EFFECT_NONE,
  CAMERA_COLOR_EFFECT_BLUE,
  CAMERA_COLOR_EFFECT_RED,
  CAMERA_COLOR_EFFECT_GREEN,
  CAMERA_COLOR_EFFECT_BW,
  CAMERA_COLOR_EFFECT_SEPIA,
  CAMERA_COLOR_EFFECT_NEGATIVE,
};

uint8_t  CameraMirrorFlip[4] =
{
  CAMERA_MIRRORFLIP_NONE,
  CAMERA_MIRRORFLIP_FLIP,
  CAMERA_MIRRORFLIP_MIRROR,
  CAMERA_MIRRORFLIP_FLIP | CAMERA_MIRRORFLIP_MIRROR,
};

uint32_t CameraResX[5] = {160, 320, 480, 640, 800};
uint32_t CameraResY[5] = {120, 240, 272, 480, 480};
int32_t HueIndex = -6, BrightnessIndex = -4, SaturationIndex = -4, ContrastIndex = -4;
uint32_t MirrorFlipIndex = 0, lightIndex = 0, EffectIndex = 0, CameraResIndex = 0, PixelFormat = 0;
CAMERA_Capabilities_t Cap;

/* Private function prototypes -----------------------------------------------*/
static void Camera_SetHint(void);
static void LCD_LL_Convert_RGB565ToARGB8888(void *pSrc, void *pDst, uint16_t xsize, uint16_t ysize);
static void LCD_LL_Convert_RGB888ToARGB8888(void *pSrc, void *pDst, uint16_t xsize, uint16_t ysize);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
void Camera_demo (void)
{
  uint32_t y_size;

  BSP_LCD_GetYSize(0, &y_size);

  Camera_SetHint();
  CameraTest = 1;

  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  /* Display USB initialization message */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_SetFont(&Font24);

  CameraResIndex = 0;

  /* Initialize the Camera */
  if(BSP_CAMERA_Init(0, CameraResolution[CameraResIndex], CAMERA_PF_RGB565) != BSP_ERROR_NONE)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(20, (y_size - 24), (uint8_t *)" NO CAMERA CONNECTED", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, (y_size - 24), (uint8_t *)"CAMERA CONTINUOUS MODE", CENTER_MODE);
    /* Wait for the camera initialization after HW reset*/
    HAL_Delay(100);

    BSP_CAMERA_GetCapabilities(0, &Cap);

    /* Start the Camera Capture */
    BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);

    /* Test Camera Zooming */
    if(Cap.Zoom != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Zoom feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x2", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x2);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x4", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x4);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x8", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x8);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x1", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x1);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      CameraResIndex = 1;
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetResolution(0, CameraResolution[CameraResIndex]);
      HAL_Delay(100);
      /* Start the Camera Capture */
      UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);

      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x2", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x2);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x4", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x4);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x1", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x1);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      CameraResIndex = 2;
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetResolution(0, CameraResolution[CameraResIndex]);
      HAL_Delay(100);
      /* Start the Camera Capture */
      UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);

      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x2", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x2);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Zoom x1", CENTER_MODE);
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetZoom(0, CAMERA_ZOOM_x1);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      CameraResIndex = 3;
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetResolution(0, CameraResolution[CameraResIndex]);
      HAL_Delay(100);
      /* Start the Camera Capture */
      UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);

      CameraResIndex = 4;
      BSP_CAMERA_Stop(0);
      BSP_CAMERA_SetResolution(0, CameraResolution[CameraResIndex]);
      HAL_Delay(100);
      /* Start the Camera Capture */
      UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      HAL_Delay(2000);
    }

    CameraResIndex = 2;
    BSP_CAMERA_Stop(0);
    BSP_CAMERA_SetResolution(0, CameraResolution[CameraResIndex]);
    /* Start the Camera Capture */
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
    if(Cap.MirrorFlip != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera MirrorFlip feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"                                               ", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Mirror/Flip TEST", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      /* Test Camera Mirror/Flip */
      while(MirrorFlipIndex < 4)
      {
        HAL_Delay(2000);
        BSP_CAMERA_SetMirrorFlip(0, CameraMirrorFlip[MirrorFlipIndex]);
        MirrorFlipIndex++;
      }
      HAL_Delay(2000);
      MirrorFlipIndex = 1;
      BSP_CAMERA_SetMirrorFlip(0, CameraMirrorFlip[MirrorFlipIndex]);
    }
    BSP_CAMERA_SetMirrorFlip(0, CAMERA_MIRRORFLIP_FLIP | CAMERA_MIRRORFLIP_MIRROR);

    HAL_Delay(2000);
    if(Cap.HueDegree != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Hue degree feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"                                               ", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Hue Degree TEST", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);

      /* Test Camera Hue Degree */
      while(HueIndex < 6)
      {
        HAL_Delay(2000);
        BSP_CAMERA_SetHueDegree(0, HueIndex);
        HueIndex++;
      }

      HAL_Delay(2000);
      HueIndex = 0;
      BSP_CAMERA_SetHueDegree(0, HueIndex);
    }

    if(Cap.LightMode != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Light Mode feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"                                               ", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Light Mode TEST", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      /* Test Camera Light Mode */
      while(lightIndex < 5)
      {
        HAL_Delay(2000);
        BSP_CAMERA_SetLightMode(0, CameraLightMode[lightIndex]);
        lightIndex++;
      }
      HAL_Delay(2000);
      lightIndex = 0;
      BSP_CAMERA_SetLightMode(0, CameraLightMode[0]);
    }

    if(Cap.Contrast != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Contrast feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"                                               ", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Contrast TEST", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      /* Test Contrast */
      while(ContrastIndex < 5)
      {
        HAL_Delay(2000);
        BSP_CAMERA_SetContrast(0, ContrastIndex);
        ContrastIndex++;
      }
      ContrastIndex = 0;
      HAL_Delay(2000);
      BSP_CAMERA_SetContrast(0, ContrastIndex);
    }

    if(Cap.Saturation != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Saturation feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"                                               ", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Saturation TEST", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      /* Test Saturation */
      while(SaturationIndex < 5)
      {
        HAL_Delay(2000);
        BSP_CAMERA_SetSaturation(0, SaturationIndex);
        SaturationIndex++;
      }

      SaturationIndex = 0;
      HAL_Delay(2000);
      BSP_CAMERA_SetSaturation(0, SaturationIndex);
    }

    if(Cap.Brightness != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Brightness feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"                                               ", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Brightness TEST", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);

      /* Test Brightness */
      while(BrightnessIndex < 5)
      {
        HAL_Delay(2000);
        BSP_CAMERA_SetBrightness(0, BrightnessIndex);
        BrightnessIndex++;
      }
      BrightnessIndex = 0;
      HAL_Delay(2000);
      BSP_CAMERA_SetBrightness(0, BrightnessIndex);
    }

    if(Cap.ColorEffect != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Special Effect feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"                                               ", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Special Effect TEST                 ", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      /* Test Camera Special Effects */
      while(EffectIndex < 7)
      {
        HAL_Delay(2000);
        BSP_CAMERA_SetColorEffect(0, CameraEffect[EffectIndex]);
        EffectIndex++;
      }
      EffectIndex = 0;
      HAL_Delay(2000);
      BSP_CAMERA_SetColorEffect(0, CameraEffect[EffectIndex]);
    }

    if(Cap.NightMode != 1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Camera Night Mode feature not supported", CENTER_MODE);
      HAL_Delay(2000);
    }
    else
    {
      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAMERA NIGHT MODE ENABLED                  ", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      BSP_CAMERA_EnableNightMode(0);
      HAL_Delay(2000);

      BSP_CAMERA_Stop(0);
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAMERA NIGHT MODE DISABLED                 ", CENTER_MODE);
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);
      BSP_CAMERA_DisableNightMode(0);
      HAL_Delay(2000);
    }
  }
  ButtonState = 0;
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      CameraTest = 0;
      BSP_CAMERA_DeInit(0);
      return;
    }
  }
}

/**
  * @brief  Display Camera demo hint
  * @param  None
  * @retval None
  */
static void Camera_SetHint(void)
{
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set Camera Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 120, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAMERA EXAMPLE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"     Press User button for next menu     ", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 130, x_size - 20, y_size - 130, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 131, x_size - 22, y_size - 132, UTIL_LCD_COLOR_BLUE);
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(uint32_t Instance)
{
  BSP_CAMERA_Suspend(0);

  if(PixelFormat == CAMERA_PF_RGB888)
  {
  /* Convert captured frame to ARGB8888 and copy it to LCD FRAME BUFFER */
  LCD_LL_Convert_RGB888ToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER), (uint32_t *)(LCD_FRAME_BUFFER), CameraResX[CameraResIndex], CameraResY[CameraResIndex]);
  }
  else if(PixelFormat == CAMERA_PF_RGB565)
  {
  /* Convert captured frame to ARGB8888 and copy it to LCD FRAME BUFFER */
  LCD_LL_Convert_RGB565ToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER), (uint32_t *)(LCD_FRAME_BUFFER), CameraResX[CameraResIndex], CameraResY[CameraResIndex]);
  }
  else
  {
  }

  BSP_CAMERA_Resume(0);
}

/**
  * @brief  Copy the Captured Picture to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @retval None
  */
static void LCD_LL_Convert_RGB565ToARGB8888(void *pSrc, void *pDst, uint16_t xsize, uint16_t ysize)
{
  uint32_t xPos, yPos, destination;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hlcd_dma2d.Init.Mode         = DMA2D_M2M_PFC;
  hlcd_dma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hlcd_dma2d.Init.OutputOffset = LcdResX - xsize;
  /* DMA2D Callbacks Configuration */
  hlcd_dma2d.XferCpltCallback  = NULL;

  /* Foreground Configuration */
  hlcd_dma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  hlcd_dma2d.LayerCfg[1].InputAlpha = 0xFF;
  hlcd_dma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hlcd_dma2d.LayerCfg[1].InputOffset = 0;

  hlcd_dma2d.Instance = DMA2D;

  /* Calculate the destination transfer address */
  xPos = (LcdResX  - xsize)/2;
  yPos = (LcdResY  - ysize)/2;

  destination = (uint32_t)pDst + (yPos * LcdResX + xPos) * 4;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hlcd_dma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hlcd_dma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hlcd_dma2d, (uint32_t)pSrc, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hlcd_dma2d, 100);
      }
    }
  }
}

/**
  * @brief  Copy the Captured Picture to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @retval None
  */
static void LCD_LL_Convert_RGB888ToARGB8888(void *pSrc, void *pDst, uint16_t xsize, uint16_t ysize)
{
  uint32_t xPos, yPos, destination;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hlcd_dma2d.Init.Mode         = DMA2D_M2M_PFC;
  hlcd_dma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hlcd_dma2d.Init.OutputOffset = LcdResX - xsize;
  /* DMA2D Callbacks Configuration */
  hlcd_dma2d.XferCpltCallback  = NULL;

  /* Foreground Configuration */
  hlcd_dma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  hlcd_dma2d.LayerCfg[1].InputAlpha = 0xFF;
  hlcd_dma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
  hlcd_dma2d.LayerCfg[1].InputOffset = 0;

  hlcd_dma2d.Instance = DMA2D;

  /* Calculate the destination transfer address */
  xPos = (LcdResX  - xsize)/2;
  yPos = (LcdResY  - ysize)/2;

  destination = (uint32_t)pDst + (yPos * LcdResX + xPos) * 4;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hlcd_dma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hlcd_dma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hlcd_dma2d, (uint32_t)pSrc, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hlcd_dma2d, 100);
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

