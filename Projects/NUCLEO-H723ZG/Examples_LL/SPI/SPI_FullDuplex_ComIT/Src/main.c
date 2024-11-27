/**
******************************************************************************
* @file    SPI_FullDuplex_ComIT/Src/main.c
* @author  MCD Application Team
* @brief   Main program body through the LL API
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

/** @addtogroup SPI_FullDuplex_ComIT
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t timeout = 0;

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
static void     Error_Handler(void);
static void     LED_OK_Init(void);
static void     LED_ERROR_Init(void);
static void     GPIOPins_SPI1_Config(void);
static void     GPIOPins_SPI6_Config(void);
static void     SPI1_Config (void);
static void     SPI6_Config (void);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{
  /* Configure the system clock to 520 MHz */
  SystemClock_Config();

  /* Initialize LED_OK */
  LED_OK_Init();

  /* Initialize LED_ERROR */
  LED_ERROR_Init();

  /* Configure GPIO Pins of SPI1 */
  GPIOPins_SPI1_Config();

  /* Configure GPIO Pins of SPI6 */
  GPIOPins_SPI6_Config();

  /* Configure SPI1 Parametres */
  SPI1_Config ();

  /* Configure SPI6 Parametres */
  SPI6_Config ();

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
      /* Turn On LED_ERROR: incorrect received data */
      Error_Handler();
    }
    /* 4 - Compare Transmit data to received data : Slave to Master ***********/
    if(BufferCmp(SPIx_TxBuffer, SPI1_RxBuffer, SPIx_NbDataToTransmit))
    {
      /* Turn On LED_ERROR: incorrect received data */
      Error_Handler();
    }
  }
  else
  {
      /* Turn On LED_ERROR: Transfer error */
    Error_Handler();    
  }

  /* Communication done with success : Turn On LED_OK  */
  LL_GPIO_SetOutputPin(LED_OK_GPIO_PORT, LED_OK_PIN);

  /* Infinite loop */
  while (1)
  {
  }
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
  SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_HIGH;
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
  SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_HIGH;
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
  * @brief  Initialize LED_OK .
  * @param  None
  * @retval None
  */
void LED_OK_Init(void)
{
  /* Enable the LED1 Clock */
  LED_OK_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED_OK */
  LL_GPIO_SetPinMode(LED_OK_GPIO_PORT, LED_OK_PIN, LL_GPIO_MODE_OUTPUT);
}

/**
  * @brief  Initialize LED_ERROR.
  * @param  None
  * @retval None
  */
void LED_ERROR_Init(void)
{
  /* Enable the LED_ERROR Clock */
  LED_ERROR_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED_ERROR */
  LL_GPIO_SetPinMode(LED_ERROR_GPIO_PORT, LED_ERROR_PIN, LL_GPIO_MODE_OUTPUT);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED_ERROR on */
  LL_GPIO_SetOutputPin(LED_ERROR_GPIO_PORT, LED_ERROR_PIN);
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

  /* Update SPI1 Transfer Complete glogal variable*/
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

  /* Update SPI6 Transfer Complete glogal variable*/
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

  /* Update Xfr Error detection glogal variable*/
  SPIx_XfrErrorDetect = 1;
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

