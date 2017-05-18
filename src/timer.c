///////////////////////////////////////////////////////
// timer.c
//
// common_controller

#include "..\\..\\common_controller\\include\\timer.h"

#if ENABLE_TIMER_COUNTER_SHORTCUTS && CO_PERIOD_IPOT
	//
#elif CO_PERIOD < 256
	uint8_t _COCounter;
#else
	uint16_t _COCounter;
#endif

#if ENABLE_TIMER_COUNTER_SHORTCUTS && CU_PERIOD_IPOT
	//
#elif CU_PERIOD < 256
	uint8_t _CUCounter;
#else
	uint16_t _CUCounter;
#endif

#if ENABLE_TIMER_COUNTER_SHORTCUTS && SERVICE_PERIOD_IPOT
	//
#elif SERVICE_PERIOD < 256
	uint8_t _ServiceCounter;
#else
	uint16_t _ServiceCounter;
#endif

#if ENABLE_TIMER_COUNTER_SHORTCUTS
	//
#elif T0_FREQ < 256
	uint8_t _SecondCounter;
#else
	uint16_t _SecondCounter;
#endif



//////////////////////////////////////////////////////////
// channel window timer
void init_timer0(void)
{
	config_timer0(T0_CONFIG);
	clear_timer0();
	set_timer0_end(T0_RESET);
}

//////////////////////////////////////////////////////////
// output pulse duration timer
void init_timer1(void)
{
	config_timer1(T1_CONFIG);
	clear_timer1();
}


