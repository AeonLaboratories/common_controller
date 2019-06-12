///////////////////////////////////////////////////////
// adc.c
//
// Common Controller

#include "config.h"
#include "..\\..\\common_controller\\include\\adc.h"

volatile uint8_t AdcdSettling;


////////////////////////////////////////////////////////
void init_adc()
{
	#ifdef ADC_CTL1_INIT
		ADCCTL1 = ADC_CTL1_INIT;
	#endif

	#ifdef ADC_CTL0_INIT
		ADCCTL0 = ADC_CTL0_INIT;
	#endif	
}
