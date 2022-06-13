/*****************************************************************************
*    Filename:     Parameters.h
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
#ifndef PARAMETERS_H_
#define PARAMETERS_H_

/*****************************************************************************
 *    Include Files
 *****************************************************************************/
#include <XMC1300.h>

/*****************************************************************************
*     �������²���
*****************************************************************************/

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** MCU parameters************************
#define   	MCU_FREQUENCY_D           64.0               	//CPU����ʱ�ӣ�unit in MHZ

//          ***************** PWM parameters************************
#define   	PWM_FREQUENCY_D             	16.0                // ����Ƶ��unit in KHZ unit
#define   	PWM_DEADTIME_D              	1.5                 // ����unit in uS

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** FOC mode************************
#define		  FOC_MODE_D					  	1	  	  			// ѡ�������Ժ���ģʽ
																// 1ת��ģʽ----��λ������ת��ָ��
																// 2ת��ģʽ----��λ������ת��ָ��
																														
#define		  SVPWM_MODE_D				  	7	  				// 5=5�Ρ�7=7��SVPWM


#define     QEP_CalibratedCount     960
#define     QEP_LineEncoder         1000
//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Inverter parameters************************
//��������
#define   	SHUNT_NUMBER_D               2             //define shunt number for current sample , choose from 2,3;
#define   	CURRENT_RATIO_D              0.1802
#define   	MOTOR_MAX_CURRENT_D          5   	         //��������������ֵ����λA����ѹģʽ����Ч
#define   	Delta_For_AD_Mid_D				   0.20				   //2.5V��׼�������ֵ��unit in V 0.1

#define     MOTOR_POLE_PAIRS_D           2             //��������� 

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Sensorless Startup************************
#define   	LOCK_TIME_D						  2.0					  //����ʱ�䣬��
#define   	LOCK_CURRENT_D					0.01				    //��������������

#define   	OPENLOOP_CURRENT_D				1.21  				//�������������� 2
#define   	OPENLOOP_SPEED_RPM_D			800.0				  //����������ٶȣ���λRPM  800
#define   	OPENLOOOP_ACCELARTION_TIME_D	10.0			//����ʱ�䣬��λS 10s���ٵ�500rpm

#define		  MIN_MOTOR_RPM_D					100.0				  //���������Сת�� ����ȿ����ٶȴ�һ��
#define		  MAX_MOTOR_RPM_D					1800.0				//����������ת��
#define     Speed_Max               6000      

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** PID Startup************************

//SPEED
#define   	KP_SPEEDLOOOP_D				    13824				//4.12 format
#define   	KI_SPEEDLOOOP_D				    1080				  //4.12 format

//IQ
#define   	KP_IQ_CurrentLOOOP_D			1650				//4.12 format
#define   	KI_IQ_CurrentLOOOP_D			180  				//4.12 format

//ID
#define   	KP_ID_CurrentLOOOP_D			1685		  //4.12 format
#define   	KI_ID_CurrentLOOOP_D			385		    //4.12 format


/*****************************************************************************
*     �������
*****************************************************************************/
/*************************************************************״̬�궨��*****************************************************************/
//���״̬��־
#define MFault                   0
#define MStop                    1
#define MBLDC_Start              2
#define MPMSM_Run                3
#define Ref_limit_min            90   //��������źŷ�ֵ

#define REF_MODE                 1        //1=��λ������ 0=UART����
#define STOP_MODE                1      // 1=ʹ��ͣ������  0=��ͣ������
#define IO_Mode                  0        // 0=�����ű�ͬʱPWM 1=���ű�PWM���ű۹�


/********************����л���������*************************/
#define  SquaretoSin_Speedrpm                       500  //�����л������Ҳ�ת��
#define  CommuTimeCal_Change_SpeedrpmThreshold      1000 //��ֻ����һ�λ���ʱ���л�������6�λ���ʱ���ƽ��ֵ


/*****************************************************************************
*     Local Type Definitions
*****************************************************************************/

typedef struct
{
    uint16_t PeriodValue_CentralAlign;
    uint16_t PeriodValue_EdgeAlign;
    uint16_t DeadTimeValue;
    uint16_t Mod_Threshold_H;
    uint16_t Mod_Threshold_L;
} TPWM;

typedef struct {
    float PP;
    float Rs;
    float Ls;
    float Ld;
    float Lq;
    float Kemf;
    float FluxM;
} TMOTOR_Para;

