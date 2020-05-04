/**
******************************************************************************
* @file    SPI_FullDuplex_ComIT/Src/main.c
* @author  MCD Application Team
* @brief   Main program body through the LL API
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H7xx_LL_Examples
* @{
*/

/** @addtogroup SPI_FullDuplex_ComIT
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t timeout = 0;
__IO uint8_t ubButtonPress = 0;

/* SPI Init Structure */
LL_SPI_InitTypeDef   SPI_InitStruct;

/* GPIO Init Structure */
LL_GPIO_InitTypeDef  GPIO_InitStruct;

uint8_t    SPIx_TxBuffer[] = "**** SPI_OneBoard_IT communication **** SPI_OneBoard_IT communication **** SPI_OneBoard_IT communication ****";
uint32_t   SPIx_NbDataToTransmit = ((sizeof(SPIx_TxBuffer)/ sizeof(*SPIx_TxBuffer)) - 1);

uint8_t    SPI1_RxBuffer[sizeof(SPIx_TxBuffer)];
uint32_t   SPI1_ReceiveIndex = 0;
uint32_t   SPI1_TransmitIndex = 0;

uint8_t    SPI6_RxBuffer[sizeof(SPIx_TxBuffer)];
uint32_t   SPI6_ReceiveIndex = 0;
uint32_t   SPI6_TransmitIndex = 0;

__IO uint32_t SPI1_XfrCompleteDetect = 0;
__IO uint32_t SPI6_XfrCompleteDetect = 0;

__IO uint32_t SPIx_XfrErrorDetect = 0;

