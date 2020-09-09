/* 该文件统一用于存放中断服务函数 */
#include "stm32f10x_it.h"
#include "board.h"
#include "rtthread.h"

/***********************函数声明区*******************************/


/***********************声明返回区*******************************/

extern CanRxMsg RxMessage;	

/***********************全局变量区*******************************/
uint8_t REV_Finish_LEFT=0;
uint8_t REV_Finish_RIGHT=0;
/****************************************************************/
		


void CAN_RX_IRQHandler(void)
{
		rt_interrupt_enter();
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
		{
			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		}
//			for(int i=0;i<8;i++)	
//		{
//				printf("RxMessage = 0x%x\n",RxMessage.Data[i]);				
//		}
//		rt_kprintf("laile =0x%x\n",RxMessage.StdId);		
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		rt_interrupt_leave();
}





