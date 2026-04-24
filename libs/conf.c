/*
 * conf.c
 *
 *  Created on: Feb 1, 2026
 *      Author: nhan
 */
#include "conf.h"
#include "main.h"
#include "mpu6050.h"
#include "encoder.h"

// Encoder
Encoder_t enc_l;
Encoder_t enc_r;
volatile uint8_t read_speed_flag = 0;


// IMU
MPU6050_t mpu;
volatile uint8_t read_imu_flag = 0;



// HC05
HC05_t hc05;


// L298N
L298N_t l298;
float duty_base;
float duty_corr;

// PID
PID_t pid_yaw;
PID_t pid_pitch;
PID_t pid_vec_left;
PID_t pid_vec_right;
Vec_cmd_t vec;



float prev_error = 0.0;
