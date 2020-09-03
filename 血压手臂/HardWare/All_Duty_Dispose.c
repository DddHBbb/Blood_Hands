#include "board.h"
#include "rtthread.h"

/***********************函数声明区*******************************/
void Inquire_Pressure(void);
void Pulse_BounceIndex(void);
void Pulse_Bounce_OneShot(void);
void Blood_Pressure_Calibration(void);
void Pulse_Bounce_Dispose(uint8_t Current_Status_Gong,uint8_t Current_Status_Nao,uint8_t index);
int Average_ADC(uint8_t Flag,int AD_VALUE);
/***********************声明返回区*******************************/

extern CanRxMsg RxMessage;	
extern CanTxMsg TxMessage;	
extern rt_event_t Blood_Pressure_Calibration_Event;
/***********************全局变量区*******************************/
double Get_AnalogValue;
uint16_t ADC_ConvertedValue;
uint8_t Hong_Pulse_Status=0xff;
uint8_t Nao_Pulse_Status=0xff;
int Comp_ADC_Vlaue[7];
float Calibration[7];
int Value_mmHg[7]={0};
int ADC_Value=0;
/****************************************************************/


 /****************************************
  * @brief  通用处理函数
  * @param  无
  * @retval 无
  ***************************************/
void General_Dispose(void)
{
	
}
 /****************************************
  * @brief  CAN接收处理函数
  * @param  无
  * @retval 无
  ***************************************/
void CAN_Task_Dispose(void)
{
//			printf("收到了=%x\n",RxMessage.StdId);		
			switch(RxMessage.Data[0])
			{
				case 0xc1:
									Inquire_Pressure();
									break;
				case 0xc2:
									Pulse_BounceIndex();
									break;
				case 0xc3:
									Pulse_Bounce_OneShot();
									break;
				case 0xc4:
									Blood_Pressure_Calibration();
									break;
				default:	RxMessage.Data[0] = 0;
									break;				
			}	
			RxMessage.Data[0] = 0;
}		
 /****************************************
  * @brief  压力查询处理函数
  * @param  无
  * @retval 无
  ***************************************/
void Inquire_Pressure(void)
{
		/* 
	
		此处放置查询的函数 
	
	*/
}
 /****************************************
  * @brief  脉搏跳动状态标记函数，只标记当前状态，不做处理
  * @param  无
  * @retval 无
  ***************************************/
void Pulse_BounceIndex(void)
{
	switch(RxMessage.Data[1])
	{
		case 0x00:
							Hong_Pulse_Status = GONG_DISAPPEAR;
							break;
		case 0x01:
							Hong_Pulse_Status = GONG_NORMAL;
							break;
		case 0x02:
							Hong_Pulse_Status = GONG_UP;
							break;
		case 0x03:
							Hong_Pulse_Status = GONG_DOWN;
							break;
		default:	Hong_Pulse_Status = 0xff;
							break;
		
	}
//	RxMessage.Data[1] = 0xff;
	switch(RxMessage.Data[2])
	{
		case 0x00:
							Nao_Pulse_Status = NAO_DISAPPEAR;
							break;
		case 0x01:
							Nao_Pulse_Status = NAO_NORMAL;
							break;
		case 0x02:
							Nao_Pulse_Status = NAO_UP;
							break;
		case 0x03:
							Nao_Pulse_Status = NAO_DOWN;
							break;
		default:	Nao_Pulse_Status = 0xff;
						  break;
		
	}
	Pulse_Bounce_Dispose(Hong_Pulse_Status,Nao_Pulse_Status,Is_LeftOrRight());//肱动脉处理
//	RxMessage.Data[2] = 0xff;
}
 /****************************************
  * @brief  脉搏跳动处理函数，0x00脉搏消失，0x01脉搏正常，0x02脉搏增强，0x03脉搏减弱
  * @param  无
  * @retval 无
  ***************************************/
