#ifndef _CNET_OBSERVABLE_H_
#define _CNET_OBSERVABLE_H_
#include <list>
#include "types.h"

class CNetListener
{
public:
	virtual void updateSendSize(int size) = 0;
	virtual void updateRecvSize(int size) = 0;
	virtual void updateSendPacket(int num) = 0;
	virtual void updateRecvPacket(int num) = 0;
	virtual void updateSendError(int err) = 0;
	virtual void updateRecvError(int err) = 0;
	virtual void updateConnectingNum(int num) = 0;
	virtual void updateClosedNum(int num) = 0;
	virtual void updatePacketBuffNum(int64 num) = 0;
	virtual void updatePacketListNum(int64 num) = 0;
};

class CNetObservable
{
	typedef std::list<CNetListener*> LIST_LISTEN;

public:

	void regist(CNetListener *pListener) { mListeners.push_back(pListener);}
	void updateSendSize(int size) 
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updateSendSize(size);
		}
	}
	void updateRecvSize(int size) 
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updateRecvSize(size);
		}
	}
	void updateSendPacket(int num)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updateSendPacket(num);
		}
	}
	void updateRecvPacket(int num)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updateRecvPacket(num);
		}
	}
	void updateSendError(int err)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updateSendError(err);
		}
	}
	void updateRecvError(int err)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updateRecvError(err);
		}
	}
	void updateConnectingNum(int num)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end()	; ++it) {
			(*it)->updateConnectingNum(num);
		}
	}
	void updateClosedNum(int num)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updateClosedNum(num);
		}
	}
	void updatePacketBuffNum(int64 num)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updatePacketBuffNum(num);
		}
	}
	void updatePacketListNum(int64 num)
	{
		LIST_LISTEN::iterator it;
		for (it = mListeners.begin(); it != mListeners.end(); ++it) {
			(*it)->updatePacketListNum(num);
		}
	}

private:	
	LIST_LISTEN mListeners;
};

#endif
