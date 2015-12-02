#ifndef SHARED_ATOMIC_H
#define SHARED_ATOMIC_H

#ifdef _WIN
//#include <stdint.h>
#include <windows.h>
#else
#include <stdint.h>
#include <sys/types.h>
#ifdef FREEBSD 
#include <machine/atomic.h>
#endif
#ifdef __linux__
#include "types.h"
#endif

#endif

#ifdef _WIN

inline void g_lockedIncrement(LONG volatile *linc)
{
	InterlockedIncrement(linc);
}

inline void g_lockedDecrement(LONG volatile *linc)
{
	InterlockedDecrement(linc);
}

inline void g_lockedExchange(volatile LONG* param, LONG value)
{
    InterlockedExchange(param, value);
}

inline void g_lockedExchangeAdd(volatile LONG* param, LONG value)
{
    InterlockedExchange(param, value);
}

inline LONG g_lockedCompareExchange(LONG volatile *Destination, LONG Exchange, LONG Comperand)
{
	return InterlockedCompareExchange(Destination, Exchange, Comperand) == Comperand;
}

#endif

#ifdef FREEBSD

inline void g_lockedIncrement(LONG volatile *linc)
{
	atomic_add_long((ULONG*)linc, 1);
}

inline void g_lockedDecrement(LONG volatile *linc)
{
	atomic_subtract_long((ULONG*)linc, 1);
}

inline void g_lockedExchange(volatile LONG* param,LONG value)
{
    atomic_store_rel_long((ULONG*)param, value);
}

inline LONG g_lockedExchangeAdd( LONG volatile *Addend, LONG Value )
{
	return atomic_fetchadd_long((ULONG*)Addend, (ULONG)Value);
}

inline LONG g_lockedCompareExchange( LONG volatile *Destination, LONG Exchange, LONG Comperand )
{
	/*	需要注意，这里的返回值与winapi不同, winapi返回原始值。
	 * if (*Destination == Comperand) {
	 *      *Destination = Exchange;
	 *      return 1;
	 *  } else
	 *      return 0;
	 */

	return atomic_cmpset_rel_long((ULONG*)Destination, (ULONG)Comperand, (ULONG)Exchange);
}

#endif

#ifdef __linux__ 

#ifdef CONFIG_SMP                         
#define LOCK_PREFIX \
        ".section .smp_locks,\"a\"\n"   \
        _ASM_ALIGN "\n"         \
        _ASM_PTR "661f\n" /* address */ \
        ".previous\n"           \
        "661:\n\tlock; "
                                          
#else /* ! CONFIG_SMP */                  
#define LOCK_PREFIX ""                    
#endif                                    

inline void g_lockedIncrement(LONG volatile *linc)
{
	asm volatile(LOCK_PREFIX "incq %0"
			 : "=m" (*linc)
			 : "m" (*linc));	
}

inline void g_lockedDecrement(LONG volatile *linc)
{
	 asm volatile(LOCK_PREFIX "decq %0"
			  : "=m" (*linc)
			  : "m" (*linc));
}

inline void g_lockedExchange(volatile LONG* param,LONG value)
{
	asm volatile (LOCK_PREFIX "xchg %0,%1" 
				: "=r" (value), "=m" (*param) 
				: "m" (*param) , "0" (value) ); 
}

inline LONG g_lockedExchangeAdd( LONG volatile *Addend, LONG Value )
{
	asm volatile(LOCK_PREFIX "addq %1,%0"
			 : "=m" (*Addend)
			 : "er" (Value), "m" (*Addend));
	return *Addend;
}

/*	需要注意，这里的返回值与winapi不同, winapi返回原始值。
 * if (*Destination == Comperand) {
 *      *Destination = Exchange;
 *      return 1;
 *  } else
 *      return 0;
 */
inline LONG g_lockedCompareExchange(volatile LONG *dst, LONG src, LONG expect)
{
		u_char res;
	
		__asm __volatile(
		LOCK_PREFIX
		"	cmpxchgq %2,%1 ;	"
		"       sete	%0 ;		"
		"1:				"
		"# atomic_cmpset_rel_long"
		: "=a" (res),			/* 0 */
		  "=m" (*dst)			/* 1 */
		: "r" (src),			/* 2 */
		  "a" (expect),			/* 3 */
		  "m" (*dst)			/* 4 */
		: "memory", "cc");
	
		return (res);
}

#endif


#endif	//SHARED_ATOMIC_H
