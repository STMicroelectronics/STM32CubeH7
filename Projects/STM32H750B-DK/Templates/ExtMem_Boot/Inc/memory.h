/**
  ******************************************************************************
  * @file    memory.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          all external memory configuration helper.
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
  *******************************************************************************
  */

/** @addtogroup MEMORY
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEMORY_H
#define __MEMORY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_hal.h"

/** @addtogroup MEMORY
  * @{
  */


/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/** @defgroup MEMORY_Exported_Types Memory Exported Types
  * @{
  */
/* Error codes */
#define MEMORY_OK          ((uint32_t)0x00)
#define MEMORY_ERROR       ((uint32_t)0x01)

/* QSPI Init Mode */
#define QSPI_MODE          ((uint32_t)0x00)    /* Init in quad-spi mode for XiP mode */
/**
  * @}
  */

#define USE_EXTERNAL_SDRAM   1
#define USE_INTERNAL_SRAM    2
#define USE_QSPI             3
#define USE_SPI_NOR          4
#define USE_MMC              5
   
/*
  @verbatim
  ==============================================================================
                     ##### How to use this Config #####
  ==============================================================================
    [..]
      The configuration bellow allows simple selection switch between configuration

      The configuration is composed mainly by two area:
      (#) CODE_AREA: Used to specify external memory used for code execution
         (##) XiP Mode:
             (+++) USE_QSPI : QSPI Flash is used for code execution
         (##) BootROM Mode:
             (+++) USE_EXTERNAL_SDRAM : External SDRAM is used for code execution
             (+++) USE_INTERNAL_SRAM  : Internal SRAM is used for code execution
   
      (#) DATA_AREA: Used to specify volatile memory used for data holding
         (##) USE_EXTERNAL_SDRAM : External SDRAM is used for code execution
         (##) USE_INTERNAL_SRAM  : Internal SRAM is used for code execution
   
      (#) BINARY_AREA: Used to specify storage memory used for binary storage
                       Binary file will be copied from those area into the one
                       specified by CODE_AREA after boot for execution
         (##) USE_SPI_NOR : SPI NOR Flash is used for binary storage
             (+++) BINARY_BASE_OFFSET : Offset of the binary within SPI NOR Flash
             (+++) BINARY_SIZE        : Size of the binary image
         (##) USE_MMC  : eMMC is used for binary storage
      Bellow an automatic update APPLICATION_ADDRESS to default value based on
      user configuration, which can be tailored as required

      Finally a set of check allows to avoid unsupported combined configuration.

  @endverbatim
*/
#define DATA_AREA           USE_INTERNAL_SRAM
#define CODE_AREA           USE_QSPI
/* #define BINARY_AREA         USE_MMC */
   
#if (BINARY_AREA == USE_MMC)
  #define BINARY_SIZE         33000U
  #define BINARY_ADDRESS       0U
#elif (BINARY_AREA == USE_SPI_NOR)
  #define BINARY_BASE_OFFSET   0x0U
  #define BINARY_SIZE         33000U

#endif

/*************************************************/
/*                                               */
/*     Configure Application Startup Address     */
/*                                               */
/*************************************************/
#define SDRAM_DEVICE_ADDR  ((uint32_t)0xD0000000)
   
#if (CODE_AREA == USE_QSPI)
  #define APPLICATION_ADDRESS            QSPI_BASE

#elif (CODE_AREA == USE_SPI_NOR)
  /* QSPI is used to emulate SPI-NOR*/
  #define APPLICATION_ADDRESS            QSPI_BASE

#elif (CODE_AREA == USE_EXTERNAL_SDRAM)
  #define APPLICATION_ADDRESS            SDRAM_DEVICE_ADDR

#elif (CODE_AREA == USE_INTERNAL_SRAM)
  #define APPLICATION_ADDRESS            D1_ITCMRAM_BASE

#else
  #error "APPLICATION_ADDRESS not defined"

#endif

/****************************************************/
/*                                                  */
/*     Check to avoid unsupported configuration     */
/*                                                  */
/****************************************************/
#if ((DATA_AREA != USE_EXTERNAL_SDRAM) && \
     (DATA_AREA != USE_INTERNAL_SRAM))
  #error "Wrong type used for DATA_AREA"
#endif

#if (defined(BINARY_AREA) && (BINARY_AREA != USE_SPI_NOR) && (BINARY_AREA != USE_MMC))
  #error "Wrong type used for BINARY_AREA"
#endif

#if (((CODE_AREA == USE_QSPI) && defined(BINARY_AREA))                            || \
     ((CODE_AREA == USE_NOR)  && defined(BINARY_AREA))                            || \
     ((CODE_AREA != USE_QSPI) && (CODE_AREA != USE_NOR) && !defined(BINARY_AREA)) || \
     (CODE_AREA == USE_SPI_NOR)                                                   || \
     (CODE_AREA == USE_MMC)) 
  #error "Wrong type used for CODE_AREA"
#endif

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Memory_Exported_Functions Non-Volatile memory
  * @{
  */
uint32_t QSPI_Startup(uint32_t Mode);
uint32_t MMC_Startup(void);
/**
  * @}
  */


/** @addtogroup Memory_Exported_Functions Volatile memory
  * @{
  */
uint32_t SDRAM_Startup(void);
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __MEMORY_H */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
