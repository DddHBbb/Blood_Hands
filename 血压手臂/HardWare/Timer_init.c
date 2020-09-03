#include "board.h"
#include "rtthread.h"

/***********************����������*******************************/


/***********************����������*******************************/
extern rt_sem_t Sem1_Hanle; 
extern rt_sem_t Blood_Pressure_Calibration_Sem;
/***********************ȫ�ֱ�����*******************************/
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
	swtmr1 = rt_timer_create("swtmr1_callback", /* �����ʱ�������� */
                        swtmr1_callback,/* �����ʱ���Ļص����� */
                        0,			/* ��ʱ����ʱ��������ڲ��� */
                        10000,   /* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                        RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
                        /* �����ʱ��ģʽ һ��ģʽ */

  if (swtmr1 != RT_NULL) rt_timer_start(swtmr1);
  
	swtmr2 = rt_timer_create("swtmr2_callback", /* �����ʱ�������� */
                        swtmr2_callback,/* �����ʱ���Ļص����� */
                        0,			/* ��ʱ����ʱ��������ڲ��� */
                        10000,   /* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                        RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
                        /* �����ʱ��ģʽ ����ģʽ */
  if (swtmr2 != RT_NULL) rt_timer_start(swtmr2);
}


