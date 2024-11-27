/**
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_OneBoard_FullDuplex_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to send/receive bytes over SPI IP using
  *          the STM32H7xx SPI LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup SPI_OneBoard_FullDuplex_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t ubButtonPress = 0;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = "**** SPI_OneBoard_FullDuplex_IT communication **** SPI_OneBoard_FullDuplex_IT communication **** SPI_OneBoard_FullDuplex_IT communication ****";
uint8_t ubNbDataToTransmit = sizeof(aTxBuffer);
__IO uint8_t SPI1_ubTransmitIndex = 0;
__IO uint8_t SPI6_ubTransmitIndex = 0;

/* Buffer used for reception */
uint8_t SPI1_aRxBuffer[sizeof(aTxBuffer)];
uint8_t SPI6_aRxBuffer[sizeof(aTxBuffer)];
uint8_t ubNbDataToReceive = sizeof(aTxBuffer);
__IO uint8_t SPI1_ubReceiveIndex = 0;
__IO uint8_t SPI6_ubReceiveIndex = 0;

/* Private function prototypes -----------------------------------------------*/
static void     SystemClock_Config(void);
void     Configure_SPI1(void);
void     Configure_SPI6(void);
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
  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /* Configure and enable the SPI6 */
  Configure_SPI6();

  /* Configure and enable the SPI1 */
  Configure_SPI1();

  /* Initialize User push-button in EXTI mode */
  UserButton_Init();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();



  /* 1 - Start Master Transfer(SPI1) ******************************************/
  LL_SPI_StartMasterTransfer(SPI1);

  /* Wait for the end of the transfer and check received data */
  /* LED blinking FAST during waiting time */
  WaitAndCheckEndOfTransfer();

  /* Infinite loop */
  while (1)
  {
  }
}


/**
  * @brief  This function configures SPI1.
  * @note  This function is used to :
  *        -1- Enables GPIO clock and configures the SPI1 pins.
  *        -2- Configure NVIC for SPI1.
  *        -3- Configure SPI1 functional parameters.
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

  /* Configure SCK Pin connected to pin D23 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_3, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_DOWN);

  /* Configure MISO Pin connected to pin D25 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_4, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_4, LL_GPIO_PULL_DOWN);

  /* Configure MOSI Pin connected to pin D22 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_5, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);

  /* (2) Configure NVIC for SPI1 transfer complete/error interrupts **********/
  /* Set priority for SPI1_IRQn */
  NVIC_SetPriority(SPI1_IRQn, 0);
  /* Enable SPI1_IRQn           */
  NVIC_EnableIRQ(SPI1_IRQn);

  /* (3) Configure SPI1 functional parameters ********************************/

  /* Enable the peripheral clock of SPI1 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  /* Configure SPI1 communication */
  LL_SPI_SetBaudRatePrescaler(SPI1,     LL_SPI_BAUDRATEPRESCALER_DIV256);
  LL_SPI_SetTransferDirection(SPI1,LL_SPI_FULL_DUPLEX);
  LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_2EDGE);
  /* Setup Internal SS signal level to avoid a MODF Error if NSS Polarity is LOW, NSS Mode is SOFT and SPI Mode is Master */
  LL_SPI_SetInternalSSLevel(SPI1, LL_SPI_SS_LEVEL_HIGH);
  LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);
  /* Reset value is LL_SPI_MSB_FIRST */
  LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
  LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
  LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);

  LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);
  /* Lock GPIO for master to avoid glitches on the clock output */
  LL_SPI_EnableGPIOControl(SPI1);
  LL_SPI_EnableMasterRxAutoSuspend(SPI1);

  /* Set number of data to transmit */
  LL_SPI_SetTransferSize(SPI1, ubNbDataToTransmit);

  /* Enable SPI1 */
  LL_SPI_Enable(SPI1);

  /* Enable TXP Interrupt */
  LL_SPI_EnableIT_TXP(SPI1);
  /* Enable RXP Interrupt */
  LL_SPI_EnableIT_RXP(SPI1);

  /* Enable SPI Errors Interrupt */
  LL_SPI_EnableIT_CRCERR(SPI1);
  LL_SPI_EnableIT_UDR(SPI1);
  LL_SPI_EnableIT_OVR(SPI1);
  LL_SPI_EnableIT_EOT(SPI1);
}

