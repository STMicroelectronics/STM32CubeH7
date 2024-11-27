/**
  ******************************************************************************
  * @file    PWR/PWR_Domain3SystemControl/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to retain data in the D3 SRAM
  *          memory when system enters STOP mode and D1 enters DSTANDBY mode.
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

/** @addtogroup PWR_Domain3SystemControl
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define I2C4_WAKEUP_EXTI_LINE             EXTI_LINE25
#define I2C4_EVENT_INTERRUPT_EXTI_LINE    EXTI_LINE62
#define I2C4_ERROR_INTERRUPT_EXTI_LINE    EXTI_LINE63
#define I2C4_WAKEUP_EXTI_MODE             (EXTI_MODE_IT | EXTI_MODE_EVT)
#define BUFFER_SIZE                       20U
#define TX_CHUNK_SIZE                     4U
#define BDMA_CH0_DMAMUX_REQUEST_NUMBER    1U
#define BDMA_CH7_DMAMUX_REQUEST_NUMBER    4U
#define DMA_TRANSFER_NUMBER               2U
#define DMA_TRANSFER_NO_ERROR             0U
#define I2C_ADDRESS                       0x72U
#define I2Cx_TIMING                       0x00D00E28U  /* (Rise time = 120ns, Fall time = 25ns) */
#define I2C_WAKEUP_CLEAR_FLAG_ADDRESS     (uint32_t *)0x38000400U
#define I2C_DELAY                         0xFFF

/* Private macro -------------------------------------------------------------*/
#define I2C_ENABLE_DMA_REQUEST(__HANDLE__)  (SET_BIT((__HANDLE__)->Instance->CR1,  I2C_CR1_RXDMAEN))

/* Private variables ---------------------------------------------------------*/
__IO  uint32_t DMA_TransferErrorFlag    = 0;
__IO  uint32_t DMA_TransferCompleteFlag = 0;

#if defined ( __CC_ARM )  /* !< ARM Compiler */
  uint8_t aRxBuffer[BUFFER_SIZE] __attribute__((at(0x38000000)));
#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
  #pragma location=0x38000000
  uint8_t aRxBuffer[BUFFER_SIZE];
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
  uint8_t aRxBuffer[BUFFER_SIZE] __attribute__((section(".RAM_D3")));
#endif

uint8_t aTxBuffer[BUFFER_SIZE] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'
};

DMA_HandleTypeDef  BDMA_Handle0, BDMA_Handle7;
I2C_HandleTypeDef  I2cHandle;
uint32_t *I2C_WakeupClearFlag = (uint32_t *)0x38000400;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void I2C_Config(void);
static void Flush_Buffer(uint8_t *buffer, uint32_t size);
static void CPU_CACHE_Enable(void);
#ifdef LOW_POWER_BOARD
static void DMA_TransferErrorCallback(DMA_HandleTypeDef *hdma);
static void DMA_TransferCompleteCallback(DMA_HandleTypeDef *hdma);
#else
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif /* LOW_POWER_BOARD */

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
 
  /* -1- Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* When system initialization is finished, Cortex-M7 will release (wakeup) Cortex-M4  by means of 
     HSEM notification. Cortex-M4 release could be also ensured by any Domain D2 wakeup source (SEV,EXTI..).
  */

  /* HW semaphore Clock enable */
  __HAL_RCC_HSEM_CLK_ENABLE();

  /* Take HSEM */
  HAL_HSEM_FastTake(HSEM_ID_0);  
  
  /* Release HSEM in order to notify the CPU2(CM4) */     
  HAL_HSEM_Release(HSEM_ID_0,0);
  
  /* After Domain D2 wakeup, if  CPU1 attempts to use any resource from such a domain, 
     the access will not be safe until “clock ready flag” of such a domain is set (by hardware).  */
  while(!__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY));

  /* -2- Configure LED1 */
  BSP_LED_Init(LED1);

