#ifndef SHARED_STLMAP_H
#define SHARED_STLMAP_H

#include <map>

using namespace std;

template <typename K, typename T>
class CStlMap
{
public:
	CStlMap();
	~CStlMap();

	bool	Insert(K id, T data);
	T		Remove(K id);
	void	RemoveAll();
	T		Find(K id);

	int		Count();

private:
	std::map<K, T>	m_map;
};

///////////////////////////////////////////////////////////////////////////////////////
//
template <typename K, typename T>
CStlMap<K, T>::CStlMap()
{
	
}

template <typename K, typename T>
CStlMap<K, T>::~CStlMap()
{
	
}

template <typename K, typename T>
bool CStlMap<K, T>::Insert(K id, T data)
{
	pair<typename std::map<K, T>::iterator, bool> Insert_Pair;
	Insert_Pair = m_map.insert(pair<K, T>(id, data));
	return Insert_Pair.second;
}

template <typename K, typename T>
T CStlMap<K, T>::Find(K id)
{
	typename map<K, T>::iterator iter;
	iter = m_map.find(id);
	if( iter != m_map.end() )
		return iter->second;
	else
		return NULL;
}

template <typename K, typename T>
T CStlMap<K, T>::Remove(K id)
{
	typename map<K, T>::iterator iter;
	iter = m_map.find(id);
	if( iter == m_map.end() )
		return false;
	T elem = iter->second;
	m_map.erase(iter);
	return elem;
}

template <typename K, typename T>
void CStlMap<K, T>::RemoveAll()
{
	m_map.clear();
}

template <typename K, typename T>
int CStlMap<K, T>::Count()
{
	return m_map.size();
}


#endif	//SHARED_STLMAP_H
