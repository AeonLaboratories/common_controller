///////////////////////////////////////////////////////
// error.h
//
// Actuator Controller

#ifndef ERROR_H
#define ERROR_H


#define ERROR_NONE			0		// No error
#define ERROR_SERVO			1		// Servo out of range
#define ERROR_CPW 			2		// Servo control pulse width out of range
#define ERROR_TIMEOUT		4		// StopOnT out of range
#define ERROR_BOTH_LIMITS	8		// both limit switches activated?
#define ERROR_LOW_POWER		16		// low Servo Power Supply Voltage
#define ERROR_COMMAND		32		// unrecognized command from RS232
#define ERROR_ILIM			64		// StopOnI out of range
#define ERROR_LIMSW			128		// unrecognized stop limit
#define ERROR_DLINTERVAL	256		// datalogging time invterval out of range
#define ERROR_BUF_OVFL		512		// RS232 input buffer overflow
#define ERROR_CRC			1024	// RS232 CRC error
#define ERROR_ADC			2048	// adc out of range
volatile uint16_t Error = ERROR_NONE;	// error condition

#endif