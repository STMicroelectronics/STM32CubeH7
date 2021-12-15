/**
  ******************************************************************************
  * @file    Demonstration/Oscilloscope_SignalsGenerator/Src/display.c
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
#include "display.h"
    
/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
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
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the Display
  * @param  None
  * @retval None
  */
void SG_InitDisplay(void)
{  
  /* Initialize the LCD */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  
  /* Set LCD Font  */
  UTIL_LCD_SetFont(&Font16);
  
  /* Clear the LCD */
  
  UTIL_LCD_Clear(0xFF817DAE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  /* Enable IRQ */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0x0F, 0x0F); 
  HAL_NVIC_EnableIRQ(LTDC_IRQn); 	
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0x0F, 0x0F);  
  HAL_NVIC_EnableIRQ(DMA2D_IRQn); 	
}

/**
  * @brief  Display a switch button on the screen
  * @param  None
  * @retval None
  */
void SG_DisplaySwitchButton(SG_SwitchButtonWidgetTypeDef *pButton,uint32_t *pDst, uint8_t ButtonState)
{  
  uint32_t *pSrc;
  uint32_t inputOffset;
  uint32_t X_size;
  
  BSP_LCD_GetXSize(0, &X_size);

  inputOffset =  X_size - pButton->buttonParams.Width;

  pSrc = (uint32_t *)((ButtonState == BUTTON_STATE_OFF) ? pButton->Off_FrameBuffer : pButton->On_FrameBuffer);
  
  /* Blend switch buttons image on the Screen */
  LCD_BlendImage(pSrc, pDst, pButton->buttonParams.Xpos, pButton->buttonParams.Ypos, pButton->buttonParams.Width, pButton->buttonParams.Height, pButton->buttonParams.ColorMode, inputOffset);

}

/**
  * @brief  Display button on the screen
  * @param  None
  * @retval None
  */
void SG_DisplayButton(SG_ButtonWidgetTypeDef *pButton, uint32_t *pDst)
{   
  uint32_t inputOffset;
  uint32_t X_size;
  
  BSP_LCD_GetXSize(0, &X_size);
  inputOffset =  X_size - pButton->buttonParams.Width;
  
  /* Blend switch buttons image on the Screen */
  LCD_BlendImage((uint32_t*)pButton->FrameBuffer, pDst, pButton->buttonParams.Xpos, pButton->buttonParams.Ypos, pButton->buttonParams.Width, pButton->buttonParams.Height, pButton->buttonParams.ColorMode, inputOffset);
}



