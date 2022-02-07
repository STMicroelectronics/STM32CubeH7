/**
******************************************************************************
* @file    USB_Device/HID_Standalone/CM7/Src/usbd_conf.c
* @author  MCD Application Team
* @brief   This file implements the USB Device library callbacks and MSP
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
#include "usbd_hid.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define CURSOR_STEP     5
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
PCD_HandleTypeDef hpcd;
__IO uint32_t remotewakeupon = 0;
uint8_t HID_Buffer[4];
extern USBD_HandleTypeDef USBD_Device;
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/*******************************************************************************
PCD BSP Routines
*******************************************************************************/

/**
* @brief  Initializes the PCD MSP.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_MspInit(PCD_HandleTypeDef * hpcd)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (hpcd->Instance == USB2_OTG_FS)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure DM DP Pins */
    GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure VBUS Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure ID pin */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Enable USB FS Clocks */
    __HAL_RCC_USB2_OTG_FS_CLK_ENABLE();

    /* Disable USB clock during CSleep mode */
    __HAL_RCC_USB2_OTG_FS_ULPI_CLK_SLEEP_DISABLE();

    /* Set USBFS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);

    /* Enable USBFS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

    if (hpcd->Init.low_power_enable == 1)
    {
      /* Enable EXTI Line 18 for USB wakeup */
      __HAL_USB_FS_EXTI_ENABLE_IT();

      /* Set EXTI Wakeup Interrupt priority */
      HAL_NVIC_SetPriority(OTG_FS_WKUP_IRQn, 0, 0);

      /* Enable EXTI Interrupt */
      HAL_NVIC_EnableIRQ(OTG_FS_WKUP_IRQn);
    }
  }
  else if (hpcd->Instance == USB1_OTG_HS)
  {
    /* Configure USB FS GPIOs */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    __GPIOI_CLK_ENABLE();

    /* CLK */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* D0 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* D1 D2 D3 D4 D5 D6 D7 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 |
                          GPIO_PIN_10 | GPIO_PIN_11 |
                          GPIO_PIN_12 | GPIO_PIN_13;

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* STP */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* NXT */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* DIR */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
    __HAL_RCC_USB1_OTG_HS_ULPI_CLK_ENABLE();

    /* Enable USB HS Clocks */
    __HAL_RCC_USB1_OTG_HS_CLK_ENABLE();

    /* Set USBHS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 1, 0);

    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
  }
}

/**
* @brief  De-Initializes the PCD MSP.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_MspDeInit(PCD_HandleTypeDef * hpcd)
{
  if (hpcd->Instance == USB2_OTG_FS)
  {
    /* Disable USB FS Clocks */
    __HAL_RCC_USB2_OTG_FS_CLK_DISABLE();
  }
  else if (hpcd->Instance == USB1_OTG_HS)
  {
    /* Disable USB HS Clocks */
    __HAL_RCC_USB1_OTG_HS_CLK_DISABLE();
    __HAL_RCC_USB1_OTG_HS_ULPI_CLK_DISABLE();
  }
}

/*******************************************************************************
LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/

/**
* @brief  SetupStage callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_SetupStage(hpcd->pData, (uint8_t *) hpcd->Setup);
}

/**
* @brief  DataOut Stage callback.
* @param  hpcd: PCD handle
* @param  epnum: Endpoint Number
* @retval None
*/
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_DataOutStage(hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
* @brief  DataIn Stage callback.
* @param  hpcd: PCD handle
* @param  epnum: Endpoint Number
* @retval None
*/
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_DataInStage(hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

/**
* @brief  SOF callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_SOFCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_SOF(hpcd->pData);
}

/**
* @brief  Reset callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ResetCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

  /* Set USB Current Speed */
  switch (hpcd->Init.speed)
  {
  case PCD_SPEED_HIGH:
    speed = USBD_SPEED_HIGH;
    break;

  case PCD_SPEED_FULL:
    speed = USBD_SPEED_FULL;
    break;

  default:
    speed = USBD_SPEED_FULL;
    break;
  }

  /* Reset Device */
  USBD_LL_Reset(hpcd->pData);

  USBD_LL_SetSpeed(hpcd->pData, speed);
}

/**
* @brief  Suspend callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef * hpcd)
{
  if (hpcd->Instance == USB1_OTG_HS)
  {
    __HAL_PCD_GATE_PHYCLOCK(hpcd);
    USBD_LL_Suspend(hpcd->pData);
  }
  else
  {
    __HAL_PCD_GATE_PHYCLOCK(hpcd);
    USBD_LL_Suspend(hpcd->pData);

    /* Enter in STOP mode */
    if (hpcd->Init.low_power_enable)
    {
      /* Enter Stop Mode */
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    }
  }
}

