#include <string.h>
#include "RingBuffer.h"
#include "memory.h"
#include "logger.h"


RingBuffer::RingBuffer() : m_buffer(NULL), m_bufferEnd(NULL), m_ReadPtr(NULL), m_WritePtr(NULL), m_ReadSize(0)
{
}

RingBuffer::~RingBuffer()
{
	SAFE_DELETE(m_buffer);
	m_ReadPtr = m_WritePtr = m_bufferEnd = NULL;
	m_ReadSize = 0;
}

void RingBuffer::Clear()
{
	size_t size = m_bufferEnd - m_buffer;
	memset(m_buffer, 0, size);
	m_ReadPtr = m_WritePtr = m_buffer;
	m_ReadSize = 0;
}

bool RingBuffer::Allocate(size_t size)
{
	m_buffer = NEW char[size];
	m_bufferEnd = m_buffer + size;
	m_ReadPtr = m_WritePtr = m_buffer;
	m_ReadSize = 0;
	return true;
}

size_t RingBuffer::Read(char * des, size_t bytes)
{
	//如果缓冲区没有数据，就不读了
	if( GetReadSize() <= 0 )
		return 0;

	//如果缓冲区没有足够的数据，就把剩余数据读完
	size_t size = (GetReadSize() < bytes) ? GetReadSize() : bytes;
	size_t space = GetSizeAfterRptr();

	//如果读指针后面的数据大于等于要读的大小，就读取size大小
	if( space >= size )
	{
		memcpy(des, m_ReadPtr, size);
		m_ReadPtr += size;

		//如果读指针已达缓冲区末尾，则指向缓冲头
		m_ReadPtr = m_ReadPtr < m_bufferEnd ? m_ReadPtr : m_buffer;
	}
	//如果读指针后面的数据小于要读的大小，就先把读指针和面的数据读完，再从缓冲头读取剩下的大小
	else
	{
		memcpy(des, m_ReadPtr, space);
		memcpy(des + space, m_buffer, size - space);

		m_ReadPtr = m_buffer + size - space;
	}

	m_ReadSize -= size;

	return size;
}

size_t RingBuffer::Remove(size_t bytes)
{
	//如果缓冲区没有数据，就不删除了
	if( GetReadSize() <= 0 )
		return 0;

	//如果缓冲区没有足够的数据，就把剩余数据删除
	size_t size = (GetReadSize() < bytes) ? GetReadSize() : bytes;
	size_t space = GetSizeAfterRptr();

	//如果读指针后面的数据大于等于要删除的大小，就删除size大小
	if( space >= size )
	{
		m_ReadPtr += size;

		//如果读指针已达缓冲区末尾，则指向缓冲头
		m_ReadPtr = m_ReadPtr < m_bufferEnd ? m_ReadPtr : m_buffer;
	}
	//如果读指针后面的数据小于要删除的大小，就先把读指针和面的数据删除，再从缓冲头删除剩下的大小
	else
	{
		m_ReadPtr = m_buffer + size - space;
	}

	m_ReadSize -= size;

	return GetReadSize();
}

bool RingBuffer::Write(const char * data, size_t bytes)
{
	//如果缓冲区没有足够的空间，就不写了
	if( GetFreeSize() < bytes )
	{
		LOGGER_WARN("GetFreeSize() < bytes %u", bytes);
		return false;
	}

	//如果写指针后面的剩余空间足够，就全部写入
	size_t space = GetSizeAfterWptr();
	if( space >= bytes )
	{
		memcpy(m_WritePtr, data, bytes);
		m_WritePtr += bytes;

		//如果写指针已达缓冲末尾，则将其指向缓冲区头
		m_WritePtr = m_WritePtr < m_bufferEnd ? m_WritePtr : m_buffer;
	}
	//如果写指针后面的剩余空间不够，就先把写指针后面写满，再把剩余的数据从缓冲头写入
	else if( space < bytes )
	{
		memcpy(m_WritePtr, data, space);
		memcpy(m_buffer, data + space, bytes - space);

		m_WritePtr = m_buffer + bytes - space;
	}

	m_ReadSize += bytes;

	return true;
}

//bool RingBuffer::WriteCompress(const char * data, uint32 bytes)
//{
//	ULONG size = PACKET_BUFFER_SIZE;
//	uint8 buff[PACKET_BUFFER_SIZE] = {0};
//
//	if( !ZipBufferToBuffer(buff, size, (uint8 *)data, bytes) )
//		return false;
//
//	return Write((char *)buff, size);
//}

char * RingBuffer::GetReadPtr(ULONG & len)
{
	if( GetReadSize() <= 0 )
		len = 0;
	else
		len = GetSizeAfterRptr();

	return m_ReadPtr;
}

