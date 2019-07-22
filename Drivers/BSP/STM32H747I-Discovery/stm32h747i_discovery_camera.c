/**
  ******************************************************************************
  * @file    stm32h747i_discovery.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Camera modules mounted on
  *          STM32H747I-DISCOVERY board.
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

/* File Info: ------------------------------------------------------------------
                                   User NOTES
1. How to use this driver:
--------------------------
   - This driver is used to drive the camera.
   - The OV9655 component driver MUST be included with this driver.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the camera using the BSP_CAMERA_Init() function.
     o Start the camera capture/snapshot using the CAMERA_Start() function.
     o Suspend, resume or stop the camera capture using the following functions:
      - BSP_CAMERA_Suspend()
      - BSP_CAMERA_Resume()
      - BSP_CAMERA_Stop()

  + Options
     o Increase or decrease on the fly the brightness and/or contrast
       using the following function:
       - BSP_CAMERA_ContrastBrightnessConfig
     o Add a special effect on the fly using the following functions:
       - BSP_CAMERA_BlackWhiteConfig()
       - BSP_CAMERA_ColorEffectConfig()

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32h747i_discovery_camera.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_DISCOVERY
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY_CAMERA STM32H747I_DISCOVERY_CAMERA
  * @{
  */

/** @defgroup STM32H747I_DISCOVERY_CAMERA_Exported_Variables Exported Variables
  * @{
  */ 
DCMI_HandleTypeDef  hdcmi_discovery;
CAMERA_DrvTypeDef   *camera_drv;
/**
  * @}
  */
  
/** @defgroup STM32H747I_DISCOVERY_CAMERA_Private_Variables Private Variables
  * @{
  */   
/* Camera current resolution naming (QQVGA, VGA, ...) */
static uint32_t CameraCurrentResolution;

/* Camera image rotation on LCD Displayed frame buffer */
uint32_t CameraRotation = CAMERA_ROTATION_INVALID;

/* Camera module I2C HW address */
static uint32_t CameraHwAddress;
/**
  * @}
  */

/** @defgroup STM32H747I_DISCOVERY_CAMERA_Private_FunctionPrototypes Private FunctionPrototypes
  * @{
  */
static uint32_t GetSize(uint32_t Resolution);
/**
  * @}
  */

/** @defgroup STM32H747I_DISCOVERY_CAMERA_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Set Camera image rotation on LCD Displayed frame buffer.
  * @param  rotation : uint32_t rotation of camera image in preview buffer sent to LCD
  *         need to be of type Camera_ImageRotationTypeDef
  * @retval Camera status
  */
uint8_t BSP_CAMERA_SetRotation(uint32_t rotation)
{
  uint8_t status = CAMERA_ERROR;

  if(rotation < CAMERA_ROTATION_INVALID)
  {
    /* Set Camera image rotation on LCD Displayed frame buffer */
    CameraRotation = rotation;
    status = CAMERA_OK;
  }

  return status;
}

/**
  * @brief  Get Camera image rotation on LCD Displayed frame buffer.
  * @retval rotation : uint32_t value of type Camera_ImageRotationTypeDef
  */
uint32_t BSP_CAMERA_GetRotation(void)
{
  return(CameraRotation);
}

