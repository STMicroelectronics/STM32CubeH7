/**
  ******************************************************************************
  * @file    FDCAN_Image_transmission/Src/main.c 
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
#include "Image.h"
#include "stdio.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup FDCAN_Image_transmission
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define FDCAN_receiver

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan;
FDCAN_FilterTypeDef sFilterConfig;
uint32_t idx = 0;
uint32_t counter = 0;
FDCAN_TxHeaderTypeDef TxHeader;
#ifdef FDCAN_receiver
  FDCAN_RxHeaderTypeDef RxHeader;
  ALIGN_32BYTES (uint8_t RxData[ImageSize]);
  __IO uint8_t UserButtonClickEvent = RESET;
  DMA2D_HandleTypeDef DMA2D_Handle;
  uint32_t StartTime = 0;
  uint32_t EndTime = 0;
  __IO uint32_t PropagationTime = 0;
  char s[10];
#else
  uint8_t *Image_Converted = (uint8_t *)Image;
  uint32_t i;
#endif

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void Config_FDCAN(void);
#ifdef FDCAN_receiver
  static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize);
  static void Display_Init(uint32_t step);
  static void Display_PropagationTime(uint32_t step);
  static void Flush_Rx_Buffers(void);
#endif

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
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
  */
  HAL_Init();
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
#ifdef FDCAN_receiver
  /* Configure Tamper push-button in interrupt mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  
  /* Initialize and configure the display */
  Display_Init(FIRST_RUN);
#endif
  
  /* - Initialize FDCAN and configure
     - Rx filters
     - Tx message headers
  */
  Config_FDCAN();
  
  /* Start the FDCAN module */
  HAL_FDCAN_Start(&hfdcan);
  
#ifdef FDCAN_receiver
  
  /* Wait for event on Tamper push button request Tx start */
  while ((UserButtonClickEvent) == RESET){;}
  
  /* Get tick */
  StartTime = HAL_GetTick();
  
  /* Request transmission of first 32 messages */
  HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
  
  while(idx < ImageSize)
  {
    if(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) != 0)
    {
      /* Retrieve Rx messages from RX FIFO0 */
      HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, &RxData[idx]);
      idx += 64; /* Data length per message is 64 bytes */
      counter++;
    }
    
    if((counter == 32) && (idx < ImageSize))
    {
      /* Request transmission of next 32 messages */
      HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
      counter = 0;
    }
  }
  
  /* Measure propagation time.
     This is actually the whole communication process. It includes:
      - Adding messages to Tx FIFO
      - Sending time
      - Retrieving messages from Rx FIFO
      - Sending Tx request every 32 messages (synchronization phase)
  */
  EndTime = HAL_GetTick();
  PropagationTime = EndTime - StartTime;
  sprintf(s,"%ld", PropagationTime);
  
  /* Clean Data Cache */
  SCB_CleanDCache_by_Addr((uint32_t *)RxData, ImageSize);
  
  /* Display received image */
  DMA2D_CopyBuffer((uint32_t *)RxData, (uint32_t *)LCD_FRAME_BUFFER, 0, 85, 320, 240);
  
  /* Display propagation time */
  Display_PropagationTime(FIRST_RUN);
  
  /* Reset counters */
  idx = 0;
  counter = 0;
  UserButtonClickEvent = RESET;
  
  Flush_Rx_Buffers();
  
  /* Enable Bit Rate Switching */
  TxHeader.BitRateSwitch = FDCAN_BRS_ON;
  HAL_FDCAN_AddMessageToTxBuffer(&hfdcan, &TxHeader, NULL, FDCAN_TX_BUFFER0);
  
  /* Update display for second run */
  Display_Init(SECOND_RUN);
  
  /* Wait for event on Tamper push button request Tx start */
  while ((UserButtonClickEvent) == RESET){;}
  
  /* Get tick */
  StartTime = HAL_GetTick();
  
  /* Request transmission of first 32 messages */
  HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
  
  while(idx < ImageSize)
  {
    if(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan, FDCAN_RX_FIFO0) != 0)
    {
      /* Retrieve Rx messages from RX FIFO0 */
      HAL_FDCAN_GetRxMessage(&hfdcan, FDCAN_RX_FIFO0, &RxHeader, &RxData[idx]);
      idx += 64; /* Data length per message is 64 bytes */
      counter++;
    }
    
    if(counter == 32)
    {
      /* Request transmission of next 32 messages */
      HAL_FDCAN_EnableTxBufferRequest(&hfdcan, FDCAN_TX_BUFFER0);
      counter = 0;
    }
  }
  
  /* Measure propagation time.
     This is actually the whole communication process. It includes:
      - Adding messages to Tx FIFO
      - Sending time
      - Retrieving messages from Rx FIFO
      - Sending Tx request every 32 messages (synchronization phase)
  */
  EndTime = HAL_GetTick();
  PropagationTime = EndTime - StartTime;
  sprintf(s,"%ld", PropagationTime);
  
  /* Clean Data Cache */
  SCB_CleanDCache_by_Addr((uint32_t *)RxData, ImageSize);
  
  /* Display received image */
  DMA2D_CopyBuffer((uint32_t *)RxData, (uint32_t *)LCD_FRAME_BUFFER, 320, 85, 320, 240);
  
  /* Display propagation time */
  Display_PropagationTime(SECOND_RUN);
  
  /* Update display for second run */
  Display_Init(END_TEST);
  
