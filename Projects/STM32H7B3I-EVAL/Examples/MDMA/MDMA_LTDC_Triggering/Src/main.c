/**
  ******************************************************************************
  * @file    MDMA/MDMA_LTDC_Triggering/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use MDMA HAL API to start a transfer
  *          using the LTDC Line Interrupt flag as the MDMA transfer trigger
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
#include "Image01.h"
#include "Image02.h"
#include "Image03.h"
#include "Image04.h"
#include "Image05.h"
#include "Image06.h"
#include "Image07.h"
#include "Image08.h"
#include "Image09.h"
#include "Image10.h"
#include "Image11.h"
#include "Image12.h"
#include "Image13.h"
#include "Image14.h"
#include "Image15.h"
#include "Image16.h"
#include "Image17.h"
#include "Image18.h"
#include "Image19.h"
#include "Image20.h"
#include "Image21.h"
#include "Image22.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup MDMA_LTDC_Triggering
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MDMA_IMAGE_NB 22
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MDMA_HandleTypeDef     MDMA_Handle;
extern LTDC_HandleTypeDef  hltdc_eval;

DMA_HandleTypeDef DMA_Handlel1;
DMA_HandleTypeDef DMA_Handlel2;
LPTIM_HandleTypeDef  LptimHandle;

/* ARGB8888 Default Color used to clear the LCD */
uint32_t LCD_Fill_Color = UTIL_LCD_COLOR_BLACK;

__IO uint32_t MDMA_TransferErrorDetected = 0;

static uint32_t LCD_X_Size = 0, LCD_Y_Size = 0;

uint32_t dma2d_destination = 0;
uint32_t mdma_destination  = 0;
uint32_t dma2d_source      = 0;

uint32_t dummy_source = 0;
uint32_t dummy_destination = 0;


/* LCD Offset calculatd versus the image width */
uint32_t LCD_Offset = 0;


/*
According to the Ref Manual :
  The channel configuration (LAR address) must be in the AXI address space.
  LAR value must be aligned at a Double Word address, i.e. LAR[2:0] = 0x0

The LAR address register represents the next Linked list Node address.
As consequence the Linked list transfer nodes must be 64 bits aligned and
must be in the AXI address space.
*/
/* 32-bytes Alignment is needed for cache maintenance purpose */
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_Node);
ALIGN_32BYTES(MDMA_LinkNodeTypeDef Xfer_DummyNode);

