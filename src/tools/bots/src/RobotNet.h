#pragma once

#include "shared.h"
#include "Net.h"
#include "stllist.h"

class CRobotObj;
class CRobotNet : public CNet, public Singleton< CRobotNet >
{
public:
    CRobotNet()	{};
    ~CRobotNet() {};

    bool shutdown(SOCKET sock);
    int sendMsg(int64 uid, SOCKET s, Packet* pack);
    bool sendHttpRequest(CRobotObj *robot);
    static size_t recvHttpResponse(void *buffer, size_t nsize, size_t nmemb, void *userp);
	static int getHttpVar(char* recvdata, const char* name, char* buf, int buf_len);

    bool handlePacket(Packet * pCmd);
    Packet* getHeadPacket();

private:
    Mutex m_PackLock;
    CStlList<Packet*> m_PacketList;

};

#define RobotNet CRobotNet::getSingleton()

