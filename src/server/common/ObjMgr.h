#ifndef OBJMMGR_H
#define OBJMMGR_H

#include "types.h"
#include "ClxHlist.h"
#include "memorypool.h"
#include "logger.h"

template < class T, class I = int >
class CObjMgr
{
public:
	CObjMgr();
	~CObjMgr();

	bool	Initialize(const char* name, int num);

	T*		Create(I key);
	void	Delete(I key, T* obj = NULL);

	T*		GetObj(I key);

	void	DeleteAll();

	inline int	Count()		{return m_list.Count();}

public:
	ClxHList<I, T*>			m_list;
	CObjectMemoryPool< T >	m_pool;

};

template < class T, class I >
CObjMgr<T, I>::CObjMgr()
{
}

template < class T, class I >
CObjMgr<T, I>::~CObjMgr()
{
}

template < class T, class I >
bool CObjMgr<T, I>::Initialize(const char* name, int num)
{
	m_list.Purge(num);
	return m_pool.Init(name, num);
}

template < class T, class I >
T* CObjMgr<T, I>::Create(I key)
{
	T* obj = m_pool.Alloc();
	if( !obj )
	{
		LOGGER_ERROR("%s m_pool.Alloc", m_pool.GetName());
		m_pool.MemLog();
		return NULL;
	}

	if( !m_list.Insert(key, obj) )
	{
		LOGGER_ERROR("%s m_list.Insert", m_pool.GetName());
		m_pool.MemLog();
		m_pool.Free(obj);
		return NULL;
	}

	return obj;
}

template < class T, class I >
void CObjMgr<T, I>::Delete(I key, T* obj)
{
	T* p = (obj == NULL ? GetObj(key) : obj);
	if( p != NULL )
	{
		m_list.Remove(key);
		m_pool.Free(p);
	}
}

template < class T, class I >
T* CObjMgr<T, I>::GetObj(I key)
{
	return m_list.Find(key);
}

template < class T, class I >
void CObjMgr<T, I>::DeleteAll()
{
	m_pool.FreeAll();
	m_list.RemoveAll();
}

#endif	//OBJMMGR_H
