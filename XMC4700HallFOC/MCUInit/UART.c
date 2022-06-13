/****************************************************************************
*    Filename:     UART.c
*    IDE:          DAVE_3.1.2
*    MCU:          XMC13OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    VXXX       2013-8-13    Created
*
*    Support :  IFCN.MCU@Infineon.com
*
*    Copyright (c) Infineon Semiconductor 2012. All Rights Reserved.
*****************************************************************************/


/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>
#include "Parameters.h"
#include "UART.h"
//#include  "SubFunc.h"
/*****************************************************************************
*     Macro Definitions
*****************************************************************************/


/*****************************************************************************
*     Local Type Definitions
*****************************************************************************/


/*****************************************************************************
*     Static(Local) Variable Definitions
*****************************************************************************/
//������   57.6 kbaud @ 8 MHz
//#define FDR_STEP	590
//#define BRG_PDIV	4						//5*16*1
//#define BRG_DCTQ	15
//#define BRG_PCTQ	0
//������   19.2 kbaud @ 32 MHz MCLK
#define FDR_STEP	590
#define BRG_PDIV	14						//15*16*4
#define BRG_DCTQ	15
#define BRG_PCTQ	3

/*****************************************************************************
*     Local Function Declarations
*****************************************************************************/
float speed_out[4] = {0};


/*****************************************************************************
*     Global Variable Definitions
*****************************************************************************/
int16_t Recieved_UART_Data = 0;
uint32_t Oscilloscope_Data[4] = {0};
uint16_t UARTSend_Flag=0;
uint16_t XAxis_Interval = 0;

/*****************************************************************************
 *    Global Function Definitions
 *****************************************************************************/

/*****************************************************************************
*     Variable setting for testing, delete it when release
*****************************************************************************/

/*****************************************************************************
 * Function:      void  UART_vInit(void)
 * Description:   Initialize the UART Module
 *                (Please add more details)
 * Caveats:       2013-8-13
 *                test only (Please add more details)
 *****************************************************************************/
void UART_Init(void)
{
	
	//-------------------------------------------------------------------------
    // Step 3: �ں�״̬�Ĵ������� - ʹ��ģ�飬ģ�� + MODENλ����
	//         USIC_CH_KSCFG ---RM1.1 Page500
	//-------------------------------------------------------------------------
	USIC0_CH1->KSCFG |= (1 << USIC_CH_KSCFG_MODEN_Pos) | (1 << USIC_CH_KSCFG_BPMODEN_Pos);

	//-------------------------------------------------------------------------
    // Step 4:  ѡ�������Ƶ����������Ӧ�Ĵ�����ֵ
	//     		USIC_CH_FDR ---RM1.0 Page513
	//-------------------------------------------------------------------------

	WR_REG(USIC0_CH1->FDR, USIC_CH_FDR_DM_Msk, USIC_CH_FDR_DM_Pos, 2);               //ѡ�������Ƶ��
	WR_REG(USIC0_CH1->FDR, USIC_CH_FDR_STEP_Msk, USIC_CH_FDR_STEP_Pos, FDR_STEP);    //����ֵ

	//-------------------------------------------------------------------------
    // Step 5:  ���ò����ʣ�PreDivider for CTQ, PCTQ = 0, Denominator for CTQ, DCTQ = 16
	//     USIC_CH_BRG       ---RM1.0 Page514
	//-------------------------------------------------------------------------
	WR_REG(USIC0_CH1->BRG, USIC_CH_BRG_PCTQ_Msk, USIC_CH_BRG_PCTQ_Pos, BRG_PCTQ);			//0
	WR_REG(USIC0_CH1->BRG, USIC_CH_BRG_DCTQ_Msk, USIC_CH_BRG_DCTQ_Pos, BRG_DCTQ);			//16
	WR_REG(USIC0_CH1->BRG, USIC_CH_BRG_PDIV_Msk, USIC_CH_BRG_PDIV_Pos, BRG_PDIV);			//4

	//-------------------------------------------------------------------------
    // Step 6:����USIC��λ���ƼĴ��� ��
	//        USIC_CH_SCTR      ---RM1.0 Page518
	//-------------------------------------------------------------------------
	// ����/���� LSB�ȣ� Transmission Mode (TRM) = 1, Passive Data Level (PDL) = 1
	WR_REG(USIC0_CH1->SCTR, USIC_CH_SCTR_PDL_Msk, USIC_CH_SCTR_PDL_Pos, 1);
	WR_REG(USIC0_CH1->SCTR, USIC_CH_SCTR_TRM_Msk, USIC_CH_SCTR_TRM_Pos, 1);
	// Set Word Length (WLE) & Frame Length (FLE)
	WR_REG(USIC0_CH1->SCTR, USIC_CH_SCTR_FLE_Msk, USIC_CH_SCTR_FLE_Pos, 7);
	WR_REG(USIC0_CH1->SCTR, USIC_CH_SCTR_WLE_Msk, USIC_CH_SCTR_WLE_Pos, 7);

	//-------------------------------------------------------------------------
    // Step 7: ���÷��Ϳ���״̬�Ĵ���
	//         USIC_CH_TCSR ---RM1.0 Page521
	//-------------------------------------------------------------------------
	//TBUF Data Enable (TDEN) = 1, TBUF Data Single Shot Mode (TDSSM) = 1
	WR_REG(USIC0_CH1->TCSR, USIC_CH_TCSR_TDEN_Msk, USIC_CH_TCSR_TDEN_Pos, 1);
	WR_REG(USIC0_CH1->TCSR, USIC_CH_TCSR_TDSSM_Msk, USIC_CH_TCSR_TDSSM_Pos, 1);

	//-------------------------------------------------------------------------
    // Step 8: ����Э����ƼĴ���
	//         USIC_CH_PCR_ASCMode ---RM1.0 Page401
	//-------------------------------------------------------------------------
	//Sample Mode (SMD) = 1, 1 Stop bit is selected, Sample Point (SP) = 2, Pulse Length (PL) = 0
	WR_REG(USIC0_CH1->PCR_ASCMode, USIC_CH_PCR_ASCMode_SMD_Msk,	USIC_CH_PCR_ASCMode_SMD_Pos, 1);
	WR_REG(USIC0_CH1->PCR_ASCMode, USIC_CH_PCR_ASCMode_STPB_Msk, USIC_CH_PCR_ASCMode_STPB_Pos, 0);
	WR_REG(USIC0_CH1->PCR_ASCMode, USIC_CH_PCR_ASCMode_SP_Msk, USIC_CH_PCR_ASCMode_SP_Pos, 9);
	//TBIF is set to simplify polling
	WR_REG(USIC0_CH1->PSR_ASCMode, USIC_CH_PSR_TBIF_Msk, USIC_CH_PSR_TBIF_Pos, 1);

	//-------------------------------------------------------------------------
  // Step 9: ����ͨ�����ƼĴ���
	//         USIC_CH_CCR ---RM1.0 Page518
	//-------------------------------------------------------------------------
	WR_REG(USIC0_CH1->CCR, USIC_CH_CCR_MODE_Msk, USIC_CH_CCR_MODE_Pos, 2);				//ѡ�� UARTЭ��
	WR_REG(USIC0_CH1->CCR, USIC_CH_CCR_PM_Msk, USIC_CH_CCR_PM_Pos, 0);					//��������żУ��
	WR_REG(USIC0_CH1->CCR, USIC_CH_CCR_RIEN_Msk, USIC_CH_CCR_RIEN_Pos, 1);				//ʹ�ܽ����ж�
	
	//-------------------------------------------------------------------------
  // Step 2: ������������
	//         PORT_IOCR0 ---RM1.0 Page1205
//  //-------------------------------------------------------------------------
//	P1_5_set_mode(OUTPUT_PP_AF2);							
//	P1_4_set_mode(INPUT);												//U0C0CH0__DX5E	
//	USIC0_CH0->DX5CR |=		0x04;
	
    P1_3_set_mode(INPUT);
    P1_2_set_mode(OUTPUT_PP_AF7);
//    USIC0_CH1->DX0CR |=		0x01;//U0C0CH1__DX0B	

	//-------------------------------------------------------------------------
	// Step 10: ʹ���жϽڵ�
	//         ICPR ---RMv1.0 Page112
	//         ISER ---RMv1.0 Page109
	//-------------------------------------------------------------------------
//	NVIC_SetPriority(USIC0_0_IRQn, 64);
//	NVIC_ClearPendingIRQ(USIC0_0_IRQn);													//����ڵ�ĵȴ�״̬
//	NVIC_EnableIRQ(USIC0_0_IRQn);														    //ʹ�ܽڵ��ж�����
	

}

