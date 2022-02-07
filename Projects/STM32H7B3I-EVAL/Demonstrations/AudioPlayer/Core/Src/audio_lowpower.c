/**
  ******************************************************************************
  * @file    audio_lowpower.c
  * @author  MCD Application Team
  * @brief   Audio player low power functions
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
#include "audio_lowpower.h"
#include "audio_player_app.h"
#include "cmsis_os.h"

/** @addtogroup AUDIO_LP_MODULE
* @{
*/

/** @defgroup AUDIO_LP_APPLICATION
* @brief audio lp mode application routines
* @{
*/


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t LPmodeState = 0;
extern osMessageQId   LPModeEvent;
extern osThreadId     GUITaskId;
__IO uint16_t GUITaskResumed = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void SYSCLKConfig_EnterSTOP(void);
static void SYSCLKConfig_ExitSTOP(void);
static void Error_Handler(void);
static void AUDIOLP_WakeupEventConfig(void);
static void AUDIOLP_EnterLPmode(void);
static void LCD_DisplayOn(void);
static void LCD_DisplayOff(void);
/**
* @brief  Pre Sleep Processing
* @param  ulExpectedIdleTime: Expected time in idle state
* @retval None
*/
void PreSleepProcessing(uint32_t * ulExpectedIdleTime)
{
  /* Called by the kernel before it places the MCU into a sleep mode because
  configPRE_SLEEP_PROCESSING() is #defined to PreSleepProcessing().

  NOTE:  Additional actions can be taken here to get the power consumption
  even lower.  For example, peripherals can be turned off here, and then back
  on again in the post sleep processing function.  For maximum power saving
  ensure all unused pins are in their lowest power state. */



  /*
  (*ulExpectedIdleTime) is set to 0 to indicate that PreSleepProcessing contains
  its own wait for interrupt or wait for event instruction and so the kernel vPortSuppressTicksAndSleep
  function does not need to execute the wfi instruction
  */
  *ulExpectedIdleTime = 0;
  if(LPmodeState == 0x1 && AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
  {
    /*Disable TIM6 update Interrupt */
    HAL_SuspendTick();
    HAL_NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);

    /* Enter Low power mode */
    AUDIOLP_EnterLPmode();
    /* Configures system clock after wake-up from STOP: enable HSE, PLL and select
    PLL as system clock source (HSE and PLL are disabled in STOP mode) */
    HAL_ResumeTick();
  }
}

/**
* @brief  Post Sleep Processing
* @param  ulExpectedIdleTime: Not used
* @retval None
*/
void PostSleepProcessing(uint32_t * ulExpectedIdleTime)
{
  /* Called by the kernel when the MCU exits a sleep mode because
  configPOST_SLEEP_PROCESSING is #defined to PostSleepProcessing(). */

  /* Avoid compiler warnings about the unused parameter. */
  (void) ulExpectedIdleTime;
 }

/**
* @brief  Enter the system to STOP mode with main domain in DSTOP2
*         Autonomous mode activated for SRD/D3 peripherals,
*         config EXTI for wakeup event and SRD/D3 forced in DRun
* @param  None
* @retval None
*/
static void AUDIOLP_EnterLPmode(void)
{
  
  /* critics section */
  BACKUP_PRIMASK();
  __disable_irq();
  
  /* Clear Power flags */
  /* Check and handle if the system was resumed from Stop mode */
  if(__HAL_PWR_GET_FLAG(PWR_CPU_FLAGS) != RESET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_CPU_FLAGS);
  }
  
  /* Clear BDMA2 pending bit */
  HAL_NVIC_ClearPendingIRQ(BDMA2_Channel0_IRQn);
  
  /* Configure Periphericals in Autonomous mode in SRD/D3 Domain mode  */
  /* Activate Autonomous mode bit for Periphericals: D3SRAM1, BDMA2, SPI6 */
  __HAL_RCC_SPI6_CLKAM_ENABLE();
  __HAL_RCC_BDMA2_CLKAM_ENABLE();
  __HAL_RCC_SRDSRAM_CLKAM_ENABLE();
  
  /* Configure the D3 Domain to stay in RUN mode regardless of the CPU sub-system mode. */
  HAL_PWREx_ConfigD3Domain(PWR_D3_DOMAIN_RUN);
  
  /* configure Memories: Disable AXI MEMORY block1 shut-off in DStop2 modes */
  HAL_PWREx_DisableMemoryShutOff(PWR_RAM1_AXI_MEMORY_BLOCK);
  
  /* Disable SRD AHB MEMORY block shut-off in DStop2 modes */
  HAL_PWREx_DisableMemoryShutOff(PWR_SRD_AHB_MEMORY_BLOCK);
  
  /* Enter the system to STOP mode with main domain in DSTOP2 */
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_STOP) == RESET)
  {
    HAL_PWREx_EnterSTOP2Mode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
  }
  
  __enable_irq();
  RESTORE_PRIMASK();
  
}

