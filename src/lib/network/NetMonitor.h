#ifndef __CNET_MONITOR_H_
#define __CNET_MONITOR_H_
#include <stdint.h>
#include <time.h>
#include <map>
#include <list>
#include "NetObservable.h"
#include "udsvr.h"
#include "Atomic.h"

class CNetMonitor : public CNetListener
{
public:
	CNetMonitor () ;
	virtual ~CNetMonitor();

	void updateSendSize(int size) { mSendSizeTotal += size; mSendSizeFrame += size;}
	void updateRecvSize(int size) { mRecvSizeTotal += size; mRecvSizeFrame += size;}

	void updateSendPacket(int num) { mSendPacketTotal += num; mSendPacketFrame += num;}
	void updateRecvPacket(int num) { mRecvPacketTotal += num; mRecvPacketFrame += num;}

	void updateSendError(int err);
	void updateRecvError(int err);

	void updateConnectingNum(int num) { g_lockedExchangeAdd(&mConnectingNum, num);}
	void updateClosedNum(int num) { g_lockedExchangeAdd(&mClosedNum, num);}

	void updatePacketBuffNum(int64 num) { mPacketBuffNum = num;}
	void updatePacketListNum(int64 num) { mPacketListNum = num;}

	void outputReport(char *output);
private:
	typedef std::map<int, int> MAP_I2I;

	MAP_I2I mRecvError;
	MAP_I2I mSendError;

	uint64 mSendSizeTotal;
	uint64 mRecvSizeTotal;

	uint64 mSendPacketTotal;
	uint64 mRecvPacketTotal;

	uint64 mSendSizeFrame;
	uint64 mRecvSizeFrame;

	uint64 mSendPacketFrame;
	uint64 mRecvPacketFrame;

	volatile LONG mConnectingNum;
	volatile LONG mClosedNum;

	uint64 mPacketBuffNum;
	uint64 mPacketListNum;
};

class CNetMonitorTData
{
	public:
		CNetMonitorTData(){}
		~CNetMonitorTData(){}
		void init(const char *path){ udsvr.StartUp(path);}
	public:
		CNetMonitor monitor;
		CUnixDomainServer udsvr;
};

void NetMonitorThread(void  *p);

#endif
