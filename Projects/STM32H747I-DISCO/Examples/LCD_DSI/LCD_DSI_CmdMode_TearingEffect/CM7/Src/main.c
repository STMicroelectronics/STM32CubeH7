/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_CmdMode_TearingEffect/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use LCD DSI to display
  *          on LCD KoD in mode DSI Adapted command mode using DSI link as anti-tearing trigger
  *          to start refreshing the LCD Display Graphic RAM (GRAM).
  *          The complete screen size is WVGA and oriented in mode Portrait (480x800) in this example.
  *          It uses the STM32H7xx HAL API and discovery BSP.
  *          This is the main program for Cortex-M7.
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
#include "image_320x240_argb8888.h"
#include "life_augmented_argb8888.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup LCD_DSI_CmdMode_TearingEffect
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
extern LTDC_HandleTypeDef hlcd_ltdc;
static DMA2D_HandleTypeDef   hdma2d;
extern DSI_HandleTypeDef hlcd_dsi;
DSI_VidCfgTypeDef hdsivideo_handle;
DSI_CmdCfgTypeDef CmdCfg;
DSI_LPCmdTypeDef LPCmd;
DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
/* Private define ------------------------------------------------------------*/
#define VSYNC           1  
#define VBP             1 
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            400      /* !!!! SCREEN DIVIDED INTO 2 AREAS !!!! */

#define LAYER0_ADDRESS  (LCD_FB_START_ADDRESS)

#define LEFT_AREA         1
#define RIGHT_AREA        2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static int32_t pending_buffer = -1;
static int32_t active_area = 0;
static uint32_t ImageIndex = 0;
static const uint32_t * Images[] = 
{
  image_320x240_argb8888,
  life_augmented_argb8888,  
};

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pScanCol[]    = {0x02, 0x15};             /* Scan @ 533 */
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CopyPicture(uint32_t *pSrc, 
                           uint32_t *pDst, 
                           uint16_t x, 
                           uint16_t y, 
                           uint16_t xsize, 
                           uint16_t ysize);
static uint8_t LCD_Init(void);
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
void LTDC_Init(void);

static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
int32_t LCD_GetXSize(uint32_t Instance, uint32_t *XSize);
int32_t LCD_GetYSize(uint32_t Instance, uint32_t *YSize);

static void LCD_BriefDisplay(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

void LCD_MspInit(void);

const LCD_UTILS_Drv_t LCD_UTIL_Driver =
{
  BSP_LCD_DrawBitmap,
  BSP_LCD_FillRGBRect,
  BSP_LCD_DrawHLine,
  BSP_LCD_DrawVLine,
  BSP_LCD_FillRect,
  BSP_LCD_ReadPixel,
  BSP_LCD_WritePixel,
  LCD_GetXSize,
  LCD_GetYSize,
  BSP_LCD_SetActiveLayer,
  BSP_LCD_GetPixelFormat
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7) 
     in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
  */
  
  /* Configure the MPU attributes as Write Through for SDRAM*/
  MPU_Config();
  
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* This sample code displays a fixed image 320x240 on LCD KoD in                      */
  /* orientation mode portrait and DSI mode is adapted command mode with TE signal from */
  /* LCD KoD Screen external GPIO                                                       */

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

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* When system initialization is finished, Cortex-M7 could wakeup (when needed) the Cortex-M4  by means of 
     HSEM notification or by any D2 wakeup source (SEV,EXTI..)   */

  /* Initialize the SDRAM */
  BSP_SDRAM_Init(0);
  
  /* Initialize the LCD   */
  if( LCD_Init() != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Set the LCD Context */
  Lcd_Ctx[0].ActiveLayer = 0;
  Lcd_Ctx[0].PixelFormat = LCD_PIXEL_FORMAT_ARGB8888;
  Lcd_Ctx[0].BppFactor = 4; /* 4 Bytes Per Pixel for ARGB8888 */  
  Lcd_Ctx[0].XSize = 800;  
  Lcd_Ctx[0].YSize = 480;

  /* Disable DSI Wrapper in order to access and configure the LTDC */
  __HAL_DSI_WRAPPER_DISABLE(&hlcd_dsi);

  /* Initialize LTDC layer 0 iused for Hint */  
  LCD_LayertInit(0, LCD_FRAME_BUFFER);     
  UTIL_LCD_SetFuncDriver(&LCD_UTIL_Driver);

  /* Update pitch : the draw is done on the whole physical X Size */
  HAL_LTDC_SetPitch(&hlcd_ltdc, Lcd_Ctx[0].XSize, 0);

  /* Enable DSI Wrapper so DSI IP will drive the LTDC */
  __HAL_DSI_WRAPPER_ENABLE(&hlcd_dsi);  
    
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, LCD_CMD_CASET, pColLeft);
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, LCD_CMD_PASET, pPage);
 
  /* Display example brief   */
  LCD_BriefDisplay();
  
  /* Show first image */
  CopyPicture((uint32_t *)Images[ImageIndex++], (uint32_t *)LCD_FRAME_BUFFER, 240, 160, 320, 240);
  
  pending_buffer = 0;
  active_area = LEFT_AREA;
  
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, LCD_CMD_WRTESCN, pScanCol);
  
  /* Send Display On DCS Command to display */
  HAL_DSI_ShortWrite(&(hlcd_dsi),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     LCD_CMD_DISPON,
                     0x00);  
  HAL_DSI_Refresh(&hlcd_dsi);

  /* Infinite loop */
  while (1)
  {
    if(pending_buffer < 0)
    {
      CopyPicture((uint32_t *)Images[ImageIndex++], (uint32_t *)LCD_FRAME_BUFFER, 240, 160, 320, 240);
      
      if(ImageIndex >= 2)
      {
        ImageIndex = 0;
      }
      pending_buffer = 1;
      
      HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, LCD_CMD_WRTESCN, pScanCol);
      HAL_DSI_Refresh(&hlcd_dsi);
    }
    /* Wait some time before switching to next image */
    HAL_Delay(2000);
  }
}

