/**
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_IT_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to send/receive bytes over I2C IP using
  *          the STM32H7xx I2C LL API.
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

/** @addtogroup I2C_OneBoard_Communication_IT_Init
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
LL_I2C_InitTypeDef i2c_initstruct;

/* Private define ------------------------------------------------------------*/
/**
  * @brief I2C devices settings
  */
/* Timing register value is computed with the STM32CubeMX Tool,
  * Fast Mode @400kHz with I2CCLK = 520 MHz/4,
  * rise time = 100ns, fall time = 10ns
  * Timing Value = (uint32_t)0x70D04648
  */
#define I2C_TIMING               0x70D04648

/**
  * @brief Define related to SlaveTransmit process
  */
#define SLAVE_BYTE_TO_SEND       (uint8_t)0xA5

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO uint8_t ubButtonPress       = 0;

/**
  * @brief Variables related to MasterReceive process
  */
uint8_t      aReceiveBuffer[0xF] = {0};
__IO uint8_t ubReceiveIndex      = 0;

/* Private function prototypes -----------------------------------------------*/
static void     SystemClock_Config(void);
void     Configure_I2C_Slave(void);
void     Configure_I2C_Master(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     WaitForUserButtonPress(void);
void     Handle_I2C_Master(void);
void     UserButton_Init(void);
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

  /* Set LED1 Off */
  LED_Off();

  /* Initialize User push-button in EXTI mode */
  UserButton_Init();

  /* Configure I2C1 (I2C IP configuration in Slave mode and related GPIO initialization) */
  Configure_I2C_Slave();

  /* Configure I2C4 (I2C IP configuration in Master mode and related GPIO initialization) */
  Configure_I2C_Master();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Handle I2C4 events (Master) */
  Handle_I2C_Master();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures I2C1 in Slave mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock and configures the I2C1 pins.
  *         -2- Enable the I2C1 peripheral clock and I2C1 clock source.
  *         -3- Configure NVIC for I2C1.
  *         -4- Configure and Enable I2C1 functional parameters.
  *         -5- Enable I2C1 address match/error interrupts.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_I2C_Slave(void)
{
  /* (1) Enables GPIO clock and configures the I2C1 pins **********************/
  /*    (SCL on PB.8, SDA on PB.9)                     **********************/

  /* Enable the peripheral clock of GPIOB */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
#if (EXTERNAL_PULL_UP_AVAILABLE == 0)
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);
#else
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
#endif

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
#if (EXTERNAL_PULL_UP_AVAILABLE == 0)
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);
#else
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_NO);
#endif

  /* (2) Enable the I2C1 peripheral clock and I2C1 clock source ***************/

  /* Enable the peripheral clock for I2C1 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* Set I2C1 clock source as PCLK (APB Clock)*/
  LL_RCC_SetI2CClockSource(LL_RCC_I2C123_CLKSOURCE_PCLK1);

  /* (3) Configure NVIC for I2C1 **********************************************/

  /* Configure Event IT:
   *  - Set priority for I2C1_EV_IRQn
   *  - Enable I2C1_EV_IRQn
   */
  NVIC_SetPriority(I2C1_EV_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* Configure Error IT:
   *  - Set priority for I2C1_ER_IRQn
   *  - Enable I2C1_ER_IRQn
   */
  NVIC_SetPriority(I2C1_ER_IRQn, 0);  
  NVIC_EnableIRQ(I2C1_ER_IRQn);

  /* (4) Configure I2C1 functional parameters *********************************/

  /* Reset I2C1 data registers */
  if (LL_I2C_DeInit(I2C1) != SUCCESS) 
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C1);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C1);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C1, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C1);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);

  /* Set fields of initialization structure:
   *  - Peripheral Mode     = Mode I2C
   *  - Timing Value        = fast Mode @400kHz with I2CCLK = 520 MHz
   *                          rise time = 100ns, 
   *                          fall time = 10ns
   *  - Analog Filter       = Enabled
   *  - Digital Filter      = Disabled
   *  - Own Address1        = SLAVE_OWN_ADDRESS
   *  - Type of Acknowledge = Acknowledge bytes/address received
   *  - Own Address 1 Size  = Slave Address in 7-bit
  */
  i2c_initstruct.PeripheralMode  = LL_I2C_MODE_I2C;
  i2c_initstruct.Timing          = I2C_TIMING;
  i2c_initstruct.AnalogFilter    = LL_I2C_ANALOGFILTER_ENABLE;
  i2c_initstruct.DigitalFilter   = 0x00;
  i2c_initstruct.OwnAddress1     = SLAVE_OWN_ADDRESS;
  i2c_initstruct.TypeAcknowledge = LL_I2C_ACK;
  i2c_initstruct.OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;

  /* Initialize I2C instance according to parameters defined in initialization structure. */  
  if (LL_I2C_Init(I2C1, &i2c_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* (5) Enable I2C1 address match/error interrupts:
   *  - Enable Address Match Interrupt
   *  - Enable Not acknowledge received interrupt
   *  - Enable Error interrupts
   *  - Enable Stop interrupt
   */
  LL_I2C_EnableIT_ADDR(I2C1);
  LL_I2C_EnableIT_NACK(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
  LL_I2C_EnableIT_STOP(I2C1);
}

/**
  * @brief  This function configures I2C4 in Master mode.
  * @note   This function is used to :
  *         -1- Enables GPIO clock and configures the I2C4 pins.
  *         -2- Enable the I2C4 peripheral clock and I2C4 clock source.
  *         -3- Configure NVIC for I2C4.
  *         -4- Configure and Enable I2C4 functional parameters.
  *         -5- Enable I2C4 transfer complete/error interrupts.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_I2C_Master(void)
{
  /* (1) Enables GPIO clock and configures the I2C4 pins **********************/
  /*    (SCL on PF.14, SDA on PF.15)                     **********************/

  /* Enable the peripheral clock of GPIOF */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_14, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOF, LL_GPIO_PIN_14, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOF, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOF, LL_GPIO_PIN_14, LL_GPIO_OUTPUT_OPENDRAIN);
#if (EXTERNAL_PULL_UP_AVAILABLE == 0)
  LL_GPIO_SetPinPull(GPIOF, LL_GPIO_PIN_14, LL_GPIO_PULL_UP);
#else
  LL_GPIO_SetPinPull(GPIOF, LL_GPIO_PIN_14, LL_GPIO_PULL_NO);
#endif

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOF, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOF, LL_GPIO_PIN_15, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOF, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOF, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_OPENDRAIN);
#if (EXTERNAL_PULL_UP_AVAILABLE == 0)
  LL_GPIO_SetPinPull(GPIOF, LL_GPIO_PIN_15, LL_GPIO_PULL_UP);
