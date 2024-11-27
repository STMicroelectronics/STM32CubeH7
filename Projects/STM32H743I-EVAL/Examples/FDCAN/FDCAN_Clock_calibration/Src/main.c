/**
  ******************************************************************************
  * @file    FDCAN_Clock_calibration/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
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

/** @addtogroup FDCAN_Clock_calibration
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

#define FDCAN_receiver

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan;
FDCAN_ClkCalUnitTypeDef sCcuConfig;
FDCAN_FilterTypeDef sFilterConfig;
#ifdef FDCAN_receiver
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[8];
  __IO uint8_t UserButtonClickEvent = RESET;
#else
  uint8_t TxDataCalib[] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
#endif
FDCAN_TxHeaderTypeDef TxHeader;
uint8_t TxData[] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
#ifdef FDCAN_receiver
  static uint32_t BufferCmp8b(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif

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
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
#ifdef FDCAN_receiver
  /* Configure Tamper push-button in interrupt mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
#endif
  
  /* Initializes the FDCAN peripheral */
  hfdcan.Instance = FDCANx;
  hfdcan.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan.Init.AutoRetransmission = DISABLE;
  hfdcan.Init.TransmitPause = DISABLE;
  hfdcan.Init.ProtocolException = ENABLE;
  hfdcan.Init.NominalPrescaler = 0x1; /* tq = NominalPrescaler x (1/fdcan_ker_ck) */
  hfdcan.Init.NominalTimeSeg1 = 0xF; /* NominalTimeSeg1 = Propagation_segment + Phase_segment_1 */
  hfdcan.Init.NominalTimeSeg2 = 0x4;
  hfdcan.Init.NominalSyncJumpWidth = 0x4;
#ifdef FDCAN_receiver
  /* Message RAM configuration:
    ***************************
                ________
    0x4000AC00 |________| Standard_filter[0]
    0x4000AC04 |________| Standard_filter[1]
    0x4000AC08 |||||||||| Rx_buffer[0]
               ||||||||||
               ||||||||||
               ||||||||||
    0x4000AC18 |________| Rx_buffer[1]
               |________|
               |________|
               |________|
    0x4000AC28 |||||||||| Tx_buffer[0]
               ||||||||||
               ||||||||||
    0x4000AC34 ||||||||||
  */
  hfdcan.Init.MessageRAMOffset = 0;
  hfdcan.Init.StdFiltersNbr = 2;
  hfdcan.Init.ExtFiltersNbr = 0;
  hfdcan.Init.RxFifo0ElmtsNbr = 0;
  hfdcan.Init.RxFifo1ElmtsNbr = 0;
  hfdcan.Init.RxBuffersNbr = 2;
  hfdcan.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan.Init.TxEventsNbr = 0;
  hfdcan.Init.TxBuffersNbr = 1;
  hfdcan.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  HAL_FDCAN_Init(&hfdcan);
  
  /* Enable clock calibration */
  sCcuConfig.ClockCalibration = ENABLE;
  sCcuConfig.MinOscClkPeriods = 4;
  sCcuConfig.CalFieldLength = FDCAN_CALIB_FIELD_LENGTH_64;
  sCcuConfig.TimeQuantaPerBitTime = 20;
  HAL_FDCAN_ConfigClockCalibration(&hfdcan, &sCcuConfig);
  
  /* Check that FDCAN peripheral entered Restricted Operation Mode */
  if(HAL_FDCAN_IsRestrictedOperationMode(&hfdcan) == 0)
  {
    Error_Handler();
  }
  
  /* Configure the calibration reception filter */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_DUAL;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXBUFFER;
  sFilterConfig.FilterID1 = 0x555;
  sFilterConfig.RxBufferIndex = 0;
  sFilterConfig.IsCalibrationMsg = 1;
  HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig);
  
  /* Configure the validation reception filter */
  sFilterConfig.FilterIndex = 1;
  sFilterConfig.FilterID1 = 0x123;
  sFilterConfig.RxBufferIndex = 1;
  sFilterConfig.IsCalibrationMsg = 0;
  HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig);
  
  /* Configure calibration confirmation message */
  TxHeader.Identifier = 0x111;
  TxHeader.IdType = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader.DataLength = FDCAN_DLC_BYTES_8;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0x00;
  HAL_FDCAN_AddMessageToTxBuffer(&hfdcan, &TxHeader, TxData, FDCAN_TX_BUFFER0);
  
  /* Wait for event on Tamper push button to start calibration */
  while((UserButtonClickEvent) == RESET){;}
  
  /* Start the FDCAN module */
  HAL_FDCAN_Start(&hfdcan);
  
  /* Wait for calibration message received */
  while(HAL_FDCAN_IsRxBufferMessageAvailable(&hfdcan, FDCAN_RX_BUFFER0) == 0){;}
  
  /* Check for Calibration state change */
  while(__HAL_FDCAN_GET_FLAG(&hfdcan, FDCAN_FLAG_CALIB_STATE_CHANGED) == 0){;}
  
  /* Confirm basic calibration */
  if(HAL_FDCAN_GetClockCalibrationState(&hfdcan) != FDCAN_CLOCK_BASIC_CALIBRATED)
  {
    Error_Handler();
  }
  
  /* Clear Calibration State Changed flag */
  __HAL_FDCAN_CLEAR_FLAG(&hfdcan, FDCAN_FLAG_CALIB_STATE_CHANGED);
  
  /* Check for Calibration state change */
  while(__HAL_FDCAN_GET_FLAG(&hfdcan, FDCAN_FLAG_CALIB_STATE_CHANGED) == 0){;}
  
  /* Confirm precision calibration */
  if(HAL_FDCAN_GetClockCalibrationState(&hfdcan) != FDCAN_CLOCK_PRECISION_CALIBRATED)
  {
    Error_Handler();
  }
  
  /* Exit Restricted Operation Mode */
  HAL_FDCAN_ExitRestrictedOperationMode(&hfdcan);
  
  /* Send calibration confirmation message */
  HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
  
  /* Wait for validation message received */
  while(HAL_FDCAN_IsRxBufferMessageAvailable(&hfdcan, FDCAN_RX_BUFFER1) == 0){;}
  
  /* Retrieve Rx message from RAM */
  HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_BUFFER1, &RxHeader, RxData);
  
  /* Compare payload to expected data */
  if (BufferCmp8b(TxData, RxData, 8) != 0)
  {
    Error_Handler();
  }
