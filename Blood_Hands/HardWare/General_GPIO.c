#include "board.h" 

extern __IO uint16_t ADC_ConvertedValue[3];

static void All_GPIO_CONFIG(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = LeftOrRight_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_Init(LeftOrRight_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
}
/* 左右胳膊判断*/
int Is_LeftOrRight(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,LeftOrRight_Pin) == Blood_RIGHT)
	{
		return Blood_RIGHT;
	}
	else 
	{
		return Blood_LEFT;
	}
}
/* 脉搏跳动方式*/
void Pulse_Bounce_IO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t mode)
{
	switch(mode)
	{
		case 0x02:
							GPIOx->BRR = GPIO_Pin;
							rt_thread_delay(1800); //180ms
							GPIOx->BSRR = GPIO_Pin;
							break;
		case 0x03:
							GPIOx->BRR = GPIO_Pin;
							rt_thread_delay(600);//60ms
							GPIOx->BSRR = GPIO_Pin;		
							break;
		case 0x01:
							GPIOx->BRR = GPIO_Pin;
							rt_thread_delay(1200);	//120ms
							GPIOx->BSRR = GPIO_Pin;
							break;
		case 0x00:						
							GPIOx->BSRR = GPIO_Pin;
							break;		
		default : GPIOx->BSRR = GPIO_Pin;
							
	}
}
/* 袖带位置检测*/
uint8_t Position_Check(void)
{
	printf("ADC_ConvertedValue[1]=%f\t,ADC_ConvertedValue[2]=%f\n",(ADC_ConvertedValue[1]/4096.0)*3.3,(ADC_ConvertedValue[2]/4096.0)*3.3);
	printf("ADC_ConvertedValue[1]11=%d\t,ADC_ConvertedValue[2]22=%d\n",ADC_ConvertedValue[1],ADC_ConvertedValue[2]);
	if((ADC_ConvertedValue[1]/4096.0)*3.3>0.8 && (ADC_ConvertedValue[2]/4096.0)*3.3>1.8)
		{
			return 0x01; //正常
		}
	else if((ADC_ConvertedValue[1]/4096.0)*3.3>0.8 && (ADC_ConvertedValue[2]/4096.0)*3.3<1.8)
		{
			return 0x02;//偏上
		}
	else if((ADC_ConvertedValue[1]/4096.0)*3.3<0.8 && (ADC_ConvertedValue[2]/4096.0)*3.3>1.8)
		{
			return 0x03;//偏下
		}
	else if((ADC_ConvertedValue[1]/4096.0)*3.3 <0.8 && (ADC_ConvertedValue[2]/4096.0)*3.3<1.8)
		{
			return 0x00;//未佩戴
		}
	
	return 0;
}
void All_GPIO_init(void)
{
		All_GPIO_CONFIG();
}

/*
 * 设置 IWDG 的超时时间
 * Tout = prv/40 * rlv (s)
 *      prv可以是[4,8,16,32,64,128,256]
 * prv:预分频器值，取值如下：
 *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
 *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
 *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
 *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
 *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
 *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
 *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
 *
 * rlv:预分频器值，取值范围为：0-0XFFF
 * 函数调用举例：
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s 超时溢出
 */

void IWDG_Config(void)
{	
	// 使能 预分频寄存器PR和重装载寄存器RLR可写
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	
	// 设置预分频器值
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	
	// 设置重装载寄存器值
	IWDG_SetReload(625);
	
	// 把重装载寄存器的值放到计数器中
	IWDG_ReloadCounter();
	
	// 使能 IWDG
	IWDG_Enable();	
}

// 喂狗
void IWDG_Feed(void)
{
	// 把重装载寄存器的值放到计数器中，喂狗，防止IWDG复位
	// 当计数器的值减到0的时候会产生系统复位
	IWDG_ReloadCounter();
}



























