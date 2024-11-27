/**
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describe how to configure the camera in continuous mode
  *          and QVGA resolution.
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

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup DCMI_CaptureMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VSYNC           1  
#define VBP             1 
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            400

#define LEFT_AREA         1
#define RIGHT_AREA        2

#define CAMERA_STATE_RESET               0
#define CAMERA_STATE_CAPTURE_ONGOING     1
#define CAMERA_STATE_DISPLAY_ONGOING     2

/* Private macro -------------------------------------------------------------*/
#define __DSI_MASK_TE()   (GPIOJ->AFR[0] &= (0xFFFFF0FFU))   /* Mask DSI TearingEffect Pin*/
#define __DSI_UNMASK_TE() (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8)) /* UnMask DSI TearingEffect Pin*/
/* Private variables ---------------------------------------------------------*/
uint8_t __IO cameraState = CAMERA_STATE_RESET;
static uint32_t   LcdResX    = 0;
static uint32_t   LcdResY    = 0;

/* Camera resolutions */
uint32_t CameraResolution[4] = {CAMERA_R160x120, CAMERA_R320x240, CAMERA_R480x272, CAMERA_R640x480}; 
uint32_t CameraResX[4] = {160, 320, 480, 640};
uint32_t CameraResY[4] = {120, 240, 272, 480};
uint32_t CameraResIndex = 0;

extern LTDC_HandleTypeDef hlcd_ltdc;
extern DSI_HandleTypeDef hlcd_dsi;

static __IO int32_t pending_buffer = -1;
static __IO int32_t active_area = 0;
static __IO int32_t dma2d_pending_copy = 0;

int32_t LCD_GetXSize(uint32_t Instance, uint32_t *XSize);
int32_t LCD_GetYSize(uint32_t Instance, uint32_t *YSize);

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

DSI_CmdCfgTypeDef CmdCfg;
DSI_LPCmdTypeDef LPCmd;
DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
#if (USE_LCD_CTRL_OTM8009A > 0)
uint8_t pScanCol[]    = {0x02, 0x15};             /* Scan @ 533 */
#endif /* USE_LCD_CTRL_OTM8009A */
#if (USE_LCD_CTRL_NT35510 > 0)
uint8_t pScanCol[]  = {0x01, 0x60};
#endif /* USE_LCD_CTRL_NT35510 */
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void DMA2D_ConvertFrameToARGB8888(void *pSrc, void *pDst, uint32_t xsize, uint32_t ysize);
static void DMA2D_Config(uint16_t xsize, uint32_t ysize);
static void DMA2D_TransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d);
static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
static uint8_t LCD_Init(void);
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
static void LTDC_Init(void);
static void Display_StartRefresh(void);
static void Display_WaitRefresh(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void LCD_MspInit(void);
void Error_Handler(void);

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
  
  uint32_t  change_resolution = 1;
  
  HAL_Init();
  
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
 
  /* When system initialization is finished, Cortex-M7 could wakeup (when needed) the Cortex-M4  by means of 
     HSEM notification or by any D2 wakeup source (SEV,EXTI..)   */ 

  /* LEDs initialization */  
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_GREEN);
  
  /* SDRAM initialization */
  BSP_SDRAM_Init(0);
  
  /* LCD DSI initialization in command mode  with one LTDC layers of size 800x480 */

  if(LCD_Init() != BSP_ERROR_NONE)
  {
    Error_Handler();
  } 

  /* Set the LCD Context */
  Lcd_Ctx[0].ActiveLayer = 0;
  Lcd_Ctx[0].PixelFormat = LCD_PIXEL_FORMAT_ARGB8888;
  Lcd_Ctx[0].BppFactor = 4; /* 4 Bytes Per Pixel for ARGB8888 */  
  Lcd_Ctx[0].XSize = 800;  
  Lcd_Ctx[0].YSize = 480;

  UTIL_LCD_SetFuncDriver(&LCD_UTIL_Driver);
  LCD_GetXSize(0,&LcdResX);
  LCD_GetYSize(0,&LcdResY);
  /* Disable DSI Wrapper in order to access and configure the LTDC */
  __HAL_DSI_WRAPPER_DISABLE(&hlcd_dsi);

  /* Initialize LTDC layer 0 iused for Hint */  
  LCD_LayertInit(0, LCD_FRAME_BUFFER); 

  /* Update pitch : the draw is done on the whole physical X Size */
  HAL_LTDC_SetPitch(&hlcd_ltdc,LcdResX, 0);

  /* Enable DSI Wrapper so DSI IP will drive the LTDC */
  __HAL_DSI_WRAPPER_ENABLE(&hlcd_dsi);  
#if (USE_LCD_CTRL_OTM8009A > 0)
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pScanCol);
#endif /* USE_LCD_CTRL_OTM8009A */

