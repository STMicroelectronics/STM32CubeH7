/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_CmdMode_PartialRefresh/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use LCD DSI to display
  *          on LCD KoD in DSI Adapted command mode using partial refresh.
  *          The complete screen size is WVGA and oriented in mode Portrait (480x800) in this example.
  *          It uses the STM32H7xx HAL API and discovery BSP.
  *          This is the main program for Cortex-M7    
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
#include "image_320x240_argb8888.h"
#include "life_augmented_argb8888.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup LCD_DSI_CmdMode_PartialRefresh
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
extern LTDC_HandleTypeDef hltdc_discovery;
static DMA2D_HandleTypeDef   hdma2d;
extern DSI_HandleTypeDef hdsi_discovery;
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

#define INVALID_AREA      0
#define LEFT_AREA         1
#define RIGHT_AREA        2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static int32_t pending_buffer = -1;
static int32_t active_area = INVALID_AREA;
static uint32_t ImageIndex = 0;
static const uint32_t * Images[] = 
{
  image_320x240_argb8888,
  life_augmented_argb8888,  
};

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pSyncLeft[]   = {0x02, 0x15};             /* Scan @ 533 */
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
static void LCD_BriefDisplay(void);

static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

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
  BSP_SDRAM_Init();
  
  /* Initialize the LCD   */
  if( LCD_Init() != LCD_OK)
  {
    Error_Handler();
  }

  /* Disable DSI Wrapper in order to access and configure the LTDC */
  __HAL_DSI_WRAPPER_DISABLE(&hdsi_discovery);

  /* Initialize LTDC layer 0 iused for Hint */  
  LCD_LayertInit(0, LAYER0_ADDRESS);     
  BSP_LCD_SelectLayer(0); 
  /* Update pitch : the draw is done on the whole physical X Size */
  HAL_LTDC_SetPitch(&hltdc_discovery, BSP_LCD_GetXSize(), 0);

  /* Enable DSI Wrapper so DSI IP will drive the LTDC */
  __HAL_DSI_WRAPPER_ENABLE(&hdsi_discovery);  
  
  HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
  HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);

  /* Display example brief   */
  LCD_BriefDisplay();
  
  /* Show first image */
  CopyPicture((uint32_t *)Images[ImageIndex++], (uint32_t *)LAYER0_ADDRESS, 240, 160, 320, 240);
  
  pending_buffer = 0;
  active_area = LEFT_AREA;
  
  /* set the refresh area to LCD left half */
  HAL_DSI_LongWrite(&hdsi_discovery, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);

  /* Refresh the LCD */
  HAL_DSI_Refresh(&hdsi_discovery);  
        
  /* Infinite loop */
  while (1)
  {
    if(pending_buffer < 0)
    {
      CopyPicture((uint32_t *)Images[ImageIndex++], (uint32_t *)LAYER0_ADDRESS, 240, 160, 320, 240);
      
      if(ImageIndex >= 2)
      {
        ImageIndex = 0;
      }
      pending_buffer = 1;
      
      HAL_DSI_Refresh(&hdsi_discovery);
    }
    /* Wait some time before switching to next image */
    HAL_Delay(2000);
  }
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
      /* Mask the TE */
      HAL_DSI_ShortWrite(hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_TEOFF, 0x00);
      
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(&hdsi_discovery);
      /* Update LTDC configuaration */
      LTDC_LAYER(&hltdc_discovery, 0)->CFBAR = LAYER0_ADDRESS + 400 * 4;
      __HAL_LTDC_RELOAD_CONFIG(&hltdc_discovery);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(&hdsi_discovery);
      
      /* set the refresh area to LCD right half */
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
      /* Refresh the right part of the display */
      HAL_DSI_Refresh(hdsi);    
      
    }
    else if(active_area == RIGHT_AREA)
    {

      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(&hdsi_discovery);
      /* Update LTDC configuaration */
      LTDC_LAYER(&hltdc_discovery, 0)->CFBAR = LAYER0_ADDRESS;
      __HAL_LTDC_RELOAD_CONFIG(&hltdc_discovery);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(&hdsi_discovery);
      
      /* set the refresh area to LCD left half */
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft); 
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
  
  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

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
  *     - OTM8009A LCD Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void)
{
  DSI_PHY_TimerTypeDef  PhyTimings;
  
  /* Toggle Hardware Reset of the DSI LCD using
     its XRES signal (active low) */
  BSP_LCD_Reset();
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();

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
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);   
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_discovery.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_discovery));
  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;  

  hdsi_discovery.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi_discovery.Init.TXEscapeCkdiv = 0x4;
  
  
  HAL_DSI_Init(&(hdsi_discovery), &(dsiPllInit));
    
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
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi_discovery, &CmdCfg);
  
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
  HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);

  /* Initialize LTDC */
  LTDC_Init();
  
  /* Start DSI */
  HAL_DSI_Start(&(hdsi_discovery));
 
  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi_discovery, &PhyTimings); 
  
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  */
  OTM8009A_Init(OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
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
  HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);
  
  HAL_DSI_ConfigFlowControl(&hdsi_discovery, DSI_FLOW_CONTROL_BTA);
  HAL_DSI_ForceRXLowPower(&hdsi_discovery, ENABLE);  
  
  return LCD_OK;
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
void LTDC_Init(void)
{
  /* DeInit */
  HAL_LTDC_DeInit(&hltdc_discovery);
  
  /* LTDC Config */
  /* Timing and polarity */
  hltdc_discovery.Init.HorizontalSync = HSYNC;
  hltdc_discovery.Init.VerticalSync = VSYNC;
  hltdc_discovery.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc_discovery.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc_discovery.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc_discovery.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc_discovery.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc_discovery.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  
  /* background value */
  hltdc_discovery.Init.Backcolor.Blue = 0;
  hltdc_discovery.Init.Backcolor.Green = 0;
  hltdc_discovery.Init.Backcolor.Red = 0;
  
  /* Polarity */
  hltdc_discovery.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc_discovery.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc_discovery.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc_discovery.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_discovery.Instance = LTDC;

  HAL_LTDC_Init(&hltdc_discovery);
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
    LCD_LayerCfgTypeDef  Layercfg;

  /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = BSP_LCD_GetXSize()/2;
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = BSP_LCD_GetYSize(); 
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = BSP_LCD_GetXSize()/2;
  Layercfg.ImageHeight = BSP_LCD_GetYSize();
  
  HAL_LTDC_ConfigLayer(&hltdc_discovery, &Layercfg, LayerIndex); 
}


/**
  * @brief  Display Example description.
  * @param  None
  * @retval None
  */
static void LCD_BriefDisplay(void)
{
  BSP_LCD_SetFont(&Font24);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
  BSP_LCD_FillRect(0, 0, 800, 112);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(0, 112, 800, 368);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)"        LCD_DSI_CmdMode_PartialRefresh");
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"This example shows how to display images on LCD DSI using the partial");
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Refresh method by splitting the display area.");
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
  * @brief Error Handler
  * @retval None
  */
static void Error_Handler(void)
{
  
  BSP_LED_On(LED3);
  while(1) { ; } /* Blocking on error */
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
