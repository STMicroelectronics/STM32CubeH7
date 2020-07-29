
#include "stm32h7xx_hal.h"
#include "stm32h735g_discovery.h"
#if defined(USE_SDCARD)
#include "stm32h735g_discovery_sd.h"
#endif /* USE_SDCARD */

#include <stdbool.h>
#include <cmsis_os.h>

extern OSPI_HandleTypeDef hospi_nor[];
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;
#if defined(USE_SDCARD)
extern SD_HandleTypeDef   hsd_sdmmc[];
#endif /* USE_SDCARD */
#if defined(USE_USB_FS) || defined(USE_USB_HS)
extern HCD_HandleTypeDef hhcd;
#endif /* USE_USB_FS | USE_USB_HS */


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

/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

#if defined(USE_SDCARD)
/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  BSP_SD_DETECT_IRQHandler(0);
}
#endif /* USE_SDCARD */

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  BSP_PB_IRQHandler(BUTTON_USER);
}

/**
  * @brief  This function handles OctoSPI interrupt request.
  * @param  None
  * @retval None
  */
void OCTOSPI1_IRQHandler(void)
{
  HAL_OSPI_IRQHandler(&hospi_nor[0]);
}

void DMA2D_IRQHandler(void)
{
    HAL_DMA2D_IRQHandler(&hdma2d);
}

void LTDC_IRQHandler(void)
{
    HAL_LTDC_IRQHandler(&hltdc);
}

#if defined(USE_SDCARD)
/**
 * @brief  This function handles SDMMC interrupt request.
 * @param  None
 * @retval None
 */
void SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&hsd_sdmmc[0]);
}
#endif /* USE_SDCARD */

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
