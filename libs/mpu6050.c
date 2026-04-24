
#include "mpu6050.h"

#include "common.h"

void MPU6050_Init(MPU6050_t *mpu, I2C_HandleTypeDef *hi2c, uint8_t addr, Unit_t unit, float ts)
{
	mpu->addr = addr;
	mpu->hi2c = hi2c;
	mpu->unit = unit;
	mpu->ts = ts;
}
void MPU6050_Calibrate(MPU6050_t *mpu, uint16_t n_samples)
{
	float ax = 0.0;
	float ay = 0.0;
	float az = 0.0;
	for (uint16_t i = 0; i < n_samples; i++)
	{
		MPU6050_Read_Accel(mpu);
		MPU6050_Cvt_Data_Receiver(mpu, MPU6050_ACCEL_CVT, 1, 0);
		ax += mpu->data.accel_x;
		ay += mpu->data.accel_y;
		az += mpu->data.accel_z;
		HAL_Delay(1);
	}
	ax /= (float)n_samples;
	ay /= (float)n_samples;
	az /= (float)n_samples;
	mpu->data.accel_x = ax;
	mpu->data.accel_y = ay;
	mpu->data.accel_z = az;
	float roll0 = atan2f(ay, az);
	float pitch0 = atan2f(-ax, sqrtf(ay * ay + az * az));

	roll0 = Change_Unit(roll0, RAD, mpu->unit);
	pitch0 = Change_Unit(pitch0, RAD, mpu->unit);

	mpu->gyro_angle.roll_angle = roll0;
	mpu->gyro_angle.pitch_angle = pitch0;
	mpu->gyro_angle.yaw_angle = 0;
}
uint8_t MPU6050_Config(MPU6050_t *mpu)
{

	HAL_Delay(100); // On dinh sau khi cap nguon
	uint8_t check;
	if (HAL_I2C_Mem_Read(mpu->hi2c, mpu->addr, MPU6050_WHO_AM_I, 1, &check, 1, MPU6050_TIME_OUT) == HAL_OK)
	{
		uint8_t data = 0x01; // Thoat sleep (Bit 6 = 0), chọn PLL voi X-axis Gryo làm nguồn clock (CLKSEL = 001)
		HAL_I2C_Mem_Write(mpu->hi2c, mpu->addr, MPU6050_PWR_MGMT_1, 1, &data, 1, MPU6050_TIME_OUT);

		data = 0x07; // Set sample rate = 1kHz/(7+1) = 125Hz
		HAL_I2C_Mem_Write(mpu->hi2c, mpu->addr, MPU6050_SMPLRT_DIV, 1, &data, 1, MPU6050_TIME_OUT);
		HAL_Delay(100); // PLL lock

		data = 0x00; // Full scale range cua con quay hoi chuyen (Gyroscope) +-250o/s
		HAL_I2C_Mem_Write(mpu->hi2c, mpu->addr, MPU6050_GYRO_CONFIG, 1, &data, 1, MPU6050_TIME_OUT);

		data = 0x00; // Full scale range cua gia toc ke (Accelerometer) +-2g
		HAL_I2C_Mem_Write(mpu->hi2c, mpu->addr, MPU6050_ACCEL_CONFIG, 1, &data, 1, MPU6050_TIME_OUT);

		data = 0x01; // Digital Low Pass Filter: Bandwidth 184 (Acc), 188 (Gyro); delay 2ms (Acc), 1ms (Gyro)
		HAL_I2C_Mem_Write(mpu->hi2c, mpu->addr, MPU6050_CONFIG, 1, &data, 1, MPU6050_TIME_OUT);

		mpu->data_raw.accel_x = 0;
		mpu->data_raw.accel_y = 0;
		mpu->data_raw.accel_z = 0;
		mpu->data_raw.gyro_x = 0;
		mpu->data_raw.gyro_y = 0;
		mpu->data_raw.gyro_z = 0;
		mpu->data_raw.temp = 0;

		mpu->data.accel_x = 0;
		mpu->data.accel_y = 0;
		mpu->data.accel_z = 0;
		mpu->data.gyro_x = 0;
		mpu->data.gyro_y = 0;
		mpu->data.gyro_z = 0;
		mpu->data.temp = 0;

		mpu->accel_angle.pitch_angle = 0;
		mpu->accel_angle.roll_angle = 0;
		mpu->accel_angle.yaw_angle = 0;
	}

	return 0;
}
int16_t MPU6050_Cvt2Int16(uint8_t high_byte, uint8_t low_byte) { return (int16_t)((high_byte << 8) | low_byte); }
void MPU6050_Read_Accel(MPU6050_t *mpu)
{
	uint8_t accel_data[MPU6050_ACCEL_SIZE] = {0};
	if (HAL_I2C_Mem_Read(mpu->hi2c, mpu->addr, MPU6050_ACCEL_BASE, 1, accel_data, MPU6050_ACCEL_SIZE, MPU6050_TIME_OUT) == HAL_OK)
	{
		mpu->data_raw.accel_x = MPU6050_Cvt2Int16(accel_data[0], accel_data[1]);
		mpu->data_raw.accel_y = MPU6050_Cvt2Int16(accel_data[2], accel_data[3]);
		mpu->data_raw.accel_z = MPU6050_Cvt2Int16(accel_data[4], accel_data[5]);
	}
}
void MPU6050_Read_Gyro(MPU6050_t *mpu)
{
	uint8_t gyro_data[MPU6050_GYRO_SIZE] = {0};
	if (HAL_I2C_Mem_Read(mpu->hi2c, mpu->addr, MPU6050_GYRO_BASE, 1, gyro_data, MPU6050_GYRO_SIZE, MPU6050_TIME_OUT) == HAL_OK)
	{
		mpu->data_raw.gyro_x = MPU6050_Cvt2Int16(gyro_data[0], gyro_data[1]);
		mpu->data_raw.gyro_y = MPU6050_Cvt2Int16(gyro_data[2], gyro_data[3]);
		mpu->data_raw.gyro_z = MPU6050_Cvt2Int16(gyro_data[4], gyro_data[5]);
	}
}
void MPU6050_Read_Temp(MPU6050_t *mpu)
{
	uint8_t temp_data[MPU6050_TEMP_SIZE] = {0};
	if (HAL_I2C_Mem_Read(mpu->hi2c, mpu->addr, MPU6050_TEMP_BASE, 1, temp_data, MPU6050_TEMP_SIZE, MPU6050_TIME_OUT) == HAL_OK)
	{
		mpu->data_raw.temp = MPU6050_Cvt2Int16(temp_data[0], temp_data[1]);
	}
}
void MPU6050_Read_All(MPU6050_t *mpu)
{
	uint8_t data[MPU6050_ALL_SIZE] = {0};
	if (HAL_I2C_Mem_Read(mpu->hi2c, mpu->addr, MPU6050_ACCEL_BASE, 1, data, MPU6050_ALL_SIZE, MPU6050_TIME_OUT) == HAL_OK)
	{
		mpu->data_raw.accel_x = MPU6050_Cvt2Int16(data[0], data[1]);
		mpu->data_raw.accel_y = MPU6050_Cvt2Int16(data[2], data[3]);
		mpu->data_raw.accel_z = MPU6050_Cvt2Int16(data[4], data[5]);

		mpu->data_raw.temp = MPU6050_Cvt2Int16(data[6], data[7]);

		mpu->data_raw.gyro_x = MPU6050_Cvt2Int16(data[8], data[9]);
		mpu->data_raw.gyro_y = MPU6050_Cvt2Int16(data[10], data[11]);
		mpu->data_raw.gyro_z = MPU6050_Cvt2Int16(data[12], data[13]);
	}
}
float MPU6050_Cvt_Data_Raw(int16_t raw, float lsb_sensity)
{
	return (float)(raw / lsb_sensity);
}
void MPU6050_Cvt_Data_Receiver(MPU6050_t *mpu, MPU6050_cvt_mode_t cvt_mode, float alpha, float thresh_yaw_rate)
{
	if ((cvt_mode & 0x01) == 0x01)
	{
		mpu->data.accel_x = Smoothing_Filter(MPU6050_Cvt_Data_Raw(mpu->data_raw.accel_x, MPU6050_LSB_SEN_ACCEL), mpu->data.accel_x, alpha);
		mpu->data.accel_y = Smoothing_Filter(MPU6050_Cvt_Data_Raw(mpu->data_raw.accel_y, MPU6050_LSB_SEN_ACCEL), mpu->data.accel_y, alpha);
		mpu->data.accel_z = Smoothing_Filter(MPU6050_Cvt_Data_Raw(mpu->data_raw.accel_z, MPU6050_LSB_SEN_ACCEL), mpu->data.accel_z, alpha);
	}
	if ((cvt_mode & 0x02) == 0x02)
	{
		mpu->data.temp = (mpu->data_raw.temp) / 340.0 + 36.53;
	}
	if ((cvt_mode & 0x04) == 0x04)
	{
		float gyrox_raw = MPU6050_Cvt_Data_Raw(mpu->data_raw.gyro_x, MPU6050_LSB_SEN_GYRO);
		float gyroy_raw = MPU6050_Cvt_Data_Raw(mpu->data_raw.gyro_y, MPU6050_LSB_SEN_GYRO);
		float gyroz_raw = MPU6050_Cvt_Data_Raw(mpu->data_raw.gyro_z, MPU6050_LSB_SEN_GYRO);

		gyrox_raw = Change_Unit(gyrox_raw, DEG, mpu->unit);
		gyroy_raw = Change_Unit(gyroy_raw, DEG, mpu->unit);
		gyroz_raw = Change_Unit(gyroz_raw, DEG, mpu->unit);

		mpu->data.gyro_x = Smoothing_Filter(gyrox_raw, mpu->data.gyro_x, alpha);
		mpu->data.gyro_y = Smoothing_Filter(gyroy_raw, mpu->data.gyro_y, alpha);
		mpu->data.gyro_z = Deadzone(Smoothing_Filter(gyroz_raw, mpu->data.gyro_z, alpha), thresh_yaw_rate);
	}
}
float Complementary_Filter(float angle_gyro, float angle_accel, float alpha)
{
	return alpha * angle_gyro + (1 - alpha) * angle_accel;
}
void MPU6050_Calc_Angles(MPU6050_t *mpu)
{
	mpu->gyro_angle.roll_angle += (mpu->data.gyro_x * mpu->ts);
	mpu->gyro_angle.pitch_angle += (mpu->data.gyro_y * mpu->ts);
	mpu->gyro_angle.yaw_angle += (mpu->data.gyro_z * mpu->ts);

	mpu->accel_angle.roll_angle = Change_Unit(atan2f(mpu->data.accel_y, mpu->data.accel_z), RAD, mpu->unit);
	mpu->accel_angle.pitch_angle = Change_Unit(atan2f(-mpu->data.accel_x, sqrtf(mpu->data.accel_y * mpu->data.accel_y + mpu->data.accel_z * mpu->data.accel_z)), RAD, mpu->unit);
}
void MPU6050_Calc_Angles_With_Complementary(MPU6050_t *mpu, float alpha)
{
	float roll_gyro = mpu->gyro_angle.roll_angle + (mpu->data.gyro_x * mpu->ts);
	float pitch_gyro = mpu->gyro_angle.pitch_angle + (mpu->data.gyro_y * mpu->ts);
	float yaw_gyro = mpu->gyro_angle.yaw_angle + (mpu->data.gyro_z * mpu->ts);

	mpu->accel_angle.roll_angle = Change_Unit(atan2f(mpu->data.accel_y, mpu->data.accel_z), RAD, mpu->unit);
	mpu->accel_angle.pitch_angle = Change_Unit(atan2f(-mpu->data.accel_x, sqrtf((mpu->data.accel_y * mpu->data.accel_y) + (mpu->data.accel_z * mpu->data.accel_z))), RAD, mpu->unit);
	mpu->gyro_angle.roll_angle = Complementary_Filter(roll_gyro, mpu->accel_angle.roll_angle, alpha);
	mpu->gyro_angle.pitch_angle = Complementary_Filter(pitch_gyro, mpu->accel_angle.pitch_angle, alpha);

	mpu->gyro_angle.yaw_angle = Warp_Angle_Deg(yaw_gyro);
}