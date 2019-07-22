/**
  ******************************************************************************
  * @file    BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
typedef enum
{
  SHIELD_NOT_DETECTED = 0,
  SHIELD_DETECTED
}ShieldStatus;

/* Private define -------------------------------------------------------------*/
#define BUTTON_USER BUTTON_KEY

/* Private macro -------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
uint8_t DemoIndex = 0;
uint8_t UserButtonPressed = 0x00;
uint32_t LedToggleDelay = 0x00;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static ShieldStatus TFT_ShieldDetect(void);
#ifdef ADAFRUIT_TFT_JOY_SD_ID802
static void Display_DemoDescription(void);
#endif /* ADAFRUIT_TFT_JOY_SD_ID802 */

static void CPU_CACHE_Enable(void);

BSP_DemoTypedef  BSP_examples[]=
{
  {Joystick_demo, "JOYSTICK", 0},
  {LCD_demo, "LCD", 0},
  {SD_demo, "mSD", 0},
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @retval None
  */
int main(void)
{
  uint8_t count = 0;

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Init HAL */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize User_Button on STM32H7xx-Nucleo ------------------*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PF.03 pin (mapped to JoyStick
     available on adafruit 1.8" TFT shield). If the state of PF.03 is high then
     the adafruit 1.8" TFT shield is available. */
  if(TFT_ShieldDetect() == SHIELD_DETECTED)
  {
#if defined(ADAFRUIT_TFT_JOY_SD_ID802)
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Initialize the LCD */
    BSP_LCD_Init();

    Display_DemoDescription();

    while (1)
    {
      BSP_LED_Toggle(LED2);
      HAL_Delay(100);
      if(BSP_PB_GetState(BUTTON_USER) != RESET)
      {
        HAL_Delay(10);
        while (BSP_PB_GetState(BUTTON_USER) != RESET);

        BSP_examples[DemoIndex++].DemoFunc();

        if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
        {
          DemoIndex = 0;
        }
        Display_DemoDescription();
      }
    }
#endif /* ADAFRUIT_TFT_JOY_SD_ID802 */
  }
  else /* there is no AdaFruit shield 802 connected */
  {
    /* Initialize Led1 on STM32H7xx-Nucleo ------------------*/
    BSP_LED_Init(LED1);  /* this is in conflict with Adafruit shield */
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* 0. Wait for User button to be pressed -------------------------------------*/
    while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
    {
      /* Toggle LED2 */
      BSP_LED_Toggle(LED2);
      HAL_Delay(200);
    }
    /* Wait for User button is released */
    while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
    {}

    BSP_LED_Off(LED2);

    /* Infinite loop */
    while (1)
    {
      if(UserButtonPressed==0x01)
      {
        count++;
        LedToggleDelay += 20*count;
        UserButtonPressed = 0x00;
        if(LedToggleDelay>500)
        {
          LedToggleDelay = 20;
          count = 0;
        }
      }
      /* Toggle LED1 & LED3 */
      BSP_LED_Toggle(LED1);
      HAL_Delay(LedToggleDelay);
      BSP_LED_Toggle(LED3);
      HAL_Delay(LedToggleDelay);
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
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

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
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


#if defined(ADAFRUIT_TFT_JOY_SD_ID802)
/**
  * @brief  Display main demo messages.
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  uint8_t desc[50];

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32H743ZI BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 25, (uint8_t *)"Drivers examples", CENTER_MODE);

  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 35, (uint8_t *)stlogo);

  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 15, (uint8_t *)"Copyright (c) STM 2017", CENTER_MODE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 40);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 25, (uint8_t *)"Use User Button to start", CENTER_MODE);
  sprintf((char *)desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 40, (uint8_t *)desc, CENTER_MODE);
}
#endif /* ADAFRUIT_TFT_JOY_SD_ID802 */

/**
  * @brief  Check for user input.
  * @param  None
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_USER) != RESET)
  {
    HAL_Delay(10);
    while (BSP_PB_GetState(BUTTON_USER) != RESET);
    return 1 ;
  }
  return 0;
}

/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO PF.03 pin (mapped to
  *         JoyStick available on adafruit 1.8" TFT shield). If the state of PF.03
  *         is high then the adafruit 1.8" TFT shield is available.
  * @param  None
  * @retval SHIELD_DETECTED: 1.8" TFT shield is available
  *         SHIELD_NOT_DETECTED: 1.8" TFT shield is not available
  */
static ShieldStatus TFT_ShieldDetect(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable GPIO clock */
  NUCLEO_ADCx_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = NUCLEO_ADCx_GPIO_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(NUCLEO_ADCx_GPIO_PORT , &GPIO_InitStruct);

  HAL_Delay(1);

  if(HAL_GPIO_ReadPin(NUCLEO_ADCx_GPIO_PORT , NUCLEO_ADCx_GPIO_PIN ) != 0)
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==USER_BUTTON_PIN)
  {
    UserButtonPressed = 0x01;
  }
}

/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    /* red LED stays ON*/
    BSP_LED_On(LED2);
  }
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
  * @}
  */
  
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
