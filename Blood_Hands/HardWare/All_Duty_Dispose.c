#include "board.h"
#include "rtthread.h"

/***********************����������*******************************/
void Inquire_Pressure(void);
void Pulse_BounceIndex(uint8_t *data,uint8_t *Gong,uint8_t *Nao);
void Pulse_Bounce_OneShot(uint8_t *Gong,uint8_t *Nao);
void Blood_Pressure_Calibration(uint8_t *data);
void Pulse_Bounce_Dispose(uint8_t Current_Status_Gong,uint8_t Current_Status_Nao);
int Average_ADC(uint8_t Flag);
/***********************����������*******************************/
//extern rt_event_t Blood_Pressure_Calibration_Event;

extern CanTxMsg TxMessage;	
extern __IO uint16_t ADC_ConvertedValue[3];
extern rt_mailbox_t mx_can_Handle;
/***********************ȫ�ֱ�����*******************************/
int Value_mmHg[7]={0};
int Save_ADC_To_E2[7];
int Read_ADC_From_E2[8]={0};
float Calibration[7];

uint8_t Read_From_E2_Flag=0;						//ȷ��ֻ��ȡһ��У׼ֵ��ֻ���ڸ���У׼֮��ſɶ�ȡ����������


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

}
 /****************************************
  * @brief  CAN���մ�����
  * @param  ��
  * @retval ��
  ***************************************/
void CAN_Task_Dispose(void)
{	
	static uint8_t Gong_Pulse_Status=0xff;
	static uint8_t Nao_Pulse_Status=0xff;
	static uint8_t *CAN_Data=RT_NULL;
	
	rt_mb_recv(mx_can_Handle,(rt_uint32_t *)&CAN_Data,RT_WAITING_FOREVER);	
	switch(CAN_Data[0])
	{
		case 0xc1:
							Inquire_Pressure();
							break;
		case 0xc2:
							Pulse_BounceIndex(CAN_Data,&Gong_Pulse_Status,&Nao_Pulse_Status);
							break;
		case 0xc3:
							Pulse_Bounce_OneShot(&Gong_Pulse_Status,&Nao_Pulse_Status);
							break;
		case 0xc4:
							Blood_Pressure_Calibration(CAN_Data);
							break;
		default:	CAN_Data[0] = 0;			
	}	
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
void Pulse_BounceIndex(uint8_t *data,uint8_t *Gong,uint8_t *Nao)
{
	switch(data[1])
	{
		case 0x00:
		{
						*Gong = GONG_DISAPPEAR;
						break;
		}
		case 0x01:
		{
						*Gong = GONG_NORMAL;
						break;
		}
		case 0x02:
		{
						*Gong = GONG_UP;
						break;
		}
		case 0x03:
		{
						*Gong = GONG_DOWN;
						break;
		}
		default:	*Gong = 0x00;	
	}
	
	switch(data[2])
	{
		case 0x00:
		{
						*Nao = NAO_DISAPPEAR;
						break;
		}
		case 0x01:
		{
						*Nao = NAO_NORMAL;
						break;
		}
		case 0x02:
		{
						*Nao = NAO_UP;
						break;
		}
		case 0x03:
		{
						*Nao = NAO_DOWN;
						break;
		}
		default:	*Nao = 0x00;
	}
}
 /****************************************
  * @brief  ������������������һ����һ��
						����������������0x00������ʧ��0x01����������0x02������ǿ��0x03��������
  * @param  ��
  * @retval ��
  ****************************************/
void Pulse_Bounce_OneShot(uint8_t *Gong,uint8_t *Nao)
{
	switch(*Gong)
	{
		case GONG_DISAPPEAR:
											//��������ʧ
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_DISAPPEAR);
											break;
		case GONG_NORMAL:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_NORMAL);										
											break;
		case GONG_UP:
											//��������ǿ
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_UP);
											break;
		case GONG_DOWN:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_DOWN);
											break;
		default : 				Pulse_Bounce_IO(PULSE_PORT,GONG_PLUSE_PIN,GONG_DISAPPEAR);
	}
	switch(*Nao)
	{
		case NAO_DISAPPEAR:
											//��������ʧ
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_DISAPPEAR);
											break;
		case NAO_NORMAL:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_NORMAL);	
											break;
		case NAO_UP:
											//��������ǿ
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_UP);
											break;
		case NAO_DOWN:
											//����������
											Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_DOWN);
											break;
		default : 				Pulse_Bounce_IO(PULSE_PORT,NAO_PLUSE_PIN,NAO_DISAPPEAR);
	}
}
 /****************************************
  * @brief  ѪѹУ׼���������մ�����ҪУ׼��ֵ������������ϡ�������û���զд
  * @param  ��
  * @retval ��
  ****************************************/
void Blood_Pressure_Calibration(uint8_t *data)
{
	ADC_Cmd(ADCx, ENABLE);	
	switch(data[1])
	{
		case Zero_mmhg:																	
									//0mmhg										
									ee_WriteBytes((uint8_t *)&_0_mmHg,0x14,2);
									Save_ADC_To_E2[0]	= Average_ADC(0);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[0],0,2);
									TxMessage.Data[2] = Zero_mmhg;
//									rt_kprintf("Zero_mmhgn\n");
									break;
		case Fifty_mmhg:
									//50mmhg
									ee_WriteBytes((uint8_t *)&_50_mmHg,0x14+2,2);			
									Save_ADC_To_E2[1]	= Average_ADC(0);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[1],2,2);
