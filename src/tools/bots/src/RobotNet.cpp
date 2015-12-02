#include "RobotNet.h"
#include "Packet.h"
#include "RobotObj.h"
#include "curl.h"
#include "MessageTypeDefine.pb.h"
#include "MessageUser.pb.h"

extern char	g_szServerIp[32];
extern int g_nServerPort;

bool CRobotNet::shutdown(SOCKET sock)
{
    return CNet::shutdown(sock);
}

int CRobotNet::sendMsg(int64 uid, SOCKET s, Packet* pack)
{
    pack->SetTrans(uid);

    return CNet::send(s, (char*)pack->Data(), pack->Size());
}

bool CRobotNet::handlePacket(Packet * pack)
{
    m_PackLock.LOCK();
    m_PacketList.AddToTail(pack);
    m_PackLock.UNLOCK();

    return true;
}

Packet*	 CRobotNet::getHeadPacket()
{
    Packet* pack = NULL;
    m_PackLock.LOCK();
    if (m_PacketList.Count() > 0) {
        pack = m_PacketList.Head();
        m_PacketList.RemoveFromHead();
    }
    m_PackLock.UNLOCK();
    return pack;
}

bool CRobotNet::sendHttpRequest(CRobotObj *robot)
{
	char postStr[128], authUrl[128];
	snprintf(postStr, sizeof(postStr), "action=1&platform=0&guest=%s", robot->GetUserName());
	snprintf(authUrl, sizeof(authUrl), "119.29.22.222:1313");

    CURL *pUrl = curl_easy_init();
    curl_easy_setopt(pUrl, CURLOPT_URL, authUrl);
    curl_easy_setopt(pUrl, CURLOPT_POSTFIELDS, postStr);
    curl_easy_setopt(pUrl, CURLOPT_WRITEFUNCTION, recvHttpResponse);
    curl_easy_setopt(pUrl, CURLOPT_WRITEDATA, robot);
    curl_easy_setopt(pUrl, CURLOPT_POST, 1);
    curl_easy_setopt(pUrl, CURLOPT_NOSIGNAL, 1);
    //curl_easy_setopt(pUrl, CURLOPT_VERBOSE, 1);
    //curl_easy_setopt(pUrl, CURLOPT_HEADER, 1);
    //curl_easy_setopt(pUrl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(pUrl, CURLOPT_COOKIEFILE, "curlauth.cookie");
    CURLcode res = curl_easy_perform(pUrl);
    curl_easy_cleanup(pUrl);

    return (res == 0);
}

size_t CRobotNet::recvHttpResponse(void *buffer, size_t nsize, size_t nmemb, void *userp)
{
    CRobotObj *robot = (CRobotObj *)userp;
    char *recvdata = (char *)buffer;
	
	char uid[128], accesstoken[128];
	getHttpVar(recvdata, "uid", uid, sizeof(uid));
	getHttpVar(recvdata, "accesstoken", accesstoken, sizeof(accesstoken));

	robot->m_uid = atoll(uid);
	robot->m_sock = RobotNet.connect(g_szServerIp, g_nServerPort);

	Message::ClientLogin msg;
	msg.set_uid(robot->m_uid);
	msg.set_accesstoken(accesstoken);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_USER_LOGIN);
	RobotNet.sendMsg(robot->m_uid, robot->m_sock, &pack);

    return nsize * nmemb;
}

int CRobotNet::getHttpVar(char* recvdata, const char* name, char* buf, int buf_len) 
{
	char *begin_word;
	int value_len;

	begin_word = strstr(recvdata, name) + strlen(name) + 1; // exclude '='
	value_len = strcspn(begin_word, "&");
	if (buf_len > value_len) {
		strncpy(buf, begin_word, value_len);
		buf[value_len] = '\0';
	}
	return value_len;
}


