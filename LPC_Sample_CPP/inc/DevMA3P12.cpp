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
	SysTick_Config(Chip_Clock_GetSystemClockRate()/1E6);
	NVIC_SetPriority(SysTick_IRQn, 0);

	if(serialDebug) {
		const char txt[] = "- MA3P12 initialized\n- SysTick interval enabled - 1us res.\n";
		Chip_UART_SendRB(LPC_USART, txring, txt, sizeof(txt)-1);
	}
}

int16_t DevMA3P12::getPosition() {
	uS_on = 0;
	uS_off = 0;
	position = 0;

	while(true) {
		currState = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);

		// wait until rising edge
		if(!prevState && currState && uS_on==0) {
			uS_on = us_count;
		}

		// wait until falling edge and track t_on
		if(prevState && !currState && uS_on>0) {
			uS_on = us_count - uS_on;
			uS_off = us_count;
		}

		// wait until rising edge and track t_off
		if(!prevState && currState && uS_on>0) {
			uS_off = us_count - uS_off;

			// use datasheet formula for position
			position = ((uS_on*4098) / (uS_on+uS_off)) -1;
			if(position>4096) {
				position = 4095;
			}

			break;
		}

		prevState = currState;

	}

	if(serialDebug) {
		const char txt[] = "- Position:\t%-8d°\n";
		uint16_t len = sprintf(txt_buffer, txt, position);
		Chip_UART_SendRB(LPC_USART, txring, txt_buffer, len);
	}
	return position;
}

void DevMA3P12::pin_setup(void) {
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	Chip_IOCON_PinMuxSet(LPC_IOCON, MA3P12_DEFAULT_IOCON_PIN_ID /* TODO make this parametric! */, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
}

void DevMA3P12::var_init(void) {
	position = 0;
	currState = 0;
	prevState = 0;
	uS_on = 0;
	uS_off = 0;
}
