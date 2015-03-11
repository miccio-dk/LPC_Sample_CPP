/*
 * DevMA3P12.cpp
 *
 *  Created on: 09/mar/2015
 *      Author: Miccio
 */

#include <DevMA3P12.h>

DevMA3P12::DevMA3P12() {
	port = MA3P12_DEFAULT_GPIO_PININT_PORT;
	pin = MA3P12_DEFAULT_GPIO_PININT;
	position = 0;
	turns = 0;
	serialDebug = false;
	pinint_setup();

	// TODO add init. code
}

DevMA3P12::DevMA3P12(uint8_t _port, uint8_t _pin) {
	port = _port;
	pin = _pin;
	position = 0;
	turns = 0;
	serialDebug = false;
	pinint_setup();

	// TODO add init. code
}

DevMA3P12::DevMA3P12(uint8_t _port, uint8_t _pin, RINGBUFF_T* _txring) {
	port = _port;
	pin = _pin;
	position = 0;
	turns = 0;
	serialDebug = true;
	txring = _txring;
}

DevMA3P12::~DevMA3P12() {
	// TODO Auto-generated destructor stub
}

void DevMA3P12::initialize() {

}

int16_t DevMA3P12::getPositionRaw() {
	return position;
}

int16_t DevMA3P12::getPositionDeg() {
	return (position*MA3P12_RES);
}

int16_t DevMA3P12::getTurnsNum() {
}

void DevMA3P12::pinint_setup(void) {
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIN_ID /* TODO make this parametric! */, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
	Chip_GPIO_SetupPinInt(LPC_GPIO, port, pin, GPIO_INT_BOTH_EDGES);
	Chip_GPIO_EnableInt(LPC_GPIO, port, (1 << pin));


}
