#include "NetHeader.h"

#if defined(_IOSMAC) || defined(__linux)
namespace SocketOps
{
	// init winsock service
	bool NetStartup()
	{
		return true;
	}

	// close winsock service
	bool NetClose()
	{
		return true;
	}

	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor()
	{
		// create a socket for use with overlapped i/o.
		return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd)
	{
		int val = fcntl(fd, F_GETFL, 0);
		if (val < 0){
			return false;
		}
		return fcntl(fd, F_SETFL, val | O_NONBLOCK) >= 0;
	}

	// Disable blocking send/recv calls.
	bool Blocking(SOCKET fd)
	{
		int val = fcntl(fd, F_GETFL, 0);
		if (val < 0){
			return false;
		}
		return fcntl(fd, F_SETFL, val & ~O_NONBLOCK) >= 0;
	}

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd)
	{
		return false;
	}

	// Enable nagle buffering algorithm
	bool EnableBuffering(SOCKET fd)
	{
		return false;
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
		::shutdown(fd, SHUT_WR);
		//::close(fd);
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
		// return (::setsockopt(fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&listenfd, sizeof(listenfd)) == 0);
		return false;
	}

	// Set keep alive
	bool SetKeepAlive(SOCKET fd, int idle, int intv, int times)
	{
		int keepalive = 1;	// 非0值，开启keepalive属性
		::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepalive, sizeof(keepalive));

        int keepidle = idle;	// 如该连接在idle秒内没有任何数据往来,则进行此TCP层的探测
        ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, (void*)&keepidle, sizeof(keepidle));

		int keepinterval = intv;	// 探测发包间隔为intv秒
        ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL,(void*)&keepinterval, sizeof(keepinterval));

		int keepcount = times;	// 尝试探测的次数.如果第1次探测包就收到响应了,则后2次的不再发
        ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepcount, sizeof(keepcount));

        return true;
	}

	// Get last error
	int GetLastError()
	{
		return errno;
	}
}
#endif
