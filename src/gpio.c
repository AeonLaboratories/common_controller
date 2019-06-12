///////////////////////////////////////////////////////
// gpio.c
//
// Common Controller

#include <eZ8.h>
#include "gpio.h"


////////////////////////////////////////////////////////
void init_gpio(void)
{

#ifdef PA_OC
	PAOC	= PA_OC;
#endif

#ifdef PA_AF
	PAAF	= PA_AF;
#endif

#ifdef PA_OUT
	PAOUT	= PA_OUT;		// default values
#endif

#ifdef PA_DD
	PADD	= PA_DD;
#endif


//
// NOTE: open drain mode does not work for Port B gpio pins
// (See "Errata for Z8 Encore XP F082A Series UP0069.pdf")
//

#ifdef PB_AF
	PBAF	= PB_AF;
#endif

#ifdef PB_OUT
	PBOUT	= PB_OUT;		// default values
#endif

#ifdef PB_DD
	PBDD	= PB_DD;
#endif


//
// NOTE: open drain mode does not work for Port B gpio pins
// (See "Errata for Z8 Encore XP F082A Series UP0069.pdf")
//

#ifdef PC_AF
	PCAF	= PC_AF;
#endif

#ifdef PC_OUT
	PCOUT	= PC_OUT;		// default values
#endif

#ifdef PC_DD
	PCDD	= PC_DD;
#endif

}
