/*
 * common.h
 *
 *  Created on: Feb 2, 2026
 *      Author: nhan
 */

#ifndef __COMMON_H_
#define __COMMON_H_

#include "main.h"

typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
} Pin_t;
typedef enum {
	RAD,
	DEG,
	RadPS,
	RoundPS,
	RoundPM,
	MetrePS,
	DegPS,
} Unit_t;
//typedef struct
//{
//	float min;
//	float max;
//} Range_t;

typedef struct
{
	float vl;
	float vr;
} Vec_cmd_t;

#define PI	((float) 3.14159)

int8_t Sign(float num);
float Dead_band(float duty, float db);
float Clamping(float num, float min, float max);
float Smoothing_Filter(float inp, float inp_prev, float alpha);
float Warp_Angle_Rad(float angle);
float Warp_Angle_Deg(float angle);
float Deadzone(float input, float thresh);
float Change_Unit(float input, Unit_t unit_before, Unit_t unit_after);
#endif /* COMMON_H_ */
