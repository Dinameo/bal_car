/*
 * pidcb.h
 *
 *  Created on: Jan 22, 2026
 *      Author: nhan
 */

#ifndef __PIDCB_H
#define __PIDCB_H

typedef enum
{
	PID_CB,
	PID_TS
} PID_index_t;

typedef struct {
	float kp;
	float ki;
	float kd;
	float setpoint;
} PID_Param_t;

typedef struct {
	PID_Param_t pid_param;

	float ts;
	float integral;
	float error;
	float derivative;

	float out_min;
	float out_max;
	float integral_min;
	float integral_max;
	float output;
} PID_t;



void PID_Init(PID_t* pid, float setpoint, float kp, float ki, float kd, float ts, float out_min, float out_max, float integral_min, float integral_max);
void PID_Reset(PID_t* pid);
float Normalize(float input, float min, float max);
void PID_Update(PID_t* pid);

#endif /* PIDCB_H_ */