/**
* @brief  Resume callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef * hpcd)
{
  if ((hpcd->Init.low_power_enable) && (remotewakeupon == 0))
  {
    SystemClockConfig_STOP();

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &=
      (uint32_t) ~
        ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
  }
  __HAL_PCD_UNGATE_PHYCLOCK(hpcd);
  USBD_LL_Resume(hpcd->pData);
  remotewakeupon = 0;
}

/**
* @brief  ISOOUTIncomplete callback.
* @param  hpcd: PCD handle
* @param  epnum: Endpoint Number
* @retval None
*/
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_IsoOUTIncomplete(hpcd->pData, epnum);
}

/**
* @brief  ISOINIncomplete callback.
* @param  hpcd: PCD handle
* @param  epnum: Endpoint Number
* @retval None
*/
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_IsoINIncomplete(hpcd->pData, epnum);
}

/**
* @brief  ConnectCallback callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_DevConnected(hpcd->pData);
}

/**
* @brief  Disconnect callback.
* @param  hpcd: PCD handle
* @retval None
*/
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_DevDisconnected(hpcd->pData);
}


/*******************************************************************************
LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

/**
* @brief  Initializes the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef * pdev)
{
#ifdef USE_USB_FS
  /* Set LL Driver parameters */
  hpcd.Instance = USB2_OTG_FS;
  hpcd.Init.dev_endpoints = 8;
  hpcd.Init.use_dedicated_ep1 = 0;
  hpcd.Init.low_power_enable = 0;
  hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd.Init.Sof_enable = 0;
  hpcd.Init.speed = PCD_SPEED_FULL;
  hpcd.Init.vbus_sensing_enable = 0;
  hpcd.Init.lpm_enable = 0;

  /* Link The driver to the stack */
  hpcd.pData = pdev;
  pdev->pData = &hpcd;

  /* Initialize LL Driver */
  HAL_PCD_Init(&hpcd);

  HAL_PCDEx_SetRxFiFo(&hpcd, 0x80);
  HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x40);
  HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x80);

#endif

#ifdef USE_USB_HS
  /* Set LL Driver parameters */
  hpcd.Instance = USB1_OTG_HS;
  hpcd.Init.dev_endpoints = 8;
  hpcd.Init.use_dedicated_ep1 = 0;

  /* Be aware that enabling DMA mode will result in data being sent only by
  * multiple of 4 packet sizes. This is due to the fact that USB DMA does not
  * allow sending data from non word-aligned addresses. For this specific
  * application, it is advised to not enable this option unless required. */
  hpcd.Init.dma_enable = 0;
  hpcd.Init.low_power_enable = 0;
  hpcd.Init.lpm_enable = 0;
  hpcd.Init.phy_itface = PCD_PHY_ULPI;
  hpcd.Init.Sof_enable = 0;
  hpcd.Init.speed = PCD_SPEED_HIGH;
  hpcd.Init.vbus_sensing_enable = 0;

  /* Link The driver to the stack */
  hpcd.pData = pdev;
  pdev->pData = &hpcd;

  if (hpcd.Init.dma_enable == 1U)
  {
    SCB_DisableDCache();
  }

  /* Initialize LL Driver */
  HAL_PCD_Init(&hpcd);

  HAL_PCDEx_SetRxFiFo(&hpcd, 0x200);
  HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x40);
  HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x100);
#endif

  return USBD_OK;
}

/**
* @brief  De-Initializes the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef * pdev)
{
  HAL_PCD_DeInit(pdev->pData);
  return USBD_OK;
}

/**
* @brief  Starts the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef * pdev)
{
  HAL_PCD_Start(pdev->pData);
  return USBD_OK;
}

/**
* @brief  Stops the Low Level portion of the Device driver.
* @param  pdev: Device handle
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef * pdev)
{
  HAL_PCD_Stop(pdev->pData);
  return USBD_OK;
}

/**
* @brief  Opens an endpoint of the Low Level Driver.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @param  ep_type: Endpoint Type
* @param  ep_mps: Endpoint Max Packet Size
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef * pdev,
                                  uint8_t ep_addr,
                                  uint8_t ep_type, uint16_t ep_mps)
{
  HAL_PCD_EP_Open(pdev->pData, ep_addr, ep_mps, ep_type);

  return USBD_OK;
}

/**
* @brief  Closes an endpoint of the Low Level Driver.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  HAL_PCD_EP_Close(pdev->pData, ep_addr);
  return USBD_OK;
}

/**
* @brief  Flushes an endpoint of the Low Level Driver.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  HAL_PCD_EP_Flush(pdev->pData, ep_addr);
  return USBD_OK;
}

/**
* @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  HAL_PCD_EP_SetStall(pdev->pData, ep_addr);
  return USBD_OK;
}

/**
* @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef * pdev,
                                        uint8_t ep_addr)
{
  HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);
  return USBD_OK;
}

/**
* @brief  Returns Stall condition.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @retval Stall (1: Yes, 0: No)
*/
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  PCD_HandleTypeDef *hpcd = pdev->pData;

  if ((ep_addr & 0x80) == 0x80)
  {
    return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
  }
  else
  {
    return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
  }
}

