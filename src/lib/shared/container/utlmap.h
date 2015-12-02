#pragma once
#include "utlrbtree.h"

#define UNREFERENCED_PARAMETER(P)          (P)

#define UTLMAP_INSERT( MAP, KEY, DATA, SUCCEED )			\
	{														\
	SUCCEED = TRUE;											\
	try	{	MAP.Insert( KEY ,DATA); }						\
	catch(char* p)	{ SUCCEED = FALSE;assert( 0 && p ); UNREFERENCED_PARAMETER(p); }	\
	}														\

#define UTLMAPPTR_INSERT( MAP, KEY, DATA, SUCCEED )			\
	{														\
	SUCCEED = TRUE;											\
	try	{	MAP->Insert( KEY ,DATA );}						\
	catch(char* p){	SUCCEED = FALSE; assert( 0 && p ); UNREFERENCED_PARAMETER(p); }	\
	}														\


//-----------------------------------------------------------------------------
//
// Purpose:	An associative container. Pretty much identical to std::map.
//			Note this class is not thread safe
//
//template <typename T >
//class CMapLog
//{
//public:
//	static void LogFunc( const T &lhs )	{ ; }
//};
//
//#define MapLog( type ) CMapLog<type>::LogFunc
//
//template <> inline void CMapLog<uint8>::LogFunc( uint8 const & nKey ){ Log( "插入Key重复 = %d", nKey ); }
//template <> inline void CMapLog<int>::LogFunc( int const & nKey ){ Log( "插入Key重复 = %d", nKey ); }
//template <> inline void CMapLog<char*>::LogFunc( char * const & szKey ){ Log( "插入Key重复 = %s", szKey ); }



template <typename K, typename T, typename I = unsigned short>
class CUtlMap
{
public:
	typedef K KeyType_t;
	typedef T ElemType_t;
	typedef I IndexType_t;

	// Less func typedef
	// Returns true if the first parameter is "less" than the second
	typedef bool (*LessFunc_t)( const KeyType_t &, const KeyType_t & );

	// Returns true if the first parameter is "less" than the second
	typedef void (*LogFunc_t)( const KeyType_t & );

	// constructor, destructor
	// Left at growSize = 0, the memory will first allocate 1 element and double in size
	// at each increment.
	// LessFunc_t is required, but may be set after the constructor using SetLessFunc() below
	CUtlMap( int growSize = 0, int initSize = 0, LessFunc_t lessfunc = 0 )
	 : m_Tree( growSize, initSize, TreeLessFunc )
	{
		m_pfnLess = lessfunc;
	}

	// gets particular elements
	ElemType_t &		Element( IndexType_t i )			{ return m_Tree.Element( i ).elem; }
	const ElemType_t &	Element( IndexType_t i ) const		{ return m_Tree.Element( i ).elem; }
	ElemType_t &		operator[]( IndexType_t i )			{ return m_Tree.Element( i ).elem; }
	const ElemType_t &	operator[]( IndexType_t i ) const	{ return m_Tree.Element( i ).elem; }
	KeyType_t &			Key( IndexType_t i )				{ return m_Tree.Element( i ).key; }
	const KeyType_t &	Key( IndexType_t i ) const			{ return m_Tree.Element( i ).key; }


	// Num elements
	unsigned int Count() const								{ return m_Tree.Count(); }

	// Max "size" of the vector
	IndexType_t  MaxElement() const							{ return m_Tree.MaxElement(); }

	// Checks if a node is valid and in the map
	bool  IsValidIndex( IndexType_t i ) const				{ return m_Tree.IsValidIndex( i ); }

	// Checks if the map as a whole is valid
	bool  IsValid() const									{ return m_Tree.IsValid(); }

	// Invalid index
	static IndexType_t InvalidIndex()						{ return CTree::InvalidIndex(); }

	// Sets the less func
	void SetLessFunc( LessFunc_t func )
	{
		m_pfnLess = func;

		//m_LogFn = MapLog(K);
	}


