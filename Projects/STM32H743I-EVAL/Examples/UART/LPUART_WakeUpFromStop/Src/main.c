/**
  ******************************************************************************
  * @file    UART/LPUART_WakeUpFromStop/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART HAL API (LPUART instance)
  *          to wake up the MCU from STOP mode
  *          Two boards are used, one which enters STOP mode and the second
  *          one which sends the wake-up stimuli.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup LPUART_WakeUpFromStop
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* to enable for the board entering STOP mode,
   to disable for the board sending wake-up stimuli */
#define BOARD_IN_STOP_MODE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
UART_WakeUpTypeDef WakeUpSelection; 
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */

/* Buffer used for confirmation messages transmission */
uint8_t aTxBuffer1[] = "RXNE wake-up successful";
uint8_t aTxBuffer2[] = "Start bit detection wake-up successful";
uint8_t aTxBuffer3[] = "7-bit address match wake-up successful";
uint8_t aTxBuffer4[] = "4-bit address match wake-up successful";

uint8_t aTxBuffer[] = "Start bit detection wake-up successful";
uint8_t aWakeUpTrigger1[] = "R";
uint8_t aWakeUpTrigger2[] = "S";
uint8_t aWakeUpTrigger3[] = {0xA9};
uint8_t aWakeUpTrigger4[] = {0x82};

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
#if !defined(BOARD_IN_STOP_MODE)
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif /* !defined(BOARD_IN_STOP_MODE) */
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
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
  
  /* Configure the system clock to have a frequency of 400 MHz */
  SystemClock_Config();
  
#ifdef BOARD_IN_STOP_MODE
  /* Select HSI as wakeup clock from stop mode */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
