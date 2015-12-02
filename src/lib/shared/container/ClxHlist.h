#ifndef SHARED_CLXHLIST_H
#define SHARED_CLXHLIST_H

#include <unistd.h>
#include "types.h"
#include "memory.h"
#include "logger.h"

#define DEFAULT_SIZE 1
#define INVALID_KID(x) (x)(-1)  //无效值
#define INVALID_TID(x) (x)(0)


template <typename K, typename T, typename I = unsigned long>
class ClxHList
{
	struct NODE_st {
		K	id;
		T	data;
		struct NODE_st *pNextNode;		//链表的后向节点指针
		struct NODE_st *pPrevNode;		//链表的前向节点指针
		struct NODE_st *pConflictNextNode;	//冲突后向节点指针
		struct NODE_st *pConflictPrevNode;	//冲突前向节点指针

		void init()
		{
			id = INVALID_KID(K);
			data = 0;
			pNextNode = 0;
			pPrevNode = 0;
			pConflictNextNode = 0;
			pConflictPrevNode = 0;
		}

		void operator=(NODE_st& node)
		{
			id = node.id;
			data = node.data;
			pNextNode = node.pNextNode;
			pPrevNode = node.pPrevNode;
			pConflictNextNode = node.pConflictNextNode;
			pConflictPrevNode = node.pConflictPrevNode;
		}
	};

public:
	ClxHList();
	~ClxHList();

	typedef I (__stdcall *HashFunc_t)(K);

	/*void SetHashFunc( HashFunc_t func )
	{
		m_HashFn = func;
	}*/

	void	Release();
	void	Purge(I nSize, HashFunc_t func=0, bool incrFlag=true);

	bool	Insert(K id, T data);
	bool	Remove(K id);
	void	RemoveAll();
	T		Find(K id);

	K		Head();
	K		Tail();
	K		Next(K id);
	K		Prev(K id);

	void	LinkToHead(K id);
	void	LinkToTail(K id);

	bool	IsListTail(K id);
	inline bool	IsInvalid(K id)			{return id != INVALID_KID(K);}

	inline int	Count()					{return m_nNodeCount;}

protected:
	inline I HashValue(K id) {return id % m_tablelength;}
	inline I GetHashValue(K id)
	{
		if(m_HashFn)
			return (*m_HashFn)(id);
		else
			return HashValue(id);
	}

	NODE_st* FindNode(K id)
	{
		if(m_tablelength <= 0)
			return 0;

		if(!IsInvalid(id))
			return 0;

		I key = GetHashValue(id);

		if(m_HashIndexTable[key].id != INVALID_KID(K))
		{
			NODE_st* pNode = &m_HashIndexTable[key];
			while(pNode && pNode->id != id)
				pNode = pNode->pConflictNextNode;
			return pNode;
		}

		return 0;
	}

	bool	RemoveFromHmap(NODE_st* pNode);
	bool	RemoveFromList(NODE_st* pNode);

protected:
	int		m_nNodeCount;			//节点个数
	bool	m_dAutoIncrease;		//哈希表大小自动扩展标记
	I		m_tablelength;			//哈希散列表长度 
	NODE_st *m_HashIndexTable;		//哈希散列表指针
	I		m_ConflictTablelen;		//冲突列表长度
	NODE_st *m_pConflictTable;		//冲突列表指针
	NODE_st *m_pListHead;			//链表的头指针
	NODE_st *m_pListTail;			//链表的尾指针
	HashFunc_t	m_HashFn;

};

/////////////////////////////////////////////////////////////////////////////
//

template <typename K, typename T, typename I>
ClxHList<K,T,I>::ClxHList()
{
	m_nNodeCount = 0;
	m_HashIndexTable = 0;
	m_pConflictTable = 0;
	m_pListHead = 0;
	m_pListTail = 0;
	m_HashFn = 0;

	//Purge(DEFAULT_SIZE, true);
}

template <typename K, typename T, typename I>
ClxHList<K,T,I>::~ClxHList()
{
	Release();
}

