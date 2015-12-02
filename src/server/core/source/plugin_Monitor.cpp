#include "plugin_Monitor.h"
#include "ThreadLib.h"
#include "server.h"

#define MONITRO_BUFFER_SIZE	40960

CMonitor::CMonitor()
{
}

CMonitor::~CMonitor()
{
}

bool CMonitor::startup(const char *path)
{
	m_udsvr.StartUp(path);
	ThreadLib::CreateByPool(MonitorThread, this);

	return true;
}

void CMonitor::MonitorThread(void *param)
{
	CMonitor *pData = (CMonitor*)param;
	char output[MONITRO_BUFFER_SIZE] = {0};	

	while (true) {
		//FPS 
		sleep(1);

		if (!pData->m_udsvr.IsConnected()) {
			continue;
		}

		pData->m_server->onPrint(output);
		pData->m_udsvr.SendToMonitor(1, output);
	}
}
