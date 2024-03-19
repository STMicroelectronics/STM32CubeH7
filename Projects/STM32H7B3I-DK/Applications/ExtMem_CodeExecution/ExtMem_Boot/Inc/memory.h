/**
  ******************************************************************************
  * @file    memory.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          all external memory configuration helper.
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
#include "stm32h7b3i_discovery_ospi.h"
#include "stm32h7b3i_discovery_sdram.h"   
#include "stm32h7b3i_discovery_sd.h"    

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
#define OSPI_MODE          ((uint32_t)0x00)    /* Init in octo-spi mode for XiP mode */
/**
  * @}
  */

#define USE_EXTERNAL_SDRAM   1
#define USE_INTERNAL_SRAM    2
#define USE_OSPI             3
#define USE_SPI_NOR          4
#define USE_SDMMC            5
   
/*
  @verbatim
  ==============================================================================
                     ##### How to use this Config #####
  ==============================================================================
    [..]
      The configuration below allows simple selection switch between configuration

      The configuration is composed mainly by two area:
      (#) CODE_AREA: Used to specify external memory used for code execution
         (##) XiP Mode:
             (+++) USE_OSPI : OSPI Flash is used for code execution
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
         (##) USE_SDMMC  : Micro SD is used for binary storage
      Below an automatic update APPLICATION_ADDRESS to default value based on
      user configuration, which can be tailored as required

      Finally a set of check allows to avoid unsupported combined configuration.

  @endverbatim
*/
#define DATA_AREA           USE_INTERNAL_SRAM      
#define CODE_AREA           USE_OSPI                    
/*#define BINARY_AREA         USE_SDMMC*/       
   
#if (BINARY_AREA == USE_SDMMC)
  #define BINARY_SIZE          33000U
  #define BINARY_ADDRESS       0U
  #define NUM_OF_BLOCKS        5
#elif (BINARY_AREA == USE_SPI_NOR)
  #define BINARY_BASE_OFFSET   0x0U
  #define BINARY_SIZE         33000U
#endif

/*************************************************/
/*                                               */
/*     Configure Application Startup Address     */
/*                                               */
/*************************************************/
   
#if (CODE_AREA == USE_OSPI)
  #define APPLICATION_ADDRESS            OCTOSPI1_BASE

#elif (CODE_AREA == USE_SPI_NOR)
  /* OSPI is used to emulate SPI-NOR */
  #define APPLICATION_ADDRESS            OCTOSPI1_BASE

#elif (CODE_AREA == USE_EXTERNAL_SDRAM)
  #define APPLICATION_ADDRESS            SDRAM_DEVICE_ADDR

#elif (CODE_AREA == USE_INTERNAL_SRAM)
  #define APPLICATION_ADDRESS            0x20000000UL

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

#if (defined(BINARY_AREA) && (BINARY_AREA != USE_SPI_NOR) && (BINARY_AREA != USE_SDMMC))
  #error "Wrong type used for BINARY_AREA"
#endif

#if (((CODE_AREA == USE_OSPI) && defined(BINARY_AREA))    || \
     ((CODE_AREA != USE_OSPI) && !defined(BINARY_AREA))   || \
      (CODE_AREA == USE_SPI_NOR)                          || \
      (CODE_AREA == USE_SDMMC)) 
  #error "Wrong type used for CODE_AREA"
#endif

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Memory_Exported_Functions Non-Volatile memory
  * @{
  */
uint32_t OSPI_Startup(uint32_t Mode);
uint32_t SDMMC_Startup(void);
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

