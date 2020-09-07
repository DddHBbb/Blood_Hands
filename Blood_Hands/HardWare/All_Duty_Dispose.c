#include "board.h"
#include "rtthread.h"

/***********************����������*******************************/
void Inquire_Pressure(void);
void Pulse_BounceIndex(void);
void Pulse_Bounce_OneShot(void);
void Blood_Pressure_Calibration(void);
void Pulse_Bounce_Dispose(uint8_t Current_Status_Gong,uint8_t Current_Status_Nao);
int Average_ADC(uint8_t Flag,int AD_VALUE);
/***********************����������*******************************/

extern CanRxMsg RxMessage;	
extern CanTxMsg TxMessage;	
extern rt_event_t Blood_Pressure_Calibration_Event;
/***********************ȫ�ֱ�����*******************************/
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
  * @brief  ͨ�ô�����
  * @param  ��
  * @retval ��
  ***************************************/
void General_Dispose(void)
{
	if(Switch_Flag == 1)
	{
		Pulse_Bounce_Dispose(Gong_Pulse_Status,Nao_Pulse_Status);//�Ŷ�������
	}
}
 /****************************************
  * @brief  CAN���մ�����
  * @param  ��
  * @retval ��
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
  * @brief  ѹ����ѯ������
  * @param  ��
  * @retval ��
  ***************************************/
void Inquire_Pressure(void)
{
		/* 
	
		�˴����ò�ѯ�ĺ��� 
	
	*/
}
 /****************************************
  * @brief  ��������״̬��Ǻ�����ֻ��ǵ�ǰ״̬����������
  * @param  ��
  * @retval ��
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
  * @brief  ����������������0x00������ʧ��0x01����������0x02������ǿ��0x03��������
  * @param  ��
  * @retval ��
  ***************************************/
void Pulse_Bounce_Dispose(uint8_t Current_Status_Gong,uint8_t Current_Status_Nao)
{
	switch(Current_Status_Gong)
	{
		case GONG_DISAPPEAR:
											//��������ʧ
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_DISAPPEAR,DELAY_TIES);
											break;
		case GONG_NORMAL:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_NORMAL,DELAY_TIES);										
											break;
		case GONG_UP:
											//��������ǿ
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_UP,DELAY_TIES);
											break;
		case GONG_DOWN:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_DOWN,DELAY_TIES);
											break;
	}
	switch(Current_Status_Nao)
	{
		case NAO_DISAPPEAR:
											//��������ʧ
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_DISAPPEAR,DELAY_TIES);
											break;
		case NAO_NORMAL:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_NORMAL,DELAY_TIES);	
											break;
		case NAO_UP:
											//��������ǿ
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_UP,DELAY_TIES);
											break;
		case NAO_DOWN:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_DOWN,DELAY_TIES);
											break;
	}
}
 /****************************************
  * @brief  ������������������һ����һ��
  * @param  ��
  * @retval ��
  ****************************************/
void Pulse_Bounce_OneShot(void)
{
	Switch_Flag = 0;
	switch(Gong_Pulse_Status)
	{
		case GONG_DISAPPEAR:
											//������������ʧ
											GPIOB->BSRR = GPIO_Pin_10;
											break;
		case GONG_UP:	
		case GONG_DOWN:	
		case GONG_NORMAL:
											//��������������
											GPIOB->BRR = GPIO_Pin_10;	
											rt_thread_delay(1000);
											GPIOB->BSRR = GPIO_Pin_10;
											break;
	}
	switch(Nao_Pulse_Status)
	{
		case NAO_DISAPPEAR:
											//������������ʧ
											GPIOB->BSRR = GPIO_Pin_11;
											break;
		case NAO_UP:
		case NAO_DOWN:
		case NAO_NORMAL:
											//��������������
											GPIOB->BRR = GPIO_Pin_11;	
											rt_thread_delay(1000);
											GPIOB->BSRR = GPIO_Pin_11;
											break;
	}
}
 /****************************************
  * @brief  ѪѹУ׼���������մ�����ҪУ׼��ֵ������������ϡ�������û���զд
  * @param  ��
  * @retval ��
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
  * @brief  ADC���մ�����
  * @param  ��
  * @retval ��
  ****************************************/
void ADC_DataToSend(void)
{
	static int data=0,data1=0;
	int Current_Vlaue;
	/* ��ȡ���ݣ���ֵһ��*/
	if(!Read_From_E2_Flag)
	{	
		/* ȷ��ommhGʱADC�ɼ���ֵΪ0���൱����������Read_ADC_From_E2[0]ֵ�ľ���*/
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
	//���λ�ã��Դ˿���������δд
	
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
	Current_Vlaue = Average_ADC(0,ADC_ConvertedValue);
	TxMessage.Data[0] = 0xd0;//Э��ID
	TxMessage.Data[1] = 0xaa;//ID  Ϲ���
	TxMessage.Data[2] = Is_LeftOrRight();//��첲�����Ҹ첲
	TxMessage.Data[3] = 0;//���λ��
	/* ��ʼУ׼����*/
	for(int i=0;i<6;i++)
	{
		/* ���ڻ������⣬�ܻ���ֳ�ʼֵ��Ϊ0������������ַ�ʽȥƽ����*/
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
	* @brief  ADC�����Ѿ������������
  * @param  ��
  * @retval ��
  ****************************************/
void ADC_Task_Dispose(void)
{
	ADC_DataToSend();
}
 /****************************************
  * @brief  ADC��ֵ�˲���ƽ���˲�
  * @param  ��
  * @retval ��
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
	
	for(j=0;j<N-1;j++)    //����
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
	for(count=1;count<N-1;count++)   //��λ��ƽ�� ��Flag��־����
					sum+=value_buf[count];
					return (float)(sum/(N-2));
#endif
	return value_buf[(N-1)/2]; 
}


































