/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComDMA/CM4/Src/main.c
  * @author  MCD Application Team
  * @brief   This is the main program for Cortex-M4.
  *          This sample code shows how to use STM32H7xx SPI HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          DMA transfer.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
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

/** @addtogroup SPI_FullDuplex_ComDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
enum {
  TRANSFER_WAIT,
  TRANSFER_COMPLETE,
  TRANSFER_ERROR
};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle4;

volatile uint32_t Notif_Recieved;
/* Buffer used for transmission */
const uint8_t aTxBuffer[] = "****SPI - Two Boards communication based on DMA **** SPI Message ********* SPI Message *********";

/* Buffer used for reception */
#define BUFFER_ALIGNED_SIZE (((BUFFERSIZE+31)/32)*32)
ALIGN_32BYTES(uint8_t aRxBuffer[BUFFER_ALIGNED_SIZE]);

/* transfer state */
__IO uint32_t wTransferState = TRANSFER_WAIT;

#define USE_HAL_SPI_REGISTER_CALLBACKS = 1U;

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
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

  /* Add Cortex-M4 user application code here */

    /*##-1- Configure the SPI peripheral #######################################*/
/* Set the SPI3 parameters */
  SpiHandle4.Instance               = SPI4;
  SpiHandle4.Init.Mode              = SPI_MODE_SLAVE;
  SpiHandle4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  SpiHandle4.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle4.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SpiHandle4.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SpiHandle4.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle4.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle4.Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle4.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle4.Init.CRCPolynomial     = 7;
  SpiHandle4.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
  SpiHandle4.Init.NSS               = SPI_NSS_SOFT;
  SpiHandle4.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
  SpiHandle4.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;  /* Recommended setting to avoid glitches */

  if(HAL_SPI_Init(&SpiHandle4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

    /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

    if(HAL_SPI_TransmitReceive_DMA(&SpiHandle4, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, BUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }

  while (wTransferState == TRANSFER_WAIT)
  {
  }

    switch(wTransferState)
  {
    case TRANSFER_COMPLETE :
      /*##-4- Compare the sent and received buffers ##############################*/
      if(Buffercmp((uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, BUFFERSIZE))
      {
        /* Processing Error */
        Error_Handler();
      }
      break;
    default :
      Error_Handler();
      break;
  }

  /* Infinite loop */
  while (1)
  {

  }
}



/**
  * @brief Semaphore Released Callback.
  * @param SemMask: Mask of Released semaphores
  * @retval None
  */
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  Notif_Recieved = 1;
}

/**
  * @brief  TxRx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of DMA TxRx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Turn LED1 on: Transfer in transmission process is complete */
  BSP_LED_On(LED1);
  /* Turn LED2 on: Transfer in reception process is complete */
  BSP_LED_On(LED2);
  wTransferState = TRANSFER_COMPLETE;
}


/**
  * @brief  SPI error callbacks.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  wTransferState = TRANSFER_ERROR;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_Off(LED1);
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

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
    if((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
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

