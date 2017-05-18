///////////////////////////////////////////////////////
// adc.h
//
// Common Controller

#pragma once // Include this file only once

#include "..\\..\\common_controller\\include\\c99types.h"

///////////////////////////////////////////////////////
extern volatile int Adcd;
extern volatile uint8_t AdcdReads;

#define adc_reset()				(AdcdReads = 0)

///////////////////////////////////////////////////////
// prototypes
//
void init_adc();
