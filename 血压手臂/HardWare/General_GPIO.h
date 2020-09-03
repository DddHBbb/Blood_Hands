#ifndef __GENERAL_GPIO_H
#define __GENERAL_GPIO_H

#include "stm32f10x.h"



#define LeftOrRight_Port  GPIOB
#define LeftOrRight_Pin   GPIO_Pin_7




int Is_LeftOrRight(void);
void All_GPIO_init(void);
#endif









