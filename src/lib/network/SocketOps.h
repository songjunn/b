#ifndef SHARED_SOCKETOPS_H
#define SHARED_SOCKETOPS_H

#include "NetHeader.h"

namespace SocketOps
{
	// init winsock service
	bool NetStartup();

	// close winsock service
	bool NetClose();

	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor();

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd);

	// Enable blocking send/recv calls.
	bool Blocking(SOCKET fd);

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd);

	// Enables nagle buffering algorithm
	bool EnableBuffering(SOCKET fd);

	// Set internal buffer size to socket.
	bool SetRecvBufferSize(SOCKET fd, int size);

	// Set internal buffer size to socket.
	bool SetSendBufferSize(SOCKET fd, int size);

	// Closes socket completely.
	bool CloseSocket(SOCKET fd);

	// Sets SO_REUSEADDR
	bool ReuseAddr(SOCKET fd);

	// Set graceful close
	bool SetGracefulClose(SOCKET fd, USHORT time=0);

	// Inherit listen socket attribute
	bool SetInherit(SOCKET fd, SOCKET listenfd);

	// Set keep alive
	bool SetKeepAlive(SOCKET fd, int idle, int intv, int times);

	// Get last error
	int GetLastError();
};

#endif

