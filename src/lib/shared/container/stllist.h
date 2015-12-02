#ifndef SHARED_STLLIST_H
#define SHARED_STLLIST_H

#include <list>

using namespace std;

template <typename T>
class CStlList
{
public:
	CStlList();
	~CStlList();

	T		Head();
	T		Tail();

	void	AddToHead(T data);
	void	AddToTail(T data);
	void	RemoveFromHead();
	void	RemoveFromTail();
	void	RemoveAll();

	size_t	Count();

private:
	std::list<T>	m_list;
};

///////////////////////////////////////////////////////////////////////////////////////
//
template <typename T>
CStlList<T>::CStlList()
{
	
}

template <typename T>
CStlList<T>::~CStlList()
{
	
}

template <typename T>
T CStlList<T>::Head()
{
	return m_list.front();
}

template <typename T>
T CStlList<T>::Tail()
{
	return m_list.back();
}

template <typename T>
void CStlList<T>::AddToTail(T data)
{
	m_list.push_back(data);
}

template <typename T>
void CStlList<T>::AddToHead(T data)
{
	m_list.push_front(data);
}

template <typename T>
void CStlList<T>::RemoveFromHead()
{
	m_list.pop_front();
}

template <typename T>
void CStlList<T>::RemoveFromTail()
{
	m_list.pop_back();
}

template <typename T>
void CStlList<T>::RemoveAll()
{
	m_list.clear();
}

template <typename T>
size_t CStlList<T>::Count()
{
	return m_list.size();
}


#endif	//SHARED_STLLIST_H