/*List of the images to be transferred to the LCD frame buffer using the MDMA upon an LTDC Line Interrupt Flag*/
/* 32-bytes Alignment is needed for cache maintenance purpose */
ALIGN_32BYTES(uint32_t Nodes_SourceAddress[MDMA_IMAGE_NB]);

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma);
static void MDMA_Config(void);
void DMA_Config(void);
void LPTIM_Config(void);
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
  uint32_t hal_status = HAL_OK;


  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  /* Configure the MPU attributes as Write Through */
  MPU_Config();

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

  /* Configure the system clock to 280 MHz */
  SystemClock_Config();

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Initialize the LCD   */
  BSP_LCD_Init(0,LCD_ORIENTATION_LANDSCAPE);

  /* Link Board LCD drivers to BASIC GUI LCD drivers*/
  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

  /* Get the LCD Width and Height */
  BSP_LCD_GetXSize(0,&LCD_X_Size);
  BSP_LCD_GetYSize(0,&LCD_Y_Size);

  /* LCD Offset calculated versus the image width */
  LCD_Offset = (LCD_X_Size - IMAGE_WIDTH) * ARGB8888_BYTES_PER_PIXEL;

  /*List of the images to be transferred to the LCD frame buffer using the MDMA upon an LTDC Line Interrupt Flag*/
   Nodes_SourceAddress[0] = (uint32_t)Image01;
   Nodes_SourceAddress[1] = (uint32_t)Image02;
   Nodes_SourceAddress[2] = (uint32_t)Image03;
   Nodes_SourceAddress[3] = (uint32_t)Image04;
   Nodes_SourceAddress[4] = (uint32_t)Image05;
   Nodes_SourceAddress[5] = (uint32_t)Image06;
   Nodes_SourceAddress[6] = (uint32_t)Image07;
   Nodes_SourceAddress[7] = (uint32_t)Image08;
   Nodes_SourceAddress[8] = (uint32_t)Image09;
   Nodes_SourceAddress[9] = (uint32_t)Image10;
   Nodes_SourceAddress[10] = (uint32_t)Image11;
   Nodes_SourceAddress[11] = (uint32_t)Image12;
   Nodes_SourceAddress[12] = (uint32_t)Image13;
   Nodes_SourceAddress[13] = (uint32_t)Image14;
   Nodes_SourceAddress[14] = (uint32_t)Image15;
   Nodes_SourceAddress[15] = (uint32_t)Image16;
   Nodes_SourceAddress[16] = (uint32_t)Image17;
   Nodes_SourceAddress[17] = (uint32_t)Image18;
   Nodes_SourceAddress[18] = (uint32_t)Image19;
   Nodes_SourceAddress[19] = (uint32_t)Image20;
   Nodes_SourceAddress[20] = (uint32_t)Image21;
   Nodes_SourceAddress[21] = (uint32_t)Image22;
   /*It is necessary to clean the cache, this buffer will be used by the DMA*/
   SCB_CleanDCache_by_Addr((uint32_t *)Nodes_SourceAddress, sizeof(Nodes_SourceAddress));

   /*##-1- Config and Start the DMA Streams transfer process ###################*/
  DMA_Config();

  /*##-2- Config the MDMA Channel  ###########################################*/
  MDMA_Config();

  /*##-3- Config and Start the LPTIM1 (Transfer Trigger of DMA1 Stream 0 ) ###*/
  LPTIM_Config();


  /*##-4- Config The LTDC Line Interrupt to LCD height/2 ###*/
  HAL_LTDC_ProgramLineEvent(&hlcd_ltdc, LCD_Y_Size/2);


  /* Start MDMA transfer in Repeat Block to clear the LCD frame buffer with default color
     Then to transfer a new image to the LCD frame buffer upon each LTDC Line Interrupt Flag */
  hal_status = HAL_MDMA_Start_IT(&MDMA_Handle, (uint32_t)&LCD_Fill_Color  ,
                                               (uint32_t)LCD_FRAME_BUFFER ,
                                               IMAGE_WIDTH * ARGB8888_BYTES_PER_PIXEL,
                                               IMAGE_HEIGHT);

  if(hal_status != HAL_OK)
  {
    /* Transfer Error */
    Error_Handler();
  }

  /* The CPU can go to sleep mode, the images animation will be handled by the MDMA */
  HAL_SuspendTick();
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  HAL_ResumeTick();
  /* Infinite loop */
  while (1)
  {

  }
}

/**
  * @brief  Initialize the MDMA For repeat block transfer in linked list circular
    repeat block transfer.
  * @param  None
  * @retval None
  */
