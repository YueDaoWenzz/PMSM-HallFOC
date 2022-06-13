/****************************************************************************
*    Filename:     Parameters.c
*    IDE:          DAVE_3.1.2
*    MCU:          XMC45OO
*
*    Revision History:
*    -------------------------------------------------------------------------
*    VXXX       2013-1-15    Created
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

/*****************************************************************************
*     Macro Definitions
*****************************************************************************/


/*****************************************************************************
*     Local Type Definitions
*****************************************************************************/

/*****************************************************************************
*     Global Variable Definitions
*****************************************************************************/
TMOTOR_Para 		Motor;
TPWM 						PWM;
TFLAGS 					System_State_Flags;


//          ***************** MCU parameters************************
float     MCU_FREQUENCY; 				//unit in MHZ 64

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** PWM parameters************************
float     PWM_FREQUENCY; 				// unit in KHZ unit  16
float     PWM_DEADTIME;					// unit in uS

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Inverter parameters************************
int16_t	  SHUNT_NUMBER;          //����������Ŀ
float     SHUNT_VALUE;  				 //Shunt value  unit in Ohm���������С
float     CURRENT_RATIO;   				//
float     MOTOR_MAX_CURRENT;   			// max allowable phase current

uint16_t  FOC_MODE;

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Motor parameters************************
float     MOTOR_POLE_PAIRS; 			//���������

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Emulator parameters************************
uint16_t Motor_Counter;          //�����תȦ��

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Sensorless Startup************************
float     LOCK_TIME;					  //����ʱ�䣬��
float     LOCK_CURRENT;					//��������������
float     OPENLOOP_CURRENT;				//��������������
float     OPENLOOP_SPEED_RPM;			//����������ٶȣ���λRPM
float     OPENLOOOP_ACCELARTION_TIME;	//����ʱ�䣬��λS

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** PID Startup************************
int16_t    KP_SPEEDLOOOP;			  //4.12 format
int16_t    KI_SPEEDLOOOP;			  //4.12 format

int16_t      KP_IQ_CurrentLOOOP	;		//4.12 format
int16_t      KI_IQ_CurrentLOOOP	;		//4.12 format
int16_t      KP_ID_CurrentLOOOP	;		//4.12 format
int16_t      KI_ID_CurrentLOOOP	;		//4.12 format


//�������
int32_t	  t_lock_time;
int16_t	  Lock_Current_Value;
uint16_t  Lock_Current_Step=0;
uint32_t	Angle_Openloop=0;
int16_t   Openloop_Current_Value,Angle_Inc,Angle_Inc_MAX,ACC_Interval;
uint16_t  Release;
uint16_t  t_Switch;
int32_t		t_inc,t_stall=0;


int16_t		Delta_AD;
int32_t		Per_Ampere,PU_Ampere;
float     Delta_For_AD_Mid;

//����
int32_t  Current_Instruction;
int16_t  Tets_V	;  					 //��λ��ʩ�ӿ�����ѹ��С���󣬵�����,1 �򿪣� 0 �ر�

/******************************ȫ�ֱ���***********************************/
uint16_t Motor_state = MStop;
uint16_t Start_flag = 0;
uint8_t  Hall_start_flag = 0;
uint16_t Stop_Cnt = 0;
uint16_t LED_cnt = 0;
uint16_t Bridge_cnt = 0;

/*****************************************************************************
 *    Global Function Definitions
 *****************************************************************************/

void Set(void);
/*****************************************************************************
*     Variable setting for testing, delete it when release
*****************************************************************************/

/*****************************************************************************
 * Function:      void  XXX_vInit(void)
 * Description:   Initialize the XXX Module
 *                (Please add more details)
 * Caveats:       2013-1-15
 *                test only (Please add more details)
 *****************************************************************************/
