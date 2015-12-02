#include "NetHeader.h"

#ifdef _WIN
namespace SocketOps
{
	// init winsock service
	bool NetStartup()
	{
		WSAData arg;
		return (::WSAStartup(MAKEWORD(2,2), &arg) == 0);
	}

	// close winsock service
	bool NetClose()
	{
		return (::WSACleanup() == 0);
	}

	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor()
	{
		// create a socket for use with overlapped i/o.
		return ::WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
	}

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd)
	{
		ULONG arg = 1;
		return (::ioctlsocket(fd, FIONBIO, &arg) == 0);
	}

	// Disable blocking send/recv calls.
	bool Blocking(SOCKET fd)
	{
		ULONG arg = 0;
		return (::ioctlsocket(fd, FIONBIO, &arg) == 0);
	}

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd)
	{
		UINT arg = 1;
		return (::setsockopt(fd, 0x6, TCP_NODELAY, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Enable nagle buffering algorithm
	bool EnableBuffering(SOCKET fd)
	{
		UINT arg = 0;
		return (::setsockopt(fd, 0x6, TCP_NODELAY, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Set internal buffer size to socket.
	//底层发送缓冲大小设为0，只会在网络非常理想的情况下带来性能提升
	bool SetSendBufferSize(SOCKET fd, int size)
	{
		return (::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == 0);
	}

	// Set internal buffer size to socket.
	//接收缓冲区设为0，不会带来任何性能提升，底层的数据拷贝工作是无法避免的
	bool SetRecvBufferSize(SOCKET fd, int size)
	{
		return (::setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == 0);
	}

	// Closes socket completely.
	bool CloseSocket(SOCKET fd)
	{
		if(::shutdown(fd, SD_SEND) == SOCKET_ERROR)
		{
			return (::closesocket(fd) == 0);
		}
		return true;
	}

	// Sets reuseaddr
	bool ReuseAddr(SOCKET fd)
	{
		UINT option = 1;
		return (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) == 0);
	}

	// Set graceful close
	bool SetGracefulClose(SOCKET fd, USHORT time)
	{
		LINGER lingerStruct = {1, time};
		return (::setsockopt(fd, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)) == 0);
	}

	// Inherit listen socket attribute
	bool SetInherit(SOCKET fd, SOCKET listenfd)
	{
		return (::setsockopt(fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&listenfd, sizeof(listenfd)) == 0);
	}

	// Set keep alive
	bool SetKeepAlive(SOCKET fd, int idle, int intv, int times)
	{
		BOOL bKeepAlive = TRUE;
		::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));

		unsigned long ulBytesReturn = 0;
		tcp_keepalive alive_in		= {0};
		tcp_keepalive alive_out     = {0};
		alive_in.keepalivetime      = idle;	// 开始首次KeepAlive探测前的TCP空闭时间
		alive_in.keepaliveinterval	= intv;	// 两次KeepAlive探测间的时间间隔1分钟
		alive_in.onoff              = TRUE;
		return (::WSAIoctl(fd, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in), &alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL) == 0);
	}

	// Get last error
	int GetLastError()
	{
		return ::WSAGetLastError();
	}
}
#endif