/**
  * @brief  Initializes the camera.
  * @param  Resolution : camera sensor requested resolution (x, y) : standard resolution
  *         naming QQVGA, QVGA, VGA ...
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Init(uint32_t Resolution)
{
  DCMI_HandleTypeDef *phdcmi;
  uint8_t status = CAMERA_ERROR;

  /* Get the DCMI handle structure */
  phdcmi = &hdcmi_discovery;

  /*** Configures the DCMI to interface with the camera module ***/
  /* DCMI configuration */
  phdcmi->Init.CaptureRate      = DCMI_CR_ALL_FRAME;
  phdcmi->Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
  phdcmi->Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
  phdcmi->Init.VSPolarity       = DCMI_VSPOLARITY_HIGH;
  phdcmi->Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  phdcmi->Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;
  phdcmi->Instance              = DCMI;

  /* Power up camera */
  BSP_CAMERA_PwrUp();

  /* Read ID of Camera module via I2C */
  if(ov9655_ReadID(CAMERA_I2C_ADDRESS) == OV9655_ID)
  {
    /* Initialize the camera driver structure */
    camera_drv = &ov9655_drv;
    CameraHwAddress = CAMERA_I2C_ADDRESS;

    /* DCMI Initialization */
    BSP_CAMERA_MspInit(&hdcmi_discovery, NULL);
    HAL_DCMI_Init(phdcmi);

    /* Camera Module Initialization via I2C to the wanted 'Resolution' */
    if (Resolution == CAMERA_R480x272)
    {     /* For 480x272 resolution, the OV9655 sensor is set to VGA resolution
           * as OV9655 doesn't supports 480x272 resolution,
           * then DCMI is configured to output a 480x272 cropped window */
      camera_drv->Init(CameraHwAddress, CAMERA_R640x480);
      HAL_DCMI_ConfigCROP(phdcmi,           /* Crop in the middle of the VGA picture */
                         (CAMERA_VGA_RES_X - CAMERA_480x272_RES_X)/2,
                         (CAMERA_VGA_RES_Y - CAMERA_480x272_RES_Y)/2,
                         (CAMERA_480x272_RES_X * 2) - 1,
                          CAMERA_480x272_RES_Y - 1);
      HAL_DCMI_EnableCROP(phdcmi);
    }
    else
    {
      camera_drv->Init(CameraHwAddress, Resolution);
      HAL_DCMI_DisableCROP(phdcmi);
    }

    CameraCurrentResolution = Resolution;

    /* Return CAMERA_OK status */
    status = CAMERA_OK;
  }
  else
  {
    /* Return CAMERA_NOT_SUPPORTED status */
    status = CAMERA_NOT_SUPPORTED;
  }

  return status;
}


/**
  * @brief  DeInitializes the camera.
  * @retval Camera status
  */
uint8_t BSP_CAMERA_DeInit(void)
{ 
  hdcmi_discovery.Instance              = DCMI;

  HAL_DCMI_DeInit(&hdcmi_discovery);
  BSP_CAMERA_MspDeInit(&hdcmi_discovery, NULL);
  return CAMERA_OK;
}

/**
  * @brief  Starts the camera capture in continuous mode.
  * @param  buff: pointer to the camera output buffer
  * @retval None
  */
void BSP_CAMERA_ContinuousStart(uint8_t *buff)
{
  /* Start the camera capture */
  HAL_DCMI_Start_DMA(&hdcmi_discovery, DCMI_MODE_CONTINUOUS, (uint32_t)buff, GetSize(CameraCurrentResolution));
}

/**
  * @brief  Starts the camera capture in snapshot mode.
  * @param  buff: pointer to the camera output buffer
  * @retval None
  */
void BSP_CAMERA_SnapshotStart(uint8_t *buff)
{
  /* Start the camera capture */
  HAL_DCMI_Start_DMA(&hdcmi_discovery, DCMI_MODE_SNAPSHOT, (uint32_t)buff, GetSize(CameraCurrentResolution));
}

/**
  * @brief Suspend the CAMERA capture 
  * @retval None
  */
void BSP_CAMERA_Suspend(void) 
{
  /* Suspend the Camera Capture */
  HAL_DCMI_Suspend(&hdcmi_discovery);
}

/**
  * @brief Resume the CAMERA capture 
  * @retval None
  */
void BSP_CAMERA_Resume(void)
{
  /* Start the Camera Capture */
  HAL_DCMI_Resume(&hdcmi_discovery);
}

/**
  * @brief  Stop the CAMERA capture 
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Stop(void)
{
  uint8_t status = CAMERA_ERROR;

  if(HAL_DCMI_Stop(&hdcmi_discovery) == HAL_OK)
  {
     status = CAMERA_OK;
  }

  /* Set Camera in Power Down */
  BSP_CAMERA_PwrDown();

  return status;
}

/**
  * @brief  CANERA power up
  * @retval None
  */
