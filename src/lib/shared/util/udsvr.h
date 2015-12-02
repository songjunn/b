#ifndef UDSVR_H
#define	UDSVR_H
#include <string>
#include <unistd.h>
#include "Singleton.h"

class CUnixDomainServer//: public Singleton<CUnixDomainServer>
{
	public:
		CUnixDomainServer();
		~CUnixDomainServer();

		void StartUp(const char *listenPath);
		
		bool IsConnected();

		void SendToMonitor(int line, const char *str);

	private:
		bool _AcceptServer();
		void _CloseClient() { close(m_clientFd); m_clientFd = -1; }
		void _CloseServer() { close(m_serverFd); m_serverFd = -1; }

	private:
		int m_serverFd;
		int m_clientFd;
};

extern CUnixDomainServer udsvr;
#endif
