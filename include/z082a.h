///////////////////////////////////////////////////////
// z082a.h
//
// common_controller

#pragma once // Include this file only once

#include <eZ8.h>
#include "..\\..\\common_controller\\include\\mask.h"

///////////////////////////////////////////////////////
#define IRQ_T1						0x40
#define IRQ_T0						0x20
#define IRQ_U0R						0x10
#define IRQ_U0T						0x08
#define IRQ_ADC						0x01

#define IRQ_PA7						0x80
#define IRQ_PA6						0x40
#define IRQ_PA5						0x20
#define IRQ_PA4						0x10
#define IRQ_PA3						0x08
#define IRQ_PA2						0x04
#define IRQ_PA1						0x02
#define IRQ_PA0						0x01

// NOTE: interrupts cannot be configured for Port B pins

#define IRQ_PC3						0x08
#define IRQ_PC2						0x04
#define IRQ_PC1						0x02
#define IRQ_PC0						0x01

#define IRQ0_CLEAR(x)				mask_clr(IRQ0, x)
#define IRQ1_CLEAR(x)				mask_clr(IRQ1, x)
#define IRQ2_CLEAR(x)				mask_clr(IRQ2, x)

#define IRQ0_TRIGGER(x)				mask_set(IRQ0, x)
#define IRQ1_TRIGGER(x)				mask_set(IRQ1, x)
#define IRQ2_TRIGGER(x)				mask_set(IRQ2, x)

#define IRQ0_PRIORITY_HIGH(x)		(mask_set(IRQ0ENH, x), mask_set(IRQ0ENL, x))
#define IRQ0_PRIORITY_NOMINAL(x)	(mask_set(IRQ0ENH, x), mask_clr(IRQ0ENL, x))
#define IRQ0_PRIORITY_LOW(x)		(mask_clr(IRQ0ENH, x), mask_set(IRQ0ENL, x))
#define IRQ0_DISABLE(x)				(mask_clr(IRQ0ENH, x), mask_clr(IRQ0ENL, x))

#define IRQ1_PRIORITY_HIGH(x)		(mask_set(IRQ1ENH, x), mask_set(IRQ1ENL, x))
#define IRQ1_PRIORITY_NOMINAL(x)	(mask_set(IRQ1ENH, x), mask_clr(IRQ1ENL, x))
#define IRQ1_PRIORITY_LOW(x)		(mask_clr(IRQ1ENH, x), mask_set(IRQ1ENL, x))
#define IRQ1_DISABLE(x)				(mask_clr(IRQ1ENH, x), mask_clr(IRQ1ENL, x))

#define IRQ2_PRIORITY_HIGH(x)		(mask_set(IRQ2ENH, x), mask_set(IRQ2ENL, x))
#define IRQ2_PRIORITY_NOMINAL(x)	(mask_set(IRQ2ENH, x), mask_clr(IRQ2ENL, x))
#define IRQ2_PRIORITY_LOW(x)		(mask_clr(IRQ2ENH, x), mask_set(IRQ2ENL, x))
#define IRQ2_DISABLE(x)				(mask_clr(IRQ2ENH, x), mask_clr(IRQ2ENL, x))


// Note: only Port A GPIO pins can be configured to interrupt on rising/falling edge
// Port B pins cannot be used as hardware interrupts.
// Port C pins are hard-configured to interrupt on both rising and falling edges.
#define IRQ_RISE(x)					mask_set(IRQES, x)
#define IRQ_FALL(x)					mask_clr(IRQES, x)

#define IS_EI_IRQ1(x)				((IRQ1ENH & x) || (IRQ1ENL & x))
#define IS_EI_PA7()					IS_EI_IRQ1(IRQ_PA7)
#define IS_EI_PA6()					IS_EI_IRQ1(IRQ_PA6)
#define IS_EI_PA5()					IS_EI_IRQ1(IRQ_PA5)
#define IS_EI_PA4()					IS_EI_IRQ1(IRQ_PA4)
#define IS_EI_PA3()					IS_EI_IRQ1(IRQ_PA3)
#define IS_EI_PA2()					IS_EI_IRQ1(IRQ_PA2)
#define IS_EI_PA1()					IS_EI_IRQ1(IRQ_PA1)
#define IS_EI_PA0()					IS_EI_IRQ1(IRQ_PA0)

