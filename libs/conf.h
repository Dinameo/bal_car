/*
 * conf.h
 *
 *  Created on: Feb 1, 2026
 *      Author: nhan
 */

#ifndef __CONF_H_
#define __CONF_H_
#include "main.h"
#include "encoder.h"
#include "mpu6050.h"
#include "hc05.h"
#include "l298n.h"




#define ENCODER_PPR     	11
#define ENCODER_MULT   		4
#define ENC_CENTER 			32768
#define ENC_TS				0.025




// Encoder left and right
#define ENC_ALPHA 0.75
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;

extern Encoder_t enc_l;
extern Encoder_t enc_r;
extern volatile uint8_t read_speed_flag;


// Read IMU
#define MPU_TS 0.01 // 100Hz
#define MPU_UNIT DEG
#define PITCH_COMP_ALP	0.80
#define PITCH_LP_ALP 0.5
#define MPU_N_SAMPLES 500
extern I2C_HandleTypeDef hi2c1;
extern MPU6050_t mpu;
extern volatile uint8_t read_imu_flag;

// HC05
#define HC_NAME	"HC05"
#define HC_PASW	"1234"
#define HC_UART	"9600,1,0"		// Baudrate,stopbit,parity


extern UART_HandleTypeDef huart1;
extern HC05_t hc05;
extern volatile uint8_t flag_connect;
extern HC05_t hc05;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

// L298N
extern L298N_t l298;
extern TIM_HandleTypeDef htim3;

extern float duty_base;
extern float duty_corr;
extern float duty_left;
extern float duty_right;


// PID



/*
------------------High level pid-------------------------------
*/


// PID pitch
#define TS_P 0.01
#define SP_P 0
#define KP_P 7
#define KI_P 0.01
#define KD_P 0.02

// PID yaw
#define THRESH_YR 0.2
#define SP_Y 0
#define KP_Y 7
#define KI_Y 0.01
#define KD_Y 0

#define OUT_MIN_P -VMAX
#define OUT_MAX_P VMAX
#define INT_MIN_P -10
#define INT_MAX_P 10

#define OUT_MIN_Y -VMAX
#define OUT_MAX_Y VMAX
#define INT_MIN_Y -10
#define INT_MAX_Y 10


/*
------------------Low level pid-------------------------------
*/
// PID veclocity left and right wheel
#define TS_VEC 0.025
#define KP_VECL 6.5
#define KI_VECL 0.5
#define KD_VECL 0

#define KP_VECR 6.5
#define KI_VECR 0.5
#define KD_VECR 0

#define OUT_MIN_VEC -100
#define OUT_MAX_VEC 100
#define INT_MIN_VEC -10
#define INT_MAX_VEC 10

#define DEAD_BAND 30


// High level pid
extern PID_t pid_yaw;
extern PID_t pid_pitch;
// Low level pid
extern PID_t pid_vec_left;
extern PID_t pid_vec_right;

extern Vec_cmd_t vec;
extern float prev_error;
//extern Kalman_t kalman;


// MIXER
#define PROP 0.87
#define VMAX 75







#endif /* CONF_H_ */