/**
* @brief  Assigns a USB address to the device.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef * pdev,
                                         uint8_t dev_addr)
{
  HAL_PCD_SetAddress(pdev->pData, dev_addr);
  return USBD_OK;
}

/**
* @brief  Transmits data over an endpoint.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @param  pbuf: Pointer to data to be sent
* @param  size: Data size
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef * pdev,
                                    uint8_t ep_addr,
                                    uint8_t * pbuf, uint32_t size)
{
  HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;
}

/**
* @brief  Prepares an endpoint for reception.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @param  pbuf: Pointer to data to be received
* @param  size: Data size
* @retval USBD Status
*/
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef * pdev,
                                          uint8_t ep_addr,
                                          uint8_t * pbuf, uint32_t size)
{
  HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;
}

/**
* @brief  Returns the last transferred packet size.
* @param  pdev: Device handle
* @param  ep_addr: Endpoint Number
* @retval Received Data Size
*/
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  return HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
}

/**
* @brief  Configures system clock after wakeup from STOP mode.
* @param  None
* @retval None
*/
void SystemClockConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
  RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
  uint32_t pFLatency = 0;


  /* Get the Oscillators configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  * clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency);
}

/**
* @brief  GPIO EXTI Callback function
*         Handle remote-wakeup through key button
* @param  GPIO_Pin
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    if ((((USBD_HandleTypeDef *) hpcd.pData)->dev_remote_wakeup == 1) &&
        (((USBD_HandleTypeDef *) hpcd.pData)->dev_state == USBD_STATE_SUSPENDED))
    {
      if ((&hpcd)->Init.low_power_enable)
      {
        SystemClockConfig_STOP();
      }

      /* Ungate PHY clock */
      __HAL_PCD_UNGATE_PHYCLOCK((&hpcd));

      /* Activate Remote wakeup */
      HAL_PCD_ActivateRemoteWakeup((&hpcd));

      /* Remote wakeup delay */
      HAL_Delay(10);

      /* Disable Remote wakeup */
      HAL_PCD_DeActivateRemoteWakeup((&hpcd));

      /* change state to configured */
      ((USBD_HandleTypeDef *) hpcd.pData)->dev_state = USBD_STATE_CONFIGURED;

      /* Change remote_wakeup feature to 0 */
      ((USBD_HandleTypeDef *) hpcd.pData)->dev_remote_wakeup = 0;
      remotewakeupon = 1;
    }
    else
    {
      GetPointerData(HID_Buffer);
      USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
    }
  }

}

/**
* @brief  Gets Pointer Data.
* @param  pbuf: Pointer to report
* @retval None
*/
void GetPointerData(uint8_t *pbuf)
{
  static int8_t cnt = 0;
  int8_t  x = 0, y = 0 ;

  if(cnt++ > 0)
  {
    x = CURSOR_STEP;
  }
  else
  {
    x = -CURSOR_STEP;
  }
  pbuf[0] = 0;
  pbuf[1] = x;
  pbuf[2] = y;
  pbuf[3] = 0;
}


/**
  * @brief  Static single allocation.
  * @param  size: Size of allocated memory
  * @retval None
  */
void *USBD_static_malloc(uint32_t size)
{
  static uint32_t mem[(sizeof(USBD_HID_HandleTypeDef)/4)+1];/* On 32-bit boundary */
  return mem;
}

/**
  * @brief  Dummy memory free
  * @param  p: Pointer to allocated  memory address
  * @retval None
  */
void USBD_static_free(void *p)
{

}


/**
* @brief  Delays routine for the USB Device Library.
* @param  Delay: Delay in ms
* @retval None
*/
void USBD_LL_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