volatile typedef struct {
    unsigned    AD_OK_F:1;
    unsigned    AD_OFFSET_F:1;
    unsigned    START_F:1;
    unsigned    ERROR_F:1;
    unsigned    INITIAL_OK_F:1;
    unsigned    FREE_RUNNING_CW:1;
    unsigned    FREE_RUNNING_CCW:1;
    unsigned    MOTOR_STALL_F:1;

    unsigned    ESTIMATOR_ERROR_F:1;
    unsigned    INSERT_F:1;
    unsigned    :1;
    unsigned    :1;
    unsigned    HALL_F:1;
    unsigned    LOCK_F:1;
    unsigned    OPENLOOP_F:1;
    unsigned    CLOSE_LOOP_F:1;

    unsigned    MOD_L:1;
    unsigned    MOD_H:1;
    unsigned    BRAKE_F:1;
    unsigned    CURRENT_SAMPLING_F:1;
    unsigned    INIT_BRIDGE_F    :1;  //IPM�ų�ʼ��
    unsigned    INIT_BRIDGE_ONE_F:1;//IPM�ų�ʼ����һ��
    unsigned    AD_OFFSET_ERROR_F:1;
    unsigned    PAR_OVERFLOW_F:1;
    unsigned    OVERCURRENT_F:1;
    unsigned    OVERVOLTAGE_F:1;
    unsigned    UNDERVOLTAGE_F:1;
    unsigned    MOTOR_Fault_F:1;
    unsigned    UART_REF_F:1;   //UART��������
    unsigned    START_CORRECT_F:1;  
    unsigned    STOP_F:1;     //ͣ��
    unsigned    FIRST_F:1;    //��һ�ο���

} TFLAGS;


/*****************************************************************************
*     extern Global Variable Definitions
*****************************************************************************/
extern TMOTOR_Para 		Motor;
extern TPWM 					PWM;
extern TFLAGS System_State_Flags;

/*****************************************************************************
*     Static(Local) Variable Definitions
*****************************************************************************/

//�������
extern int32_t	 t_inc,t_stall;
extern int32_t   t_lock_time;
extern int16_t	 Lock_Current_Value;
extern uint16_t  Lock_Current_Step;
extern uint32_t	 Angle_Openloop;
extern int16_t   Openloop_Current_Value,Angle_Inc,ACC_Interval,Angle_Inc_MAX;
extern uint16_t  t_Switch;
extern uint16_t  Release;

extern int32_t	 Per_Ampere,PU_Ampere;
extern int16_t   Delta_AD;
extern float     Delta_For_AD_Mid	;     //2.5V��׼�������ֵ��unit in V

//����
extern int32_t  Current_Instruction;
extern int16_t  Tets_V;

//          ***************** MCU parameters************************
extern float     MCU_FREQUENCY ;       //unit in MHZ

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** PWM parameters************************
extern float     PWM_FREQUENCY ;       // unit in KHZ unit
extern float     PWM_DEADTIME  ;       // unit in uS
#define		Passive_LEVEL_HighSide	  	  	0	  	  			// ���ű�passive level: 1 = H,  0 = L
#define		Passive_LEVEL_LowSide		  	    0	  	  			// ���ű�passive level: 1 = H,  0 = L

//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Inverter parameters************************
extern int16_t	 SHUNT_NUMBER;
extern float     SHUNT_VALUE  ;         //Shunt value  unit in Ohm 
extern float     CURRENT_RATIO ;   
extern float     MOTOR_MAX_CURRENT  ;    // max allowable phase current


extern uint16_t  FOC_MODE;
//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Motor parameters************************
extern float     MOTOR_POLE_PAIRS ;       //���������
//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Sensorless Startup************************
extern float     LOCK_TIME		;			//����ʱ�䣬��
extern float     LOCK_CURRENT	;			//��������������

extern float     OPENLOOP_CURRENT	;		//��������������
extern float     OPENLOOP_SPEED_RPM	;	//����������ٶȣ���λRPM
extern float     OPENLOOOP_ACCELARTION_TIME;//����ʱ�䣬��λS

extern float		MIN_MOTOR_RPM	;		//���������Сת��
extern float		MAX_MOTOR_RPM	;		//����������ת��
//--------------------------------------------------------------------------------------------------------------------------
//          ***************** Emulator parameters************************
extern  uint16_t Motor_Counter;          //�����תȦ��
	
//--------------------------------------------------------------------------------------------------------------------------
//          ***************** PID Startup************************
extern int16_t      KP_SPEEDLOOOP;				//4.12 format
extern int16_t      KI_SPEEDLOOOP;				//4.12 format

extern int16_t      KP_IQ_CurrentLOOOP	;		//4.12 format
extern int16_t      KI_IQ_CurrentLOOOP	;		//4.12 format
extern int16_t      KP_ID_CurrentLOOOP	;		//4.12 format
extern int16_t      KI_ID_CurrentLOOOP	;		//4.12 format
extern int16_t			POT,POT_old;

//--------------------------------------------------------------------------------------------------------------------------
/************************************************************************ȫ�ֱ������*********************************************/
extern uint32_t SystemCoreClock;
extern uint16_t Motor_state;   
extern uint16_t Start_flag;
extern uint8_t  Hall_start_flag;
extern uint16_t Stop_Cnt;       //ͣ������
extern int16_t Recieved_UART_Data ;
extern uint16_t LED_cnt;
extern uint16_t Bridge_cnt;



/*****************************************************************************
 *    Global Function Definitions
 *****************************************************************************/
void Parameters_Init(void);


/*****************************************************************************
*     Variable setting for testing, delete it when release
*****************************************************************************/


#endif /* PARAMETERS_H_ */
