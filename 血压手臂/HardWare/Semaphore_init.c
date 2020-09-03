#include "board.h"
#include "rtthread.h"
/***********************函数声明区*******************************/


/***********************声明返回区*******************************/


/***********************全局变量区*******************************/
rt_sem_t Sem1_Hanle=RT_NULL;
rt_sem_t Blood_Pressure_Calibration_Sem = RT_NULL;

/****************************************************************/
void Semaphore_init(void)
{
	Sem1_Hanle = rt_sem_create("srm1",1,RT_IPC_FLAG_FIFO);
	
	Blood_Pressure_Calibration_Sem = rt_sem_create("Blood_Pressure_Calibration_Sem",2,RT_IPC_FLAG_FIFO);
}
void Event_init(void)
{
	 //Blood_Pressure_Calibration_Event = rt_event_create("Blood_Pressure_Calibration_Event",RT_IPC_FLAG_PRIO);
	
}












