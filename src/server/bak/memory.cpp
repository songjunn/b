#include <stdlib.h>
#include "memory.h"
#ifdef _NEDMALLOC_
#include "nedmalloc.h"
#include "nedmalloc.c"
#endif

// -------------------------------------------------------------------------------------------------- //
// Memory stuff.
// -------------------------------------------------------------------------------------------------- //

void AllocErrorHandler( unsigned long size, const char * szFile /*= NULL*/, int nLine /*= 0*/ )
{
}

void* Debug_Alloc( unsigned long size, const char * szFile, int nLine )
{
	void *p = malloc( size );
	if( !p )
	{
		AllocErrorHandler( size, szFile, nLine );
	}
	return p;
}

void* Debug_Realloc( void *ptr, unsigned long size, const char * szFile, int nLine )
{
	void *p = realloc( ptr, size );
	if( !p )
	{
		AllocErrorHandler( size, szFile, nLine );
	}
	return p;
}

void Debug_Free( void *ptr, const char * szFile, int nLine )
{
	if( ptr )
		free( ptr );
}

void* Release_Alloc( unsigned long size )
{
	void *p = malloc( size );
	if( !p )
	{
		AllocErrorHandler( size );
	}
	return p;
}

void* Release_Realloc( void *ptr, unsigned long size )
{
	void *p = realloc( ptr, size );
	if( !p )
	{
		AllocErrorHandler( size );
	}
	return p;
}

void Release_Free( void *ptr )
{
	if( ptr )
		free( ptr );
}

// -------------------------------------------------------------------------------------------------- //
// overwirte new and delete
// -------------------------------------------------------------------------------------------------- //
#ifdef _RELOADNEW_
void * operator new(size_t size) throw (std::bad_alloc)
{
#ifdef _NEDMALLOC_
	return nedalloc::nedmalloc(size);
#endif
}

void operator delete(void* ptr) throw ()
{
	if( !ptr )
		return;

#ifdef _NEDMALLOC_
	nedalloc::nedfree(ptr);
#endif
}
#endif

void * operator new[](size_t size) throw (std::bad_alloc)
{
	return operator new(size);
}

void operator delete[](void* ptr) throw ()
{
	operator delete(ptr);
}

void * operator new(size_t size, const char* file, const size_t line) throw ()
{
	return operator new(size);
}
void operator delete(void * ptr, const char* file, const size_t line) throw ()
{
	return operator delete(ptr);
}

void * operator new[](size_t size, const char* file, const size_t line) throw ()
{
	return operator new(size, file, line);
}

void operator delete[](void * ptr, const char* file, const size_t line) throw ()
{
	return operator delete(ptr, file, line);
}
