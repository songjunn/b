#ifdef WIN32
#include <windows.h>
#else
#include <ctype.h>
#include <wchar.h>
#include "types.h"
#endif
#include <stdlib.h>
#include <string.h>
#include "strtools.h"
#include "memory.h"


void strnupper(const char* src, char* des, int len)
{
	strncpy(des, src, len);
	char* str = des;
	while( str && *str )
	{
		*str = (char)toupper(*str);
		str++;
	}
}

void strnlower(const char* src, char* des, int len)
{
	strncpy(des, src, len);
	char* str = des;
	while( str && *str )
	{
		*str = (char)tolower(*str);
		str++;
	}
}

void WideCharToChar( const wchar_t * lpSrc, char * lpDst, int nMax )
{
	int nCountSrc = wcslen( lpSrc ) + 1;
	int nCount = nCountSrc * 2;
	if( nCount < nMax )
	{
#ifdef WIN32
		WideCharToMultiByte(CP_UTF8, 0, lpSrc, nCountSrc, lpDst, nCount, NULL, NULL );
#else
		setlocale(LC_ALL,"");
		wcstombs(lpDst, lpSrc, nCount);
#endif
		lpDst[nCount] = '\0';
	}
}

void CharToWideChar( const char * lpSrc, wchar_t * lpDst, int nMax )
{
	int nCount = strlen( lpSrc ) + 1;
	if( nCount <= nMax )
	{
#ifdef 	WIN32	
		MultiByteToWideChar(CP_UTF8, 0, lpSrc, nCount, lpDst, nCount );
#else
		setlocale(LC_ALL,"");
		mbstowcs(lpDst, lpSrc, nCount );
#endif		
	}
}

int ConvertGBKToUTF8(char* strGBK, char* strDestUTF8, int nMaxSize )
{
#ifdef WIN32
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
	wchar_t* wszUtf8 = NEW wchar_t[nMaxSize + 1];
	if( !wszUtf8 )
		return 0;

	memset(wszUtf8, 0, sizeof(wchar_t) * (nMaxSize + 1));
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, wszUtf8, nMaxSize);

	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
	char* szUtf8 = NEW char[nMaxSize + 1];
	if( !szUtf8 )
		return 0;

	memset(szUtf8, 0, sizeof(char) * (nMaxSize + 1));
	WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, nMaxSize, NULL,NULL);

	if( (len-1) <= nMaxSize )	// len -1 表示 去掉最后一个\0 nMaxSize  不带\0
	{
		memcpy(strDestUTF8,szUtf8,nMaxSize);
	}
	else
	{
		strDestUTF8[0] = '\0';
	}

	SAFE_DELETE(wszUtf8);
	SAFE_DELETE(szUtf8);
#endif
	return strlen(strDestUTF8);
} 

int ConvertUTF8ToGBK(char* strUtf8, char* strDestGBK, int nMaxSize )
{
#ifdef WIN32
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wchar_t* wszGBK = NEW wchar_t[nMaxSize + 1];
	if( !wszGBK )
		return 0;

	memset(wszGBK, 0, sizeof(wchar_t) * (nMaxSize + 1));
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, nMaxSize);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = NEW char[nMaxSize + 1];
	if( !szGBK )
		return 0;

	memset(szGBK, 0, sizeof(char) * (nMaxSize + 1));
	len = WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, nMaxSize, NULL,NULL);
	
	if( (len-1) <= nMaxSize )		// len -1 表示 去掉最后一个\0 nMaxSize  不带\0
	{
		memcpy(strDestGBK,szGBK,nMaxSize);
	}
	else
	{
		strDestGBK[0] = '\0';
	}

	SAFE_DELETE(wszGBK);
	SAFE_DELETE(szGBK);
#endif
	return strlen(strDestGBK);
}

bool IsTextUTF8(const char* str,int length)
{
	int i;
	unsigned long nBytes = 0;	//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr;
	bool bAllAscii = true;		//如果全部都是ASCII, 说明不是UTF-8
	for(i=0;i<length;i++)
	{
		chr= *(str+i);
		if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii= false;
		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)
					nBytes=6;
				else if(chr>=0xF8)
					nBytes=5;
				else if(chr>=0xF0)
					nBytes=4;
				else if(chr>=0xE0)
					nBytes=3;
				else if(chr>=0xC0)
					nBytes=2;
				else
					return false;

				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
				return false;
			nBytes--;
		}
	}
	if( nBytes > 0 ) //违返规则
	{
		return false;
	}
	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
	{
		return false;
	}
	return true;
}

unsigned char ToHex(unsigned char x)
{
	return (x > 9) ? (x + 55) : (x + 48);
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

unsigned char *UrlEncode(const unsigned char *srcstr, unsigned char *desstr, size_t nlen)
{
	size_t n = 0;
	for (size_t i = 0; i < nlen && srcstr[i]; i++) {
		unsigned char uc = srcstr[i];
		if (isalnum(uc) || uc == '-' || uc == '_' || uc == '.' || uc == '~') {
			desstr[n++] = uc;
		}
		else if (uc == ' ') {
			desstr[n++] = '+';
		}
		else {
			desstr[n++] = '%';
			desstr[n++] = ToHex(uc >> 4);
			desstr[n++] = ToHex(uc % 16);
		}
	}
	desstr[n] = 0;

	return desstr;
}

unsigned char *UrlDecode(const unsigned char *srcstr, unsigned char *desstr, size_t nlen)
{
	size_t n = 0;
	for (size_t i = 0; i < nlen && srcstr[i]; i++) {
		unsigned char uc = srcstr[i];
		if (uc == '+') {
			desstr[n++] = ' ';
		}
		else if (uc == '%') {
			assert(i + 2 < nlen);
			unsigned char high = FromHex(srcstr[++i]);
			unsigned char low = FromHex(srcstr[++i]);
			desstr[n++] = high * 16 + low;
		}
		else {
			desstr[n++] = uc;
		}
	}
	desstr[n] = 0;

	return desstr;
}

char *ParseXmlString(const char *xmlstr, const char *xmlname, char *xmlvalue, int nlen)
{
	if (!xmlstr || !xmlname || !xmlstr[0] || !xmlname[0]) {
		return NULL;
	}

	const char *substr = strstr(xmlstr, xmlname);
	if (!substr) {
		return NULL;
	}

	if (xmlstr == substr) { //对于xml串来说不可能
		return NULL;
	}

	int fc = *(substr - 1);
	substr += strlen(xmlname);
	int bc = *(substr + 0);
	if (!isalnum(fc) && fc != '_' && !isalnum(bc) && bc != '_' && bc != 0) {
		if (fc == ' ' && bc == '=') {
			int n = 0;
			for (int i = 2; n < nlen && i < strlen(substr) && substr[i] != '"'; i++) {
				xmlvalue[n++] = substr[i];
			}
			xmlvalue[n] = 0;

			return xmlvalue;
		}
		if (fc == '<' && bc == '>') {
			int n = 0;
			for (int i = 1; n < nlen && i < strlen(substr) && substr[i] != '<'; i++) {
				xmlvalue[n++] = substr[i];
			}
			xmlvalue[n] = 0;

			return xmlvalue;
		}
	}
	else {
		return ParseXmlString(substr, xmlname, xmlvalue, nlen);
	}
}

