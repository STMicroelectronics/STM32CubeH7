/**
  ******************************************************************************
  * @file    BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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


/* Private macro -------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
uint8_t DemoIndex = 0;
uint8_t UserButtonPressed = 0x00;
uint32_t LedToggleDelay = 0x00;
uint32_t ButtonState = 0;
/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static ShieldStatus TFT_ShieldDetect(void);
static void Display_DemoDescription(void);

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
  static char dataOut[64];
  COM_InitTypeDef COM_Init;
  
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Init HAL */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
 
  /* Initialize User_Button on STM32H7xx-Nucleo ------------------*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI); 

  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PB.01 pin (mapped to 
     JoyStick available on adafruit 1.8" TFT shield). If the state of PB.01 
     is high then the adafruit 1.8" TFT shield is available. */   
  if(TFT_ShieldDetect() == SHIELD_DETECTED)
  {
    /* Initialize the LCD */
    ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_PORTRAIT);    
    UTIL_LCD_SetFuncDriver(&LCD_Driver);
    Display_DemoDescription();
    
    while (1)
    {
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
  }  
  else /* there is no AdaFruit shield 802 connected */
  {
    /* Initialize Led1 on STM32H7xx-Nucleo_144 ------------------*/
    BSP_LED_Init(LED1); 
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
    
    /* Initialize COM port on STM32H7xx-Nucleo_144 ------------------*/
    COM_Init.BaudRate   = 115200;
    COM_Init.WordLength = COM_WORDLENGTH_8B;
    COM_Init.StopBits   = COM_STOPBITS_1;
    COM_Init.Parity     = COM_PARITY_NONE;
    COM_Init.HwFlowCtl  = COM_HWCONTROL_NONE;
    BSP_COM_Init(COM1, &COM_Init);
    
    sprintf(dataOut, "\r\n LED2 OFF\r\n Press Key button to put it ON\r\n ");
    printf("%s", dataOut);
    
    /* 0. Wait for User button to be pressed -------------------------------------*/
    while(CheckForUserInput() == 0)
    {}
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    
    /* Infinite loop */
    while (1)
    {
      while(CheckForUserInput() == 0)
      {}
      /* LED1 and LED3 On */
      BSP_LED_On(LED1);
      BSP_LED_On(LED3);
      
      sprintf(dataOut, "\r\n LED1 and LED3 ON\r\n Press Key button to put them OFF\r\n ");
      printf("%s", dataOut);
      ButtonState = 0;
      
      
      while(CheckForUserInput() == 0)
      {}
      /* LED1 and LED3 Off */
      BSP_LED_Off(LED1);
      BSP_LED_Off(LED3);
      
      sprintf(dataOut, "\r\n LED1 and LED3 OFF\r\n Press Key button to put them ON\r\n ");
      printf("%s", dataOut);
      ButtonState = 0;
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
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
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
  * @brief  Display main demo messages.
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  uint8_t desc[50];
  uint32_t x_size;
  uint32_t y_size;
  
  ADAFRUIT_802_LCD_GetXSize(0, &x_size);
  ADAFRUIT_802_LCD_GetYSize(0, &y_size);
  
  UTIL_LCD_SetFont(&Font8);
  
  /* Clear the LCD */ 
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE); 
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);  
  
  /* Display LCD messages */
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32H743ZI BSP", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 25, (uint8_t *)"Drivers examples", CENTER_MODE);
  
  /* Draw Bitmap */
  UTIL_LCD_DrawBitmap((x_size - 80)/2, 35, (uint8_t *)stlogo);
  
  UTIL_LCD_DisplayStringAt(0, y_size- 15, (uint8_t *)"Copyright (c) STM 2018", CENTER_MODE);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, y_size/2 + 15, x_size, 40, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE); 
  UTIL_LCD_DisplayStringAt(0, y_size/2 + 25, (uint8_t *)"Use User Button to start", CENTER_MODE);
  sprintf((char *)desc,"%s example", BSP_examples[DemoIndex].DemoName);
  UTIL_LCD_DisplayStringAt(0, y_size/2 + 40, (uint8_t *)desc, CENTER_MODE);   
}

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
  ADAFRUIT_802_ADCx_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin  = ADAFRUIT_802_ADCx_GPIO_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ADAFRUIT_802_ADCx_GPIO_PORT , &GPIO_InitStruct);
  HAL_Delay(1);
  if(HAL_GPIO_ReadPin(ADAFRUIT_802_ADCx_GPIO_PORT , ADAFRUIT_802_ADCx_GPIO_PIN ) != 0)
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
  if(GPIO_Pin==BUTTON_USER_PIN)
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

/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef  USE_FULL_ASSERT
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

