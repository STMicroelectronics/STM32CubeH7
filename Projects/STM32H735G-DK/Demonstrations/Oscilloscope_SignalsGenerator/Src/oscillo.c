/**
  ******************************************************************************
  * @file    Demonstration/Oscilloscope_SignalsGenerator/Src/oscillo.c
  * @author  MCD Application Team
  * @brief   This demonstartion describes how to use STM32H7 to create a digital oscilloscope
  *          able to display and detect signals with frequency up to 2 MHz.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "oscillo.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "cmsis_os.h"

/** @addtogroup STM32H7xx_HAL_Demonstration
  * @{
  */

/** @addtogroup Oscilloscope_SignalsGenerator
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Definition of ADCx conversions data table size */
#define SAMPLES_BUFFER_SIZE   ((uint32_t)  10240)   /* Size of array including ADC's Samlpes[] */
/* Private macro -------------------------------------------------------------*/
#define ADAPT_SAMPLES_BUFFER_SIZE(v) (SAMPLES_BUFFER_SIZE/(1+(4000/(v))))


#define mainADCCONV_TASK_PRIORITY			( configMAX_PRIORITIES - 2  )
osSemaphoreId osSemaphore;
/* ADCs,TIM,UART and DMA handlers declaration */  
ADC_HandleTypeDef             AdcHandle_Master;
ADC_HandleTypeDef             AdcHandle;
ADC_HandleTypeDef             AdcHandle_Slave;
TIM_HandleTypeDef             TimHandle;
UART_HandleTypeDef            UartHandle;
DMA_HandleTypeDef             DmaHandle;

/* Buffer containing the single and the dual mode ADC conversions data  */
ALIGN_32BYTES (uint16_t   Samples[SAMPLES_BUFFER_SIZE]);

/* Variable containing the Received User command */
uint8_t Received_Command[Received_Command_Size];

/* Variable containing the new frequency conversion value selected by the PC oscilloscope (Khz)*/
uint32_t Conversion_Frequency_changed ;
uint32_t Conversion_Frequency ;
/* Private  variables*/
static uint32_t TransmitSuspend = 0;
static uint32_t Prev_Sampling_Mode = 1;
__IO static uint32_t Conversion_Complete = 0;

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);
static void ADC_Config(void);
static void DMA_Config(void);
static void TIM_Config(void);
static void UART_Config(void);
static void TIM_Reconfig(void);
static void Samples_Send(void);
static void Oscilloscope_Start(void);
static void Oscilloscope_Receiving_Process(void);
static void ADC_CONV_Thread(void const *argument);
 
void OscilloThread( void *pvParameters )
{

  /* LED Initialization */
  BSP_LED_Init(LED2);

    /* Define used semaphore */
  osSemaphoreDef(SEM);
  /* Create the semaphore */
  osSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1);  

  /*Secondary Task for Samples conversion and sending*/
  osThreadDef(ADC_Thread, ADC_CONV_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2); 
  osThreadCreate(osThread(ADC_Thread), 0);

  /*  Oscilloscope initialization, Default configurations and starting */
  Oscilloscope_Start();

  while (1)
  {

    BSP_LED_Toggle(LED2); 
    osDelay(100);
    /* Put UART peripheral in reception process */
    /* Any data received will be stored "Received_Command" buffer : the number max of
    data received is 10 */
    Oscilloscope_Receiving_Process();

    
  }  
  
}



