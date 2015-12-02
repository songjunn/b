#pragma once
#ifndef WIN32
#include <ctype.h>
#endif

void strnupper(const char* src, char* des, int len);
void strnlower(const char* src, char* des, int len);

void WideCharToChar( const wchar_t * lpSrc, char * lpDst, int nMax );
void CharToWideChar( const char * lpSrc, wchar_t * lpDst, int nMax );

int ConvertGBKToUTF8( char* strGBK, char* strDestUTF8, int nMaxSize );
int ConvertUTF8ToGBK( char* strUtf8, char* strDestGBK, int nMaxSize );

bool IsTextUTF8(const char* str,int length);		//检查utf8

//inline	bool IsUtf8ChsChar(char c1, char c2, char c3)	{ return ((c1 < 0xF0 || c1 > 0xe0) && (c2 < 0 || c2 > 0x7f) && (c3 < 0 || c3 > 0x7f)); }			//中文字符
inline	bool IsChsWChar(wchar_t wchar)		{return (wchar >= 0x4e00 && wchar <= 0x9fa5);}		//中文字符
inline	bool IsEngWChar(wchar_t wchar)		{return (wchar >= 65 && wchar <= 90) || (wchar >= 97 && wchar <= 122);}	//英文字符
inline	bool IsNumWChar(wchar_t wchar)		{return (wchar >= 48 && wchar <= 57);}				//数字
inline	bool IsUnusualSymbol(const char c)	{return ispunct(c) || isspace(c) || iscntrl(c);}	//特殊符号

unsigned char ToHex(unsigned char x);
unsigned char FromHex(unsigned char x);
unsigned char *UrlEncode(const unsigned char *srcstr, unsigned char *desstr, size_t nlen);
unsigned char *UrlDecode(const unsigned char *srcstr, unsigned char *desstr, size_t nlen);

//XML解析 xmlvalue为输出的值 前提：xmlstr必须是合法的XML串
char *ParseXmlString(const char *xmlstr, const char *xmlname, char *xmlvalue, int nlen);
