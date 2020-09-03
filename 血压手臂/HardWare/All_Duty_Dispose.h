#ifndef __All_Duty_Dispose__H
#define __All_Duty_Dispose__H

#include "stm32f10x.h"

enum Parameter
{
	Blood_LEFT = 0x00,
	Blood_RIGHT,
	GONG_DISAPPEAR = 0x00,
	GONG_NORMAL,
	GONG_UP,
	GONG_DOWN,
	NAO_DISAPPEAR = 0x00,
	NAO_NORMAL,
	NAO_UP,
	NAO_DOWN
};

enum MMHG
{
	Zero_mmhg = 0x00,
	Fifty_mmhg = 0x32,
	One_Hundred_mmhg = 0x64,
	One_Hundred_Fifty_mmhg = 0x96,
	Two_Hundred_mmhg = 0xc8,
	Two_Hundred_Fifty_mmhg = 0xFA,
	Three_Hundred_mmhg = 0xFF,
	Done = 0,
	UNDONE = 1
		
};

#define  _0_mmHg  	0
#define  _50_mmHg  	50
#define  _100_mmHg  100
#define  _150_mmHg  150
#define  _200_mmHg  200
#define  _250_mmHg  250
#define  _300_mmHg  300


void CAN_Task_Dispose(void);
void ADC_Task_Dispose(void);
void General_Dispose(void);




#endif







































