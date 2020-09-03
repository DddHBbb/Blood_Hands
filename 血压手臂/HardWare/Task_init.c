#include "board.h"
#include "rtthread.h"


/***********************函数声明区*******************************/

/***********************声明返回区*******************************/
extern rt_sem_t Sem1_Hanle;
extern rt_sem_t Blood_Pressure_Calibration_Sem;
/***********************全局变量区*******************************/
static rt_thread_t CAN_Handle = RT_NULL;
static rt_thread_t I2C_Handle = RT_NULL;
static rt_thread_t ADC_Handle = RT_NULL;
static rt_thread_t DISPOSE_Handle = RT_NULL;

CanTxMsg TxMessage;
CanRxMsg RxMessage;		
/****************************************************************/

 /****************************************
  * @brief  通用处理函数，全部功能在此实现
  * @param  无
  * @retval 无
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
  * @brief  CAN通信函数,标记功能状态，不做处理
  * @param  无
  * @retval 无
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
  * @brief  I2C通信处理函数
  * @param  无
  * @retval 无
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
  * @brief  ADC采集处理函数
  * @param  无
  * @retval 无
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
  * @brief  任务创建、初始化函数
  * @param  无
  * @retval 无
  ***************************************/
void Task_init(void)
{
	CAN_Handle = rt_thread_create( "CAN_Task",              /* 线程名字 */
                      CAN_Task,   				 /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      3,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (CAN_Handle != RT_NULL)    rt_thread_startup(CAN_Handle);
	
	I2C_Handle = rt_thread_create( "I2C_Task",              /* 线程名字 */
                      I2C_Task,   					/* 线程入口函数 */
                      RT_NULL,              /* 线程入口函数参数 */
                      1024,                 /* 线程栈大小 */
                      2,                    /* 线程的优先级 */
                      20);                  /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
//   if (I2C_Handle != RT_NULL)    rt_thread_startup(I2C_Handle);
	
	ADC_Handle = rt_thread_create( "ADC_Task",              /* 线程名字 */
                      ADC_Task,  					 /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      1,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (ADC_Handle != RT_NULL)    rt_thread_startup(ADC_Handle);
 
	DISPOSE_Handle = rt_thread_create( "Dispose_Task",              /* 线程名字 */
                      Dispose_Task,   		 /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      2,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (DISPOSE_Handle != RT_NULL)    rt_thread_startup(DISPOSE_Handle);
}
