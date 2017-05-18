///////////////////////////////////////////////////////
// main.c
//
// common_controller

#include "gpio.h"	
#include "..\\..\\common_controller\\include\\timer.h"
#include "..\\..\\common_controller\\include\\adc.h"
#include "..\\..\\common_controller\\include\\uart.h"
#include "..\\..\\common_controller\\include\\irq.h"
#include "config.h"

///////////////////////////////////////////////////////
// prototypes
//


// Default user flash optionbits.
FLASH_OPTION1 = 0xFF;
FLASH_OPTION2 = 0xFF;

///////////////////////////////////////////////////////
// globals


///////////////////////////////////////////////////////
void main ()
{
	DI();						// disable interrupts
	init_gpio();				// configure I/O ports
	init_adc();					// configure adc
	init_uart0();				// configure RS232 comms
	init_timer0();				// configure timer 0
	init_timer1();				// configure timer 1
	init_irq();					// irq.c contains the system workhorses
	EI();						// enable interrupts
	
	start_timer0();
	
	for(;;)
	{
		process_rx();			// in uart.c
		do_commands();			// in irq.c
		update_controller();	// in irq.c
	}
}