/**
  * @brief  Gets the LCD X size.
  * @param  Instance  LCD Instance
  * @param  XSize     LCD width
  * @retval BSP status
  */
int32_t LCD_GetXSize(uint32_t Instance, uint32_t *XSize)
{
  *XSize = Lcd_Ctx[0].XSize;
 
  return BSP_ERROR_NONE;
}

/**
  * @brief  Gets the LCD Y size.
  * @param  Instance  LCD Instance
  * @param  YSize     LCD Height
  * @retval BSP status
  */
int32_t LCD_GetYSize(uint32_t Instance, uint32_t *YSize)
{
  *YSize = Lcd_Ctx[0].YSize;
 
  return BSP_ERROR_NONE;
}

/**
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
  /* Mask the TE */
  HAL_DSI_ShortWrite(hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, LCD_CMD_TEOFF, 0x00);
  
  /* Refresh the right part of the display */
  HAL_DSI_Refresh(hdsi);   
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  if(pending_buffer >= 0)
  {
    if(active_area == LEFT_AREA)
    {     
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(hdsi);
      /* Update LTDC configuration */
      LTDC_LAYER(&hlcd_ltdc, 0)->CFBAR = LCD_FRAME_BUFFER + 400 * 4;
      __HAL_LTDC_RELOAD_CONFIG(&hlcd_ltdc);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(hdsi);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, LCD_CMD_CASET, pColRight);
      /* Refresh the right part of the display */
      HAL_DSI_Refresh(hdsi);    
      
    }
    else if(active_area == RIGHT_AREA)
    {

      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(&hlcd_dsi);
      /* Update LTDC configuration */
      LTDC_LAYER(&hlcd_ltdc, 0)->CFBAR = LCD_FRAME_BUFFER;
      __HAL_LTDC_RELOAD_CONFIG(&hlcd_ltdc);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(&hlcd_dsi);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, LCD_CMD_CASET, pColLeft); 
      pending_buffer = -1;     
    }
  }
  active_area = (active_area == LEFT_AREA)? RIGHT_AREA : LEFT_AREA; 
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CM7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (CM4 CPU, AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
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

 /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)       
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
 */
 
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
}

