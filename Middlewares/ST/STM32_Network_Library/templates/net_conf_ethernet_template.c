/**
  ******************************************************************************
  * @file    net_conf.c
  * @author  MCD Application Team
  * @brief   Implement functions called to initialize the Ethernet low level driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include "net_connect.h"


/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "lwip/pbuf.h"

#include "stm32f7xx_ll_utils.h"
#ifdef GENERATOR_WAKAAMACLIENT_CLOUD
/* Use a tinydtls hash function to generate an 'MCU-unique' MAC address */
#include "sha2/sha2.h"
#ifndef WITH_SHA256
#error "WITH_SHA256 must be enabled in the tinydtls configuration file"
#endif  /* WITH_SHA256 */
#define HASH_OUTPUT_LENGTH  DTLS_SHA256_DIGEST_STRING_LENGTH
#else /* GENERATOR_WAKAAMACLIENT_CLOUD */
/* Use an mbedTLS hash function to generate an 'MCU-unique' MAC address */
#include "mbedtls/sha256.h"
#define HASH_OUTPUT_LENGTH  32
#endif /* GENERATOR_WAKAAMACLIENT_CLOUD */

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* Private define ------------------------------------------------------------*/




/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Private function ---------------------------------------------------------*/
static void ethernetif_low_errhandle(void);

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ )
#pragma data_alignment = 4
#pragma location = ".sram2_non_cached_device"
static  ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */
#pragma data_alignment = 4
#pragma location = ".sram2_non_cached_device"
static ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */
#ifdef USE_CACHED_ETH_BUFFERS
#pragma data_alignment = 4
#pragma location = ".ram_cache"
#else
#pragma data_alignment = 4
#pragma location = ".sram2_non_cached_normal"
#endif /* USE_CACHED_ETH_BUFFERS */
static uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffer */
#ifdef USE_CACHED_ETH_BUFFERS
#pragma data_alignment = 4
#pragma location = ".ram_cache"
#else /* USE_CACHED_ETH_BUFFERS */
#pragma data_alignment = 4
#pragma location = ".sram2_non_cached_normal"
#endif /* USE_CACHED_ETH_BUFFERS */
static uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffer */

#elif defined(__CC_ARM)
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__((section(".sram2_non_cached_device"),
        zero_init)); /* Ethernet Rx DMA Descriptor */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__((section(".sram2_non_cached_device"),
        zero_init)); /* Ethernet Tx DMA Descriptor */
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__((section(".sram2_non_cached_normal"),
        zero_init)); /* Ethernet Receive Buffer */
uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__((section(".sram2_non_cached_normal"),
        zero_init)); /* Ethernet Transmit Buffer */
#elif defined (__GNUC__)
/* Ethernet Rx MA Descriptor */
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__((
            section(".sram2_non_cached_device")));
/* Ethernet Tx DMA Descriptor */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__((
            section(".sram2_non_cached_device")));
/* Ethernet Received Buffer */
uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__((
            section(".sram2_non_cached_normal")));
/* Ethernet Transmit Buffer */
uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__((
            section(".sram2_non_cached_normal")));
#endif /* __CC_ARM */

static uint8_t MACAddr[6] = {0x00};
static void (*event_callback)(void);
static void (*buffer_output_callback)(uint8_t *);

/* Global Ethernet handle */
ETH_HandleTypeDef EthHandle;







void HAL_ETH_MspInit(ETH_HandleTypeDef *EthHandle)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOs clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* Ethernet pins configuration ************************************************/
    /*
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PG14
    */

    /* Configure PA1, PA2 and PA7 */
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PC1, PC4 and PC5 */
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PG2, PG11, PG13 and PG14 */
    GPIO_InitStructure.Pin =  GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* Enable the Ethernet global Interrupt */
    HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);

    /* Enable ETHERNET clock  */
    __HAL_RCC_ETH_CLK_ENABLE();
}


