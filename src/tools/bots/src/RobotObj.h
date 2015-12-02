#pragma once
#include "types.h"
#include "Net.h"

enum ERobotStat {
    ESTAT_NONE	= 0,
    ESTAT_LOGINGATE = 100,
};

class CRobotObj
{
public:
    CRobotObj(int index, const char* name, const char* pwd, int actionTime, int world);
    ~CRobotObj();

    inline char* GetUserName() {return m_szName;}
    inline char* GetUserPwd() {return m_szPwd;}
    inline SOCKET GetSocket() {return m_sock;}

    inline void SetUid(int64 id){m_uid = id;}
    inline void SetKey(int64 key){m_key = key;}

    void setStat(ERobotStat stat) {m_Stat = stat;}
    ERobotStat getStat() {return m_Stat;}
	
    //player
    inline int64 GetPlayerID() {return m_PlayerID;}
	
	bool	OnMsg( Packet* pPacket );
	bool	Logic();
	bool	DoActiveBehavior(int type);

	bool	Check(const char* szip, int port, const char* ver);
	bool	Logon(const char* szip, int port);
	void	Logout();
	void	Heart();

	void	CreatePlayer(int64 uid, int server, const char* name, int vocation, int sex, int camp);

	void	AttackMonster(int64 pid, int scene, int monster);
	void	ActiveCity(int64 pid, int scene);
	void	AddExp(int exp);
	void	AddItem();
	void	AddHero();
	void	SendChat();
	void	ActiveStar();
	void	UplevelStar();
	void	RequestObserver();
	void	RequestJoinBattle();
	void	RequestAttackBoss();
	void	RequestHelper();

	void	OnNetTest(int no, const char* data);
	
	void	DoNetTest();
	void	DoLoginTest();

	inline bool	IsLiving()		{return m_sock != INVALID_SOCKET;}

	void	DoNonScriptBehavior();

private:
	void	DoWorship(); // 祭神
	void	DoSignIn();  // 签到
	void	ExpandBag(); // 扩展背包

public:
	int			m_index;
	char		m_szName[32];
	char		m_szPwd[32];
	int			m_server;
	SOCKET		m_sock;
	int64		m_uid;
	int64		m_key;
	uint64		m_HeartTime;

	//player
	int64		m_PlayerID;

	int			m_SendNO;
	int			m_RecvNO;
	unsigned int m_LastTime;

	ERobotStat	m_Stat;
	int			m_ActionTime;

private:
	int			m_OperateCount;
	int			m_WorshipType;
	int			m_SignInType;
};

