/**
  ******************************************************************************
  * @file    USB_Host/HID_RTOS/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   USB host HID Mouse and Keyboard demo main file
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

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
USBH_HandleTypeDef hUSB_Host;
HID_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
osMessageQId AppliEvent;
#define HSEM_ID_0 (0U) /* HW semaphore 0 */
/* Private function prototypes ----------------------------------------------- */
static void SystemClock_Config(void);
static void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id);
static void HID_InitApplication(void);
static void StartThread(void const *argument);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
/* Private functions --------------------------------------------------------- */

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{
  /* This project calls firstly two functions in order to configure MPU feature
  and to enable the CPU Cache, respectively MPU_Config() and CPU_CACHE_Enable()*/

  /* Configure the MPU attributes as Write Through for SDRAM*/
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
  - TIM6 timer is configured by default as source of HAL time base, but user
  can eventually implement his proper time base source (another general purpose
  timer for application or other time source), keeping in mind that Time base
  duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
  handled in milliseconds basis.
  This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms.
  The systick is then used for FreeRTOS time base.
  - Set NVIC Group Priority to 4
  - Low Level Initialization: global MSP (MCU Support Package) initialization
  */
  HAL_Init();

  /* Configure the System clock to have a frequency of 400 Mhz */
  SystemClock_Config();

  /* Enable the USB voltage level detector */
  HAL_PWREx_EnableUSBVoltageDetector();

  /* Start task */
  osThreadDef(USER_Thread, StartThread, osPriorityNormal, 0,
              8 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(USER_Thread), NULL);

  /* Create Application Queue */
  osMessageQDef(osqueue, 1, uint16_t);
  AppliEvent = osMessageCreate(osMessageQ(osqueue), NULL);

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for (;;);
}

/**
* @brief  Start task
* @param  pvParameters not used
* @retval None
*/
static void StartThread(void const *argument)
{
  osEvent event;

  /* Init HID Application */
  HID_InitApplication();

  /* Start Host Library */
  USBH_Init(&hUSB_Host, USBH_UserProcess, 0);

  /* Add Supported Class */
  USBH_RegisterClass(&hUSB_Host, USBH_HID_CLASS);

  /* Start Host Process */
  USBH_Start(&hUSB_Host);

  for (;;)
  {
    event = osMessageGet(AppliEvent, osWaitForever);

    if (event.status == osEventMessage)
    {
      switch (event.value.v)
      {
      case APPLICATION_DISCONNECT:
        Appli_state = APPLICATION_DISCONNECT;
        HID_UpdateMenu();
        break;

      case APPLICATION_READY:
        Appli_state = APPLICATION_READY;

      default:
        break;
      }
    }
  }
}

/**
* @brief  User Process
* @param  phost: Host Handle
* @param  id: Host Library user message ID
* @retval None
*/
static void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    osMessagePut(AppliEvent, APPLICATION_DISCONNECT, 0);
    break;

  case HOST_USER_CLASS_ACTIVE:
    osMessagePut(AppliEvent, APPLICATION_READY, 0);
    break;

  case HOST_USER_CONNECTION:
    osMessagePut(AppliEvent, APPLICATION_START, 0);
    break;

  default:
    break;
  }
}

/**
* @brief  HID application Init.
* @param  None
* @retval None
*/
static void HID_InitApplication(void)
{
  /* Configure Joystick in EXTI mode */
  BSP_JOY_Init(JOY1, JOY_MODE_EXTI, JOY_ALL);

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

/* Initialize the LCD */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);


  /* Initialize the LCD Log module */
  UTIL_LCD_TRACE_Init();


#ifdef USE_USB_HS
  UTIL_LCD_TRACE_SetHeader((uint8_t *) " USB OTG HS HID Host");
#else
  UTIL_LCD_TRACE_SetHeader((uint8_t *) " USB OTG FS HID Host");
#endif

  LCD_UsrTrace("USB Host library started.\n");

  /* Start HID Interface */
  USBH_UsrLog("Starting HID Demo");
  HID_MenuInit();
}

/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow :
*            System Clock source            = PLL (HSE)
*            SYSCLK(Hz)                     = 400000000 (CPU Clock)
*            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
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
*            PLL3_M                         = 5
*            PLL3_N                         = 96
*            PLL3_P                         = 2
*            PLL3_Q                         = 10
*            PLL3_R                         = 18
*            VDD(V)                         = 3.3
*            Flash Latency(WS)              = 4
* @param  None
* @retval None
*/
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

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

  /* PLL1 for System Clock */
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* PLL3 for USB Clock */

  PeriphClkInitStruct.PLL3.PLL3M = 5;
  PeriphClkInitStruct.PLL3.PLL3N = 96;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 10;
  PeriphClkInitStruct.PLL3.PLL3R = 18;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
    RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

  /*activate CSI clock mondatory for I/O Compensation Cell*/
  __HAL_RCC_CSI_ENABLE() ;

  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  /* Enables the I/O Compensation Cell */
  HAL_EnableCompensationCell();
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

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xD0000000;
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

  /* Configure the MPU attributes as Normal Non Cacheable for SRAM1 */

  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
  * @brief  LTDC Clock Config for LCD DSI display.
  * @param  hltdc  LTDC Handle
  * @retval HAL_status
  */
 HAL_StatusTypeDef MX_LTDC_ClockConfig(LTDC_HandleTypeDef *hltdc)
{
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 480 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 480/18 = 26.666 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 26.666 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;
  PeriphClkInitStruct.PLL3.PLL3N = 96;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 10;
  PeriphClkInitStruct.PLL3.PLL3R = 18;
  return HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}
#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
  * line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

