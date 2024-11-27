/**
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to send/receive bytes over SPI IP using
  *          the STM32H7xx SPI LL API.
  *          Peripheral initialization done using LL initialization function.
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

/** @addtogroup STM32H7xx_LL_Examples
  * @{
  */

/** @addtogroup SPI_OneBoard_HalfDuplex_DMA_Init
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
LL_SPI_InitTypeDef spi_initstruct;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t ubButtonPress = 0;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = "***** SPI_OneBoard_HalfDuplex_DMA_Init communication **** SPI_OneBoard_HalfDuplex_DMA_Init communication **** SPI_OneBoard_HalfDuplex_DMA_Init communication  *****";
uint8_t ubNbDataToTransmit = sizeof(aTxBuffer);
__IO uint8_t ubTransmissionComplete = 0;

/* Buffer used for reception */
/* Buffer location and size should aligned to cache line size (32 bytes) */
#if defined ( __ICCARM__ )
#pragma location = 0x38000000
uint8_t aRxBuffer[sizeof(aTxBuffer)];
#elif defined ( __CC_ARM )
uint8_t __attribute__((section (".RAM_D3"))) aRxBuffer[sizeof(aTxBuffer)];
#elif defined ( __GNUC__ )
uint8_t __attribute__((section (".RAM_D3"))) aRxBuffer[sizeof(aTxBuffer)];
#endif /* __ICCARM__ */

uint8_t ubNbDataToReceive = sizeof(aTxBuffer);
__IO uint8_t ubReceptionComplete = 0;

/* Private function prototypes -----------------------------------------------*/
static void  SystemClock_Config(void);
void     Configure_DMA(void);
void     Configure_SPI1(void);
void     Configure_SPI6(void);
void     Activate_SPI1(void);
void     Activate_SPI6(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
void     LED_Off(void);
void     UserButton_Init(void);
void     WaitForUserButtonPress(void);
void     WaitAndCheckEndOfTransfer(void);
uint8_t  Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength);
static void CPU_CACHE_Enable(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /* Configure the SPI1 parameters */
  Configure_SPI1();

  /* Configure the SPI6 parameters */
  Configure_SPI6();

  /* Configure DMA channels for the SPI1  */
  Configure_DMA();

  /* Initialize User push-button in EXTI mode */
  UserButton_Init();

  /* Enable the SPI6 peripheral */
  Activate_SPI6();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Enable the SPI1 peripheral */
  Activate_SPI1();

  /* Wait for the end of the transfer and check received data */
  /* LED blinking FAST during waiting time */
  WaitAndCheckEndOfTransfer();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures the DMA Channels for SPI1 and SPI6
  * @note  This function is used to :
  *        -1- Enable DMA1 and BDMA clock
  *        -2- Configure NVIC for DMA1 and BDMA transfer complete/error interrupts
  *        -3- Configure the DMA1_Stream7 functional parameters
  *        -4- Configure the BDMA_Channel0 functional parameters
  *        -5- Enable DMA1 and BDMA interrupts complete/error
  * @param   None
  * @retval  None
  */
void Configure_DMA(void)
{
  /* DMA1 used for SPI1 Transmission
   * BDMA used for SPI6 Reception
   */
  /* (1) Enable the clock of DMA1 and BDMA */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_BDMA);

 /* (2) Configure NVIC for DMA transfer complete/error interrupts */
  NVIC_SetPriority(DMA1_Stream7_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Stream7_IRQn);
  NVIC_SetPriority(BDMA_Channel0_IRQn, 0);
  NVIC_EnableIRQ(BDMA_Channel0_IRQn);

  /* (3) Configure the DMA1_Stream7 functional parameters */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_STREAM_7,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                        LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);
  LL_DMA_ConfigAddresses(DMA1, 
                         LL_DMA_STREAM_7,
                         (uint32_t)aTxBuffer, (uint32_t) &(SPI1->TXDR),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_7));

  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_7, ubNbDataToTransmit);
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_7, LL_DMAMUX1_REQ_SPI1_TX);

  /* (4) Configure the BDMA_Channel0 functional parameters */
  LL_BDMA_ConfigTransfer(BDMA,
                        LL_BDMA_CHANNEL_0,
                        LL_BDMA_DIRECTION_PERIPH_TO_MEMORY | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL |
                        LL_BDMA_PERIPH_NOINCREMENT | LL_BDMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);
  LL_BDMA_ConfigAddresses(BDMA, LL_BDMA_CHANNEL_0,(uint32_t) &(SPI6->RXDR), (uint32_t)aRxBuffer,
	LL_BDMA_GetDataTransferDirection(BDMA, LL_BDMA_CHANNEL_0));
  LL_BDMA_SetDataLength(BDMA, LL_BDMA_CHANNEL_0, ubNbDataToTransmit);
  LL_BDMA_SetPeriphRequest(BDMA, LL_BDMA_CHANNEL_0, LL_DMAMUX2_REQ_SPI6_RX);


  /* (5) Enable DMA interrupts complete/error */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_7);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_7);
  LL_BDMA_EnableIT_TC(BDMA, LL_BDMA_CHANNEL_0);
  LL_BDMA_EnableIT_TE(BDMA, LL_BDMA_CHANNEL_0);
}

