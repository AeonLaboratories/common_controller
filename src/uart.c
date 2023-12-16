///////////////////////////////////////////////////////
// uart.c
//
// common_controller

#include <defines.h>
#include "..\\..\\common_controller\\include\\c99types.h"
#include "..\\..\\common_controller\\include\\uart.h"
#include "error.h"
#include "config.h"

rom char END_LINE[]	= R"\r\n";
rom char MSG_END[]	= R"\r\n\x03";

char RxBuf[RXB_SIZE];					// receive buffer
volatile char RxByte;					// most recent byte received
volatile uint8_t Rxb_in;				// place for next byte from receiver
volatile uint8_t Rxb_head;				// beginning of commands to process
uint8_t Rxb_tail;						// 1 place past the end of commands to process

#define maxCmdChars			2			// neglecting '-', '.', and any digits
char Command[maxCmdChars+1];			// command characters, stripped of numeric content 
far BOOL NargPresent;					// whether a number was provided with the command
far int Narg;							// the provided number, ignoring any decimal point
far uint8_t NargDecimals;				// the number of digits following the decimal point (if any) in the argument
#define MaxDecimals			3			// max NargDecimals

uint16_t RxCrc = CRC_INIT;				// received data CRC
uint16_t TxCrc = CRC_INIT;

#if TXB_SIZE > 0
	far char TxBuf[TXB_SIZE];
	volatile far uint8_t Txb_out;		// pointer to next byte to be transmitted
	volatile far uint8_t Txb_in;		// place for next byte to be queued for transmission
#endif


///////////////////////////////////////////////////////
void init_uart0()
{

	U0BR = (SYS_FREQ + SYS_BAUD * 8uL) / (SYS_BAUD * 16);        
	PAAF |= 0x30;     // enable the UART0 Rx/Tx with the AF register
	U0CTL1 = 0x00;    // clear for normal non-Multiprocessor operation
	U0CTL0 = 0xc0;    // Transmit enable, Receive Enable, No Parity, 1 Stop

	// interrupt service routine to handle received data
   	SET_VECTOR(UART0_RX, isr_uart0_rx);
	EI_RX();
	#if TXB_SIZE > 0
		SET_VECTOR(UART0_TX, isr_uart0_tx);
	#endif
}


///////////////////////////////////////////////////////
// This function is 4x faster than (a % 10) because the
// eZ8 doesn't implement DIV in hardware.
// Returns the correct answer for all unsigned ints < 43691.
// Don't know why the routine fails there, but it is not used
// for values > 32767 so, "to be debugged later."
unsigned int divideByTen(uint16_t a, uint16_t *remainder)
{
	uint16_t q;
	uint16_t r;
	
	q = ((a >> 1) + a) >> 1;
	q = (q >> 4) + q;
	q = ((q >> 8) + q) >> 3;
	
	r = ((q << 2) + q) << 1;
	r = a - r;
	
	if (r >= 10)
	{
		r -= 10;
		q++;
	}
	*remainder = r;
	return q;
}

#define timesTen(x)	(10*(x))
/*
int timesTen(int i)
{
	return ((i << 2) + i) << 1;
}
*/


///////////////////////////////////////////////////////
void update_crc(uint16_t *crc, char c)
{
	uint8_t bit;
	uint16_t lsb;

	// c must be treated as unsigned here
	*crc ^= (0x00FFu & (uint16_t)c);
	for (bit = 0; bit < 8; bit++) {
		lsb = *crc & 0x0001u;
		*crc >>= 1;
		if (lsb) *crc ^= CRC_POLY;
	}
}


///////////////////////////////////////////////////////
// ring buffer
// This implementation requires the pointers to be
// unsigned and the buffer size to be a power of 2.
//
#define RxbAdvance(p) ((p + 1) & (RXB_SIZE-1))
#define RxbRetreat2(p) ((p - 2) & (RXB_SIZE-1))

///////////////////////////////////////////////////////
// whether there's room for another byte from the receiver
#define RxbFull() (RxbAdvance(Rxb_in) == Rxb_head)

///////////////////////////////////////////////////////
// whether there are any accepted commands to process
#define RxbEmpty() (Rxb_head == Rxb_tail)


///////////////////////////////////////////////////////
// serial data received
void interrupt isr_uart0_rx()
{
	BOOL ErrorDetected = (U0STAT0 & 0x70);
	RxByte = U0RXD;		// clear errors or grab byte
	if (!ErrorDetected)
	{
		if (RxbFull())
			mask_set(Error, ERROR_BUF_OVFL);
		else
		{
			RxBuf[Rxb_in] = RxByte;
			Rxb_in = RxbAdvance(Rxb_in);
			mask_clr(Error, ERROR_BUF_OVFL);
		}
	}
}


