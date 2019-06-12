///////////////////////////////////////////////////////
// timer.h
//
// common_controller

#pragma once // Include this file only once

#include "..\\..\\common_controller\\include\\mask.h"
#include "c99types.h"
#include "config.h"


#define ENABLE_TIMER_COUNTER_SHORTCUTS		(T0_DIVIDES_SYS && T0_FREQ_IPOT)

#if ENABLE_TIMER_COUNTER_SHORTCUTS && CO_PERIOD_IPOT
	#define CO_INTERVAL			((T0Ticks & (CO_PERIOD-1)) == 0)
#elif CO_PERIOD < 256
	extern uint8_t _COCounter;
	#define CO_INTERVAL			uint8CounterReset(&_COCounter,CO_PERIOD-1)
#else
	extern uint16_t _COCounter;
	#define CO_INTERVAL			uint16CounterReset(&_COCounter,CO_PERIOD-1)
#endif

#if ENABLE_TIMER_COUNTER_SHORTCUTS && CU_PERIOD_IPOT
	#define CU_INTERVAL			((T0Ticks & (CU_PERIOD-1)) == 0)
#elif CU_PERIOD < 256
	extern uint8_t _CUCounter;
	#define CU_INTERVAL		uint8CounterReset(&_CUCounter,CU_PERIOD-1)
#else
	extern uint16_t _CUCounter;
	#define CU_INTERVAL		uint16CounterReset(&_CUCounter,CU_PERIOD-1)
#endif

#if ENABLE_TIMER_COUNTER_SHORTCUTS && SERVICE_PERIOD_IPOT
	#define SERVICE_INTERVAL	((T0Ticks & (SERVICE_PERIOD-1)) == 0)
#elif SERVICE_PERIOD < 256
	extern uint8_t _ServiceCounter;
	#define SERVICE_INTERVAL	uint8CounterReset(&_ServiceCounter,SERVICE_PERIOD-1)
#else
	extern uint16_t _ServiceCounter;
	#define SERVICE_INTERVAL	uint16CounterReset(&_ServiceCounter,SERVICE_PERIOD-1)
#endif

#if ENABLE_TIMER_COUNTER_SHORTCUTS
	#define ONE_SECOND			((T0Ticks & (T0_FREQ-1)) == 0)
#elif T0_FREQ < 256
	extern uint8_t _SecondCounter;
	#define ONE_SECOND		uint8CounterReset(&_SecondCounter,T0_FREQ-1)
#else
	extern uint16_t _SecondCounter;
	#define ONE_SECOND		uint16CounterReset(&_SecondCounter,T0_FREQ-1)
#endif



#if (T0_PRESCALE == 128)
	#define T0_PRESCALE_BITS	0x38
#elif (T0_PRESCALE == 64)
	#define T0_PRESCALE_BITS	0x30
#elif (T0_PRESCALE == 32)
	#define T0_PRESCALE_BITS	0x28
#elif (T0_PRESCALE == 16)
	#define T0_PRESCALE_BITS	0x20
#elif (T0_PRESCALE == 8)
	#define T0_PRESCALE_BITS	0x18
#elif (T0_PRESCALE == 4)
	#define T0_PRESCALE_BITS 	0x10
#elif (T0_PRESCALE == 2)
	#define T0_PRESCALE_BITS	0x08
#elif (T0_PRESCALE == 1)
	#define T0_PRESCALE_BITS	0x00
#endif

// Control register 1:
// If TIMER_INVERT is included, the timer Polarity
// bit is set.
#define T0_CONFIG				(TIMER_MODE_CONTINUOUS | T0_PRESCALE_BITS)
#define T0_CLOCK_FREQ			(SYS_FREQ / T0_PRESCALE)
#define T0_CLOCK_PERIOD			(1.0 * T0_PRESCALE / SYS_FREQ)	// a float

#define T0_PERIOD				(1.0/T0_FREQ)
#define T0_RESET				(T0_CLOCK_FREQ / T0_FREQ)


#if (T1_PRESCALE == 128)
	#define T1_PRESCALE_BITS	0x38
#elif (T1_PRESCALE == 64)
	#define T1_PRESCALE_BITS	0x30
#elif (T1_PRESCALE == 32)
	#define T1_PRESCALE_BITS	0x28
#elif (T1_PRESCALE == 16)
	#define T1_PRESCALE_BITS	0x20
#elif (T1_PRESCALE == 8)
	#define T1_PRESCALE_BITS	0x18
#elif (T1_PRESCALE == 4)
	#define T1_PRESCALE_BITS 	0x10
#elif (T1_PRESCALE == 2)
	#define T1_PRESCALE_BITS	0x08
#elif (T1_PRESCALE == 1)
	#define T1_PRESCALE_BITS	0x00
#endif

// Control register 1:
// If TIMER_INVERT is included, the timer Polarity
// bit is set.
#define T1_CONFIG				(TIMER_MODE_ONESHOT | T1_PRESCALE_BITS)
#define T1_CLOCK_FREQ			(SYS_FREQ / T1_PRESCALE)
#define T1_CLOCK_PERIOD			(1.0 * T1_PRESCALE / SYS_FREQ)


// One-Shot mode timer frequency is its clock frequency
#define T1_FREQ					T1_CLOCK_FREQ
#define T1_PERIOD				(1.0/T1_FREQ)
#define T1_RESET				(T1_CLOCK_FREQ / T1_FREQ)



#define CLOCKS_PER_TICK			T0_RESET						// ideally, an unsigned int
#define T0_CLOCK_PERIOD_US		(1000000.0 * T0_CLOCK_PERIOD)	// a float



///////////////////////////////////////////////////////

#define config_timer0(x)		(T0CTL = (x))
#define start_timer0()			(mask_set(T0CTL, TIMER_ENABLE))
#define stop_timer0()			(mask_clr(T0CTL, TIMER_ENABLE))
#define set_timer0_count(x)		(T0 = (x))
#define clear_timer0()			set_timer0_count(0)
#define set_timer0_end(x)		(T0R = (x))		// AKA "reset"
#define set_timer0_mark(x)		(T0CP = (x))

#define config_timer1(x)		(T1CTL = (x))
#define start_timer1()			(mask_set(T1CTL, TIMER_ENABLE))
#define stop_timer1()			(mask_clr(T1CTL, TIMER_ENABLE))
#define set_timer1_count(x)		(T1 = (x))
#define clear_timer1()			set_timer1_count(0)
#define set_timer1_end(x)		(T1R = (x))		// AKA "reset"
#define set_timer1_mark(x)		(T1CP = (x))



///////////////////////////////////////////////////////
// prototypes
//
void init_timer0(void);
void init_timer1(void);