#endif
  
  /* Configure LED1, LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /*##-1- Configure the LPUART peripheral ####################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* LPUART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals)
      - Oversampling at 16
      - One bit sampling feature disabled */
  UartHandle.Instance        = LPUARTx;

  
  UartHandle.Init.BaudRate       = 115200;
  UartHandle.Init.WordLength     = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits       = UART_STOPBITS_1;
  UartHandle.Init.Parity         = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode           = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling   = UART_OVERSAMPLING_16;
  UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef BOARD_IN_STOP_MODE
  /* LED1 is on till stop mode */
  BSP_LED_On(LED1);
  
  /* wait for two seconds before test start */
  HAL_Delay(2000);
  
  /* Activate LPUART1 autonomous mode */
  __HAL_RCC_LPUART1_CLKAM_ENABLE();
  
  /* make sure that no LPUART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  /* make sure that LPUART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
  
  /* set the wake-up event:
   * specify wake-up on RXNE flag */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_READDATA_NONEMPTY;
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler(); 
  }
  
  /* Enable the LPUART Wake UP from STOP mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter STOP mode: switch off LED */
  BSP_LED_Off(LED1);
  
  /* Enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle);
  
  /* Enter STOP mode */
  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
  
  /* ... STOP mode ... */
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config();
  
  /* Wake Up based on RXNE flag successful */
  HAL_UARTEx_DisableStopMode(&UartHandle);
  
  /* wait for some delay */
  HAL_Delay(100);
  
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer1, COUNTOF(aTxBuffer1)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }

  /*##-2- Wake Up second step  ###############################################*/
  /* make sure that no LPUART transfer is on-going */ 
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  
  /* make sure that LPUART is ready to receive 
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
  
  /* set the wake-up event:
   * specify wake-up on start-bit detection */
  WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable the LPUART Wake UP from STOP mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter STOP mode: switch off LED */
  BSP_LED_Off(LED1);
  
  /* enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle);
  
  /* enter STOP mode */
  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
  
  /* ... STOP mode ... */
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config();
  
  /* Wake Up on start bit detection successful */ 
  HAL_UARTEx_DisableStopMode(&UartHandle);
  
  /* wait for some delay */
  HAL_Delay(100);
  
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer2, COUNTOF(aTxBuffer2)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-3- Wake Up third step  ################################################*/
 /* make sure that no LPUART transfer is on-going */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  
  /* make sure that LPUART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
  
  /* set the wake-up event:  
   * specify address-to-match type. 
   * The address is 0x29, meaning the character triggering the 
   * address match is 0xA9 */
  WakeUpSelection.WakeUpEvent   = UART_WAKEUP_ON_ADDRESS;
  WakeUpSelection.AddressLength = UART_ADDRESS_DETECT_7B;
  WakeUpSelection.Address       = 0x29;
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable the LPUART Wake UP from stop mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter stop mode: switch off LED */
  BSP_LED_Off(LED1);
  
  /* enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle);
  
  /* enter STOP mode */
  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
  
  /* ... STOP mode ... */
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config();
  
  /* Wake Up on 7-bit address detection successful */
  HAL_UARTEx_DisableStopMode(&UartHandle);
  
  /* wait for some delay */
  HAL_Delay(100);
  
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer3, COUNTOF(aTxBuffer3)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-4- Wake Up fourth step  ###############################################*/
  /* make sure that no LPUART transfer is on-going */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_BUSY) == SET);
  
  /* make sure that LPUART is ready to receive
   * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
  while(__HAL_UART_GET_FLAG(&UartHandle, USART_ISR_REACK) == RESET);
  
  /* set the wake-up event:  
   * specify address-to-match type. 
   * The address is 0x2, meaning the character triggering the 
   * address match is 0x82 */
  WakeUpSelection.WakeUpEvent   = UART_WAKEUP_ON_ADDRESS;
  WakeUpSelection.AddressLength = UART_ADDRESS_DETECT_4B;
  WakeUpSelection.Address       = 0x2;
  if (HAL_UARTEx_StopModeWakeUpSourceConfig(&UartHandle, WakeUpSelection)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable the LPUART Wake UP from STOP mode Interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);
  
  /* about to enter STOP mode: switch off LED */
  BSP_LED_Off(LED1);
  
  /* enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&UartHandle);
  
  /* enter STOP mode */
  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
  
  /* ... STOP mode ... */
  
  /* at that point, MCU has been awoken: the LED has been turned back on */
  SystemClock_Config();
  
  /* Wake Up on 4-bit address detection successful */
  /* wait for some delay */
  HAL_Delay(100);
  
  /* Inform other board that wake up is successful */
  if (HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer4, COUNTOF(aTxBuffer4)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
#else /* BOARD_IN_STOP_MODE */

  /* initialize the User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  
  /* Wait for User push-button press before starting the test.
     In the meantime, LED1 is blinking */
  while(UserButtonStatus == 0)
  {
      /* Toggle LED1 */
      BSP_LED_Toggle(LED1);
      HAL_Delay(100);
  }
  
  /*##-2- Send the wake-up from stop mode first trigger ######################*/
  /*      (RXNE flag setting)                                                 */
  BSP_LED_On(LED1);
  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger1, COUNTOF(aWakeUpTrigger1)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer1)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED1);
  
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer1,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer1)-1))
  {
    Error_Handler();
  }
  
  /* wait for two seconds before test second step */
  HAL_Delay(2000);
  
  /*##-3- Send the wake-up from stop mode second trigger #####################*/
  /*      (start Bit detection)                                               */
  BSP_LED_On(LED1);  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger2, COUNTOF(aWakeUpTrigger2)-1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer2)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED1);
  
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer2,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer2)-1))
  {
    Error_Handler();
  }
  
  /* wait for two seconds before test third step */
  HAL_Delay(2000);
  
  /*##-4- Send the wake-up from stop mode third trigger ######################*/
  /*      (7-bit address match)                                               */
  BSP_LED_On(LED1);
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger3, 1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer3)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED1);
  
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer3,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer3)-1))
  {
    Error_Handler();
  }
  
  /* wait for two seconds before test fourth and last step */
  HAL_Delay(2000);
  
  /*##-5- Send the wake-up from stop mode fourth trigger #####################*/
  /*      (4-bit address match)                                               */
  BSP_LED_On(LED1);
  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aWakeUpTrigger4, 1, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /* Put LPUART peripheral in reception process to wait for other board
     wake up confirmation */  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer4)-1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_Off(LED1);
  
  /* Compare the expected and received buffers */
  if(Buffercmp((uint8_t*)aTxBuffer4,(uint8_t*)aRxBuffer,COUNTOF(aTxBuffer4)-1))
  {
    Error_Handler();
  }
  
  HAL_Delay(2000);
  
#endif /* BOARD_IN_STOP_MODE */
  
  /* Turn on LED1 if test passes then enter infinite loop */
  BSP_LED_On(LED1);
  while (1)
  {
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

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
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
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  Error_Handler();
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == BUTTON_TAMPER_PIN)
  {  
    UserButtonStatus = 1;
  }
}

#if !defined(BOARD_IN_STOP_MODE)
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }
  
  return 0;
}
#endif /* !defined(BBOARD_IN_STOP_MODE) */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
    /* In case of error turn On LED3 */
    BSP_LED_On(LED3);
    HAL_Delay(100);
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
#endif

/**
  * @}
  */

/**
  * @}
  */