#else
    /* Message RAM configuration:
    ***************************
                ________
    0x4000AC00 |________| Standard_filter[0]
    0x4000AC04 |||||||||| Rx_buffer[0]
               ||||||||||
               ||||||||||
               ||||||||||
    0x4000AC14 |________| Tx_buffer[0]
               |________|
               |________|
               |________|
    0x4000AC24 |||||||||| Tx_buffer[1]
               ||||||||||
               ||||||||||
    0x4000AC30 ||||||||||
  */
  hfdcan.Init.MessageRAMOffset = 0;
  hfdcan.Init.StdFiltersNbr = 1;
  hfdcan.Init.ExtFiltersNbr = 0;
  hfdcan.Init.RxFifo0ElmtsNbr = 0;
  hfdcan.Init.RxFifo1ElmtsNbr = 0;
  hfdcan.Init.RxBuffersNbr = 1;
  hfdcan.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan.Init.TxEventsNbr = 0;
  hfdcan.Init.TxBuffersNbr = 2;
  hfdcan.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  HAL_FDCAN_Init(&hfdcan);
  
  /* Keep clock calibration disabled */
  sCcuConfig.ClockCalibration = DISABLE;
  sCcuConfig.ClockDivider = FDCAN_CLOCK_DIV4;
  HAL_FDCAN_ConfigClockCalibration(&hfdcan, &sCcuConfig);
  
  /* Configure reception filter */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_DUAL;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXBUFFER;
  sFilterConfig.FilterID1 = 0x111;
  sFilterConfig.RxBufferIndex = 0;
  sFilterConfig.IsCalibrationMsg = 0;
  HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig);
  
  /* Configure calibration message */
  TxHeader.Identifier = 0x555;
  TxHeader.IdType = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader.DataLength = FDCAN_DLC_BYTES_8;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0x00;
  HAL_FDCAN_AddMessageToTxBuffer(&hfdcan, &TxHeader, TxDataCalib, FDCAN_TX_BUFFER0);
  
  /* Configure validation message */
  TxHeader.Identifier = 0x123;
  HAL_FDCAN_AddMessageToTxBuffer(&hfdcan, &TxHeader, TxData, FDCAN_TX_BUFFER1);
  
  /* Start the FDCAN module */
  HAL_FDCAN_Start(&hfdcan);
  
  while(HAL_FDCAN_IsRxBufferMessageAvailable(&hfdcan, FDCAN_RX_BUFFER0) == 0)
  {
    /* Send calibration messages */
    HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
  }
  
  /* Send validation message */
  HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER1);
#endif
  
  /* Infinite loop */
  while (1)
  {
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);
    HAL_Delay(100);
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
  *            PLL_Q                          = 10
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
  
  RCC_OscInitStruct.PLL.PLLQ = 10;  /* fdcan_ker_ck = 80 MHz */
  
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; /* fdcan_pclk = 100 MHz */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED3);
  
  while(1)
  {
  }
}

#ifdef FDCAN_receiver

/**
  * @brief EXTI line detection callback
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_TAMPER_PIN)
  {
    /* Report Tamper push button event to main program */
    UserButtonClickEvent = SET;
    
  }
}

/**
  * @brief Compares two buffers.
  * @par Input         
  *  - pBuffer1, pBuffer2: buffers to be compared.
  *  - BufferLength: buffer's length
  * @par Output
  * None.
  * @retval
  *   0: pBuffer1 identical to pBuffer2
  *   1: pBuffer1 differs from pBuffer2
  */
static uint32_t BufferCmp8b(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  return 0;  
}

#endif

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

