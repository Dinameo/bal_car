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
float To_RadPS(float input, Unit_t unit)
{

	if(unit == RoundPS) return (input * 2.0f * PI);
	if(unit == RoundPM) return ((input * 2.0f * PI) / 60.0f);
	if(unit == DegPS) return ((input * PI) / 180.0f);
	if(unit == RadPS) return input;
	return 0.0f;
}
float From_RadPS(float input, Unit_t to_unit)
{
	if(to_unit == RoundPS) return (input / (2.0f * PI));
	if(to_unit) return ((input * 60.0f) / (2.0f * PI));
	if(to_unit == DegPS) return ((input * 180.0f) / PI);
	if(to_unit == RadPS) return input;
	return 0.0f;
}
float Change_Unit(float input, Unit_t unit_before, Unit_t unit_after)
{
	if(unit_before == unit_after) return input;
	
	if(unit_before == RAD)
	{
		if(unit_after == DEG) return ((input * 180.0f) / (PI));
	}
	if(unit_before == DEG)
	{
		if(unit_after == RAD) return ((input * PI) / (180.0f));
	}
	float to_radps = To_RadPS(input, unit_before);
	float to_final = From_RadPS(to_radps, unit_after);
	return to_final;
}
