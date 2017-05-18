///////////////////////////////////////////////////////
// uart.h
//
// common_controller

#pragma once // Include this file only once

#include <defines.h>
#include <uart.h>
#include "..\\..\\common_controller\\include\\c99types.h"
#include "config.h"

// end-of-text character is used to terminate transmisions
#define ETX						0x03u		// ASCII ETX - end of data block character
#define ESC						0x1Bu		// escape code (decimal 27)
#define ACK						0x06u		// ASCII ACK - Acknowledge
#define NAK						0x21u		// ASCII NAK - Negative Acknowledge

#define CRC_INIT				0xFFFFu
#define CRC_POLY				0xDAAEu
#define CRC_GOOD				0x82C0u

// The ez8 processors are big-endian
#define HIGHBYTE(v)  			(*((BYTE *) (&v)))
#define LOWBYTE(v)   			(*((BYTE *) (&v) + 1))

///////////////////////////////////////////////////////
// const globals
extern rom char SPACE[];
extern rom char MSG_END[];
#define printSpace()			printromstr(SPACE)
#define endMessage()			printromstr(MSG_END)


///////////////////////////////////////////////////////
void init_uart0();
void isr_uart0_rx();
#if TXB_SIZE > 0
	void isr_uart0_tx();
#endif

BOOL RxbEmpty();
void process_rx();
char getc();


void getArgs();
BOOL argPresent();
int atoi();

int tryArg(int min, int max, uint16_t error, int default_value, BOOL hundredths);

void printstr(char *);
void printfarstr(far char *);
void printromstr(rom char *);
void printi(int i, uint8_t len, char pad);
void printdec(int i, uint8_t len, char, int8_t decimal_position);
void printfarTenths(far float *units, uint8_t len, char pad);
