#ifndef __BOARD_H__
#define __BOARD_H__

/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/
#include "rtthread.h"
/* STM32 固件库头文件 */
#include "stm32f10x.h"

/* 开发板硬件bsp头文件 */
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_i2c_gpio.h"
#include "bsp_i2c_ee.h"
#include "bsp_can.h"
#include "bsp_adc.h"
#include "General_GPIO.h"
#include "All_Duty_Dispose.h"

/*
*************************************************************************
*                               函数声明
*************************************************************************
*/

void rt_hw_board_init(void);
void SysTick_Handler(void);

void Timer_init(void);
void Task_init(void);
void CAN_Task(void* parameter);
void I2C_Task(void* parameter);
void ADC_Task(void* parameter);
void Semaphore_init(void);
void Dispose_Task(void* parameter);

#endif /* __BOARD_H__ */
