#include "board.h"
#include "rtthread.h"

/***********************函数声明区*******************************/
void Inquire_Pressure(void);
void Pulse_BounceIndex(void);
void Pulse_Bounce_OneShot(void);
void Blood_Pressure_Calibration(void);
void Pulse_Bounce_Dispose(uint8_t Current_Status_Gong,uint8_t Current_Status_Nao);
int Average_ADC(uint8_t Flag,int AD_VALUE);
/***********************声明返回区*******************************/

extern CanRxMsg RxMessage;	
extern CanTxMsg TxMessage;	
extern rt_event_t Blood_Pressure_Calibration_Event;
/***********************全局变量区*******************************/
uint16_t ADC_ConvertedValue;
uint8_t Gong_Pulse_Status=0xff;
uint8_t Nao_Pulse_Status=0xff;
int Save_ADC_To_E2[7];
int Read_ADC_From_E2[8]={0};
float Calibration[7];
int Value_mmHg[7]={0};
uint8_t Read_From_E2_Flag=0;
uint8_t Switch_Flag=0;

const int  _0_mmHg   = 0;
const int  _50_mmHg  = 50;
const int  _100_mmHg = 100;
const int  _150_mmHg = 150;
const int  _200_mmHg = 200;
const int  _250_mmHg = 250;
const int  _300_mmHg = 300;
/****************************************************************/


 /****************************************
  * @brief  通用处理函数
  * @param  无
  * @retval 无
  ***************************************/
void General_Dispose(void)
{
	if(Switch_Flag == 1)
	{
		Pulse_Bounce_Dispose(Gong_Pulse_Status,Nao_Pulse_Status);//肱动脉处理
	}
}
 /****************************************
  * @brief  CAN接收处理函数
  * @param  无
  * @retval 无
  ***************************************/
void CAN_Task_Dispose(void)
{	
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
	Switch_Flag = 1;
	switch(RxMessage.Data[1])
	{
		case 0x00:
							Gong_Pulse_Status = GONG_DISAPPEAR;
							break;
		case 0x01:
							Gong_Pulse_Status = GONG_NORMAL;
							break;
		case 0x02:
							Gong_Pulse_Status = GONG_UP;
							break;
		case 0x03:
							Gong_Pulse_Status = GONG_DOWN;
							break;
		default:	Gong_Pulse_Status = 0xff;
							break;	
	}
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
}
 /****************************************
  * @brief  脉搏跳动处理函数，0x00脉搏消失，0x01脉搏正常，0x02脉搏增强，0x03脉搏减弱
  * @param  无
  * @retval 无
  ***************************************/
