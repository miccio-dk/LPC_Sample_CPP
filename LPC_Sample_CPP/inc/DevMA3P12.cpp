/*
 * DevMA3P12.cpp
 *
 *  Created on: 09/mar/2015
 *      Author: Miccio
 */

#include <DevMA3P12.h>

uint8_t port;
uint8_t pin;
uint16_t position;
int16_t turns;

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
	return turns;
}


uint8_t DevMA3P12::getPin() const {
	return pin;
}

uint8_t DevMA3P12::getPort() const {
	return port;
}

void DevMA3P12::setPosition(uint16_t position) {
	this->position = position;
}

void DevMA3P12::setTurns(int16_t turns) {
	this->turns = turns;
}


void DevMA3P12::pinint_setup(void) {
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	Chip_IOCON_PinMuxSet(LPC_IOCON, MA3P12_DEFAULT_IOCON_PIN_ID /* TODO make this parametric! */, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
	SysTick_Config(Chip_Clock_GetSystemClockRate()/1000000);

}





extern "C" {
// wrapper functions to call call member functions from C-linked code
// this avoids the risk of having to use global variables! - see:
// https://isocpp.org/wiki/faq/mixing-c-and-cpp

void call_setPosition(DevMA3P12* p, uint16_t position)
{ return p->setPosition(position); }

void call_setTurns(DevMA3P12* p, uint16_t turns)
{ return p->setTurns(turns); }

uint8_t call_getPin(DevMA3P12* p)
{ return p->getPin(); }

uint8_t call_getPort(DevMA3P12* p)
{ return p->getPort(); }



bool currState = false;
bool prevState = false;

int16_t uS_on = 0;
int16_t uS_off = 0;

void SysTick_Handler(DevMA3P12* p)
{
	currState = Chip_GPIO_GetPinState(LPC_GPIO, call_getPort(p), call_getPin(p));
	if(!prevState && currState) {			// rising edge
		call_setPosition(p, 20); /* TODO formula */
		uS_on = 1;
		uS_off = 0;
	} else if (currState && prevState) {	// high state
		uS_on++;
	} else if (!currState && !prevState) {	// low state
		uS_off++;
	}
	prevState = currState;
}
}

