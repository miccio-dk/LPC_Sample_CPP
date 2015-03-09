/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "chip.h"
#include "board.h"

#include "DevMPU6050.h"
#include "string.h"
#include <stdlib.h>


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Transmit and receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 128	/* Send */
#define UART_RRB_SIZE 32	/* Receive */

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];


/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void Init_UART_PinMux(void)
{
	// check http://www.nxp.com/documents/user_manual/UM10398.pdf
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));/* RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));/* TXD */
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/

// C++ handlers require C linkage - see:
// http://www.lpcware.com/content/faq/lpcxpresso/startup-code-interrupt-handlers
#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

void UART_IRQHandler(void) {
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif


int main(void) {

#if defined (__USE_LPCOPEN)
#if !defined(NO_BOARD_LIB)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    Init_UART_PinMux();

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, 115200);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);

	/* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);

	uint8_t key = 0;
	int16_t accX, accY, accZ, temp, girX, girY, girZ;
	Dev_MPU6050 device(0x68, &txring);
	device.initialize();

	while (key != 'q') {
		Chip_UART_ReadRB(LPC_USART, &rxring, &key, 1);
		if (key == 'i') {
			device.getMotion(&accX, &accY, &accZ, &girX, &girY, &girZ);
			temp = device.getTemperature();
		}
	}

	/* DeInitialize UART0 peripheral */
	NVIC_DisableIRQ(UART0_IRQn);
	Chip_UART_DeInit(LPC_USART);


    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}
