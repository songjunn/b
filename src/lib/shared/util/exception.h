/* Copyright (C) Bruce Dawson, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Bruce Dawson, 2001"
 */
#ifndef	SHARED_EXCEPTION_H
#define	SHARED_EXCEPTION_H

// This enables some useful things - like loading default images and
// maximizing window - to make the programs play better with PowerPoint.
// This wouldn't be necessary if PowerPoint used proper shortcuts that
// allowed control over such things...
//#define	GDCDEMO

/*
This header defines a useful set of defines, types, pragmas and
includes that I like to always have available. It should very rarely
be changed, and should be included from everywhere.
*/


// Some generally useful header files that I want always available.
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "types.h"
#include "types.h"


#ifdef _WIN32
// Record detailed information about a crash, to give you a hope
// of being able to diagnose it.
struct _EXCEPTION_POINTERS;
typedef _EXCEPTION_POINTERS* PEXCEPTION_POINTERS;
#else
#ifdef __linux__
typedef struct _EXCEPTION_POINTERS {
	  PEXCEPTION_RECORD ExceptionRecord;
	  PCONTEXT          ContextRecord;
} EXCEPTION_POINTERS;
typedef _EXCEPTION_POINTERS* PEXCEPTION_POINTERS;
#endif
#endif

int HandleException(PEXCEPTION_POINTERS data, const char *Message);

BOOL DumpMini(PEXCEPTION_POINTERS ExceptionInfo);
BOOL DumpFull();

#endif	// SHARED_EXCEPTION_H