/**
  * @brief  This function configures SPI6.
  * @note  This function is used to :
  *        -1- Enables GPIO clock and configures the SPI6 pins.
  *        -2- Configure NVIC for SPI6.
  *        -3- Configure SPI6 functional parameters.
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

  /* Configure SCK Pin connected to pin D13 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_8);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);

  /* Configure MISO Pin connected to pin D12 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_6, LL_GPIO_AF_8);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);

  /* Configure MOSI Pin connected to pin D11 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_7, LL_GPIO_AF_8);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_DOWN);

  /* (2) Configure NVIC for SPI6 transfer complete/error interrupts **********/
  /* Set priority for SPI6_IRQn */
  NVIC_SetPriority(SPI6_IRQn, 0);
  /* Enable SPI6_IRQn           */
  NVIC_EnableIRQ(SPI6_IRQn);

  /* (3) Configure SPI6 functional parameters ********************************/

  /* Enable the peripheral clock of SPI6 */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SPI6);

  /* Configure SPI6 communication */
  LL_SPI_SetBaudRatePrescaler(SPI6,     LL_SPI_BAUDRATEPRESCALER_DIV256);
  LL_SPI_SetTransferDirection(SPI6,LL_SPI_FULL_DUPLEX);
  LL_SPI_SetClockPhase(SPI6, LL_SPI_PHASE_2EDGE);
  LL_SPI_SetClockPolarity(SPI6, LL_SPI_POLARITY_LOW);
  /* Reset value is LL_SPI_MSB_FIRST */
  LL_SPI_SetTransferBitOrder(SPI6, LL_SPI_MSB_FIRST);
  LL_SPI_SetDataWidth(SPI6, LL_SPI_DATAWIDTH_8BIT);
  LL_SPI_SetNSSMode(SPI6, LL_SPI_NSS_SOFT);

  /* Reset value is LL_SPI_MODE_SLAVE */
  LL_SPI_SetMode(SPI6, LL_SPI_MODE_SLAVE);
  /* Lock GPIO for master to avoid glitches on the clock output */
  LL_SPI_DisableGPIOControl(SPI6);
  LL_SPI_DisableMasterRxAutoSuspend(SPI6);

  /* Set number of data to transmit */
  LL_SPI_SetTransferSize(SPI6, ubNbDataToTransmit);

  /* Enable SPI6 */
  LL_SPI_Enable(SPI6);

  /* Enable TXP Interrupt */
  LL_SPI_EnableIT_TXP(SPI6);
  /* Enable RXP Interrupt */
  LL_SPI_EnableIT_RXP(SPI6);

  /* Enable SPI Errors Interrupt */
  LL_SPI_EnableIT_CRCERR(SPI6);
  LL_SPI_EnableIT_UDR(SPI6);
  LL_SPI_EnableIT_OVR(SPI6);
  LL_SPI_EnableIT_EOT(SPI6);
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
  /* 1 - Wait end of transmission : SPI1 to SPI6 */
  while (SPI1_ubTransmitIndex != ubNbDataToTransmit)
  {
  }
  /* Disable TXP Interrupt */
  LL_SPI_DisableIT_TXP(SPI1);

  /* 2 - Wait end of transmission : SPI6 to SPI1 */
  while (SPI6_ubTransmitIndex != ubNbDataToTransmit)
  {
  }
  /* Disable TXP Interrupt */
  LL_SPI_DisableIT_TXP(SPI6);


  /* 3 - Wait end of reception : SPI1 from SPI6 */
  while (ubNbDataToReceive > SPI1_ubReceiveIndex)
  {
  }
  /* Disable RXP Interrupt */
  LL_SPI_DisableIT_RXP(SPI1);

  /* 4 - Wait end of reception : SPI6 from SPI1 */
  while (ubNbDataToReceive > SPI6_ubReceiveIndex)
  {
  }
  /* Disable RXP Interrupt */
  LL_SPI_DisableIT_RXP(SPI6);


  /* 5 - Compare Transmit data to receive data in SPI1 and SPI6 */
  if( (Buffercmp8((uint8_t*)aTxBuffer, (uint8_t*)SPI1_aRxBuffer, ubNbDataToTransmit)) || (Buffercmp8((uint8_t*)aTxBuffer, (uint8_t*)SPI6_aRxBuffer, ubNbDataToTransmit)) )
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
  *            System Clock source            = PLL1 (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 280
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 6
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
  LL_RCC_HSE_SelectDigitalClock();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_6);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(4);
  LL_RCC_PLL1_SetN(280);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(2);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set Sys & AHB & APB1 & APB2 & APB4  prescaler */
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);

  /* Set PLL1 as System Clock Source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Set systick to 1ms */
  SysTick_Config(280000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 280000000;
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
  * @brief  Function called from SPI1 IRQ Handler when RXP flag is set
  *         Function is in charge of retrieving received byte from SPI1 lines.
  * @param  None
  * @retval None
  */
void  SPI1_Rx_Callback(void)
{
  /* Read character in Data register.
  RXP flag is cleared by reading data in DR register */
  SPI1_aRxBuffer[SPI1_ubReceiveIndex++] = LL_SPI_ReceiveData8(SPI1);
}

/**
  * @brief  Function called from SPI1 IRQ Handler when TXP flag is set
  *         Function is in charge  to transmit byte on SPI1 lines.
  * @param  None
  * @retval None
  */
void  SPI1_Tx_Callback(void)
{
  /* Write character in Data register.
  TXP flag is cleared by reading data in DR register */
  LL_SPI_TransmitData8(SPI1, aTxBuffer[SPI1_ubTransmitIndex++]);
}

/**
  * @brief  Function called in case of error detected in SPI1 IT Handler
  * @param  None
  * @retval None
  */
void SPI1_TransferError_Callback(void)
{
  /* Disable RXP  Interrupt             */
  LL_SPI_DisableIT_RXP(SPI1);

  /* Disable TXP   Interrupt             */
  LL_SPI_DisableIT_TXP(SPI1);

  /* Set LED1 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
}


/**
  * @brief  Function called from SPI1 IRQ Handler when EOT flag is set
  *         Function is in charge of transfer close down.
  * @param  None
  * @retval None
  */
void  SPI1_EOT_Callback(void)
{
  LL_SPI_Disable(SPI1);
  LL_SPI_DisableIT_TXP(SPI1);
  LL_SPI_DisableIT_RXP(SPI1);
  LL_SPI_DisableIT_CRCERR(SPI1);
  LL_SPI_DisableIT_OVR(SPI1);
  LL_SPI_DisableIT_UDR(SPI1);
  LL_SPI_DisableIT_EOT(SPI1);
}


/**
  * @brief  Function called from SPI6 IRQ Handler when RXP flag is set
  *         Function is in charge of retrieving received byte from SPI6 lines.
  * @param  None
  * @retval None
  */
void  SPI6_Rx_Callback(void)
{
  /* Read character in Data register.
  RXP flag is cleared by reading data in DR register */
  SPI6_aRxBuffer[SPI6_ubReceiveIndex++] = LL_SPI_ReceiveData8(SPI6);
}

/**
  * @brief  Function called from SPI6 IRQ Handler when TXP flag is set
  *         Function is in charge  to transmit byte on SPI6 lines.
  * @param  None
  * @retval None
  */
void  SPI6_Tx_Callback(void)
{
  /* Write character in Data register.
  TXP flag is cleared by reading data in DR register */
  LL_SPI_TransmitData8(SPI6, aTxBuffer[SPI6_ubTransmitIndex++]);
}

/**
  * @brief  Function called in case of error detected in SPI6 IT Handler
  * @param  None
  * @retval None
  */
void SPI6_TransferError_Callback(void)
{
  /* Disable RXP  Interrupt             */
  LL_SPI_DisableIT_RXP(SPI6);

  /* Disable TXP   Interrupt             */
  LL_SPI_DisableIT_TXP(SPI6);

  /* Set LED1 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
}

/**
  * @brief  Function called from SPI6 IRQ Handler when EOT flag is set
  *         Function is in charge of transfer close down.
  * @param  None
  * @retval None
  */
void  SPI6_EOT_Callback(void)
{
  LL_SPI_Disable(SPI6);
  LL_SPI_DisableIT_TXP(SPI6);
  LL_SPI_DisableIT_RXP(SPI6);
  LL_SPI_DisableIT_CRCERR(SPI6);
  LL_SPI_DisableIT_OVR(SPI6);
  LL_SPI_DisableIT_UDR(SPI6);
  LL_SPI_DisableIT_EOT(SPI6);
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

