/**
  ******************************************************************************
  * @file    FDCAN_Image_transmission/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body for Cortex-M4.
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
#define HSEM_ID_0 (0U) /* HW semaphore 0*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FDCAN_HandleTypeDef hfdcan;
FDCAN_FilterTypeDef sFilterConfig;
uint32_t idx = 0;
uint32_t counter = 0;
FDCAN_TxHeaderTypeDef TxHeader;
uint8_t *Image_Converted = (uint8_t *)Image;
uint32_t i;

/* Private function prototypes -----------------------------------------------*/
static void Config_FDCAN(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

  /* 
    Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
    perform system initialization (system clock config, external memory configuration.. )   
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

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

  /* - Initialize FDCAN and configure
     - Rx filters
     - Tx message headers
  */
  Config_FDCAN();

  /* Start the FDCAN module */
  HAL_FDCAN_Start(&hfdcan);

  for(i = 0; i < 2; i++)
  {
    /* Wait for transmission request */
    while(HAL_FDCAN_IsRxBufferMessageAvailable(&hfdcan, FDCAN_RX_BUFFER0) == 0){;}

    /* Send image by packets of 16 messages */
    while (idx < ImageSize)
    {
      for(counter = 0; counter < 16; counter++)
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
  hfdcan.Init.MessageRAMOffset = 0;
  hfdcan.Init.StdFiltersNbr = 1;
  hfdcan.Init.ExtFiltersNbr = 0;
  hfdcan.Init.RxFifo1ElmtsNbr = 0;
  hfdcan.Init.TxEventsNbr = 0;
  hfdcan.Init.RxBuffersNbr = 1;
  hfdcan.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan.Init.RxFifo0ElmtsNbr = 0;
  hfdcan.Init.TxBuffersNbr = 0;
  hfdcan.Init.TxFifoQueueElmtsNbr = 16;
  hfdcan.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan.Init.TxElmtSize = FDCAN_DATA_BYTES_64;

  /* Initializes the FDCAN module */
  HAL_FDCAN_Init(&hfdcan);

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

  /* Configure and enable Tx Delay Compensation : TdcOffset = DataTimeSeg1*DataPrescaler */
  HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan, 5, 0);
  HAL_FDCAN_EnableTxDelayCompensation(&hfdcan);
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

