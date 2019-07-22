
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma2d.h"
#include "stm32h7xx_hal_ltdc.h"
#include "stm32h7xx_hal_jpeg.h"
#include "stm32h750b_discovery.h"

#include "hwjpeg_decode.h"

#include <stdbool.h>

#include <cmsis_os.h>

extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;
#if defined(USE_USB_FS) || defined(USE_USB_HS)
extern HCD_HandleTypeDef hhcd;
#endif /* USE_USB_FS | USE_USB_HS */

static volatile int overrunCnt;

extern struct HwJPEG_Context_s HwJPEG_Context;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void DebugMon_Handler(void)
{
}

void SysTick_Handler(void)
{
    osSystickHandler();
}

void DMA2D_IRQHandler(void)
{
    HAL_DMA2D_IRQHandler(&hdma2d);
}

void LTDC_IRQHandler(void)
{
    HAL_LTDC_IRQHandler(&hltdc);
}

void LTDC_ER_IRQHandler(void)
{
  if (LTDC->ISR & 2)
  {
      LTDC->ICR = 2;
      overrunCnt++;
  }
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

#if defined(USE_USB_FS) || defined(USE_USB_HS)
/**
  * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
  HAL_HCD_IRQHandler(&hhcd);
}
#endif /* USE_USB_FS | USE_USB_HS */