#else
  LL_GPIO_SetPinPull(GPIOF, LL_GPIO_PIN_15, LL_GPIO_PULL_NO);
#endif

  /* (2) Enable the I2C4 peripheral clock and I2C4 clock source ***************/

  /* Enable the peripheral clock for I2C4 */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_I2C4);

  /* Set I2C4 clock source as PCLK (APB Clock)*/
  LL_RCC_SetI2CClockSource(LL_RCC_I2C4_CLKSOURCE_PCLK4);

  /* (3) Configure NVIC for I2C4 **********************************************/

  /* Configure Event IT:
   *  - Set priority for I2C4_EV_IRQn
   *  - Enable I2C4_EV_IRQn
   */
  NVIC_SetPriority(I2C4_EV_IRQn, 0);  
  NVIC_EnableIRQ(I2C4_EV_IRQn);

  /* Configure Error IT:
   *  - Set priority for I2C4_ER_IRQn
   *  - Enable I2C4_ER_IRQn
   */
  NVIC_SetPriority(I2C4_ER_IRQn, 0);  
  NVIC_EnableIRQ(I2C4_ER_IRQn);

  /* (4) Configure I2C4 functional parameters ********************************/
  /* Reset I2C4 data registers */
  if (LL_I2C_DeInit(I2C4) != SUCCESS) 
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C4);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C4, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C4);

  /* Configure the Master to operate in 7-bit or 10-bit addressing mode */
  /* Reset Value is LL_I2C_ADDRESSING_MODE_7BIT                         */
  //LL_I2C_SetMasterAddressingMode(I2C4, LL_I2C_ADDRESSING_MODE_7BIT);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C4, LL_I2C_MODE_I2C);

  /* Set fields of initialization structure:
   *  - Peripheral Mode     = Mode I2C
   *  - Timing Value        = fast Mode @400kHz with I2CCLK = 520 MHz
   *                          rise time = 100ns, 
   *                          fall time = 10ns
   *  - Analog Filter       = Enabled
   *  - Digital Filter      = Disabled
   *  - Own Address1        = No own address 1
   *  - Type of Acknowledge = Acknowledge bytes/address received
   *  - Own Address 1 Size  = default value Slave Address in 7-bit
  */
  i2c_initstruct.PeripheralMode  = LL_I2C_MODE_I2C;
  i2c_initstruct.Timing          = I2C_TIMING;
  i2c_initstruct.AnalogFilter    = LL_I2C_ANALOGFILTER_ENABLE;
  i2c_initstruct.DigitalFilter   = 0x00;
  i2c_initstruct.OwnAddress1     = 0x00;
  i2c_initstruct.TypeAcknowledge = LL_I2C_ACK;
  i2c_initstruct.OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;

  /* Initialize I2C instance according to parameters defined in initialization structure. */  
  if (LL_I2C_Init(I2C4, &i2c_initstruct) != SUCCESS)
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* (5) Enable I2C4 transfer complete/error interrupts:
   *  - Enable Receive Interrupt
   *  - Enable Not acknowledge received interrupt
   *  - Enable Error interrupts
   *  - Enable Stop interrupt
   */
  LL_I2C_EnableIT_RX(I2C4);
  LL_I2C_EnableIT_NACK(I2C4);
  LL_I2C_EnableIT_ERR(I2C4);
  LL_I2C_EnableIT_STOP(I2C4);
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
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);

  /* Toggle IO in an infinite loop */
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

  /* Enable a falling trigger External line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
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
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  This Function handle Master events to perform a reception process
  * @note  This function is composed in one step :
  *        -1- Initiate a Start condition to the Slave device.
  * @param  None
  * @retval None
  */