#if (USE_LCD_CTRL_NT35510 > 0)
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, NT35510_CMD_CASET, pColLeft);
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, NT35510_CMD_RASET, pPage);
  HAL_DSI_LongWrite(&hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, NT35510_CMD_TEEON, pScanCol);
#endif /* USE_LCD_CTRL_NT35510 */
  
  /* Configure TAMPER Button */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
  
   

  /* Run Application */
  while (1)
  {
    if(change_resolution == 1)
    {
      change_resolution = 0;
      
      /* Reset and power down camera to be sure camera is Off prior start */
      cameraState = CAMERA_STATE_CAPTURE_ONGOING; 
        
      UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
      /* Display USB initialization message */
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE); 
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
      UTIL_LCD_SetFont(&Font24);
      UTIL_LCD_DisplayStringAt(20, (LcdResY- 24), (uint8_t *)"CAMERA CONTINUOUS MODE", CENTER_MODE);  
      
      pending_buffer = 0;
      active_area = LEFT_AREA; 
     
      Display_StartRefresh();
      Display_WaitRefresh(); 
      
      /* Initialize and Configure the DMA2D with the new picture X size and ySize*/
      DMA2D_Config(CameraResX[CameraResIndex], CameraResY[CameraResIndex]);
      
      /* Initialize the Camera */
      if(BSP_CAMERA_Init(0,CameraResolution[CameraResIndex],CAMERA_PF_RGB565) != BSP_ERROR_NONE)
      {
        Error_Handler(); 
      }
      /* Wait for the camera initialization after HW reset*/

      HAL_Delay(100);
      /* Start the Camera Capture */    
      if(BSP_CAMERA_Start(0,(uint8_t *)CAMERA_FRAME_BUFFER,CAMERA_MODE_CONTINUOUS)!=BSP_ERROR_NONE)
      {
        while(1);
      }
    }
    
    /* Wait for user press button */
    while (BSP_PB_GetState(BUTTON_WAKEUP) != SET)
    {
      HAL_Delay(100);
      BSP_LED_Toggle(LED_GREEN);
    }
    /* Wait for user release button */
    while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET);
    
    cameraState = CAMERA_STATE_RESET; 
    BSP_CAMERA_DeInit(0);
    Display_WaitRefresh();
    
    /* Change camera resolution index and stop the camera */
    CameraResIndex++;
    change_resolution = 1;
    if(CameraResIndex > 3)
    {
      CameraResIndex = 0;
    }
    
  }
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(uint32_t Instance)
{
  BSP_CAMERA_Suspend(0);
  if(cameraState == CAMERA_STATE_CAPTURE_ONGOING)
  {
    cameraState = CAMERA_STATE_DISPLAY_ONGOING;  
    /* Convert captured frame to ARGB8888 and copy it to LCD FRAME BUFFER */
    DMA2D_ConvertFrameToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER), (uint32_t *)(LCD_FRAME_BUFFER), CameraResX[CameraResIndex], CameraResY[CameraResIndex]);
  }
}
/**
  * @brief  Copy the Captured Picture to the display Frame buffer.
  * @param  xsize: Picture X size
  * @param  ysize: Picture Y Size
  * @retval None
  */
static void DMA2D_Config(uint16_t xsize, uint32_t ysize)
{
	
/* Configure the DMA2D Mode, Color Mode and output offset */
  hlcd_dma2d.Instance = DMA2D;
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hlcd_dma2d.Init.Mode         = DMA2D_M2M_PFC;
  hlcd_dma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hlcd_dma2d.Init.OutputOffset = LcdResX - xsize;     
  
  /* DMA2D Callbacks Configuration */
  hlcd_dma2d.XferCpltCallback  = DMA2D_TransferCompleteCallback;
  
  /* Foreground Configuration */
  hlcd_dma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  hlcd_dma2d.LayerCfg[1].InputAlpha = 0xFF;
  hlcd_dma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hlcd_dma2d.LayerCfg[1].InputOffset = 0;
  
  hlcd_dma2d.Instance = DMA2D;
  HAL_DMA2D_DeInit(&hlcd_dma2d);
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hlcd_dma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hlcd_dma2d, 1) != HAL_OK) 
    {
      Error_Handler();      
    }
  }
  else
  {
    Error_Handler();
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

 void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* hdma2d)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma2d);
  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();
  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_DMA2D_MspInit can be implemented in the user file.
   */
}
/**
  * @brief  Copy the Captured Picture to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  xsize: Picture X size
  * @param  ysize: Picture Y Size
  * @retval None
  */
