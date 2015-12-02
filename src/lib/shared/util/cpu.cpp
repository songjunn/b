//=============================================================================
//
// Purpose: determine CPU speed
//
//=============================================================================

#include <string.h>
#if defined(_WIN) && defined(_X64)
#include <intrin.h>
#endif
#include "cpu.h"
#include "fasttimer.h"
#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "types.h"
#endif

static bool cpuid(unsigned long function, unsigned long& out_eax, unsigned long& out_ebx, unsigned long& out_ecx, unsigned long& out_edx)
{
#ifdef _WIN
	bool retval = true;
#ifdef _X64
	
	int CPUInfo[4] = {-1};
	__cpuid(CPUInfo, function);

	out_eax = CPUInfo[0];
	out_ebx = CPUInfo[1];
	out_ecx = CPUInfo[2];
	out_edx = CPUInfo[3];
	
//	assert(0);

#else
	unsigned long local_eax, local_ebx, local_ecx, local_edx;
	_asm pushad;

	__try
	{
        _asm
		{
			xor edx, edx		// Clue the compiler that EDX is about to be used.
            mov eax, function   // set up CPUID to return processor version and features
								//      0 = vendor string, 1 = version info, 2 = cache info
            cpuid				// code bytes = 0fh,  0a2h
            mov local_eax, eax	// features returned in eax
            mov local_ebx, ebx	// features returned in ebx
            mov local_ecx, ecx	// features returned in ecx
            mov local_edx, edx	// features returned in edx
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = false; 
	}

	out_eax = local_eax;
	out_ebx = local_ebx;
	out_ecx = local_ecx;
	out_edx = local_edx;

	_asm popad
#endif
	return retval;
#else	

//	asm("cpuid": "=a" (out_eax), "=b" (out_ebx), "=c" (out_ecx), "=d" (out_edx) : "a" (function));

#ifdef X86AMD64
	uint64_t local_rax, local_rbx, local_rcx, local_rdx;
	asm volatile("push %%rbx      \n\t"
                 "cpuid           \n\t"
                 "mov %%rbx, %1   \n\t"
                 "pop %%rbx       \n\t"
                 : "=a"(local_rax), "=r"(local_rbx), "=c"(local_rcx), "=d"(local_rdx)
                 : "a"(function)
                 : "cc");

	out_eax = local_rax;
	out_ebx = local_rbx;
	out_ecx = local_rcx;
	out_edx = local_rdx;
#else
#ifdef __x86_64
    asm volatile("pushq %%rbx      \n\t"
                 "cpuid            \n\t"
                 "movq %%rbx, %1   \n\t"
                 "popq %%rbx       \n\t"
                 : "=a"(out_eax), "=r"(out_ebx), "=c"(out_ecx), "=d"(out_edx)
                 : "a"(function)
                 : "cc");
#else
    asm volatile("pushl %%ebx      \n\t"
                 "cpuid            \n\t"
                 "movl %%ebx, %1   \n\t"
                 "popl %%ebx       \n\t"
                 : "=a"(out_eax), "=r"(out_ebx), "=c"(out_ecx), "=d"(out_edx)
                 : "a"(function)
                 : "cc");
#endif
#endif


	return true;
#endif

}

bool CheckMMXTechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x800000 ) != 0;
}

bool CheckSSETechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x2000000L ) != 0;
}

bool CheckSSE2Technology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x04000000 ) != 0;
}

bool Check3DNowTechnology(void)
{
    unsigned long eax, unused;
    if( !cpuid(0x80000000,eax,unused,unused,unused) )
		return false;

    if ( eax > 0x80000000L )
    {
     	if( !cpuid(0x80000001,unused,unused,unused,eax) )
			return false;

		return ( eax & 1<<31 ) != 0;
    }
    return false;
}

bool CheckCMOVTechnology()
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & (1<<15) ) != 0;
}

bool CheckFCMOVTechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & (1<<16) ) != 0;
}

bool CheckRDTSCTechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x10 ) != 0;
}