void HAL_ETH_MspDeInit(ETH_HandleTypeDef *ethHandle)
{
    if (ethHandle->Instance == ETH)
    {
        /* USER CODE BEGIN ETH_MspDeInit 0 */

        /* USER CODE END ETH_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ETH_CLK_DISABLE();

        /**ETH GPIO Configuration
        PC1     ------> ETH_MDC
        PA1     ------> ETH_REF_CLK
        PA2     ------> ETH_MDIO
        PA7     ------> ETH_CRS_DV
        PC4     ------> ETH_RXD0
        PC5     ------> ETH_RXD1
        PB13     ------> ETH_TXD1
        PG11     ------> ETH_TX_EN
        PG13     ------> ETH_TXD0
          */
#if 0
        HAL_GPIO_DeInit(GPIOC, RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin);

        HAL_GPIO_DeInit(GPIOA, RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin);

        HAL_GPIO_DeInit(RMII_TXD1_GPIO_Port, RMII_TXD1_Pin);

        HAL_GPIO_DeInit(GPIOG, RMII_TX_EN_Pin | RMII_TXD0_Pin);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(ETH_IRQn);
#endif /* 0 */
        /* USER CODE BEGIN ETH_MspDeInit 1 */

        /* USER CODE END ETH_MspDeInit 1 */
    }
}

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  EthHandle: ETH handle
  * @retval None
  */

int32_t bufferprocess = 0;
int32_t bufferin = 0;
int32_t bufferout = 0;

void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *EthHandle)
{
    bufferin++;
    if (event_callback != NULL)
    {
        (*event_callback)();
    }
}

void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *EthHandle)
{
    if (__HAL_ETH_DMA_GET_FLAG(EthHandle, ETH_DMASR_RBUS))
    {
        /* The Receive Buffer Unavailable interrupt is cleared by the Ethernet HAL handler before the
           Ethernet task is scheduled.
           Temporarily mask it so that the Ethernet task is scheduled before a new IT is trigged.
         */
        __HAL_ETH_DMA_DISABLE_IT(EthHandle, ETH_DMA_IT_RBU);
    }
    if (event_callback != NULL)
    {
        (*event_callback)();
    }
}

void ethernetif_low_init(void (*param_event_callback)(), void (*param_buffer_output_callback)(uint8_t *))
{
    /* Init ETH */
    event_callback = param_event_callback;
    buffer_output_callback = param_buffer_output_callback;

    EthHandle.Instance = ETH;
    EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    EthHandle.Init.PhyAddress = LAN8742A_PHY_ADDRESS;
    MACAddr[0] = 0x00;
    MACAddr[1] = 0x80;
    MACAddr[2] = 0xE1;
    MACAddr[3] = 0x00;
    MACAddr[4] = 0x00;
    MACAddr[5] = 0x01;

    /* Generate an MCU-unique MAC address */
    unsigned char output[HASH_OUTPUT_LENGTH];
    uint32_t UID[3];
    /* Generate a hash digest from the unique CPU ID */
    UID[0] = LL_GetUID_Word0();
    UID[1] = LL_GetUID_Word1();
    UID[2] = LL_GetUID_Word2();

#ifdef GENERATOR_WAKAAMACLIENT_CLOUD
    /* Hash with tinydtls */
    dtls_sha256_data((uint8_t *) UID, sizeof(UID), (char *) output);
#else /* GENERATOR_WAKAAMACLIENT_CLOUD */
    /* Hash with mbedTLS */
    int32_t hash_success = 0;
    mbedtls_sha256_context sha_context;
    mbedtls_sha256_init(&sha_context);

    if (0 == mbedtls_sha256_starts_ret(&sha_context, 0))
    {
        if (0 == mbedtls_sha256_update_ret(&sha_context, (const unsigned char *) UID, sizeof(UID)))
        {
            if (0 == mbedtls_sha256_finish_ret(&sha_context, output))
            {
                hash_success = 1;
            }
        }
    }
    mbedtls_sha256_free(&sha_context);

    while (hash_success == 0)
    {
        ;
    }
#endif  /* GENERATOR_WAKAAMACLIENT_CLOUD */

    /* Copy the 3 first bytes of the digest to the second half of the MAC address. */
    memcpy(&MACAddr[3], output, 3);

    EthHandle.Init.MACAddr = &MACAddr[0];
    EthHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
    EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    EthHandle.Init.Speed = ETH_SPEED_100M;
    EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

    HAL_ETH_Init(&EthHandle);

    /* Enable both the transmission and the read buffer underflow interrupts. */
    __HAL_ETH_DMA_ENABLE_IT(&EthHandle, ETH_DMA_IT_NIS | ETH_DMA_IT_R | ETH_DMA_IT_T | ETH_DMA_IT_AIS | ETH_DMA_IT_RBU);

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, 0, ETH_TXBUFNB);
    for (int32_t i = 0; i < ETH_TXBUFNB; i++)
    {
        DMATxDscrTab[i].Buffer1Addr = 0;
    }
    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(&EthHandle);

    /* USER CODE BEGIN PHY_PRE_CONFIG */

    /* USER CODE END PHY_PRE_CONFIG */

