/**
  ******************************************************************************
  * @file    Demonstration/STM32H745-Discovery_Demo/CM7/Src/display.c
  * @author  MCD Application Team
  * @brief   Signals Generator , display module file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "display_menu.h"
    
/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup MainMenu
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/ 
extern LTDC_HandleTypeDef hlcd_ltdc;
DMA2D_HandleTypeDef       DMA2D_Handle;
__IO uint32_t pending_buffer = 0;

/* Private function prototypes -----------------------------------------------*/
void LCD_BlendImage(uint32_t *pSrc, uint32_t *pDst, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorMode, uint32_t InputOffset);
void DisplayButton(ButtonWidgetTypeDef *pButton, uint32_t *pDst);
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the Display
  * @param  None
  * @retval None
  */
void InitMenuDisplay(void)
{  
  /* Initialize the LCD */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  
  
  BSP_LCD_SetBrightness(0, 100);
  BSP_LCD_DisplayOn(0);
  
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  /* Set LCD Font  */
  UTIL_LCD_SetFont(&Font16);
  
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  /* Enable IRQ */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0x0F, 0x0F); 
  HAL_NVIC_EnableIRQ(LTDC_IRQn); 	
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0x0F, 0x0F);  
  HAL_NVIC_EnableIRQ(DMA2D_IRQn); 	
}



/**
  * @brief  Display button on the screen
  * @param  None
  * @retval None
  */
void DisplayButton(ButtonWidgetTypeDef *pButton, uint32_t *pDst)
{   
  uint32_t inputOffset;
  uint32_t X_size;
  
  BSP_LCD_GetXSize(0, &X_size);
  inputOffset =  X_size - pButton->buttonParams.Width;
  
  /* Blend switch buttons image on the Screen */
  LCD_BlendImage((uint32_t*)pButton->FrameBuffer, pDst, pButton->buttonParams.Xpos, pButton->buttonParams.Ypos, pButton->buttonParams.Width, pButton->buttonParams.Height, pButton->buttonParams.ColorMode, inputOffset);
}



void DisplayGenericButtons(ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber)
{
  uint8_t buttonIndex;
  
  /* Display the waves buttons */
  for(buttonIndex = 0; buttonIndex < buttonsNumber; buttonIndex++)
  {
    DisplayButton(&pButton[buttonIndex],(uint32_t *)LCD_FRAME_BUFFER); 
  }

} 





/**
  * @brief  Display the background image on the screen
  * @param  None
  * @retval None
  */
void DisplayMenuImages(BackgroundWidgetTypeDef *pImage,uint32_t buttonsNumber)
{
  uint32_t i;
  uint32_t inputOffset;
  uint32_t X_size;
  uint32_t Y_size;
  
  BSP_LCD_GetXSize(0, &X_size);
  BSP_LCD_GetYSize(0, &Y_size);
  
  pending_buffer = 1;
  /* Set LTDC Line Event  */
  HAL_LTDC_ProgramLineEvent(&hlcd_ltdc, Y_size/2);  
  while(pending_buffer != 0)
  {
  }

  /* Display the waves buttons */
  for(i = 0; i < buttonsNumber; i++)
  {
      inputOffset =  X_size - pImage[i].imageParams.Width;
    /* Copy background image on the Screen */
      LCD_BlendImage((uint32_t*)(pImage[i].FrameBuffer), (uint32_t *)LCD_FRAME_BUFFER, pImage[i].imageParams.Xpos, pImage[i].imageParams.Ypos, pImage[i].imageParams.Width, pImage[i].imageParams.Height, pImage[i].imageParams.ColorMode, inputOffset);

  }
 
}

/* Private functions ---------------------------------------------------------*/

void LCD_BlendImage(uint32_t *pSrc, uint32_t *pDst, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorMode, uint32_t InputOffset)
{
  uint32_t destination;
  uint32_t X_size;
  
  BSP_LCD_GetXSize(0, &X_size);
  
  pending_buffer = 1;
  /* Set LTDC Line Event  */
  HAL_LTDC_ProgramLineEvent(&hlcd_ltdc, 0);  
  while(pending_buffer != 0) {}

  /* Init DMA2D */
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_BLEND;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = InputOffset;

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[1].InputColorMode = ColorMode;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;

  /*##-3- Background Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  DMA2D_Handle.LayerCfg[0].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
  DMA2D_Handle.LayerCfg[0].InputOffset = InputOffset;

  DMA2D_Handle.Instance          = DMA2D; 

  /*##-4- DMA2D Initialization     ###########################################*/
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);     
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 0); 

  /*##-1- calculate the destination transfer address  ############*/ 
  destination = (uint32_t)pDst + (Ypos * X_size + Xpos) * 4;

  HAL_DMA2D_BlendingStart(&DMA2D_Handle, (uint32_t)pSrc, destination, destination, Width, Height);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);
}

void LCD_CopyImage(uint32_t *pSrc,uint32_t *pDst, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorMode, uint32_t InputOffset)
{
  uint32_t destination;
  uint32_t X_size;
  
  BSP_LCD_GetXSize(0, &X_size);
  pending_buffer = 1;
  /* Set LTDC Line Event  */
  HAL_LTDC_ProgramLineEvent(&hlcd_ltdc, 0);  
  while(pending_buffer != 0) {}
  
  
  /* Init DMA2D */
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = X_size - Width;     
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[1].InputColorMode = ColorMode;
  DMA2D_Handle.LayerCfg[1].InputOffset = InputOffset;
  
  DMA2D_Handle.Instance          = DMA2D; 
  
  /*##-4- DMA2D Initialization     ###########################################*/
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);     
  
  /*##-1- calculate the destination transfer address  ############*/ 
  
  destination = (uint32_t)pDst + (Ypos * X_size + Xpos) * 4;
  
  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, Width, Height);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);
}

void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
  pending_buffer = 0;    
}

/**
  * @}
  */

/**
  * @}
  */