void Set()
{
	
	  Tets_V = 1;                               //��λ��ʩ�ӿ�����ѹ��С���󣬵�����,1 �򿪣� 0 �ر�
	
		MCU_FREQUENCY 		=    MCU_FREQUENCY_D;   //unit in MHZ

		//--------------------------------------------------------------------------------------------------------------------------
		//          ***************** PWM parameters************************
		PWM_FREQUENCY 		=   PWM_FREQUENCY_D;  // unit in KHZ unit
		PWM_DEADTIME  		=   PWM_DEADTIME_D;  // unit in uS

		//--------------------------------------------------------------------------------------------------------------------------
		//          ***************** Inverter parameters************************
		//define shunt number for current sample , choose from 2,3;
		SHUNT_NUMBER		=	 (int16_t)(SHUNT_NUMBER_D);
	  
	  Delta_For_AD_Mid=	 Delta_For_AD_Mid_D;  //2.5V��׼�������ֵ��unit in V
	
	  MOTOR_MAX_CURRENT  	=    MOTOR_MAX_CURRENT_D;// max allowable phase current
    CURRENT_RATIO   =  CURRENT_RATIO_D; //
	
	
		FOC_MODE			=	FOC_MODE_D;
		//--------------------------------------------------------------------------------------------------------------------------
		//          ***************** Motor parameters************************

		MOTOR_POLE_PAIRS    	=    MOTOR_POLE_PAIRS_D;//���������


		//--------------------------------------------------------------------------------------------------------------------------
		//          ***************** Sensorless Startup************************
    LOCK_TIME				=	LOCK_TIME_D;//����ʱ�䣬��
		LOCK_CURRENT			=	LOCK_CURRENT_D;//��������������

		OPENLOOP_CURRENT		=	OPENLOOP_CURRENT_D;//��������������
		OPENLOOP_SPEED_RPM		=	OPENLOOP_SPEED_RPM_D;//����������ٶȣ���λRPM
		OPENLOOOP_ACCELARTION_TIME=OPENLOOOP_ACCELARTION_TIME_D;//����ʱ�䣬��λS

		//--------------------------------------------------------------------------------------------------------------------------
		//          ***************** PID Startup************************
		KP_SPEEDLOOOP			=	KP_SPEEDLOOOP_D;//4.12 format
	  KI_SPEEDLOOOP			=	KI_SPEEDLOOOP_D;//4.12 format

	  KP_IQ_CurrentLOOOP		=	KP_IQ_CurrentLOOOP_D;	//4.12 format
	  KI_IQ_CurrentLOOOP		=	KI_IQ_CurrentLOOOP_D; //4.12 format
		
		KP_ID_CurrentLOOOP		=	KP_ID_CurrentLOOOP_D;	//4.12 format
	  KI_ID_CurrentLOOOP		=	KI_ID_CurrentLOOOP_D; //4.12 format
 
}


void Parameters_Init(void)
{

		Set();

		//-------------------------------------------------------------------------------------------------------------
		PWM.PeriodValue_CentralAlign =   (uint16_t)(((MCU_FREQUENCY * 500) /   PWM_FREQUENCY)   -   1);     //center aligned Period value,
		PWM.PeriodValue_EdgeAlign    =   (PWM.PeriodValue_CentralAlign << 1)  +   1;                        //edge aligned Period value
		PWM.DeadTimeValue            =   (uint16_t)(PWM_DEADTIME * MCU_FREQUENCY);                          //Dead Time Counter Value
 
	  //-----------------------------------------------------------------------------------------------------------
		Per_Ampere				=	(int32_t)((4095*16*CURRENT_RATIO)/5.0);  //2361.9 //����16 to 16bit CURRENT_RATIO ��������ת���ɵ�ѹ��ϵ��  //ÿ�������ֵ��Ӧ16bit����

		//PU_Ampere					=	(int32_t)(16384*5.0 /(SHUNT_VALUE * MOTOR_MAX_CURRENT * CURRENT_RATIO));
	
	  Delta_AD					=	(int16_t)(65536 * Delta_For_AD_Mid/5.0);
	
	  t_lock_time					=	(int32_t)(LOCK_TIME * 1000.0 * PWM_FREQUENCY);
    Lock_Current_Value			=	(int16_t)(Per_Ampere * LOCK_CURRENT);
    Lock_Current_Step			=	(uint16_t)((Per_Ampere * LOCK_CURRENT) / (LOCK_TIME * PWM_FREQUENCY*1000));	



    Openloop_Current_Value		=	(int16_t)(Per_Ampere * OPENLOOP_CURRENT);

    Angle_Inc_MAX				=	(int16_t)(OPENLOOP_SPEED_RPM * MOTOR_POLE_PAIRS /60.0 * 65536.0 /PWM_FREQUENCY/1000.0);
		ACC_Interval				=	(int16_t)((OPENLOOOP_ACCELARTION_TIME * 1000.0 * PWM_FREQUENCY)/(OPENLOOP_SPEED_RPM * MOTOR_POLE_PAIRS /60.0 * 65536.0 /PWM_FREQUENCY/1000.0));


}
