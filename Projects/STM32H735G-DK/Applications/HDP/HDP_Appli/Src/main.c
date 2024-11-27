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
#include "RSS.h"
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup HDP_Boot
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define sec_bit_address   ((uint32_t) 0x5200201c)
#define sec_area_address  ((uint32_t) 0x52002030)
#define rdp_bits_address  ((uint32_t) 0x5200201C)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t security_register = 0;
uint8_t security_bit = 0;
uint8_t StepCounter = 0;
__IO uint32_t ButtonState = 0;
uint8_t security_area_set = 0;
uint8_t rdp_level = 0;
uint32_t x_size;
uint32_t y_size;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void Error_Handler(void);
static void HDP_Appli_Config(void);
static void Display_background (void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This project template calls firstly two functions in order to configure MPU feature
     and to enable the CPU Cache, respectively MPU_Config() and CPU_CACHE_Enable().
     These functions are provided as template implementation that User may integrate
     in his application, to enhance the performance in case of use of AXI interface
     with several masters. */

  /* Configure the MPU attributes as Write Through for OctoSPI RAM*/
  MPU_Config();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);

  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  /* Configure USER Button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
  Display_background();
  
/* Configuration of Security area and RDP level */
   HDP_Appli_Config();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 104
  *            PLL_P                          = 1
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 104;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
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
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

/*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, OSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
*/


  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();

}

/**
  * @brief  Display main demo messages
  * @retval None
  */
