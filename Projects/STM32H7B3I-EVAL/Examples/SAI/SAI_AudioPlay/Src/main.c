/**
  ******************************************************************************
  * @file    SAI/SAI_AudioPlay/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use SAI HAL API to realize
  *          audio play.
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

/** @addtogroup SAI_AudioPlay
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AUDIO_FILE_ADDRESS   0x08080000
#define AUDIO_FILE_SIZE      (180*1024)
#define PLAY_HEADER          0x2C
#define PLAY_BUFF_SIZE       4096
#define AUDIO_I2C_ADDRESS    0x94U
#define AUDIO_FREQUENCY_22K  22050U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SAI_HandleTypeDef            SaiHandle;
DMA_HandleTypeDef            hSaiDma;
static AUDIO_Drv_t *AudioDrv = NULL;
void *AudioCompObj;

/* Buffer location should aligned to cache line size (32 bytes) */
ALIGN_32BYTES (uint16_t PlayBuff[PLAY_BUFF_SIZE]);

__IO int16_t                 UpdatePointer = -1;
/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void Playback_Init(void);
static void CPU_CACHE_Enable(void);
static int32_t CS42L51_Probe(void);
static int32_t CS42L51_PowerUp(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t PlaybackPosition   = PLAY_BUFF_SIZE + PLAY_HEADER;

  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

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

  /* Configure the system clock to have a frequency of 280 MHz */
  SystemClock_Config();

  /* Configure LED1 */
  BSP_LED_Init(LED1);
  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Check if the buffer has been loaded in flash */
  if(*((uint64_t *)AUDIO_FILE_ADDRESS) != 0x017EFE2446464952 ) Error_Handler();

  /* Initialize playback */
  Playback_Init();

  /* Initialize the data buffer */
  for(int i=0; i < PLAY_BUFF_SIZE; i+=2)
  {
    PlayBuff[i]=*((__IO uint16_t *)(AUDIO_FILE_ADDRESS + PLAY_HEADER + i));
  }
    
  /* Start the playback */
  if(0 != AudioDrv->Play(AudioCompObj))
  {
    Error_Handler();
  }

  if(HAL_OK != HAL_SAI_Transmit_DMA(&SaiHandle, (uint8_t *)PlayBuff, PLAY_BUFF_SIZE))
  {
    Error_Handler();
  }

  /* Start loopback */
  while(1)
  {
    BSP_LED_Toggle(LED1);

    /* Wait a callback event */
    while(UpdatePointer==-1);

    int position = UpdatePointer;
    UpdatePointer = -1;

    /* Update the first or the second part of the buffer */
    for(int i = 0; i < PLAY_BUFF_SIZE/2; i++)
    {
      PlayBuff[i+position] = *(uint16_t *)(AUDIO_FILE_ADDRESS + PlaybackPosition);
      PlaybackPosition+=2;
    }

    /* Clean Data Cache to update the content of the SRAM */
    SCB_CleanDCache_by_Addr((uint32_t*)&PlayBuff[position], PLAY_BUFF_SIZE);
	
    /* check the end of the file */
    if((PlaybackPosition+PLAY_BUFF_SIZE/2) > AUDIO_FILE_SIZE)
    {
      PlaybackPosition = PLAY_HEADER;
    }

    if(UpdatePointer != -1)
    {
      /* Buffer update time is too long compare to the data transfer time */
      Error_Handler();
    }
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
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t CS42L51_Probe(void)
{
  int32_t                   ret = BSP_ERROR_NONE;
  CS42L51_IO_t              IOCtx;
  static CS42L51_Object_t   CS42L51Obj;
  uint32_t                  cs42l51_id;

  /* Configure the audio driver */
  IOCtx.Address     = AUDIO_I2C_ADDRESS;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  if(CS42L51_RegisterBusIO (&CS42L51Obj, &IOCtx) != CS42L51_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else if(CS42L51_ReadID(&CS42L51Obj, &cs42l51_id) != CS42L51_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if((cs42l51_id & CS42L51_ID_MASK) != CS42L51_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    AudioDrv = (AUDIO_Drv_t *) &CS42L51_Driver;
    AudioCompObj = &CS42L51Obj;
  }

  return ret;
}

/**
  * @brief  Un-reset CS42L51.
  * @param  None.
  * @retval BSP status.
  */
static int32_t CS42L51_PowerUp(void)
{
  int32_t ret = BSP_ERROR_NONE;;
  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin       = GPIO_PIN_4;
  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( GPIOB, &GPIO_InitStruct);

#if (USE_BSP_IO_CLASS == 1)
  BSP_IO_Init_t IO_Init;

  /* Initialize the BSP IO driver and configure the CS42L51 reset pin */
  IO_Init.Pin  = IO_PIN_3;
  IO_Init.Mode = IO_MODE_OUTPUT_PP;
  IO_Init.Pull = IO_NOPULL;
  if(BSP_IO_Init(0, &IO_Init) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else if (BSP_IO_WritePin(0, IO_PIN_3, IO_PIN_RESET) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    /* Un-reset the CS42L51 */
    HAL_Delay(10);
    if(BSP_IO_WritePin(0, IO_PIN_3, IO_PIN_SET) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      HAL_Delay(10);
    }
  }

  /* Wait 1ms according CS42L51 datasheet */
  HAL_Delay(1);
#endif /* USE_BSP_IO_CLASS == 1 */

  return ret;
}
/**
  * @brief  Playback initialization
  * @param  None
  * @retval None
  */
static void Playback_Init(void)
{
  CS42L51_Init_t codec_init;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* Configure PLLSAI prescalers */
  /* PLL2SAI_VCO: VCO_271M
     SAI_CLK(first level) = PLLSAI_VCO/PLLSAIP = 271/24 = 11.291 Mhz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  RCC_PeriphCLKInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  RCC_PeriphCLKInitStruct.PLL2.PLL2P = 24;
  RCC_PeriphCLKInitStruct.PLL2.PLL2Q = 24;
  RCC_PeriphCLKInitStruct.PLL2.PLL2R = 1;
  RCC_PeriphCLKInitStruct.PLL2.PLL2N = 271;
  RCC_PeriphCLKInitStruct.PLL2.PLL2FRACN = 0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  RCC_PeriphCLKInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;  
  RCC_PeriphCLKInitStruct.PLL2.PLL2M = 24;

  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialize SAI */
  __HAL_SAI_RESET_HANDLE_STATE(&SaiHandle);

  SaiHandle.Instance = AUDIO_SAIx;

  __HAL_SAI_DISABLE(&SaiHandle);

  SaiHandle.Init.AudioMode      = SAI_MODEMASTER_TX;
  SaiHandle.Init.Synchro        = SAI_ASYNCHRONOUS;
  SaiHandle.Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
  SaiHandle.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
  SaiHandle.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
  SaiHandle.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_22K;
  SaiHandle.Init.Protocol       = SAI_FREE_PROTOCOL;
  SaiHandle.Init.DataSize       = SAI_DATASIZE_16;
  SaiHandle.Init.FirstBit       = SAI_FIRSTBIT_MSB;
  SaiHandle.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;
  SaiHandle.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
  SaiHandle.Init.Mckdiv         = 0; /* N.U */
  SaiHandle.Init.MonoStereoMode = SAI_STEREOMODE;
  SaiHandle.Init.CompandingMode = SAI_NOCOMPANDING;
  SaiHandle.Init.TriState       = SAI_OUTPUT_NOTRELEASED;
  SaiHandle.Init.MckOverSampling      = SAI_MCK_OVERSAMPLING_DISABLE;
  SaiHandle.Init.PdmInit.Activation   = DISABLE;

  SaiHandle.FrameInit.FrameLength       = 32;
  SaiHandle.FrameInit.ActiveFrameLength = 16;
  SaiHandle.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  SaiHandle.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  SaiHandle.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  SaiHandle.SlotInit.FirstBitOffset = 0;
  SaiHandle.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  SaiHandle.SlotInit.SlotNumber     = 2; 
  SaiHandle.SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

  if(HAL_OK != HAL_SAI_Init(&SaiHandle))
  {
    Error_Handler();
  }
  
  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(&SaiHandle);

  CS42L51_PowerUp();
  CS42L51_Probe();

  /* Fill codec_init structure */
  codec_init.InputDevice  = CS42L51_IN_NONE;
  codec_init.OutputDevice = CS42L51_OUT_HEADPHONE;
  codec_init.Frequency    = AUDIO_FREQUENCY_22K;
  codec_init.Resolution   = CS42L51_RESOLUTION_16b; /* Not used */
  codec_init.Volume       = 80;

  /* Initialize the codec internal registers */
  if(AudioDrv->Init(AudioCompObj, &codec_init) < 0) 
  {
     Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* LED2 On in error case */
  BSP_LED_On(LED2);
  while (1)
  {
  }
}

/**
  * @brief  SAI MSP Init.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  GPIO_Init;

  /* Enable SAI1 clock */
  AUDIO_SAIx_CLK_ENABLE();

  /* Configure GPIOs used for SAI1 */
  AUDIO_SAIx_MCLK_ENABLE();
  AUDIO_SAIx_SCK_ENABLE();
  AUDIO_SAIx_FS_ENABLE();
  AUDIO_SAIx_SD_ENABLE();
  
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_Init.Alternate = AUDIO_SAIx_FS_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_FS_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_FS_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_SAIx_SCK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SCK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SCK_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_SAIx_SD_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SD_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SD_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_SAIx_MCLK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_MCLK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_MCLK_GPIO_PORT, &GPIO_Init);
  
  /* Configure DMA used for SAI1 */
  __HAL_RCC_DMA2_CLK_ENABLE();

  if(hsai->Instance == AUDIO_SAIx)
  {
    hSaiDma.Init.Request             = DMA_REQUEST_SAI1_A;
    hSaiDma.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hSaiDma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hSaiDma.Init.MemInc              = DMA_MINC_ENABLE;
    hSaiDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hSaiDma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hSaiDma.Init.Mode                = DMA_CIRCULAR;
    hSaiDma.Init.Priority            = DMA_PRIORITY_HIGH;
    hSaiDma.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    hSaiDma.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hSaiDma.Init.MemBurst            = DMA_MBURST_SINGLE;
    hSaiDma.Init.PeriphBurst         = DMA_PBURST_SINGLE;

    /* Select the DMA instance to be used for the transfer : DMA2_Stream6 */
    hSaiDma.Instance                 = DMA2_Stream6;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmatx, hSaiDma);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hSaiDma);

    /* Configure the DMA Stream */
    if (HAL_OK != HAL_DMA_Init(&hSaiDma))
    {
      Error_Handler();
    }
  }

  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

/**
  * @brief Tx Transfer completed callbacks.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  UpdatePointer = PLAY_BUFF_SIZE/2;
}

/**
  * @brief Tx Transfer Half completed callbacks
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  UpdatePointer = 0;
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
  * @brief  Configure the MPU attributes
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
void assert_failed(uint8_t* file, uint32_t line)
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

