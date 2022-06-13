/*
 * MCUInit.c
 *
 *  Created on: Nov 11, 2016
 *      Author: summerzhongzai
 */



#include <XMC4700.h>
                                                                                                                                              
#include "MCUInit.h"

void MCUInit(void)
{
 // 时钟设置
	SystemInit();
  SCU_CLK->CCUCLKCR |= ((0x01)<<2);//禁止外部时钟
  SCU_CLK->CGATCLR0 |= ((0x01)<<0)|((0x01)<<2)|((0x01)<<3)|((0x01)<<4)|((0x01)<<7)|((0x01)<<8)|((0x01)<<9)|((0x01)<<10)|((0x01)<<11);//stop gating VADC,CUU40,CCU41,CCU42,CCU80,CCU81,POSIF0,POSIF1
  SCU_CLK->CGATCLR1 |= ((0x01)<<5);//stop gating DAC
  //模块初始化
  
  //主时钟中断
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000 );
//  P5_8_set_mode(OUTPUT_PP_GP);
}
uint32_t cnt = 0;
uint32_t timingdelay = 0;
void SysTick_Handler(void)  //主时钟中断
{
  cnt++;
  if(cnt >=20000)
  {
     P5_8_toggle();
     cnt = 0;
  }
  if(timingdelay!=0)
  {
    timingdelay --;
  }
}

void Delay_ms(uint32_t time)  //准确延时函数
{
  timingdelay = time;
  while(timingdelay!=0);
}
void Delay(uint32_t ms)//粗略延时函数
{
  uint32_t i;
  uint32_t j;
  for(i = ms;i > 0;i--)
  {
    for(j=8000;j > 0;j--)
    {
      __nop();__nop();__nop();__nop();
    }
  }
}