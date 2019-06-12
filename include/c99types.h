///////////////////////////////////////////////////////
// c99types.h
//
// Common Controller

#pragma once

typedef char int8_t;
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;


// This anonymous union allows the compiler to 
// detect and report typedef errors.

static union
{
    char   int8_t_incorrect[sizeof(  int8_t) == 1];
    char  uint8_t_incorrect[sizeof( uint8_t) == 1];
    char  int16_t_incorrect[sizeof( int16_t) == 2];
    char uint16_t_incorrect[sizeof(uint16_t) == 2];
    char  int32_t_incorrect[sizeof( int32_t) == 4];
    char uint32_t_incorrect[sizeof(uint32_t) == 4];
};