#else
  
  for(i = 0; i < 2; i++)
  {
    /* Wait for transmission request */
    while(HAL_FDCAN_IsRxBufferMessageAvailable(&hfdcan, FDCAN_RX_BUFFER0) == 0){;}
    
    /* Send image by packets of 32 messages */
    while (idx < ImageSize)
    {
      for(counter = 0; counter < 32; counter++)
      {
        if(idx >= ImageSize)
        {
          break;
        }
        
        HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan, &TxHeader, &Image_Converted[idx]);
        
        idx += 64;
      }
      
      if(idx >= ImageSize)
      {
        break;
      }
      
      /* Wait for synchronization message */
      while(HAL_FDCAN_IsRxBufferMessageAvailable(&hfdcan, FDCAN_RX_BUFFER0) == 0){;}
    }
    
    /* Reset counters */
    idx = 0;
    
    /* Enable Bit Rate Switching */
    TxHeader.BitRateSwitch = FDCAN_BRS_ON;
  }
  
#endif  
  
  /* Infinite loop */
  while (1)
  {}
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
    Propagation_segment        | 47 tq        | 11 tq
    Phase_segment_1            | 16 tq        | 4 tq
    Phase_segment_2            | 16 tq        | 4 tq
    Synchronization_Jump_width | 16 tq        | 4 tq
    Bit_length                 | 80 tq = 1 µs | 20 tq = 0.25 µs
    Bit_rate                   | 1 MBit/s     | 4 MBit/s
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
  hfdcan.Init.DataTimeSeg1 = 0xF; /* DataTimeSeg1 = Propagation_segment + Phase_segment_1 */
  hfdcan.Init.DataTimeSeg2 = 0x4;
  hfdcan.Init.MessageRAMOffset = 0;
  hfdcan.Init.StdFiltersNbr = 1;
  hfdcan.Init.ExtFiltersNbr = 0;
  hfdcan.Init.RxFifo1ElmtsNbr = 0;
  hfdcan.Init.TxEventsNbr = 0;
#ifdef FDCAN_receiver
  hfdcan.Init.RxBuffersNbr = 0;
  hfdcan.Init.RxFifo0ElmtsNbr = 32;
  hfdcan.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_64;
  hfdcan.Init.TxBuffersNbr = 1;
  hfdcan.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan.Init.TxFifoQueueElmtsNbr = 0;
#else
  hfdcan.Init.RxBuffersNbr = 1;
  hfdcan.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan.Init.RxFifo0ElmtsNbr = 0;
  hfdcan.Init.TxBuffersNbr = 0;
  hfdcan.Init.TxFifoQueueElmtsNbr = 32;
  hfdcan.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan.Init.TxElmtSize = FDCAN_DATA_BYTES_64;
