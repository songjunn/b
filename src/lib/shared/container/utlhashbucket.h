//
//	链表解决冲突的哈希表实现
//	1. hash表空间可设置的较大，1M空间在64位系统下表空间为65536，32位系统131072。
//	2. init初始化时自动将表空间扩展为2的倍数。
//	3. 内部可使用内存池管理class Node，直接调用hash表的Alloc()，在Remove(Key)时删除。
//	   亦可使用外部指针,Remove(Key)会返回Node的指针。
//	4. FOR_EACH_HASH宏遍历hash表。
//																li9chuan
//	使用示例：
//struct SUserInfo
//{
//	uint64_t	uid;
//	char szname[64];
//
//	SUserInfo()
//	{
//		memset( this, 0, sizeof(SUserInfo) );
//	}
//};
//
//struct _GetKey : public std::unary_function<uint64_t, uint>
//{
//	uint operator()( uint64_t const key ) const
//	{
//		return key;
//	}
//};
//
//struct _EqualKey : public std::binary_function<SUserInfo*, uint64_t, bool>
//{
//	bool operator()( SUserInfo* const data, uint64_t const key ) const
//	{
//		if( !data ) {	return false; }
//		return data->uid==key;
//	}
//};
//
//CHashBucket<uint64_t,SUserInfo,_GetKey,_EqualKey>		g_HashUserInfo;
//
//int testhash()
//{
//	g_HashUserInfo.init( 500, 100, 100 );		//	自动扩展hash空间为512，100个用于解决冲突的结点，内部预先创建了100个SUserInfo
//
//	cout<<g_HashUserInfo.Size()<<endl;
//
//	SUserInfo* p = g_HashUserInfo.Alloc();
//	p->uid = 0;
//	strcpy( p->szname, "s" );
//	g_HashUserInfo.Insert( p->uid, p );
//
//	p = g_HashUserInfo.Alloc();
//	p->uid = 2048;
//	strcpy( p->szname, "s10" );
//	g_HashUserInfo.Insert( p->uid, p );
//	cout<<g_HashUserInfo.Count()<<endl;
//
//
//	FOR_EACH_HASH( g_HashUserInfo )
//	{
//		cout<<g_HashUserInfo.CurrentItem()<<" id: "<<g_HashUserInfo.CurrentItem()->uid<<"  name: "<<g_HashUserInfo.CurrentItem()->szname<<endl;
//	}
//
//	g_HashUserInfo.Remove( 2048 );
//	cout<<g_HashUserInfo.Count()<<endl;
//
//	g_HashUserInfo.RemoveAll();
//	cout<<g_HashUserInfo.Count()<<endl;
//
//	getchar();
//	return 0;
//}

#pragma once

#include "memorypool.h"
#include "utlvector.h"
#include <assert.h>

template<class Key,class Node, class ExtractKey, class EqualKey>
class CHashBucket
{
public:

	CHashBucket():LOCK_FENCE(256)	{}
	~CHashBucket()	{	Purge(); }
	void Purge( void );

	// 参数1：hash表空间大小  参数2：用于解决冲突的备用元素个数  参数3：内部管理的Node内存池个数
	// 参数1会自动扩展为2的倍数，>=bucketCount。
	void init( int bucketCount, int spareCount, int nodeCount=1 );

	Node* Alloc( void );		//	从内部Node内存池取出一个Node，返回这个Node的指针
	Node* Insert( Key const &srckey, Node* const src );
	Node* Find( Key const &src );
	Node* Remove( Key const &src );
	void  RemoveAll( void );

	uint32_t Count()	{	return m_Count;	}
	uint32_t Size()		{	return bucketMax; }

public:

	void First(void);
	void Next(void);
	bool isDone(void);
	Node* CurrentItem(void);

private:

	const int	LOCK_FENCE;

	struct CHashParam
	{
		Node*		node;
		CHashParam*	next;
		CHashParam() { Init(); }
		~CHashParam(){ Init(); }
		void Init()
		{
			node	= NULL;
			next	= NULL;
		}
	};

	CUtlVector<CHashParam>			m_Buckets;
	CObjectMemoryPool<CHashParam>	m_Spares;				//
	CObjectMemoryPool<Node>			m_Nodes;				//	内部管理的Node内存池

	ExtractKey						getKey;
	EqualKey						equals;

	uint32_t						m_ModMask;				// use the mod mask to "mod"
	uint32_t						m_Count;
	uint32_t						bucketMax;

	uint32_t						iterator_idx;
	CHashParam*						iterator_ptr;
	bool							iterator_isdone;

	uint32_t ToPowerOfTwo( const uint32_t value );

};

