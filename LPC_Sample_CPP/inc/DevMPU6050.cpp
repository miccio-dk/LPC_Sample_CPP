/*
 * DevMPU6050.cpp
 *
 *  Created on: 09/mar/2015
 *      Author: Riccardo Miccini
 */

#include <DevMPU6050.h>

#define len(x) (sizeof(x)-1)

Dev_MPU6050::Dev_MPU6050() {
	address = MPU6050_DEFAULT_ADDRESS;
	serialDebug = false;
	i2cDev = DEFAULT_I2C;
	i2c_init_pinmux();
	i2c_app_init(DEFAULT_I2C, I2C_DEFAULT_SPEED);
}

Dev_MPU6050::Dev_MPU6050(uint8_t _address) {
	address = _address;
	serialDebug = false;
	i2cDev = DEFAULT_I2C;
	i2c_init_pinmux();
	i2c_app_init(DEFAULT_I2C, I2C_DEFAULT_SPEED);
}

Dev_MPU6050::Dev_MPU6050(uint8_t _address, RINGBUFF_T *_txring) {
	address = _address;
	serialDebug = true;
	txring = _txring;
	i2cDev = DEFAULT_I2C;
	i2c_init_pinmux();
	i2c_app_init(DEFAULT_I2C, I2C_DEFAULT_SPEED);
}

Dev_MPU6050::~Dev_MPU6050() {
	// TODO Auto-generated destructor stub
}


void Dev_MPU6050::initialize() {
	// TODO set full ranges for accel. and gyro (+-2g and and +-250°/s by default)
	// TODO change clock source
	uint8_t pwr_mgmt_data[] = {MPU6050_RA_PWR_MGMT_1, 0};
	Chip_I2C_MasterSend(i2cDev, MPU6050_DEFAULT_ADDRESS, pwr_mgmt_data, 2);

	if(serialDebug) {
		const char txt[] = "- MPU6050 initialized\n- Sleep mode disabled\n";
		Chip_UART_SendRB(LPC_USART, txring, txt, len(txt));
	}
}

bool Dev_MPU6050::testConnection() {
	// TODO implement
	return true;
}

void Dev_MPU6050::getMotion(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx,
		int16_t* gy, int16_t* gz) {

	uint8_t temp_data[] = {MPU6050_RA_ACCEL_XOUT_H};
	Chip_I2C_MasterSend(i2cDev, MPU6050_DEFAULT_ADDRESS, temp_data, 1);
	Chip_I2C_MasterRead(i2cDev, MPU6050_DEFAULT_ADDRESS, buffer, 14);

	*ax = (((int16_t)buffer[0]) << 8) | buffer[1];
	*ay = (((int16_t)buffer[2]) << 8) | buffer[3];
	*az = (((int16_t)buffer[4]) << 8) | buffer[5];
	*gx = (((int16_t)buffer[8]) << 8) | buffer[9];
	*gy = (((int16_t)buffer[10]) << 8) | buffer[11];
	*gz = (((int16_t)buffer[12]) << 8) | buffer[13];

	if(serialDebug) {
		const char txt[] = "- Acceleration X, Y, Z:\t%-8d, %-8d, %-8d\n"
				"- Rotation X, Y, Z:\t\t%-8d, %-8d, %-8d\n";
		uint16_t len = sprintf(txt_buffer, txt, *ax, *ay, *az, *gx, *gy, *gz);
		Chip_UART_SendRB(LPC_USART, txring, txt_buffer, len);
	}
}

void Dev_MPU6050::getAcceleration(int16_t* x, int16_t* y, int16_t* z) {
	uint8_t temp_data[] = {MPU6050_RA_ACCEL_XOUT_H};
	Chip_I2C_MasterSend(i2cDev, MPU6050_DEFAULT_ADDRESS, temp_data, 1);
	Chip_I2C_MasterRead(i2cDev, MPU6050_DEFAULT_ADDRESS, buffer, 6);

	*x = (((int16_t)buffer[0]) << 8) | buffer[1];
	*y = (((int16_t)buffer[2]) << 8) | buffer[3];
	*z = (((int16_t)buffer[4]) << 8) | buffer[5];

	if(serialDebug) {
		const char txt[] = "- Acceleration X, Y, Z:\t%-8d, %-8d, %-8d\n";
		uint16_t len = sprintf(txt_buffer, txt, *x, *y, *z);
		Chip_UART_SendRB(LPC_USART, txring, txt_buffer, len);
	}
}

