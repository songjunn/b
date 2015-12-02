/*
 * =====================================================================================
 *
 *       Filename:  stringreference.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Monday, May 06, 2013 04:01:06 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RAPIDJSON_STRINGREFERENCE_H_
#define RAPIDJSON_STRINGREFERENCE_H_

#include <string>
using namespace std;

#include "rapidjson.h"

namespace rapidjson {

//! Represents an in-memory output stream.
/*!
	\tparam Encoding Encoding of the stream.
	\tparam Allocator type for allocating memory buffer.
	\implements Stream
*/
template <typename Encoding>
struct GenericStringReference{
	typedef typename Encoding::Ch Ch;

	GenericStringReference(string *pStr) : m_pStackStr(pStr) {}

	void Put(Ch c) { 
		m_pStackStr->push_back(c); 
	}

	void Clear() { m_pStackStr->clear(); }

	const string* GetString() const {
		// Push and pop a null terminator. This is safe.
		m_pStackStr->push_back('\0'); 

		return m_pStackStr;
	}

	size_t Size() const { return m_pStackStr->size(); }

	string *m_pStackStr;
};

typedef GenericStringReference<UTF8<> > StringReference;

//! Implement specialized version of PutN() with memset() for better performance.
template<>
inline void PutN(GenericStringReference<UTF8<> >& stream, char c, size_t n) {
	stream.m_pStackStr->assign(c, n * sizeof(c));
}

} // namespace rapidjson

#endif // RAPIDJSON_STRINGBUFFER_H_
