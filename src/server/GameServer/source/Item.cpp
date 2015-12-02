#include "Item.h"
#include "LuaEngine.h"
#include "PlayerMgr.h"
#include "Packet.h"
#include "DataModule.h"
#include "attrs_defines.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "MessageTypeDefine.pb.h"
#include "MessageItem.pb.h"


void CItem::Release()
{

}

void CItem::Init()
{
	IBaseObj::Init();

	m_ParentID = INVALID_VALUE;
	m_Type = INVALID_VALUE;
	m_Career = INVALID_VALUE;
	m_Sex = INVALID_VALUE;
	m_MinLevel = INVALID_VALUE;
	m_Quatily = 0;
	m_StackSize = 0;
	m_StackMax = 0;
	m_SellPrice = 0;
	m_Position = INVALID_VALUE;
	m_RoleId = INVALID_VALUE;
	m_Intensify = 0;

	m_EffectUse.effectID = INVALID_VALUE;	
	m_EffectUse.effectValue = INVALID_VALUE;	
	m_EffectUse.effectProbability = INVALID_VALUE;	
	m_EffectUse.effectAttr = INVALID_VALUE;	

	for(int i=0; i<EQUIP_EFFECT_MAX; ++i)
	{
		m_EffectEquip[i].effectID = INVALID_VALUE;	
		m_EffectEquip[i].effectValue = INVALID_VALUE;	
		m_EffectEquip[i].effectProbability = INVALID_VALUE;	
		m_EffectEquip[i].effectAttr = INVALID_VALUE;	
	}
	
	m_Strength = 0;             //力量
	m_Intellect = 0;            //智力
	m_Technique = 0;			//技巧
	m_Agility = 0;				//敏捷

	m_Hit = 0;					//命中值
	m_Jouk = 0;					//闪避值
	m_Crit = 0;					//暴击值
	m_Tenacity = 0;				//韧性值
	m_Parry = 0;				//格挡值
	m_Treat = 0;				//治疗力

	m_BaseHpMax = 0;			//基础生命值上限
	m_BasePhysiDamage = 0;      //基础物理攻击
	m_BasePhysiDefense = 0;		//基础物理防御
	m_BaseMagicDamage = 0;		//基础魔法攻击
	m_BaseMagicDefense = 0;		//基础魔法防御
	m_BaseStuntDamage = 0;		//基础绝技攻击
	m_BaseStuntDefense = 0;		//基础绝技防御
}

