/*
 * DevMA3P12.h
 *
 *  Created on: 09/mar/2015
 *      Author: Riccardo Miccini
 */

#ifndef DEVMA3P12_H_
#define DEVMA3P12_H_


#include "chip.h"
#include "board.h"

#define MA3P12_DEFAULT_GPIO_PININT			3
#define MA3P12_DEFAULT_GPIO_PININT_PORT		0
#define MA3P12_DEFAULT_IOCON_PIN_ID			IOCON_PIO0_3

extern uint32_t ms_count;

class DevMA3P12 {
public:
	DevMA3P12();
	DevMA3P12(uint8_t port, uint8_t pin);
	DevMA3P12(uint8_t port, uint8_t pin, RINGBUFF_T *_txring);
	virtual ~DevMA3P12();

	void initialize(void);
	int16_t getPosition(void);
	int16_t getTurnsNum(void);

	void timerHandler(void);

private:
	uint8_t port;
	uint8_t pin;
	uint16_t position;
	char txt_buffer[128];
	bool serialDebug;
	RINGBUFF_T *txring;

	bool currState;
	bool prevState;
	uint32_t uS_on;
	uint32_t uS_off;

	void pin_setup(void);
	void var_init(void);

};

#endif /* DEVMA3P12_H_ */
