#ifndef __L298N_H
#define __L298N_H
#include "main.h"
#include "common.h"
/* 
Timer 2 config:
	- PWM 25kHz
	- F		64MHz
	- PSC	0
	- ARR 2559
	- motor1 is TIM_CHANNEL1
	- motor2 is TIM_CHANNEL2
GPIO config:
	- L298N_IN1_GPIO_Pin, L298N_IN1_Pin
	- L298N_IN2_GPIO_Pin, L298N_IN2_Pin
	- L298N_IN3_GPIO_Pin, L298N_IN3_Pin
	- L298N_IN4_GPIO_Pin, L298N_IN4_Pin
	
	- BT1_GPIO_Pin, BT1_Pin
	- BT2_GPIO_Pin, BT2_Pin
	- BT3_GPIO_Pin, BT3_Pin
	- BT4_GPIO_Pin, BT4_Pin

*/



typedef enum
{
	MOTOR1,
	MOTOR2
} L298N_Index_t;
typedef enum
{
	CW,		// ClockWise
	CCW,	// CounterClockWise
	STOP
} L298N_Direction_t;
typedef struct
{
	Pin_t in1;
	Pin_t in2;
	Pin_t in3;
	Pin_t in4;
	
	float duty1;
	float duty2;
	uint16_t ch1;
	uint16_t ch2;
	
	TIM_HandleTypeDef* htim;
	
} L298N_t;

void L298N_Init(L298N_t* l298n, TIM_HandleTypeDef* htim, Pin_t in1, Pin_t in2, Pin_t in3, Pin_t in4, uint16_t ch1, uint16_t ch2);
void L298N_Ctrl(L298N_t* l298n, L298N_Index_t index, uint8_t sf, uint8_t sl);
void L298N_Forward(L298N_t* l298n, L298N_Index_t index);
void L298N_Backward(L298N_t* l298n, L298N_Index_t index);
void L298N_Braking(L298N_t* l298n, L298N_Index_t index);
void L298N_Set_Duty(L298N_t* l298n, L298N_Index_t index, float duty);
L298N_Direction_t L298N_Read_Direction(L298N_t* l298n, L298N_Index_t index);
void L298N_Update_Motor(L298N_t* l298n);

void L298N_Start(L298N_t* l298n, L298N_Index_t motor, float duty);
void L298N_Stop(L298N_t* l298n, L298N_Index_t motor);
uint8_t L298N_Check_channel(uint16_t channel);
void L298N_Toggle_Direction(L298N_t* l298n, L298N_Index_t motor);



#endif
