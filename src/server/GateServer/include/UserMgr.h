#ifndef GATESERVER_USERMGR_H
#define GATESERVER_USERMGR_H

#include "commdata.h"
#include "Net.h"
#include "Packet.h"
#include "ObjMgr.h"
#include "ClxHlist.h"
#include "Singleton.h"

struct CUser {
    UserID m_id;
    int	m_worldID;
    int	m_svrID;
    SOCKET m_ClientSock;
    SOCKET m_GameSock;
    PersonID m_LogonPlayer;
    TMV	m_HeartTime;
    bool m_CanCreate;
    int	m_PackCount;
    TMV	m_PackTime;
    std::string m_AccessToken;

    ~CUser() {
        m_AccessToken.clear();
    }
};

class CUserMgr : public CObjMgr<CUser, SOCKET>, public Singleton<CUserMgr>
{
public:
    CUserMgr();
    ~CUserMgr();

    void InitConfig(int hearttime, int packlimit = 0);

    bool OnMsg(Packet* pack);
    bool OnLogic();

    void Exit(CUser* user);
    void Displace(CUser* user);	//顶号
    void RemoveUser(CUser* user, bool sync = true);
    bool UserPacketLimit(CUser* user);

    void SendHeartResponse(CUser* user);
    void SendErrorMsg(SOCKET sock, int errid);

    inline CUser* GetUserByUID(UserID uid) 
    {
	m_UserLock.LOCK();
	CUser* user = m_UserList.Find(uid);
	m_UserLock.UNLOCK();
	return user;
    }

protected:
    void _UserHeartLogic();

    bool _HandlePacket_UserHeart(Packet* pack);
    bool _HandlePacket_UserLogin(Packet* pack);
    bool _HandlePacket_UserLogout(Packet* pack);
    bool _HandlePacket_UserDisplace(Packet* pack);
    bool _HandlePacket_PlayerLogin(Packet* pack);
    bool _HandlePacket_PlayerCreate(Packet* pack);
    bool _HandlePacket_PlayerCount(Packet* pack);
    bool _HandlePacket_NetClose(Packet* pack);

    static void httpCheckUserThread(void *pParam);
    static size_t recvBackData(void *buffer, size_t nsize, size_t nmemb, void *userp);

protected:
    int	m_HeartTimeout;  //心跳超时时间.
    //int m_PacketLimit; //平局每秒的收包限制

    Mutex m_UserLock;
    CStlMap<UserID, CUser*> m_UserList;
};

#define UserMgr CUserMgr::getSingleton()

#endif	//GATESERVER_USERMGR_H
