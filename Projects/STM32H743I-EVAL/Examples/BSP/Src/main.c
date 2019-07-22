/**
  ******************************************************************************
  * @file    BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM32H743I_EVAL BSP Drivers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stlogo.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
__IO uint8_t NbLoop = 1;

/* Wave Player Pause/Resume Status. Defined as external in waveplayer.c file */
__IO uint32_t PauseResumeStatus = IDLE_STATUS;

/* Counter for Sel Joystick pressed*/
__IO uint32_t PressCount = 0;

/* Joystick Status */
static JOYState_TypeDef JoyState = JOY_NONE;

uint8_t mfx_toggle_led = 0;

/* Volume of the audio playback */
/* Initial volume level (from 0% (Mute) to 100% (Max)) */
__IO uint8_t volume = 60;
__IO uint8_t VolumeChange = 0;

__IO uint32_t SRAMTest = 0;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_DemoDescription(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

BSP_DemoTypedef  BSP_examples[]=
{
  {Joystick_demo, "JOYSTICK", 0},
  {Touchscreen_demo, "TOUCHSCREEN", 0},
  {LCD_demo, "LCD", 0},
  {POTENTIOMETER_demo, "POTENTIOMETER", 0},
  {SD_demo, "SD", 0},
  {AudioPlay_demo, "AUDIO PLAY", 0},
  {AudioRecord_demo, "AUDIO RECORD", 0},
  {QSPI_demo, "QSPI", 0},
  {EEPROM_demo, "EEPROM", 0},
  {NOR_demo, "NOR", 0},
  {SRAM_demo, "SRAM", 0},
  {SRAM_DMA_demo, "SRAM MDMA", 0},
  {SDRAM_demo, "SDRAM", 0},
  {SDRAM_DMA_demo, "SDRAM MDMA", 0},
  {Log_demo, "LCD LOG", 0},
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the MPU attributes as Write Through */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* Configure the Tamper push-button in GPIO Mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init();

  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);

  Display_DemoDescription();

  /* Wait For User inputs */
  while (1)
  {
    /* Toggle LED1 before start test */
    HAL_Delay(100);
    BSP_LED_Toggle(LED_GREEN);

    if(BSP_PB_GetState(BUTTON_TAMPER) == RESET)
    {
      BSP_LED_Off(LED_GREEN);
      HAL_Delay(10);
      while (BSP_PB_GetState(BUTTON_TAMPER) == RESET);

      BSP_examples[DemoIndex++].DemoFunc();

      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        NbLoop++;
        DemoIndex = 0;
      }
      Display_DemoDescription();
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /*activate CSI clock mondatory for I/O Compensation Cell*/
  __HAL_RCC_CSI_ENABLE() ;

  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  /* Enables the I/O Compensation Cell */
  HAL_EnableCompensationCell();
}

/**
  * @brief  Display main demo messages
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  char desc[50];

  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(1);

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32H743I BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Drivers examples", CENTER_MODE);

  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)stlogo);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2017", CENTER_MODE);

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 30, (uint8_t *)"Press Tamper push-button to start :", CENTER_MODE);
  sprintf(desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)desc, CENTER_MODE);
}

/**
  * @brief  Check for user input
  * @param  None
* @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_TAMPER) == RESET)
  {
    HAL_Delay(10);
    while (BSP_PB_GetState(BUTTON_TAMPER) == RESET);
    return 1 ;
  }
  return 0;
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* If the interruption comes from the Joystick, check which
     Joystick button has been pressed */
  JoyState = BSP_JOY_GetState();

  if(JoyState == JOY_SEL)
  {
    /* SEL is used to pause and resume the audio playback */
    if (PressCount == 1)
    {
      /* Resume playing Wave status */
      PauseResumeStatus = RESUME_STATUS;
      PressCount = 0;
    }
    else
    {
      /* Pause playing Wave status */
      PauseResumeStatus = PAUSE_STATUS;
      PressCount = 1;
    }
  }
  else if(JoyState == JOY_UP)
  {
    /* UP is used to increment the volume of the audio playback */
    volume ++;
    if (volume > 100)
    {
      volume = 100;
    }
    VolumeChange = 1;
  }
  else if(JoyState == JOY_DOWN)
  {
    /* DOWN is used to decrement the volume of the audio playback */
    volume --;
    if ((int8_t)volume < 50)
    {
      volume = 50;
    }
    VolumeChange = 1;
  }

  /* Clear MFX IT */
  BSP_IO_ITClear();
}

/**
  * @brief Toggle Leds
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
  static uint8_t ticks = 0;

  if(ticks++ > 100)
  {
    mfx_toggle_led = 1;   /* Toggle LED performed in main loop */
    ticks = 0;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED REDon */
  BSP_LED_On(LED_RED);
  while(1)
  {
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/**
  * @brief  Configure the MPU attributes as Write Through for SDRAM.
  * @note   The Base Address is SDRAM_DEVICE_ADDR.
  *         The Region Size is 32MB.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* Configure the MPU attributes as WT for NOR */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = NOR_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;  

  HAL_MPU_ConfigRegion(&MPU_InitStruct);  

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
