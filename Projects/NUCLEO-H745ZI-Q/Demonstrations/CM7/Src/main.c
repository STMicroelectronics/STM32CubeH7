/**
  ******************************************************************************
  * @file    Demonstrations/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This demo describes how display bmp images from SD card on LCD using
             the Adafruit 1.8" TFT shield with Joystick and microSD mounted on top
             of the STM32 Nucleo board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_HAL_Demonstrations
  * @{
  */

/** @addtogroup Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  SHIELD_NOT_DETECTED = 0,
  SHIELD_DETECTED
}ShieldStatus;

/* Private define ------------------------------------------------------------*/
#define SD_CARD_NOT_FORMATTED                    0
#define SD_CARD_FILE_NOT_SUPPORTED               1
#define SD_CARD_OPEN_FAIL                        2
#define FATFS_NOT_MOUNTED                        3
#define BSP_SD_INIT_FAILED                       4
#define SD_CARD_NO_FILE                          5

#define POSITION_X_BITMAP                        0
#define POSITION_Y_BITMAP                        0

#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t BlinkSpeed = 0, str[20];
__IO uint8_t JoystickValue = 0;
char* pDirectoryFiles[MAX_BMP_FILES];
FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);
static void LED1_Blink(void);
static ShieldStatus TFT_ShieldDetect(void);
static void SDCard_Config(void);
static void TFT_DisplayErrorMessage(uint8_t message);
static void TFT_DisplayMenu(void);
static void TFT_DisplayImages(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  int32_t timeout;
  
  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7) 
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }

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

  /* When system initialization is finished, Cortex-M7 will release Cortex-M4  by means of
     HSEM notification */

  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

  /*Take HSEM */
  HAL_HSEM_FastTake(HSEM_ID_0);
  /*Release HSEM in order to notify the CPU2(CM4)*/
  HAL_HSEM_Release(HSEM_ID_0,0);
  

  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PF.03 pin (mapped to JoyStick
     available on adafruit 1.8" TFT shield). If the state of PF.03 is high then
     the adafruit 1.8" TFT shield is available. */
  
  if(TFT_ShieldDetect() == SHIELD_DETECTED)
  {
    /* Initialize the LCD */
    BSP_LCD_Init();

    /* Configure SD card */
    SDCard_Config();

    /* Display on TFT Images existing on SD card */
    TFT_DisplayImages();
  }
  else /* Shield not mounted */
  {
    LED1_Blink();
  }

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
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
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

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

/**
  * @brief  Displays demonstration menu.
  * @param  None
  * @retval None
  */
static void TFT_DisplayMenu(void)
{
  JOYState_TypeDef tmp;

  /* Set Menu font */
  BSP_LCD_SetFont(&Font12);

  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  /* Display message */
  BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"   NUCLEO-H745ZI    ");
  BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"       DEMO         ");

  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  /* Display message */
  BSP_LCD_DisplayStringAtLine(4, (uint8_t*)"Display images      ");
  BSP_LCD_DisplayStringAtLine(6, (uint8_t*)"stored under uSD    ");
  BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"on TFT LCD          ");

  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  /* Display message */
  BSP_LCD_DisplayStringAtLine(11, (uint8_t*)"  Press JOY DOWN   ");
  BSP_LCD_DisplayStringAtLine(12, (uint8_t*)"  to continue...   ");

  /* Wait for JOY_DOWN is pressed */
  while (BSP_JOY_GetState() != JOY_DOWN)
  {}
  HAL_Delay(200);
  
  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  /* Display message */
  BSP_LCD_DisplayStringAtLine(4,  (uint8_t*)"                   ");
  BSP_LCD_DisplayStringAtLine(6,  (uint8_t*)"  Press Joystick   ");

  /* Set Text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  /* Display message */
  BSP_LCD_DisplayStringAtLine(8,  (uint8_t*)"  UP for:          ");
  BSP_LCD_DisplayStringAtLine(9,  (uint8_t*)"  Manual Mode      ");
  BSP_LCD_DisplayStringAtLine(11, (uint8_t*)"  DOWN for:        ");
  BSP_LCD_DisplayStringAtLine(12, (uint8_t*)"  Automatic Mode   ");

  /* Wait until Joystick is released */
  while (BSP_JOY_GetState() == JOY_DOWN)
  {}
  HAL_Delay(200);

  /* Wait for JOY_DOWN or JOY_UP is pressed */
  tmp = JOY_RIGHT;
  while ((tmp != JOY_DOWN) && (tmp != JOY_UP))
  {
    tmp = BSP_JOY_GetState();
  }

  /* LCD Clear */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* JOY_UP is pressed: Display Manual mode menu #############################*/
  if(tmp == JOY_UP )
  {
    /* Set Text color */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    /* Display message */
    BSP_LCD_DisplayStringAtLine(3,  (uint8_t*)"   Manual Mode   ");
    BSP_LCD_DisplayStringAtLine(5,  (uint8_t*)"    Selected     ");

    /* Set Text color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    /* Display message */
    BSP_LCD_DisplayStringAtLine(9,  (uint8_t*)"RIGHT: Next image");
    BSP_LCD_DisplayStringAtLine(10, (uint8_t*)"LEFT : Previous  ");
    BSP_LCD_DisplayStringAtLine(11, (uint8_t*)"SEL  : Switch to ");
    BSP_LCD_DisplayStringAtLine(12, (uint8_t*)"automatic mode   ");
    JoystickValue = 2;
  }
  /* JOY_DOWN is pressed: Display Automatic mode menu ########################*/
  else if (tmp == JOY_DOWN)
  {
    /* Set Text color */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    /* Display message */
    BSP_LCD_DisplayStringAtLine(3,  (uint8_t*)"  Automatic Mode  ");
    BSP_LCD_DisplayStringAtLine(5,  (uint8_t*)"     Selected     ");

    JoystickValue = 1;
    HAL_Delay(200);
  }
}

