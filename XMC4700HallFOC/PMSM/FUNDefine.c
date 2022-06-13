
#include "UserDefine.h" 


/*********************************************************************************************************
** ��������: ״̬������
** ��������: �л����������״̬
**
** ��  ��:   YZZ
** ��  ��:  2017/03/09
**********************************************************************************************************/
void State_Function(void)
{
//    Get_Mortor_state(); //��ȡ���״̬
     Get_POT();
    switch(	Motor_state)
    {
      
      /*case MFault:                                                          //����״̬
      {
        if(!System_State_Flags.MOTOR_Fault_F)
        {
           if(System_State_Flags.UART_REF_F )
           {
             Motor_state = MFault;
             System_State_Flags.START_CORRECT_F=0;
           }
           if(!System_State_Flags.UART_REF_F )  //
           {
             Motor_state = MStop;
            System_State_Flags.START_CORRECT_F=0;
           }
        }
        else
        {
//          Motor_Fault();
          Motor_state = MFault;
//          System_State_Flags.START_CORRECT_F = 0;
        }
        
      }
      break;
*/
      case MStop:                                                     //ͣ��״̬
      {
        #if !REF_MODE
        if(System_State_Flags.UART_REF_F)
        {
           Motor_state = MBLDC_Start;
           break;
        }
        #elif REF_MODE
        if(!System_State_Flags.STOP_F)
        {
           Motor_state = MBLDC_Start;
           break;
        } 
        #endif
        else
        {
          NVIC_DisableIRQ(CCU80_0_IRQn);  //�ص�����HALL_FOC�ж�
          Motor_state = MStop;
          Mortor_Stop();   //ͣ��
        }               
      }
      break;
      case MBLDC_Start:                                         //��������
      {
        NVIC_EnableIRQ(CCU80_0_IRQn);  //��HALL_FOC�жϴ���   
        #if !REF_MODE
        if((!System_State_Flags.UART_REF_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;
          System_State_Flags.START_CORRECT_F=0;
          break;
        } 
        #elif REF_MODE
        if((System_State_Flags.STOP_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;

          System_State_Flags.START_CORRECT_F=0;
          break;
        }  
        #endif
        if(!System_State_Flags.HALL_F)
        {
          Motor_state = MPMSM_Run;   //�л������Ҳ�����
        }
      }
      break;
      case MPMSM_Run:                                           //���Ҳ�����
      {

        #if !REF_MODE
        if((!System_State_Flags.UART_REF_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;
          System_State_Flags.START_CORRECT_F=0;
          break;
        } 
        #elif REF_MODE
        if((System_State_Flags.STOP_F)&(!System_State_Flags.MOTOR_Fault_F))  
        {						
          Motor_state = MStop;

          System_State_Flags.START_CORRECT_F=0;
          break;
        }  
        #endif
        System_State_Flags.CLOSE_LOOP_F	=1;	  //�л������Ҳ�ģʽ	
      }
      break;
      default:break;
    }
}

/*********************************************************************************************************
** ��������: �������״̬�ж�
** ��������: ����ʵ��Motor_state�ĸ�ֵ
**
** ��  ��:   YZZ
** ��  ��:  2017/03/09
**********************************************************************************************************/
void Get_Mortor_state(void)
{

   if(System_State_Flags.OVERVOLTAGE_F||System_State_Flags.UNDERVOLTAGE_F)  //�����ж�
  {
   System_State_Flags.MOTOR_Fault_F = 1;
   Motor_state=MFault;		
  }
  else
  {
    System_State_Flags.MOTOR_Fault_F= 0;
  }		
   if(POT > 90)
  {
   System_State_Flags.UART_REF_F = 1;//���������־
  }
  else
  {
    System_State_Flags.UART_REF_F  = 0;//ͣ��
  }
}
/*********************************************************************************************************
** ��������: ���ͣ������
** ��������: ����ʵ��ͣ��
**
** ��  ��:   YZZ
** ��  ��:  2017/03/09
**********************************************************************************************************/
void Mortor_Stop(void)
{
  POSIF0->MCSM	=	0x0000;							//�ض�ͨ��
  POSIF0->MCMS	|=	0x01;							//shadow transfer
  
  Refresh_CCU80_PWM_Dutycycle(2000,2000,2000);//�������
  
//�л�PWMģʽ
  CCU80_CC82->CHC=0x00000004;//�Գ�PWM���ǻ������
  CCU80_CC81->CHC=0x00000004;//�Գ�PWM���ǻ������
  CCU80_CC80->CHC=0x00000004;//�Գ�PWM���ǻ������
  System_Flags_Reset();//ϵͳ״̬��λ
}
/*********************************************************************************************************
** ��������: ��־λ��ʼ������
** ��������: ����ʵ�ֱ�־λ��ʼ��
**
** ��  ��:   YZZ
** ��  ��:  2017/03/09
**********************************************************************************************************/
void System_Flags_Init(void)
{
	System_State_Flags.START_F=1;
	System_State_Flags.LOCK_F=0;
	System_State_Flags.HALL_F=1;  //����ģʽ
  System_State_Flags.FIRST_F = 1; //��һ�ο���
  System_State_Flags.INIT_BRIDGE_F = 0; //���ų�ʼ��
  System_State_Flags.INIT_BRIDGE_ONE_F = 1; //���ų�ʼ��
  Hall1.Startcorrect = 0;
}
/*********************************************************************************************************
** ��������: ��־λ��λ����
** ��������: ����ʵ�ֱ�־λ��λ
**
** ��  ��:   YZZ
** ��  ��:  2017/04/10
**********************************************************************************************************/
void System_Flags_Reset(void)
{
  System_State_Flags.START_F=1; //������־
	System_State_Flags.LOCK_F=0;
	System_State_Flags.HALL_F=1;  //����ģʽ
  System_State_Flags.FIRST_F = 1; //��һ�ο���
  System_State_Flags.CLOSE_LOOP_F = 0;
  Hall1.Startcorrect = 0;  
  System_State_Flags.INIT_BRIDGE_F = 0;     //���ų�ʼ����־λ
  System_State_Flags.INIT_BRIDGE_ONE_F = 1; //����IPM�ų�ʼ����һ��
  LED_OFF; //ͣ����ָʾ��
  Hall1.Pattpre = 0;
}

/*********************************************************************************************************
** ��������: �ӳٺ���
** ��������: 
**
** ��  ��:  LuJun 
** ��  ��: 
**********************************************************************************************************/

void delayTimer(uint32_t ms) 
{                            
   uint32_t x,y; 
   for(x = ms;x > 0; x--)
   	for(y = 64000 ; y > 0; y--);                                  //ms      	
}

/*********************************************************************************************************
** ��������: PWM�����жϴ���
** ��������: �����ж���HALL_FOC����
**
** ��  ��:  BLL
** ��  ��:  2017/03/22
**********************************************************************************************************/

//ע�����ﺯ�����ָĹ���
void PWM_One_Match_INTER()           //CCU8SR0
{

    if(CCU80_CC80->INTS & 0x02)             //one match (CCU80_SR0)
    {

    	if(System_State_Flags.START_F )
    	{
				if(!System_State_Flags.STOP_F)    /*�������*/
        {
          if(System_State_Flags.FIRST_F)    //ͣ�����ٿ�����ͨ��
					{
					  	POSIF0->MCSM	 =	0x0333;						//��ͨ��״̬ ȫ��
              POSIF0->MCMS	|=	0x01;							//shadow transfer
						  System_State_Flags.FIRST_F = 0;
					}
//				  Get_Current();                  //��ȡA��B�����ֵ
				
				  Foc16.Idref_in 		= 	0;        //����Id=0����
				
				  if(System_State_Flags.OPENLOOP_F)
				     POT = 1600; //2000
					if(System_State_Flags.LOCK_F)
						 POT = 800;//1100
					if(System_State_Flags.CLOSE_LOOP_F)
					{
//					    POT = 2000;        //2500
						  Foc16.Instruction_in 	= 	(int16_t)(((int32_t)(POT) * (int32_t)(Speed_Max))>>12); //����Iq�������ٶȱջ��ٶȸ���
					}
//						 
//					HALL_FOC_RUN();//HALL_FOCģʽ����
				}
        else
        {
          Mortor_Stop();
        }
    	}  	
        CCU80_CC80->SWR|=0x00000002;            //���one match�ж�״̬��־λ
    }
}
/*********************************************************************************************************
** ��������: UART����
** ��������: 
**
** ��  ��:  BLL
** ��  ��:  2017/03/24
**********************************************************************************************************/

int16_t Get_UART_Val(void)
{
  int16_t UART_VAL = 0;
  Recieved_UART_Data = U0C0_UART_vReadData();
  UART_VAL = Recieved_UART_Data << 4; // 0-255 to 0-4095
  if (UART_VAL <= Ref_limit_min )
  {
    Stop_Cnt++;
    if(Stop_Cnt >=100)
    {
      System_State_Flags.STOP_F = 1;
      UART_VAL = 0;
    }            
  }
  else
  {
     System_State_Flags.STOP_F = 0;
     Stop_Cnt = 0;//ͣ����������
  }
    return ((int16_t)UART_VAL);
}
/*********************************************************************************************************
** ��������: ���ų�ʼ��
** ��������: ����ͨ�紦��
**
** ��  ��:  BLL
** ��  ��:  2017/03/24
**********************************************************************************************************/
void Bridge_Init(void)
{
  if(System_State_Flags.INIT_BRIDGE_ONE_F)
  {
     P0_1_set_mode(OUTPUT_PP_GP);
     P0_3_set_mode(OUTPUT_PP_GP);
     P0_9_set_mode(OUTPUT_PP_GP);   
     System_State_Flags.INIT_BRIDGE_F =1;
    while(System_State_Flags.INIT_BRIDGE_F);
    if(!System_State_Flags.INIT_BRIDGE_F)
    {
      P0_1_set_mode(OUTPUT_PP_AF5);    //U_L    
      P0_3_set_mode(OUTPUT_PP_AF7);  //V_L
      P0_9_set_mode(OUTPUT_PP_AF5);    //W_L
    }
  System_State_Flags.INIT_BRIDGE_ONE_F = 0;            
  }
 
}
void Refresh_Duty(uint16_t PWM_U,uint16_t PWM_V,uint16_t PWM_W)//���»���ռ�ձ�
{
  Hall1.PWM_U = PWM_U;
  Hall1.PWM_V = PWM_V;
  Hall1.PWM_W = PWM_W;
}

