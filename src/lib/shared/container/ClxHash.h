#ifndef SHARED_CLXHASH_H
#define SHARED_CLXHASH_H

#define CLXHASH_DEFAULT_SIZE 1
#define CLXHASH_INVALID_KID(x) (x)(-1)  //无效值
#define CLXHASH_INVALID_TID(x) (x)(NULL)


template <typename K, typename T>
class ClxHash
{
	struct NODE_st {
		K	id;
		T	data;
		struct NODE_st *pConflictNextNode;	//冲突后向节点指针
		struct NODE_st *pConflictPrevNode;	//冲突前向节点指针

		void init()
		{
			id = CLXHASH_INVALID_KID(K);
			data = NULL;
			pConflictNextNode = NULL;
			pConflictPrevNode = NULL;
		}

		void operator=(NODE_st& node)
		{
			id = node.id;
			data = node.data;
			pConflictNextNode = node.pConflictNextNode;
			pConflictPrevNode = node.pConflictPrevNode;
		}
	};

public:
	ClxHash();
	~ClxHash();

	typedef unsigned long (__stdcall *HashFunc_t)(K);

	/*void SetHashFunc( HashFunc_t func )
	{
		m_HashFn = func;
	}*/

	void	Release();
	void	Purge(unsigned long nSize, HashFunc_t func=NULL, bool incrFlag=true);

	bool	Insert(K id, T data);
	bool	Remove(K id);
	void	RemoveAll();
	T		Find(K id);

	inline int	Count()					{return m_nNodeCount;}

protected:
	inline unsigned long HashValue(K id) {return id % m_tablelength;}

	NODE_st* FindNode(K id)
	{
		unsigned long key = 0;
		if(m_HashFn)
			key = (*m_HashFn)(id);
		else
			key = HashValue(id);

		if(m_HashIndexTable[key].id != CLXHASH_INVALID_KID(K))
		{
			NODE_st* pNode = &m_HashIndexTable[key];
			while(pNode && pNode->id != id)
				pNode = pNode->pConflictNextNode;
			return pNode;
		}

		return NULL;
	}

	bool	RemoveFromHmap(NODE_st* pNode);

protected:
	int		m_nNodeCount;			//节点个数
	bool	m_dAutoIncrease;		//哈希表大小自动扩展标记
	unsigned long m_tablelength;	//哈希散列表长度 
	NODE_st *m_HashIndexTable;		//哈希散列表指针
	unsigned long m_ConflictTablelen;//冲突列表长度
	NODE_st *m_pConflictTable;		//冲突列表指针
	HashFunc_t	m_HashFn;

};

/////////////////////////////////////////////////////////////////////////////
//

template <typename K, typename T>
ClxHash<K,T>::ClxHash()
{
	m_nNodeCount = 0;
	m_HashIndexTable = NULL;
	m_pConflictTable = NULL;
	m_HashFn = NULL;

	//Purge(CLXHASH_DEFAULT_SIZE, true);
}

template <typename K, typename T>
ClxHash<K,T>::~ClxHash()
{
	Release();
}

template <typename K, typename T>
void ClxHash<K,T>::Purge(unsigned long nSize, HashFunc_t func, bool incrFlag)
{
	m_HashFn = func;
	m_dAutoIncrease = incrFlag;
	m_tablelength = nSize;
	m_ConflictTablelen = nSize;

	//m_HashIndexTable = NEW NODE_st[m_tablelength];
	m_HashIndexTable = (NODE_st*)MemAlloc(sizeof(NODE_st)*m_tablelength);
	for(unsigned long i=0;i<m_tablelength;i++)
	{
		m_HashIndexTable[i].init();
	}
	//m_pConflictTable = NEW NODE_st[m_ConflictTablelen];
	m_pConflictTable = (NODE_st*)MemAlloc(sizeof(NODE_st)*m_tablelength);
	for(unsigned long i=0;i<m_ConflictTablelen;i++)
	{
		m_pConflictTable[i].init();
	}
}

template <typename K, typename T>
void ClxHash<K,T>::Release()
{
	//m_dAutoIncrease = true;
	m_tablelength = 0;
	m_ConflictTablelen = 0;
	m_nNodeCount = 0;

	MemFree(m_HashIndexTable);
	MemFree(m_pConflictTable);
}

template <typename K, typename T>
bool ClxHash<K,T>::Insert(K id,T data)
{
	//NODE_st *pNode = NULL;

	unsigned long key = 0;
	if(m_HashFn)
		key = (*m_HashFn)(id);
	else
		key = HashValue(id);

	if(m_HashIndexTable[key].id == CLXHASH_INVALID_KID(K))
	{
		m_HashIndexTable[key].id = id;
		m_HashIndexTable[key].data = data;
		//pNode = &m_HashIndexTable[key];
	}
	else
	{
		if(id == m_HashIndexTable[key].id)
			return false;

		unsigned long n = 0;
		while(m_pConflictTable[n].id != CLXHASH_INVALID_KID(K))
		{
			if(id == m_pConflictTable[n].id)
				return false;
			if(++n >= m_ConflictTablelen)
				return false;
		}
		m_pConflictTable[n].id = id;
		m_pConflictTable[n].data = data;
		//pNode = &m_pConflictTable[n];

		NODE_st *pConflict = &m_HashIndexTable[key];
		if ( !pConflict )
		{
			//assert(0);
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

	return true;
}

template <typename K, typename T>
bool ClxHash<K,T>::Remove(K id)
{
	NODE_st* pNode = FindNode(id);
	if(pNode)
	{
		return RemoveFromHmap(pNode);
	}

	return false;
}

template <typename K, typename T>
bool ClxHash<K,T>::RemoveFromHmap(NODE_st* pNode)
{
	if ( !pNode )
	{
		//assert(0);
		return false;
	}

	--m_nNodeCount;

	if(pNode->pConflictPrevNode && pNode->pConflictNextNode)
	{
		pNode->pConflictPrevNode->pConflictNextNode = pNode->pConflictNextNode;
		pNode->pConflictNextNode->pConflictPrevNode = pNode->pConflictPrevNode;
	}
	else if(pNode->pConflictPrevNode && !pNode->pConflictNextNode)
	{
		pNode->pConflictPrevNode->pConflictNextNode = NULL;
	}
	else if(!pNode->pConflictPrevNode && pNode->pConflictNextNode)
	{
		unsigned long key = 0;
		if(m_HashFn)
			key = (*m_HashFn)(pNode->id);
		else
			key = HashValue(pNode->id);

		NODE_st* temp = pNode->pConflictNextNode;
		m_HashIndexTable[key] = *(pNode->pConflictNextNode);
		m_HashIndexTable[key].pConflictPrevNode = NULL;
		if(m_HashIndexTable[key].pConflictNextNode)
			m_HashIndexTable[key].pConflictNextNode->pConflictPrevNode = &m_HashIndexTable[key];
		temp->init();
		return true;
	}

	pNode->init();

	return true;
}

template <typename K, typename T>
void ClxHash<K,T>::RemoveAll()
{
	for(unsigned long i=0; i<m_tablelength; i++)
	{
		m_HashIndexTable[i].init();
	}
	for(unsigned long i=0; i<m_ConflictTablelen; i++)
	{
		m_pConflictTable[i].init();
	}
	m_nNodeCount = 0;
}

template <typename K, typename T>
T ClxHash<K,T>::Find(K id)
{
	NODE_st* pNode = FindNode(id);
	if(pNode)
		return pNode->data;
	return CLXHASH_INVALID_TID(T);
}

#endif	//SHARED_CLXHASH_H