#define IS_EI_IRQ2(x)				((IRQ2ENH & x) || (IRQ2ENL & x))
#define IS_EI_PC3()					IS_EI_IRQ2(IRQ_PC3)
#define IS_EI_PC2()					IS_EI_IRQ2(IRQ_PC2)
#define IS_EI_PC1()					IS_EI_IRQ2(IRQ_PC1)
#define IS_EI_PC0()					IS_EI_IRQ2(IRQ_PC0)

#define DI_RX()						IRQ0_DISABLE(IRQ_U0R);
#define DI_TX()						IRQ0_DISABLE(IRQ_U0T);

#define DI_T0()						IRQ0_DISABLE(IRQ_T0);
#define DI_T1()						IRQ0_DISABLE(IRQ_T1);

#define DI_ADC()					IRQ0_DISABLE(IRQ_ADC);

#define DI_PA7()					IRQ1_DISABLE(IRQ_PA7);
#define DI_PA6()					IRQ1_DISABLE(IRQ_PA6);
#define DI_PA5()					IRQ1_DISABLE(IRQ_PA5);
#define DI_PA4()					IRQ1_DISABLE(IRQ_PA4);
#define DI_PA3()					IRQ1_DISABLE(IRQ_PA3);
#define DI_PA2()					IRQ1_DISABLE(IRQ_PA2);
#define DI_PA1()					IRQ1_DISABLE(IRQ_PA1);
#define DI_PA0()					IRQ1_DISABLE(IRQ_PA0);

#define DI_PC3()					IRQ2_DISABLE(IRQ_PC3);
#define DI_PC2()					IRQ2_DISABLE(IRQ_PC2);
#define DI_PC1()					IRQ2_DISABLE(IRQ_PC1);
#define DI_PC0()					IRQ2_DISABLE(IRQ_PC0);

#define IRQ_TRIGGER_TX()			IRQ0_TRIGGER(IRQ_U0T)
#define IRQ_CLEAR_TX()				IRQ0_CLEAR(IRQ_U0T)

#define IRQ_CLEAR_T1()				IRQ0_CLEAR(IRQ_T1)

#define IRQ_CLEAR_PA7()				IRQ1_CLEAR(IRQ_PA7)
#define IRQ_CLEAR_PA6()				IRQ1_CLEAR(IRQ_PA6)
#define IRQ_CLEAR_PA5()				IRQ1_CLEAR(IRQ_PA5)
#define IRQ_CLEAR_PA4()				IRQ1_CLEAR(IRQ_PA4)
#define IRQ_CLEAR_PA3()				IRQ1_CLEAR(IRQ_PA3)
#define IRQ_CLEAR_PA2()				IRQ1_CLEAR(IRQ_PA2)
#define IRQ_CLEAR_PA1()				IRQ1_CLEAR(IRQ_PA1)
#define IRQ_CLEAR_PA0()				IRQ1_CLEAR(IRQ_PA0)

// NOTE: interrupts cannot be configured for Port B pins

#define IRQ_CLEAR_PC3()				IRQ2_CLEAR(IRQ_PC3)
#define IRQ_CLEAR_PC2()				IRQ2_CLEAR(IRQ_PC2)
#define IRQ_CLEAR_PC1()				IRQ2_CLEAR(IRQ_PC1)
#define IRQ_CLEAR_PC0()				IRQ2_CLEAR(IRQ_PC0)


///////////////////////////////////////////////////////
// ADCCTL0 - ADC Control Register 0
// bit 7 CEN 		= 1		// ADC Conversion Enable
// bit 6 REFSELL	= 0		// use 2.0V internal reference (default is 0 on reset)
// bit 5 REFEXT 	= 0		// 1 = provide buffered reference voltage out at Vref pin
// bit 4 CONT		= 1		// 0 = single-shot mode, 1 = continuous
// bit 3 ANAIN3		= 0		// Analog Input select ANAIN[3:0]
// bit 2 ANAIN2		= 0
// bit 1 ANAIN1		= 0
// bit 0 ANAIN0		= 0

#define ADC_CEN						0x80
#define ADC_REFSEL_L				0x40
#define ADC_REFEXT					0x20
#define ADC_CONT					0x10
#define ADC_CHANNEL					0x0F	// ANAIN[3:0]

#define ADC_SELECT(x)				mask_clr(ADCCTL0, ADC_CHANNEL), mask_set(ADCCTL0, x)
#define SELECTED_ADC_CHANNEL		(ADCCTL0 & ADC_CHANNEL)

// Setting ADCCTL0 CEN (bit 7) initiates a single-shot conversion.
#define ADC_START()					mask_set(ADCCTL0, ADC_CEN)
#define ADC_STOP()					mask_clr(ADCCTL0, ADC_CEN)	// was adc_disable()