bool CItem::OnCreate(int templateid)
{
	CItemTemplate* temp = ItemTemplateMgr.GetObj(templateid);
	if( !temp )
	{
		LOGGER_ERROR("[CItem] Invalid Item Template:%d", templateid);
		return false;
	}

	SetFieldInt(Item_Attrib_TemplateID, temp->m_Id);
	SetFieldInt(Item_Attrib_Type, temp->m_Type);
	SetFieldInt(Item_Attrib_Career, temp->m_Career);
	SetFieldInt(Item_Attrib_Sex, temp->m_Sex);
	SetFieldInt(Item_Attrib_MinLevel, temp->m_MinLevel);
	SetFieldInt(Item_Attrib_Quatily, temp->m_Quatily);
	SetFieldInt(Item_Attrib_StackMax, temp->m_StackMax);
	SetFieldInt(Item_Attrib_SellPrice, temp->m_SellPrice);
	SetFieldInt(Item_Attrib_StackSize, 1);
	SetFieldInt(Item_Attrib_Intensify, temp->m_Intensify);

	m_EffectUse.effectID = temp->m_EffectUse.effectID;
	m_EffectUse.effectValue = temp->m_EffectUse.effectValue;
	m_EffectUse.effectProbability = temp->m_EffectUse.effectProbability;
	m_EffectUse.effectAttr = temp->m_EffectUse.effectAttr;

	for(int i=0; i<EQUIP_EFFECT_MAX; ++i)
		SedEffect(i, temp->m_EffectEquip[i]);

	SetFieldInt(Item_Attrib_Strength, temp->m_Strength);
	SetFieldInt(Item_Attrib_Intellect, temp->m_Intellect);
	SetFieldInt(Item_Attrib_Technique, temp->m_Technique);
	SetFieldInt(Item_Attrib_Agility, temp->m_Agility);
	SetFieldInt(Item_Attrib_Hit, temp->m_Hit);
	SetFieldInt(Item_Attrib_Jouk, temp->m_Jouk);
	SetFieldInt(Item_Attrib_Crit, temp->m_Crit);
	SetFieldInt(Item_Attrib_Tenacity, temp->m_Tenacity);

	SetFieldInt(Item_Attrib_Parry, temp->m_Parry);
	SetFieldInt(Item_Attrib_Treat, temp->m_Treat);
	SetFieldInt(Item_Attrib_BaseHpMax, temp->m_BaseHpMax);
	SetFieldInt(Item_Attrib_BasePhysiDamage, temp->m_BasePhysiDamage);
	SetFieldInt(Item_Attrib_BasePhysiDefense, temp->m_BasePhysiDefense);
	SetFieldInt(Item_Attrib_BaseMagicDamage, temp->m_BaseMagicDamage);
	SetFieldInt(Item_Attrib_BaseMagicDefense, temp->m_BaseMagicDefense);
	SetFieldInt(Item_Attrib_BaseStuntDamage, temp->m_BaseStuntDamage);
	SetFieldInt(Item_Attrib_BaseStuntDefense, temp->m_BaseStuntDefense);

	return true;
}

int* CItem::_FindFieldInt(int i)
{
	switch(i)
	{
	case Item_Attrib_TemplateID:return &m_templateId;
	case Item_Attrib_Type:		return &m_Type;
	case Item_Attrib_Career:	return &m_Career;
	case Item_Attrib_Sex:		return &m_Sex;
	case Item_Attrib_MinLevel:	return &m_MinLevel;
	case Item_Attrib_Quatily:	return &m_Quatily;
	case Item_Attrib_StackMax:	return &m_StackMax;
	case Item_Attrib_StackSize:	return &m_StackSize;
	case Item_Attrib_SellPrice:	return &m_SellPrice;
	case Item_Attrib_Position:	return &m_Position;
	case Item_Attrib_Intensify: return &m_Intensify;

	case Item_Attrib_Strength:	return &m_Strength;
	case Item_Attrib_Intellect:	return &m_Intellect;
	case Item_Attrib_Technique:	return &m_Technique;
	case Item_Attrib_Agility:	return &m_Agility;
	
	case Item_Attrib_Hit:		return &m_Hit;
	case Item_Attrib_Jouk:		return &m_Jouk;
	case Item_Attrib_Crit:		return &m_Crit;
	case Item_Attrib_Tenacity:	return &m_Tenacity;
	case Item_Attrib_Parry:		return &m_Parry;
	case Item_Attrib_Treat: 	return &m_Treat;

	case Item_Attrib_BaseHpMax:			return &m_BaseHpMax;
	case Item_Attrib_BasePhysiDamage:	return &m_BasePhysiDamage;
	case Item_Attrib_BasePhysiDefense:	return &m_BasePhysiDefense;
	case Item_Attrib_BaseMagicDamage:	return &m_BaseMagicDamage;
	case Item_Attrib_BaseMagicDefense:	return &m_BaseMagicDefense;
	case Item_Attrib_BaseStuntDamage: 	return &m_BaseStuntDamage;
	case Item_Attrib_BaseStuntDefense:	return &m_BaseStuntDefense;

	default:	return NULL;
	}

	return NULL;
}

int64* CItem::_FindFieldI64(int i)
{
	switch(i)
	{
	case Item_Attrib_ID:		return &m_ObjID;
	case Item_Attrib_Parent:	return &m_ParentID;
	case Item_Attrib_EquipID:	return &m_RoleId;
	
	default:	return NULL;
	}

	return NULL;
}

