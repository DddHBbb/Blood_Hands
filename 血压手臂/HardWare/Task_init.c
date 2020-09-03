#include "board.h"
#include "rtthread.h"


/***********************����������*******************************/

/***********************����������*******************************/
extern rt_sem_t Sem1_Hanle;
extern rt_sem_t Blood_Pressure_Calibration_Sem;
/***********************ȫ�ֱ�����*******************************/
static rt_thread_t CAN_Handle = RT_NULL;
static rt_thread_t I2C_Handle = RT_NULL;
static rt_thread_t ADC_Handle = RT_NULL;
static rt_thread_t DISPOSE_Handle = RT_NULL;

CanTxMsg TxMessage;
CanRxMsg RxMessage;		
/****************************************************************/

 /****************************************
  * @brief  ͨ�ô�������ȫ�������ڴ�ʵ��
  * @param  ��
  * @retval ��
  ***************************************/
void Dispose_Task(void* parameter)
{
	while(1)
	{
		General_Dispose();	
		rt_thread_delay(10);
	}
}
 /****************************************
  * @brief  CANͨ�ź���,��ǹ���״̬����������
  * @param  ��
  * @retval ��
  ***************************************/
void CAN_Task(void* parameter)
{
	Init_RxMes(&RxMessage);
	while(1)
	{
		CAN_Task_Dispose();
		rt_thread_delay(1);
	}
	
}
 /****************************************
  * @brief  I2Cͨ�Ŵ�����
  * @param  ��
  * @retval ��
  ***************************************/
void I2C_Task(void* parameter)
{
	while(1)
	{
		if(rt_sem_take(Sem1_Hanle,RT_WAITING_FOREVER) == RT_EOK)
		{
		//	ee_Test();		
		}
		rt_thread_delay(1);
	}
}
 /****************************************
  * @brief  ADC�ɼ�������
  * @param  ��
  * @retval ��
  ***************************************/
void ADC_Task(void* parameter)
{
	while(1)
	{
		if(rt_sem_take(Blood_Pressure_Calibration_Sem,RT_WAITING_FOREVER) == RT_EOK)
		{
				ADC_Task_Dispose();		
		}
		rt_thread_delay(1);
	}
}
 /***************************************
  * @brief  ���񴴽�����ʼ������
  * @param  ��
  * @retval ��
  ***************************************/
void Task_init(void)
{
	CAN_Handle = rt_thread_create( "CAN_Task",              /* �߳����� */
                      CAN_Task,   				 /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      3,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (CAN_Handle != RT_NULL)    rt_thread_startup(CAN_Handle);
	
	I2C_Handle = rt_thread_create( "I2C_Task",              /* �߳����� */
                      I2C_Task,   					/* �߳���ں��� */
                      RT_NULL,              /* �߳���ں������� */
                      1024,                 /* �߳�ջ��С */
                      2,                    /* �̵߳����ȼ� */
                      20);                  /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
//   if (I2C_Handle != RT_NULL)    rt_thread_startup(I2C_Handle);
	
	ADC_Handle = rt_thread_create( "ADC_Task",              /* �߳����� */
                      ADC_Task,  					 /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      1,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (ADC_Handle != RT_NULL)    rt_thread_startup(ADC_Handle);
 
	DISPOSE_Handle = rt_thread_create( "Dispose_Task",              /* �߳����� */
                      Dispose_Task,   		 /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      2,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (DISPOSE_Handle != RT_NULL)    rt_thread_startup(DISPOSE_Handle);
}
