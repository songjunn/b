/*
 * 用于缓存网络消息报的环形缓冲，非线程安全
 *
 *				songjun 2012.12.25
 */

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include "types.h"

class RingBuffer
{
protected:
	//缓冲区头和尾指针
	char * m_buffer;
	char * m_bufferEnd;

	//数据写入和读取指针
	char * m_ReadPtr;
	char * m_WritePtr;

	//未读取数据长度
	size_t m_ReadSize;

	inline size_t GetSizeAfterWptr() { return (m_WritePtr < m_ReadPtr) ? (m_ReadPtr - m_WritePtr) : (m_bufferEnd - m_WritePtr); }
	inline size_t GetSizeAfterRptr() { return (m_ReadPtr < m_WritePtr) ? (m_WritePtr - m_ReadPtr) : (m_bufferEnd - m_ReadPtr); }

public:
	/** 构造函数
	*/
	RingBuffer();

	/** 析构函数
	*/
	~RingBuffer();

	/**初始化内存，注意：构造后需首先调用
	* @param size 申请缓冲区的大小
	*/
	bool Allocate(size_t size);

	/**清空数据，并不释放内存
	*/
	void Clear();

	/** 从缓冲区读取
	* @param des 读取的数据所存放的地址
	* @param bytes 读取的字节数
	* @return 实际读取的字节数
	*/
	size_t Read(char * des, size_t bytes);

	/** 从缓冲区删除
	* @param bytes 删除的字节数
	* @return 实际删除的字节数
	*/
	size_t Remove(size_t bytes);

	/** 向缓冲区写入
	* @param data 要写入的数据指针
	* @param bytes 要写入的字节数
	* @return 成功或失败
	*/
	bool Write(const char * data, size_t bytes);

	///** 将数据压缩后，再向缓冲区写入
	//* @param data 要写入的数据指针
	//* @param bytes 要写入的字节数
	//* @return 成功或失败
	//*/
	//bool WriteCompress(const char * data, uint32 bytes);

	/** 获取读指针和其到末尾的长度
	* @param len 通过引用返回读指针到末尾的长度
	* @return 读指针
	*/
	char * GetReadPtr(size_t & len);

	/** 获取写指针和其到末尾的长度
	* @param len 通过引用返回写指针到末尾的长度
	* @return 写指针
	*/
	char * GetWritePtr(size_t & len);

	/** 返回缓冲区剩余大小
	*/
	inline size_t GetFreeSize()	{ return m_bufferEnd - m_buffer - m_ReadSize; }

	/** 返回缓冲区已占用大小
	*/
	inline size_t GetReadSize()	{ return m_ReadSize; }

};

#endif	//_RINGBUFFER_H_
