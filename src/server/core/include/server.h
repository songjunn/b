#ifndef _SERVER_H
#define _SERVER_H

#include "types.h"
#include "utlmap.h"
#include "utllinkedlist.h"
#include "utldoublequeue.h"
#include "Packet.h"
#include "plugin_Mongodb.h"
#include "plugin_HttpServe.h"
#include "plugin_Network.h"
#include "plugin_Monitor.h"

enum EServerState
{
	EStateRunning = 0,
	EStateStopping ,
	EStateLoading,
	EStateReloadScript,
	EStateReloadData
};

#define GETSERVERNET(Server)	((CNetwork *)(Server)->getPlugin(CBaseServer::Plugin_Net4Server))
#define GETCLIENTNET(Server)	((CNetwork *)(Server)->getPlugin(CBaseServer::Plugin_Net4Client))
#define GETMONGODB(Server)		((CMongoDB *)(Server)->getPlugin(CBaseServer::Plugin_Mongodb))
#define GETHTTPSERVE(Server)	((CHttpServe *)(Server)->getPlugin(CBaseServer::Plugin_HttpServe))

struct CLinker {
	SOCKET	m_Socket;
	bool	m_bBreak;
	bool	m_bWaiting;
	int		m_type;
	int		m_nID;
	char	m_szIP[32];
	int		m_nPort;
	bool	m_bHost;
	long	m_LostTime;
	int		m_count;
	char	m_extIP[32];
	int		m_extPort;
	int		m_worldID;

	CLinker() {
		m_Socket = INVALID_SOCKET;
		m_bBreak = true;
		m_bWaiting = false;
		m_type = -1;
		m_nID = -1;
		m_szIP[0] = '\0';
		m_nPort = -1;
		m_bHost = false;
		m_LostTime = -1;
		m_count = 0;
		m_extIP[0] = '\0';
		m_extPort = -1;
		m_worldID = -1;
	}
};

class CPlugin;
class CBaseServer
{
public:
	//连接类型
	enum LinkerType {
		Linker_Server_Central = 1,	//中央服务器
		Linker_Server_Data,			//数据服务器
		Linker_Server_World,		//世界服务器
		Linker_Server_Login,		//登陆服务器
		Linker_Server_Game,			//逻辑服务器
		Linker_Server_GateWay,		//网关服务器
		Linker_Server_DataAnalysis,	//数据分析服务器
		Linker_Server_Payment,
		Linker_Server_End,
		Linker_User,				//玩家
	};

	//插件种类
	enum Plugin_Type {
		Plugin_Net4Server = 0,
		Plugin_Net4Client,
		Plugin_Mongodb,
		Plugin_HttpServe,
		Plugin_Monitor,

		Plugin_End,
	};

public:
	CBaseServer();
	virtual ~CBaseServer();

	bool run(string logconf, string logfile, int frame = 0);
	void setType(int type);
	void initSelf(int world, int type, int id, int port, const char* szip, int extport = 0, const char* extip = NULL, const char* udPath = NULL);
	CLinker* createLinker(int type, int id, int port, const char* szip, int extport, const char* extip, int world, bool host, SOCKET sock = INVALID_SOCKET);
	CLinker* createLinker(CLinker* linker, int type, int id, int port, const char* szip, int extport, const char* extip, int world, bool host, SOCKET sock = INVALID_SOCKET);

	virtual bool onStartup(string logconf, string logfile);
	virtual bool onLogic();
	virtual void onShutdown();
	virtual bool onMessage(Packet* pack);
	virtual void onPrint(char* output);
	virtual bool onLoadData() { return true; }
	virtual bool onLoadScript() { return true; }

	CPlugin* createPlugin(int plugin);
	CPlugin* getPlugin(int plugin) { return m_plugins[plugin]; }

	CLinker* getLinker(SOCKET s);
	CLinker* getServer(SOCKET s);
	CLinker* getServerById(int id);
	CLinker* getServerByType(int type);
	SOCKET getServerSock(int type);

	inline int getSelfWorld() { return m_self.m_worldID; }

	static bool isServer(int type) { return type >= Linker_Server_Central && type < Linker_Server_End; }
	static bool isUser(int type) { return type == Linker_User; }

	inline Packet* getHeadPacket() { return m_PacketQueue.Pop(); }
	inline CDoubleQueue<Packet>& getPacketQueue() { return m_PacketQueue; }
	inline CUtlLinkedList<CLinker*>& getLinkerList() { return m_linkerList; }

protected:
	// handle signals
	void setServerState(EServerState state) { m_ServerState = state;}
	EServerState getServerState() { return m_ServerState; }
	void sigHandle(int sig);
	bool startupSigThread();
	static void SignalThread(void *param);

	// create and delete servers
	bool addLinker(CLinker* linker);
	void breakLinker(SOCKET s);
	void deleteLinker(CLinker* pObj);

	virtual bool _onAddLinker(CLinker* pServer) { return true; }

	// regist servers
	bool regist(CLinker* pObj);
	bool registAsync(CLinker* pObj);
	bool registAsyncReturn(SOCKET sock, int error = 0);
	void syncLoadNumber(int socket, int number);

	bool loop_linkers();
	bool loop_message();

	bool _handlePacket_NetAccept(Packet* pack);
	bool _handlePacket_NetClose(Packet* pack);
	bool _handlePacket_RegistServer(Packet* pack);
	bool _handlePacket_ConnectServer(Packet* pack);
	bool _handlePacket_RegistAsyncReturn(Packet* pack);
	bool _handlePacket_SyncLoadNumber(Packet* pack);

private:
	CLinker m_self;
	EServerState m_ServerState;
	CPlugin* m_plugins[Plugin_End];

	CUtlMap<SOCKET, CLinker*> m_linkerMap;
	CUtlLinkedList<CLinker*> m_linkerList;
	CDoubleQueue<Packet> m_PacketQueue;

};

#endif	//_SERVER_H