static void MDMA_Config(void)
{
  uint32_t hal_status = HAL_OK;
  MDMA_LinkNodeConfTypeDef mdmaLinkNodeConfig;

  /*##-1- Enable the MDMA clock ###############################################*/
  __HAL_RCC_MDMA_CLK_ENABLE();

  /*##-2- Select the MDMA instance to be used : MDMA_Channel0 #################*/
  MDMA_Handle.Instance = MDMA_Channel0;

  /*##-3- Configure the MDMA for block transfer in HW request mode ############*/
  /*
     This is the Node 0 of the linked list, node 0 will be executed only one time
     to clear the LCD frame buffer with default color
  */

  MDMA_Handle.Init.Request              = MDMA_REQUEST_LTDC_LINE_IT;
  MDMA_Handle.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;
  MDMA_Handle.Init.Priority             = MDMA_PRIORITY_HIGH;
  MDMA_Handle.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  MDMA_Handle.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
  MDMA_Handle.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  MDMA_Handle.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  MDMA_Handle.Init.BufferTransferLength = 32;

  /* Source and Destination data size are word, 4 bytes that correspond to an ARGB8888 pixel 32 bpp */
  MDMA_Handle.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  MDMA_Handle.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;

  /* Source Increment Disabled as the source is always the LCD_Fill_Color
     that represents the default color used to fill the LCD frame buffer */
  MDMA_Handle.Init.SourceInc            = MDMA_SRC_INC_DISABLE;
  /* Destination Increment is word , 4 bytes that correspond to an ARGB8888 pixel 32 bpp */
  MDMA_Handle.Init.DestinationInc       = MDMA_DEST_INC_WORD;

  /* Source block offset set to Zero : no increment of the source block address */
  MDMA_Handle.Init.SourceBlockAddressOffset  = 0;

  /* Destination block offset set to LCD_Offset */
  MDMA_Handle.Init.DestBlockAddressOffset    = LCD_Offset;

  /*##-4- Initialize the MDMA channel ##########################################*/
  hal_status = HAL_MDMA_Init(&MDMA_Handle);

  if(hal_status != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* MDMA Post request address and  Mask set respectively to the LTDC Interrupt Clear register address
     and the Clear Line Interrupt Flag mask in order to clear the LTDC Line Interrupt flag
     after each transfer knowing that this last flag is the MDMA transfer trigger
  */
  HAL_MDMA_ConfigPostRequestMask(&MDMA_Handle, (uint32_t)(&(LTDC->ICR)), LTDC_ICR_CLIF);

  /*##-5- Create Linked list Nodes ############################################*/

  /* Node 1 : First node is used to transfer an image from the flash to the LCD frame buffer
    The MDMA transfer trigger is set to the LTDC Line Interrupt flag.
  */
  mdmaLinkNodeConfig.Init.Request              = MDMA_REQUEST_LTDC_LINE_IT;
  mdmaLinkNodeConfig.Init.TransferTriggerMode  = MDMA_REPEAT_BLOCK_TRANSFER;
  mdmaLinkNodeConfig.Init.Priority             = MDMA_PRIORITY_HIGH;
  mdmaLinkNodeConfig.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;

  mdmaLinkNodeConfig.Init.SourceInc            = MDMA_SRC_INC_WORD;
  mdmaLinkNodeConfig.Init.DestinationInc       = MDMA_DEST_INC_WORD;

  mdmaLinkNodeConfig.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  mdmaLinkNodeConfig.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  mdmaLinkNodeConfig.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
  mdmaLinkNodeConfig.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.BufferTransferLength = 32;

  mdmaLinkNodeConfig.Init.SourceBlockAddressOffset  = 0;
  mdmaLinkNodeConfig.Init.DestBlockAddressOffset    = LCD_Offset;

  /* MDMA Post request address and Mask set respectively to the LTDC Interrupt Clear register address
     and the Clear Line Interrupt Flag mask in order to clear the LTDC Line Interrupt flag
     after each transfer knowing that this last flag is the MDMA transfer trigger
  */
  mdmaLinkNodeConfig.PostRequestMaskAddress = (uint32_t)(&(LTDC->ICR));
  mdmaLinkNodeConfig.PostRequestMaskData = LTDC_ICR_CLIF;

  /* Destination Address is set to the LCD_FRAME_BUFFER address*/
  mdmaLinkNodeConfig.DstAddress      = LCD_FRAME_BUFFER;
  /* BlockDataLength is set to the size in bytes of the image width (line size) */
  mdmaLinkNodeConfig.BlockDataLength = IMAGE_WIDTH * ARGB8888_BYTES_PER_PIXEL;
  /* BlockCount is set to the image height (number of lines) */
  mdmaLinkNodeConfig.BlockCount      = IMAGE_HEIGHT;

  /* Source Address is set to the first image address */
  mdmaLinkNodeConfig.SrcAddress = Nodes_SourceAddress[0];

  /* Create Node 1*/
  HAL_MDMA_LinkedList_CreateNode(&Xfer_Node, &mdmaLinkNodeConfig);
  /*Add created Node to the linkedlist */
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_Node, 0);


  /* Node 2 : Dummy node (inserted to able to wait for Node 1 source address update done by DMA1 stream 0 transfer)
    For this node the MDMA transfer trigger is set to DMA1 stream1 Transfer complete flag.
    Knowing that DMA1 stream1 (circular) transfer is a dummy transfer triggered by DMA1 Stream0
    transfer event
  */
  mdmaLinkNodeConfig.Init.Request              = MDMA_REQUEST_DMA1_Stream1_TC;
  mdmaLinkNodeConfig.Init.TransferTriggerMode  = MDMA_BUFFER_TRANSFER;
  mdmaLinkNodeConfig.Init.Priority             = MDMA_PRIORITY_HIGH;
  mdmaLinkNodeConfig.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;

  mdmaLinkNodeConfig.Init.SourceInc            = MDMA_SRC_INC_WORD;
  mdmaLinkNodeConfig.Init.DestinationInc       = MDMA_DEST_INC_WORD;

  mdmaLinkNodeConfig.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  mdmaLinkNodeConfig.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  mdmaLinkNodeConfig.Init.DataAlignment        = MDMA_DATAALIGN_PACKENABLE;
  mdmaLinkNodeConfig.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;
  mdmaLinkNodeConfig.Init.BufferTransferLength = 32;

  mdmaLinkNodeConfig.Init.SourceBlockAddressOffset  = 0;
  mdmaLinkNodeConfig.Init.DestBlockAddressOffset    = 0;

  /* MDMA Post request address and  Mask set respectively to the DMA1 stream1 Interrupt Clear register address
     and the DMA1 stream1 Clear Transfer Interrupt flag mask in order to clear the DMA1 stream1
     Transfer complete flag
  */
  mdmaLinkNodeConfig.PostRequestMaskAddress = (uint32_t)(&(DMA1->LIFCR));
  mdmaLinkNodeConfig.PostRequestMaskData = DMA_LIFCR_CTCIF1;

  /* Source and destination address are dummy , BlockDataLength  is 4 bytes and block count is 1*/
  mdmaLinkNodeConfig.DstAddress      = (uint32_t)(&dummy_destination);
  mdmaLinkNodeConfig.BlockDataLength = 4;
  mdmaLinkNodeConfig.BlockCount      = 1;

  mdmaLinkNodeConfig.SrcAddress = (uint32_t)(&dummy_source);

  HAL_MDMA_LinkedList_CreateNode(&Xfer_DummyNode, &mdmaLinkNodeConfig);
  /*Add created Node to the linkedlist */
  HAL_MDMA_LinkedList_AddNode(&MDMA_Handle, &Xfer_DummyNode, 0);


  /* Make the linked list circular */
  HAL_MDMA_LinkedList_EnableCircularMode(&MDMA_Handle);

  /*
    As the MDMA Node descriptor "Xfer_Node" is located in the D1 AXI-SRAM which
    is cacheable, it is necessary to clean the data cache after creating the node
    in order to make sure that the MDMA will load up to date data from the linked list node
  */
  SCB_CleanDCache_by_Addr((uint32_t *)&(Xfer_Node), sizeof(MDMA_LinkNodeTypeDef));
  SCB_CleanDCache_by_Addr((uint32_t *)&(Xfer_DummyNode), sizeof(MDMA_LinkNodeTypeDef));

  /*##-6- Select Callbacks functions called in case of MDMA Transfer error */
  HAL_MDMA_RegisterCallback(&MDMA_Handle, HAL_MDMA_XFER_ERROR_CB_ID, MDMA_TransferErrorCallback);

  /*##-7- Configure NVIC for MDMA transfer complete/error interrupts ##########*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);

  /* Enable the MDMA channel global Interrupt */
  HAL_NVIC_EnableIRQ(MDMA_IRQn);
}


