/*
 * mixer.c
 *
 *  Created on: Mar 19, 2026
 *      Author: nhan
 */
#include "mixer.h"
#include "main.h"
#include "conf.h"
#include "common.h"


void Mixer(Vec_cmd_t* vec, float vp, float vy, float prop, float vmax)
{

	float vp_clamp = Clamping(vp, -vmax*prop, vmax*prop);
	float vy_clamp = Clamping(vy, -vmax*(1-prop), vmax*(1-prop));
	vec->vr = vp_clamp - vy_clamp;
	vec->vl = vp_clamp + vy_clamp;
}
