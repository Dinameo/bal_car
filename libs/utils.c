/*
 * utils.c
 *
 *  Created on: Mar 31, 2026
 *      Author: nhan
 */


#include "common.h"
#include "math.h"
#include "conf.h"

float Dead_band(float duty, float db)
{
	int8_t sign = Sign(duty);
	float duty_abs = fabs(duty);
	if (duty_abs < 0.5f) return 0.0f;
	duty = db + duty_abs*(100.0f-db)/100.0f;

	return (duty * sign);
}
int8_t Sign(float num)
{
	return (num < 0)?(-1):(1);
}
float Clamping(float num, float min, float max)
{
	if(num > max) return max;
	if(num < min) return min;
	return num;
}
float Smoothing_Filter(float inp, float inp_prev, float alpha)
{
	return alpha*inp + (1-alpha)*inp_prev;
}

float Warp_Angle_Deg(float angle)
{
	while(angle > 180) angle -= 360;
	while(angle < -180) angle += 360;
	return angle;
}
float Warp_Angle_Rad(float angle)
{
	while(angle > PI) angle -= 2*PI;
	while(angle < -PI) angle += 2*PI;
	return angle;
}
float Deadzone(float input, float thresh)
{
	return (fabs(input) <= thresh)?(0):(input);
}
