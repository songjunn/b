/*
 * =====================================================================================
 *
 *       Filename:  linux_time.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月28日 18时26分44秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *        Company:  ciwen.com
 *
 * =====================================================================================
 */

#include <time.h>   
unsigned int timeGetTime()  
{  
        unsigned int uptime = 0;  
        struct timespec on;  
        if(clock_gettime(CLOCK_MONOTONIC, &on) == 0)  
                 uptime = on.tv_sec*1000 + on.tv_nsec/1000000;  
        return uptime;  
}  
