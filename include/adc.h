///////////////////////////////////////////////////////
// adc.h
//
// Common Controller

#pragma once // Include this file only once

#include "..\\..\\common_controller\\include\\c99types.h"

///////////////////////////////////////////////////////
extern volatile uint8_t AdcdSettling;

#define adc_reset()				(AdcdSettling = ADC_SETTLING_TIME)

///////////////////////////////////////////////////////
// prototypes
//
void init_adc();
