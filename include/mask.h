///////////////////////////////////////////////////////
// mask.h
//
// Common Controller

#pragma once // Include this file only once

// Clear all bits in x that are set in y.
#define	mask_clr( x, y )	( (x) &= ~(y) )

// Set all bits in x that are set in y.
#define mask_set( x, y )	( (x) |= (y) )

// Toggle all bits in x that are set in y.
#define mask_tog( x, y )	( (x) ^= (y) )
