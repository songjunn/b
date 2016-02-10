#ifndef SHARED_PACKET_H
#define SHARED_PACKET_H

#include <string.h>
#include <stdlib.h>
#include <string>
#include "NetHeader.h"
#include "types.h"
#include "crc.h"

using namespace std;

#define USE_SHARED_PARSER		0
#define USE_PROTOCOL_BUFFER		1

#define PACKET_BUFFER_SIZE		4096

const char BYTE_FLAG	= 1;	//%b
const char WORD_FLAG	= 2;	//%w
const char INT_FLAG		= 3;	//%d
const char INT64_FLAG	= 4;	//%i
const char STRING_FLAG	= 5;	//%s

////////////////////////////////////////////////////////////////////////////////////
////	Packet 数据结构定义：
////	网络发送和接收的消息数据保存在 data 内存块中
////	其中 HEAD_SIZE 为消息长度数据的起始位置，长度数据占一个uint16
////		 HEAD_TYPE 为消息类型数据的起始位置，类型数据占一个uint16
////		 DATA_PARAM 为消息内容数据的其实位置，
////	消息总长度不超过 PACKET_BUFFER_SIZE
////////////////////////////////////////////////////////////////////////////////////
class Packet
{
private:
	SOCKET	sock;			//发送的网络id,在接受Packet时设置
	uint16	_rpos, _wpos, _cpos;	//拼包解包收包时的读写指针

	static const int8 HEAD_SIZE = 0;	//消息长度(int16)
	static const int8 HEAD_TYPE = 2;	//消息号(int16)
	static const int8 HEAD_CRC = 4;		//数据校验位(int32)
	static const int8 HEAD_TRANS = 8;	//发送对象(int64)
	static const int8 DATA_PARAM = 16;	//数据位
	char data[PACKET_BUFFER_SIZE];		//消息缓冲

public:
	Packet();
	virtual ~Packet();

	inline SOCKET GetNetID() {return sock;}
	inline void	SetNetID(SOCKET netid) {sock = netid;}

	inline int64 GetTrans()	{return *(int64*)(data + HEAD_TRANS);}
	inline void	SetTrans(int64 s) {*(int64*)(data + HEAD_TRANS) = s;}

	inline uint16 Type() {return *(uint16*)(data + HEAD_TYPE);}
	inline char * Data() {return data;}
	inline uint16 Size() {return *(uint16*)(data + HEAD_SIZE);}

	inline bool isFull() {return _getLeftSize() == 0;}

    int assemble(char * data, int& size);
	void recvData(char * buf, uint16 size);
	bool crcCheck();

#if USE_SHARED_PARSER
	void put(int8 value);
	void put(uint16 value);
	void put(int value);
	void put(int64 value);
	void put(const char * value, uint16 size);

	void get(int8 & value);
	void get(uint16 & value);
	void get(int & value);
	void get(int64 & value);
	void get(char * value, uint16 size);
#endif

#if USE_PROTOCOL_BUFFER
#define PROTOBUF_CMD_PARSER( pack, messge )	\
	{										\
		std::string buf;					\
		pack->getBuffer(buf);				\
		messge.ParseFromString(buf);		\
		LOGGER_DEBUG("[Packet]Recv Message %d size %u: {%s}", pack->Type(), pack->Size(), messge.Utf8DebugString().c_str()); \
	}										\

#define PROTOBUF_CMD_PACKAGE( pack, messge, type )		\
	{													\
		std::string buf;								\
		messge.SerializeToString(&buf);					\
		pack.setBuffer(type, buf.c_str(), buf.size());	\
		LOGGER_DEBUG("[Packet]Send Message %d size %u: {%s}", type, pack.Size(), messge.Utf8DebugString().c_str()); \
	}													\

	void setBuffer(uint16 wType, const char* buf, uint16 size);
	void getBuffer(std::string& buf);
#endif

private:
	inline void	_SetSize(uint16 wSize) {*(uint16*)(data + HEAD_SIZE) = wSize;}
	inline void	_SetType(uint16 wType) {*(uint16*)(data + HEAD_TYPE) = wType;}

	inline uint16 _HeadSize() {return DATA_PARAM;}
	inline char * _DataBuffer()	{return data + DATA_PARAM;}
	inline uint16 _DataSize() {return Size() - _HeadSize();}

	inline uint32_t	_CRC() {return *(uint32_t*)(data + HEAD_CRC);}
	inline void	_SetCRC(uint32_t CRC) {*(uint32_t*)(data + HEAD_CRC) = CRC;}

	inline int _getLeftSize() {return Size() - _cpos;}
	inline int _getHeadLeftSize() {return DATA_PARAM - _cpos;}

	void setHeader(uint16 wType);

#if USE_SHARED_PARSER
	template <typename T> void append(T value);
	template <typename T> void read(T & value);

	void append(const char * value, uint16 size);
	void read(char * value, int size);

	void _append(const char * value, uint16 size);
	void _read(void * value, int size);

	void appendFlag(int8 flag);
	bool checkFlag(int8 flag);
#endif

};

#endif	//SHARED_PACKET_H