///////////////////////////////////////////////////////
// Because the CRC may contain ETX values, a CRC 
// error is only known for certain after receiving
// at least 2 more characters following an ETX.
void process_rx()
{
	static uint8_t rxb_read;
	static uint8_t next_head;
	static uint8_t bwuETX;	// bytes with unexpected ETX
	static BOOL bufferOverflow;
	char c;

	if (Error & ERROR_BUF_OVFL)
		bufferOverflow = TRUE;
	else
	{
		while (rxb_read != Rxb_in)
		{
			if (Error & ERROR_BUF_OVFL)
			{
				bufferOverflow = TRUE;
				break;
			}
			if (bwuETX > 0) bwuETX++;
			c = RxBuf[rxb_read];
			if (c == ETX)
			{
				if (RxCrc == CRC_GOOD)
				{
					// accept the sequence
					Rxb_tail = RxbRetreat2(rxb_read);
					RxBuf[Rxb_tail] = '\0';	// terminate it
					Rxb_head = next_head;
				}
				else
					bwuETX = 1;
				
				if (RxCrc == CRC_GOOD || (Error & ERROR_CRC))
				{
					next_head = rxb_read = RxbAdvance(rxb_read);
					bwuETX = 0;
					RxCrc = CRC_INIT;
					mask_clr(Error, ERROR_CRC);
					break;	// defer to doCommands()
				}
			}
			
			update_crc(&RxCrc, c);
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
				RxBuf[rxb_read] = '\0';	// convert whitespace to null
			rxb_read = RxbAdvance(rxb_read);
			
			if (bwuETX > 2) mask_set(Error, ERROR_CRC);
		}
	}
	if (bufferOverflow)	// incomplete message
	{
		mask_set(Error, ERROR_CRC);
		Rxb_head = Rxb_tail = next_head = rxb_read = Rxb_in;
		bufferOverflow = FALSE;
	}
}

///////////////////////////////////////////////////////
// returns the next character in the receive buffer 
char getc()
{
	char c = RxBuf[Rxb_head];
	Rxb_head = RxbAdvance(Rxb_head);
	return c;
}


///////////////////////////////////////////////////////
// optional character utilities
#ifdef _PEEK
char peek() { return RxBuf[Rxb_head]; }
#endif
#ifdef _ISDIGIT
BOOL isdigit(char c) { return (c >= '0') && (c <= '9');}
#endif
#ifdef _ISLOWER
BOOL islower(char c) { return (c >= 'a') && (c <= 'z');}
#endif
#ifdef _ISUPPER
BOOL isupper(char c) { return (c >= 'A') && (c <= 'Z');}
#endif
#ifdef _ISLETTER
BOOL isletter(char c) { return islower(c) || isupper(c);}
#endif


///////////////////////////////////////////////////////
void GetInput()
{
	char *p = Command;
	char c;	
	uint8_t i = 0; 		// command character count
	int n = 0, d = 0;	// for numeric argument interpretation
	BOOL neg = FALSE, dotFound = FALSE, digitFound = FALSE;

	NargPresent = FALSE;
	
	while (!RxbEmpty())
	{
		c = getc();
		if (i == 0)
		{
			// first char is always considered a "command"
			*p++ = c; i++;
		}
		else if (!NargPresent && c == '-')
		{
			neg = TRUE;
		}
		else if (c == '.')
		{
			dotFound = TRUE;
		}
		else if (isdigit(c))
		{
			NargPresent = TRUE;
			if (d < MaxDecimals)
			{
				if (dotFound) ++d;
				n = timesTen(n) + (c - '0');
			}
		}
		else if (NargPresent)
		{
			break;
		}
		else if (i <= maxCmdChars)	// further command characters are silently ignored
		{
			*p++ = c; i++;
		}
	}	
	*p = '\0';

	Narg = neg ? -n : n;
	NargDecimals = d;
}


