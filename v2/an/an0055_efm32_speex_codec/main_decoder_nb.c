/**************************************************************************//**
 * @file main_decoder_nb.c
 * @brief SPEEX NB decoder demo for EFM32LG/GG
 * @author Silicon Labs
 * @version 1.06
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#include <stdlib.h>
#include <stdbool.h>

#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "clock.h"
#include "dac.h"
#include "dma.h"
#include "speexfunc.h"
#include "timer.h"

//#define GG_STK

#ifdef GG_STK
#include "gpio.h"
#else
#include "bsp.h"
#endif

#include "voice8k.h"
#include "voice11k.h"
#include "voice15k.h"
#include "voice18k2.h"

struct speexEncFile playList[LIST_SIZE];

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
#ifdef GG_STK
  uint32_t mode = 0;
#else
  /* Initialize DK board register access, necessary if run on DK */
  BSP_Init(BSP_INIT_DEFAULT);
  BSP_PeripheralAccess(BSP_AUDIO_OUT, true);
#endif
  
  /* Initialize peripherals */
  initClock();
  initDma();
  initDac();
  initDacDma();
  initTimer();
#ifdef GG_STK
  initGpio();
#endif

  while(1)
  {
/*** Operation for EFM32GG-DK3750 ***/    
#ifndef GG_STK
    /* Wait key press */
    while(!BSP_PushButtonsGet())
      ;
    if (BSP_PushButtonsGet() == BC_UIF_PB1)
    {
      BSP_LedsSet(0x0001);
      /* Wait key release */
      while(BSP_PushButtonsGet()!=0);
      /* Start play files */      
      playList[0].frameStart = (char *)voice8k;
      playList[1].frameStart = NULL;
      playList[0].frameNum = sizeof(voice8k)/FRAME_SIZE_8K;
      speexPlayBack(0, NARROWBAND, FRAME_SIZE_8K, &playList[0]);
      BSP_LedsSet(0x0000);
    }
    if (BSP_PushButtonsGet() == BC_UIF_PB2)
    {    
      BSP_LedsSet(0x0002);
      /* Wait key release */
      while(BSP_PushButtonsGet()!=0);
      /* Start play files */      
      playList[0].frameStart = (char *)voice11k;
      playList[1].frameStart = NULL;
      playList[0].frameNum = sizeof(voice11k)/FRAME_SIZE_11K;
      speexPlayBack(0, NARROWBAND, FRAME_SIZE_11K, &playList[0]);
      BSP_LedsSet(0x0000);
    }
    if (BSP_PushButtonsGet() == BC_UIF_PB3)
    {
      BSP_LedsSet(0x0004);
      /* Wait key release */
      while(BSP_PushButtonsGet()!=0);
      /* Start play files */      
      playList[0].frameStart = (char *)voice15k;
      playList[1].frameStart = NULL;
      playList[0].frameNum = sizeof(voice15k)/FRAME_SIZE_15K;
      speexPlayBack(0, NARROWBAND, FRAME_SIZE_15K, &playList[0]);
      BSP_LedsSet(0x0000);
    }
    if (BSP_PushButtonsGet() == BC_UIF_PB4)
    {    
      BSP_LedsSet(0x0008);
      /* Wait key release */
      while(BSP_PushButtonsGet()!=0);
      /* Start play files */      
      playList[0].frameStart = (char *)voice18k2;
      playList[1].frameStart = NULL;
      playList[0].frameNum = sizeof(voice18k2)/FRAME_SIZE_18K2;
      speexPlayBack(0, NARROWBAND, FRAME_SIZE_18K2, &playList[0]);
      BSP_LedsSet(0x0000);
    }

/*** Operation for EFM32GG-STK3750 ***/        
#else
    /* Wait key press */
    EMU_EnterEM1();

    switch (keyCheck())
    {
    case 1:
      mode++;
      if (mode == 4)
      {
        mode = 0;
      }
      GPIO_PortOutSetVal(gpioPortE, (mode << 2), 0x000c);
      break;
      
    case 2:
      if (mode == 0x00)
      {
        /* Start play files */      
        playList[0].frameStart = (char *)voice8k;
        playList[1].frameStart = NULL;
        playList[0].frameNum = sizeof(voice8k)/FRAME_SIZE_8K;
        speexPlayBack(0, NARROWBAND, FRAME_SIZE_8K, &playList[0]);
      }
      else if (mode == 0x01)
      {
        /* Start play files */      
        playList[0].frameStart = (char *)voice11k;
        playList[1].frameStart = NULL;
        playList[0].frameNum = sizeof(voice11k)/FRAME_SIZE_11K;
        speexPlayBack(0, NARROWBAND, FRAME_SIZE_11K, &playList[0]);
      }
      else if (mode == 0x02)
      {
        /* Start play files */      
        playList[0].frameStart = (char *)voice15k;
        playList[1].frameStart = NULL;
        playList[0].frameNum = sizeof(voice15k)/FRAME_SIZE_15K;
        speexPlayBack(0, NARROWBAND, FRAME_SIZE_15K, &playList[0]);
      }
      else
      {
        /* Start play files */      
        playList[0].frameStart = (char *)voice18k2;
        playList[1].frameStart = NULL;
        playList[0].frameNum = sizeof(voice18k2)/FRAME_SIZE_18K2;
        speexPlayBack(0, NARROWBAND, FRAME_SIZE_18K2, &playList[0]);
      }
      break;
      
    default:
      break;
    }    
#endif
  }
}
