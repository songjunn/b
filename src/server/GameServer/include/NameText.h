#pragma once
#include "Singleton.h"
#include "utlsymbol.h"
#include "utlmap.h"


class CNameTextMgr : public Singleton< CNameTextMgr >
{
public:
	CNameTextMgr();
	~CNameTextMgr();

	bool	LoadCSVData(const char* csvfile);
	bool	CheckName(const char* szString);

protected:
	bool	_AddText(char* szString, CUtlMap<CUtlSymbol, CUtlSymbol>& vec);

	bool	_CheckNameInvalid(const char* szString);	//检查名字合法性，只允许使用中英文字符和数字
	bool	_CheckNameSensitive(const char* szString);	//检查屏蔽字库

protected:
	CUtlMap<CUtlSymbol, CUtlSymbol>	m_NameVector;

};

#define NameTextMgr CNameTextMgr::getSingleton()
