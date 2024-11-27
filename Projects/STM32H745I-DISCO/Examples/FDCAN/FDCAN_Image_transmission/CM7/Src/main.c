/**
  ******************************************************************************
  * @file    FDCAN_Image_transmission/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body for Cortex-M7.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup FDCAN_Image_transmission
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t LCD_X_Size = 0, LCD_Y_Size = 0;
FDCAN_HandleTypeDef hfdcan;
FDCAN_FilterTypeDef sFilterConfig;
uint32_t idx = 0;
uint32_t counter = 0;
FDCAN_TxHeaderTypeDef TxHeader;  /*Tx Messsage Header with Bit Rate Switching Off */
FDCAN_TxHeaderTypeDef TxHeader2; /*Tx Messsage Header with Bit Rate Switching ON */
FDCAN_RxHeaderTypeDef RxHeader;
ALIGN_32BYTES (uint8_t RxData[ImageSize]);
__IO uint8_t UserButtonClickEvent = RESET;
DMA2D_HandleTypeDef DMA2D_Handle;
uint32_t StartTime = 0;
uint32_t EndTime = 0;
__IO uint32_t PropagationTime = 0;
char message[16];

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Config_FDCAN(void);
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize);
static void Display_Init(uint32_t step);
static void Display_PropagationTime(uint32_t step);
static void Flush_Rx_Buffers(void);

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

  /* This project calls firstly two functions in order to configure MPU feature
  and to enable the CPU Cache, respectively MPU_Config() and CPU_CACHE_Enable() */

  /* Configure the MPU attributes as Write Through */
  MPU_Config();

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

  /* wait until CPU2 wakes up from stop mode */
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }

  /* Configure User push-button in interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize and configure the display */
  Display_Init(FIRST_RUN);

  /* - Initialize FDCAN and configure
     - Rx filters
     - Tx message headers
  */
  Config_FDCAN();

  /* Start the FDCAN module */
  HAL_FDCAN_Start(&hfdcan);

  /* Wait for event on User push button request Tx start */
  while ((UserButtonClickEvent) == RESET){;}

  /* Get tick */
  StartTime = HAL_GetTick();

  /* Request transmission of first 16 messages */
  HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);

  while(idx < ImageSize)
  {
    if(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) != 0)
    {
      /* Retreive Rx messages from RX FIFO0 */
      HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, &RxData[idx]);
      idx += 64; /* Data length per message is 64 bytes */
      counter++;
    }

    if((counter == 16) && (idx < ImageSize))
    {
      /* Request transmission of next 16 messages */
      HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
      counter = 0;
    }
  }

  /* Measure propagation time.
     This is actually the whole communication process. It includes:
      - Adding messages to Tx FIFO
      - Sending time
      - Retrieving messages from Rx FIFO
      - Sending Tx request every 16 messages (synchronization phase)
  */
  EndTime = HAL_GetTick();
  PropagationTime = EndTime - StartTime;
  sprintf(message,"BRS OFF : %lu ms", PropagationTime);

  /* Clean Data Cache */
  SCB_CleanDCache_by_Addr((uint32_t *)RxData, ImageSize);

  /* Display received image */
  DMA2D_CopyBuffer((uint32_t *)RxData, (uint32_t *)LCD_FRAME_BUFFER, 6, 29, 230, 172);

  /* Display propagation time */
  Display_PropagationTime(FIRST_RUN);

  /* Reset counters */
  idx = 0;
  counter = 0;
  UserButtonClickEvent = RESET;

  Flush_Rx_Buffers();

  /* Enable Bit Rate Switching, using TxHeader2 */
  HAL_FDCAN_AddMessageToTxBuffer(&hfdcan, &TxHeader2, NULL, FDCAN_TX_BUFFER0);

  /* Update display for second run */
  Display_Init(SECOND_RUN);

  /* Wait for event on User push button request Tx start */
  while ((UserButtonClickEvent) == RESET){;}

  /* Get tick */
  StartTime = HAL_GetTick();

  /* Request transmission of first 16 messages */
  HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);

  while(idx < ImageSize)
  {
    if(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) != 0)
    {
      /* Retreive Rx messages from RX FIFO0 */
      HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, &RxData[idx]);
      idx += 64; /* Data length per message is 64 bytes */
      counter++;
    }

    if(counter == 16)
    {
      /* Request transmission of next 16 messages */
      HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
      counter = 0;
    }
  }

  /* Measure propagation time.
     This is actually the whole communication process. It includes:
      - Adding messages to Tx FIFO
      - Sending time
      - Retrieving messages from Rx FIFO
      - Sending Tx request every 16 messages (synchronization phase)
  */
  EndTime = HAL_GetTick();
  PropagationTime = EndTime - StartTime;
  sprintf(message,"BRS ON : %lu ms", PropagationTime);

  /* Clean Data Cache */
  SCB_CleanDCache_by_Addr((uint32_t *)RxData, ImageSize);

  /* Display received image */
  DMA2D_CopyBuffer((uint32_t *)RxData, (uint32_t *)LCD_FRAME_BUFFER, 243, 29, 230, 172);

  /* Display propagation time */
  Display_PropagationTime(SECOND_RUN);

  /* Update display for second run */
  Display_Init(END_TEST);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Initialize FDCAN modules and configure Rx filters and Tx messages
  * @param  None
  * @retval None
  */
