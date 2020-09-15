/* ���ļ�ͳһ���ڴ���жϷ����� */
#include "stm32f10x_it.h"
#include "board.h"
#include "rtthread.h"

/***********************����������*******************************/


/***********************����������*******************************/

extern CanRxMsg RxMessage;	
extern rt_mailbox_t mx_can_Handle;

/***********************ȫ�ֱ�����*******************************/
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
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);			
		rt_interrupt_leave();
		rt_mb_send(mx_can_Handle,(rt_uint32_t)RxMessage.Data);
}





