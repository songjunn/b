#ifndef SHARED_CPU_H
#define SHARED_CPU_H

#include "types.h"

//-----------------------------------------------------------------------------
// Processor Information
//-----------------------------------------------------------------------------
struct CPUInformation
{
	int	 m_Size;		// Size of this structure, for forward compatability.

	bool m_bRDTSC : 1,	// Is RDTSC supported?
		 m_bCMOV  : 1,  // Is CMOV supported?
		 m_bFCMOV : 1,  // Is FCMOV supported?
		 m_bSSE	  : 1,	// Is SSE supported?
		 m_bSSE2  : 1,	// Is SSE2 Supported?
		 m_b3DNow : 1,	// Is 3DNow! Supported?
		 m_bMMX   : 1,	// Is MMX supported?
		 m_bHT	  : 1;	// Is HyperThreading supported?

	unsigned char m_nLogicalProcessors,		// Number op logical processors.
		          m_nPhysicalProcessors;	// Number of physical processors

#ifdef _WIN
	int64 m_Speed;						// In cycles per second.
#else
	long long m_Speed;
#endif

	char* m_szProcessorID;				// Processor vendor Identification.
};

const CPUInformation& GetCPUInformation();


#endif	//SHARED_CPU_H
