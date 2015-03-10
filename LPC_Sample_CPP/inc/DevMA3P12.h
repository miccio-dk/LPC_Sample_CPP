/*
 * DevMA3P12.h
 *
 *  Created on: 09/mar/2015
 *      Author: Miccio
 */

#ifndef DEVMA3P12_H_
#define DEVMA3P12_H_


#include "chip.h"
#include "board.h"



class DevMA3P12 {
public:
	DevMA3P12();
	DevMA3P12(bool tmp);
	DevMA3P12(bool tmp, RINGBUFF_T *_txring);
	virtual ~DevMA3P12();

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
	int16_t readReg2B(uint8_t reg);
};

#endif /* DEVMA3P12_H_ */
