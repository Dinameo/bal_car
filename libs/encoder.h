#ifndef __ENCODER_H
#define __ENCODER_H

#include <stdint.h>
#include "main.h"


typedef struct
{
	TIM_HandleTypeDef* tim;



	uint16_t ppr;
	uint8_t mult;
	int32_t cnt_prev;
	uint16_t center;
	float ts;

//	float speed_prev;
	float speed;
//	float speed_filtered_prev;
	float speed_filtered;

} Encoder_t;

void Encoder_Init(Encoder_t* enc, TIM_HandleTypeDef* htim, uint16_t ppr, uint8_t mult, uint16_t center, float ts);
void Encoder_Start(Encoder_t* enc);
void Encoder_Calc_Speed(Encoder_t* enc, float alpha);
void Encoder_Reset(Encoder_t* enc);


#endif