/**
  * @brief This function configures SPI1.
  * @note  This function is used to :
  *        -1- Enables GPIO clock and configures the SPI1 pins.
  *        -2- Configure SPI1 functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_SPI1(void)
{
  /* (1) Enables GPIO clock and configures the SPI1 pins ********************/
  /* Enable the peripheral clock of GPIOB */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

  /* Configure SCK Pin connected to pin 23 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_3, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_DOWN);

  /* Configure MOSI Pin connected to pin 22 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_5, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);

  /* (2) Configure SPI1 functional parameters ********************************/
  /* Enable the peripheral clock of GPIOB */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  /* Configure SPI1 communication */
  spi_initstruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV256;
  spi_initstruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
  spi_initstruct.ClockPhase        = LL_SPI_PHASE_2EDGE;
  spi_initstruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
  spi_initstruct.BitOrder          = LL_SPI_MSB_FIRST;
  spi_initstruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
  spi_initstruct.NSS               = LL_SPI_NSS_SOFT;
  spi_initstruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
  spi_initstruct.Mode              = LL_SPI_MODE_MASTER;

  /* Initialize SPI instance according to parameters defined in initialization structure. */  
  if (LL_SPI_Init(SPI1, &spi_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Configure SPI1 DMA transfer interrupts */
  /* Enable DMA TX Interrupt */
  LL_SPI_EnableDMAReq_TX(SPI1);
}

/**
  * @brief  This function configures SPI6.
  * @note  This function is used to :
  *        -1- Enables GPIO clock and configures the SPI6 pins.
  *        -2- Configure SPI6 functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_SPI6(void)
{
  /* (1) Enables GPIO clock and configures the SPI6 pins ********************/

  /* Enable the peripheral clock of GPIOA */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

  /* Configure SCK Pin connected to pin 13 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_8);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);

  /* Configure MISO Pin connected to pin 12 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_6, LL_GPIO_AF_8);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);

  /* (2) Configure SPI6 functional parameters ********************************/
  /* Enable the peripheral clock of GPIOA */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SPI6);

  /* Configure SPI6 communication */
  spi_initstruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV256;
  spi_initstruct.TransferDirection = LL_SPI_HALF_DUPLEX_RX;
  spi_initstruct.ClockPhase        = LL_SPI_PHASE_2EDGE;
  spi_initstruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
  spi_initstruct.BitOrder          = LL_SPI_MSB_FIRST;
  spi_initstruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
  spi_initstruct.NSS               = LL_SPI_NSS_SOFT;
  spi_initstruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
  spi_initstruct.Mode              = LL_SPI_MODE_SLAVE;

  /* Initialize SPI instance according to parameters defined in initialization structure. */  
  if (LL_SPI_Init(SPI6, &spi_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Configure SPI1 DMA transfer interrupts */
  /* Enable DMA RX Interrupt */
  LL_SPI_EnableDMAReq_RX(SPI6);
}

/**
  * @brief  This function Activate SPI1
  * @param  None
  * @retval None
  */
void Activate_SPI1(void)
{
  /* Enable SPI1 */
  LL_SPI_Enable(SPI1);
  /* Enable DMA Stream Tx */
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_7);
  /* Master transfer start */
  LL_SPI_StartMasterTransfer(SPI1);
}