void DMA_Config(void)
{
  HAL_DMA_MuxRequestGeneratorConfigTypeDef dmamux_ReqGenParams;
  HAL_DMA_MuxSyncConfigTypeDef dmamux_syncParams;

  /* Enable BDMA clock */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA1_Stream0     */
  /* DMA mode is set to circular for an infinite DMA transfer :
     This transfer is used to update the MDMA Node 1 source address
     to the next image address each time.
     This transfer is triggered by the DMA request generator 0 with Signal ID set to
     LPTIM1 output knoing that the LPTIM1 is configured with 100ms auto-reload
  */
  DMA_Handlel1.Instance                 = DMA1_Stream0;

  DMA_Handlel1.Init.Request             = DMA_REQUEST_GENERATOR0;
  DMA_Handlel1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  DMA_Handlel1.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMA_Handlel1.Init.MemInc              = DMA_MINC_ENABLE;
  DMA_Handlel1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DMA_Handlel1.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  DMA_Handlel1.Init.Mode                = DMA_CIRCULAR;
  DMA_Handlel1.Init.Priority            = DMA_PRIORITY_LOW;
  DMA_Handlel1.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  DMA_Handlel1.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  DMA_Handlel1.Init.MemBurst            = DMA_MBURST_SINGLE;
  DMA_Handlel1.Init.PeriphBurst         = DMA_PBURST_SINGLE;

  DMA_Handlel1.XferCpltCallback         = NULL;
  DMA_Handlel1.XferErrorCallback        = NULL;

  /* Initialize the DMA with for Transmission process */
  HAL_DMA_Init(&DMA_Handlel1);

  /*##-3- Configure and enable the DMAMUX Request generator  ####################*/
  dmamux_ReqGenParams.SignalID  = HAL_DMAMUX1_REQ_GEN_LPTIM1_OUT; /* External request signal is EXTI0 signal */
  dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_RISING;      /* External request signal edge is Rising  */
  dmamux_ReqGenParams.RequestNumber = 1;                          /* 1 requests on each edge of the external request signal  */

  /* Configure the DMAMUX Synchronization with Synchro disabled and event generation enabled
     in order to generate an event on each transfer request that will trig the second DMA (DMA_Handlel2)
  */
  dmamux_syncParams.EventEnable    = ENABLE;                 /* Enable DMAMUX event generation each time  RequestNumber are passed from DMAMUX to the DMA */
  dmamux_syncParams.SyncPolarity  = HAL_DMAMUX_SYNC_RISING;  /* Synchronization edge is Rising  */
  dmamux_syncParams.RequestNumber = 1;                       /* 1 requests are autorized after each edge of the sync signal */
  dmamux_syncParams.SyncSignalID  = 0;                       /* No need for synchro signal as the   Synchronization is disabled */
  dmamux_syncParams.SyncEnable     = DISABLE;                /* Synchronization is disabled */

  HAL_DMAEx_ConfigMuxSync(&DMA_Handlel1, &dmamux_syncParams);


  HAL_DMAEx_ConfigMuxRequestGenerator(&DMA_Handlel1, &dmamux_ReqGenParams);
  HAL_DMAEx_EnableMuxRequestGenerator (&DMA_Handlel1);

 /* Config DMA1_Stream1 */
 /* DMA mode is set to circular for an infinite DMA transfer.
    This transfer is a dummy transfer synchronized with DMA1 Stream0 transfer event.
    The MDMA Node2 is a dummy transfer that is triggered with this DMA1 stream1 transfer complete
    which indicates that DMA1 Stream0 has updated the Node1 source address with the next image address
 */
  DMA_Handlel2.Instance                 = DMA1_Stream1;

  DMA_Handlel2.Init.Request             = DMA_REQUEST_GENERATOR1;
  DMA_Handlel2.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  DMA_Handlel2.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMA_Handlel2.Init.MemInc              = DMA_MINC_DISABLE;
  DMA_Handlel2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DMA_Handlel2.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  DMA_Handlel2.Init.Mode                = DMA_CIRCULAR;
  DMA_Handlel2.Init.Priority            = DMA_PRIORITY_LOW;
  DMA_Handlel2.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  DMA_Handlel2.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  DMA_Handlel2.Init.MemBurst            = DMA_MBURST_SINGLE;
  DMA_Handlel2.Init.PeriphBurst         = DMA_PBURST_SINGLE;

  DMA_Handlel2.XferCpltCallback         = NULL;
  DMA_Handlel2.XferErrorCallback        = NULL;

  /* Initialize the DMA with for Transmission process */
  HAL_DMA_Init(&DMA_Handlel2);

  /*##-3- Configure and enable the DMAMUX Request generator  ####################*/
  dmamux_ReqGenParams.SignalID  = HAL_DMAMUX1_REQ_GEN_DMAMUX1_CH0_EVT; /* External request signal DMAMUX1 CH0 event : i.e DMA1_Steam0 event */
  dmamux_ReqGenParams.Polarity  = HAL_DMAMUX_REQ_GEN_RISING;           /* External request signal edge is Rising  */
  dmamux_ReqGenParams.RequestNumber = 1;                                   /* 1 requests on each edge of the external request signal  */

  HAL_DMAEx_ConfigMuxRequestGenerator(&DMA_Handlel2, &dmamux_ReqGenParams);
  HAL_DMAEx_EnableMuxRequestGenerator (&DMA_Handlel2);

  /* Start DMAs*/
  HAL_DMA_Start_IT(&DMA_Handlel1, (uint32_t)Nodes_SourceAddress, (uint32_t)(&(Xfer_Node.CSAR)), MDMA_IMAGE_NB);
  HAL_DMA_Start_IT(&DMA_Handlel2, (uint32_t)(&dummy_source), (uint32_t)(&dummy_destination), 1);

}