void Handle_I2C_Master(void)
{
  /* (1) Initiate a Start condition to the Slave device ***********************/

  /* Master Generate Start condition for a read request:
   *  - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS
   *  - with a auto stop condition generation when receive 1 byte
   */
  LL_I2C_HandleTransfer(I2C4, SLAVE_OWN_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
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
/*   IRQ HANDLER TREATMENT Functions                                          */
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
  * @brief  Function called from I2C IRQ Handler when TXIS flag is set
  *         Function is in charge of transmit a byte on I2C lines.
  * @param  None
  * @retval None
  */
void Slave_Ready_To_Transmit_Callback(void)
{
  /* Send the Byte requested by the Master */
  LL_I2C_TransmitData8(I2C1, SLAVE_BYTE_TO_SEND);
}

/**
  * @brief  Function called from I2C IRQ Handler when RXNE flag is set
  *         Function is in charge of reading byte received on I2C lines.
  * @param  None
  * @retval None
  */
void Master_Reception_Callback(void)
{
  /* Read character in Receive Data register.
  RXNE flag is cleared by reading data in RXDR register */
  aReceiveBuffer[ubReceiveIndex++] = LL_I2C_ReceiveData8(I2C4);
}

/**
  * @brief  Function called from I2C IRQ Handler when STOP flag is set
  *         Function is in charge of checking data received,
  *         LED1 is On if data are correct.
  * @param  None
  * @retval None
  */
void Master_Complete_Callback(void)
{
  /* Read Received character.
  RXNE flag is cleared by reading of RXDR register */
  if(aReceiveBuffer[ubReceiveIndex-1] == SLAVE_BYTE_TO_SEND)
  {
    /* Turn LED1 On:
     * - Expected byte has been received
     * - Master Rx sequence completed successfully
     */
    LED_On();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}

/**
  * @brief  Function called in case of error detected in I2C IT Handler
  * @param  None
  * @retval None
  */
void Error_Callback(void)
{
  /* Disable I2C1_EV_IRQn and I2C4_EV_IRQn */
  NVIC_DisableIRQ(I2C1_EV_IRQn);
  NVIC_DisableIRQ(I2C4_EV_IRQn);

  /* Disable I2C1_ER_IRQn and I2C4_ER_IRQn */
  NVIC_DisableIRQ(I2C1_ER_IRQn);
  NVIC_DisableIRQ(I2C4_ER_IRQn);

  /* Unexpected event : Set LED1 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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

