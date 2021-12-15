/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Inc/jdata_conf.h
  * @author  MCD Application Team
  * @brief    this file contains the libJPEG abstruction defines for the memory
  *          management routines and IO File operations
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
#include "ff.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define JFILE            FIL

#define JMALLOC   malloc
#define JFREE     free

size_t read_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf);
size_t write_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf) ;

#define JFREAD(file,buf,sizeofbuf)  \
   read_file (file,buf,sizeofbuf)

#define JFWRITE(file,buf,sizeofbuf)  \
   write_file (file,buf,sizeofbuf)

