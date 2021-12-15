/**
  ******************************************************************************
  * @file    motion_detection.c
  * @author  MCD Application Team
  * @brief   motion detection implementation module
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

#ifdef USE_STM32H747I_DISCO

#include <stdint.h>
#include <stdlib.h>

#include <stm32h7xx_hal.h>

#include "motion_detection.h"

#pragma location = 0x10000000
uint8_t CAMERA_FRAME_BUFFER[160 * 120 * 2]; /* RGB565 buffer */

uint8_t MV_BUFFER0[160 * 120];
uint8_t MV_BUFFER1[160 * 120];

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup DCMI_CaptureMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static uint32_t  Buffs_Ready =0;
static uint32_t  Buffs_cnt =0;

static int32_t MVs[24][2];

/* Camera resolutions */

static __IO int32_t pending_buffer = -1;
static __IO int32_t active_area = 0;
static __IO int32_t dma2d_pending_copy = 0;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Camera Frame Event callback.
  */



void MotionDetect(struct ipc_msg *msg)
{
#if 1
  uint32_t i,sumX=0,sumY=0;
#if 0
    if (Buffs_cnt%3==2)
      Buffs_Ready=1;
    
    if (Buffs_cnt%3==0)   
      StoreMVs(160, 120,0);
    if (Buffs_cnt%3==2)   
      StoreMVs(160, 120,1);
#else
    if (Buffs_cnt%2==1)
      Buffs_Ready=1;
    
    if (Buffs_cnt%2==0)   
      StoreMVs(160, 120,0);
    if (Buffs_cnt%2==1)   
      StoreMVs(160, 120,1);
#endif    
    if ((Buffs_Ready==1) && (Buffs_cnt>10))
    {

      MotionCompute(160,120);

      msg->MotionX = 0;
      msg->MotionY = 0;

      for (i=0;i<23;i++)
      {
        if(ABS(MVs[i][0])>6)
        {
          sumX++;
          msg->MotionX += ABS(MVs[i][0]);
        }
        if(ABS(MVs[i][1])>6)
        {
          sumY++;
          msg->MotionY+=ABS(MVs[i][1]);
        }        

      }
      if ((sumX>5) /*&& (sumX<10)*/)
        msg->MotionX /= sumX;
      else
        msg->MotionX = 0;
          
      if (sumY>5)
        msg->MotionY /= sumY;
      else
        msg->MotionY = 0;
      
      //MailBox.Picture_Ready = PICTURE_READY;
     // while( MailBox.Picture_Ready!= PICTURE_NOT_READY);
    }
    else
    {
      msg->MotionX = 0;
      msg->MotionY = 0;
    }
    Buffs_cnt++;

#endif

  //MotionSimpleDetect(160, 120, (uint32_t *) &MailBox.MotionX);    
  //MailBox.MotionY=MailBox.MotionX;    

}


/**
  * @brief  Copy the Captured Picture to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  xsize: Picture X size
  * @param  ysize: Picture Y Size
  * @retval None
  */

void MotionSimpleDetect(uint32_t xsize, uint32_t ysize, uint32_t * motion)
{

  int8_t *MvBuff0 =(int8_t *)MV_BUFFER0; 
  int8_t *MvBuff1 =(int8_t *)MV_BUFFER1;  
  
  uint64_t my_Diff=0;
  uint32_t x,y;
  for(y=0;y<160;y++)
  {
    for(x=0;x<120;x++)
    {
      my_Diff+= ABS(MvBuff1[x+y*160]-MvBuff0[x+y*160]);
    }

  }

  *motion = (my_Diff/10000)-40;
}

void MotionCompute(uint32_t xsize, uint32_t ysize)
{
  uint32_t i;  

  for (i=0;i<6;i++)
    CalcMVs(1*160*10+(10*(i+1)), 0*(160*10)+i*10,&MVs[i][0],&MVs[i][1]);
  for (i=0;i<6;i++)
    CalcMVs(2*160*10+(10*(i+1)), 1*(160*10)+i*10,&MVs[6+i][0],&MVs[6+i][1]);
  for (i=0;i<6;i++)
    CalcMVs(3*160*10+(10*(i+1)), 2*(160*10)+i*10,&MVs[12+i][0],&MVs[12+i][1]);
  for (i=0;i<6;i++)
    CalcMVs(4*160*10+(10*(i+1)), 3*(160*10)+i*10,&MVs[18*i][0],&MVs[18+i][1]);


  
}

void StoreMVs(uint32_t xsize, uint32_t ysize,uint32_t BuffIdx)
{

  uint16_t *CamBuff = (uint16_t *)CAMERA_FRAME_BUFFER;   
  uint8_t *Buff = (BuffIdx==0)?(uint8_t *)MV_BUFFER0:(uint8_t *)MV_BUFFER1; 

  uint32_t x,y,i=0;
  for(y=0;y<ysize;y+=(ysize/120))
  {
    for(x=0;x<xsize;x+=(xsize/160))
    {
      Buff[i++] = 0xF & (CamBuff[y * xsize + x] >> 7);
    }
  }
}

void CalcMVs(uint32_t x0_Offset, uint32_t x1_Offset,int32_t *MVx,int32_t *MVy)
{
  uint8_t *MvBuff0 =(uint8_t *)MV_BUFFER0; 
  uint8_t *MvBuff1 =(uint8_t *)MV_BUFFER1;    
  uint64_t SAD = 0, MinSAD = 0xFFFFFFFFFFFFFFFF;
  int32_t x,y,z,t,x1,step;
  *MVx=0;
  *MVy=0;
  step=1;

  for(z=0;z<20;z+=step)
  {
    for(t=0;t<20;t+=step)
    {
      x1 = x1_Offset+t+z*160;
      SAD=0; 
                  for(y=0;y<10;y++)
                  {
                    for(x=0;x<10;x++)
                    {
                      SAD+= ABS(MvBuff1[x+x0_Offset+y*160]-MvBuff0[x+x1+y*160]);
                      
                    }
                  }
       if(SAD+10 < MinSAD)
       {
         MinSAD = SAD;
         *MVx=ABS(t-10);
         *MVy=ABS(z-10);
                 
       }
    }
    
    
  }


#if 1  
//  x1 = x1_Offset+10+10*160; 
  SAD=0; 
  for(y=0;y<10;y++)
  {
    for(x=0;x<10;x++)
    {
      SAD+= ABS(MvBuff1[x+x0_Offset+y*160]-MvBuff0[x+x0_Offset+y*160]);
      
    }
  }

  if(SAD < (MinSAD+20))
   {
     MinSAD = SAD;
     *MVx=0;
     *MVy=0; 
    // if(MinSAD<100)
    //   return;
   } 

   if(MinSAD>300)
   {
     MinSAD = SAD;
     *MVx=0;
     *MVy=0; 
    // if(MinSAD<100)
    //   return;
   } 


#endif  

  

}

#endif