template <typename K, typename T, typename I>
void ClxHList<K,T,I>::Purge(I nSize, HashFunc_t func, bool incrFlag)
{
	m_HashFn = func;
	m_dAutoIncrease = incrFlag;
	m_tablelength = nSize;
	m_ConflictTablelen = nSize;

	//m_HashIndexTable = NEW NODE_st[m_tablelength];
	m_HashIndexTable = (NODE_st*)MemAlloc(sizeof(NODE_st)*m_tablelength);
	for(I i=0;i<m_tablelength;i++)
	{
	  //new(&m_HashIndexTable[i]) NODE_st;
		m_HashIndexTable[i].init();
	}
	//m_pConflictTable = NEW NODE_st[m_ConflictTablelen];
	m_pConflictTable = (NODE_st*)MemAlloc(sizeof(NODE_st)*m_tablelength);
	for(I i=0;i<m_ConflictTablelen;i++)
	{
	  //new(&m_pConflictTable[i]) NODE_st;
		m_pConflictTable[i].init();
	}
}

template <typename K, typename T, typename I>
void ClxHList<K,T,I>::Release()
{
	//m_dAutoIncrease = true;
	m_tablelength = 0;
	m_ConflictTablelen = 0;
	m_nNodeCount = 0;
	m_pListHead = 0;
	m_pListTail = 0;

	MemFree(m_HashIndexTable);
	MemFree(m_pConflictTable);
}

template <typename K, typename T, typename I>
bool ClxHList<K,T,I>::Insert(K id,T data)
{
	if(m_tablelength <= 0)
	{
		LOGGER_ERROR("[ClxHList]m_tablelength<=0");
		return false;
	}

	NODE_st *pNode = 0;

	I key = GetHashValue(id);

	if(m_HashIndexTable[key].id == INVALID_KID(K))
	{
		m_HashIndexTable[key].id = id;
		m_HashIndexTable[key].data = data;
		pNode = &m_HashIndexTable[key];
	}
	else
	{
		if(id == m_HashIndexTable[key].id)
		{
			LOGGER_ERROR("[ClxHList]m_HashIndexTable key repeat:"INT64_FMT, id);
			return false;
		}

		I n = 0;
		while(m_pConflictTable[n].id != INVALID_KID(K))
		{
			if(id == m_pConflictTable[n].id)
			{
				LOGGER_ERROR("[ClxHList]m_pConflictTable key repeat:"INT64_FMT, id);
				return false;
			}
			if(++n >= m_ConflictTablelen)
			{
				LOGGER_ERROR("[ClxHList]m_ConflictTablelen <= %d:", n);
				return false;
			}
		}
		m_pConflictTable[n].id = id;
		m_pConflictTable[n].data = data;
		pNode = &m_pConflictTable[n];

		NODE_st *pConflict = &m_HashIndexTable[key];
		if ( !pConflict )
		{
			LOGGER_ERROR("[ClxHList]m_HashIndexTable[%d] is null, id:"INT64_FMT, key, id);
			return false;
		}
		while(pConflict->pConflictNextNode)
		{
			pConflict = pConflict->pConflictNextNode;
		}
		pConflict->pConflictNextNode = &m_pConflictTable[n];
		m_pConflictTable[n].pConflictPrevNode = pConflict;

		//Log("insert conflict at %d, %I64d\n", key, id);
	}

	++m_nNodeCount;

	if(!m_pListHead)
	{
		m_pListHead = pNode;
		m_pListTail = pNode;
		//pNode->pNextNode = pNode;
		//pNode->pPrevNode = pNode;
	}
	else
	{
		//m_pListHead->pPrevNode = pNode;
		m_pListTail->pNextNode = pNode;
		pNode->pPrevNode = m_pListTail;
		m_pListTail = pNode;
	}
	return true;
}

template <typename K, typename T, typename I>
bool ClxHList<K,T,I>::Remove(K id)
{
	NODE_st* pNode = FindNode(id);
	if(pNode)
	{
		//先从list删除，再从map删除，RemoveFromHmap中做了节点数据的初始化
		RemoveFromList(pNode);

		RemoveFromHmap(pNode);

		--m_nNodeCount;

		return true;
	}

	return false;
}

