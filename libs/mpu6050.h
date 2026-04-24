#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"
#include "math.h"
#include "common.h"




/*
- Fast mode 400kHz
- Duty cycle Tlow/Thigh = 2
- MPU6050v
*/

#define MPU6050_TIME_OUT	1000
#define MPU6050_ADDR_1		(0x68 << 1)
#define MPU6050_ADDR_2		(0x69 << 1)


// MPU6050 REGISTER
#define MPU6050_WHO_AM_I		0x75
#define MPU6050_PWR_MGMT_1		0x6B
#define MPU6050_SMPLRT_DIV		0x19
#define MPU6050_GYRO_CONFIG		0x1B
#define MPU6050_ACCEL_CONFIG	0x1C
#define MPU6050_CONFIG			0x1A
#define MPU6050_ACCEL_BASE		0x3B
#define MPU6050_TEMP_BASE		0x41
#define MPU6050_GYRO_BASE		0x43

#define MPU6050_ACCEL_SIZE		6
#define MPU6050_TEMP_SIZE		2
#define MPU6050_GYRO_SIZE		6
#define MPU6050_ALL_SIZE		(MPU6050_ACCEL_SIZE + MPU6050_TEMP_SIZE + MPU6050_GYRO_SIZE)


#define MPU6050_LSB_SEN_GYRO	((float) 131.0)		// +-250o/s -> LSB/o/s
#define MPU6050_LSB_SEN_ACCEL	((float) 16384.0)	// +-2g -> LSB/g






typedef struct {
    float angle;
    float bias;
    float rate;
    float P[2][2];
} Kalman_t;

typedef enum
{
	MPU6050_ACCEL_CVT = 0x01,
	MPU6050_TEMP_CVT = 0x02,
	MPU6050_GYRO_CVT = 0x04,
	MPU6050_CVT_ALL = 0x07
} MPU6050_cvt_mode_t;
typedef enum {
	X_AX,
	Y_AX,
	Z_AX
} MPU6050_axis_t;
typedef struct
{
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;

	int16_t temp;

	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;

} MPU6050_Data_Raw_t;

typedef struct
{
	float accel_x;
	float accel_y;
	float accel_z;
	float temp;
	float gyro_x;
	float gyro_y;
	float gyro_z;
} MPU6050_Data_t;

typedef struct
{
	float pitch_angle;
	float roll_angle;
	float yaw_angle;
} MPU6050_Angle_t;
typedef struct
{
	I2C_HandleTypeDef* hi2c;
	uint8_t addr;
	Unit_t unit;
	MPU6050_Data_Raw_t data_raw;
	MPU6050_Data_t data;
	MPU6050_Angle_t accel_angle;
	MPU6050_Angle_t gyro_angle;
	float ts;

} MPU6050_t;

void MPU6050_Init(MPU6050_t* mpu, I2C_HandleTypeDef* hi2c, uint8_t addr, Unit_t unit, float ts);
uint8_t MPU6050_Config(MPU6050_t* mpu);
int16_t MPU6050_Cvt2Int16(uint8_t high_byte, uint8_t low_byte);		//	2 bytes to int16
float MPU6050_Cvt_Data_Raw(int16_t raw, float lsb_sensity);		//	Data_raw to data
void MPU6050_Calibrate(MPU6050_t* mpu, uint16_t n_samples);

void MPU6050_Read_Accel(MPU6050_t* mpu);
void MPU6050_Read_Gyro(MPU6050_t* mpu);
void MPU6050_Read_Temp(MPU6050_t* mpu);
void MPU6050_Read_All(MPU6050_t* mpu);
void MPU6050_Cvt_Data_Receiver(MPU6050_t *mpu, MPU6050_cvt_mode_t cvt_mode, float alpha, float thresh_yaw_rate);
void MPU6050_Accel_To_Angle(MPU6050_t* mpu);
float Complementary_Filter(float angle_gyro, float angle_accel, float alpha);
void MPU6050_Calc_Angles_With_Complementary(MPU6050_t *mpu, float alpha);
void MPU6050_Calc_Angles(MPU6050_t* mpu);



#endif
