#include "NetMonitor.h"
#include "gtime.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE	40960
#define FORMAT_MAX	100

using namespace std;

#define _K	(1024)
#define _M	(1024*1024)
#define _G	(1024*1024*1024)

#define _P_K	(1000)

void getGMKB(uint64 iNum, char *oBuff)
{
	uint64 oG , oM, oK, oB;
	oG = iNum/_G;
	uint64 temp = iNum%_G;
	oM = temp/_M; 
	temp = temp%_M;
	oK = temp/_K;
	oB = temp%_K;

	sprintf(oBuff, "%3lluG %3lluM %3lluK %3lluB", oG, oM, oK, oB);
}

void getApproximately(uint64 iNum, char *oBuff)
{
	uint64 i, r;
	char u;
	const char *format = "%3llu.%3llu%c";
	do {
		i = iNum/_G; r = iNum%_G;
		if (i > 0) {
			u = 'G';
			break;
		}
		i = iNum/_M; r = iNum%_M;
		if (i > 0) {
			u = 'M';
			break;
		}
		i = iNum/_K; r = iNum%_K;
		if (i > 0) {
			u = 'K';
			break;
		}
		i = iNum ; r = 0;
		u = 'B';
	} while(0);
	sprintf(oBuff, format, i, r, u);
}

void getPacketString(uint64 iNum, char *oBuff)
{
	uint64 i , r;

	i = iNum/_P_K; r = iNum%_P_K;

	if (i > 0) {
		sprintf(oBuff, "%3llu,%3llu", i, r);
	} else {
		sprintf(oBuff, "%3llu", r);
	}
}

CNetMonitor::CNetMonitor()
	:mSendSizeTotal(0),mRecvSizeTotal(0),mSendPacketTotal(0),mRecvPacketTotal(0),
    mSendSizeFrame(0), mRecvSizeFrame(0), mSendPacketFrame(0), mRecvPacketFrame(0),
	mConnectingNum(0), mClosedNum(0)
{
}

CNetMonitor::~CNetMonitor()
{
}

void CNetMonitor::updateSendError(int err)
{
	MAP_I2I::iterator it = mSendError.find(err);
	if (it == mSendError.end() ) {
		mSendError[err] = 1;
	} else {
		mSendError[err] += 1;
	}
}

void CNetMonitor::updateRecvError(int err)
{
	MAP_I2I::iterator it = mRecvError.find(err);
	if (it == mRecvError.end() ) {
		mRecvError[err] = 1;
	} else {
		mRecvError[err] += 1;
	}
}

void CNetMonitor::outputReport( char *output)
{
    MAP_I2I::iterator it; 
    int i = 0;
    char send_err[1024] = {0};

    for (it = mSendError.begin(); it != mSendError.end(); ++it)
    {
        sprintf(send_err + strlen(send_err), " %d: %d |" , it->first, it->second);
        ++i;
        if (i == 8) strcat(send_err, "\n");/* code */
    }

    i = 0;
    char recv_err[BUFFER_SIZE] = {0};
    for (it = mRecvError.begin(); it != mRecvError.end(); ++it)
    {
        sprintf(recv_err + strlen(recv_err), " %3d: %3d " , it->first, it->second);
        ++i;
        if (i == 8) strcat(recv_err, "\n");/* code */
    }

	//char output[BUFFER_SIZE] = {0};	
	char tSendSizeTotalBuff[FORMAT_MAX];
	char tRecvSizeTotalBuff[FORMAT_MAX];

	getGMKB(mSendSizeTotal, tSendSizeTotalBuff);
	getGMKB(mRecvSizeTotal, tRecvSizeTotalBuff);

	char tSendSizeFrame[FORMAT_MAX];
	char tRecvSizeFrame[FORMAT_MAX];

	getApproximately(mSendSizeFrame, tSendSizeFrame);
	getApproximately(mRecvSizeFrame, tRecvSizeFrame);
	
	char tSendPacketTotal[FORMAT_MAX];
	char tRecvPacketTotal[FORMAT_MAX];

	getPacketString(mSendPacketTotal, tSendPacketTotal);
	getPacketString(mRecvPacketTotal, tRecvPacketTotal);

	char tSendPacketFrame[FORMAT_MAX];
	char tRecvPacketFrame[FORMAT_MAX];

	getPacketString(mSendPacketFrame, tSendPacketFrame);
	getPacketString(mRecvPacketFrame, tRecvPacketFrame);

	sprintf(output, 
    " Net monitor: connecting %lld closed %lld\n"
	"=================================================================================\n"	
	"                 total size        total packets     avg size     avg packets         \n"
	"            =====================    ===============   ==========   =============        \n"
	" send       %s              %s           %s         %s\n"
	" recv       %s              %s           %s         %s\n\n"
    "            ---------------------------------------------------------------------\n"
	" packet buff %lld  packet list %lld\n"
    " send error    %s\n"
    " recv error    %s",
	mConnectingNum, mClosedNum,
    tSendSizeTotalBuff, tSendPacketTotal, tSendSizeFrame, tSendPacketFrame,
    tRecvSizeTotalBuff, tRecvPacketTotal, tRecvSizeFrame, tRecvPacketFrame,
	mPacketBuffNum, mPacketListNum, 
    send_err, recv_err
    );
	mSendPacketFrame = 0;
	mRecvPacketFrame = 0;
	mSendSizeFrame = 0;
	mRecvSizeFrame = 0;
}

void NetMonitorThread(void *param)
{
	CNetMonitorTData *pData = (CNetMonitorTData*)param;
	CNetMonitor *pMonitor = &pData->monitor;
	CUnixDomainServer *pUdsvr = &pData->udsvr;

	char output[BUFFER_SIZE] = {0};	

	while (true) {
		//FPS 
		sleep(1);

		if (!pUdsvr->IsConnected()) {
			continue;
		}
		//bzero(output, sizeof(output));

		pMonitor->outputReport(output);

		pUdsvr->SendToMonitor(1, output);	
	}
}