void CItem::Serialize(std::string& jsonstr)
{
	rapidjson::Document root;
	root.SetObject();

	_SerializeFieldI64(Item_Attrib_ID, root, root);
	_SerializeFieldI64(Item_Attrib_Parent, root, root);
	_SerializeFieldInt(Item_Attrib_TemplateID, root, root);
	_SerializeFieldInt(Item_Attrib_Position, root, root);
	_SerializeFieldInt(Item_Attrib_StackSize, root, root);
	_SerializeFieldI64(Item_Attrib_EquipID, root, root);
	_SerializeFieldInt(Item_Attrib_Intensify, root, root);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	root.Accept(writer);
	jsonstr.assign(buffer.GetString(), buffer.Size());
}

void CItem::Deserialize(rapidjson::Value& json)
{
	for (rapidjson::Value::MemberIterator it = json.MemberBegin(); it != json.MemberEnd(); ++it) {

		if (it->value.IsInt()) {
			SetFieldInt(GetFieldType(it->name.GetString()), it->value.GetInt());
		}
		else if (it->value.IsInt64()) {
			SetFieldI64(GetFieldType(it->name.GetString()), it->value.GetInt64());
		}
		else if (it->value.IsString()) {
			SetFieldStr(GetFieldType(it->name.GetString()), it->value.GetString());
		}
		else if (it->value.IsArray()) {

		}
		else if (it->value.IsObject()) {

		}
	}
}

void CItem::Deserialize(std::string jsonstr)
{
	rapidjson::Document attrs;
	attrs.Parse<0>(jsonstr.c_str());

	Deserialize(attrs);
}

void CItem::SedEffect(int index, CEffect eff)
{
	if( index >= EQUIP_EFFECT_MAX || index < 0 )
		return;
	
	m_EffectEquip[index].effectID = eff.effectID;
	m_EffectEquip[index].effectValue = eff.effectValue;
	m_EffectEquip[index].effectProbability = eff.effectProbability;
	m_EffectEquip[index].effectAttr = eff.effectAttr;
}

bool CItem::GetEffect(int index, CEffect& eff)
{
	if( index >= EQUIP_EFFECT_MAX || index < 0 )
		return false;

	eff.effectID = m_EffectEquip[index].effectID;
	eff.effectValue = m_EffectEquip[index].effectValue;
	eff.effectProbability = m_EffectEquip[index].effectProbability;
	eff.effectAttr = m_EffectEquip[index].effectAttr;

	return true;
}

bool CItem::OnCost(int num)
{
	if( GetFieldInt(Item_Attrib_StackSize) <= num )
		return false;

	ChangeFieldInt(Item_Attrib_StackSize, -num, true, true);

	return true;
}

bool CItem::OnUse()
{
	//脚本输入参数
	int index = 0;
	LuaParam param[3];
	param[index++].SetDataNum( m_ParentID );
	param[index++].SetDataNum( m_EffectUse.effectAttr );
	param[index++].SetDataNum( m_EffectUse.effectValue );

	//执行脚本
	char szLuaTable[128] = {0};
	sprintf(szLuaTable, "Effect_%d", m_EffectUse.effectID);
	
	if (!LuaEngine.RunLuaFunction("OnEffect", szLuaTable, NULL, param, index))
	{
		LOGGER_ERROR("[CItem] OnUse Failed: %d", m_templateId);
		return false;
	}

	if (!OnCost())
	{
		LOGGER_ERROR("[CItem] OnCost Failed: %d", m_templateId);
		return false;
	}
	
	return true;
}