	// Insert method (inserts in order)
	IndexType_t  Insert( const KeyType_t &key, const ElemType_t &insert )
	{
		Node_t node;
		node.key = key;
		node.elem = insert;
		assert( m_pfnLess );
		KeyLessFunc( m_pfnLess );
		return m_Tree.Insert( node );
	}

	// Find method
	IndexType_t  Find( const KeyType_t &key ) const
	{
		Node_t dummyNode;
		dummyNode.key = key;
		assert( m_pfnLess );
		KeyLessFunc( m_pfnLess );
		return m_Tree.Find( dummyNode );
	}

	//大于等于这个关键字的最小的元素
	IndexType_t  FindLarger( const KeyType_t &key ) const
	{
		Node_t dummyNode;
		dummyNode.key = key;
		assert( m_pfnLess );
		KeyLessFunc( m_pfnLess );
		return m_Tree.FindLarger( dummyNode );
	}

	// Remove methods
	void     RemoveAt( IndexType_t i )						{ m_Tree.RemoveAt( i ); }
	bool     Remove( const KeyType_t &key )
	{
		Node_t dummyNode;
		dummyNode.key = key;
		assert( m_pfnLess );
		KeyLessFunc( m_pfnLess );
		return m_Tree.Remove( dummyNode );
	}

	void     RemoveAll( )					{ m_Tree.Purge(); }

	// Iteration
	IndexType_t  FirstInorder() const						{ return m_Tree.FirstInorder(); }
	IndexType_t  NextInorder( IndexType_t i ) const			{ return m_Tree.NextInorder( i ); }
	IndexType_t  PrevInorder( IndexType_t i ) const			{ return m_Tree.PrevInorder( i ); }
	IndexType_t  LastInorder() const						{ return m_Tree.LastInorder(); }

	// If you change the search key, this can be used to reinsert the
	// element into the map.
	void	Reinsert( const KeyType_t &key, IndexType_t i )
	{
		m_Tree[i].key = key;
		m_Tree.Reinsert(i);
	}

	IndexType_t InsertOrReplace( const KeyType_t &key, const ElemType_t &insert )
	{
		IndexType_t i = Find( key );
		if ( i != InvalidIndex() )
		{
			Element( i ) = insert;
			return i;
		}

		return Insert( key, insert );
	}
	// Find Element		//by wangwei
	ElemType_t  GetElement( const KeyType_t &key ) const
	{
		IndexType_t index = Find(key);
		if( IsValidIndex(index) )
		{
			return Element(index);
		}
		return NULL;
	}

	void SetMemoryMode(int nGrowSize, int nInitAllocationCount)
	{
		m_Tree.SetMemoryMode(nGrowSize,nInitAllocationCount);
	}

protected:
	struct Node_t
	{
		KeyType_t	key;
		ElemType_t	elem;
	};

	typedef CUtlRBTree<Node_t, I> CTree;

	static bool TreeLessFunc( const Node_t &left, const Node_t &right )
	{
		return (*KeyLessFunc())( left.key, right.key );
	}

	static LessFunc_t KeyLessFunc( LessFunc_t pfnNew = NULL )
	{
		// @Note (toml 12-10-02): This is why the class is not thread safe. The way RB
		//						  tree is implemented, I could see no other efficient
		//						  and portable way to do this. This function local
		//						  static approach is used to not require
		//						  instantiation of a static data member or use
		//						  of Microsoft extensions (_declspec(selectany))
		static LessFunc_t pfnLess;

		if ( pfnNew != NULL )
			pfnLess = pfnNew;
		return pfnLess;
	}

	CTree 	   m_Tree;
	LessFunc_t m_pfnLess;
	//LogFunc_t  m_LogFn;
};

#define FOR_EACH_MAP(mapname,indexname)\
	for(int indexname = mapname.FirstInorder(); mapname.IsValidIndex(indexname); indexname = mapname.NextInorder(indexname))