void BSP_CAMERA_PwrUp(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable GPIO clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*** Configure the GPIO ***/
  /* Configure DCMI GPIO as alternate function */
  gpio_init_structure.Pin       = GPIO_PIN_14;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

  /* De-assert the camera POWER_DOWN pin (active high) */
  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14, GPIO_PIN_RESET);

  HAL_Delay(3);     /* POWER_DOWN de-asserted during 3ms */
}

/**
  * @brief  CAMERA power down
  * @retval None
  */
void BSP_CAMERA_PwrDown(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable GPIO clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*** Configure the GPIO ***/
  /* Configure DCMI GPIO as alternate function */
  gpio_init_structure.Pin       = GPIO_PIN_14;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

  /* Assert the camera POWER_DOWN pin (active high) */
  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14, GPIO_PIN_SET);
}

/**
  * @brief  Configures the camera contrast and brightness.
  * @param  contrast_level: Contrast level
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_CONTRAST_LEVEL4: for contrast +2
  *            @arg  CAMERA_CONTRAST_LEVEL3: for contrast +1
  *            @arg  CAMERA_CONTRAST_LEVEL2: for contrast  0
  *            @arg  CAMERA_CONTRAST_LEVEL1: for contrast -1
  *            @arg  CAMERA_CONTRAST_LEVEL0: for contrast -2
  * @param  brightness_level: Contrast level
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_BRIGHTNESS_LEVEL4: for brightness +2
  *            @arg  CAMERA_BRIGHTNESS_LEVEL3: for brightness +1
  *            @arg  CAMERA_BRIGHTNESS_LEVEL2: for brightness  0
  *            @arg  CAMERA_BRIGHTNESS_LEVEL1: for brightness -1
  *            @arg  CAMERA_BRIGHTNESS_LEVEL0: for brightness -2
  */
void BSP_CAMERA_ContrastBrightnessConfig(uint32_t contrast_level, uint32_t brightness_level)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CameraHwAddress, CAMERA_CONTRAST_BRIGHTNESS, contrast_level, brightness_level);
  }  
}

/**
  * @brief  Configures the camera white balance.
  * @param  Mode: black_white mode
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_BLACK_WHITE_BW
  *            @arg  CAMERA_BLACK_WHITE_NEGATIVE
  *            @arg  CAMERA_BLACK_WHITE_BW_NEGATIVE
  *            @arg  CAMERA_BLACK_WHITE_NORMAL
  */
void BSP_CAMERA_BlackWhiteConfig(uint32_t Mode)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CameraHwAddress, CAMERA_BLACK_WHITE, Mode, 0);
  }
}

/**
  * @brief  Configures the camera color effect.
  * @param  Effect: Color effect
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_COLOR_EFFECT_ANTIQUE               
  *            @arg  CAMERA_COLOR_EFFECT_BLUE        
  *            @arg  CAMERA_COLOR_EFFECT_GREEN    
  *            @arg  CAMERA_COLOR_EFFECT_RED        
  * @retval None
  */
void BSP_CAMERA_ColorEffectConfig(uint32_t Effect)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CameraHwAddress, CAMERA_COLOR_EFFECT, Effect, 0);
  }
}

/**
  * @brief  Get the capture size in pixels unit.
  * @param  Resolution: the current resolution.
  * @retval capture size in pixels unit.
  */
static uint32_t GetSize(uint32_t Resolution)
{
  uint32_t size = 0;

  /* Get capture size */
  switch (Resolution)
  {
  case CAMERA_R160x120:
    {
      size =  0x2580;
    }
    break;
  case CAMERA_R320x240:
    {
      size =  0x9600;
    }
    break;
  case CAMERA_R480x272:
    {
      size =  0xFF00;
    }
    break;
  case CAMERA_R640x480:
    {
      size =  0x25800;
    }
    break;
  default:
    {
      break;
    }
  }

  return size;
}

/**
  * @brief  Initializes the DCMI MSP.
  * @param  hdcmi: HDMI handle
  * @param  Params : pointer on additional configuration parameters, can be NULL. 
  * @retval None
  */
