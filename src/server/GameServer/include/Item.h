#pragma once
#include "IBaseObj.h"
#include "ItemTemplate.h"
#include "attrs.h"
#include "gamedef.h"
#include "MessageItem.pb.h"

class CPlayer;
class CItem : public IBaseObj
{
public:
	CItem() {}
	virtual ~CItem() {}

	void	Init();
	void	Release();
	bool	OnCreate(int templateid);

	bool	OnCost(int num=1);
	bool	OnUse();
	bool	OnStack(CItem* item);

	void	SedEffect(int index, CEffect eff);
	bool	GetEffect(int index, CEffect& eff);
	const CEffect &GetUseEffect() { return m_EffectUse; }

	inline	bool	IsEquipable()	{return g_IsItemEquipment(m_Type);}		//是否可装备
	inline	bool	IsUseble()		{return g_IsItemFunction(m_Type);}		//是否可使用
	inline	bool	IsStackable()	{return GetFieldInt(Item_Attrib_StackMax) > 1;}	//是否可堆叠

	void	SendClientMsg(Packet* pack);
	void	SendDataMsg(Packet* pack);

	void	Serialize(std::string& jsonstr);
	void	Deserialize(string jsonstr);
	void	Deserialize(rapidjson::Value& json);

	inline string GetFieldName(int i) { return attrs::get_item_field_name(i); }
	inline int GetFieldType(string name) { return attrs::get_item_field_type(name); }

	void	SyncFieldIntToData(int i);
	void	SyncFieldI64ToData(int i);
	void	SyncFieldIntToClient(int i, CPlayer* toPlayer = NULL);
	void	SyncFieldI64ToClient(int i, CPlayer* toPlayer = NULL);
	void	SyncAllAttrToClient(CPlayer* toPlayer = NULL);

protected:
	int*	_FindFieldInt(int i);
	int64*	_FindFieldI64(int i);

	void	_PackageMsgAttr32(Message::ItemAttrSync& msg, int i);
	void	_PackageMsgAttr64(Message::ItemAttrSync& msg, int i);

private:
	PersonID	m_ParentID;				//拥有者ID
	int			m_Type;					//类型
	int			m_Career;				//职业限制
	int			m_Sex;					//性别限制
	int			m_MinLevel;				//最低等级限制
	int			m_Quatily;				//品质
	int			m_StackMax;				//最大堆叠数量
	int			m_SellPrice;			//出售价格
	int 		m_Intensify;			//当前装备强化等级, 初始0
	CEffect		m_EffectUse;			//使用效果
	CEffect		m_EffectEquip[EQUIP_EFFECT_MAX];	//装备效果

	int			m_StackSize;			//堆叠数量
	int			m_Position;				//装备位置
	PersonID	m_RoleId;               //装备英雄id

	int 		m_Strength;             //力量
	int 		m_Intellect;            //智力
	int 		m_Technique;			//技巧
	int 		m_Agility;				//敏捷

	int 		m_Hit;					//命中值
	int			m_Jouk;					//闪避值
	int 		m_Crit;					//暴击值
	int 		m_Tenacity;				//韧性值
	int 		m_Parry;				//格挡值
	int 		m_Treat;				//治疗力

	int 		m_BaseHpMax;			//基础生命值上限
	int 		m_BasePhysiDamage;      //基础物理攻击
	int 		m_BasePhysiDefense;		//基础物理防御
	int 		m_BaseMagicDamage;		//基础魔法攻击
	int 		m_BaseMagicDefense;		//基础魔法防御
	int			m_BaseStuntDamage;		//基础绝技攻击
	int			m_BaseStuntDefense;		//基础绝技防御

};