/**
  * @brief  Initializes the DSI LCD. 
  * The ititialization is done as below:
  *     - DSI PLL ititialization
  *     - DSI ititialization
  *     - LTDC ititialization
  *     - LCD component Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void)
{
  DSI_PHY_TimerTypeDef  PhyTimings;
  LCD_COMP_IO                IOCtx;
  static LCD_COMP_OBJ        LCD_Obj;
  static void                *Lcd_CompObj = NULL;
  
  /* Toggle Hardware Reset of the DSI LCD using
     its XRES signal (active low) */
  BSP_LCD_Reset(0);
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  LCD_MspInit();

  /* LCD clock configuration */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/19 = 42 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 42 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;    
  PeriphClkInitStruct.PLL3.PLL3N = 160;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;  
  PeriphClkInitStruct.PLL3.PLL3R = 19;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);   
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hlcd_dsi.Instance = DSI;
  
  HAL_DSI_DeInit(&(hlcd_dsi));
  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;  

  hlcd_dsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hlcd_dsi.Init.TXEscapeCkdiv = 0x4;
  
  
  HAL_DSI_Init(&(hlcd_dsi), &(dsiPllInit));
    
  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hlcd_dsi, &CmdCfg);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(&hlcd_dsi, &LPCmd);

  /* Initialize LTDC */
  LTDC_Init();
  
  /* Start DSI */
  HAL_DSI_Start(&(hlcd_dsi));

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hlcd_dsi, &PhyTimings);
  
  /* Initialize the LCD Component Display IC Driver (KoD LCD IC Driver) */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;
  LCD_COMP_RegisterBusIO(&LCD_Obj, &IOCtx);
  Lcd_CompObj=(&LCD_Obj);
  LCD_COMP_Init(Lcd_CompObj, LCD_COMP_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hlcd_dsi, &LPCmd);
  
  HAL_DSI_ConfigFlowControl(&hlcd_dsi, DSI_FLOW_CONTROL_BTA);
  HAL_DSI_ForceRXLowPower(&hlcd_dsi, ENABLE);  
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
void LTDC_Init(void)
{
  /* DeInit */
  hlcd_ltdc.Instance = LTDC;
  HAL_LTDC_DeInit(&hlcd_ltdc);
  
  /* LTDC Config */
  /* Timing and polarity */
  hlcd_ltdc.Init.HorizontalSync = HSYNC;
  hlcd_ltdc.Init.VerticalSync = VSYNC;
  hlcd_ltdc.Init.AccumulatedHBP = HSYNC+HBP;
  hlcd_ltdc.Init.AccumulatedVBP = VSYNC+VBP;
  hlcd_ltdc.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hlcd_ltdc.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hlcd_ltdc.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hlcd_ltdc.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  
  /* background value */
  hlcd_ltdc.Init.Backcolor.Blue = 0;
  hlcd_ltdc.Init.Backcolor.Green = 0;
  hlcd_ltdc.Init.Backcolor.Red = 0;
  
  /* Polarity */
  hlcd_ltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hlcd_ltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hlcd_ltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hlcd_ltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hlcd_ltdc.Instance = LTDC;

  HAL_LTDC_Init(&hlcd_ltdc);
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
  LTDC_LayerCfgTypeDef  layercfg;

  /* Layer Init */
  layercfg.WindowX0 = 0;
  layercfg.WindowX1 = Lcd_Ctx[0].XSize/2;
  layercfg.WindowY0 = 0;
  layercfg.WindowY1 = Lcd_Ctx[0].YSize; 
  layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  layercfg.FBStartAdress = Address;
  layercfg.Alpha = 255;
  layercfg.Alpha0 = 0;
  layercfg.Backcolor.Blue = 0;
  layercfg.Backcolor.Green = 0;
  layercfg.Backcolor.Red = 0;
  layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  layercfg.ImageWidth = Lcd_Ctx[0].XSize/2;
  layercfg.ImageHeight = Lcd_Ctx[0].YSize;
  
  HAL_LTDC_ConfigLayer(&hlcd_ltdc, &layercfg, LayerIndex);
}

/**
  * @brief  DCS or Generic short/long write command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be written
  * @param  pData pointer to a buffer of data to be write
  * @param  Size To precise command to be used (short or long)
  * @retval BSP status
  */
static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Size <= 1U)
  {
    if(HAL_DSI_ShortWrite(&hlcd_dsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  else
  {
    if(HAL_DSI_LongWrite(&hlcd_dsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  DCS or Generic read command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be read
  * @param  pData pointer to a buffer to store the payload of a read back operation.
  * @param  Size  Data size to be read (in byte).
  * @retval BSP status
  */
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_DSI_Read(&hlcd_dsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  return ret;
}

void LCD_MspInit(void)
{
  /** @brief Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /** @brief Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /** @brief Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();

  /** @brief Soft Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  /** @brief NVIC configuration for LTDC interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}

/**
  * @brief  Display Example description.
  * @param  None
  * @retval None
  */
static void LCD_BriefDisplay(void)
{
  UTIL_LCD_SetFont(&Font24);  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE); 
  UTIL_LCD_FillRect(0, 0, Lcd_Ctx[0].XSize, 112,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(0, 112, Lcd_Ctx[0].XSize, 368, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAtLine(1, (uint8_t *)"        LCD_DSI_CmdMode_TearingEffect");
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAtLine(4, (uint8_t *)"This example shows how to display images on LCD DSI and prevent");
  UTIL_LCD_DisplayStringAtLine(5, (uint8_t *)"Tearing Effect");
}

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode   
  * @retval None
  */
static void CopyPicture(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{   
  uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
  uint32_t source      = (uint32_t)pSrc;
  
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 800 - xsize;
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */    
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */  

  hdma2d.Instance          = DMA2D; 
   
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
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
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is 0xD0000000 .
  *         The Configured Region Size is 32MB because same as SDRAM size.
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief Error Handler
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LED_On(LED3);
  while(1) { ; } /* Blocking on error */
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