///////////////////////////////////////////////////////
// parameter validation helper
int TryInput(int min, int max, uint16_t error, int default_value, uint8_t decimals)
{
	int n = Narg, d;
	uint16_t r;					// divideByTen remainder; not used
	BOOL neg;
	
	// drop extraneous decimal digits
	d = NargDecimals - decimals;
	if (d > 0)
	{
		neg = Narg < 0;
		if (neg) n = -n;
		for (; d > 0; d--)
			n = divideByTen(n, &r);
		if (neg) n = -n;
	}
		
	// Account for any omitted decimals...
	// For example,
	//	If the argument were "100.0" and hundredths were expected
	// 		narg would be 1000, since it ignores the decimal
	//		nargDecimals would be 1, since one digit was counted after the decimal
	//		decimals would be 2, because hundredths are expected
	//	Thus, d will start at 2-1 = 1, so n will be 1000 * 10 = 10000 hundredths,
	//	which equals the 100.0 provided.
	for (d = decimals - NargDecimals; d > 0; d--)
		n *= 10;
	
	if (!NargPresent || n < min || n > max)
	{
		mask_set(Error, error);
		return default_value;	// fail
	}
	mask_clr(Error, error);
	return n;	// success
}


///////////////////////////////////////////////////////
#if TXB_SIZE > 0
	#define TxbAdvance(p) ((p + 1) & (TXB_SIZE-1))
	#define TxbFull() (TxbAdvance(Txb_in) == Txb_out)
	#define TxbEmpty() (Txb_out == Txb_in)
#endif


///////////////////////////////////////////////////////
// Called when transmit data register (TDR) becomes empty
// or if TDR is empty when EI_TX() occurs.
// At 115,200 baud, 8N1, a byte takes about
//		10/15200 * 5529600 = 480
// system clock cyles to transmit a byte.
#if TXB_SIZE > 0
	void interrupt isr_uart0_tx()
	{
		U0TXD = TxBuf[Txb_out];
		Txb_out = TxbAdvance(Txb_out);
		if (TxbEmpty())
			DI_TX();		// so the next EI_TX() will immediately generate an interrupt
	}
#endif


///////////////////////////////////////////////////////
void send0(char c)
{
	#if TXB_SIZE > 0
		while (TxbFull());		// busywait for buffer space
		TxBuf[Txb_in] = c;
		Txb_in = TxbAdvance(Txb_in);
		EI_TX();
	#else
		while (!U0_TX_READY);	// busywait for transmit data register to be empty
		U0TXD = c;
	#endif
}


///////////////////////////////////////////////////////
void putc(char c)
{
	if (c == ETX)
	{
		TxCrc = ~TxCrc;
		//transmit the CRC code, low byte first
		send0(LOWBYTE(TxCrc));
		send0(HIGHBYTE(TxCrc));
		send0(c);
		TxCrc = CRC_INIT; 		// reset the CRC code
	} else {
		update_crc(&TxCrc, c);
		send0(c);
	}
}


///////////////////////////////////////////////////////
void printstr(char *s) { for (; *s; ++s) putc(*s); }

///////////////////////////////////////////////////////
// optional versions
#ifdef _PRINTFARSTR
void printfarstr(far char *s) { for (; *s; ++s) putc(*s); }
#endif

#ifdef _PRINTROMSTR
void printromstr(rom char *s) { for (; *s; ++s) putc(*s); }
#endif


// Output an int i, right justified in a field of length n
// with a decimal point inserted after position decpos,
// counting from the right. If decpos = 0, or is too big
// to fall in the field width, no decimal point will be printed.
void printdec(int i, uint8_t n, char pad, int8_t decpos)
{
	char buf[8];
	char *s = buf + (8-1);
	char *w = s - n;			// start of desired-width string
	BOOL neg = (i < 0);
	uint16_t r;					// divideByTen remainder
	
	if (neg) i = -i;
	*s = '\0';
	do
	{
		i = divideByTen(i, &r);
		*--s = '0' + r;
		if (--decpos == 0) *--s = '.';
	} while (i > 0 || decpos > 0);
	if (*s == '.') *--s = '0';
	if (neg) *--s = '-';
	while (s > w) *--s = pad;
	printstr(s);
}


////////////////////////////////////////////////////////
// output an int i, right justified in a field of length n
void printi(int i, uint8_t n, char pad)
{
	printdec(i, n, pad, 0);
}


///////////////////////////////////////////////////////
//void printfarTenths(far float *units, uint8_t n, char pad)
//{
//	int tenths = *units * 10.0;
//	printdec(tenths, n, pad, 1);
//}

///////////////////////////////////////////////////////
//void printTenths(float *units, uint8_t n, char pad)
//{
//	int tenths = *units * 10.0;
//	printdec(tenths, n, pad, 1);
//}
