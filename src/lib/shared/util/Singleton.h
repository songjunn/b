/*
 * 整理自Ascent MMORPG Server
 *
 *				songjun 2012.12.14
 */

#ifndef SHARED_SINGLETON_H
#define SHARED_SINGLETON_H

/// Should be placed in the appropriate .cpp file somewhere, then new an instance
#define initialiseSingleton( type ) \
  template <> type * Singleton < type > :: m_Singleton = 0

/// To be used as a replacement for initialiseSingleton( )
///  Creates a file-scoped Singleton object, to be retrieved with getSingleton
#define createFileSingleton( type ) \
  initialiseSingleton( type ); \
  type the##type

template < class type > class Singleton 
{
public:
	Singleton() 
	{
		m_Singleton = static_cast<type *>(this);
	}
	virtual ~Singleton() 
	{
		m_Singleton = 0;
	}

	__inline static type & getSingleton() { return *m_Singleton; }
	__inline static type * getSingletonPtr() { return m_Singleton; }

protected:
	/// Singleton pointer, must be set to 0 prior to creating the object
	static type * m_Singleton;

};

#endif	//SHARED_SINGLETON_H
