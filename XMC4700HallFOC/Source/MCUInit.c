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
 // ʱ������
	SystemInit();
  SCU_CLK->CCUCLKCR |= ((0x01)<<2);//��ֹ�ⲿʱ��
  SCU_CLK->CGATCLR0 |= ((0x01)<<0)|((0x01)<<2)|((0x01)<<3)|((0x01)<<4)|((0x01)<<7)|((0x01)<<8)|((0x01)<<9)|((0x01)<<10)|((0x01)<<11);//stop gating VADC,CUU40,CCU41,CCU42,CCU80,CCU81,POSIF0,POSIF1
  SCU_CLK->CGATCLR1 |= ((0x01)<<5);//stop gating DAC
  //ģ���ʼ��
  
  //��ʱ���ж�
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000 );
//  P5_8_set_mode(OUTPUT_PP_GP);
}
uint32_t cnt = 0;
uint32_t timingdelay = 0;
void SysTick_Handler(void)  //��ʱ���ж�
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

void Delay_ms(uint32_t time)  //׼ȷ��ʱ����
{
  timingdelay = time;
  while(timingdelay!=0);
}
void Delay(uint32_t ms)//������ʱ����
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