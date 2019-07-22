/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "k_bsp.h"
#include "stdio.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern uint8_t    SelLayer;
extern void       k_MenuResetAutoDemo(void);

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USARTx                           USART1
#define BOARD_NAME                      "STM32H747I-EVAL_MB1246"
/* STemWin Demonstration ID : 1 */
#define DEMO_CODE_REVISION              0xC1010000 /* 0xAAXXYYZZ - 0xCx : Demo Code ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_DATA_REVISION              0xD1010000 /* 0xAAXXYYZZ - 0xDx : Demo Data ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_DESCRITION                 "STemWin Out of Box Demo"

/* Private macros ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define GET_DEMO_VERSION_STR(tag)  "V%lu.%lu.%lu"
#define GET_DEMO_VERSION_P(tag)    ((tag  &0x00FF0000) >> 16), ((tag  &0x0000FF00) >> 8), (tag  &0x000000FF)

/* Private variables ---------------------------------------------------------*/
extern const uint32_t demo_icon __attribute__ ((section ("demo_icon_section")));
extern const uint32_t demo_logo __attribute__ ((section ("demo_logo_section")));

#if defined ( __ICCARM__ )
#pragma location="IntThirdPartyHeaderSection"
__root
#else
__attribute__((section(".IntThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t IntThirdPartyHeader = {
  .demo_id = DEMO_CODE_REVISION,
  .icon_addr  = 0,
  .icon_res   = { 0, 0 },
  .logo_addr  = 0,
  .logo_res   = { 0, 0 },
  .sw_flags   = SW_FLAG_AUTO_DEMO,
  .demo_descr = DEMO_DESCRITION
};

#if defined ( __ICCARM__ )
#pragma location="ExtThirdPartyHeaderSection"
__root
#else
__attribute__((section(".ExtThirdPartyHeaderSection"))) __attribute__ ((__used__))
#endif
static const Demo_Header_t ExtThirdPartyHeader = {
  .demo_id = DEMO_DATA_REVISION,
  .icon_addr  = &demo_icon,
  .icon_res   = { 180, 180 },
  .logo_addr  = &demo_logo,
  .logo_res   = { 60, 60 },
  .sw_flags   = SW_FLAG_AUTO_DEMO,
  .demo_descr = DEMO_DESCRITION
};

uint8_t detected = 0;
uint8_t Touchdetected = 0;
static UART_HandleTypeDef UART_Handle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void BSP_ConfigSerial(void)
{
  UART_Handle.Instance            = USARTx;
  UART_Handle.Init.BaudRate       = 115200;
  UART_Handle.Init.WordLength     = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits       = UART_STOPBITS_1;
  UART_Handle.Init.Parity         = UART_PARITY_NONE;
  UART_Handle.Init.Mode           = UART_MODE_TX_RX;
  UART_Handle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UART_Handle.Init.OverSampling   = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UART_Handle) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

static void LCD_LL_Reset(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Configure the GPIO on PF10 */
  gpio_init_structure.Pin   = GPIO_PIN_10;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);

  HAL_Delay(20); /* wait 20 ms */

  /* Desactivate XRES */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);

  /* Wait for 300 ms after releasing XRES before sending commands */
  HAL_Delay(300);
}

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None
  * @retval RetVal
  */
