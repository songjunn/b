#include "Packet.h"
#include "crc.h"
#include "logger.h"

Packet::Packet() : sock(0), _rpos(DATA_PARAM), _wpos(DATA_PARAM), _cpos(0)
{
}

Packet::~Packet()
{
}

int Packet::assemble(char * data, int size)
{
	int lsize = 0, rsize = 0, wsize = 0;

	while ((lsize = size - rsize) > 0) {

		if (_getHeadLeftSize() > 0)	//包头未满，把包头写满
		{
			wsize = _getHeadLeftSize() < lsize ? _getHeadLeftSize() : lsize;
		}
		else if (_getLeftSize() > 0)	//包数据未满，把整个包写满
		{
			wsize = _getLeftSize() < lsize ? _getLeftSize() : lsize;
		}

		recvData(data + rsize, wsize);
		rsize += wsize; 
		
		if (_getLeftSize() == 0)		//已摘出一个完整包
		{
			if (!crcCheck())		//完整性校验
			{
				LOGGER_ERROR("crcCheck failed sock=%d size=%d lsize=%d rsize=%d", sock, size, lsize, rsize);
				return 0;
			} else {
				return rsize;
			}
		} 
		else if (_getLeftSize() < 0) 
		{
			LOGGER_ERROR("_getLeftSize() < 0 sock=%d", sock);
			return 0;
	   }
	}

	return rsize;
}

void Packet::recvData(char * buf, uint16 size)
{
	if( size <= 0 || _cpos + size > PACKET_BUFFER_SIZE )
	{
		LOGGER_ERROR("[Packet] Copy faild size=%d", size);
		return;
	}
	memcpy(data+_cpos, buf, size);
	_cpos += size;
}

bool Packet::crcCheck()
{
	uint32_t crc = make_crc32(_DataBuffer(), _DataSize());

	if (_CRC() == crc ) return true;
	else LOGGER_ERROR("[Packet] _CRC():%d != crc:%d ", _CRC(), crc);
	return false;
}

void Packet::setHeader(uint16 wType)
{
	_SetType(wType);
	_SetSize(_wpos);

	uint32_t crc = make_crc32(_DataBuffer(), _DataSize());
	_SetCRC(crc);
}

#if USE_SHARED_PARSER
void Packet::put(int8 value)
{
	appendFlag(BYTE_FLAG);
	append(value);
}

void Packet::put(uint16 value)
{
	appendFlag(WORD_FLAG);
	append(value);
}

void Packet::put(int value)
{
	appendFlag(INT_FLAG);
	append(value);
}

void Packet::put(int64 value)
{
	appendFlag(INT64_FLAG);
	append(value);
}

void Packet::put(const char * value, uint16 size)
{
	appendFlag(STRING_FLAG);
	append(size);
	append(value, size);
}

void Packet::get(int8 & value)
{
	if( checkFlag(BYTE_FLAG) )
		read(value);
}

void Packet::get(uint16 & value)
{
	if( checkFlag(WORD_FLAG) )
		read(value);
}

void Packet::get(int & value)
{
	if( checkFlag(INT_FLAG) )
		read(value);
}

void Packet::get(int64 & value)
{
	if( checkFlag(INT64_FLAG) )
		read(value);
}

void Packet::get(char * value, uint16 size)
{
	if( !checkFlag(STRING_FLAG) )
		return;
	
	uint16 svalue = 0;
	read(svalue);

	int s = svalue < size ? svalue : size;

	read(value, s);
}

template<typename T> void Packet::append(T value) 
{
	_append((char *)&value, sizeof(value));
}

template<typename T> void Packet::read(T & value) 
{
	_read((char *)&value, sizeof(value));
}

inline void Packet::append(const char * value, uint16 size) 
{
	_append(value, size);
}

inline void Packet::read(char * value, int size)
{
	_read(value, size);
}

inline void Packet::_append(const char * value, uint16 size)
{
	assert( _wpos + size <= PACKET_BUFFER_SIZE - DATA_PARAM );
	if( _wpos + size <= PACKET_BUFFER_SIZE - DATA_PARAM )
	{
		memcpy(&data[_wpos], value, size);
		_wpos += size;
	}
}

inline void Packet::_read(void * value, int size)
{
	assert( _rpos + size <= (int)PACKET_BUFFER_SIZE - DATA_PARAM );
	if( _rpos + size <= (int)PACKET_BUFFER_SIZE - DATA_PARAM )
	{
		memcpy(value, &data[_rpos], size);
		_rpos += size;
	}
}

inline void Packet::appendFlag(int8 flag)
{
	data[_wpos] = flag;
	_wpos += sizeof(int8);
}

inline bool Packet::checkFlag(int8 flag)
{
	int8 value = 0;
	read(value);
	return value == flag;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if USE_PROTOCOL_BUFFER
void Packet::setBuffer(uint16 wType, const char* buf, uint16 size)
{
	if( _wpos + size <= PACKET_BUFFER_SIZE )
	{
		memcpy(&data[DATA_PARAM], buf, size);
		_wpos += size;
		setHeader(wType);
	}
	else
	{
		LOGGER_ERROR("[Packet] setBuffer failed, type:%d _wpos:%d size:%d", wType, _wpos, size);
	}
}

void Packet::getBuffer(std::string& buf)
{
	if( Size() >= _rpos )
	{
		buf.assign((char*)&data[_rpos], _DataSize());
		//_rpos += _DataSize();
	}
	else
	{
		LOGGER_ERROR("[Packet] getBuffer failed, _rpos:%d size:%d", _rpos, Size());
	}
}
#endif

