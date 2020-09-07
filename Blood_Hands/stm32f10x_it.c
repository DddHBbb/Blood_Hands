/* ���ļ�ͳһ���ڴ���жϷ����� */
#include "stm32f10x_it.h"
#include "board.h"
#include "rtthread.h"

/***********************����������*******************************/


/***********************����������*******************************/
extern uint16_t ADC_ConvertedValue;
extern CanRxMsg RxMessage;	

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
		rt_kprintf("laile =0x%x\n",RxMessage.StdId);		
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		rt_interrupt_leave();
}
void ADC_IRQHandler(void)
{	
	rt_interrupt_enter();
	if (ADC_GetITStatus(ADCx,ADC_IT_EOC)!= RESET ) 
	{
		ADC_ConvertedValue = ADC_GetConversionValue(ADCx);
	}
	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
	rt_interrupt_leave();
}