// ADCCTL0 CEN (bit 7) is cleared by eZ8 when conversion is complete.
#define ADC_BUSY()					(ADCCTL0 & ADC_CEN)

///////////////////////////////////////////////////////
// ADCCTL1 - ADC Control Register 1
// bit 7 REFSELH	= 1		// 2.0V internal reference (default is 1 on reset)
// bit 6 reserved	= 0
// bit 5 reserved	= 0
// bit 4 reserved	= 0
// bit 3 reserved	= 0
// bit 2 BUFMODE2	= 0		// input buffer mode [2:0]
// bit 1 BUFMODE1	= 0
// bit 0 BUFMODE0	= 0

#define ADC_REFSEL_H				0x80
#define ADC_BUFMODE					0x07		// BUFMODE[2:0]

// BUFMODE[2:0] options:
//
// 000 = Single-ended, unbuffered input
// 001 = Single-ended, buffered input with unity gain
// 100 = Differential, unbuffered input
// 101 = Differential, buffered input with unity gain
// (all other values reserved)
#define ADC_BUFMODE_SE_UNB			0x00		// default
#define ADC_BUFMODE_SE_BUF			0x01		// default
#define ADC_BUFMODE_DIFF_UNB		0x04		// default
#define ADC_BUFMODE_DIFF_BUF		0x05		// default


// this is the default on reset:
//#define ADC_CTL1_INIT				(ADC_REFSEL_L)

#define ADC_MODE(x)					mask_clr(ADCCTL1, ADC_BUFMODE), mask_set(ADCCTL1, x)

// Ideally, Adcd would range from [0..4096] but
// the converter has a significant, uncompensated, 
// hardware-dependent offset. A value of 0 is read
// if the analog input voltage is over- or under-range,
// and an overflow bit is set in the ADCD_L register.
// This "overflow" 0 can be confused with a legitimate
// value of 0, and "slightly negative" values are
// possible.

#define ADC_OVERFLOW			0x01
#define ADCD_VALID(x)			(((x) & ADC_OVERFLOW) == 0)
#define ADC_OUTOFRANGE			-8192


///////////////////////////////////////////////////////
// for some reason, these are missing from <uart.h>
#define BAUD_28800				28800UL
#define BAUD_14400				14400UL
#define BAUD_4800				4800UL
#define BAUD_2400				2400UL
#define BAUD_1200				1200UL
#define BAUD_600				600UL
#define BAUD_300				300UL

// UART status register bits
#define U_RDA					0x80			// received data available
#define U_PE					0x40			// parity error
#define U_OE					0x20			// overrun error
#define U_FE					0x10			// framing error
#define U_BRKD					0x08			// break detected
#define U_TDRE					0x04			// transmitter data register empty
#define U_TXE					0x02			// transmitter empty
#define U_CTS					0x01			// 0 = CTS signal is high; 1 = CTS low (CTS is active low)

#define U0_RX_ERROR				(U0STAT0 & (U_PE | U_OE | U_FE | U_BRKD))
#define U0_BYTE_RECEIVED		(U0STAT0 & U_RDA)
#define U0_TX_READY				(U0STAT0 & U_TDRE)
#define U0_TX_COMPLETE			(U0STAT0 & U_TXE)





///////////////////////////////////////////////////////
// Timer control register 1 bits
#define TIMER_ENABLE			0x80
#define TIMER_INVERT			0x40

// The timer prescale divides the system clock
// to create the timer's counter clock. The prescale
// is a power of two from 1 to 128. With the system
// clock driven by the internal precision oscillator (IPO)
// at 5,529,600 Hz, the timer clock's frequency
// ranges from 43,200 Hz (prescale = 128)
// to 5,529,600 Hz (prescale = 1).
// With the system clock driven by a crystal at 
// 18,432,000 Hz, the timer clock's frequency
// ranges from 144,000 Hz to 18,432,000 Hz.
//
// The prescale value is set in TxCTL1[3:5]
//    00xx x000
#define TIMER_MODE_ONESHOT		0x00
#define TIMER_MODE_CONTINUOUS	0x01
#define TIMER_MODE_COUNTER		0x02
#define TIMER_MODE_PWM			0x03
#define TIMER_MODE_CAPTURE		0x04
#define TIMER_MODE_COMPARE		0x05
#define TIMER_MODE_GATED		0x06
#define TIMER_MODE_CAPCOMP		0x07



