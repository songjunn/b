#include "NameText.h"
#include "CSVParser.h"
#include "strtools.h"
#include "logger.h"


CNameTextMgr::CNameTextMgr()
{
	SetDefLessFunc(m_NameVector);
}

CNameTextMgr::~CNameTextMgr()
{
}

bool CNameTextMgr::LoadCSVData(const char* csvfile)
{
	CCSVParser file;
	if( !csvfile || !file.Load(csvfile) )
	{
		LOGGER_ERROR("[InitData] Load File Failed:%s", csvfile);
		return false;
	}

	int count = file.Line();
	for(int i = 0;i<count; ++i)
	{
		char szString[64]={0};
		file.GetAttribute(i,0,szString,64);

		_AddText(szString, m_NameVector);
	}

	file.Release();

	return true;
}

bool CNameTextMgr::_AddText(char* szString, CUtlMap<CUtlSymbol, CUtlSymbol>& vec)
{
	CUtlSymbol symbol(szString);
	int index = vec.Find(symbol);
	if(vec.IsValidIndex(index))
	{
		LOGGER_ERROR("[NameText] Error:%s", szString);
		return false;
	}
	vec.Insert(symbol, symbol);
	return true;
}

bool CNameTextMgr::CheckName(const char* szString)
{
	if(!szString)
		return false;

	if( !_CheckNameInvalid(szString) )
	{
		LOGGER_DEBUG("[CNameTextMgr] _CheckNameInvalid:%s", szString);
		return false;
	}

	if( !_CheckNameSensitive(szString) )
	{
		LOGGER_DEBUG("[CNameTextMgr] _CheckNameSensitive:%s", szString);
		return false;
	}

	return true;
}

bool CNameTextMgr::_CheckNameSensitive(const char* szString)
{
	if(!szString)
		return false;

	FOR_EACH_MAP(m_NameVector, idx)
	{
		if (strstr(szString, m_NameVector.Element(idx).String()))
			return false;
	}

	return true;
}

bool CNameTextMgr::_CheckNameInvalid(const char* szString)
{
	if(!szString)
		return false;

#define CREATE_NAME_LEN	18		//取名长度限制为12个英文字符或6个中文字符，utf8编码中文字符占3字节
	int len = strlen(szString);
	if (len <= 0 || len > CREATE_NAME_LEN)
		return false;

	/*if (!IsTextUTF8(szString, len))
	{
		Log.Debug("Create Name:%s is not utf8", szString);
		return false;
	}*/

	int i = 0;
	wchar_t cText[CREATE_NAME_LEN+1] = { 0 };
	CharToWideChar(szString, cText, CREATE_NAME_LEN + 1);
	//sprintf(cText, "%s", szString);
	while (cText[i] != '\0')
	{
		//if (!isalpha(cText[i]) && !isdigit(cText[i]) && !IsUtf8ChsChar(cText[i], cText[++i], cText[++i]))
		if (!IsNumWChar(cText[i]) && !IsEngWChar(cText[i]) && !IsChsWChar(cText[i]))
			return false;
		++i;
	}

	return true;
}
