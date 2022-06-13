/*
 * DAC.c
 *
 *  Created on: 2013-8-15
 *      Author: mengdeyu
 */
#include "DAC.h"

int16_t DAC_Test[8]= {0};

void DAOutputs(int16_t *Data,int16_t Num)
{
    int16_t channel, DAFIFO[8];

    for (channel=0;channel<=Num;channel++)
    {
           DAFIFO[channel] = (channel<<12)|(((Data[channel]+0x8000)>>4)&0x0FFF);//

           DAC_SendData(DAFIFO[channel]);
    }
	

}

void U0C0_SSC_vSendData(uint16_t Data)
{

  while(!(USIC0_CH0->PSR_SSCMode & 0x2000));   //  wait until tx buffer indication flag is set

  USIC0_CH0->PSCR     |= 0x2000;   //  clear transmit buffer indication flag
  USIC0_CH0->TBUF[0]   = Data;   //  load transmit buffer register

} //  End of function U1C1_SSC_vSendData