uint8_t k_BspInit(void)
{
  uint8_t RetVal = 0;
  uint8_t counter = 10;

  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();

  /* Configure LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
#if defined (DEBUG_HW_JPEG)
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_ORANGE);
#endif /* DEBUG_HW_JPEG */

  /* Init Wakeup/Tamper push-button in EXTI Mode */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_EXTI);

  BSP_ConfigSerial();

  printf( "\n\n%s: %s " GET_DEMO_VERSION_STR(IntThirdPartyHeader.demo_id) " (Build %s at %s)\n", BOARD_NAME, IntThirdPartyHeader.demo_descr, GET_DEMO_VERSION_P(IntThirdPartyHeader.demo_id), __DATE__, __TIME__ );
  printf( "CPU running at %luMHz, Peripherals at %luMHz/%luMHz\n", (HAL_RCCEx_GetD1SysClockFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );

  /* Initialize the SDRAM memory */
  if ((RetVal = BSP_SDRAM_Init()) != SDRAM_OK)
  {
    printf("Failed to initialize the SDRAM !! (Error %d)\n", RetVal);
    return 0;
  }

  /* Initialize the NOR QuadSPI flash */
  if ((RetVal = BSP_QSPI_Init()) != QSPI_OK)
  {
    printf("Failed to initialize the QSPI !! (Error %d)\n", RetVal);
    return 0;
  }
  else
  {
    if((RetVal = BSP_QSPI_EnableMemoryMappedMode()) != QSPI_OK)
    {
      printf("Failed to configure the QSPI !! (Error %d)\n", RetVal);
      return 0;
    }
  }

  /* Initialize the Touch screen */
  LCD_LL_Reset(); /* LCD controller need to be initialized */
  TS_IO_Init();
  do
  {
    RetVal = BSP_TS_Init(800, 480);
    HAL_Delay(100);
    counter--;
  }
  while (counter && (RetVal != TS_OK));

  if(RetVal != TS_OK)
  {
    printf("Failed to initialize TS !! (Error %d)\n", RetVal);
    return 0;
  }

  RetVal = BSP_TS_ITConfig();
  if(RetVal != TS_OK)
  {
    printf("Failed to initialize TS (IT) !! (Error %d)\n", RetVal);
    return 0;
  }

  /* Enable Back up SRAM */
  /* Enable write access to Backup domain */
  PWR->CR1 |= PWR_CR1_DBP;
  while((PWR->CR1 & PWR_CR1_DBP) == RESET)
  {
  }
  /*Enable BKPRAM clock*/
  __HAL_RCC_BKPRAM_CLK_ENABLE();
  
  BSP_LED_On(LED_GREEN);
  
  return 1;
}

/**
  * @brief  Read the coordinate of the point touched and assign their
  *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
  * @param  None
  * @retval None
  */
void k_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_StateTypeDef ts;
  uint16_t xDiff, yDiff;

  BSP_TS_GetState((TS_StateTypeDef *)&ts);

  ts.touchX[0] = TouchScreen_Get_Calibrated_X(ts.touchX[0]);
  ts.touchY[0] = TouchScreen_Get_Calibrated_Y(ts.touchY[0]);

  if((ts.touchX[0] >= LCD_GetXSize()) ||(ts.touchY[0] >= LCD_GetYSize()) )
  {
    ts.touchX[0] = 0;
    ts.touchY[0] = 0;
  }

  xDiff = (TS_State.x > ts.touchX[0]) ? (TS_State.x - ts.touchX[0]) : (ts.touchX[0] - TS_State.x);
  yDiff = (TS_State.y > ts.touchY[0]) ? (TS_State.y - ts.touchY[0]) : (ts.touchY[0] - TS_State.y);

  if((TS_State.Pressed != ts.touchDetected ) ||
     (xDiff > 8) || (yDiff > 8))
  {
    TS_State.Pressed = ts.touchDetected;
    TS_State.Layer = SelLayer;
    if(ts.touchDetected)
    {
      k_MenuResetAutoDemo();

      TS_State.x = ts.touchX[0];
      TS_State.y = ts.touchY[0];
      if(ts.touchY[0] < 240)
      {
        TS_State.y = (ts.touchY[0] * 100) / 105; /* -5% */
      }
      else
      {
        TS_State.y = (ts.touchY[0] * 105) / 100; /* +5% */
      }
      GUI_TOUCH_StoreStateEx(&TS_State);
    }
    else
    {
      GUI_TOUCH_StoreStateEx(&TS_State);
      
      TS_State.x = 0;
      TS_State.y = 0;
    }
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UART_Handle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