static void DMA2D_ConvertFrameToARGB8888(void *pSrc, void *pDst, uint32_t xsize, uint32_t ysize)
{
  uint32_t xPos, yPos, destination;   
  
  /* Calculate the destination transfer address */
  xPos = (LcdResX  - xsize)/2;
  yPos = (LcdResY  - ysize)/2;  
  
  destination = (uint32_t)pDst + (yPos * LcdResX + xPos) * ARGB8888_BYTE_PER_PIXEL;
  dma2d_pending_copy = 1;
  /* Starts the DMA2D transfer */
  if(HAL_DMA2D_Start_IT(&hlcd_dma2d, (uint32_t)pSrc, destination, xsize, ysize) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  DMA2D Transfer completed callback
 * @param  hdma2d: DMA2D handle.
 * @retval None
 */
static void DMA2D_TransferCompleteCallback(DMA2D_HandleTypeDef *hdma2d)
{
  dma2d_pending_copy = 0;
  Display_StartRefresh();
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
static uint8_t LCD_Init(void){
  
  GPIO_InitTypeDef GPIO_Init_Structure;
  DSI_PHY_TimerTypeDef  PhyTimings;

#if (USE_LCD_CTRL_NT35510 > 0)
  NT35510_IO_t              IOCtx;
  static NT35510_Object_t   NT35510Obj;
  static void                *Lcd_CompObj = NULL;  
#endif /* USE_LCD_CTRL_NT35510 */
#if (USE_LCD_CTRL_OTM8009A > 0)
  OTM8009A_IO_t              IOCtx;
  static OTM8009A_Object_t   OTM8009AObj;
  static void                *Lcd_CompObj = NULL;  
#endif /* USE_LCD_CTRL_OTM8009A */  
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
  CmdCfg.TearingEffectSource   = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_DISABLE;
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

#if (USE_LCD_CTRL_NT35510 > 0)
 /* Initialize the NT35510 LCD Display IC Driver (KoD LCD IC Driver) */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;
  NT35510_RegisterBusIO(&NT35510Obj, &IOCtx);
  Lcd_CompObj=(&NT35510Obj);
  NT35510_Init(Lcd_CompObj, NT35510_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
#endif /* USE_LCD_CTRL_NT35510 */

#if (USE_LCD_CTRL_OTM8009A > 0)
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver) */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;
  OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx);
  Lcd_CompObj=(&OTM8009AObj);
  OTM8009A_Init(Lcd_CompObj, OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
#endif /* USE_LCD_CTRL_OTM8009A */
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
  
  /* Enable GPIOJ clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  
  /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
  /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
  /* This pin is used only when the LCD and DSI link is configured in command mode    */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
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
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
  LTDC_LayerCfgTypeDef  layercfg;

  /* Layer Init */
  layercfg.WindowX0 = 0;
  layercfg.WindowX1 = Lcd_Ctx[0].XSize/2 ;
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
  layercfg.ImageWidth = Lcd_Ctx[0].XSize / 2;
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
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
  /* Mask the TE */
  __DSI_MASK_TE();
  
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
      LTDC_LAYER(&hlcd_ltdc, 0)->CFBAR = LCD_FRAME_BUFFER + 400 * ARGB8888_BYTE_PER_PIXEL;
      __HAL_LTDC_RELOAD_CONFIG(&hlcd_ltdc);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(hdsi);
#if (USE_LCD_CTRL_OTM8009A > 0)
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
#endif /* USE_LCD_CTRL_OTM8009A */
#if (USE_LCD_CTRL_NT35510 > 0)
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, NT35510_CMD_CASET, pColRight);
#endif /* USE_LCD_CTRL_NT35510 */
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
#if (USE_LCD_CTRL_OTM8009A > 0)
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft); 
#endif /* USE_LCD_CTRL_OTM8009A */
#if (USE_LCD_CTRL_NT35510 > 0)
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, NT35510_CMD_CASET, pColLeft);
#endif /* USE_LCD_CTRL_NT35510 */

      if(cameraState == CAMERA_STATE_DISPLAY_ONGOING)
      {
        cameraState = CAMERA_STATE_CAPTURE_ONGOING;
        BSP_CAMERA_Resume(0);
      }      
        
      pending_buffer = -1; 
    }
  }
  active_area = (active_area == LEFT_AREA)? RIGHT_AREA : LEFT_AREA; 
}


/**
  * @brief  Start display refresh.
  * @param  None
  * @retval None
  */
static void Display_StartRefresh(void)
{
  /* Frame Buffer updated */ 
  pending_buffer = 1;
  
  /* UnMask the TE */
  __DSI_UNMASK_TE(); 
}

/**
  * @brief  Wait for Display end of refresh.
  * @param  None
  * @retval None
  */
static void Display_WaitRefresh(void)
{
  while((pending_buffer != -1) && (dma2d_pending_copy != 0))
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Cortex-M7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED REDon */
  BSP_LED_On(LED_RED);
  while(1)
  {
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
#endif /* USE_FULL_ASSERT */


/**
  * @}
  */

/**
  * @}
  */