/* Private function prototypes -----------------------------------------------*/
static void     SystemClock_Config(void);
static uint16_t BufferCmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
static void     LED_Init(void);
static void     LED_Blinking(uint32_t Period);
static void     LED_On(void);
static void     LED_Off(void);
static void     UserButton_Init(void);
static void     WaitForUserButtonPress(void);
static void     GPIOPins_SPI1_Config(void);
static void     GPIOPins_SPI6_Config(void);
static void     SPI1_Config (void);
static void     SPI6_Config (void);
static void     CPU_CACHE_Enable(void);
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{
  timeout = 0xFF;

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /* Initialize User push-button in EXTI mode */
  UserButton_Init();

  /* Configure GPIO Pins of SPI1 */
  GPIOPins_SPI1_Config();

  /* Configure GPIO Pins of SPI6 */
  GPIOPins_SPI6_Config();

  /* Configure SPI1 Parameters */
  SPI1_Config ();

  /* Configure SPI6 Parameters */
  SPI6_Config ();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* 1 - Start Master Transfer(SPI1) ******************************************/
  LL_SPI_StartMasterTransfer(SPI1);

  /* 2 - Wait end of transfer *************************************************/
  /* Wait for the end of the transfer on (SPI1 And SPI6) or transfer error    */
  while(((SPI1_XfrCompleteDetect == 0) || (SPI6_XfrCompleteDetect == 0)) && (SPIx_XfrErrorDetect == 0))
  {
  }

  if(SPIx_XfrErrorDetect == 0)
  {
    /* 3 - Compare Transmit data to received data : Master to Slave ***********/
    if(BufferCmp(SPIx_TxBuffer, SPI6_RxBuffer, SPIx_NbDataToTransmit))
    {
    /* Set LED1 to slow blinking mode to indicate incorrect received data */
    LED_Blinking(LED_BLINK_ERROR);
    }
    /* 4 - Compare Transmit data to received data : Slave to Master ***********/
    if(BufferCmp(SPIx_TxBuffer, SPI1_RxBuffer, SPIx_NbDataToTransmit))
    {
    /* Set LED1 to slow blinking mode to indicate incorrect received data */
    LED_Blinking(LED_BLINK_ERROR);
    }
  }
  else
  {
    /* Set LED1 to slow blinking mode to indicate error occurs */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Communication done with success : Turn On LED1  */
  LED_On();

  /* Infinite loop */
  while (1)
  {
  }
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
  /* Power Configuration */
  LL_PWR_ConfigSupply(LL_PWR_DIRECT_SMPS_SUPPLY);
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

/**
  * @brief  Configure GPIO Pins of SPI1.
  * @param  None
  * @retval None
  */
static void GPIOPins_SPI1_Config(void)
{
  /* (1) Enables GPIO clock and configures the SPI1 pins *******************/
  /* Enable the peripherals clocks of GPIOs */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

  /* SPI1 SCK GPIO pin configuration*/
  GPIO_InitStruct.Pin       = SPIx_MASTER_SCK_PIN;
  GPIO_InitStruct.Mode      = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Pull      = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Speed     = LL_GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = SPIx_MASTER_SCK_AF;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
    /* SPI1 MISO GPIO pin configuration*/
  GPIO_InitStruct.Pin       = SPIx_MASTER_MISO_PIN;
  GPIO_InitStruct.Alternate = SPIx_MASTER_MISO_AF;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
    /* SPI1 MOSI GPIO pin configuration*/
  GPIO_InitStruct.Pin       = SPIx_MASTER_MOSI_PIN;
  GPIO_InitStruct.Alternate = SPIx_MASTER_MOSI_AF;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* (2) Configure NVIC for SPI1 transfer complete/error interrupts **********/
  /* Set priority for SPI1_IRQn */
  NVIC_SetPriority(SPI1_IRQn, 0);
  /* Enable SPI1_IRQn */
  NVIC_EnableIRQ(SPI1_IRQn);
}

/**
  * @brief  Configure GPIO Pins of SPI6.
  * @param  None
  * @retval None
  */
static void GPIOPins_SPI6_Config(void)
{
  /* (1) Enables GPIO clock and configures the SPI6 pins ********************/
  /* Enable the peripherals clocks of GPIOs */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

  /* SPI6 SCK GPIO pin configuration*/
  GPIO_InitStruct.Pin       = SPIx_SLAVE_SCK_PIN;
  GPIO_InitStruct.Mode      = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Pull      = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Speed     = LL_GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = SPIx_SLAVE_SCK_AF;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
    /* SPI6 MISO GPIO pin configuration*/
  GPIO_InitStruct.Pin       = SPIx_SLAVE_MISO_PIN;
  GPIO_InitStruct.Alternate = SPIx_SLAVE_MISO_AF;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
    /* SPI6 MOSI GPIO pin configuration*/
  GPIO_InitStruct.Pin       = SPIx_SLAVE_MOSI_PIN;
  GPIO_InitStruct.Alternate = SPIx_SLAVE_MOSI_AF;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* (2) Configure NVIC for SPI6 transfer complete/error interrupts **********/
  /* Set priority for SPI6_IRQn */
  NVIC_SetPriority(SPI6_IRQn, 0);
  /* Enable SPI1_IRQn           */
  NVIC_EnableIRQ(SPI6_IRQn);
}

/**
  * @brief  Configure SPI1 parameters.
  * @param  None
  * @retval None
  */
static void SPI1_Config (void)
{
  /* Configure SPI MASTER ****************************************************/
  /* Enable SPI1 Clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  /* Configure the SPI1 parameters */
  SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV256;
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
  SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
  SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;

  LL_SPI_Init(SPI1, &SPI_InitStruct);

  /* Lock GPIO for master to avoid glitches on the clock output */
  LL_SPI_EnableGPIOControl(SPI1);
  LL_SPI_EnableMasterRxAutoSuspend(SPI1);

  /* Set number of date to transmit */
  LL_SPI_SetTransferSize(SPI1, SPIx_NbDataToTransmit);

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
  * @brief  Configure SPI6 parameters.
  * @param  None
  * @retval None
  */
static void SPI6_Config (void)
{
  /* Configure SPI SLAVE *****************************************************/
  /* Enable SPI6 Clock */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SPI6);

  /* Configure the SPI6 parameters */
  SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV256;
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
  SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
  SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.Mode              = LL_SPI_MODE_SLAVE;

  LL_SPI_Init(SPI6, &SPI_InitStruct);

  /* Lock GPIO for master to avoid glitches on the clock output */
  LL_SPI_DisableGPIOControl(SPI6);
  LL_SPI_DisableMasterRxAutoSuspend(SPI6);

  /* Set number of date to transmit */
  LL_SPI_SetTransferSize(SPI6, SPIx_NbDataToTransmit);

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
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t BufferCmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
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
  *         Function is in charge of retrieving received byte from SPI lines.
  * @param  None
  * @retval None
  */
void  SPI1_Rx_Callback(void)
{
  /* Read character in Data register.
   * RXP flag is cleared by reading data in RXDR register */
  SPI1_RxBuffer[SPI1_ReceiveIndex++] = LL_SPI_ReceiveData8(SPI1);
}

/**
  * @brief  Function called from SPI1 IRQ Handler when TXP flag is set
  *         Function is in charge  to transmit byte on SPI lines.
  * @param  None
  * @retval None
  */
void  SPI1_Tx_Callback(void)
{
  /* Write character in Data register.
   * TXP flag is cleared by filling data into TXDR register */
  LL_SPI_TransmitData8(SPI1, SPIx_TxBuffer[SPI1_TransmitIndex++]);
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
  
  /* Update SPI1 Transfer Complete global variable*/
  SPI1_XfrCompleteDetect = 1;
}

/**
  * @brief  Function called from SPI6 IRQ Handler when RXP flag is set
  *         Function is in charge of retrieving received byte from SPI lines.
  * @param  None
  * @retval None
  */
void  SPI6_Rx_Callback(void)
{
  /* Read character in Data register.
   * RXP flag is cleared by reading data in RXDR register */
  SPI6_RxBuffer[SPI6_ReceiveIndex++] = LL_SPI_ReceiveData8(SPI6);
}

/**
  * @brief  Function called from SPI6 IRQ Handler when TXP flag is set
  *         Function is in charge  to transmit byte on SPI lines.
  * @param  None
  * @retval None
  */
void  SPI6_Tx_Callback(void)
{
  /* Write character in Data register.
   * TXP flag is cleared by filling data into TXDR register */
  LL_SPI_TransmitData8(SPI6, SPIx_TxBuffer[SPI6_TransmitIndex++]);
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
  
  /* Update SPI6 Transfer Complete global variable*/
  SPI6_XfrCompleteDetect = 1;
}

/**
  * @brief  Function called in case of error detected in SPI IT Handler
  * @param  None
  * @retval None
  */
void SPI_TransferError_Callback(void)
{

  /* Disable ALL Interrupts */
  LL_SPI_DisableIT_TXP(SPI1);
  LL_SPI_DisableIT_RXP(SPI1);
  LL_SPI_DisableIT_CRCERR(SPI1);
  LL_SPI_DisableIT_OVR(SPI1);
  LL_SPI_DisableIT_UDR(SPI1);
  LL_SPI_DisableIT_EOT(SPI1);

  /* Disable SPI1 */
  LL_SPI_Disable(SPI1);

  /* Disable ALL Interrupts */
  LL_SPI_DisableIT_TXP(SPI6);
  LL_SPI_DisableIT_RXP(SPI6);
  LL_SPI_DisableIT_CRCERR(SPI6);
  LL_SPI_DisableIT_OVR(SPI6);
  LL_SPI_DisableIT_UDR(SPI6);
  LL_SPI_DisableIT_EOT(SPI6);

  /* Disable SPI6 */
  LL_SPI_Disable(SPI6);
  
  /* Update Xfr Error detection global variable*/
  SPIx_XfrErrorDetect = 1;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
