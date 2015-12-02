#include <string.h>
#include "Socker.h"
#include "SocketOps.h"
#include "logger.h"
#include "memory.h"


CSocker::CSocker() : m_socket(INVALID_SOCKET), m_status(Key_Free)
{
}

CSocker::~CSocker()
{
}

void CSocker::Release()
{
	LOGGER_DEBUG("Release CSocker:%d", m_socket);

	if( !SocketOps::CloseSocket(m_socket) )
	{
		LOGGER_ERROR("[CSocker] Error:%s,%d, SocketOps::CloseSocket error:%d, socket=%d", __FILE__, __LINE__, SocketOps::GetLastError(), m_socket);
	}

	m_socket = INVALID_SOCKET;
	m_timeout = 0;
	m_szIP[0] = '\0';
	m_status = Key_Free;	
	m_RecvSize = 0;

	SAFE_DELETE(m_RecvBuffer);
	SAFE_DELETE(m_SendBuffer);
}

void CSocker::Clear()
{
	LOGGER_DEBUG("Clear CSocker:%d", m_socket);

	if (!SocketOps::CloseSocket(m_socket))
	{
		LOGGER_ERROR("[CSocker] Error:%s,%d, SocketOps::CloseSocket error:%d, socket=%d", __FILE__, __LINE__, SocketOps::GetLastError(), m_socket);
	}

	m_socket = INVALID_SOCKET;
	m_timeout = 0;
	m_szIP[0] = '\0';
	m_status = Key_Free;

	memset(m_RecvBuffer, 0, m_RecvSize);
	m_SendBuffer->Clear();
}

bool CSocker::InitBuffer(int nSendSize, int nRecvSize)
{
	//初始化接收数据缓冲区
	m_RecvSize = nRecvSize;
	m_RecvBuffer = NEW char[nRecvSize];
	if( !m_RecvBuffer )
	{
		LOGGER_ERROR("new recv buffer failed，size=%d", nRecvSize);
		return false;
	}

	//初始化发送数据缓冲区
	m_SendBuffer = NEW RingBuffer;
	if (!m_SendBuffer || !m_SendBuffer->Allocate(nSendSize))
	{
		LOGGER_ERROR("new send buffer failed，size=%d", nSendSize);
		return false;
	}

	return true;
}

bool CSocker::CreateSocket(SOCKET sock)
{
	if( m_socket != INVALID_SOCKET )
	{
		LOGGER_ERROR("create socket failed: socket not invalid %d", m_socket);
		return false;
	}

	if( sock != INVALID_SOCKET )
	{
		m_socket = sock;
	}
	else
	{
		//创建socket
		m_socket = SocketOps::CreateTCPFileDescriptor();
		if( m_socket == INVALID_SOCKET )
		{
			LOGGER_ERROR("创建socket失败：%d", SocketOps::GetLastError());
			return false;
		}
	}

	if( !SocketOps::SetGracefulClose(m_socket) )
	{
		LOGGER_ERROR("SocketOps::SetGracefulClose error:%d, socket=%d", SocketOps::GetLastError(), m_socket);
		return false;
	}

	m_status = Key_Work;

	return true;
}