/**
  * @brief  Displays on TFT Images or error messages when error occurred.
  * @param  None
  * @retval None
  */
static void TFT_DisplayImages(void)
{
  uint32_t bmplen = 0x00;
  uint32_t checkstatus = 0x00;
  uint32_t filesnumbers = 0x00;
  uint32_t joystickstatus = JOY_NONE;
  uint32_t bmpcounter = 0x00;
  DIR directory;
  FRESULT res;

  /* Initialize the Joystick available on adafruit 1.8" TFT shield */
  BSP_JOY_Init();

  /* Welcome message */
  TFT_DisplayMenu();

  /* Open directory */
  res = f_opendir(&directory, "/");
  if((res != FR_OK))
  {
    if(res == FR_NO_FILESYSTEM)
    {
      /* Display message: SD card not FAT formatted */
      TFT_DisplayErrorMessage(SD_CARD_NOT_FORMATTED);
    }
    else
    {
      /* Display message: Fail to open directory */
      TFT_DisplayErrorMessage(SD_CARD_OPEN_FAIL);
    }
  }

  /* Get number of bitmap files */
  filesnumbers = Storage_GetDirectoryBitmapFiles ("/", pDirectoryFiles);
  if(filesnumbers == 0)
  {
    /* Display message: Fail to open directory */
    TFT_DisplayErrorMessage(SD_CARD_NO_FILE);
  }
  /* Set bitmap counter to display first image */
  bmpcounter = 1;

  /* Infinite loop */
  while (1)
  {
    /* Get JoyStick status */
    joystickstatus = BSP_JOY_GetState();

    if(joystickstatus == JOY_SEL )
    {
      JoystickValue++;
      if (JoystickValue > 2)
      {
        JoystickValue = 1;
      }
      joystickstatus = JOY_NONE;
    }
    if((JoystickValue == 1) || ((JoystickValue == 2) && (( joystickstatus == JOY_RIGHT) || (joystickstatus == JOY_LEFT))))
    {
      /*Take HSEM */
      HAL_HSEM_FastTake(HSEM_ID_0);
    }
    /*## Display BMP pictures in Automatic mode ##############################*/
    if(JoystickValue == 1)
    {
      sprintf((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter -1]);

      checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);

      if(checkstatus == 0)
      {
        /* Format the string */
        checkstatus = Storage_OpenReadFile(POSITION_X_BITMAP, POSITION_Y_BITMAP, (const char*)str);
        HAL_Delay(1500);
      }
      else if (checkstatus == 1)
      {
        /* Display message: File not supported */
        TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
      }

      bmpcounter++;
      if(bmpcounter > filesnumbers)
      {
        bmpcounter = 1;
      }
    }

    /*## Display BMP pictures in Manual mode #################################*/
    if(JoystickValue == 2)
    {
      if( joystickstatus == JOY_RIGHT )
      {
        if((bmpcounter + 1) > filesnumbers)
        {
          bmpcounter = 1;
        }
        else
        {
          bmpcounter++;
        }
        sprintf ((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter - 1]);

        checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);

        if(checkstatus == 0)
        {
          /* Format the string */
          Storage_OpenReadFile(POSITION_X_BITMAP, POSITION_Y_BITMAP, (const char*)str);
        }

        if(checkstatus == 1)
        {
          /* Display message: File not supported */
          TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
        }
      }
      else if(joystickstatus == JOY_LEFT )
      {
        if((bmpcounter - 1) < 1)
        {
          bmpcounter = filesnumbers;
        }
        else
        {
          bmpcounter--;
        }
        sprintf ((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter - 1]);
        checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);

        if(checkstatus == 0)
        {
          /* Format the string */
          Storage_OpenReadFile(POSITION_X_BITMAP, POSITION_Y_BITMAP, (const char*)str);
        }

        if (checkstatus == 1)
        {
          /* Display message: File not supported */
          TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
        }
      }
    }
    
    if((JoystickValue == 1) || (joystickstatus != JOY_NONE ))
    {
      /*Release HSEM in order to notify the CPU2(CM4)*/
      HAL_HSEM_Release(HSEM_ID_0,0);
    }
    
    joystickstatus = JOY_NONE;

  }
}