void SG_DisplayWaveButtons(SG_SwitchButtonWidgetTypeDef *pButton, uint32_t buttonsNumber)
{
  uint8_t buttonIndex;
  uint32_t Y_size;
  uint32_t buttonYPosition;
  
  BSP_LCD_GetYSize(0, &Y_size);
  
  /* Set the buttons Y positions */
  buttonYPosition = (Y_size /4);
  
  /* Display the waves buttons */
  for(buttonIndex = 0; buttonIndex < buttonsNumber; buttonIndex++)
  {
    if((buttonIndex == 0) || (buttonIndex == 2) || (buttonIndex == 4))
    {
      pButton[buttonIndex].buttonParams.Xpos = SG_WAVE_BUTTON_XPOSITION;
      pButton[buttonIndex].buttonParams.Ypos = (buttonYPosition + ((pButton[buttonIndex].buttonParams.Height + pButton[buttonIndex].buttonParams.Height /2) * buttonIndex/2));
      pButton[buttonIndex].State = BUTTON_STATE_OFF;
    }
    else
    {
      pButton[buttonIndex].buttonParams.Xpos = SG_WAVE_BUTTON_XPOSITION*2 + pButton[buttonIndex].buttonParams.Width;
      pButton[buttonIndex].buttonParams.Ypos = (buttonYPosition + ((pButton[buttonIndex].buttonParams.Height + pButton[buttonIndex].buttonParams.Height /2) * (buttonIndex -1)/2));
      pButton[buttonIndex].State = BUTTON_STATE_OFF;
    }
    SG_DisplaySwitchButton(&pButton[buttonIndex],(uint32_t *)LCD_LAYER_0_ADDRESS, pButton[buttonIndex].State); 
  }
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_LIGHTGRAY);

  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAt(SG_WAVE_BUTTON_XPOSITION-2, 45, (uint8_t*)" Signal Type ", LEFT_MODE);
  UTIL_LCD_DrawRect(SG_WAVE_BUTTON_XPOSITION-4, 41, pButton[1].buttonParams.Xpos + 70, 160,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(SG_WAVE_BUTTON_XPOSITION-3, 42, pButton[1].buttonParams.Xpos + 68, 158,UTIL_LCD_COLOR_BLUE);

  UTIL_LCD_DisplayStringAt(SG_WAVE_BUTTON_XPOSITION+160, 45, (uint8_t*)" Signal Params ", LEFT_MODE);
  UTIL_LCD_DrawRect(SG_WAVE_BUTTON_XPOSITION+150, 41, 190, 160,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(SG_WAVE_BUTTON_XPOSITION+150+1, 42, 188, 158,UTIL_LCD_COLOR_BLUE);
  
  UTIL_LCD_DisplayStringAt(SG_WAVE_BUTTON_XPOSITION+360, 45,(uint8_t*) "  Status ", LEFT_MODE);
  UTIL_LCD_DrawRect(SG_WAVE_BUTTON_XPOSITION+350, 41, 120, 160,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(SG_WAVE_BUTTON_XPOSITION+351, 42, 118, 158,UTIL_LCD_COLOR_BLUE);
}   

/**
  * @brief  Display up down buttons on the screen
  * @param  None
  * @retval None
  */
void SG_DisplayUpDownButtons(SG_ButtonWidgetTypeDef *pButton, uint32_t buttonsNumber)
{
  uint8_t buttonIndex;
  
  /* Display the waves buttons */
  for(buttonIndex = 0; buttonIndex < buttonsNumber; buttonIndex++)
  {
    SG_DisplayButton(&pButton[buttonIndex], (uint32_t *)LCD_LAYER_0_ADDRESS); 
  }
}   

/**
  * @brief  Display frequency icon on the screen
  * @param  None
  * @retval None
  */
void SG_DisplayFrequencyIcon(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint8_t *Frequency)
{
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGRAY); 
  UTIL_LCD_FillRect(Xpos, Ypos, Width, Height,UTIL_LCD_COLOR_DARKGRAY);  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_DARKGRAY);
  UTIL_LCD_DisplayStringAt(Xpos + Width/4-6, Ypos+5, (uint8_t*)"FREQUENCY", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(Xpos+ Width/4-10, Ypos+Height/2+4, Frequency, LEFT_MODE);
  
}   

/**
  * @brief  Display amplitude icon on the screen
  * @param  None
  * @retval None
  */
void SG_DisplayAmplitudeIcon(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint8_t *Amplitude)
{
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGRAY); 
  UTIL_LCD_FillRect(Xpos, Ypos, Width, Height,UTIL_LCD_COLOR_DARKGRAY);  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_DARKGRAY);
  UTIL_LCD_DisplayStringAt(Xpos + Width/4-6, Ypos+5,(uint8_t*) "AMPLITUDE", LEFT_MODE);
  UTIL_LCD_DisplayStringAt(Xpos+ Width/4+10, Ypos+Height/2+4, Amplitude, LEFT_MODE);
}   

/**
  * @brief  Switch a button state on the screen
  * @param  None
  * @retval None
  */
void SG_SwitchWaveButtonState(SG_SwitchButtonWidgetTypeDef *pButton, uint32_t buttonIndex, uint32_t buttonsNumber)
{
  uint32_t i;
  
  /* Display the waves buttons */
  for(i = 0; i < buttonsNumber; i++)
  {
    if(pButton[i].ButtonIndex != buttonIndex)
    {
      pButton[i].State = BUTTON_STATE_OFF;
      SG_DisplaySwitchButton(&pButton[i],(uint32_t *)LCD_LAYER_0_ADDRESS, pButton[i].State); 
    }
    else
    {
      (pButton[i].State == BUTTON_STATE_ON) ? (pButton[i].State = BUTTON_STATE_OFF) : (pButton[i].State = BUTTON_STATE_ON);
      SG_DisplaySwitchButton(&pButton[i],(uint32_t *)LCD_LAYER_0_ADDRESS, pButton[i].State); 
    }  
  }
}

/**
  * @brief  Display the background image on the screen
  * @param  None
  * @retval None
  */
void SG_DisplayBackgroundImage(SG_BackgroundWidgetTypeDef *pImage)
{
  uint32_t Y_size;
  pending_buffer = 1;
  /* Set LTDC Line Event  */
  BSP_LCD_GetYSize(0, &Y_size);
  HAL_LTDC_ProgramLineEvent(&hlcd_ltdc, Y_size/2);  
  while(pending_buffer != 0)
  {
  }
  
  /* Copy background image on the Screen */
  LCD_CopyImage((uint32_t*)pImage->FrameBuffer,(uint32_t *)LCD_LAYER_0_ADDRESS, pImage->imageParams.Xpos, pImage->imageParams.Ypos, pImage->imageParams.Width, pImage->imageParams.Height, pImage->imageParams.ColorMode, 0);
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

