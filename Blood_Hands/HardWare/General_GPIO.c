#include "board.h" 


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
/* ×óÓÒ¸ì²²ÅÐ¶Ï*/
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
/* Âö²«Ìø¶¯·½Ê½*/
void Pulse_Bounce_IO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t mode,uint16_t time)
{
	switch(mode)
	{
		case 0x02:
							GPIOx->BRR = GPIO_Pin;
							rt_thread_delay(time*1);
							GPIOx->BSRR = GPIO_Pin;
							break;
		case 0x03:
							GPIOx->BRR = GPIO_Pin;
							rt_thread_delay(time/40);
							GPIOx->BSRR = GPIO_Pin;		
							break;
		case 0x01:
							GPIOx->BRR = GPIO_Pin;
							rt_thread_delay(time/10);	
							GPIOx->BSRR = GPIO_Pin;
							break;
		case 0x00:						
							GPIOx->BSRR = GPIO_Pin;
							break;		
	}
}
uint8_t Position_Check(void)
{
	if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 1) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 1))
		{
			return 0x01; //Õý³£
		}
	if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 1) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 0))
		{
			return 0x02;//Æ«ÉÏ
		}
	if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 1))
		{
			return 0x03;//Æ«ÏÂ
		}
	if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0) && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 0))
		{
			return 0x00;//Î´¼ì²â
		}

	
}
void All_GPIO_init(void)
{
		All_GPIO_CONFIG();
}