/**
  * @brief  SD Card Configuration.
  * @param  None
  * @retval None
  */
static void SDCard_Config(void)
{
  uint32_t counter = 0;

  if(FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
  {
    /* Initialize the SD mounted on adafruit 1.8" TFT shield */
    if(BSP_SD_Init() != MSD_OK)
    {
      TFT_DisplayErrorMessage(BSP_SD_INIT_FAILED);
    }

    /* Check the mounted device */
    if(f_mount(&SD_FatFs, (TCHAR const*)"/", 0) != FR_OK)
    {
      TFT_DisplayErrorMessage(FATFS_NOT_MOUNTED);
    }
    else
    {
      /* Initialize the Directory Files pointers (heap) */
      for (counter = 0; counter < MAX_BMP_FILES; counter++)
      {
        pDirectoryFiles[counter] = malloc(11);
      }
    }
  }
}

/**
  * @brief  Displays adequate message on TFT available on adafruit 1.8" TFT shield
  * @param  message: Error message to be displayed on the LCD.
  *   This parameter can be one of following values:
  *     @arg SD_CARD_NOT_FORMATTED: SD CARD is not FAT formatted
  *     @arg SD_CARD_FILE_NOT_SUPPORTED: File is not supported
  *     @arg SD_CARD_OPEN_FAIL: Failure to open directory
  *     @arg FATFS_NOT_MOUNTED: FatFs is not mounted
  * @retval None
  */
static void TFT_DisplayErrorMessage(uint8_t message)
{
  /* LCD Clear */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  /* Set Error Message Font */
  BSP_LCD_SetFont(&Font12);
  /* Set Text and Back colors */
  BSP_LCD_SetBackColor(LCD_COLOR_GREY);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);

  if(message == SD_CARD_NOT_FORMATTED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)" SD Card is not    ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" FAT formatted.    ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" Please Format the ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)" microSD card.     ");
    while (1)
    {
    }
  }
  if(message == SD_CARD_FILE_NOT_SUPPORTED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" File type is not  ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" supported.        ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while(1)
    {
    }
  }
  if(message == SD_CARD_OPEN_FAIL)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" Open directory    ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" fails.            ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while(1)
    {
    }
  }
  if(message == FATFS_NOT_MOUNTED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" Cannot mount      ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" FatFs on Drive.   ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while (1)
    {
    }
  }
  if(message == BSP_SD_INIT_FAILED)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" SD Init           ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" fails.            ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while(1)
    {
    }
  }
  if(message == SD_CARD_NO_FILE)
  {
    /* Display message */
    BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"                   ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t*)" NO image found     ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t*)" in the SD card.   ");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"                   ");
    while(1)
    {
    }
  }
}

/**
  * @brief  Blinks LED1 with two frequencies depending on User press button.
  * @param  None
  * @retval None
  */
static void LED1_Blink(void)
{
  /* Configure LED1 on Nucleo */
  BSP_LED_Init(LED1);

  /* Configure the User Button in EXTI Mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initiate BlinkSpeed variable */
  BlinkSpeed = 0;

  /*Take HSEM */ 
  HAL_HSEM_FastTake(HSEM_ID_0); 

  /* Infinite loop */
  while(1)
  {
    /*Release HSEM in order to notify the CPU2(CM4)*/     
    HAL_HSEM_Release(HSEM_ID_0,0);
    
    /* Test on blink speed */
    if(BlinkSpeed == 0)
    {
      BSP_LED_Toggle(LED1);
      /* Wait for 500ms */
      HAL_Delay(500);
    }
    else if(BlinkSpeed == 1)
    {
      BSP_LED_Toggle(LED1);
      /* Wait for 100ms */
      HAL_Delay(100);
    }
    else if(BlinkSpeed == 2)
    {
      BSP_LED_Toggle(LED1);
      /* Wait for 50ms */
      HAL_Delay(50);
    }
    /*Take HSEM */ 
    HAL_HSEM_FastTake(HSEM_ID_0);    
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(BlinkSpeed == 2)
  {
    BlinkSpeed = 0;
  }
  else
  {
    BlinkSpeed ++;
  }
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

  if(HAL_GPIO_ReadPin(NUCLEO_ADCx_GPIO_PORT, NUCLEO_ADCx_GPIO_PIN) != 0)
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
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
void assert_failed(uint8_t *file, uint32_t line)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