#if defined(LOW_POWER_BOARD)

  HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams;
  HAL_DMA_MuxSyncConfigTypeDef dmamux_syncParams;

  /* -3- Set LED1 On to inform the user that the system is start */
  BSP_LED_On(LED1);

  /* -4- Enable BDMA clock */
  __HAL_RCC_BDMA_CLK_ENABLE();

  /* -5- Configure the BDMA channle0 as I2C wakeup clear source */
  BDMA_Handle0.Instance                  = BDMA_Channel0;
  BDMA_Handle0.Init.Request              = DMA_REQUEST_GENERATOR0;
  BDMA_Handle0.Init.Direction            = DMA_MEMORY_TO_PERIPH;
  BDMA_Handle0.Init.PeriphInc            = DMA_PINC_DISABLE;
  BDMA_Handle0.Init.MemInc               = DMA_MINC_DISABLE;
  BDMA_Handle0.Init.PeriphDataAlignment  = DMA_PDATAALIGN_WORD;
  BDMA_Handle0.Init.MemDataAlignment     = DMA_MDATAALIGN_WORD;
  BDMA_Handle0.Init.Mode                 = DMA_NORMAL;
  BDMA_Handle0.Init.Priority             = DMA_PRIORITY_HIGH;
  BDMA_Handle0.Init.FIFOMode             = DMA_FIFOMODE_ENABLE;
  BDMA_Handle0.Init.FIFOThreshold        = DMA_FIFO_THRESHOLD_FULL;
  BDMA_Handle0.Init.MemBurst             = DMA_MBURST_SINGLE;
  BDMA_Handle0.Init.PeriphBurst          = DMA_PBURST_SINGLE;
  BDMA_Handle0.XferCpltCallback          = DMA_TransferCompleteCallback;
  BDMA_Handle0.XferErrorCallback         = DMA_TransferErrorCallback;

  if(HAL_DMA_Init(&BDMA_Handle0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select Callbacks functions called after Transfer complete and Transfer error */
  if(HAL_DMA_RegisterCallback(&BDMA_Handle0, HAL_DMA_XFER_CPLT_CB_ID, DMA_TransferCompleteCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_DMA_RegisterCallback(&BDMA_Handle0, HAL_DMA_XFER_ERROR_CB_ID, DMA_TransferErrorCallback)  != HAL_OK)
  {
    Error_Handler();
  }

  /* NVIC configuration for DMA_Handle7 transfer complete interrupt */
  HAL_NVIC_SetPriority(BDMA_Channel0_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(BDMA_Channel0_IRQn);

  /* -6- Configure the DMAMUX request generator */
  dmamux_ReqGenParams.SignalID  = HAL_DMAMUX2_REQ_GEN_I2C4_WKUP;  /* External request signal is I2c4_it_async */
  dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_RISING;      /* External request signal edge is Rising  */
  dmamux_ReqGenParams.RequestNumber = BDMA_CH0_DMAMUX_REQUEST_NUMBER; /* 1 request on each edge of the external request signal  */
  if(HAL_DMAEx_ConfigMuxRequestGenerator(&BDMA_Handle0, &dmamux_ReqGenParams) != HAL_OK)
  {
    Error_Handler();
  }

  /* NVIC configuration for DMAMUX request generator overrun errors */
  HAL_NVIC_SetPriority(DMAMUX2_OVR_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMAMUX2_OVR_IRQn);

  /* -7-Configure the BDMA channle7 as D3 penclear source */
  BDMA_Handle7.Instance                  = BDMA_Channel7;
  BDMA_Handle7.Init.Request              = BDMA_REQUEST_I2C4_RX;
  BDMA_Handle7.Init.Direction            = DMA_PERIPH_TO_MEMORY;
  BDMA_Handle7.Init.PeriphInc            = DMA_PINC_DISABLE;
  BDMA_Handle7.Init.MemInc               = DMA_MINC_ENABLE;
  BDMA_Handle7.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
  BDMA_Handle7.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
  BDMA_Handle7.Init.Mode                 = DMA_NORMAL;
  BDMA_Handle7.Init.Priority             = DMA_PRIORITY_HIGH;
  BDMA_Handle7.Init.FIFOMode             = DMA_FIFOMODE_ENABLE;
  BDMA_Handle7.Init.FIFOThreshold        = DMA_FIFO_THRESHOLD_FULL;
  BDMA_Handle7.Init.MemBurst             = DMA_MBURST_SINGLE;
  BDMA_Handle7.Init.PeriphBurst          = DMA_PBURST_SINGLE;
  BDMA_Handle7.XferCpltCallback          = DMA_TransferCompleteCallback;
  BDMA_Handle7.XferErrorCallback         = DMA_TransferErrorCallback;

  if(HAL_DMA_Init(&BDMA_Handle7) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select Callbacks functions called after Transfer complete and Transfer error */
  if(HAL_DMA_RegisterCallback(&BDMA_Handle7, HAL_DMA_XFER_CPLT_CB_ID, DMA_TransferCompleteCallback) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_DMA_RegisterCallback(&BDMA_Handle7, HAL_DMA_XFER_ERROR_CB_ID, DMA_TransferErrorCallback)  != HAL_OK)
  {
    Error_Handler();
  }

  /* NVIC configuration for DMA_Handle7 transfer complete interrupt */
  HAL_NVIC_SetPriority(BDMA_Channel7_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(BDMA_Channel7_IRQn);

  /* -8- Configure the DMAMUX synchronisation */
  /* Configure the DMAMUX with the Synchronization parameters for BDMA_Handle7 */
  dmamux_syncParams.EventEnable   = ENABLE; /* Enable DMAMUX event generation each time  RequestNumber are passed from DMAMUX to the DMA */
  dmamux_syncParams.SyncPolarity  = HAL_DMAMUX_SYNC_RISING; /* Synchronization edge is Rising  */
  dmamux_syncParams.RequestNumber = BDMA_CH7_DMAMUX_REQUEST_NUMBER; /* 4 request are autorized after each edge of the sync signal */
  dmamux_syncParams.SyncSignalID  = HAL_DMAMUX2_SYNC_DMAMUX2_CH0_EVT; /* Sync signal is dmamux1_ch0_evt */
  dmamux_syncParams.SyncEnable    = DISABLE;  /* Synchronization is disabled */

  if(HAL_DMAEx_ConfigMuxSync(&BDMA_Handle7, &dmamux_syncParams)  != HAL_OK)
  {
    Error_Handler();
  }

  /* -9- Activate autonomous bit for SRAM1, BDAM and I2C4 peripherals */
  __HAL_RCC_I2C4_CLKAM_ENABLE();
  __HAL_RCC_BDMA_CLKAM_ENABLE();
  __HAL_RCC_D3SRAM1_CLKAM_ENABLE();

  /* -10- Configure the I2C4 EXTI lines */
  /* Deactivate all I2C4 interrupts and events */
  HAL_EXTI_D1_EventInputConfig(I2C4_WAKEUP_EXTI_LINE, I2C4_WAKEUP_EXTI_MODE, DISABLE);
  HAL_EXTI_D1_EventInputConfig(I2C4_EVENT_INTERRUPT_EXTI_LINE, I2C4_WAKEUP_EXTI_MODE, DISABLE);
  HAL_EXTI_D1_EventInputConfig(I2C4_ERROR_INTERRUPT_EXTI_LINE, I2C4_WAKEUP_EXTI_MODE, DISABLE);

  /* Enable DAM1 Channel 7 interrupt */
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE73, I2C4_WAKEUP_EXTI_MODE, ENABLE);

  /* Disable DAM1 Channel 0 interrupt */
  HAL_EXTI_D1_EventInputConfig(EXTI_LINE66, I2C4_WAKEUP_EXTI_MODE, DISABLE);

  /* -11- Insert 5 seconds delay before entering Stop mode */
  HAL_Delay(5000);

  while(1)
  {
    /* -12- Initialize DMA flags */
    DMA_TransferCompleteFlag = 0;
    DMA_TransferErrorFlag    = 0;

    /* Flush I2C Rx buffer */
    Flush_Buffer((uint8_t*)&aRxBuffer, BUFFER_SIZE);

    /* -13- Configure BDMA Channel7 event as D3 domain pendclear source  */
    HAL_EXTI_D3_EventInputConfig(I2C4_WAKEUP_EXTI_LINE, DISABLE, BDMA_CH7_CLEAR);
    HAL_EXTI_D3_EventInputConfig(I2C4_WAKEUP_EXTI_LINE, ENABLE, BDMA_CH7_CLEAR);

    /* -14- Configure the I2C4 peripheral */
    I2C_Config();

    /* -15- Enable the DMAMUX request generator */
    if(HAL_DMAEx_EnableMuxRequestGenerator (&BDMA_Handle0) != HAL_OK)
    {
      Error_Handler();
    }

    /* -16- Start the BDMA Channel0 in interrupt mode to clear the I2C address flag */
    *I2C_WAKEUP_CLEAR_FLAG_ADDRESS = I2C_ICR_ADDRCF;

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)I2C_WAKEUP_CLEAR_FLAG_ADDRESS, 4);

    HAL_DMA_Start_IT(&BDMA_Handle0, (uint32_t)I2C_WAKEUP_CLEAR_FLAG_ADDRESS, (uint32_t)&(I2cHandle.Instance->ICR), (BUFFER_SIZE/TX_CHUNK_SIZE));

    /* -17- Start the BDMA Channel 7 in interrupt mode to receive I2C RX data */
    if(HAL_DMA_Start_IT(&BDMA_Handle7, (uint32_t)&(I2cHandle.Instance->RXDR),(uint32_t)&(aRxBuffer), BUFFER_SIZE)  != HAL_OK)
    {
      Error_Handler();
    }

    /* -18- Enable I2C DMA Request */
    I2C_ENABLE_DMA_REQUEST(&I2cHandle);

    /* -19- Clear I2C Address matched ISR */
    __HAL_I2C_CLEAR_FLAG(&I2cHandle, I2C_FLAG_ADDR);

    /* -20- Set LED1 Off and enter Stop mode */
    BSP_LED_Off(LED1);

    /* Enter the system in STOP mdoe */
    HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

    /* -21- Check the results after wake-up */
    /* The transfer number must be equal to one because we used one DMA.
       If not the case then goto  Error_Handler (LED2 will turn on) */
    if(DMA_TransferCompleteFlag != DMA_TRANSFER_NUMBER)
    {
      Error_Handler();
    }

    /* If a DMA transfer ERROR occurred then goto Error_Handler (LED2 will turn on) */
    if(DMA_TransferErrorFlag != 0)
    {
      Error_Handler();
    }

    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t *)aRxBuffer, BUFFER_SIZE);

    /* -22- Send back the received data when the system wake-up without errors */
    if(HAL_I2C_Slave_Transmit(&I2cHandle, (uint8_t*)&aRxBuffer, BUFFER_SIZE, I2C_DELAY) != HAL_OK)
    {
      Error_Handler();
    }

    /* -23- Disable the DMAMUX request generator */
    HAL_DMAEx_DisableMuxRequestGenerator(&BDMA_Handle0);
  }

#else

  uint32_t bufferIndex = 0;

  /* -3- Configure the I2C4 peripheral */
  I2C_Config();

  /* -4- Toggle LED1 then waiting for user to press button */
  BSP_LED_On(LED1);

  /* -5- Configure User Key */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* -6- Wait for User Key press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) == RESET)
  {
  }

  /* -7- Wait for User Key to be release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) == SET)
  {
  }

  /* The board sends the message and expects to receive it back */
  while(1)
  {
    /* Flush I2C Rx buffer */
    Flush_Buffer(aRxBuffer, BUFFER_SIZE);

    /* -8- Transmit the aTxBuffer data */
    if(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)(aTxBuffer + bufferIndex), TX_CHUNK_SIZE, I2C_DELAY) != HAL_OK)
    {
      Error_Handler();
    }

    bufferIndex += TX_CHUNK_SIZE;

    if(bufferIndex >= BUFFER_SIZE)
    {
      /* -9- Receive the I2C4 data */
      if(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&aRxBuffer, BUFFER_SIZE, I2C_DELAY) != HAL_OK)
      {
        Error_Handler();
      }

      /* -10- Check the received data */
      if(Buffercmp(aRxBuffer, aTxBuffer, BUFFER_SIZE))
      {
        Error_Handler();
      }

      bufferIndex = 0;

      /* -11- Toggle LED1 each 100 ms if the transmission and reception are passed */
      HAL_Delay(100);
      BSP_LED_Toggle(LED1);
    }
  }