static void Display_background(void)
{

  UTIL_LCD_SetFont(&Font12);

  /* Clear the LCD */
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  UTIL_LCD_DisplayStringAt(0, 5, (uint8_t *)"STM32H735G-DK ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 15, (uint8_t *)"HDP Application", CENTER_MODE);

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Draw Bitmap */
  
  /* Draw Bitmap, ST logo */
  UTIL_LCD_DrawBitmap(x_size/2-40, 30 , (uint8_t *)stlogo);
  UTIL_LCD_SetFont(&Font8);
  UTIL_LCD_DisplayStringAt(0, y_size - 10, (uint8_t *)"Copyright (c) STMicroelectronics 2020", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  BSP_LCD_FillRect(0, 0, 85, x_size, y_size - (80+20), UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
}

/**
* @brief  HDP_Appli_Config function :Displaying the LCD Configuration of secure area and RDP level set
* @retval None
*/
static void HDP_Appli_Config(void)
{
  /* Display the LCD screen Background */
  Display_background ();
  ButtonState = 0;
  /* Display LCD messages */
  UTIL_LCD_DisplayStringAt(0, 90 , (uint8_t *)"The HDP_Appli is loaded at 0x08040000", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 105 , (uint8_t *)"The yellow LED is ON", CENTER_MODE);
  BSP_LED_Toggle(LED1);    /* Switch on the yellow led */
  HAL_Delay(3000); 
  UTIL_LCD_DisplayStringAt(0, y_size / 2 - 15  , (uint8_t *)"Press the USER button to continue", CENTER_MODE);
  HAL_Delay(1000);

  /* readout of the security user option bit status */
  security_bit = (*((uint32_t *)(sec_bit_address)) >> 21) & 1;

    /* check if security area is set  
     secure area is set if: secure area start address < secure area end address
     For STM32H7B3 one secure area can be defined for each flash bank
     In this example a secure area will be set in the first bank where the first application is running
     See RM0455 flash register for more details
  */
  if ((*((uint32_t *)(sec_area_address)) & 0x0FFF0000) > ((*((uint32_t *)(sec_area_address)) & 0x00000FFF)<< 16))
    {
      security_area_set = 1;
    }
  else
    {
      security_area_set = 0;
    }

    /* check the RDP readout protection level, set through user option bits 
     AA: Level 0, CC: Level 2, else Level 1 */
  if ((*((uint32_t *)(rdp_bits_address)) & 0x0000FF00) == 0x0000AA00)
    {
      rdp_level = 0;
    }
  else if ((*((uint32_t *)(rdp_bits_address)) & 0x0000FF00) == 0x0000CC00)
    {
      rdp_level = 2;
    }
  else
    {
      rdp_level = 1;
    }

/* Execution of all possible Conditions : All possible RDP level, secure area and Security bit values conditions */
  StepCounter = 0;

   while (StepCounter < 1)
   {

   /* Code executed when the security bit, RDP level and the secure area are not set  */
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 0) && (security_area_set == 0) && (rdp_level == 0))
    {
      /* Display all required steps should be followed by user when the security bit, RDP level and the secure area are not set */

      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 15 , (uint8_t *)"The HDP_Appli has been executed completely", CENTER_MODE);
      HAL_Delay(2000); 
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"The red led is ON", CENTER_MODE);
      BSP_LED_Toggle(LED2); /*red led*/
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 50 , (uint8_t *)"Now, set the security bit using CubeProgrammer!", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 75 , (uint8_t *)"Then, power Off/On the board", CENTER_MODE);
      StepCounter = StepCounter +1 ;
      ButtonState = 0;
    }

   /* Code executed when the security bit and the secure area are not set but the RDP is set to level 1 */
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 0) && (security_area_set == 0) && (rdp_level == 1))
    {
       /* Display all required steps should be followed by user when the security bit and the secure area are not set but the RDP is set to level 1 */
    	BSP_LED_Toggle(LED2); /*red led*/
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
        UTIL_LCD_DisplayStringAt(0, y_size / 2 + 15 , (uint8_t *)"The RDP is set to level 1 but", CENTER_MODE);
        HAL_Delay(2000); 
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(0, y_size / 2 + 45 , (uint8_t *)"the security bit and secure area are not set", CENTER_MODE);
        HAL_Delay(2000);
        UTIL_LCD_DisplayStringAt(0, y_size / 2 + 75 , (uint8_t *)"You didn't follow the correct steps ", CENTER_MODE);
        HAL_Delay(2000);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
        UTIL_LCD_DisplayStringAt(0, y_size / 2 + 105 , (uint8_t *)"Please set the RDP level to 0 and reflash the two applications", CENTER_MODE);
        StepCounter = StepCounter +1 ;
        ButtonState = 0;
    }

   /* This combination should not be possible, it should not be possible to set the security area without the security bit */ 
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 0) && (security_area_set == 1) && (rdp_level == 0))
    {
      /* This condition is impossible, the user don't have the possibility to set the security area without the security bit*/
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 30 , (uint8_t *)"Something went wrong !!", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 15 , (uint8_t *)"You are not allowed to set a secure area", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 , (uint8_t *)"without setting first the security bit", CENTER_MODE);
      HAL_Delay(2000);
      BSP_LED_Toggle(LED1); /* red led*/
      HAL_Delay(1000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"Please contact ST support", CENTER_MODE);
      StepCounter = StepCounter +1 ;
      ButtonState = 0;
    } 

   /* This combination should not be possible, it should not be possible to set the security area and RDP level without the security bit */
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 0) && (security_area_set == 1) && (rdp_level == 1))
    {
      /* This condition is impossible, the user don't have the possibility to set the security area and RDP level without the security bit*/
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 30 , (uint8_t *)"Something went wrong !!", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 15 , (uint8_t *)"You are not allowed to set a secure area", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 , (uint8_t *)"without setting first the security bit", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 , (uint8_t *)"the RDP is set to level 1", CENTER_MODE);
      HAL_Delay(2000);
      BSP_LED_Toggle(LED1); /* red led*/
      HAL_Delay(1000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"Please contact ST support", CENTER_MODE);
      StepCounter = StepCounter +1 ;
      ButtonState = 0;
    }

   /* Code executed when the security bit is set and the secure area and RDP level are not set  */
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 1) && (security_area_set == 0) && (rdp_level == 0))
    {
      /* Display all required steps should be followed by user when the security bit is set and the secure area and RDP level are not set */

      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 15 , (uint8_t *)"The HDP_Appli has been executed completely", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"The red led is ON", CENTER_MODE);
      BSP_LED_Toggle(LED2); /* red led*/
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 45 , (uint8_t *)"The security bit is well activated !! ", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 60 , (uint8_t *)"Now, set the secure area using CubeProgrammer", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 75 ,(uint8_t *)"Between 0x08000000 and 0x0801FFFF", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 90 , (uint8_t *)"Then, power Off/On the board !", CENTER_MODE);
      StepCounter = StepCounter +1 ;
      ButtonState = 0;
    }

   /* Code executed when the security bit is set, the RDP set to level 1 but by mistake the security area has not been set */
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 1) && (security_area_set == 0) && (rdp_level == 1))
    {
      /* Display all required steps should be followed by user when the security bit is set, the RDP set to level 1 but by mistake the security area has not been set */
      BSP_LED_Toggle(LED2); /* red led*/
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"The RDP is set to level 1 but", CENTER_MODE);
      HAL_Delay(2000); 
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 60 , (uint8_t *)"the secure area is not set", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 75 , (uint8_t *)"You didn't follow the correct steps ", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 105 , (uint8_t *)"Please set the RDP level to 0 and reflash the two applications!", CENTER_MODE);
      StepCounter = StepCounter +1 ;
      ButtonState = 0;
    }

   /* Code executed when both the security bit, the secure area are set and RDP level is not set*/
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 1) && (security_area_set == 1) && (rdp_level == 0))
    {
      /* Display all required steps should be followed by user when both the security bit, the secure area are set and RDP level is not set */
      BSP_LED_Toggle(LED2); /* red led*/
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 15 , (uint8_t *)"The HDP_Appli has been executed completely", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"The HDP_Boot executed in a secure area at 0x08000000", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 45 , (uint8_t *)"The HDP_Appli executed in a non-secure area at 0x08040000", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 85 , (uint8_t *)"Connection to CubeProgrammer is allowed", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 100 ,(uint8_t *)"Only when HDP_Appli is executed completely", CENTER_MODE);
      HAL_Delay(3000);
      Display_background();
      HAL_Delay(1000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 45 ,(uint8_t *)"Now you can Connect to CubeProgrammer!", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 30 ,(uint8_t *)"The secured flash area is readout as 0x00000000!", CENTER_MODE);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 15, (uint8_t *)"To remove the secure area, first set the RDP level ", CENTER_MODE);
      HAL_Delay(2000);
      /* Display LCD messages to warn the user before setting the RDP to level 2 */
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2  , (uint8_t *)"Caution do not set RDP level 2, the flash will be locked definitively !!!", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 15 ,(uint8_t *)"Then, refer to the documentation", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"You can set the RDP to level 1 using CubeProgrammer ! ", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 85 , (uint8_t *)"The debugger connection will be lost", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 100 , (uint8_t *)"Power Off/On the board !!", CENTER_MODE);
      StepCounter = StepCounter +1 ;
      ButtonState = 0;
    }

   /* Code executed when the security bit, the secure area and the RDP level 1 are set */
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 1) && (security_area_set == 1) && (rdp_level == 1))
    {
      /* Display all required steps should be followed by user when the security bit, the secure area and the RDP level 1 are set */ 
      BSP_LED_Toggle(LED2); /* red led*/
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 , (uint8_t *)"The RDP is set to level 1", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(0, y_size / 2+ 15 , (uint8_t *)"The HDP_Boot executed in a secure area at 0x08000000", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"The HDP_Appli executed in a non-secure area at 0x08040000", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 60 , (uint8_t *)"Connect CubeProgrammer, the flash content can't be readout", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 85 , (uint8_t *)"but the option bytes can be modified !", CENTER_MODE);
      HAL_Delay(3000);
      Display_background ();
      HAL_Delay(1000);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 45, (uint8_t *)"Please refer to the doc to remove the secure area.", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 - 15, (uint8_t *)"Set RDP level 0 and secure area range setting in one step!", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 +15, (uint8_t *)"The complete flash content will be erased", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 60, (uint8_t *)"Then, reset the security option bit using CubeProgrammer", CENTER_MODE);
      StepCounter = StepCounter +1 ;
      ButtonState = 0;
    }

   /* Code executed when the security bit, the secure area are set and the RDP is set to level 2 */
    if((ButtonState != 0) && (StepCounter == 0) && (security_bit == 1) && (security_area_set == 1) && (rdp_level == 2))
    {
      /* Display all required steps should be followed by user when the security bit, the secure area are set and the RDP is set to level 2 */ 
      BSP_LED_Toggle(LED2); /* red led*/
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 15 , (uint8_t *)"The HDP_Appli has been executed completely", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 30 , (uint8_t *)"The HDP_Boot was executed in a secure area at 0x08000000", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 45 , (uint8_t *)"The HDP_Appli executed in a non-secure area at 0x08040000", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 90 , (uint8_t *)"But the RDP is set to level 2 ", CENTER_MODE);
      HAL_Delay(2000);
      UTIL_LCD_DisplayStringAt(0, y_size / 2 + 105 , (uint8_t *)"The flash is locked and can't be erased anymore !!!", CENTER_MODE);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    }
   
    if((ButtonState != 0) && (StepCounter > 0))
    {
        StepCounter = StepCounter +1 ;
    }
   }

   while (1)
  {
  }
 }


/**
  * @brief  Button Callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if(Button == BUTTON_USER)
  {
    ButtonState = 1;
  }
}

/**
  * @brief  CPU MPU Config.
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

  /* Configure the MPU attributes as WT for OctoSPI RAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = OSPI_RAM_WRITE_READ_ADDR;
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

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

