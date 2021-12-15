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
extern uint8_t    SelLayer;
extern void       k_MenuResetAutoDemo(void);

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USARTx                           USART1
#define BOARD_NAME                      "STM32H747I-DISCO_MB1248"
/* STemWin Demonstration ID : 1 */
#define DEMO_CODE_REVISION              0xC5010000 /* 0xAAXXYYZZ - 0xCx : Demo Code ID | 0xXXYYZZ : MajorMinorPatch Version number */
#define DEMO_DATA_REVISION              0xD5010000 /* 0xAAXXYYZZ - 0xDx : Demo Data ID | 0xXXYYZZ : MajorMinorPatch Version number */
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
BSP_QSPI_Init_t init;
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

  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Configure the GPIO on PG3 */
  gpio_init_structure.Pin   = GPIO_PIN_3;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);

  HAL_Delay(20); /* wait 20 ms */

  /* Deactivate XRES */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);

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
  if ((RetVal = BSP_SDRAM_Init(0)) != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the SDRAM !! (Error %d)\n", RetVal);
    return 0;
  }
  init.InterfaceMode=MT25TL01G_QPI_MODE;
  init.TransferRate= MT25TL01G_DTR_TRANSFER ;
  init.DualFlashMode= MT25TL01G_DUALFLASH_ENABLE;
  /* Initialize the NOR QuadSPI flash */
  if ((RetVal = BSP_QSPI_Init(0,&init)) != BSP_ERROR_NONE)
  {
    printf("Failed to initialize the QSPI !! (Error %d)\n", RetVal);
    return 0;
  }
  else
  {
    if((RetVal = BSP_QSPI_EnableMemoryMappedMode(0)) != BSP_ERROR_NONE)
    {
      printf("Failed to configure the QSPI !! (Error %d)\n", RetVal);
      return 0;
    }
  }

  /* Initialize the Touch screen */
  LCD_LL_Reset(); /* LCD controller need to be initialized */
  TS_Init_t hTS;
  hTS.Width = 800;
  hTS.Height = 480;
  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy = 5;
  do
  {
    RetVal = BSP_TS_Init(0,&hTS);
    HAL_Delay(100);
    counter--;
  }
  while (counter && (RetVal != BSP_ERROR_NONE));

  if(RetVal != BSP_ERROR_NONE)
  {
    printf("Failed to initialize TS !! (Error %d)\n", RetVal);
    return 0;
  }

  RetVal =  BSP_TS_EnableIT(0);;
  if(RetVal != BSP_ERROR_NONE)
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
  __IO TS_State_t ts;
  uint16_t xDiff, yDiff;

  BSP_TS_GetState(0,(TS_State_t *)&ts);

  ts.TouchX = TouchScreen_Get_Calibrated_X(ts.TouchX);
  ts.TouchY = TouchScreen_Get_Calibrated_Y(ts.TouchY);

  if((ts.TouchX >= LCD_GetXSize()) ||(ts.TouchY >= LCD_GetYSize()) )
  {
    ts.TouchX = 0;
    ts.TouchY = 0;
  }

  xDiff = (TS_State.x > ts.TouchX) ? (TS_State.x - ts.TouchX) : (ts.TouchX - TS_State.x);
  yDiff = (TS_State.y > ts.TouchY) ? (TS_State.y - ts.TouchY) : (ts.TouchY - TS_State.y);

  if((TS_State.Pressed != ts.TouchDetected ) ||
     (xDiff > 8) || (yDiff > 8))
  {
    TS_State.Pressed = ts.TouchDetected;
    TS_State.Layer = SelLayer;
    if(ts.TouchDetected)
    {
      k_MenuResetAutoDemo();

      TS_State.x = ts.TouchX;
      TS_State.y = ts.TouchY;
      if(ts.TouchY < 240)
      {
        TS_State.y = (ts.TouchY * 100) / 105; /* -5% */
      }
      else
      {
        TS_State.y = (ts.TouchY * 105) / 100; /* +5% */
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