bool CItem::OnStack(CItem* item)
{
	if( !item )
		return false;

	if( GetTemplateID() != item->GetTemplateID() )
		return false;

	if( !IsStackable() || !item->IsStackable() )
		return false;

	int spare = GetFieldInt(Item_Attrib_StackMax) - GetFieldInt(Item_Attrib_StackSize);
	if( spare >= item->GetFieldInt(Item_Attrib_StackSize) )
	{
		ChangeFieldInt(Item_Attrib_StackSize, item->GetFieldInt(Item_Attrib_StackSize), true, true);
		item->ChangeFieldInt(Item_Attrib_StackSize, -item->GetFieldInt(Item_Attrib_StackSize));
		return true;
	}
	else
	{
		ChangeFieldInt(Item_Attrib_StackSize, spare, true, true);
		item->ChangeFieldInt(Item_Attrib_StackSize, -spare);
		return false;
	}
}

void CItem::SendClientMsg(Packet* pack)
{
	if( !pack )
		return;

	CPlayer* player = PlayerMgr.GetObj( GetFieldI64(Item_Attrib_Parent) );
	if( !player )
		return;

	player->SendClientMsg( pack );
}

void CItem::SendDataMsg(Packet* pack)
{
	if( !pack )
		return;

	CPlayer* player = PlayerMgr.GetObj( GetFieldI64(Item_Attrib_Parent) );
	if( !player )
		return;

	player->SendDataMsg( pack );
}

void CItem::SyncFieldIntToData(int i)
{
	DataModule.syncSetMap(GetFieldI64(Item_Attrib_Parent), this, GROUP_ITEMS, GetFieldName(i), GetFieldInt(i), GameServer.getServerSock(CBaseServer::Linker_Server_Data));
}

void CItem::SyncFieldI64ToData(int i)
{
	DataModule.syncSetMap(GetFieldI64(Item_Attrib_Parent), this, GROUP_ITEMS, GetFieldName(i), GetFieldI64(i), GameServer.getServerSock(CBaseServer::Linker_Server_Data));
}

void CItem::SyncFieldIntToClient(int i, CPlayer* toPlayer)
{
	Message::ItemAttrSync msg;
	msg.set_itemid(GetID());
	_PackageMsgAttr32(msg, i);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_ITEM_ATTRINT_SYNC);

	if( toPlayer )
		SendObserveMsg(&pack, toPlayer);
	else
		SendClientMsg(&pack);
}

void CItem::SyncFieldI64ToClient(int i, CPlayer* toPlayer)
{
	Message::ItemAttrSync msg;
	msg.set_itemid(GetID());
	_PackageMsgAttr64(msg, i);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_ITEM_ATTRI64_SYNC);

	if (toPlayer)
		SendObserveMsg(&pack, toPlayer);
	else
		SendClientMsg(&pack);
}

void CItem::_PackageMsgAttr32(Message::ItemAttrSync& msg, int i)
{
	Message::ItemAttrSync::Attr * attr = msg.add_attr();
	attr->set_type(i);
	attr->set_val32(GetFieldInt(i));
}

void CItem::_PackageMsgAttr64(Message::ItemAttrSync& msg, int i)
{
	Message::ItemAttrSync::Attr * attr = msg.add_attr();
	attr->set_type(i);
	attr->set_val64(GetFieldI64(i));
}

void CItem::SyncAllAttrToClient(CPlayer* toPlayer)
{
	Message::ItemAttrSync msg;
	msg.set_itemid(GetID());
	_PackageMsgAttr64(msg, Item_Attrib_Parent);
	_PackageMsgAttr32(msg, Item_Attrib_TemplateID);
	_PackageMsgAttr32(msg, Item_Attrib_Position);
	_PackageMsgAttr32(msg, Item_Attrib_StackSize);
	_PackageMsgAttr64(msg, Item_Attrib_EquipID);
	_PackageMsgAttr32(msg, Item_Attrib_Intensify);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_ITEM_DATA_SYNC);

	if (toPlayer)
		SendObserveMsg(&pack, toPlayer);
	else
		SendClientMsg(&pack);
}