/**
  * @brief  This function Activate SPI6
  * @param  None
  * @retval None
  */
void Activate_SPI6(void)
{
  /* Enable SPI6 */
  LL_SPI_Enable(SPI6);
  /* Enable DMA Channels Rx */
  LL_BDMA_EnableChannel(BDMA, LL_BDMA_CHANNEL_0);
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Turn-off LED1.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle LED1 in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();

  /* Enable a rising trigger External line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 0x03);
}

/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None
  * @retval None
  */
  /*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Ensure that LED1 is turned Off */
  LED_Off();
}

/**
  * @brief  Wait end of transfer and check if received Data are well.
  * @param  None
  * @retval None
  */
void WaitAndCheckEndOfTransfer(void)
{
  /* 1 - Wait end of transmission */
  while (ubTransmissionComplete != 1)
  {
  }
  /* Disable DMA1 Tx Channel */
  LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_7);
  /* 2 - Wait end of reception */
  while (ubReceptionComplete != 1)
  {
  }
  /* Disable BDMA Rx Channel */
  LL_BDMA_DisableChannel(BDMA, LL_BDMA_CHANNEL_0);
  /* 3 - Compare Transmit data to receive data */
  if(Buffercmp8((uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, ubNbDataToTransmit))
  {
    /* Processing Error */
    LED_Blinking(LED_BLINK_ERROR);
  }
  else
  {
    /* Turn On Led if data are well received */
    LED_On();
  }
}

/**
* @brief Compares two 8-bit buffers and returns the comparison result.
* @param pBuffer1: pointer to the source buffer to be compared to.
* @param pBuffer2: pointer to the second source buffer to be compared to the first.
* @param BufferLength: buffer's length.
* @retval   0: Comparison is OK (the two Buffers are identical)
*           Value different from 0: Comparison is NOK (Buffers are different)
*/
uint8_t Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL1 (HSE BYPASS)
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
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(260);
  LL_RCC_PLL1_SetP(1);
  LL_RCC_PLL1_SetQ(4);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set Sys & AHB & APB1 & APB2 & APB4  prescaler */
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

  /* Set PLL1 as System Clock Source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(520000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 520000000;
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT Functions                                     */
/******************************************************************************/
/**
  * @brief  Function to manage User push-button
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  Function called from DMA1 IRQ Handler when Tx transfer is completed
  * @param  None
  * @retval None
  */
void DMA1_TransmitComplete_Callback(void)
{
  /* DMA Tx transfer completed */
  ubTransmissionComplete = 1;
}

/**
  * @brief  Function called from BDMA IRQ Handler when Rx transfer is completed
  * @param  None
  * @retval None
  */
void BDMA_ReceiveComplete_Callback(void)
{
  /* DMA Rx transfer completed */
  ubReceptionComplete = 1;
}

/**
  * @brief  Function called in case of error detected in SPI IT Handler
  * @param  None
  * @retval None
  */
void SPI_TransferError_Callback(void)
{
  /* Disable DMA1 Tx Channel */
  LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_7);
    /* Disable BDMA Rx Channel */
  LL_BDMA_DisableChannel(BDMA, LL_BDMA_CHANNEL_0);

  /* Set LED1 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
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
  * @}
  */

/**
  * @}
  */