/**
* @brief  Configure the EXTI input event line for CPU wakeup 
* @param  None
* @retval None
*/
static void AUDIOLP_WakeupEventConfig(void)
{
  uint32_t exti_line = 0x0;
  
  /* Disable EXTI input event line for Domain D1 */
  for(exti_line = EXTI_LINE0; exti_line < EXTI_LINE88; exti_line++)
  {
    HAL_EXTI_D1_EventInputConfig(exti_line, EXTI_MODE_IT, DISABLE);
    HAL_EXTI_D1_ClearFlag(exti_line);
  }
  
  /* Config BDMA2 wakeup EXTI event(66) for CPU wakeup (in D1 domain) */
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE66, EXTI_MODE_IT, ENABLE);

  /* Config micro SD wakeup EXTI event(10) for CPU wakeup (in D1 domain) */
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE10, EXTI_MODE_IT, ENABLE);

 /* Config User button wakeup EXTI event(0) for CPU wakeup (in D1 domain) */
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE0, EXTI_MODE_IT, ENABLE);
  
}

/**
* @brief  Low Power mode  task
* @param  argument: pointer that is passed to the thread function as start argument.
* @retval None
*/
void AUDIOLPModeThread(void const * argument)
{
  osEvent event;

  for(;;)
  {
    event = osMessageGet(LPModeEvent, osWaitForever );
    if(event.status == osEventMessage)
    {
      switch(event.value.v)
      {
      case AUDIOLP_ENTER_LPMODE:
        /* enter to LP Mode */
        osThreadSuspend(GUITaskId);
        /* Switch OFF  the display */
        LCD_DisplayOff();
        AUDIOLP_WakeupEventConfig();
        SYSCLKConfig_EnterSTOP();
        LPmodeState = 0x1;
        break;

      case AUDIOLP_EXIT_LPMODE:
        /* exit from LP Mode */
        GUITaskResumed = 0x1;
        SYSCLKConfig_ExitSTOP();
        osThreadResume(GUITaskId);
        /* Switch ON  the display */
        osDelay(500);
        LCD_DisplayOn();
        break;

      default:
        break;
      }
    }

  }
}

/**
  * @brief  Set system clock to 280 Mhz after exit from low power mode
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_ExitSTOP(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  uint32_t FLatency;

  /* Change SYSCK source to HSE */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &FLatency);
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSE;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
 
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLP = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Set  SYSCK source to PLL1*/
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &FLatency);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  Set system clock to 84 Mhz before entering DSTOP2 mode to reduce 
  *         power consumption
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_EnterSTOP(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  uint32_t FLatency;

  /** Change SYSCK source to HSE */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &FLatency);
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSE;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 24;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLP = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_0;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* set SYSCK source to PLL1*/
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &FLatency);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);   /*by default*/

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if (Button == BUTTON_WAKEUP)
  {
    if (LPmodeState == 0x1)
    {
      osMessagePut ( LPModeEvent, AUDIOLP_EXIT_LPMODE, 0);
      LPmodeState = 0x0;
    }
  }
}

/**
 * @brief  Notify end of audio file.
 * @param  None
 * @retval Audio error
*/
AUDIO_ErrorTypeDef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  AUDIO_ErrorTypeDef ret = AUDIO_ERROR_NONE;
  
  if(LPmodeState == 0x1 && AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
  {
    if(++FileList.FileIndex >= FileList.ptr)
    {
      FileList.FileIndex = 0;
    }
    AUDIOPLAYER_Stop();
    ret = AUDIOPLAYER_Play(FileList.file[FileList.FileIndex].full_name, AUDIOPLAYER_GetVolume()); 
  }
  
  return ret;
}

/**
  * @brief  Switch On the display.
  * @param  none
  * @retval BSP status
  */
static void LCD_DisplayOn(void)
{
  __HAL_LTDC_ENABLE(&hlcd_ltdc);
  /* Assert LCD_DISP_EN pin */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_SET);
  /* Assert LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);
}

/**
* @brief  Switch Off the display.
* @param  none
* @retval BSP status
*/
static void LCD_DisplayOff(void)
{
  __HAL_LTDC_DISABLE(&hlcd_ltdc);
  /* Assert LCD_DISP_EN pin */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_RESET);
  /* Assert LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_RESET);
}


/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{

}

/**
* @}
*/

/**
* @}
*/

