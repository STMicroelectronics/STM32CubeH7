/**
******************************************************************************
* @file    OpenAMP/OpenAMP_RTOS_PingPong/CM7/Src/main.c
* @author  MCD Application Team
* @brief   This example describes how to configure and use OpenAMP MW.
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

/** @addtogroup OpenAMP_RTOS_PingPong
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define RPMSG_SERVICE_NAME              "openamp_pingpong_demo"

/* Private variables ---------------------------------------------------------*/
static  uint32_t message;
static volatile int message_received;
static volatile unsigned int received_data;

static struct rpmsg_endpoint rp_endpoint;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

HSEM_TypeDef * HSEM_DEBUG= HSEM;

static int rpmsg_recv_callback(struct rpmsg_endpoint *ept, void *data,
               size_t len, uint32_t src, void *priv)
{
  received_data = *((unsigned int *) data);
  message_received=1;

  return 0;
}

unsigned int receive_message(void)
{
  while (message_received == 0)
  {
    OPENAMP_check_for_message();
  }
  message_received = 0;

  return received_data;
}

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{
  int32_t status = 0;

  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
  /*HW semaphore Notification enable*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

  /* When system initialization is finished, Cortex-M7 will release Cortex-M4  by means of
  HSEM notification */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

  /* Initialize HAL : systick*/
  if (HAL_Init() != HAL_OK)
  {
    Error_Handler();
  }

  /*Clear Flags generated during the wakeup notification */
  HSEM_COMMON->ICR |= ((uint32_t)__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  HAL_NVIC_ClearPendingIRQ(HSEM2_IRQn);

  /* Add Cortex-M4 user application code here */

  /* Inilitize the mailbox use notify the other core on new message */
  MAILBOX_Init();

  /* Inilitize OpenAmp and libmetal libraries */
  if (MX_OPENAMP_Init(RPMSG_REMOTE, NULL)!= HAL_OK)
    Error_Handler();

  /* create a endpoint for rmpsg communication */
  status = OPENAMP_create_endpoint(&rp_endpoint, RPMSG_SERVICE_NAME, RPMSG_ADDR_ANY,
                                    rpmsg_recv_callback, NULL);
  if (status < 0)
  {
    Error_Handler();
  }

  /* Pingpong application*/
  /* Reveice an integer from the master, incremennt it and send back the result to the master*/
  while (message < 100)
  {
    message = receive_message();
    message++;
    status = OPENAMP_send(&rp_endpoint, &message, sizeof(message));
    if (status < 0)
    {
      Error_Handler();
    }
  }

  /* Deinitialize OpenAMP */
  OPENAMP_DeInit();

  while(1)
  {
  }

}



/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
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