static void ADC_CONV_Thread(void const *argument)
{

  while (1)
  {
    
    
    if (osSemaphore != NULL)
    {
      /* Try to obtain the semaphore */
      if(osSemaphoreWait(osSemaphore , 0) == osOK)
      {
        
        Conversion_Frequency = Conversion_Frequency_changed;
        
        switch (Conversion_Frequency) /* Dual Mode Data management */
        { 
        case 8000:     
          {

            /* Transfer samples */
            Samples_Send();
            /* Reconfigure TIM3 to trigger the ADC with the new conversion frequency */  
            TIM_Reconfig();
            /* Reconfigure the ADC and DMA */
            ADC_Config();
            DMA_Config();
            /* Restart conversion process */
            HAL_ADCEx_MultiModeStart_DMA(&AdcHandle_Master,(uint32_t *)Samples,ADAPT_SAMPLES_BUFFER_SIZE(Conversion_Frequency));
            Prev_Sampling_Mode=2; /* dual ADC*/
            break;
          }            
          
        case    4000 : 
        case    2000 : 
        case    1000 : 
        case    100  :
        case    10   :
        case    5     :
          {

            
            /* Transfer samples */
            Samples_Send();
            
            /* Reconfigure TIM3 to trigger the ADC with the new conversion frequency */  
            TIM_Reconfig();
            /* Reconfigure the ADC and DMA */
            ADC_Config();
            
            DMA_Config();
            /* Restart conversion process */
            HAL_ADC_Start_DMA(&AdcHandle_Master,(uint32_t *)Samples,ADAPT_SAMPLES_BUFFER_SIZE(Conversion_Frequency) );  
            Prev_Sampling_Mode=1; /* single ADC*/
            break;
          }
          
        default:
          {
          }
        }
      }
    }
    
  }  
  
}

/**
* @brief  This function is executed in purpose to configure the MCU UART.
* @param  None
* @retval None
*/
static void UART_Config(void)
{
  
  UartHandle.Instance        = USARTx;
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  
  HAL_UART_DeInit(&UartHandle);
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
* @brief  This function is executed to mmodify the Timer's Time base frequency in purpose to handle the conversion frequency.
* @param  None
* @retval None
*/
void TIM_Reconfig()
{
  uint32_t Period;
  uint32_t Prescalar;
  HAL_TIM_Base_Stop(&TimHandle);
  switch (Conversion_Frequency)
  {
    
    /*** Different configurations set by the user from the windows application, each
  case of those represent a way to configure a fixed ADC conversion frequency   **/
    
  case 5:    {Period = 399;    Prescalar =1249;}        // Timer update rate = ADC conversion Frequency = 500 Hz 
  break;
  case 10:   {Period = 19;     Prescalar =1249;}        // Timer update rate = ADC conversion Frequency = 10 KHz   
  break;
  case 100:  {Period = 1;      Prescalar =1249;}        // Timer update rate = ADC conversion Frequency = 100 KHz
  break;
  case 1000: {Period = 1;      Prescalar =124;}         // Timer update rate = ADC conversion Frequency = 1 MHz
  break;  
  case 2000: {Period = 1;      Prescalar =61;}         // Timer update rate = ADC conversion Frequency = 2 MHz
  break;
  case 4000: {Period = 31;      Prescalar =1;}         // Timer update rate = ADC conversion Frequency = 4 MHz   
  break;
  default:   {Period = 31 ;     Prescalar =1;}         // Sinon ADC frequency = ADC Frequency MAX
  break;
  
  }
  
  TimHandle.Instance = TIM_ADC;
  TIM_MasterConfigTypeDef sMasterConfig;
  TimHandle.Init.Period            = Period;
  TimHandle.Init.Prescaler         = Prescalar;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;    
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_Base_Start(&TimHandle);
}

/**
* @brief  This function is executed to send samples coming from ADC's, stocked in RAM.
* @param  None
* @retval None
*/
static void Samples_Send(void)
{
  uint32_t i = 0;
  
  SCB_InvalidateDCache_by_Addr((uint32_t *) Samples, ADAPT_SAMPLES_BUFFER_SIZE(Conversion_Frequency)*2); 

  /* there is a double process made to send data from the board to the GUI depending on
  the mode of conversion (Multi-channel,single channel) */
  if (Conversion_Frequency == 2) 
  { 
    for(i=0;  i <  (ADAPT_SAMPLES_BUFFER_SIZE(Conversion_Frequency)/4);  i++)
    {
      Samples[i] = (Samples[(2 * i)+0]* 3300) / 0x3FFF; /* Max Voltage = 3.300V, 14Bits */ 
      printf("%d,%d\r",Samples[i],Samples[i]);
      if (TransmitSuspend == 1)
      {
        TransmitSuspend=0;
        break;
      }
    }
  }
  
  else
  { 
  
    for(i=0;i<ADAPT_SAMPLES_BUFFER_SIZE(Conversion_Frequency);i++)
    {
      Samples[i] = ((Samples[i]) * 3300) /0x3FFF; /* Max Voltage = 3.300V, 14Bits */
      printf("%d\r", Samples[i]);

      if (TransmitSuspend == 1)
      {
        TransmitSuspend=0;
        break;
      }
    }
    i++;

  }
}

/**
* @brief  This Callback is generated every conversion complete status.
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

  if (Prev_Sampling_Mode == 1)
    HAL_ADC_Stop_DMA(&AdcHandle_Master);
  if (Prev_Sampling_Mode == 2)
    HAL_ADCEx_MultiModeStop_DMA(&AdcHandle_Master);

  osSemaphoreRelease(osSemaphore);

}


/**
* @brief  This Callback is generated every moment a user command is received.
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{

    TransmitSuspend = 1;
    if ((strncmp((char *)Received_Command,"8M sample\n",Received_Command_Size)==0) ||
        (strncmp((char *)Received_Command,"8 Msample\n",Received_Command_Size)==0))
    {
      Conversion_Frequency_changed = 8000;
    } 
    else if (strncmp((char *)Received_Command,"100  Ks/s\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 100;
    }
    else if (strncmp((char *)Received_Command,"1M sample\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 1000;
    }
    else if (strncmp((char *)Received_Command,"2M sample\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 2000;
    }
    else if (strncmp((char *)Received_Command,"4M sample\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 4000;
    }
    else if (strncmp((char *)Received_Command,"10Ksamp/s\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 10;
    }
    else if (strncmp((char *)Received_Command,"500Sample\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 5;
    }
    else if (strncmp((char *)Received_Command,"2channels\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 2;
    }
    else if (strncmp((char *)Received_Command,"1channels\n",Received_Command_Size)==0)
    {
      Conversion_Frequency_changed = 1;
    }
    else
    {
      
      Conversion_Frequency_changed = 1000;
    }
 
}


static void Oscilloscope_Receiving_Process(void)
{
  HAL_UART_Receive_IT(&UartHandle, (uint8_t *)Received_Command, sizeof Received_Command);
}

/**
* @brief  Configure the general purpose TIM3 to generate a TIM base coressponding
to the maximum allowed conversion frequency.
* @param  None
* @retval None
*/
static void TIM_Config(void)
{
  
  /* Set TIMx instance */
  TimHandle.Instance = TIM_ADC;
  TIM_MasterConfigTypeDef sMasterConfig; 
  TimHandle.Init.Period            = 20;
  TimHandle.Init.Prescaler         = 1;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;   
  
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &sMasterConfig) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }


  
}

