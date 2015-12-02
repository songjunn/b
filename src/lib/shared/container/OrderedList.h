#pragma once

#include "utllinkedlist.h"

#define FOR_EACH_ORDER( listName, iteratorName ) \
	for( int iteratorName=listName.m_list.Head(); iteratorName != listName.m_list.InvalidIndex(); iteratorName = listName.m_list.Next( iteratorName ) )

template< class T >
class COrderedList
{
public:
	typedef bool (*LessFunc_t)( const T src1, const T src2 );

	COrderedList() {}
	~COrderedList(){}

	void Initialize( LessFunc_t lessFunc, int size );

	void Insert( T src );
	T Element( int i ); 
	void RemoveAll();

protected:
	int	m_nSize;
	LessFunc_t m_Lessfunc;

public:
	CUtlLinkedList<T> m_list;

};

template< class T >
void COrderedList<T>::Initialize( LessFunc_t lessFunc, int size )
{
	m_nSize = size;
	m_Lessfunc = lessFunc;
}

template< class T >
void COrderedList<T>::Insert( T src )
{
	//空链表就直接插入
	if( m_list.Count() <= 0 )
	{
		m_list.AddToHead(src);
		return;
	}

	int jIdx = m_list.Find(src);
	if( m_list.IsValidIndex(jIdx) )
	{
		//循环和前一个比较
		int idx = m_list.Previous(jIdx);	
		for(; m_list.IsValidIndex(idx); idx = m_list.Previous(idx))
		{
			T lister = m_list[idx];
			if( m_Lessfunc(src, lister) )
			{
				m_list.Unlink(jIdx);
				m_list.InsertAfter(idx, src);
				return;
			}
		}
		m_list.Unlink(jIdx);
		m_list.AddToHead(src);
		return;
	}
	else
	{
		if( m_list.Count() >= m_nSize )
		{
			//和末尾比较
			int tail = m_list.Tail();
			T jTail = m_list[tail];
			if( m_Lessfunc(src, jTail) )
				return;

			m_list.Unlink(tail);
		}

		int idx = m_list.Tail();	
		for(; m_list.IsValidIndex(idx); idx = m_list.Previous(idx))
		{
			T lister = m_list[idx];
			if( m_Lessfunc(src, lister) )
			{
				m_list.InsertAfter(idx, src);
				return;
			}
		}
		m_list.AddToHead(src);
		return;
	}
}

template< class T >
T COrderedList<T>::Element(int i)
{
	return m_list.Element(i);
}

template< class T >
void COrderedList<T>::RemoveAll()
{
	m_list.RemoveAll();
}