// Return the Processor's vendor identification string, or "Generic_x86" if it doesn't exist on this CPU
const char* GetProcessorVendorId()
{
	unsigned long unused, VendorIDRegisters[3];

	static char VendorID[13];
	
	memset( VendorID, 0, sizeof(VendorID) );
	if( !cpuid(0,unused, VendorIDRegisters[0], VendorIDRegisters[2], VendorIDRegisters[1] ) )
	{
		strcpy( VendorID, "Generic_x86" ); 
	}
	else
	{
		memcpy( VendorID+0, &(VendorIDRegisters[0]), sizeof( VendorIDRegisters[0] ) );
		memcpy( VendorID+4, &(VendorIDRegisters[1]), sizeof( VendorIDRegisters[1] ) );
		memcpy( VendorID+8, &(VendorIDRegisters[2]), sizeof( VendorIDRegisters[2] ) );
	}


	return VendorID;
}

// Returns non-zero if Hyper-Threading Technology is supported on the processors and zero if not.  This does not mean that 
// Hyper-Threading Technology is necessarily enabled.
static bool HTSupported(void)
{
	const unsigned int HT_BIT		 = 0x10000000;  // EDX[28] - Bit 28 set indicates Hyper-Threading Technology is supported in hardware.
	const unsigned int FAMILY_ID     = 0x0f00;      // EAX[11:8] - Bit 11 thru 8 contains family processor id
	const unsigned int EXT_FAMILY_ID = 0x0f00000;	// EAX[23:20] - Bit 23 thru 20 contains extended family  processor id
	const unsigned int PENTIUM4_ID   = 0x0f00;		// Pentium 4 family processor id

	unsigned long unused,
				  reg_eax = 0, 
				  reg_edx = 0,
				  vendor_id[3] = {0, 0, 0};

	// verify cpuid instruction is supported
	if( !cpuid(0,unused, vendor_id[0],vendor_id[2],vendor_id[1]) 
	 || !cpuid(1,reg_eax,unused,unused,reg_edx) )
	 return false;

	//  Check to see if this is a Pentium 4 or later processor
	if (((reg_eax & FAMILY_ID) ==  PENTIUM4_ID) || (reg_eax & EXT_FAMILY_ID))
		if (vendor_id[0] == 'uneG' && vendor_id[1] == 'Ieni' && vendor_id[2] == 'letn')
			return (reg_edx & HT_BIT) != 0;	// Genuine Intel Processor with Hyper-Threading Technology

	return false;  // This is not a genuine Intel processor.
}

// Returns the number of logical processors per physical processors.
static unsigned char LogicalProcessorsPerPackage(void)
{
	const unsigned NUM_LOGICAL_BITS = 0x00FF0000; // EBX[23:16] indicate number of logical processors per package

    unsigned long unused,
		          reg_ebx = 0;
    if (!HTSupported()) 
		return 1; 

	if( !cpuid(1,unused,reg_ebx,unused,unused) )
		return 1;

	return (unsigned char) ((reg_ebx & NUM_LOGICAL_BITS) >> 16);
}

// Measure the processor clock speed by sampling the cycle count, waiting
// for some fraction of a second, then measuring the elapsed number of cycles.
static int64 CalculateClockSpeed()
{
#ifdef _WIN
	LARGE_INTEGER waitTime, startCount, curCount;
	QueryPerformanceCounter(&startCount);
	QueryPerformanceFrequency(&waitTime);
	int scale = 2;		// Take 1/32 of a second for the measurement.
	waitTime.QuadPart >>= scale;

	CCycleCount start, end;
	start.Sample();
	do
	{
		QueryPerformanceCounter(&curCount);
	}
	while(curCount.QuadPart - startCount.QuadPart < waitTime.QuadPart);
	end.Sample();

	return (end.m_Int64 - start.m_Int64) << scale;
#else
	uint64 CalculateCPUFreq(); // from cpu_linux.cpp
	int64 freq =(int64)CalculateCPUFreq();
	/*if ( freq == 0 ) // couldn't calculate clock speed
	{
		Log.Error( "Unable to determine CPU Frequency\n" );
	}*/
	return freq;
#endif
}