__weak void BSP_CAMERA_MspInit(DCMI_HandleTypeDef *hdcmi, void *Params)
{
  static DMA_HandleTypeDef hdma_handler;
  GPIO_InitTypeDef gpio_init_structure;

  /*** Enable peripherals and GPIO clocks ***/
  /* Enable DCMI clock */
  __HAL_RCC_DCMI_CLK_ENABLE();

  /* Enable DMA clock */
  CAMERA_DCMI_DMAx_CLK_ENABLE();

  /* Enable GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*** Configure the GPIO ***/
  /* Configure DCMI GPIO as alternate function */
  gpio_init_structure.Pin       = GPIO_PIN_4 | GPIO_PIN_6;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  gpio_init_structure.Pin       = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  gpio_init_structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_11;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  gpio_init_structure.Pin       = GPIO_PIN_3;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  gpio_init_structure.Pin       = GPIO_PIN_10;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /*** Configure the DMA ***/
  /* Set the parameters to be configured */
  hdma_handler.Init.Request             = DMA_REQUEST_DCMI;
  hdma_handler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_handler.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_handler.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_handler.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_handler.Init.Mode                = DMA_CIRCULAR;
  hdma_handler.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  hdma_handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_handler.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_handler.Init.PeriphBurst         = DMA_PBURST_SINGLE; 

  hdma_handler.Instance = CAMERA_DCMI_DMAx_STREAM;

  /* Associate the initialized DMA handle to the DCMI handle */
  __HAL_LINKDMA(hdcmi, DMA_Handle, hdma_handler);
  
  /*** Configure the NVIC for DCMI and DMA ***/
  /* NVIC configuration for DCMI transfer complete interrupt */
  HAL_NVIC_SetPriority(DCMI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DCMI_IRQn);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(CAMERA_DCMI_DMAx_IRQ, 0x0F, 0);
  HAL_NVIC_EnableIRQ(CAMERA_DCMI_DMAx_IRQ);

  /* Configure the DMA stream */
  HAL_DMA_Init(hdcmi->DMA_Handle);
}

/**
  * @brief  DeInitializes the DCMI MSP.
  * @param  hdcmi: HDMI handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  * @retval None
  */
__weak void BSP_CAMERA_MspDeInit(DCMI_HandleTypeDef *hdcmi, void *Params)
{
  /* Disable NVIC  for DCMI transfer complete interrupt */
  HAL_NVIC_DisableIRQ(DCMI_IRQn);  
  
  /* Disable NVIC for DMA2 transfer complete interrupt */
  HAL_NVIC_DisableIRQ(CAMERA_DCMI_DMAx_IRQ);
  
  /* Configure the DMA stream */
  HAL_DMA_DeInit(hdcmi->DMA_Handle);  

  /* Disable DCMI clock */
  __HAL_RCC_DCMI_CLK_DISABLE();

  /* GPIO pins clock and DMA clock can be shut down in the application
     by surcharging this __weak function */
}

/**
  * @brief  Line event callback
  * @param  hdcmi: pointer to the DCMI handle
  * @retval None
  */
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  BSP_CAMERA_LineEventCallback();
}

/**
  * @brief  Line Event callback.
  * @retval None
  */
__weak void BSP_CAMERA_LineEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_LineEventCallback could be implemented in the user file
   */
}

/**
  * @brief  VSYNC event callback
  * @param  hdcmi: pointer to the DCMI handle 
  * @retval None
  */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_VsyncEventCallback();
}

/**
  * @brief  VSYNC Event callback.
  * @retval None
  */
__weak void BSP_CAMERA_VsyncEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_VsyncEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Frame event callback
  * @param  hdcmi: pointer to the DCMI handle  
  * @retval None
  */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_FrameEventCallback();
}

/**
  * @brief  Frame Event callback.
  * @retval None
  */
__weak void BSP_CAMERA_FrameEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_FrameEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Error callback
  * @param  hdcmi: pointer to the DCMI handle  
  * @retval None
  */
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_ErrorCallback();
}

/**
  * @brief  Error callback.
  * @retval None
  */
__weak void BSP_CAMERA_ErrorCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_ErrorCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
