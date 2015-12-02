#pragma once
#include "Fighter.h"
#include "attrs.h"
#include "GameServer.h"
#include "EventUnit.h"
#include "MessagePlayer.pb.h"

//class CFriendsUnit;
class CProp;
class CPlayer : public CFighter
{
public:
	CPlayer() {}
	virtual ~CPlayer() {}

	bool	OnCreate(int templateid);
	void	Init();
	void	Release();

	bool	OnMsg(Packet* pack);

	virtual void	Copy(const CPlayer& player);

	inline CPlayer* GetPlayer() { return this; }
	inline	void	OnEvent(CEvent* ev)	{m_EventUnit.OnEvent(ev);}

	inline  void	SendClientMsg(Packet* pack)	{ if (pack) { pack->SetTrans(m_UserID); GETSERVERNET(&GameServer)->sendMsg(m_GateSocket, pack); } }
	inline	void	SendDataMsg(Packet* pack){ if (pack) { pack->SetTrans(m_UserID); GETSERVERNET(&GameServer)->sendMsg(GameServer.getServerSock(CBaseServer::Linker_Server_Data), pack); } }
	inline	void	SendObserveMsg(Packet* pack, CPlayer* player) { if (pack && player) { pack->SetTrans(player->m_UserID); GETSERVERNET(&GameServer)->sendMsg(player->m_GateSocket, pack); } }

	inline	void	SetGateSocket(SOCKET socket){ m_GateSocket = socket; }
	inline	SOCKET	GetGateSocket()				{ return m_GateSocket; }

	inline	void	SetOnline(int flag)		{m_OnlineFlag = (m_OnlineFlag == Online_Flag_On ? m_OnlineFlag : flag);}
	inline	int		GetOnline()				{return m_OnlineFlag;}

	inline	void	SetLoadTime(TMV time)	{m_LoadTime = time;}
	inline	TMV		GetLoadTime()			{return m_LoadTime;}

	void	DataInit();
	void	DataSync();

	void	Serialize(std::string& jsonstr);
	void	Serialize(string name, rapidjson::Document& root);
	void	Deserialize(std::string jsonstr);
	void	Deserialize(rapidjson::Value& json);

	inline string GetFieldName(int i) { return attrs::get_player_field_name(i); }
	inline int GetFieldType(string name) { return attrs::get_player_field_type(name); }

	void	SyncFieldIntToData(int i);
	void	SyncFieldI64ToData(int i);
	void	SyncFieldStrToData(int i);
	void	SyncFieldIntToClient(int i, CPlayer* toPlayer = NULL);
	void	SyncFieldI64ToClient(int i, CPlayer* toPlayer = NULL);
	//void	SyncFieldStrToClient(int i, CPlayer* toPlayer = NULL);
	void	SyncAllAttrToClient(CPlayer* toPlayer = NULL);

protected:
	int*	_FindFieldInt(int i);
	int64*	_FindFieldI64(int i);
	string*	_FindFieldStr(int i);

	void	_PackageMsgAttr32(Message::PlayerAttrSync& msg, int i);
	void	_PackageMsgAttr64(Message::PlayerAttrSync& msg, int i);

public:
	CEventUnit			m_EventUnit;

private:
	SOCKET		m_GateSocket;
	int			m_OnlineFlag;
	TMV			m_LoadTime;
	UserID		m_UserID;

	//基础属性
	int			m_Level;			//等级
	int			m_GoldCoin;			//金币
	int			m_SilverCoin;		//银币
	int			m_VipLevel;			//vip等级
	int64		m_Exp;				//经验
	int64		m_ExpMax;			//最大经验

	//登陆相关
	string		m_CreateTime;		//角色创建时间
	string		m_LoginTime;		//上次登录时间
	string		m_LogoutTime;		//上次下线时间

	//战斗相关
	int			m_Fighting;			//战斗力
	int			m_Hp;				//当前生命值
	int			m_BaseStrength;		//基础力量
	int			m_BaseIntellect;	//基础智力
	int			m_BaseTechnique;	//基础技巧
	int			m_BaseAgility;		//基础敏捷
	int			m_BaseHpMax;		//基础生命值上限
	int			m_BasePhysiDamage;	//基础物理攻击
	int			m_BasePhysiDefense;	//基础物理防御
	int			m_BaseMagicDamage;	//基础魔法攻击
	int			m_BaseMagicDefense;	//基础魔法防御
	int			m_BaseStuntDamage;	//基础绝技攻击
	int			m_BaseStuntDefense;	//基础绝技防御
	int			m_BaseHit;			//基础命中值
	int			m_BaseJouk;			//基础闪避值
	int			m_BaseCrit;			//基础暴击值
	int			m_BaseTenacity;		//基础韧性值
	int			m_BaseParry;		//基础格挡值

};