int16_t Dev_MPU6050::getAccelerationX() {
	return readReg2B(MPU6050_RA_ACCEL_XOUT_H);
}

int16_t Dev_MPU6050::getAccelerationY() {
	return readReg2B(MPU6050_RA_ACCEL_YOUT_H);
}

int16_t Dev_MPU6050::getAccelerationZ() {
	return readReg2B(MPU6050_RA_ACCEL_ZOUT_H);
}

void Dev_MPU6050::getRotation(int16_t* x, int16_t* y, int16_t* z) {
	uint8_t temp_data[] = {MPU6050_RA_GYRO_XOUT_H};
	Chip_I2C_MasterSend(i2cDev, MPU6050_DEFAULT_ADDRESS, temp_data, 1);
	Chip_I2C_MasterRead(i2cDev, MPU6050_DEFAULT_ADDRESS, buffer, 6);

	*x = (((int16_t)buffer[0]) << 8) | buffer[1];
	*y = (((int16_t)buffer[2]) << 8) | buffer[3];
	*z = (((int16_t)buffer[4]) << 8) | buffer[5];

	if(serialDebug) {
		const char txt[] = "- Rotation X, Y, Z:\t\t%-8d, %-8d, %-8d\n";
		uint16_t len = sprintf(txt_buffer, txt, *x, *y, *z);
		Chip_UART_SendRB(LPC_USART, txring, txt_buffer, len);
	}
}

int16_t Dev_MPU6050::getRotationX() {
	return readReg2B(MPU6050_RA_GYRO_XOUT_H);
}

int16_t Dev_MPU6050::getRotationY() {
	return readReg2B(MPU6050_RA_GYRO_YOUT_H);
}

int16_t Dev_MPU6050::getRotationZ() {
	return readReg2B(MPU6050_RA_GYRO_ZOUT_H);
}

int16_t Dev_MPU6050::getTemperature() {
	return readReg2B(MPU6050_RA_TEMP_OUT_H);
}

/* Set I2C mode to polling/interrupt */
void Dev_MPU6050::i2c_set_mode(I2C_ID_T id, int polling)
{
	if (!polling) {
		mode_poll &= ~(1 << id);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
		NVIC_EnableIRQ(I2C0_IRQn);
	}
	else {
		mode_poll |= 1 << id;
		NVIC_DisableIRQ(I2C0_IRQn);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandlerPolling);
	}
}

/* Initialize the I2C bus */
void Dev_MPU6050::i2c_app_init(I2C_ID_T id, int speed)
{
	/* Initialize I2C */
	Chip_I2C_Init(id);
	Chip_I2C_SetClockRate(id, speed);

	/* Set default mode to interrupt */
	i2c_set_mode(id, 0);
}

void Dev_MPU6050::i2c_init_pinmux(void)
{
	/* I2C */
	Chip_SYSCTL_PeriphReset(RESET_I2C0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
}

int16_t Dev_MPU6050::readReg2B(uint8_t reg) {
	Chip_I2C_MasterSend(i2cDev, MPU6050_DEFAULT_ADDRESS, &reg, 1);
	Chip_I2C_MasterRead(i2cDev, MPU6050_DEFAULT_ADDRESS, buffer, 2);
	return (((int16_t)buffer[0]) << 8) | buffer[1];
}


extern "C" {

void I2C_IRQHandler(void) {
	if (Chip_I2C_IsMasterActive(I2C0)) {
			Chip_I2C_MasterStateHandler(I2C0);
		}
		else {
			Chip_I2C_SlaveStateHandler(I2C0);
		}
}
}