//									rt_kprintf("Fifty_mmhg\n");
									TxMessage.Data[2] = Fifty_mmhg;
									break;
		case One_Hundred_mmhg:
									//100mmhg
									ee_WriteBytes((uint8_t *)&_100_mmHg,0x14+2*2,2);		
									Save_ADC_To_E2[2]	= Average_ADC(0);
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[2],2*2,2);
//									rt_kprintf("One_Hundred_mmhg\n");
									TxMessage.Data[2] = One_Hundred_mmhg;
									break;
		case One_Hundred_Fifty_mmhg:
									//150mmhg
									ee_WriteBytes((uint8_t *)&_150_mmHg,0x14+2*3,2);	
									Save_ADC_To_E2[3]	= Average_ADC(0);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[3],2*3,2);
//									rt_kprintf("One_Hundred_Fifty_mmhg\n");
									TxMessage.Data[2] = One_Hundred_Fifty_mmhg;
									break;
		case Two_Hundred_mmhg:
									//200mmhg
									ee_WriteBytes((uint8_t *)&_200_mmHg,0x14+2*4,2);		
									Save_ADC_To_E2[4]	= Average_ADC(0);
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[4],2*4,2);
									TxMessage.Data[2] = Two_Hundred_mmhg;
									break;
		case Two_Hundred_Fifty_mmhg:
									//250mmhg
									ee_WriteBytes((uint8_t *)&_250_mmHg,0x14+2*5,2);	
									Save_ADC_To_E2[5]	= Average_ADC(0);	
									ee_WriteBytes((uint8_t *)&Save_ADC_To_E2[5],2*5,2);		
									TxMessage.Data[2] = Two_Hundred_Fifty_mmhg;
									break;
		case Three_Hundred_mmhg:
									//300mmhg
									ee_WriteBytes((uint8_t *)&_300_mmHg,0x14+2*6,2);		
									Save_ADC_To_E2[6]	= Average_ADC(0);	
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
	static int data=0;
	int Current_Vlaue;
	/* ��ȡ���ݣ���ֵһ��*/
	if(!Read_From_E2_Flag)
	{	
		/* ֻ��ȡһ�Σ���ǰmmHg��ֵ��У׼λ�õ�ֵ*/
		for(int i=0;i<7;i++)
		{
			ee_ReadBytes((uint8_t *)&Read_ADC_From_E2[i],i*2,2);
			rt_thread_delay(10);
			ee_ReadBytes((uint8_t *)&Value_mmHg[i],0x14+i*2,2);
			rt_thread_delay(10);	
		}
		for(int i=0;i<6;i++) //ֻ�ɼ���200mmHg��Ҳ���Ը���
		{
			Calibration[i] = (float)(Read_ADC_From_E2[i+1] - Read_ADC_From_E2[i]) / (float)(Value_mmHg[i+1] - Value_mmHg[i]);
		}
		Read_From_E2_Flag = 1;
	}
	Current_Vlaue = Average_ADC(0);
	TxMessage.Data[0] = 0xd0;//Э��ID
	TxMessage.Data[1] = 0xaa;//ID  Ϲ���
	TxMessage.Data[2] = Is_LeftOrRight();//��첲�����Ҹ첲
	TxMessage.Data[3] = Position_Check();//���λ��
	rt_kprintf("Position_Check =%d\n",Position_Check());
	if(Position_Check() == 0x01)
	{
	/* ��ʼУ׼����*/
		for(int i=0;i<6;i++)
		{
			/* */
			if((Current_Vlaue >= Read_ADC_From_E2[i]) && (Current_Vlaue <= Read_ADC_From_E2[i+1]))
				{			
					if(Calibration[i] != 0)
						{
							data = (int)((float)(Current_Vlaue - Read_ADC_From_E2[i]) / Calibration[i]) + Value_mmHg[i];
						}						
				}
//			printf("Calibration =%f\t Read_ADC_From_E2 = %d\n",Calibration[i],Read_ADC_From_E2[i]);		
		}
		TxMessage.Data[4] = (data & 0xff00)>>8;
		TxMessage.Data[5] = data & 0xff;	
	}
	else
	{
		data=0;//���δ�󶨣�����Ϊ0
	}
	CAN_SetMsg(0x71,&TxMessage);
	CAN_Transmit(CAN1,&TxMessage);
	rt_kprintf("current =%d\n",Current_Vlaue);
//	rt_kprintf("positon =0x%x\n",Position_Check());
	rt_kprintf("data =%d\n",data);
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
const int N = 27;
int Average_ADC(uint8_t Flag)
{
	int value_buf[N],temp=0;                        
#if(Flag)
	float sum=0;
#endif
	unsigned char count,i,j;
	
	for (count=0;count<N;count++)
	{
					value_buf[count] = (int)ADC_ConvertedValue[0];  
					rt_thread_delay(1);//������Ϊ���г�for��ʹ��DMAʵʱֵ���Դ�����
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


































