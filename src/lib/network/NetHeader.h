#ifndef SHARED_NETHEADER_H
#define SHARED_NETHEADER_H

#ifdef _WIN
#include <WinSock2.h>
#include <mswsock.h>
#include <mstcpip.h>
#else
#include "types.h"
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#define LINGER linger
#include <netinet/in.h>
#ifdef FREEBSD 
#include <netinet/tcp_timer.h>
#endif
#ifdef __linux__
#include <sys/time.h>
#endif
#include <arpa/inet.h>
#include <errno.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#endif

#endif	//SHARED_NETHEADER_H
