#ifndef __GENERAL_GPIO_H
#define __GENERAL_GPIO_H

#include "stm32f10x.h"



#define LeftOrRight_Port  GPIOB
#define LeftOrRight_Pin   GPIO_Pin_7

#define PULSE_PORT  		 GPIOB
#define GONG_PLUSE_PIN   GPIO_Pin_10
#define NAO_PLUSE_PIN    GPIO_Pin_11
#define DELAY_TIES       10000

int Is_LeftOrRight(void);
void All_GPIO_init(void);
void Pulse_Bounce_IO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint8_t mode,uint16_t time);
#endif