template <typename K, typename T, typename I>
bool ClxHList<K,T,I>::RemoveFromHmap(NODE_st* pNode)
{
	if(!pNode || m_tablelength <= 0)
	{
		//assert(0);
		return false;
	}
	if(pNode->pConflictPrevNode && pNode->pConflictNextNode)
	{
		pNode->pConflictPrevNode->pConflictNextNode = pNode->pConflictNextNode;
		pNode->pConflictNextNode->pConflictPrevNode = pNode->pConflictPrevNode;
	}
	else if(pNode->pConflictPrevNode && !pNode->pConflictNextNode)
	{
		pNode->pConflictPrevNode->pConflictNextNode = 0;
	}
	else if(!pNode->pConflictPrevNode && pNode->pConflictNextNode)
	{
		I key = GetHashValue(pNode->id);

		NODE_st* temp = pNode->pConflictNextNode;
		m_HashIndexTable[key] = *(pNode->pConflictNextNode);
		m_HashIndexTable[key].pConflictPrevNode = 0;
		if(m_HashIndexTable[key].pConflictNextNode)
			m_HashIndexTable[key].pConflictNextNode->pConflictPrevNode = &m_HashIndexTable[key];

		//修改链表
		if(m_HashIndexTable[key].pNextNode)
		{
			m_HashIndexTable[key].pNextNode->pPrevNode = &m_HashIndexTable[key];
		}
		else
		{
			m_pListTail = &m_HashIndexTable[key];
		}
		if(m_HashIndexTable[key].pPrevNode)
		{
			m_HashIndexTable[key].pPrevNode->pNextNode = &m_HashIndexTable[key];
		}
		else
		{
			m_pListHead = &m_HashIndexTable[key];
		}

		temp->init();
		return true;
	}

	pNode->init();

	return true;
}

template <typename K, typename T, typename I>
bool ClxHList<K,T,I>::RemoveFromList(NODE_st* pNode)
{
	if ( !pNode )
	{
		//assert(0);
		return false;
	}
	//head & tail
	if(m_pListHead == pNode && m_pListTail == pNode)
	{
		m_pListHead = 0;
		m_pListTail = 0;
		return true;
	}

	if(m_pListHead == pNode)
	{
		m_pListHead = pNode->pNextNode;
	}
	if(m_pListTail == pNode)
	{
		m_pListTail = pNode->pPrevNode;
	}
	
	if(pNode->pNextNode)
	{
		pNode->pNextNode->pPrevNode = pNode->pPrevNode;
	}
	if(pNode->pPrevNode)
	{
		pNode->pPrevNode->pNextNode = pNode->pNextNode;
	}
	
	return true;
}

template <typename K, typename T, typename I>
void ClxHList<K,T,I>::RemoveAll()
{
	for(I i=0; i<m_tablelength; i++)
	{
		m_HashIndexTable[i].init();
	}
	for(I i=0; i<m_ConflictTablelen; i++)
	{
		m_pConflictTable[i].init();
	}
	m_pListHead = NULL;
	m_pListTail = NULL;
	m_nNodeCount = 0;
}

template <typename K, typename T, typename I>
T ClxHList<K,T,I>::Find(K id)
{
	NODE_st* pNode = FindNode(id);
	if(pNode)
		return pNode->data;
	return INVALID_TID(T);
}

template <typename K, typename T, typename I>
K ClxHList<K,T,I>::Head()
{
	if(m_pListHead)
		return m_pListHead->id;
	return INVALID_KID(K);
}

template <typename K, typename T, typename I>
K ClxHList<K,T,I>::Tail()
{
	if(m_pListTail)
		return m_pListTail->id;
	return INVALID_KID(K);
}

template <typename K, typename T, typename I>
K ClxHList<K,T,I>::Next(K id)
{
	NODE_st* pNode = FindNode(id);

	if(pNode && pNode->pNextNode)
	{
		return pNode->pNextNode->id;
	}

	return INVALID_KID(K);
}

template <typename K, typename T, typename I>
K ClxHList<K,T,I>::Prev(K id)
{
	NODE_st* pNode = FindNode(id);

	if(pNode && pNode->pPrevNode)
	{
		return pNode->pPrevNode->id;
	}

	return INVALID_KID(K);
}

template <typename K, typename T, typename I>
void ClxHList<K,T,I>::LinkToHead(K id)
{
	NODE_st* pNode = FindNode(id);

	if (!pNode || pNode == m_pListHead)
		return;

	RemoveFromList(pNode);

	pNode->pNextNode = m_pListHead;
	pNode->pPrevNode = 0;
	m_pListHead = pNode;
}

template <typename K, typename T, typename I>
void ClxHList<K,T,I>::LinkToTail(K id)
{
	NODE_st* pNode = FindNode(id);

	if (!pNode || pNode == m_pListTail)
		return;

	RemoveFromList(pNode);

	pNode->pNextNode = 0;
	pNode->pPrevNode = m_pListTail;
	m_pListTail = pNode;
}


template <typename K, typename T, typename I>
bool ClxHList<K,T,I>::IsListTail(K id)
{
	NODE_st* pNode = FindNode(id);

	if(pNode)
	{
		return pNode == m_pListTail;
	}

	return false;
}


#endif	//SHARED_CLXHLIST_H