static void Config_FDCAN(void)
{
  /*                     Bit time configuration:
    Bit time parameter         | Nominal      |  Data
    ---------------------------|--------------|------------------
    fdcan_ker_ck               | 80 MHz       | 80 MHz
    Time_quantum (tq)          | 12.5 ns      | 12.5 ns
    Synchronization_segment    | 1 tq         | 1 tq
    Propagation_segment        | 47 tq        | 1 tq
    Phase_segment_1            | 16 tq        | 4 tq
    Phase_segment_2            | 16 tq        | 4 tq
    Synchronization_Jump_width | 16 tq        | 4 tq
    Bit_length                 | 80 tq = 1 µs | 10 tq = 0.125 µs
    Bit_rate                   | 1 MBit/s     | 8 MBit/s
  */
  hfdcan.Instance = FDCANx;
  hfdcan.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
  hfdcan.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan.Init.AutoRetransmission = ENABLE;
  hfdcan.Init.TransmitPause = DISABLE;
  hfdcan.Init.ProtocolException = ENABLE;
  hfdcan.Init.NominalPrescaler = 0x1; /* tq = NominalPrescaler x (1/fdcan_ker_ck) */
  hfdcan.Init.NominalSyncJumpWidth = 0x10;
  hfdcan.Init.NominalTimeSeg1 = 0x3F; /* NominalTimeSeg1 = Propagation_segment + Phase_segment_1 */
  hfdcan.Init.NominalTimeSeg2 = 0x10;
  hfdcan.Init.DataPrescaler = 0x1;
  hfdcan.Init.DataSyncJumpWidth = 0x4;
  hfdcan.Init.DataTimeSeg1 = 0x5; /* DataTimeSeg1 = Propagation_segment + Phase_segment_1 */
  hfdcan.Init.DataTimeSeg2 = 0x4;
  hfdcan.Init.MessageRAMOffset = 1250;
  hfdcan.Init.StdFiltersNbr = 1;
  hfdcan.Init.ExtFiltersNbr = 0;
  hfdcan.Init.RxFifo1ElmtsNbr = 0;
  hfdcan.Init.TxEventsNbr = 0;
  hfdcan.Init.RxBuffersNbr = 0;
  hfdcan.Init.RxFifo0ElmtsNbr = 16;
  hfdcan.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_64;
  hfdcan.Init.TxBuffersNbr = 1;
  hfdcan.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan.Init.TxFifoQueueElmtsNbr = 0;

  /* Initializes the FDCAN module */
  HAL_FDCAN_Init(&hfdcan);

  /* Configure Rx filter to receive image data */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x001;
  sFilterConfig.FilterID2 = 0x7FF; /* For acceptance, MessageID and FilterID1 must match exactly */
  HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig);

  /* Configure Tx synchronization message with Bit Rate Switching Off*/
  TxHeader.Identifier = 0x111;
  TxHeader.IdType = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader.DataLength = FDCAN_DLC_BYTES_0;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader.FDFormat = FDCAN_FD_CAN;
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0;
  HAL_FDCAN_AddMessageToTxBuffer(&hfdcan, &TxHeader, NULL, FDCAN_TX_BUFFER0);

 /* Fill TxHeader2 for Tx synchronization message with Bit Rate Switching ON*/
  TxHeader2.Identifier = 0x111;
  TxHeader2.IdType = FDCAN_STANDARD_ID;
  TxHeader2.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader2.DataLength = FDCAN_DLC_BYTES_0;
  TxHeader2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader2.BitRateSwitch = FDCAN_BRS_ON;
  TxHeader2.FDFormat = FDCAN_FD_CAN;
  TxHeader2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader2.MessageMarker = 0;

  /* Configure and enable Tx Delay Compensation : TdcOffset = DataTimeSeg1*DataPrescaler */
  HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan, 5, 0);
  HAL_FDCAN_EnableTxDelayCompensation(&hfdcan);
}