#endif  /* LOW_POWER_BOARD */
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Domain1 frequency)
  *            HCLK(Hz)                       = 20000000  (Domain2 frequency)
  *            D1AHB Prescaler                = 1
  *            APB3 Prescaler                 = 1
  *            D2APB1 Prescaler               = 1
  *            APB2 Prescaler                 = 1
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
* @brief  Configure and enable I2C4.
* @param  None
* @retval None
*/
static void I2C_Config(void)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  HAL_StatusTypeDef         ret = HAL_OK;

  /* -1- Enable the HSI clock */
  __HAL_RCC_HSI_ENABLE();
  RCC_OscInitStruct.OscillatorType  =  RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;
  RCC_OscInitStruct.HSIState        = RCC_HSI_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* -2- Set the HSI as I2C4 clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C4;
  PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* -3- Configure the I2C4 peripheral*/
  I2cHandle.Instance             = I2Cx;        /* I2C4 */

  I2cHandle.Init.Timing          = I2Cx_TIMING;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.OwnAddress2     = 0xFF;
  I2cHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_DeInit(&I2cHandle)  != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_I2CEx_DisableWakeUp(&I2cHandle)  != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable the I2C4 wakeup mode */
  if(HAL_I2CEx_EnableWakeUp(&I2cHandle))
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Flush buffer.
  * @param  buffer: Buffer to flush.
  * @param  size: Size of the buffer to flush.
  * @retval None
*/
static void Flush_Buffer(uint8_t *buffer, uint32_t size)
{
  uint32_t index;

  for (index = 0; index < size; index++)
  {
    buffer[index] = 0;
  }

  /* Clean Data Cache to update the content of the SRAM */
  SCB_CleanDCache_by_Addr((uint32_t*)buffer, size);
}

#ifdef LOW_POWER_BOARD
/**
  * @brief  DMA Transfer Complete callback
  * @param  DMA_HandleTypeDef: DMA handle
  * @retval None
  */
static void DMA_TransferCompleteCallback(DMA_HandleTypeDef *hdma)
{
  DMA_TransferCompleteFlag++;
}

/**
  * @brief  DMA Transfer Error callback
  * @param  DMA_HandleTypeDef: DMA handle
  * @retval None
  */
static void DMA_TransferErrorCallback(DMA_HandleTypeDef *hdma)
{
  DMA_TransferErrorFlag++;
}

#else

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
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

#endif /* LOW_POWER_BOARD */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED1 on */
  BSP_LED_On(LED1);
  while(1)
  {
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

