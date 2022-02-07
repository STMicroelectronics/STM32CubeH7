/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
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
uint8_t SelLayer = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USARTx                           USART1
#define BOARD_NAME                      "STM32H7B3I-EVAL"
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
extern uint32_t demo_icon __attribute__ ((section ("demo_icon_section")));
extern uint32_t demo_logo __attribute__ ((section ("demo_logo_section")));

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
  .icon_res   = { 102, 102 },
  .logo_addr  = &demo_logo,
  .logo_res   = { 52, 52 },
  .sw_flags   = SW_FLAG_AUTO_DEMO,
  .demo_descr = DEMO_DESCRITION
};

uint8_t detected = 0;
uint8_t Touchdetected = 0;
static UART_HandleTypeDef UART_Handle;
  TS_State_t  ts;
  
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

static void ConfigureExternalRam(void)
{
  int32_t RetVal = BSP_ERROR_NONE;
  /* Initialize the SDRAM memory */
  if ((RetVal = BSP_SDRAM_Init(0)) != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the SDRAM !! (Error %ld)\n", RetVal);
    Error_Handler();
  }
}

static void ConfigureExternalRom(void)
{
  int32_t RetVal = BSP_ERROR_NONE;
  BSP_OSPI_NOR_Init_t Flash;

  BSP_OSPI_NOR_DeInit(0);

  Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
  Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER;
  if((RetVal = BSP_OSPI_NOR_Init(0, &Flash)) != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the OSPI !! (Error %ld)\n", RetVal);
    Error_Handler();
  }
  if((RetVal = BSP_OSPI_NOR_EnableMemoryMappedMode(0)) != BSP_ERROR_NONE)
  {
    printf("Failed to configure MM Mode for OSPI !! (Error %ld)\n", RetVal);
    Error_Handler();
  }
}

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None
  * @retval RetVal
  */
uint8_t k_BspInit(void)
{
  int32_t RetVal = 0;
  uint8_t counter = 10;

  TS_Init_t TS_Init;

  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();

  /* Configure LEDs */
  BSP_LED_Init(LED_RED);

  /* Init Wakeup/Tamper push-button in EXTI Mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  BSP_ConfigSerial();

  printf( "\n\n%s: %s " GET_DEMO_VERSION_STR(IntThirdPartyHeader.demo_id) " (Build %s at %s)\n", BOARD_NAME, IntThirdPartyHeader.demo_descr, GET_DEMO_VERSION_P(IntThirdPartyHeader.demo_id), __DATE__, __TIME__ );
  printf( "CPU running at %luMHz, Peripherals at %luMHz/%luMHz\n", (HAL_RCCEx_GetD1SysClockFreq()/1000000U), (HAL_RCC_GetPCLK1Freq()/1000000U), (HAL_RCC_GetPCLK2Freq()/1000000U) );

  
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  BSP_LED_Toggle(LED1);
  BSP_LED_Toggle(LED2);
  BSP_LED_Toggle(LED3);
  BSP_LED_Toggle(LED4);
  
  /* Initialize the SDRAM memory */
  if ((RetVal = BSP_SDRAM_Init(0)) != 0)
  {
    printf("Failed to initialize the SDRAM !! (Error %ld)\n", RetVal);
    return 0;
  }

  /* Initialize the External Ram */
  ConfigureExternalRam();

  /* Initialize the External Rom */
  ConfigureExternalRom();

  TS_Init.Width = 800;
  TS_Init.Height = 480;
  TS_Init.Orientation = TS_SWAP_Y;
  TS_Init.Accuracy = 0;
  do
  {

    RetVal = BSP_TS_Init(0, &TS_Init);
    HAL_Delay(100);
    counter--;
  }
  while (counter && (RetVal != BSP_ERROR_NONE));

  if(RetVal != BSP_ERROR_NONE)
  {
    printf("Failed to initialize TS !! (Error %ld)\n", RetVal);
    return 0;
  }

#if defined(USE_TS_INT)
  RetVal = BSP_TS_EnableIT(0);
  if(RetVal != BSP_ERROR_NONE)
  {
    printf("Failed to initialize TS (IT) !! (Error %ld)\n", RetVal);
    return 0;
  }
#endif /* USE_TS_INT */

  /* Enable Back up SRAM */
  /* Enable write access to Backup domain */
  PWR->CR1 |= PWR_CR1_DBP;
  while((PWR->CR1 & PWR_CR1_DBP) == RESET)
  {
  }
  /*Enable BKPRAM clock*/
  __HAL_RCC_BKPRAM_CLK_ENABLE();

  return 1;
}

/**
  * @brief  Read the coordinate of the point touched and assign their
  *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
  * @param  None
  * @retval None
  */
uint8_t k_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};

  uint16_t xDiff, yDiff;

  BSP_TS_GetState(0, (TS_State_t *) &ts);

  if((ts.TouchX >= LCD_GetXSize()) ||(ts.TouchY >= LCD_GetYSize()) )
  {
    ts.TouchX = 0;
    ts.TouchY = 0;
  }

  xDiff = (TS_State.x > ts.TouchX) ? (TS_State.x - ts.TouchX) : (ts.TouchX - TS_State.x);
  yDiff = (TS_State.y > ts.TouchY) ? (TS_State.y - ts.TouchY) : (ts.TouchY - TS_State.y);

  if((TS_State.Pressed != ts.TouchDetected ) ||
     (xDiff > 3 )||
       (yDiff > 3))
  {
    TS_State.Pressed = ts.TouchDetected;
    TS_State.Layer = SelLayer;
    if(ts.TouchDetected)
    {
      TS_State.x = ts.TouchX;
      TS_State.y = ts.TouchY ;
      GUI_TOUCH_StoreStateEx(&TS_State);
    }
    else
    {
      GUI_TOUCH_StoreStateEx(&TS_State);
      TS_State.x = 0;
      TS_State.y = 0;
    }

    return 1;
  }

  return 0;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the DISCO_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UART_Handle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @}
  */

/**
  * @}
  */

