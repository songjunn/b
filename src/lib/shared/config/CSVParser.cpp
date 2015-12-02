#include <stdio.h>
#include "CSVParser.h"


CCSVParser::CCSVParser()
{
	m_nLine= 0;
	m_nColumn= 0;
	m_fileText = 0;
}

CCSVParser::~CCSVParser()
{
	Release();
}

void CCSVParser::Release()
{
	m_nLine= 0;
	m_nColumn= 0;

	SAFE_DELETE(m_fileText);

	m_ElementTable.Purge( );
}

bool CCSVParser::Load(const char* lpName, const char* lpPathID)
{
	FILE* file = fopen( lpName, "rb" );
	if( file )
	{
		//获取文件长度
		fseek( file, 0, SEEK_END );
		int size = ftell( file );

		m_fileText = NEW char[size];
		if( !m_fileText )
			return false;

		//读取文件内容
		fseek( file, 0, SEEK_SET );
		fread( m_fileText, 1, size, file );

		//解析内容
		Parser( m_fileText, size );

		//释放资源
		fclose( file );

		return true;
	}
	return false;
}

bool CCSVParser::Parser(char* buff, int size)
{
	if(!buff)
		return false;

	bool InsertFlag =true;//是否插入的标�?
	bool dFlag = true;
	m_ElementTable.EnsureCapacity( size );//不希望频繁GROW,不能是strlen
	char* TempChar = buff;
	while (true)
	{
		if(buff + size <= TempChar)
			break;

		if(InsertFlag)
		{
			m_ElementTable.AddToTail(TempChar);
			InsertFlag = false;
		}

		if(*TempChar == 0x2c)		//����
		{
			if(dFlag)
				m_nColumn++;
			InsertFlag = true;
			*TempChar ='\0';
		}
		else if(*TempChar == 0x09)	// ˮƽ��λ����'\t'
		{
			if(dFlag)
				m_nColumn++;
			InsertFlag = true;
			*TempChar ='\0';
		}
		else if(*TempChar == 0x0d)	//��λ��
		{
			if(dFlag)
				m_nColumn++;
			InsertFlag = true;
			*TempChar ='\0';
		}
		else if(*TempChar == 0x0a)	//���з�
		{
			dFlag = false;
			InsertFlag = true;
			m_nLine++;
			*TempChar = '\0';
		}

		TempChar++;
	}
	return true;
}
