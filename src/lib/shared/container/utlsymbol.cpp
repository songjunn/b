//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: Defines a symbol table
//
// $Header: $
// $NoKeywords: $
//=============================================================================

#pragma warning (disable:4514)

#include "utlsymbol.h"

#define INVALID_STRING_INDEX 0xFFFFFFFF

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------

CUtlSymbolTable* CUtlSymbol::s_pSymbolTable = 0; 
bool CUtlSymbol::s_bAllowStaticSymbolTable = true;


//-----------------------------------------------------------------------------
// symbol methods
//-----------------------------------------------------------------------------

void CUtlSymbol::Initialize()
{
	// If this assert fails, then the module that this call is in has chosen to disallow
	// use of the static symbol table. Usually, it's to prevent confusion because it's easy
	// to accidentally use the global symbol table when you really want to use a specific one.
	//Assert( s_bAllowStaticSymbolTable );

	// necessary to allow us to create global symbols
	static bool symbolsInitialized = false;
	if (!symbolsInitialized)
	{
		s_pSymbolTable = new CUtlSymbolTable;
		symbolsInitialized = true;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Singleton to delete table on exit from module
//-----------------------------------------------------------------------------
class CCleanupUtlSymbolTable
{
public:
	~CCleanupUtlSymbolTable()
	{
		SAFE_DELETE( CUtlSymbol::s_pSymbolTable );
	}
};

static CCleanupUtlSymbolTable g_CleanupSymbolTable;

CUtlSymbolTable* CUtlSymbol::CurrTable()
{
	Initialize();
	return s_pSymbolTable; 
}

//CPlatformCS g_SymbolCS;

//-----------------------------------------------------------------------------
// string->symbol->string
//-----------------------------------------------------------------------------

CUtlSymbol::CUtlSymbol( char const* pStr )
{
	//g_SymbolCS.Lock( );
	//m_Id = CurrTable()->AddString( pStr );
	//g_SymbolCS.Unlock( );
	AddString(pStr);
}

char const* CUtlSymbol::String( ) const
{
	//g_SymbolCS.Lock( );
	char const * pString = CurrTable()->String(m_Id);
	//g_SymbolCS.Unlock( );
	return pString;
}

UtlSymId_t CUtlSymbol::AddString( char const* pStr )
{
	//g_SymbolCS.Lock( );
	m_Id = CurrTable()->AddString( pStr );
	//g_SymbolCS.Unlock( );
	return m_Id;
}

CUtlSymbol CUtlSymbol::Find( char const* pStr )
{
	//g_SymbolCS.Lock( );
	CUtlSymbol res = CurrTable()->Find( pStr );
	//g_SymbolCS.Unlock( );
	return res;
}

void CUtlSymbol::DisableStaticSymbolTable()
{
	s_bAllowStaticSymbolTable = false;
}

//-----------------------------------------------------------------------------
// checks if the symbol matches a string
//-----------------------------------------------------------------------------

bool CUtlSymbol::operator==( char const* pStr ) const
{
	if (m_Id == UTL_INVAL_SYMBOL) 
		return false;
	return strcmp( String(), pStr ) == 0;
}



//-----------------------------------------------------------------------------
// symbol table stuff
//-----------------------------------------------------------------------------

struct LessCtx_t
{
	char const* m_pUserString;
	CUtlSymbolTable* m_pTable;
	
	LessCtx_t( ) : m_pUserString(0), m_pTable(0) {}
};

static LessCtx_t g_LessCtx;


bool CUtlSymbolTable::SymLess( unsigned int const& i1, unsigned int const& i2 )
{
	char const* str1 = (i1 == INVALID_STRING_INDEX) ? g_LessCtx.m_pUserString :
											&g_LessCtx.m_pTable->m_Strings[i1];
	char const* str2 = (i2 == INVALID_STRING_INDEX) ? g_LessCtx.m_pUserString :
											&g_LessCtx.m_pTable->m_Strings[i2];
	
	return strcmp( str1, str2 ) < 0;
}


bool CUtlSymbolTable::SymLessi( unsigned int const& i1, unsigned int const& i2 )
{
	char const* str1 = (i1 == INVALID_STRING_INDEX) ? g_LessCtx.m_pUserString :
											&g_LessCtx.m_pTable->m_Strings[i1];
	char const* str2 = (i2 == INVALID_STRING_INDEX) ? g_LessCtx.m_pUserString :
											&g_LessCtx.m_pTable->m_Strings[i2];
#ifdef WIN32
	return stricmp( str1, str2 ) < 0;
#else
	return strcasecmp( str1, str2 ) < 0;
#endif
}

//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------

CUtlSymbolTable::CUtlSymbolTable( int growSize, int initSize, bool caseInsensitive ) : 
	m_Lookup( growSize, initSize, caseInsensitive ? SymLessi : SymLess ), m_Strings( 256 )
{
}

CUtlSymbolTable::~CUtlSymbolTable()
{
}

CUtlSymbol CUtlSymbolTable::Find( char const* pString )
{	
	if (!pString)
		return CUtlSymbol();

	//g_SymbolCS.Lock( );

	// Store a special context used to help with insertion
	g_LessCtx.m_pUserString = pString;
	g_LessCtx.m_pTable = this;
	
	// Passing this special invalid symbol makes the comparison function
	// use the string passed in the context
	UtlSymId_t idx = m_Lookup.Find( INVALID_STRING_INDEX );
	CUtlSymbol syRet( idx );

	//g_SymbolCS.Unlock( );

	return syRet;
}


//-----------------------------------------------------------------------------
// Finds and/or creates a symbol based on the string
//-----------------------------------------------------------------------------

CUtlSymbol CUtlSymbolTable::AddString( char const* pString )
{
	if (!pString) 
		return CUtlSymbol( UTL_INVAL_SYMBOL );

	CUtlSymbol id = Find( pString );
	
	//g_SymbolCS.Lock( );

	if (id.IsValid())
	{
		//g_SymbolCS.Unlock( );
		return id;
	}

	// Store a special context used to help with insertion
	g_LessCtx.m_pUserString = pString;
	g_LessCtx.m_pTable = this;

	// didn't find, insert the string into the vector.
	int len = strlen(pString) + 1;
	int stridx = m_Strings.AddMultipleToTail( len );
	memcpy( &m_Strings[stridx], pString, len * sizeof(char) );
	UtlSymId_t idx = m_Lookup.Insert( stridx );
	CUtlSymbol syRet( idx );

	//g_SymbolCS.Unlock( );

	return syRet;
}


//-----------------------------------------------------------------------------
// Look up the string associated with a particular symbol
//-----------------------------------------------------------------------------

char const* CUtlSymbolTable::String( CUtlSymbol id ) const
{
	if (!id.IsValid()) 
		return "";
	
	//g_SymbolCS.Lock( );

	const char * pRet = NULL;

	if(!m_Lookup.IsValidIndex((UtlSymId_t)id))
		return "";
	pRet = &m_Strings[m_Lookup[id]];

	//g_SymbolCS.Unlock( );

	return pRet;
}


//-----------------------------------------------------------------------------
// Remove all symbols in the table.
//-----------------------------------------------------------------------------

void CUtlSymbolTable::RemoveAll()
{
	//g_SymbolCS.Lock( );
	m_Lookup.RemoveAll();
	m_Strings.RemoveAll();
	//g_SymbolCS.Unlock( );
}

