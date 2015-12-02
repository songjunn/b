#include "fasttimer.h"



int64 g_ClockSpeed;	// Clocks/sec
unsigned long g_dwClockSpeed;
double g_ClockSpeedMicrosecondsMultiplier;
double g_ClockSpeedMillisecondsMultiplier;
double g_ClockSpeedSecondsMultiplier;

// Constructor init the clock speed.
static CClockSpeedInit g_ClockSpeedInit;