/**
  ******************************************************************************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithLCD/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure 
  *          DMA2D peripheral in Memory to Memory transfer mode 
  *          and display the result on LCD, in resorting to DMA2D LL APIs.
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
#include "RGB565_128x160.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithLCD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
  SHIELD_NOT_DETECTED = 0, 
  SHIELD_DETECTED
}ShieldStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA2D output address in SRAM : this is the buffer displayed on LCD screen */
/* DMA2D output buffer shall be 32 Bytes aligned to ensure correct cache maintenance (aligned with L1 Data cache line size) */
ALIGN_32BYTES(uint16_t aBufferResult[LAYER_SIZE_X * LAYER_SIZE_Y]);
/* Blended image Ready flag */
__IO uint32_t   blended_image_ready;
__IO uint32_t   dma2d_error_status;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static ShieldStatus TFT_ShieldDetect(void);
static void LCD_ImageDisplay(void);
static void DMA2D_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  blended_image_ready = 0;
  dma2d_error_status = 0;
  
  
  /* Configure the MPU attributes */
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
  
  /* Configure the system clock */
  SystemClock_Config(); 
    
  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PF.03 pin (mapped to 
     JoyStick available on adafruit 1.8" TFT shield). If the state of PF.03 
     is high then the adafruit 1.8" TFT shield is available. */
  
  if(TFT_ShieldDetect() != SHIELD_DETECTED)
  {
    Error_Handler();
  }

  /* Initialize the LCD */
  ADAFRUIT_802_LCD_Init(0, LCD_ORIENTATION_PORTRAIT);
  
  /*##-2- DMA2D configuration ################################################*/
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_DMA2D);
  NVIC_SetPriority(DMA2D_IRQn, 0);  
  NVIC_EnableIRQ(DMA2D_IRQn);
  
  DMA2D_Config();
  
  /*##-3- Start DMA2D transfer ###############################################*/  
  LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&RGB565_128x160);      /* Source buffer in format RGB565 and size 128x160                 */
  LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)aBufferResult);    /* Output Buffer                                                   */

  LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y, LAYER_SIZE_X);   /* Configure DMA2D transfer number of lines and number of pixels per line */
  LL_DMA2D_SetLineOffset(DMA2D, 0);  /* Configure DMA2D output line offset to Zero as the image width is equal to LCD  width */
   
  /* Enable the transfer complete, transfer error and configuration error interrupts */
  LL_DMA2D_EnableIT_TC(DMA2D);
  LL_DMA2D_EnableIT_TE(DMA2D);
  LL_DMA2D_EnableIT_CE(DMA2D);  
  
  /* Enable the Peripheral */
  LL_DMA2D_Start(DMA2D);
  
  /*##-4- Wait until DMA2D transfer is over ################################################*/
  while((blended_image_ready == 0) && (dma2d_error_status == 0)) {;}

  if(dma2d_error_status != 0)
  {
    /* Call Error function */
    OnError_Handler(1);	  
  }
  else
  {
    /*
     CPU Data Cache maintenance :
     It is recommended to invalidate the CPU Data cache after the DMA2D transfer.
     As the destination buffer may be used by the CPU, this guarantees Up-to-date data when CPU accesses
     to the destination buffer located in the AXI-SRAM (which is cacheable).
    */
    SCB_InvalidateDCache_by_Addr((uint32_t *) aBufferResult, sizeof(aBufferResult) );

    /* Turn LED1 On */
    BSP_LED_On(LED1);  
  }

  /*##-5- Display the image ################################################*/
  LCD_ImageDisplay();
  
  while (1)
  { 
    ; 
  }
}

/**
  * @brief DMA2D configuration.
  * @note  This function Configure the DMA2D peripheral :
  *        1) Configure the transfer mode : memory to memory
  *        2) Configure the output color mode as RGB565
  *        3) Configure the transfer from FLASH to SRAM   
  *        4) Configure the data size : 128x160 (pixels)  
  * @retval
  *  None
  */
static void DMA2D_Config(void)
{
  /* Configure the DMA2D Color Mode */  
  LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_RGB565);
  
  /* Foreground Configuration:     */
  /* Set Alpha value to full opaque */
  LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF);
  /* Foreground layer format is RGB565 (16 bpp) */
  LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
}

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED2);
    while(1) { ; } /* Blocking on error */
  }
}

/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO pin (mapped to JoyStick
  *         available on adafruit 1.8" TFT shield). If the state of this pin 
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
  * @brief  LCD display image on Adafruit LCD.
  * @param  None
  * @retval None
  */
static void LCD_ImageDisplay(void)
{
  int16_t line = 0;
  uint16_t pixel = 0;
  uint16_t *img_ptr = NULL;
  
  /* Set pointer to DMA2D output buffer */
  img_ptr = aBufferResult;
  line = LAYER_SIZE_Y - 1;
  while (line >= 0)
  {
    /* Write pixels */ 
    ADAFRUIT_802_LCD_WritePixel(0, pixel, line, (uint32_t)*img_ptr);
    img_ptr++;
    pixel++;
    if (pixel == LAYER_SIZE_X)
    {
      /* Move to next line */
      pixel = 0;
      line--;	
    }
  }
}

/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
  *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 260
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
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 260;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    /* Initialization Error */
    while(1);
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
    /* Initialization Error */
    while(1);
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

