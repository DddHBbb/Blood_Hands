#include "board.h"
#include "rtthread.h"

/***********************函数声明区*******************************/


/***********************声明返回区*******************************/
extern rt_sem_t Sem1_Hanle; 
extern rt_sem_t Blood_Pressure_Calibration_Sem;
/***********************全局变量区*******************************/
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;
static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;
/****************************************************************/

static void swtmr1_callback(void* parameter)
{		
 rt_sem_release(Blood_Pressure_Calibration_Sem);  
}

static void swtmr2_callback(void* parameter)
{			
	  rt_sem_release(Sem1_Hanle);
	
}


void Timer_init(void)
{	
	swtmr1 = rt_timer_create("swtmr1_callback", /* 软件定时器的名称 */
                        swtmr1_callback,/* 软件定时器的回调函数 */
                        0,			/* 定时器超时函数的入口参数 */
                        10000,   /* 软件定时器的超时时间(周期回调时间) */
                        RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
                        /* 软件定时器模式 一次模式 */

  if (swtmr1 != RT_NULL) rt_timer_start(swtmr1);
  
	swtmr2 = rt_timer_create("swtmr2_callback", /* 软件定时器的名称 */
                        swtmr2_callback,/* 软件定时器的回调函数 */
                        0,			/* 定时器超时函数的入口参数 */
                        10000,   /* 软件定时器的超时时间(周期回调时间) */
                        RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
                        /* 软件定时器模式 周期模式 */
  if (swtmr2 != RT_NULL) rt_timer_start(swtmr2);
}