/**
  * @brief  Configure and start the LPTIM1 with 100ms period and 50% duty cycle.
  * @param  None
  * @retval None
  */
void LPTIM_Config(void)
{

  uint32_t periodValue;
  uint32_t pulseValue ;

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;


  /* Enable the LSE clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* LPTIM1 clock source set to LSE*/
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  PeriphClkInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  periodValue =  (2 * (LSE_VALUE/20))/4;  /* Calculate the Timer Auto-reload value for 100ms period */
  pulseValue  = periodValue/2;            /* Set the Timer pulse value for 50% duty cycle         */

  /* TIM1 Peripheral clock enable */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  LptimHandle.Instance                           = LPTIM1;

  LptimHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LptimHandle.Init.UpdateMode                    = LPTIM_UPDATE_ENDOFPERIOD;
  LptimHandle.Init.OutputPolarity                = LPTIM_OUTPUTPOLARITY_LOW;
  LptimHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LptimHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV4;
  LptimHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LptimHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LptimHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LptimHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LptimHandle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;

  if(HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start the timer */
  if (HAL_LPTIM_PWM_Start(&LptimHandle, periodValue, pulseValue) != HAL_OK)
  {
    Error_Handler();
  }

}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 280000000 (CPU Clock)
  *            HCLK(Hz)                       = 280000000 (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            CD APB3 Prescaler              = 2 (APB3 Clock  140MHz)
  *            CD APB1 Prescaler              = 2 (APB1 Clock  140MHz)
  *            CD APB2 Prescaler              = 2 (APB2 Clock  140MHz)
  *            SRD APB4 Prescaler             = 2 (APB4 Clock  140MHz)
  *            HSE Frequency(Hz)              = 24000000
  *            PLL_M                          = 12
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
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 280;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
  while(1) {};
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
  while(1) {};
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
  * @note   The Base Address is SDRAM_DEVICE_ADDR
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while(1)
  {
  }
}

/**
  * @brief  MDMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during MDMA transfer
  * @retval None
  */
static void MDMA_TransferErrorCallback(MDMA_HandleTypeDef *hmdma)
{
  MDMA_TransferErrorDetected = 1;

  /* Turn LED2 on: MDMA Transfer error */
  BSP_LED_On(LED2);
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

