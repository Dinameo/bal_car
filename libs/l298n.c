#include "main.h"
#include "math.h"
#include "l298n.h"
#include "conf.h"

void L298N_Init(L298N_t* l298n, TIM_HandleTypeDef* htim, Pin_t in1, Pin_t in2, Pin_t in3, Pin_t in4, uint16_t ch1, uint16_t ch2)
{
	l298n->in1 = in1;
	l298n->in2 = in2;
	l298n->in3 = in3;
	l298n->in4 = in4;


	l298n->duty1 = 0;
	l298n->duty2 = 0;
	l298n->ch1 = ch1;
	l298n->ch2 = ch2;
	l298n->htim = htim;



	if(L298N_Check_channel(l298n->ch1))
	{
		L298N_Stop(l298n, MOTOR1);
		HAL_TIM_PWM_Start(l298n->htim, l298n->ch1);
	}
	if(L298N_Check_channel(l298n->ch2))
	{
		L298N_Stop(l298n, MOTOR2);
		HAL_TIM_PWM_Start(l298n->htim, l298n->ch2);
	}
}
void L298N_Ctrl(L298N_t* l298n, L298N_Index_t index, uint8_t sf, uint8_t sl) 
{
	if(index == MOTOR1)
	{
		HAL_GPIO_WritePin(l298n->in1.port, l298n->in1.pin, sf);
		HAL_GPIO_WritePin(l298n->in2.port, l298n->in2.pin, sl);
	} else if(index == MOTOR2)
	{
		HAL_GPIO_WritePin(l298n->in3.port, l298n->in3.pin, sl);
		HAL_GPIO_WritePin(l298n->in4.port, l298n->in4.pin, sf);
	}
}
void L298N_Forward(L298N_t* l298n, L298N_Index_t index)
{
	L298N_Ctrl(l298n, index, 1, 0);
}
void L298N_Backward(L298N_t* l298n, L298N_Index_t index)
{
	L298N_Ctrl(l298n, index, 0, 1);
}
void L298N_Braking(L298N_t* l298n, L298N_Index_t index)
{
	L298N_Ctrl(l298n, index, 0, 0);
}

void L298N_Set_Duty(L298N_t* l298n, L298N_Index_t index, float duty)
{
	if(duty > 100.0)
		duty = 100.0;
	else if(duty < -100)
		duty = -100.0;
	
	
	uint16_t CCR = (fabs(duty)*l298n->htim->Init.Period)/100.0;
	if(index == MOTOR1)
	{
		l298n->duty1 = duty;
		__HAL_TIM_SET_COMPARE(l298n->htim, l298n->ch1, CCR);
	}
	else if(index == MOTOR2)
	{
		l298n->duty2 = duty;
		__HAL_TIM_SET_COMPARE(l298n->htim, l298n->ch2, CCR);
	}
}
L298N_Direction_t L298N_Read_Direction(L298N_t* l298n, L298N_Index_t index)
{
	uint8_t ina, inb;
	if(index == MOTOR1)
	{
		ina = HAL_GPIO_ReadPin(l298n->in1.port, l298n->in1.pin);
		inb = HAL_GPIO_ReadPin(l298n->in2.port, l298n->in2.pin);
	} else if(index == MOTOR2)
	{
		ina = HAL_GPIO_ReadPin(l298n->in3.port, l298n->in3.pin);
		inb = HAL_GPIO_ReadPin(l298n->in4.port, l298n->in4.pin);
	}
	
	if(ina == 1 && inb == 0)
		return CW;
	if(ina == 0 && inb == 1)
		return CCW;	
	return STOP;
}
void L298N_Toggle_Direction(L298N_t* l298n, L298N_Index_t motor)
{
	L298N_Direction_t dir = L298N_Read_Direction(l298n, motor);
	if(dir == CW)
		L298N_Backward(l298n, motor);
	else if(dir == CCW)
		L298N_Forward(l298n, motor);
}
void L298N_Start(L298N_t* l298n, L298N_Index_t motor, float duty)
{
	L298N_Set_Duty(l298n, motor, duty);
	L298N_Forward(l298n, motor);
}
void L298N_Stop(L298N_t* l298n, L298N_Index_t motor)
{
	L298N_Set_Duty(l298n, motor, 0);
	L298N_Braking(l298n, motor);
}
uint8_t L298N_Check_channel(uint16_t channel)
{
	if((channel == TIM_CHANNEL_1) || (channel == TIM_CHANNEL_2) || (channel == TIM_CHANNEL_3) || (channel == TIM_CHANNEL_4))
		return 1;
	return 0;
}
void L298N_Update_Motor(L298N_t* l298n)
{
	if(l298n->duty1 > 0)
	{
		L298N_Backward(l298n, MOTOR1);
		L298N_Set_Duty(l298n, MOTOR1, l298n->duty1);
	}
	else
	{
		L298N_Set_Duty(l298n, MOTOR1, l298n->duty1);
		L298N_Forward(l298n, MOTOR1);
	}

	if(l298n->duty2 > 0)
	{
		L298N_Backward(l298n, MOTOR2);
		L298N_Set_Duty(l298n, MOTOR2, l298n->duty2);
	}
	else
	{
		L298N_Set_Duty(l298n, MOTOR2, l298n->duty2);
		L298N_Forward(l298n, MOTOR2);
	}
}

