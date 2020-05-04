


#include "stm32h7xx_it.h"

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma2d.h"
#include "stm32h7xx_hal_dsi.h"
#include "stm32h7xx_hal_ltdc.h"
#include "stm32h7xx_hal_jpeg.h"

#include <stdbool.h>

#include "hwjpeg_decode.h"

#include <cmsis_os.h>

#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_audio.h"
#include "stm32h743i_eval_sd.h"

#ifdef LCD_CLK_TUNING
extern void LCD_CLK_Tuning(void);
extern TIM_HandleTypeDef TimHandleLcd;
#endif /* LCD_CLK_TUNING */

extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef  hlcd_ltdc;
extern SD_HandleTypeDef    uSdHandle;

extern struct HwJPEG_Context_s HwJPEG_Context;

static volatile int overrunCnt;

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  SysTick interrupt handler
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  osSystickHandler();
}

/**
  * @brief  DMA2D interrupt handler
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&hdma2d);
}

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&hlcd_ltdc);
}

/**
  * @brief  LTDC_ER interrupt handler
  * @param  None
  * @retval None
  */
void LTDC_ER_IRQHandler(void)
{
  if (LTDC->ISR & 2)
  {
    LTDC->ICR = 2;
    overrunCnt++;
  }
}

void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MFX_IRQOUT_PIN);
}

void JPEG_IRQHandler(void)
{
  HAL_JPEG_IRQHandler(&HwJPEG_Context.hjpeg);
}

void MDMA_IRQHandler(void)
{
  HAL_MDMA_IRQHandler(HwJPEG_Context.hjpeg.hdmain);
  HAL_MDMA_IRQHandler(HwJPEG_Context.hjpeg.hdmaout);
}

/* SAI Audio OUT DMA2_Stream1 */
void DMA2_Stream1_IRQHandler(void)
{
  BSP_AUDIO_OUT_IRQHandler(0);
}

/**
 * @brief  This function handles SDMMC interrupt request.
 * @param  None
 * @retval None
 */
void SDMMC1_IRQHandler(void)
{
  BSP_SD_IRQHandler(0);
}

#ifdef LCD_CLK_TUNING
/**
  * @brief  This function handles TIM7 interrupt request.
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void)
{
  LCD_CLK_Tuning();
  __HAL_TIM_CLEAR_IT(&TimHandleLcd, TIM_IT_UPDATE);
}
#endif /* LCD_CLK_TUNING */
