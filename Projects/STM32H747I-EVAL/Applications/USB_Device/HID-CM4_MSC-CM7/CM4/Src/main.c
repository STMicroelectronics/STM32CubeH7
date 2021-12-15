 /**
  ******************************************************************************
  * @file    USB_Device/HID-CM4_MSC-CM7/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   the main CM4 application program
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

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Applications
  * @{
  */

/** @addtogroup HID_Standalone
  * @{
  */

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
USBD_HandleTypeDef USBD_Device;
extern PCD_HandleTypeDef hpcd;

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
/*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));


  HAL_Init();

  /* When system initialization is finished, Cortex-M7 will release (wakeup) Cortex-M4  by means of
     HSEM notification. Cortex-M4 release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).
  */

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_ORANGE);

  /* Configure Key button for remote wakeup */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_EXTI);

    /* Configure Key button for remote wakeup */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  
  HAL_PWREx_EnableUSBVoltageDetector();

  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);

  /* Start Device Process */
  USBD_Start(&USBD_Device);

  BSP_LED_On(LED_ORANGE);

  while (1)
  {
  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
   * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
   * * line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