template<class Key,class Node, class ExtractKey, class EqualKey>
inline void CHashBucket<Key,Node,ExtractKey,EqualKey>::init( int bucketCount, int spareCount, int nodeCount )
{
	iterator_idx = 0;
	iterator_ptr = NULL;
	iterator_isdone = true;

	bucketMax = ToPowerOfTwo( bucketCount );
	m_ModMask = bucketMax - 1;
	m_Count	= 0;
	m_Buckets.SetSize( bucketMax );
	m_Spares.Init( "Spares", spareCount );
	m_Nodes.Init( "Nodes", nodeCount );
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline void CHashBucket<Key,Node,ExtractKey,EqualKey>::Purge( void )
{
	m_Buckets.Purge();
	m_Spares.Purge();
	m_Nodes.Purge();
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline Node* CHashBucket<Key,Node,ExtractKey,EqualKey>::Find( Key const &src )
{
	unsigned int key = getKey( src );
	unsigned int ndxBucket = ( key & m_ModMask );
	CHashParam* p = &m_Buckets[ndxBucket];

	do{
		if( equals( p->node, src ) )
		{
			return (p->node);
		}
	}while( (p = p->next) );

	return NULL;
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline Node* CHashBucket<Key,Node,ExtractKey,EqualKey>::Remove( Key const &src )
{
	unsigned int key = getKey( src );
	unsigned int ndxBucket = ( key & m_ModMask );
	Node* res = NULL;

	if( equals( m_Buckets[ndxBucket].node, src ) )
	{
		--m_Count;
		res = m_Buckets[ndxBucket].node;
		m_Buckets[ndxBucket].node = NULL;
		return res;
	}

	CHashParam* cur = &m_Buckets[ndxBucket];
	CHashParam* pre = cur;

	while( (cur = cur->next) )
	{
		if( equals( cur->node, src ) )
		{
			--m_Count;
			res = cur->node;
			pre->next = cur->next;
			m_Spares.Free(cur);
			break;
		}
		pre = cur;
	}

	m_Nodes.Free( res );
	return res;
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline void CHashBucket<Key,Node,ExtractKey,EqualKey>::RemoveAll( void )
{
	m_Count = 0;
	m_Spares.FreeAll();
	m_Nodes.FreeAll();
	m_Buckets.SetSize( bucketMax );
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline Node* CHashBucket<Key,Node,ExtractKey,EqualKey>::Alloc( void )
{
	return m_Nodes.Alloc();
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline Node* CHashBucket<Key,Node,ExtractKey,EqualKey>::Insert( Key const &srckey, Node* const src )
{
	assert( equals( src, srckey ) );

	unsigned int key = getKey( srckey );
	unsigned int ndxBucket = ( key & m_ModMask );

	if( m_Buckets[ndxBucket].node )
	{
		CHashParam* cur = &m_Buckets[ndxBucket];

		do{
			//if( m_CompareFunc( cur->node, srckey ) )
			//{
			//	return cur->node;
			//}
			if( cur->next == NULL )
			{
				CHashParam* p = m_Spares.Alloc();
				if(!p)
				{
					assert(0);
					break;
				}

				++m_Count;
				cur->next = p;
				p->Init();
				p->node = src;
				return p->node;
			}

		}while( (cur = cur->next) );
	}
	else
	{
		++m_Count;
		m_Buckets[ndxBucket].node = src;
		return m_Buckets[ndxBucket].node;
	}

	m_Nodes.Free( src );
	return NULL;
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline uint32_t CHashBucket<Key,Node,ExtractKey,EqualKey>::ToPowerOfTwo( const uint32_t value )
{
	uint32_t res = 1;
	for( int i=0; i<32; i++ )
	{
		res = res << 1;
		if( res >= value )	{	break;	}
	}
	return res;
}

//////////////////////////////////////////////////////////////

template<class Key,class Node, class ExtractKey, class EqualKey>
inline void CHashBucket<Key,Node,ExtractKey,EqualKey>::First( void )
{
	iterator_isdone = false;
	iterator_idx = 0;
	iterator_ptr = &m_Buckets[iterator_idx];
	if( !iterator_ptr->node )
	{
		Next();
	}
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline void CHashBucket<Key,Node,ExtractKey,EqualKey>::Next( void )
{
	if( !iterator_isdone )
	{
		do{
			if( iterator_ptr == NULL || iterator_ptr->next == NULL )
			{
				++iterator_idx;
				if( iterator_idx >= bucketMax )	{	iterator_isdone = true;	break;	}
				iterator_ptr = &m_Buckets[iterator_idx];
				continue;
			}
			iterator_ptr = iterator_ptr->next;
		}while( !iterator_ptr->node );
	}
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline bool CHashBucket<Key,Node,ExtractKey,EqualKey>::isDone( void )
{
	return iterator_isdone;
}

template<class Key,class Node, class ExtractKey, class EqualKey>
inline Node* CHashBucket<Key,Node,ExtractKey,EqualKey>::CurrentItem( void )
{
	return iterator_ptr->node;
}

//	遍历hash表的宏，使用CurrentItem()取出当前遍历到的结点：
//	FOR_EACH_HASH( m_HashUserInfo )
//	{
//		Log("user  name: %s  ip:%s", m_HashUserInfo.CurrentItem()->szname, m_HashUserInfo.CurrentItem()->szip );
//	}

#define FOR_EACH_HASH(hashname)\
	for( hashname.First(); !hashname.isDone(); hashname.Next() )