const CPUInformation& GetCPUInformation()
{
	static CPUInformation pi;

	// Has the structure already been initialized and filled out?
	if( pi.m_Size == sizeof(pi) )
		return pi;

	// Redundant, but just in case the user somehow messes with the size.
	memset(&pi, 0, sizeof(pi));

	// Fill out the structure, and return it:
	pi.m_Size = sizeof(pi);

	// Grab the processor frequency:
	pi.m_Speed = CalculateClockSpeed();

	
	// Get the logical and physical processor counts:
	pi.m_nLogicalProcessors = LogicalProcessorsPerPackage();

#ifdef _WIN
	SYSTEM_INFO si;
	memset(&si, 0, sizeof(si));
	GetSystemInfo( &si );

	pi.m_nPhysicalProcessors = (unsigned char)(si.dwNumberOfProcessors / pi.m_nLogicalProcessors);
	pi.m_nLogicalProcessors *= pi.m_nPhysicalProcessors;

	// Make sure I always report at least one, when running WinXP with the /ONECPU switch, 
	// it likes to report 0 processors for some reason.
	if( pi.m_nPhysicalProcessors == 0 && pi.m_nLogicalProcessors == 0 )
	{
		pi.m_nPhysicalProcessors = 1;
		pi.m_nLogicalProcessors = 1;
	}
#else
	// TODO: poll /dev/cpuinfo when we have some benefits from multithreading
	pi.m_nPhysicalProcessors = 1;
	pi.m_nLogicalProcessors = 1;
#endif

	// Determine Processor Features:
	pi.m_bRDTSC = CheckRDTSCTechnology();
	pi.m_bCMOV  = CheckCMOVTechnology();
	pi.m_bFCMOV = CheckFCMOVTechnology();
	pi.m_bMMX   = CheckMMXTechnology();
	pi.m_bSSE   = CheckSSETechnology();
	pi.m_bSSE2  = CheckSSE2Technology();
	pi.m_b3DNow = Check3DNowTechnology();

	pi.m_szProcessorID = (char*)GetProcessorVendorId();
	return pi;
}

#ifdef __linux__

#define rdtsc(x) \
	__asm__ __volatile__ ("rdtsc" : "=A" (x))

// Compute 64 bit value / timeval 
static inline uint64 operator / (uint64 v, timeval t)
{
  double tmp = (double)(t.tv_sec) + ((double)(t.tv_usec) / 1000000.0);
  uint64 ret = (uint64) ((double) v / tmp);
  return ret;
//  return uint64 (v / ( ((double)(t.tv_sec)) + ((double)(t.tv_usec) / 1000000.)));
}

// Compute left - right for timeval structures.
static inline timeval operator - (const timeval &left, const timeval &right)
{
  uint64 left_us = (uint64) left.tv_sec * 1000000 + left.tv_usec;
  uint64 right_us = (uint64) right.tv_sec * 1000000 + right.tv_usec;
  uint64 diff_us = left_us - right_us;
  timeval r = { diff_us / 1000000, diff_us % 1000000 };
  return r;
}

// Compute the positive difference between two 64 bit numbers.
static inline uint64 diff(uint64 v1, uint64 v2)
{
  uint64 d = v1 - v2;
  if (d >= 0) return d; else return -d;
}

uint64 CalculateCPUFreq()
{
  // Compute the period. Loop until we get 3 consecutive periods that
  // are the same to within a small error. The error is chosen
  // to be +/- 0.01% on a P-200.
  const uint64 error = 20000;
  const int max_iterations = 300;
  int count;
  uint64 period, period1 = error * 2, period2 = 0,  period3 = 0;

  for (count = 0; count < max_iterations; count++)
    {
      timeval start_time, end_time;
      uint64 start_tsc, end_tsc;
      gettimeofday (&start_time, 0);
      rdtsc (start_tsc);
      //usleep (5000); // sleep for 5 msec
      usleep (100*1000); // sleep for 0.1 sec
      gettimeofday (&end_time, 0);
      rdtsc (end_tsc);
	
      period3 = (end_tsc - start_tsc) / (end_time - start_time);

      if (diff (period1, period2) <= error &&
  	  diff (period2, period3) <= error &&
   	  diff (period1, period3) <= error)
 		break;

      period1 = period2;
      period2 = period3;
    }

   if (count == max_iterations)
    {
	return 0;
    }

   // Set the period to the average period measured.
   period = (period1 + period2 + period3) / 3;

   // Some Pentiums have broken TSCs that increment very
   // slowly or unevenly. 
   if (period < 10000000)
    {
	return 0;
    }

   return period;
}

#endif