void Pulse_Bounce_Dispose(uint8_t Current_Status_Gong,uint8_t Current_Status_Nao,uint8_t index)
{
	switch(Current_Status_Gong)
	{
		case GONG_DISAPPEAR:
											//肱脉搏消失
										if(index == Blood_LEFT)
											{
												rt_kprintf("左肱脉搏消失\n");
											}
										else
											{
												rt_kprintf("右肱脉搏消失\n");
											}
											break;
		case GONG_NORMAL:
											//肱脉搏正常
										if(index == Blood_LEFT)
											{
												rt_kprintf("左肱脉搏正常\n");
											}
										else
											{
												rt_kprintf("右肱脉搏正常\n");
											}
											break;
		case GONG_UP:
											//肱脉搏增强
										if(index == Blood_LEFT)
											{
												rt_kprintf("左肱脉搏增强\n");
											}
										else
											{
												rt_kprintf("右肱脉搏增强\n");
											}
											break;
		case GONG_DOWN:
											//肱脉搏减弱
										if(index == Blood_LEFT)
											{
												rt_kprintf("左肱脉搏减弱\n");
											}
										else
											{
												rt_kprintf("右肱脉搏减弱\n");
											}
											break;
		default : Current_Status_Gong = 0xff;
											break;
	}
	switch(Current_Status_Nao)
	{
		case NAO_DISAPPEAR:
											//挠脉搏消失
										if(index == Blood_LEFT)
											{
												rt_kprintf("左挠脉搏消失\n");
											}
										else
											{
												rt_kprintf("右挠脉搏消失\n");
											}
											break;
		case NAO_NORMAL:
											//挠脉搏正常
										if(index == Blood_LEFT)
											{
												rt_kprintf("左挠脉搏正常\n");
											}
										else
											{
												rt_kprintf("右挠脉搏正常\n");
											}
											break;
		case NAO_UP:
											//挠脉搏增强
										if(index == Blood_LEFT)
											{
												rt_kprintf("左挠脉搏增强\n");
											}
										else
											{
												rt_kprintf("右挠脉搏增强\n");
											}
											break;
		case NAO_DOWN:
											//挠脉搏减弱
										if(index == Blood_LEFT)
											{
												rt_kprintf("左挠脉搏减弱\n");
											}
										else
											{
												rt_kprintf("右挠脉搏减弱\n");
											}
											break;
		default:	Current_Status_Nao = 0xff;
						  break;
	}
		

}
 /****************************************
  * @brief  单次脉搏跳动，接受一次跳一次
  * @param  无
  * @retval 无
  ****************************************/
void Pulse_Bounce_OneShot(void)
{
	if(Is_LeftOrRight() == Blood_LEFT)
	{
		rt_kprintf("左脉搏跳一下\n");
	}
	else
	{
		rt_kprintf("右脉搏跳一下\n");
	}
	RxMessage.Data[0] = 0;
}
 /****************************************
  * @brief  血压校准函数，接收传入需要校准的值，结果计算整合。。。还没想好咋写
  * @param  无
  * @retval 无
  ****************************************/
void Blood_Pressure_Calibration(void)
{
	ADC_Cmd(ADCx, ENABLE);	
	switch(RxMessage.Data[1])
	{
		case Zero_mmhg:																	
									//0mmhg									
									Value_mmHg[0] = _0_mmHg;	
									Comp_ADC_Vlaue[0]	= Average_ADC(0,ADC_ConvertedValue);					
									TxMessage.Data[1] = Zero_mmhg;
									rt_kprintf("Zero_mmhgn\n");
									break;
		case Fifty_mmhg:
									//50mmhg
									Value_mmHg[1] = _50_mmHg;			
									Comp_ADC_Vlaue[1]	= Average_ADC(0,ADC_ConvertedValue);	
									Calibration[1] = (float)(_50_mmHg - _0_mmHg) / (float)(Comp_ADC_Vlaue[1] - Comp_ADC_Vlaue[0]);
									rt_kprintf("Fifty_mmhg\n");
									TxMessage.Data[1] = Fifty_mmhg;
									break;
		case One_Hundred_mmhg:
									//100mmhg
									Value_mmHg[2] = _100_mmHg;		
									Comp_ADC_Vlaue[2]	= Average_ADC(0,ADC_ConvertedValue);	
									Calibration[2] = (float)(_100_mmHg - _50_mmHg) / (float)(Comp_ADC_Vlaue[2] - Comp_ADC_Vlaue[1]);
									rt_kprintf("One_Hundred_mmhg\n");
									TxMessage.Data[1] = One_Hundred_mmhg;
									break;
		case One_Hundred_Fifty_mmhg:
									//150mmhg
									Value_mmHg[3] = _150_mmHg;	
									Comp_ADC_Vlaue[3]	= Average_ADC(0,ADC_ConvertedValue);	
									Calibration[3] = (float)(_150_mmHg - _100_mmHg) / (float)(Comp_ADC_Vlaue[3] - Comp_ADC_Vlaue[2]);
									rt_kprintf("One_Hundred_Fifty_mmhg\n");
									TxMessage.Data[1] = One_Hundred_Fifty_mmhg;
									break;
		case Two_Hundred_mmhg:
									//200mmhg
									Value_mmHg[4] = _200_mmHg;		
									Comp_ADC_Vlaue[4]	= Average_ADC(0,ADC_ConvertedValue);	
									Calibration[4] = (float)(_200_mmHg - _150_mmHg) / (float)(Comp_ADC_Vlaue[4] - Comp_ADC_Vlaue[3]);
									TxMessage.Data[1] = Two_Hundred_mmhg;
									break;
		case Two_Hundred_Fifty_mmhg:
									//250mmhg
									Value_mmHg[5] = _250_mmHg;	
									Comp_ADC_Vlaue[5]	= Average_ADC(0,ADC_ConvertedValue);	
									Calibration[5] = (float)(_250_mmHg - _200_mmHg) / (float)(Comp_ADC_Vlaue[5] - Comp_ADC_Vlaue[4]);
									TxMessage.Data[1] = Two_Hundred_Fifty_mmhg;
									break;
		case Three_Hundred_mmhg:
									//300mmhg
									Value_mmHg[6] = _300_mmHg;		
									Comp_ADC_Vlaue[6]	= Average_ADC(0,ADC_ConvertedValue);	
									Calibration[6] = (float)(_300_mmHg - _250_mmHg) / (float)(Comp_ADC_Vlaue[6] - Comp_ADC_Vlaue[5]);
									TxMessage.Data[1] = Three_Hundred_mmhg;
									break;
		default:
									break;
	}
	TxMessage.Data[0] = 0xd1;
	CAN_SetMsg(0x71,&TxMessage);
	CAN_Transmit(CAN1,&TxMessage);
	
}
 /****************************************
  * @brief  ADC接收处理函数
  * @param  无
  * @retval 无
  ****************************************/
