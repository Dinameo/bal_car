/*
 * pidcb.c
 *
 *  Created on: Jan 22, 2026
 *      Author: nhan
 */

#include "pidcb.h"
#include "math.h"

void PID_Init(PID_t *pid, float setpoint, float kp, float ki, float kd, float ts, float out_min, float out_max, float integral_min, float integral_max)
{
	pid->pid_param.kp = kp;
	pid->pid_param.ki = ki;
	pid->pid_param.kd = kd;
	pid->ts = ts;

	pid->out_min = out_min;
	pid->out_max = out_max;

	pid->integral_min = integral_min;
	pid->integral_max = integral_max;
	pid->pid_param.setpoint = setpoint;
	pid->output = 0.0;
	pid->integral = 0.0;
	pid->error = 0;
	pid->derivative = 0;
}

void PID_Reset(PID_t *pid)
{
	pid->integral = 0;
	pid->output = 0;
	pid->derivative = 0;
	pid->error = 0;
}
float Normalize(float input, float min, float max)
{
	if (input > max)
		return max;
	else if (input < min)
		return min;
	else
		return input;
}
void PID_Update(PID_t *pid)
{
	float P = pid->pid_param.kp * pid->error;
	float D = pid->pid_param.kd * pid->derivative;


	float integral_candidate = pid->integral + pid->error * pid->ts;
	integral_candidate = Normalize(integral_candidate, pid->integral_min, pid->integral_max);

	float u_candidate = P + (pid->pid_param.ki * integral_candidate) + D;


	if (!((u_candidate > pid->out_max && pid->error > 0) || (u_candidate < pid->out_min && pid->error < 0)))
	{
		pid->integral = integral_candidate;
	}

	float I = pid->pid_param.ki * pid->integral;
	float u = P + I + D;
	pid->output = Normalize(u, pid->out_min, pid->out_max);
}