#ifdef PHY_ISFR
    /* Read Register Configuration */
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_ISFR, &regvalue);
    regvalue |= (PHY_ISFR_INT4);

    /* Enable Interrupt on change of link status */
    HAL_ETH_WritePHYRegister(&EthHandle, PHY_ISFR, regvalue);

    /* Read Register Configuration */
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_ISFR, &regvalue);
#endif /* PHY_ISFR */
    /* USER CODE BEGIN PHY_POST_CONFIG */

    /* USER CODE END PHY_POST_CONFIG */

}

void ethernetif_low_deinit()
{
#ifdef PHY_ISFR
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_ISFR, &regvalue);
    regvalue &= (~PHY_ISFR_INT4);
    HAL_ETH_WritePHYRegister(&EthHandle, PHY_ISFR, regvalue);
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_ISFR, &regvalue);
#endif /* PHY_ISFR */

    HAL_ETH_Stop(&EthHandle);
    memset(&EthHandle, 0x00, sizeof(EthHandle));
    memset(MACAddr, 0x00, 6);


    /* return pending buffers */
    for (int32_t i = 0; i < ETH_TXBUFNB; i++)
    {
        if ((DMATxDscrTab[i].Buffer1Addr != 0))
        {
            DMATxDscrTab[i].Buffer1Addr = 0;
            if (*buffer_output_callback)
            {
                (*buffer_output_callback)((uint8_t *) DMATxDscrTab[i].Buffer1Addr);
            }
        }
    }
}

int16_t ethernetif_low_inputsize(void)
{
    if (HAL_ETH_GetReceivedFrame_IT(&EthHandle) != HAL_OK)
    {
        ethernetif_low_errhandle();
        return -1;
    }
    bufferprocess++;
    return  EthHandle.RxFrameInfos.length;
}

int16_t ethernetif_low_input(uint8_t *payload, uint16_t len)
{
    __IO ETH_DMADescTypeDef *dmarxdesc;
    uint32_t rlen = 0;

    /* Obtain the size of the packet and put it into the "len" variable. */
    rlen = EthHandle.RxFrameInfos.length;
    dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;

    if ((payload == NULL) || (len < rlen))
    {
        NET_DBG_INFO("LwIP pool is full or buffer is too small. Dropping an Ethernet RX frame of size %lu.\n", rlen);
    }
    else
    {
#if !defined(USE_CACHED_ETH_BUFFERS)
        SCB_InvalidateDCache_by_Addr((uint32_t *)dmarxdesc->Buffer1Addr, rlen);
#endif /* !USE_CACHED_ETH_BUFFERS */
        memcpy(payload, (uint8_t *) dmarxdesc->Buffer1Addr, rlen);
    }

    /* Clear Segment_Count */
    NET_ASSERT(EthHandle.RxFrameInfos.SegCount == 1, " multiple segments only 1 supported\n");

    EthHandle.RxFrameInfos.SegCount = 0;
    dmarxdesc->Status |= ETH_DMARXDESC_OWN;

    /* ETH_DMA_IT_R and ETH_DMA_IT_RBU may be raised simultaneously.
       Run the error handling.
     */
    ethernetif_low_errhandle();

    return rlen;
}