void Pulse_Bounce_Dispose(uint8_t Current_Status_Gong,uint8_t Current_Status_Nao)
{
	switch(Current_Status_Gong)
	{
		case GONG_DISAPPEAR:
											//肱脉搏消失
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_DISAPPEAR,DELAY_TIES);
											break;
		case GONG_NORMAL:
											//肱脉搏正常
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_NORMAL,DELAY_TIES);										
											break;
		case GONG_UP:
											//肱脉搏增强
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_UP,DELAY_TIES);
											break;
		case GONG_DOWN:
											//肱脉搏减弱
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_DOWN,DELAY_TIES);
											break;
	}
	switch(Current_Status_Nao)
	{
		case NAO_DISAPPEAR:
											//挠脉搏消失
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_DISAPPEAR,DELAY_TIES);
											break;
		case NAO_NORMAL:
											//挠脉搏正常
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_NORMAL,DELAY_TIES);	
											break;
		case NAO_UP:
											//挠脉搏增强
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_UP,DELAY_TIES);
											break;
		case NAO_DOWN:
											//挠脉搏减弱
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_DOWN,DELAY_TIES);
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
	Switch_Flag = 0;
	switch(Gong_Pulse_Status)
	{
		case GONG_DISAPPEAR:
											//肱脉搏单跳消失
											GPIOB->BSRR = GPIO_Pin_10;
											break;
		case GONG_UP:	
		case GONG_DOWN:	
		case GONG_NORMAL:
											//肱脉搏单跳正常
											GPIOB->BRR = GPIO_Pin_10;	
											rt_thread_delay(1000);
											GPIOB->BSRR = GPIO_Pin_10;
											break;
	}
	switch(Nao_Pulse_Status)
	{
		case NAO_DISAPPEAR:
											//挠脉搏单跳消失
											GPIOB->BSRR = GPIO_Pin_11;
											break;
		case NAO_UP:
		case NAO_DOWN:
		case NAO_NORMAL:
											//挠脉搏单跳正常
											GPIOB->BRR = GPIO_Pin_11;	
											rt_thread_delay(1000);
											GPIOB->BSRR = GPIO_Pin_11;
											break;
	}
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
									ee_WriteBytes((uint8_t *)&_0_mmHg,0x14,2);
									Save_ADC_To_E2[0]	= Average_ADC(0,ADC_ConvertedValue);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[0],0,2);
									TxMessage.Data[2] = Zero_mmhg;
									rt_kprintf("Zero_mmhgn\n");
									break;
		case Fifty_mmhg:
									//50mmhg
									ee_WriteBytes((uint8_t *)&_50_mmHg,0x14+2,2);			
									Save_ADC_To_E2[1]	= Average_ADC(0,ADC_ConvertedValue);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[1],2,2);
									rt_kprintf("Fifty_mmhg\n");
									TxMessage.Data[2] = Fifty_mmhg;
									break;
		case One_Hundred_mmhg:
									//100mmhg
									ee_WriteBytes((uint8_t *)&_100_mmHg,0x14+2*2,2);		
									Save_ADC_To_E2[2]	= Average_ADC(0,ADC_ConvertedValue);
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[2],2*2,2);
									rt_kprintf("One_Hundred_mmhg\n");
									TxMessage.Data[2] = One_Hundred_mmhg;
									break;
		case One_Hundred_Fifty_mmhg:
									//150mmhg
									ee_WriteBytes((uint8_t *)&_150_mmHg,0x14+2*3,2);	
									Save_ADC_To_E2[3]	= Average_ADC(0,ADC_ConvertedValue);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[3],2*3,2);
									rt_kprintf("One_Hundred_Fifty_mmhg\n");
									TxMessage.Data[2] = One_Hundred_Fifty_mmhg;
									break;
		case Two_Hundred_mmhg:
									//200mmhg
									ee_WriteBytes((uint8_t *)&_200_mmHg,0x14+2*4,2);		
									Save_ADC_To_E2[4]	= Average_ADC(0,ADC_ConvertedValue);
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[4],2*4,2);
									TxMessage.Data[2] = Two_Hundred_mmhg;
									break;
		case Two_Hundred_Fifty_mmhg:
									//250mmhg
									ee_WriteBytes((uint8_t *)&_250_mmHg,0x14+2*5,2);	
									Save_ADC_To_E2[5]	= Average_ADC(0,ADC_ConvertedValue);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[5],2*5,2);		
									TxMessage.Data[2] = Two_Hundred_Fifty_mmhg;
									break;
		case Three_Hundred_mmhg:
									//300mmhg
									ee_WriteBytes((uint8_t *)&_300_mmHg,0x14+2*6,2);		
									Save_ADC_To_E2[6]	= Average_ADC(0,ADC_ConvertedValue);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[6],2*6,2);	
									TxMessage.Data[2] = Three_Hundred_mmhg;
									break;
		default:
									break;
	}
	Read_From_E2_Flag = 0;
	TxMessage.Data[0] = 0xd1;
	TxMessage.Data[1] = Is_LeftOrRight();
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
	static int data=0,data1=0;
	int Current_Vlaue;
	/* 读取数据，赋值一次*/
	if(!Read_From_E2_Flag)
	{	
		/* 确保ommhG时ADC采集的值为0，相当于整体左移Read_ADC_From_E2[0]值的距离*/
		rt_thread_delay(10);
		for(int i=0;i<7;i++)
		{
			ee_ReadBytes((uint8_t *)&Read_ADC_From_E2[i],i*2,2);
			rt_thread_delay(10);
			ee_ReadBytes((uint8_t *)&Value_mmHg[i],0x14+i*2,2);
			rt_thread_delay(10);	
		}
		for(int i=0;i<6;i++)
		{
			Calibration[i] = (float)(Value_mmHg[i+1] - Value_mmHg[i]) / (float)(Read_ADC_From_E2[i+1] - Read_ADC_From_E2[i]);
		}
		Read_From_E2_Flag = 1;
	}
	//袖带位置，以此开启函数，未写
	
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
	Current_Vlaue = Average_ADC(0,ADC_ConvertedValue);
	TxMessage.Data[0] = 0xd0;//协议ID
	TxMessage.Data[1] = 0xaa;//ID  瞎编的
	TxMessage.Data[2] = Is_LeftOrRight();//左胳膊还是右胳膊
	TxMessage.Data[3] = 0;//袖带位置
	/* 开始校准数据*/
	for(int i=0;i<6;i++)
	{
		/* 由于环境问题，总会出现初始值不为0的情况，以这种方式去平衡它*/
		if(Current_Vlaue < 75)
			{
				data1 = (int)((float)Current_Vlaue * Calibration[0]);	
			}
		if((Current_Vlaue >= Read_ADC_From_E2[i]) && (Current_Vlaue < Read_ADC_From_E2[i+1]))
			{				
				data = (int)((float)Current_Vlaue * Calibration[i]) - data1;		
			}
		printf("Calibration =%f\n",Calibration[i]);		
	}
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


