void ADC_DataToSend(void)
{
	static int data=0; 
	int Current_Vlaue;
	
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
	Current_Vlaue = Average_ADC(0,ADC_ConvertedValue);
	TxMessage.Data[0] = 0xd0;//协议ID
	TxMessage.Data[1] = 0xaa;//ID  瞎编的
	TxMessage.Data[2] = Is_LeftOrRight();//左胳膊还是右胳膊
	TxMessage.Data[3] = 0;//袖带位置
	/* 开始校准数据*/
	for(int i=0;i<7;i++)
	{
		if((Current_Vlaue >= Comp_ADC_Vlaue[i]) && (Current_Vlaue < Comp_ADC_Vlaue[i+1]))
		{		
			data = (int)((float)Current_Vlaue * Calibration[i+1]);		
		}
		printf("Calibration =%f\n",Calibration[1]);
	}
	rt_thread_delay(1);
	TxMessage.Data[4] = (data & 0xff00)>>8;
	TxMessage.Data[5] = data & 0xff;	
	CAN_SetMsg(0x71,&TxMessage);
	CAN_Transmit(CAN1,&TxMessage);
	rt_kprintf("current =%d\n",Current_Vlaue);
	rt_kprintf("data =%d\n",data);
	ADC_SoftwareStartConvCmd(ADCx,DISABLE);
}
 /****************************************
	* @brief  ADC发送已经处理过的数据
  * @param  无
  * @retval 无
  ****************************************/
void ADC_Task_Dispose(void)
{
	ADC_DataToSend();
}
 /****************************************
  * @brief  ADC中值滤波和平均滤波
  * @param  无
  * @retval 无
  ****************************************/
#define N  13
int Average_ADC(uint8_t Flag,int AD_VALUE)
{
	int value_buf[N],temp=0;                        
#if(Flag)
	float sum=0;
#endif
	unsigned char count,i,j;
	
	for (count=0;count<N;count++)
	{
					value_buf[count] = AD_VALUE;                                                                                              
	}
	
	for(j=0;j<N-1;j++)    //排序
	{
		for(i=0;i<N-j-1;i++)        
			{
			if ( value_buf[i] > value_buf[i+1] )
					{
							temp = value_buf[i];
							value_buf[i] = value_buf[i+1];
							value_buf[i+1] = temp;
					}
			}
	}
#if(Flag)
	for(count=1;count<N-1;count++)   //中位求平均 由Flag标志控制
					sum+=value_buf[count];
					return (float)(sum/(N-2));
#endif
	return value_buf[(N-1)/2]; 
}


