/**
* @brief  Function contained the main process used by the oscilloscope. 
* @param  None
* @retval None
*/
static void Oscilloscope_Start(void)
{
  /* Default sampling rate (Khz) */
  Conversion_Frequency = 1000;
  Conversion_Frequency_changed = 1000;
    
  /*  Configure the UART */
  UART_Config();
  /*  Configure the TIM (Clock source APB(TIM clock = 200 Mhz)) */
  TIM_Config();

  /*  Configure the ADC to work with DMA (Max Conversion frequency in this example is 8 Msamples/s) */

  ADC_Config();
  /* Configure the DMA to manage data in both conversion mode of ADC(Single + Dual)*/
  DMA_Config();
  /* Time Base configuration */
  HAL_TIM_Base_Start(&TimHandle);

  /* Start the Oscilloscope  */
  HAL_ADC_Start_DMA(&AdcHandle_Master,(uint32_t *)Samples,ADAPT_SAMPLES_BUFFER_SIZE(Conversion_Frequency));
  
}

/**
* @brief  This function contains the two process of manipulation data used by the DMA 
* @param  None
* @retval None
*/
static void DMA_Config(void)
{
  if (AdcHandle_Master.Instance == ADCx)
  {  
    /* Enable clock of DMA associated to the peripheral */
    ADCx_DMA_CLK_ENABLE();
    
    /* Note: ADC slave does not need additional configuration, since it shares  */
    /*       the same clock domain, same GPIO pins (interleaved on the same     */
    /*       channel) and same DMA as ADC master.                               */
    /* Configure DMA parameters (ADC master) */
    
    DmaHandle.Instance = ADCx_DMA;
    DmaHandle.Init.Request             = DMA_REQUEST_ADC1;
    DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
    DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
    
    /* Transfer data size depends on the conversion mode (Single or Dual) */
    if (Conversion_Frequency == 8000)
    {
      DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;       /* Transfer from ADC by word to match with ADC configuration: Dual mode, ADC master contains conversion results on data register (32 bits) of ADC master and ADC slave  */
      DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;       /* Transfer to memory by word to match with buffer variable type: word */
    }
    else
    {
      DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;       /* Transfer from ADC by word to match with ADC configuration: Dual mode, ADC master contains conversion results on data register (32 bits) of ADC master and ADC slave  */
      DmaHandle.Init.MemDataAlignment    = DMA_PDATAALIGN_HALFWORD;       /* Transfer to memory by word to match with buffer variable type: word */    
    }

    DmaHandle.Init.Mode                = DMA_CIRCULAR;              /* DMA in circular mode to match with ADC configuration: DMA continuous requests */
    DmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;  
    /* Deinitialize  & Initialize the DMA for new transfer */
    HAL_DMA_DeInit(&DmaHandle);
    HAL_DMA_Init(&DmaHandle);
    
    /* Associate the initialized DMA handle to the ADC Master handle */
    __HAL_LINKDMA(&AdcHandle_Master, DMA_Handle, DmaHandle);
    
    /* NVIC configuration for DMA interrupt (transfer completion or error) */
    /* Priority: high-priority */
    HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 5,0);
    HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
  } 
}

