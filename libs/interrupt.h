/*
 * interrupt.h
 *
 *  Created on: Feb 1, 2026
 *      Author: nhan
 */

#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "main.h"
#include "mpu6050.h"
#include "l298n.h"


extern volatile uint8_t read_imu_flag;
extern volatile uint8_t read_speed_flag;
extern MPU6050_t mpu6050;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);


#endif /* INTERRUPT_H_ */
