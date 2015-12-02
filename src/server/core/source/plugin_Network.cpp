#include "plugin_Network.h"
#include "server.h"
#include "memorypool.h"
#include "MessageTypeDefine.pb.h"
#include "MessageServer.pb.h"

CNetwork::CNetwork()
{
}

CNetwork::~CNetwork()
{
}

bool CNetwork::startup(int type, int port, int connectmax, int sendbuffsize, int recvbuffsize, int packsize)
{
	if (!m_server->getPacketQueue().Init(packsize)) {
		return false;
	}

	return CNet::startup(type, port, connectmax, sendbuffsize, recvbuffsize, packsize);
}

bool CNetwork::handlePacket(Packet * pCmd)
{
	return m_server->getPacketQueue().Push(pCmd);
}

void CNetwork::_closeReturn(SOCKET sock)
{
	Packet *pCmd = new Packet;
	if (pCmd) {
		Message::NetControl tMsg;
		tMsg.set_sock(sock);
		PROTOBUF_CMD_PACKAGE((*pCmd), tMsg, Message::MSG_SERVER_NET_CLOSE);
		handlePacket(pCmd);
	}
}

void CNetwork::_acceptReturn(SOCKET sock)
{
	Packet *pCmd = new Packet;
	if (pCmd) {
		Message::NetControl tMsg;
		tMsg.set_sock(sock);
		PROTOBUF_CMD_PACKAGE((*pCmd), tMsg, Message::MSG_SERVER_NET_ACCEPT);
		handlePacket(pCmd);
	}
}

void CNetwork::_connectReturn(SOCKET sock, int error)
{
	Packet *pCmd = new Packet;
	if (pCmd) {
		Message::NetControl tMsg;
		tMsg.set_sock(sock);
		tMsg.set_error(error);
		PROTOBUF_CMD_PACKAGE((*pCmd), tMsg, Message::MSG_SERVER_NET_CONNECT);
		handlePacket(pCmd);
	}
}
