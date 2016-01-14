// MUD Programming
// Ron Penton
// (C)2003
// ThreadLibFunctions.h - Contains all of the functions for use within
// the threading library

#ifndef THREADLIBFUNCTIONS_H
#define THREADLIBFUNCTIONS_H

// ============================================================================
//  Include Files for the threading libraries
// ============================================================================
#ifdef _WIN                // windows95 and above
    #include <windows.h>
    #include <map>
#else                       // linux
    #include <pthread.h>
    #include <unistd.h>
#endif

#include "ThreadException.h"
#include "threadpool.h"

#define THREAD 16
#define QUEUE  128

static threadpool_t *g_threadpool = NULL;

namespace ThreadLib
{

    // ========================================================================
    // define the standard thread function format, which takes a void* as its
    // parameter, and returns nothing
    // ========================================================================
    typedef void (*ThreadFunc)(void*);

    // ========================================================================
    //  Define the standard ThreadID datatype, depending on the system
    // ========================================================================
    #ifdef _WIN                // windows95 and above
        typedef DWORD ThreadID;
        extern std::map< DWORD, HANDLE > g_handlemap;
    #else                       // linux
        typedef pthread_t ThreadID;
    #endif


    // ========================================================================
    // Description: This is a "dummy" class that will be used to
    //              transparently translate function pointers to whatever
    //              system the user is currently compiling on. It is meant to
    //              be passed into the DummyRun function.
    // ========================================================================
    class DummyData
    {
    public:
        ThreadFunc m_func;
        void* m_data;
    };

    // ========================================================================
    // Description: This is a "dummy" thread function that will be used to
    //              transparently translate function pointers to whatever
    //              system the user is currently compiling on.
    // ========================================================================
    #ifdef _WIN
        DWORD WINAPI DummyRun( void* p_data );
    #else
        void* DummyRun( void* p_data );
    #endif


    // ========================================================================
    // Description: Creates a thread and returns its ID.
    // ========================================================================
    inline ThreadID Create( ThreadFunc p_func, void* p_param )
    {
        ThreadID t;
        // create a new dummy data block
        DummyData* data = new DummyData;
        data->m_func = p_func;
        data->m_data = p_param;

        #ifdef _WIN    // create a Win32 thread
            HANDLE h;
            h = CreateThread( NULL, 0, DummyRun, data, 0, &t );
            if( h != 0 )
            {
                // insert the handle into the handlemap
                g_handlemap[t] = h;
            }
        #else   // create a linux thread
            pthread_create( &t, NULL, DummyRun, data );
            //pthread_detach(t); set noblock ?
        #endif

        if( t == 0 )
        {
            // delete the data first
            delete data;

            // throw an error
            throw Exception( CreationFailure );
        }
        return t;
    }

	// ========================================================================
	// Description: Creates a thread with thread pool.
	// ========================================================================
	inline void CreateByPool(ThreadFunc p_func, void* p_param)
	{
			Create(p_func, p_param);

			// 使用线程池似乎会出现内存错误
			/*if (g_threadpool == NULL && (g_threadpool = threadpool_create(THREAD, QUEUE, 0)) == NULL) {
				throw Exception(CreationFailure);
				return;
			}

			if (threadpool_add(g_threadpool, p_func, p_param, 0) != 0) {
				throw Exception(CreationFailure);
				return;
			}*/
	}

	// ========================================================================
	// Description: Destroy a thread pool.
	// ========================================================================
	inline void DestroyPool()
	{
		if (g_threadpool) {
			threadpool_destroy(g_threadpool, 0/*threadpool_graceful*/);
		}
	}

    // ========================================================================
    // Description: Get the ID of the current thread
    // ========================================================================
    inline ThreadID GetID()
    {
        #ifdef _WIN
            return GetCurrentThreadId();
        #else
            return pthread_self();
        #endif
    }


    // ========================================================================
    // Description: This waits for a thread to finish executing before 
    //              returning.
    // ========================================================================
    inline void WaitForFinish( ThreadID p_thread )
    {
        #ifdef _WIN
            // look up the handle and wait for the thread to finish
            WaitForSingleObject( g_handlemap[p_thread], INFINITE );

            // close the handle of the thread
            CloseHandle( g_handlemap[p_thread] );

            // remove the handle from the map
            g_handlemap.erase( p_thread );
        #else
            // "join" the thread. This essentially transfers control over to
            // the thread and waits for it to finish.
            pthread_join( p_thread, NULL );
        #endif
    }


    // ========================================================================
    // Description: This forcefully terminates a thread. Don't do this unless
    //              it is absolutely neccessary.
    // ========================================================================
    inline void Kill( ThreadID& p_thread )
    {
        #ifdef _WIN
            // terminate the thread
            TerminateThread( g_handlemap[p_thread], 0 );

            // close the handle of the thread
            CloseHandle( g_handlemap[p_thread] );

            // remove the handle from the map
            g_handlemap.erase( p_thread );
        #else
            // cancel the thread.
            pthread_cancel( p_thread );
        #endif
    }


    // ========================================================================
    // Description: This yields the thread to the operating system, and gives
    //              up its current timeslice.
    // ========================================================================
    inline void YieldThread( int p_milliseconds = 1 )
    {
        #ifdef _WIN
            Sleep( p_milliseconds );
        #else
            usleep( p_milliseconds * 1000 );
        #endif
    }

}   // end namespace ThreadLib


#endif
