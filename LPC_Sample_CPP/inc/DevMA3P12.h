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

#define MA3P12_DEFAULT_GPIO_PININT			3
#define MA3P12_DEFAULT_GPIO_PININT_PORT		0
#define MA3P12_DEFAULT_IOCON_PIN_ID			IOCON_PIO0_3
#define MA3P12_RES							(0 /* TODO convertion ratio*/)

class DevMA3P12 {
public:
	DevMA3P12();
	DevMA3P12(uint8_t port, uint8_t pin);
	DevMA3P12(uint8_t port, uint8_t pin, RINGBUFF_T *_txring);
	virtual ~DevMA3P12();

	void initialize();
	int16_t getPositionRaw();
	int16_t getPositionDeg();
	int16_t getTurnsNum();



private:
	uint8_t port;
	uint8_t pin;
	uint16_t position;
	int16_t turns;
	char txt_buffer[128];
	bool serialDebug;
	RINGBUFF_T *txring;

	void pinint_setup(void);

};

#endif /* DEVMA3P12_H_ */