/**
  * @brief EXTI line detection callback
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    /* Report User push button event to main program */
    UserButtonClickEvent = SET;
  }
}

/**
  * @brief Reset RxData table
  * @param None
  * @retval None
  */
static void Flush_Rx_Buffers(void)
{
  uint32_t Rx_Idx;

  for (Rx_Idx = 0; Rx_Idx < ImageSize; Rx_Idx++)
  {
    RxData[Rx_Idx] = 0;
  }
}

/**
  * @brief  Copy the Decoded image to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  x: destination Horizontal offset.
  * @param  y: destination Vertical offset.
  * @param  xsize: image width
  * @param  ysize: image Height
  * @retval None
  */
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{
  uint32_t destination = 0;
  BSP_LCD_GetXSize(0, &LCD_X_Size);
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = LCD_X_Size - xsize; 
  DMA2D_Handle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  DMA2D_Handle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */  

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  DMA2D_Handle.LayerCfg[1].ChromaSubSampling = 0;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;
  DMA2D_Handle.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  DMA2D_Handle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */  

  DMA2D_Handle.Instance          = DMA2D; 

  /*##-4- DMA2D Initialization################################################*/
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);

  /*##-5- Copy the new decoded frame to the LCD Frame buffer #################*/
  destination = (uint32_t)pDst + ((y * LCD_X_Size) + x) * 4;

  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, xsize, ysize);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 25);  /* wait for the previous DMA2D transfer to end */
}

/**
  * @brief  Initialize LCD and configure display header
  * @param  step: current run
  * @retval None
  */
static void Display_Init(uint32_t step)
{
  if(step == FIRST_RUN)
  {
    /* Initialize and configure the LCD */
    BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
    UTIL_LCD_SetFuncDriver(&LCD_Driver);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  }

  BSP_LCD_GetXSize(0, &LCD_X_Size);
  
  /* Set description */
  UTIL_LCD_FillRect(0, 0, LCD_X_Size, 23, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_DisplayStringAt(0, 2, (uint8_t *)"FDCAN image transmission", CENTER_MODE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetFont(&Font12);

  if(step == FIRST_RUN)
  {
    UTIL_LCD_DisplayStringAt(359, 75, (uint8_t *)"Press User push-button", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(359, 105, (uint8_t *)"to request image transmission", CENTER_MODE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_DisplayStringAt(359, 135, (uint8_t *)"Without Bit Rate Switching", CENTER_MODE);
  }
  else if(step == SECOND_RUN)
  {
    UTIL_LCD_DisplayStringAt(122, 75, (uint8_t *)"Press again User push-button to", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(122, 105, (uint8_t *)"request image transmission with", CENTER_MODE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGREEN);
    UTIL_LCD_DisplayStringAt(122, 135, (uint8_t *)"Bit Rate Switching activated", CENTER_MODE);
  }
}

/**
  * @brief  Display time measurement results
  * @param  step: current run
  * @retval None
  */
static void Display_PropagationTime(uint32_t step)
{
  uint32_t Xpos, Color;

  if(step == FIRST_RUN)
  {
    Xpos = 360;
    Color = UTIL_LCD_COLOR_RED;
  }
  else
  {
    Xpos = 120;
    Color = UTIL_LCD_COLOR_DARKGREEN;
  }

  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetTextColor(Color);
  UTIL_LCD_DisplayStringAt(Xpos, 205, (uint8_t *)message, CENTER_MODE);

  if(step == FIRST_RUN)
  {
    BSP_LCD_GetXSize(0, &LCD_X_Size);
    UTIL_LCD_FillRect(0, 225, LCD_X_Size, 47, UTIL_LCD_COLOR_LIGHTGRAY);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGRAY);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_LIGHTGRAY);
    UTIL_LCD_SetFont(&Font12);

    UTIL_LCD_DisplayStringAt(0, 227, (uint8_t *)"The communication process includes :", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(02, 243, (uint8_t *)"Adding messages to TxFIFO, Message propagation, Retrieving messages", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(02, 259, (uint8_t *)"from RxFIFO and Synchronization (sending request every 16 messages)", LEFT_MODE);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Cortex-M7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
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

  /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED2);

  while(1)
  {}
}

/**
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is SDRAM_DEVICE_ADDR
  *         The Configured Region Size is 32MB because same as SDRAM size.
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
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

