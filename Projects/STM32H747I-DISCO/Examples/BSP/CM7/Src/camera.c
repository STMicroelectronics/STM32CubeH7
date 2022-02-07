/**
  ******************************************************************************
  * @file    BSP/CM7/Src/camera.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the camera feature in the
  *          stm32h747i_discovery driver
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


/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Camera Frame Buffer address */

#define CAMERA_STATUS_TEXT_POS    30            /* Number of lines from bottom of screen */

#define CAMERA_RES_INDEX_MIN      CAMERA_R160x120
#define CAMERA_RES_INDEX_MAX      CAMERA_R640x480


/* Private macro -------------------------------------------------------------*/
extern __IO uint32_t ButtonState;
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t JoyState = JOY_NONE;
static uint32_t   CameraChangeResolutionAsked;
static volatile uint32_t   CameraFrameBufferInitComplete = 0;
static volatile uint32_t   CameraFrameBufferInitError = 0;
static int32_t brightness =CAMERA_BRIGHTNESS_MIN;
static int32_t contrast = CAMERA_CONTRAST_MIN;
uint32_t CameraResolution[4] = {CAMERA_R160x120, CAMERA_R320x240, CAMERA_R480x272, CAMERA_R640x480};
uint32_t CameraResX[5] = {160, 320, 480, 640, 800};
uint32_t CameraResY[5] = {120, 240, 272, 480, 480};
uint32_t xsize,ysize;
uint32_t index_resolution=0 ;
/* Global variables */
volatile uint32_t Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff = 0;
static void LCD_LL_Convert_RGB565ToARGB8888(void *pSrc, void *pDst, uint16_t xsize, uint16_t ysize);
/* Imported global variables */

/* Private function prototypes -----------------------------------------------*/
static void Camera_SetHint(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
void Camera_demo (void)
{
  uint32_t exit = 0;
  uint32_t camera_status = BSP_ERROR_NONE;
  BSP_LCD_GetXSize(0,&xsize);
  BSP_LCD_GetYSize(0,&ysize);

  Camera_SetHint();

  /*## Camera Initialization and start capture ############################*/

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"Wait for camera initialization...", CENTER_MODE);
  CameraChangeResolutionAsked = 1;
  /* Infinite loop */
  while (exit == 0)
  {
    if (CameraChangeResolutionAsked)  /* A camera change resolution has been asked */
    {
      CameraChangeResolutionAsked = 0;

      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);

      switch (CameraResolution[index_resolution])
      {
        case CAMERA_R160x120:
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(0,CameraResolution[0],CAMERA_PF_RGB565);
          UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QQVGA camera stream       ", CENTER_MODE);
          break;

        case CAMERA_R320x240:
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(0,CameraResolution[1],CAMERA_PF_RGB565);
          UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QVGA camera stream        ", CENTER_MODE);
          break;

        case CAMERA_R480x272:

          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(0,CameraResolution[2],CAMERA_PF_RGB565);
          UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"      480x272 camera stream      ", CENTER_MODE);
          break;

        case CAMERA_R640x480:
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(0,CameraResolution[3],CAMERA_PF_RGB565);
          UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        VGA camera stream        ", CENTER_MODE);
          break;

        default :
          break;
      }

      if (camera_status != BSP_ERROR_NONE)
      {
        UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS - 15, (uint8_t *)"        CAMERA INIT ERROR        ", CENTER_MODE);
        HAL_Delay(2000);
        break ;
      }

      /* Start / Restart camera stream */
      BSP_CAMERA_Start(0, (uint8_t *)CAMERA_FRAME_BUFFER, CAMERA_MODE_CONTINUOUS);

    }
    while( brightness < CAMERA_BRIGHTNESS_MAX)
    {
      brightness++;
      BSP_CAMERA_SetBrightness(0,brightness);
      UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        Brightness change        ", CENTER_MODE);
      HAL_Delay(500);
    }
     while( contrast < CAMERA_CONTRAST_MAX)
    {
      contrast++;
      BSP_CAMERA_SetContrast(0,contrast);
      UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"          Contrast change        ", CENTER_MODE);
      HAL_Delay(500);
    }
    BSP_CAMERA_EnableNightMode(0);
    UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"          Night Mode ON        ", CENTER_MODE);
    HAL_Delay(1000);
    BSP_CAMERA_DisableNightMode(0);
    UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"          Night Mode OFF        ", CENTER_MODE);
    HAL_Delay(1000);
    BSP_CAMERA_SetMirrorFlip(0,CAMERA_MIRRORFLIP_NONE);
    UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"          MirrorFlip None        ", CENTER_MODE);
    HAL_Delay(1000);
    BSP_CAMERA_SetMirrorFlip(0,CAMERA_MIRRORFLIP_FLIP);
    UTIL_LCD_DisplayStringAt(0, ysize - CAMERA_STATUS_TEXT_POS, (uint8_t *)"          MIRRORFLIP FLIP        ", CENTER_MODE);
    HAL_Delay(1000);
    BSP_LED_Toggle(LED4);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    BSP_LED_Toggle(LED2);

    /* Insert 100 ms delay */
    HAL_Delay(100);
    if (CheckForUserInput() > 0)
    {
      if (index_resolution < 3)
      {
        index_resolution++;
        /* Switch to higher resolution */
        CameraChangeResolutionAsked = 1;
        ButtonState=0;
        BSP_CAMERA_DeInit(0);
        brightness =CAMERA_BRIGHTNESS_MIN;
        contrast = CAMERA_CONTRAST_MIN;
        Camera_SetHint();
        /* Insert 10 ms delay */
        HAL_Delay(100);
      }
      else
      {

        BSP_CAMERA_DeInit(0);
        BSP_LED_Off(LED2);
        BSP_LED_Off(LED4);
        exit = 1;
        return;
      }
    }
  }
  /* End of camera demo */
}

/**
  * @brief  Display Camera demo hint
  * @param  None
  * @retval None
  */
static void Camera_SetHint(void)
{
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, 0, xsize, 80,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAMERA EXAMPLE", CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"     Press  Wakeup button for next resolution     ", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 90, xsize - 20, ysize - 100,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, xsize - 22, ysize - 102,UTIL_LCD_COLOR_BLUE);
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(uint32_t Instance)
{
  BSP_CAMERA_Suspend(0);

  /* Convert captured frame to ARGB8888 and copy it to LCD FRAME BUFFER */
  LCD_LL_Convert_RGB565ToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER), (uint32_t *)(LCD_LAYER_0_ADDRESS), CameraResX[index_resolution], CameraResY[index_resolution]);



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
  uint32_t LcdResX,LcdResY;
  BSP_LCD_GetXSize(0, &LcdResX);
  BSP_LCD_GetYSize(0, &LcdResY);
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
  * @}
  */

/**
  * @}
  */