/*****************************************************************************
 * Function:      void  USIC0_0_IRQHandler(void)
 * Description:
 *                (Please add more details)
 * Caveats:       2013-8-13
 *                test only (Please add more details)
 *****************************************************************************/


void USIC0_0_IRQHandler(void)
{
	if(RD_REG(USIC0_CH1->PSR, USIC_CH_PSR_RIF_Msk, USIC_CH_PSR_RIF_Pos) == 1)		//�жϽ����жϱ�־
	{
		CLR_BIT(USIC0_CH1->PSCR, USIC_CH_PSCR_CRIF_Pos);								//��������жϱ�־
		Recieved_UART_Data = U0C0_UART_vReadData();
		U0C0_UART_vSendData(Recieved_UART_Data); //��������

  }
}
//--------------------------------------------------------------------------------------------

uint16_t U0C0_UART_vReadData(void)
{
	uint16_t	Read_UART_Data;

	Read_UART_Data = ((USIC0_CH1->RBUF)& 0x00ff);

	return(Read_UART_Data);
}


void U0C0_UART_vSendData(uint16_t Data)
{

  while(!(USIC0_CH1->PSR_ASCMode & 0x2000));   //  wait until tx buffer indication flag is set

  USIC0_CH1->PSCR     |= 0x2000;   //  clear transmit buffer indication flag
  USIC0_CH1->TBUF[0]   = Data;     //  load transmit buffer register

} //  End of function U1C1_SSC_vSendData

                    /*����ʾ����Э�� */
////����ʾ������غ���
unsigned int CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned int CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++)
  {      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) 
    {
      if (CRC_Temp & 0x01)
          CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
      else
          CRC_Temp = CRC_Temp >> 1;
  
    }
  }
    return(CRC_Temp);
}

void OutPut_Data(uint32_t*speed)
{
	unsigned char i; 	
  unsigned char databuf[10] = {0};
  unsigned int CRC16 = 0;
	unsigned int temp[4] = {0};
	uint32_t speed_out[4];
	
	for(i=0;i<4;i++)
	{
		speed_out[i] = speed[i];
	}
	  for(i=0;i<4;i++)
  {
    
    temp[i]  = (int)speed_out[i];
    
   }
   
	
		for(i=0;i<4;i++) 
  	{
    	databuf[i*2]   = (unsigned char)(temp[i]%256);
    	databuf[i*2+1] = (unsigned char)(temp[i]/256);
    }
  
  	CRC16 = CRC_CHECK(databuf,8);
  	databuf[8] = CRC16%256;
  	databuf[9] = CRC16/256;
	
	for(i=0;i<10;i++)
	U0C0_UART_vSendData(databuf[i]);				
}