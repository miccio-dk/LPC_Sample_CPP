/*
 * DevMA3P12.cpp
 *
 *  Created on: 09/mar/2015
 *      Author: Riccardo Miccini
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
	ms_count = 0;
	SysTick_Config(SystemCoreClock/1000);
	NVIC_SetPriority(SysTick_IRQn, 0);

	if(serialDebug) {
		const char txt[] = "- MA3P12 initialized\n- SysTick interval enabled - 1ms res.\n";
		Chip_UART_SendRB(LPC_USART, txring, txt, sizeof(txt)-1);
	}
}

int16_t DevMA3P12::getPosition() {
	uS_on = 0;
	uS_off = 0;
	position = 0;
	prevState = true;

	while(true) {
		currState = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);

		// wait until rising edge
		if(!prevState && currState && uS_on==0) {
			// obtain us-level from systick - see http://micromouseusa.com/?p=296
			uS_on = ms_count*1000 + (1000 - SysTick->VAL/48);
		}

		// wait until falling edge and track t_on
		else if(prevState && !currState && uS_on>0) {
			uS_on = ms_count*1000 + (1000 - SysTick->VAL/48) - uS_on;
			uS_off = ms_count*1000 + (1000 - SysTick->VAL/48);
		}

		// wait until rising edge and track t_off
		else if(!prevState && currState && uS_on>0) {
			uS_off = ms_count*1000 + (1000 - SysTick->VAL/48) - uS_off;

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
		const char txt[] = "- Position (Ton, Toff):\t%8d\t%8lu\t%8lu\n";
		uint16_t len = sprintf(txt_buffer, txt, position, uS_on, uS_off);
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