static void ethernetif_low_errhandle(void)
{
    if (__HAL_ETH_DMA_GET_FLAG(&EthHandle, ETH_DMASR_RBUS))
    {
        NET_DBG_INFO("DMA underflow\n");
        /* Re-enable the Receive Buffer Unavailable interrupt */
        __HAL_ETH_DMA_ENABLE_IT(&EthHandle, ETH_DMA_IT_RBU);

        /* Clear RBUS ETHERNET DMA flag */
        __HAL_ETH_DMA_CLEAR_FLAG(&EthHandle, ETH_DMASR_RBUS);

        /* Resume DMA reception */
        EthHandle.Instance->DMARPDR = 0;

        /* The ETH_DMA_IT_R interrupt is not raised when restarting after a Receive Buffer Unavailable.
           Poll again.
         */
        if (event_callback != NULL)
        {
            (*event_callback)();
        }
    }
}

int16_t ethernetif_low_output(uint8_t *payload, uint16_t len)
{
    int16_t errval = 0;
    __IO ETH_DMADescTypeDef *DmaTxDesc = EthHandle.TxDesc;
    bufferout++;
    for (int32_t i = 0; i < ETH_TXBUFNB; i++)
    {
        if ((DMATxDscrTab[i].Buffer1Addr != 0) && !(DMATxDscrTab[i].Status & ETH_DMATXDESC_OWN))
        {
            DMATxDscrTab[i].Buffer1Addr = 0;
            if (*buffer_output_callback)
            {
                (*buffer_output_callback)((uint8_t *) DMATxDscrTab[i].Buffer1Addr);
            }
        }
    }

    if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
    {
        errval = -1;
        goto error;
    }

    DmaTxDesc->Buffer1Addr = (uint32_t) payload;
    SCB_CleanDCache_by_Addr(((uint32_t *)DmaTxDesc->Buffer1Addr), len + 32);

    /* Prepare transmit descriptors to give to DMA */
    if (HAL_ETH_TransmitFrame(&EthHandle, len))
    {
        errval = -1;
    }


error:
    /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
    if ((EthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
    {
        /* Clear TUS ETHERNET DMA flag */
        EthHandle.Instance->DMASR = ETH_DMASR_TUS;

        /* Resume DMA transmission*/
        EthHandle.Instance->DMATPDR = 0;
    }
    return errval;
}


void ethernetif_low_get_mac_addr(uint8_t *MACAddr_in)
{
    MACAddr_in[0] =  MACAddr[0];
    MACAddr_in[1] =  MACAddr[1];
    MACAddr_in[2] =  MACAddr[2];
    MACAddr_in[3] =  MACAddr[3];
    MACAddr_in[4] =  MACAddr[4];
    MACAddr_in[5] =  MACAddr[5];
}


uint8_t ethernetif_low_get_link_status(void)
{
    uint32_t phyreg;
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_BSR, &phyreg);
    return (uint8_t)((phyreg & PHY_LINKED_STATUS) > 0);
}


#ifdef GENERATOR_AWS_CLOUD
#include "mbedtls/x509_crt.h"

/*
 * Amazon Profile
 */
const   mbedtls_x509_crt_profile mbedtls_x509_crt_amazon_suite =
{
    /* Only SHA-256 and 384 */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_SHA256) |
    MBEDTLS_X509_ID_FLAG(MBEDTLS_MD_SHA384),
    /* Only ECDSA */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_PK_RSA) | /* */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_PK_ECKEY) | /* */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_PK_ECDSA),
#if defined(MBEDTLS_ECP_C)
    /* Only NIST P-256 and P-384 */
    MBEDTLS_X509_ID_FLAG(MBEDTLS_ECP_DP_SECP256R1) |
    MBEDTLS_X509_ID_FLAG(MBEDTLS_ECP_DP_SECP384R1),
#else
    0,
#endif /* MBEDTLS_ECP_C */
    2048
};
const int32_t net_tls_sizeof_suite_structure = sizeof(mbedtls_x509_crt_profile);
const void    *net_tls_user_suite0 = (void *) &mbedtls_x509_crt_amazon_suite;

#endif /* GENERATOR_AWS_CLOUD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
