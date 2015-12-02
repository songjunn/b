#ifndef	SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <string>
#include <stdlib.h>
#include "types.h"
#include "nedmalloc.h"

// ---------------------------------------------------------------------------------- //
// Memory allocation.
// ---------------------------------------------------------------------------------- //

// Use this to hook allocation errors.
inline void AllocErrorHandler( unsigned long size, const char * szFile = 0, int nLine = 0 )
{
}

inline void* Debug_Alloc( unsigned long size, const char * szFile, int nLine )
{
	void *p = malloc( size );
	if( !p )
	{
		AllocErrorHandler( size, szFile, nLine );
	}
	return p;
}

inline void* Debug_Realloc( void *ptr, unsigned long size, const char * szFile, int nLine )
{
	void *p = realloc( ptr, size );
	if( !p )
	{
		AllocErrorHandler( size, szFile, nLine );
	}
	return p;
}

inline void Debug_Free( void *ptr, const char * szFile, int nLine )
{
	if( ptr )
		free( ptr );
}

inline void* Release_Alloc( unsigned long size )
{
	void *p = malloc( size );
	if( !p )
	{
		AllocErrorHandler( size );
	}
	return p;
}

inline void* Release_Realloc( void *ptr, unsigned long size )
{
	void *p = realloc( ptr, size );
	if( !p )
	{
		AllocErrorHandler( size );
	}
	return p;
}

inline void Release_Free( void *ptr )
{
	if( ptr )
		free( ptr );
}


// -------------------------------------------------------------------------------------------------- //
// overwirte new and delete
// -------------------------------------------------------------------------------------------------- //

inline void * operator new(size_t size) throw (std::bad_alloc)
{
	return nedalloc::nedmalloc(size);
}
inline void operator delete(void* ptr) throw ()
{
	if (ptr) nedalloc::nedfree(ptr);
}

inline void * operator new[](size_t size) throw (std::bad_alloc)
{
	return operator new(size);
}
inline void operator delete[](void* ptr) throw ()
{
	return operator delete(ptr);
}

inline void * operator new(size_t size, const char* file, const size_t line) throw ()
{
	return operator new(size);
}
inline void operator delete(void * ptr, const char* file, const size_t line) throw ()
{
	return operator delete(ptr);
}

inline void * operator new[](size_t size, const char* file, const size_t line) throw ()
{
	return operator new(size, file, line);
}
inline void operator delete[](void * ptr, const char* file, const size_t line) throw ()
{
	return operator delete(ptr, file, line);
}

#ifdef _DEBUG
#define	MemAlloc( size )			Debug_Alloc( size, __FILE__, __LINE__ )			// Allocate some memory.
#define	MemRealloc( ptr, size )		Debug_Realloc( ptr, size, __FILE__, __LINE__ )	// Rellocate some memory. It may or may not return the same
#define	MemFree( ptr )				Debug_Free( ptr, __FILE__, __LINE__ )			// Free some memory.
#else
#define	MemAlloc( size )			Release_Alloc( size )							// Allocate some memory.
#define	MemRealloc( ptr, size )		Release_Realloc( ptr, size )					// Rellocate some memory. It may or may not return the same
#define	MemFree( ptr )				Release_Free( ptr )								// Free some memory.
#endif

#ifdef _DEBUG
#define	NEW							new(__FILE__, __LINE__)
#define SAFE_DELETE(p)				if(p){operator delete ((p), __FILE__, __LINE__);(p)=NULL;}
#define SAFE_DELETE_ARRAY(p)		if(p){operator delete [] ((p), __FILE__, __LINE__);(p)=NULL;}
#else
#define	NEW							new
#define SAFE_DELETE(p)				if(p){operator delete (p);(p)=NULL;}
#define SAFE_DELETE_ARRAY(p)		if(p){operator delete [] (p);(p)=NULL;}
#endif


#endif	//SHARED_MEMORY_H

