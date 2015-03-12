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
	serialDebug = false;

	var_init();
}

DevMA3P12::DevMA3P12(uint8_t _port, uint8_t _pin) {
	port = _port;
	pin = _pin;
	serialDebug = false;

	var_init();
}

DevMA3P12::DevMA3P12(uint8_t _port, uint8_t _pin, RINGBUFF_T* _txring) {
	port = _port;
	pin = _pin;
	serialDebug = true;
	txring = _txring;

	var_init();
}

DevMA3P12::~DevMA3P12() {}

void DevMA3P12::initialize() {
	pin_setup();
	SysTick_Config(Chip_Clock_GetSystemClockRate()/1E5);
	NVIC_SetPriority(SysTick_IRQn, 0);

	if(serialDebug) {
		const char txt[] = "- MA3P12 initialized\n- SysTick interval enabled - 10us res.\n";
		Chip_UART_SendRB(LPC_USART, txring, txt, sizeof(txt)-1);
	}
}

int16_t DevMA3P12::getPosition() {
	if(serialDebug) {
		const char txt[] = "- Position:\t%-8d°\n";
		uint16_t len = sprintf(txt_buffer, txt, position);
		Chip_UART_SendRB(LPC_USART, txring, txt_buffer, len);
	}
	return position;
}

int16_t DevMA3P12::getTurnsNum() {
	if(serialDebug) {
		const char txt[] = "- N. of turns:\t%-8d\n";
		uint16_t len = sprintf(txt_buffer, txt, turns);
		Chip_UART_SendRB(LPC_USART, txring, txt_buffer, len);
	}
	return turns;
}

/*
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
*/

void DevMA3P12::pin_setup(void) {
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	Chip_IOCON_PinMuxSet(LPC_IOCON, MA3P12_DEFAULT_IOCON_PIN_ID /* TODO make this parametric! */, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
}

void DevMA3P12::var_init(void) {

}

void DevMA3P12::timerHandler() {
	currState = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
	if(!prevState && currState) {			// rising edge
		position /= (uS_on+uS_off);			// formula 2nd half
		uS_on = 1;
		uS_off = 0;
	} else if (prevState && currState ) {	// high state
		uS_on++;
	} else if (prevState && !currState) {	// falling edge
		position = ((uint32_t)uS_on)*360;	// formula 1st half
		uS_off++;
	} else if (!prevState && !currState) {	// low state
		uS_off++;
	}
	prevState = currState;
}



extern "C" {
	// wrapper function to call call member functions from C-linked code
	// this avoids the risk of having to use global variables! - see:
	// https://isocpp.org/wiki/faq/mixing-c-and-cpp

	void call_timerHandler(DevMA3P12* p)
	{ return p->timerHandler(); }

}

