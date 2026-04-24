#include "encoder.h"
#include "main.h"
#include "common.h"


void Encoder_Init(Encoder_t* enc, TIM_HandleTypeDef* htim, uint16_t ppr, uint8_t mult, uint16_t center, float ts)
{
	enc->tim = htim;
	enc->ppr = ppr;
	enc->mult = mult;
//	enc->cnt_prev = 0;
	enc->speed = 0.0;
	enc->speed_filtered = 0.0;
//	enc->speed_filtered_prev = 0.0;
	enc->center = center;
	enc->ts = ts;
	__HAL_TIM_SET_COUNTER(enc->tim, enc->center);

}


void Encoder_Start(Encoder_t* enc)
{
	HAL_TIM_Encoder_Start(enc->tim, TIM_CHANNEL_ALL);
}
void Encoder_Reset(Encoder_t* enc)
{
	__HAL_TIM_SET_COUNTER(enc->tim, enc->center);
}

// void Encoder_Calc_Speed(Encoder_t* enc, float alpha)
// {
// 	int32_t cnt = (int32_t)__HAL_TIM_GET_COUNTER(enc->tim) - enc->center;
// 	cnt = Deadzone(cnt, 2);
// 	Encoder_Reset(enc);
// 	enc->speed = (float)cnt / (float)(enc->ppr*enc->mult) / enc->ts;
// 	enc->speed_filtered = Smoothing_Filter(enc->speed, enc->speed_filtered, alpha);
// }
void Encoder_Calc_Speed(Encoder_t* enc, float alpha)
{
	int32_t cnt_now = (int32_t)__HAL_TIM_GET_COUNTER(enc->tim);
	int32_t delta = cnt_now - enc->cnt_prev;
	int32_t mod = (int32_t)__HAL_TIM_GET_AUTORELOAD(enc->tim) + 1;
	int32_t half_mod = mod / 2;

	if(delta > half_mod)
		delta -= mod;
	else if(delta < -half_mod)
		delta += mod;

	delta = (int32_t)Deadzone((float)delta, 2.0f);
	enc->cnt_prev = cnt_now;
	enc->speed = (float)delta / (float)(enc->ppr * enc->mult) / enc->ts;
	enc->speed_filtered = Smoothing_Filter(enc->speed, enc->speed_filtered, alpha);
}
//void Encoder_Calc_Speed(Encoder_t* enc)
//{
//    int32_t cnt_now = (int32_t)__HAL_TIM_GET_COUNTER(enc->tim);
//    int16_t delta = (int16_t)(cnt_now - enc->cnt_prev);
//
//    enc->cnt_prev = cnt_now;
//
//    enc->speed = (float)delta / (enc->ppr * enc->mult) / enc->ts;
//}