#endif
  
  /* Initializes the FDCAN module */
  HAL_FDCAN_Init(&hfdcan);
  
#ifdef FDCAN_receiver
  /* Configure Rx filter to receive image data */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x001;
  sFilterConfig.FilterID2 = 0x7FF; /* For acceptance, MessageID and FilterID1 must match exactly */
  HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig);
  
  /* Configure Tx synchronization message */
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
#else
  /* Configure Rx filter to receive synchronization message */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXBUFFER;
  sFilterConfig.FilterID1 = 0x111;
  sFilterConfig.RxBufferIndex = 0;
  sFilterConfig.IsCalibrationMsg = 0;
  HAL_FDCAN_ConfigFilter(&hfdcan, &sFilterConfig);
  
  /* Prepare Tx image data message header */
  TxHeader.Identifier = 0x001;
  TxHeader.IdType = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader.DataLength = FDCAN_DLC_BYTES_64;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader.FDFormat = FDCAN_FD_CAN;
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0;
#endif
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
  uint32_t x_size;
  BSP_LCD_GetXSize(0, &x_size);
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = x_size - xsize; 
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
  destination = (uint32_t)pDst + ((y * x_size) + x) * 4;

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
  uint32_t x_size, y_size;

  if(step == FIRST_RUN)
  {
    /* Initialize and configure the LCD */
    BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
    UTIL_LCD_SetFuncDriver(&LCD_Driver);    
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  }

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  
  /* Set description */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  if(step != END_TEST)
  {
    UTIL_LCD_SetFont(&Font24);
    UTIL_LCD_DisplayStringAt(0, 2, (uint8_t *)"FDCAN image transmission", CENTER_MODE);
    UTIL_LCD_SetFont(&Font12);
  }
  if(step == FIRST_RUN)
  {
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(0, 35, (uint8_t *)"Press Tamper push-button to request image transmission", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 57, (uint8_t *)"Without Bit Rate Switching", CENTER_MODE);
  }
  else if(step == SECOND_RUN)
  {
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(0, 35, (uint8_t *)"Press again Tamper push-button to request image transmission", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 57, (uint8_t *)"With Bit Rate Switching activated", CENTER_MODE);
  }
  else
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font24);
    UTIL_LCD_DisplayStringAt(0, 33, (uint8_t *)"FDCAN image transmission", CENTER_MODE);
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
  uint8_t *BRSstate;
  
  uint32_t x_size;
  BSP_LCD_GetXSize(0, &x_size);
  
  if(step == FIRST_RUN)
  {
    Xpos = 480;
    Color = UTIL_LCD_COLOR_RED;
    BRSstate = (uint8_t *)"[BRS disabled]";
  }
  else
  {
    Xpos = 160;
    Color = UTIL_LCD_COLOR_DARKGREEN;
    BRSstate = (uint8_t *)"[BRS enabled]";
  }
  
 UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetTextColor(Color);
  UTIL_LCD_DisplayStringAt(Xpos, 330, BRSstate, CENTER_MODE);
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_DisplayStringAt(Xpos, 355, (uint8_t *)"Communication process (ms)", CENTER_MODE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetTextColor(Color);
  UTIL_LCD_DisplayStringAt(Xpos, 375, (uint8_t *)s, CENTER_MODE);
  
  if(step == FIRST_RUN)
  {
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_LIGHTGRAY);
    UTIL_LCD_FillRect(0, 400, x_size, 80, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGRAY);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_LIGHTGRAY);
    UTIL_LCD_SetFont(&Font12);
    
    UTIL_LCD_DisplayStringAt(30, 425, (uint8_t *)"The communication", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(30, 443, (uint8_t *)"process includes:", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(220, 407, (uint8_t *)"- Adding messages to Tx FIFO", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(220, 425, (uint8_t *)"- Transmission (message propagation)", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(220, 443, (uint8_t *)"- Retrieving messages from Rx FIFO", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(220, 461, (uint8_t *)"- Synchronization (sending Tx request every 32 messages)", LEFT_MODE);
  }
}

#endif

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
  
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();    
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
  {}
}

/**
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is 0xD0000000.
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

