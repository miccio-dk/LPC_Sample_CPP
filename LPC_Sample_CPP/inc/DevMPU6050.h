/*
 * DevMPU6050.h
 *
 *  Created on: 09/mar/2015
 *      Author: Miccio
 */

#ifndef DEVMPU6050_H_
#define DEVMPU6050_H_

#include "chip.h"
#include "board.h"

#define DEFAULT_I2C          I2C0
#define I2C_DEFAULT_SPEED    100000
#define I2C_FASTPLUS_BIT     0

#define MPU6050_DEFAULT_ADDRESS     0x68

// relevant registers
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_TEMP_OUT_L       0x42
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_GYRO_XOUT_L      0x44
#define MPU6050_RA_GYRO_YOUT_H      0x45
#define MPU6050_RA_GYRO_YOUT_L      0x46
#define MPU6050_RA_GYRO_ZOUT_H      0x47
#define MPU6050_RA_GYRO_ZOUT_L      0x48
#define MPU6050_RA_WHO_AM_I         0x75

class DevMPU6050 {
public:
	DevMPU6050();
	DevMPU6050(uint8_t _address);
	DevMPU6050(uint8_t _address, RINGBUFF_T *_txring);
	virtual ~DevMPU6050();

	void initialize();
	bool testConnection();

	void getMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);

	void getAcceleration(int16_t* x, int16_t* y, int16_t* z);
	int16_t getAccelerationX();
	int16_t getAccelerationY();
	int16_t getAccelerationZ();

	void getRotation(int16_t* x, int16_t* y, int16_t* z);
	int16_t getRotationX();
	int16_t getRotationY();
	int16_t getRotationZ();

	int16_t getTemperature();

private:
	uint8_t address;
	uint8_t buffer[14];
	char txt_buffer[128];
	bool serialDebug;
	RINGBUFF_T *txring;
	int mode_poll;

	void i2c_set_mode(I2C_ID_T id, int polling);
	void i2c_app_init(I2C_ID_T id, int speed);
	void i2c_init_pinmux(void);
	int16_t readReg2B(uint8_t reg, const char *dbg_txt);
};

#endif /* DEVMPU6050_H_ */