/**
* @brief  Configure the ADC's modes and the number of channel displayed 
* @param  None
* @retval None
*/
static void ADC_Config(void)
{
  
  ADC_ChannelConfTypeDef        sConfig;
  AdcHandle_Master.Instance          = ADCx;
  ADC_MultiModeTypeDef          MultiModeInit;

  if (HAL_ADC_DeInit(&AdcHandle_Master) != HAL_OK)
  {
    Error_Handler();
  }
    if (Conversion_Frequency == 8000)
  {
    AdcHandle_Slave.Instance = ADC2;
    if (HAL_ADC_DeInit(&AdcHandle_Slave) != HAL_OK)
    {
      /* ADC initialization error */
      Error_Handler();
    }
  }
  
  AdcHandle_Master.Init.ClockPrescaler                 = ADC_CLOCK_ASYNC_DIV1;                  /* Asynchronous clock mode, input ADC clock not divided */
  AdcHandle_Master.Init.Resolution                     = ADC_RESOLUTION_14B;                    /* 14-bit resolution for converted data */
  
  /* Number of Channel displayed configuration */
  if (Conversion_Frequency == 2)
  {
    AdcHandle_Master.Init.ScanConvMode                   = ENABLE; 
    AdcHandle_Master.Init.NbrOfConversion                = 2;                                     /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  }
  else
  {
    AdcHandle_Master.Init.ScanConvMode                   = DISABLE; 
    AdcHandle_Master.Init.NbrOfConversion                = 1;                                     /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  } 
  /* Number of Channel displayed configuration */
  if (Conversion_Frequency == 8000)  
  {
    AdcHandle_Master.Init.ContinuousConvMode             = ENABLE;                                /* Continuous mode enabled (conversion restart after each one finished) */
    AdcHandle_Master.Init.ExternalTrigConv               = ADC_EXTERNALTRIG_T3_TRGO;              /* External event to trig the  conversion manually */
    AdcHandle_Master.Init.ExternalTrigConvEdge           = ADC_EXTERNALTRIGCONVEDGE_RISING;       /* Conversion triggered by TIM3 Update flag */  
  }
  else
  {
    AdcHandle_Master.Init.ContinuousConvMode             = DISABLE;                               /* Continuous mode disabled (conversion restart after each Timer Update Flag) */
    AdcHandle_Master.Init.ExternalTrigConv               = ADC_EXTERNALTRIG_T3_TRGO;              /* External event to trig the  conversion manually  */
    AdcHandle_Master.Init.ExternalTrigConvEdge           = ADC_EXTERNALTRIGCONVEDGE_RISING;       /* Conversion triggered by TIM3 Update flag */
  }
  AdcHandle_Master.Init.EOCSelection                   = ADC_EOC_SINGLE_CONV;                   /* EOC flag picked-up to indicate conversion end */
  AdcHandle_Master.Init.LowPowerAutoWait               = DISABLE;                               /* Auto-delayed conversion feature disabled */
  AdcHandle_Master.Init.DiscontinuousConvMode          = DISABLE;                               /* Parameter discarded because sequencer is disabled */
  AdcHandle_Master.Init.NbrOfDiscConversion            = 0;
  AdcHandle_Master.Init.ConversionDataManagement       = ADC_CONVERSIONDATA_DMA_CIRCULAR;       /* ADC DMA circular requested */
  AdcHandle_Master.Init.Overrun                        = ADC_OVR_DATA_OVERWRITTEN;              /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle_Master.Init.OversamplingMode               = DISABLE;                               /* No oversampling */
  
  /* ADC Initialization ############################################ */  
  if (HAL_ADC_Init(&AdcHandle_Master) != HAL_OK)
  {
    Error_Handler();
  }

  /* Channel configuration ######################################## */
  sConfig.Channel      = ADC_CHANNEL_0;                                         /* Sampled channel number */
  sConfig.Rank         = ADC_REGULAR_RANK_1;                                    /* Rank of sampled channel number ADCx_CHANNEL */
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;                               /* Sampling time (number of clock cycles unit) */
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;                                      /* Single-ended input channel */
  sConfig.OffsetNumber = ADC_OFFSET_NONE;                                       /* No offset subtraction */ 
  sConfig.Offset = 0;                                                           /* Parameter discarded because offset correction is disabled */
  if (HAL_ADC_ConfigChannel(&AdcHandle_Master, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
 
  /* channel configuration in case of the conversion of two external signal */ 
  if (Conversion_Frequency == 2)
  {
    sConfig.Channel      = ADC_CHANNEL_1;                                         /* Sampled channel number */
    sConfig.Rank         = ADC_REGULAR_RANK_2;                                    /* Rank of sampled channel number ADCx_CHANNEL */
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;                               /* Sampling time (number of clock cycles unit) */
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;                                      /* Single-ended input channel */
    sConfig.OffsetNumber = ADC_OFFSET_NONE;                                       /* No offset subtraction */ 
    sConfig.Offset = 0;                                                           /* Parameter discarded because offset correction is disabled */
    if (HAL_ADC_ConfigChannel(&AdcHandle_Master, &sConfig) != HAL_OK)
    {
      Error_Handler();
    }
  }

  
  /* Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle_Master, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* channel configuration in case of an interleaved mode conversion*/ 
  if (Conversion_Frequency == 8000)
  {  
    AdcHandle_Slave.Instance = ADC2;
    AdcHandle_Slave.Init = AdcHandle_Master.Init;
    AdcHandle_Slave.Init.ContinuousConvMode    = DISABLE;
    AdcHandle_Slave.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    if (HAL_ADC_Init(&AdcHandle_Slave) != HAL_OK)
    {
      /* ADC initialization error */
      Error_Handler();
    }
    
    sConfig.Channel = ADC_CHANNEL_0;
    if (HAL_ADC_ConfigChannel(&AdcHandle_Slave, &sConfig) != HAL_OK)
    {
      /* Channel Configuration Error */
      Error_Handler();
    }
    if (HAL_ADCEx_Calibration_Start(&AdcHandle_Slave, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
    {
      /* Calibration Error */
      Error_Handler();
    } 
    /* Multimode parameters settings and set ADC2 (slave) under control of      */
    /* ADCx (master).                                                           */
    MultiModeInit.Mode = ADC_DUALMODE_INTERL;
    MultiModeInit.DualModeData = ADC_DUALMODEDATAFORMAT_32_10_BITS;  /* ADC and DMA configured in resolution 32 bits to match with both ADC master and slave resolution */
    MultiModeInit.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_3CYCLES;
    
    if (HAL_ADCEx_MultiModeConfigChannel(&AdcHandle_Master, &MultiModeInit) != HAL_OK)
    {
      /* Multimode Configuration Error */
      Error_Handler();
    }
  } 
}

/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED2);
  while (1)
  {
  }
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

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
  
  return ch;
}



/**
* @}
*/

/**
* @}
*/


