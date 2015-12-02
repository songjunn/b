#include "ItemUnit.h"
#include "ItemMgr.h"
#include "HeroMgr.h"
#include "PlayerMgr.h"
#include "Event.h"
#include "Packet.h"
#include "attrs_defines.h"
#include "MessageTypeDefine.pb.h"
#include "MessageItem.pb.h"
#include "NoticeModule.h"
#include "DataModule.h"
#include "error.h"
#include "gtime.h"


CItemUnit::CItemUnit() : m_parent(NULL)
{
	SetDefLessFunc(m_ItemList);
}

CItemUnit::~CItemUnit()
{
	
}

void CItemUnit::Initialize(CFighter* parent)
{
	m_parent = parent;

	m_FactID = 0;
}

void CItemUnit::Release()
{
	for(int i=0; i<Equip_Position_End; ++i)
	{
		if( !m_EquipList[i] )
			continue;

		ItemMgr.Delete( m_EquipList[i]->GetID() );

		m_EquipList[i] = NULL; 
	}

	FOR_EACH_MAP(m_ItemList, idx)
	{
		ItemMgr.Delete( m_ItemList.Key(idx) );
	}
	m_ItemList.RemoveAll();
}

bool CItemUnit::OnMsg(Packet* pack)
{
	if( !pack )
		return false;

	switch( pack->Type() )
	{
	case Message::MSG_REQUEST_ITEM_USE:		_HandlePacket_UseItem(pack);	break;
	//case Message::MSG_REQUEST_ITEM_DELETE:	_HandlePacket_DeleteItem(pack);	break;
	case Message::MSG_REQUEST_ITEM_SELL:		_HandlePacket_SellItem(pack);	break;
	case Message::MSG_REQUEST_ITEM_EQUIP:	_HandlePacket_EquipItem(pack);	break;
	case Message::MSG_REQUEST_ITEM_UNEQUIP:	_HandlePacket_UnequipItem(pack); break;
	default:	return false;
	}

	return true;
}

bool CItemUnit::_HandlePacket_UseItem(Packet* pack)
{
	if( !pack )
		return false;

	Message::ItemUseRequest msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CItem* item = GetItem( msg.itemid() );
	if( !item )
		return false;

	return UseItem(item);
}

bool CItemUnit::_HandlePacket_SellItem(Packet* pack)
{
	if( !pack )
		return false;

	Message::ItemSellRequest msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CItem* item = GetItem( msg.itemid() );
	if( !item )
		return false;

	return SellItem(item);
}

bool CItemUnit::_HandlePacket_EquipItem(Packet* pack)
{
	if( !pack ) {
		SendResultFlag(Error_Equipment_UnKnow);
		return false;
	}

	Message::ItemEquipRequest msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CItem* item = GetItem( msg.itemid() );
	if( !item ) {
		SendResultFlag(Error_Equipment_NoItem);
		return false;
	}

	CFighter* role = NULL;
	GET_FIGHTER( msg.roleid(), role )
	if( !role ) {
		SendResultFlag(Error_Equipment_UnKnow);
		return false;
	}

	return EquipItem(item, role, true);
}

bool CItemUnit::_HandlePacket_UnequipItem(Packet* pack)
{
	if( !pack ) {
		SendResultFlag(Error_Equipment_UnKnow);
		return false;
	}

	Message::ItemUnequipRequest msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CFighter* role = NULL;
	GET_FIGHTER( msg.roleid(), role )
	if( !role ) {
		SendResultFlag(Error_Equipment_UnKnow);
		return false;
	}

	return UnEquipItem(msg.position(), role);
}

bool CItemUnit::GainItem(int templateid, ITEM_REASON reason, int num)
{
	do
	{
		CItem* item = ItemMgr.Create(templateid, MakeItemID());
		if( !item )
		{
			LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" template:%d itempool:%d", __FILE__, __LINE__, m_parent->GetID(), templateid, ItemMgr.Count());
			return false;
		}

		int stack = 1;
		if( item->IsStackable() )
		{
			stack = item->GetFieldInt(Item_Attrib_StackMax) > num ? num : item->GetFieldInt(Item_Attrib_StackMax);
			item->SetFieldInt(Item_Attrib_StackSize, stack);
		}

		if( !GainItem(item, reason) )
		{
			LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" template:%d num:%d", __FILE__, __LINE__, m_parent->GetID(), templateid, num);
			ItemMgr.Delete(item->GetID());
			return false;
		}

		num -= stack;

	} while ( num > 0 );

	return true;
}

bool CItemUnit::DeleteItem(int templateid, ITEM_REASON reason, int num)
{
	int count = GetItemCountByTemplate(templateid);
	if( count < num )
		return false;

	while( num > 0 )
	{
		CItem* item = GetItemByTemplate(templateid);
		if( !item )
			return false;

		int size = item->GetFieldInt(Item_Attrib_StackSize);
		if( num >= size )
		{
			DeleteItem(item, reason);
			num -= size;
		}
		else
		{
			item->ChangeFieldInt(Item_Attrib_StackSize, -num, true, true);
			num = 0;

			CEvent *evLog = MakeEvent(Event_Item_Consume, m_parent->GetID(), m_parent->GetFieldI64(Role_Attrib_UserID), item->GetID(), item->GetTemplateID(), num, reason, true);
			m_parent->OnEvent(evLog);
		}
	}

	return true;
}

bool CItemUnit::DeleteItemByType(int type, ITEM_REASON reason, int num)
{
	int count = GetItemCountByType(type);
	if( count < num )
		return false;

	while( num > 0 )
	{
		CItem* item = GetItemByType(type);
		if( !item )
			return false;

		int size = item->GetFieldInt(Item_Attrib_StackSize);
		if( num >= size )
		{
			DeleteItem(item, reason);
			num -= size;
		}
		else
		{
			item->ChangeFieldInt(Item_Attrib_StackSize, -num, true, true);
			num = 0;

			CEvent *evLog = MakeEvent(Event_Item_Consume, m_parent->GetID(), m_parent->GetFieldI64(Role_Attrib_UserID), item->GetID(), item->GetTemplateID(), num, reason, true);
			m_parent->OnEvent(evLog);
		}
	}
	return true;
}

bool CItemUnit::GainItem(CItem* item, ITEM_REASON reason)
{
	if( !item )
		return false;

	//检查该物品是否已有拥有者
	if( item->GetFieldI64(Item_Attrib_Parent) > 0 )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT" parent:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID(), item->GetFieldI64(Item_Attrib_Parent));
		return false;
	}

	//首先尝试堆叠
	if( item->IsStackable() && !_StackItem(item) ) {
		CEvent *evLog = MakeEvent(Event_Item_Obtain, m_parent->GetID(), (int64)item->GetTemplateID(), (int64)item->GetFieldInt(Item_Attrib_StackSize), item->GetID(), reason, true);
		m_parent->OnEvent(evLog);
		return true;
	}

	if(GetSpareSeat() < 1) {
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}

	if( !_AddItem(item) )
		return false;

	//同步客户端
	item->SyncAllAttrToClient();

	//同步服务器
	DataModule.syncAddMap(m_parent->GetID(), item, GROUP_ITEMS, GameServer.getServerSock(CBaseServer::Linker_Server_Data));

	CEvent *evLog = MakeEvent(Event_Item_Obtain, m_parent->GetID(), (int64)item->GetTemplateID(), (int64)item->GetFieldInt(Item_Attrib_StackSize), item->GetID(), reason, true);
	m_parent->OnEvent(evLog);

	return true;
}

bool CItemUnit::SellItem(CItem* item)
{
	if( !item ) {
		SendResultFlag(Error_ItemUnit_UnKnow);
		return false;
	}

	//检查是否是自己的道具
	if( item->GetFieldI64(Item_Attrib_Parent) != m_parent->GetID() )
	{
		SendResultFlag(Error_ItemUnit_HaveNo);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT" parent:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID(), item->GetFieldI64(Item_Attrib_Parent));
		return false;
	}

	int price = item->GetFieldInt(Item_Attrib_SellPrice);
	int num = item->GetFieldInt(Item_Attrib_StackSize);

	ItemID itid = item->GetID();
	if(DeleteItem(item, Item_Reason_Sell)) {
		//收钱
		m_parent->GainSilver(price * num, Silver_Reason_SellItem);
		SendResultFlag(Error_ItemUnit_SellSuccess, 0, itid);
		return true;
	}
	else {
		SendResultFlag(Error_ItemUnit_SellFail);
		return false;
	}
}

bool CItemUnit::UseItem(CItem* item, int num)
{
	if( !item || num <= 0 )
		return false;

	//检查是否是自己的道具
	if( item->GetFieldI64(Item_Attrib_Parent) != m_parent->GetID() )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT" parent:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID(), item->GetFieldI64(Item_Attrib_Parent));
		return false;
	}

	if( !item->IsUseble() )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT" template:%d", __FILE__, __LINE__, m_parent->GetID(), item->GetID(), item->GetTemplateID());
		return false;
	}

	bool bUse = item->OnUse();
	if (bUse && item->GetFieldInt(Item_Attrib_StackSize) <= 0)
	{
		DeleteItem(item, Item_Reason_Use);
	}

	return bUse;
}

bool CItemUnit::EquipItem(CItem* item, CFighter* role, bool flag)
{
	if( !item || !role )
		return false;

	//检查是否装备
	if(!g_IsItemEquipment(item->GetFieldInt(Item_Attrib_Type))) {
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT" not equipment", __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}

	//检查等级限制
	if(item->GetFieldInt(Item_Attrib_MinLevel) > role->GetFieldInt(Role_Attrib_Level)) {
		SendResultFlag(Error_Equipment_OverLevel);
		return false;
	}

	//检查该装备是否已穿戴
	if( item->GetFieldI64(Item_Attrib_EquipID) > 0 )
	{
		SendResultFlag(Error_Equipment_HaveEquiped);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT" equiprole:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID(), item->GetFieldI64(Item_Attrib_EquipID));
		return false;
	}

	if( role->GetID() != m_parent->GetID() && role->GetFieldI64(Role_Attrib_Parent) != m_parent->GetID() )
	{
		SendResultFlag(Error_Equipment_UnKnow);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" equiprole:"INT64_FMT" item:"INT64_FMT" parent:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), role->GetID(), item->GetID(), item->GetFieldI64(Role_Attrib_Parent));
		return false;
	}

	if( !role->m_ItemUnit._Equip(item, true, true) )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}
	//将其从包裹中移除
	if( !_RemoveItem(item) )
	{
		SendResultFlag(Error_Equipment_RemoveFromBag);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}
	
	if(flag)
	{
		SendResultFlag(Error_Equipment_Success, item->GetFieldInt(Item_Attrib_Position),
			role->GetID(), item->GetID());
	}

	CEvent *ev = MakeEvent(Event_Branch_QualityItem, m_parent->GetID(), item->GetFieldInt(Item_Attrib_Quatily));
	m_parent->OnEvent(ev);

	return true;
}

bool CItemUnit::UnEquipItem(int idx, CFighter* role)
{
	if( !role )
		return false;

	if( idx < 0 || idx >= Equip_Position_End ) {
		SendResultFlag(Error_UnEquipment_WrongLocation);
		return false;
	}

	if( role->GetID() != m_parent->GetID() && role->GetFieldI64(Role_Attrib_Parent) != m_parent->GetID() )
	{
		SendResultFlag(Error_Equipment_UnKnow);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" equiprole:"INT64_FMT" position:%d", __FILE__, __LINE__, m_parent->GetID(), role->GetID(), idx);
		return false;
	}

	if(GetSpareSeat() < 1) {
		SendResultFlag(Error_UnEquipment_FullBag);
		return false;
	}

	ItemID oldId = 0;
	if( !role->m_ItemUnit._UnEquip(idx, oldId, true, true) )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" position:%d", __FILE__, __LINE__, m_parent->GetID(), idx);
		return false;
	}

	CItem *oldItem = ItemMgr.GetObj(oldId);
	if( !_AddItem(oldItem, true, true) )
	{
		SendResultFlag(Error_UnEquipment_PutToBag);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), oldItem->GetID());
		return false;
	}

	SendResultFlag(Error_UnEquipment_Success, idx, role->GetID(), oldId);

	CEvent *ev = MakeEvent(Event_Branch_QualityItem, m_parent->GetID(), oldItem->GetFieldInt(Item_Attrib_Quatily));
	m_parent->OnEvent(ev);

	return true;
}

void CItemUnit::SyncBagItemsInfo()
{
	//同步包裹中物品
	FOR_EACH_MAP( m_ItemList, idx )
	{
		CItem* item = m_ItemList[idx];
		if (item)
			item->SyncAllAttrToClient();
	}
}

void CItemUnit::SyncEquipItemsInfo(CPlayer* toPlayer)
{
	//同步穿着的装备
	for(int i=0; i<Equip_Position_End; ++i)
	{
		CItem* item = m_EquipList[i];
		if( item )
			item->SyncAllAttrToClient(toPlayer);
	}
}

bool CItemUnit::DeleteItem(CItem* item, ITEM_REASON reason)
{
	if( !item )
		return false;

	if( !_RemoveItem(item) )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}

	//同步客户端
	Message::ItemDeleteSync msg;
	msg.set_itemid( item->GetID() );
	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_ITEM_DELETE_SYNC);
	m_parent->SendClientMsg( &pack );

	//同步DataServer
	DataModule.syncDelMap(m_parent->GetID(), item, GROUP_ITEMS, GameServer.getServerSock(CBaseServer::Linker_Server_Data));

	CEvent* ev = MakeEvent(Event_Item_Delete, m_parent->GetID(), (int64)item->GetTemplateID(), (int64)item->GetFieldInt(Item_Attrib_StackSize));
	m_parent->OnEvent(ev);

	ItemMgr.Delete(item->GetID());

	return true;
}

ItemID CItemUnit::MakeItemID()
{
	m_FactID = m_FactID > 0 ? m_FactID : g_MakeInitItemID(m_parent->GetID()); 
	return ++m_FactID;
}

bool CItemUnit::LoadItem(ItemID itemid, string jsonstr)
{
	rapidjson::Document obj;
	obj.Parse<0>(jsonstr.c_str());
	int templateid = obj["templateid"].GetInt();

	CItem* item = ItemMgr.Create(templateid, itemid);
	if (!item)
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" id:"INT64_FMT" template:%d", __FILE__, __LINE__, m_parent->GetID(), itemid, templateid);
		return false;
	}

	item->Deserialize(obj);

	if (item->GetFieldInt(Item_Attrib_Position) < 0)
	{
		_AddItem(item);
	}
	else
	{
		CFighter* role = NULL;
		GET_FIGHTER(item->GetFieldI64(Item_Attrib_EquipID), role)
		if (role) {
			role->m_ItemUnit._Equip(item);
		}
		_SetItemFactID(itemid);
	}

	return true;
}

bool CItemUnit::_AddItem(CItem* item, bool client, bool data)
{
	if( !item )
		return false;

	int idx = m_ItemList.Find( item->GetID() );
	if( m_ItemList.IsValidIndex(idx) )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}

	m_ItemList.Insert( item->GetID(), item );

	if( item->IsStackable() )
		m_StackList.AddToTail( item->GetID() );

	_SetItemFactID(item->GetID());

	item->SetFieldI64( Item_Attrib_Parent, m_parent->GetID(), client, data );
	item->SetFieldInt( Item_Attrib_Position, INVALID_VALUE, client, data );

	return true;
}

bool CItemUnit::_RemoveItem(CItem* item)
{
	if( !item )
		return false;

	if( item->IsStackable())
	{
		int idx = m_StackList.Find( item->GetID() );
		if( !m_StackList.IsValidIndex(idx) )
		{
			m_StackList.Remove(idx);
		}
		else
		{
			LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		}
	}

	int idx = m_ItemList.Find( item->GetID() );
	if( !m_ItemList.IsValidIndex(idx) )
	{
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}
	m_ItemList.RemoveAt( idx );

	return true;
}

bool CItemUnit::_StackItem(CItem* item)
{
	if( !item )
		return true;

	FOR_EACH_LL( m_StackList, idx )
	{
		CItem* stackItem = ItemMgr.GetObj( m_StackList[idx] );
		if( !stackItem )
			continue;

		if( stackItem->OnStack(item) )
			break;
	}

	if( item->GetFieldInt(Item_Attrib_StackSize) <= 0 )
	{
		ItemMgr.Delete( item->GetID() );
		return false;
	}
	
	return true;
}

bool CItemUnit::_Equip(CItem* item, bool client, bool data)
{
	if( !item )
		return false;

	CPlayer *player = m_parent->GetPlayer();
	if(!player) {
		SendResultFlag(Error_Equipment_UnKnow);
		return false;
	}

	//检查职业限制
	int career = item->GetFieldInt(Item_Attrib_Career);
	int vocation = m_parent->GetFieldInt(Role_Attrib_Vocation);
	if( career > 0 && Role_Vocation_Tongyong != career && career != vocation )
	{
		SendResultFlag(Error_Equipment_MisMatchCareer);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT" career:%d vocation:%d", __FILE__, __LINE__, m_parent->GetID(), item->GetID(), career, vocation);
		return false;
	}
	
	int idx = 0;
	//寻找装备位置
	if(!_GetEquipPostion(item, idx))
	{
		SendResultFlag(Error_Equipment_NoLocation);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), item->GetID());
		return false;
	}

	//卸掉该位置已有的装备
	ItemID oldId = 0;
	if( m_EquipList[idx] && !_UnEquip(idx, oldId, client, data) )
	{
		SendResultFlag(Error_Equipment_UnEquip);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" position:%d", __FILE__, __LINE__, m_parent->GetID(), idx);
		return false;
	}

	if(oldId > 0) {
		CItem *oldItem = ItemMgr.GetObj(oldId);
		if( !player->m_ItemUnit._AddItem(oldItem, true, true) )
		{
			SendResultFlag(Error_UnEquipment_PutToBag);
			LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" item:"INT64_FMT, __FILE__, __LINE__, m_parent->GetID(), oldItem->GetID());
			return false;
		}
	}

	m_EquipList[idx] = item;

	_SetItemFactID(item->GetID());

	//同步装备位置
	item->SetFieldI64(Item_Attrib_EquipID, m_parent->GetID(), client, data);
	item->SetFieldInt(Item_Attrib_Position, idx, client, data);

	//同步穿装属性
	_EquipItemAttrib(item, client, false);

	return true;
}

bool CItemUnit::_UnEquip(int idx, ItemID &oldId, bool client, bool data)
{
	if( 0 > idx || Equip_Position_End <= idx )
	{
		SendResultFlag(Error_UnEquipment_WrongLocation);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" pos:%d", __FILE__, __LINE__, m_parent->GetID(), idx);
		return false;
	}

	if( !m_EquipList[idx] )
	{
		SendResultFlag(Error_UnEquipment_NoEquip);
		LOGGER_ERROR("[CItemUnit] Error:%s:%d, Player:"INT64_FMT" pos:%d", __FILE__, __LINE__, m_parent->GetID(), idx);
		return false;
	}

	//同步装备属性
	m_EquipList[idx]->SetFieldI64(Item_Attrib_EquipID, INVALID_VALUE, client, data);
	m_EquipList[idx]->SetFieldInt(Item_Attrib_Position, INVALID_VALUE, client, data);

	//同步卸装属性
	_UnEquipItemAttrib(m_EquipList[idx], client, false);

	CItem *oldItem = m_EquipList[idx];
	m_EquipList[idx] = NULL;

	oldId = oldItem->GetID();

	return true;
}

CItem*	CItemUnit::GetItem(ItemID itemid)
{
	int idx = m_ItemList.Find(itemid);
	if( m_ItemList.IsValidIndex(idx) )
		return m_ItemList[idx];
	return NULL;
}

CItem* CItemUnit::GetItemByTemplate(int templateid)
{
	FOR_EACH_MAP(m_ItemList, idx)
	{
		CItem* item = m_ItemList[idx];
		if( item && item->GetTemplateID() == templateid )
			return item;
	}
	return NULL;
}

CItem* CItemUnit::GetItemByType(int type)
{
	FOR_EACH_MAP(m_ItemList, idx)
	{
		CItem* item = m_ItemList[idx];
		if( item && item->GetFieldInt(Item_Attrib_Type) == type )
			return item;
	}
	return NULL;
}

int CItemUnit::GetItemCountByTemplate(int templateid)
{
	int num = 0;
	FOR_EACH_MAP(m_ItemList, idx)
	{
		CItem* item = m_ItemList[idx];
		if( item && item->GetTemplateID() == templateid )
			num += item->GetFieldInt(Item_Attrib_StackSize);
	}
	return num;
}

int	CItemUnit::GetItemCountByType(int type)
{
	int num = 0;
	FOR_EACH_MAP(m_ItemList, idx)
	{
		CItem* item = m_ItemList[idx];
		if( item && item->GetFieldInt(Item_Attrib_Type) == type )
			num += item->GetFieldInt(Item_Attrib_StackSize);
	}
	return num;
}

int	CItemUnit::GetItemCountById(int templateId)
{
	int num = 0;

	FOR_EACH_MAP(m_ItemList, idx)
	{
		CItem* item = m_ItemList[idx];
		if(!item) {
			continue;
		}

		if(item->GetTemplateID() == templateId) {
			num += item->GetFieldInt(Item_Attrib_StackSize);
		}
	}

	for(int i = Equip_Position_MainWeapon; i < Equip_Position_End; i++) {
		CItem* item = m_EquipList[i];
		if(!item) {
			continue;
		}

		if(item->GetTemplateID() == templateId) {
			num += item->GetFieldInt(Item_Attrib_StackSize);
		}
	}

	return num;
}

bool CItemUnit::_GetEquipPostion(CItem* item, int& pos)
{
	if( !item )
		return false;

	if( g_IsItemEquipmentWeaponMain(item->GetFieldInt(Item_Attrib_Type)) )	
		pos = Equip_Position_MainWeapon;
	else if( g_IsItemEquipmentWeaponAssi(item->GetFieldInt(Item_Attrib_Type)) )
		pos = Equip_Position_AssiWeapon;
	else if( g_IsItemEquipmentArmorChest(item->GetFieldInt(Item_Attrib_Type)) )
		pos = Equip_Position_Chest;
	else if( g_IsItemEquipmentArmorHands(item->GetFieldInt(Item_Attrib_Type)) )
		pos = Equip_Position_Hands;
	else if( g_IsItemEquipmentArmorFeets(item->GetFieldInt(Item_Attrib_Type)) )
		pos = Equip_Position_Feets;
	else if( g_IsItemEquipmentJewelryNecklace(item->GetFieldInt(Item_Attrib_Type)) )
		pos = Equip_Position_Necklace; //项链
	else if( g_IsItemEquipmentJewelryBracelet(item->GetFieldInt(Item_Attrib_Type)) )
		pos = Equip_Position_Bracelet;  //手镯	
	else if( g_IsItemEquipmentJewelryRings(item->GetFieldInt(Item_Attrib_Type)) )
		pos = Equip_Position_Rings;    //戒指
	else
		return false;
	
	return true;
}

bool CItemUnit::_EquipItemAttrib(CItem* item, bool client, bool data)
{
	if( !item || (!client && !data))
		return false;

	if(item->GetFieldInt(Item_Attrib_Strength) > 0)
		m_parent->ChangeFieldInt(Role_Attrib_StrengthAddons, item->GetFieldInt(Item_Attrib_Strength), client, data);
	if(item->GetFieldInt(Item_Attrib_Intellect)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_IntellectAddons, item->GetFieldInt(Item_Attrib_Intellect), client, data);
	if(item->GetFieldInt(Item_Attrib_Technique)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_TechniqueAddons, item->GetFieldInt(Item_Attrib_Technique), client, data);
	if(item->GetFieldInt(Item_Attrib_Agility)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_AgilityAddons, item->GetFieldInt(Item_Attrib_Agility), client, data);
	if(item->GetFieldInt(Item_Attrib_Hit)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_HitAddons, item->GetFieldInt(Item_Attrib_Hit), client, data);
	if(item->GetFieldInt(Item_Attrib_Jouk)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_JoukAddons, item->GetFieldInt(Item_Attrib_Jouk), client, data);
	if(item->GetFieldInt(Item_Attrib_Crit)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_CritAddons, item->GetFieldInt(Item_Attrib_Crit), client, data);
	if(item->GetFieldInt(Item_Attrib_Tenacity)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_TenacityAddons, item->GetFieldInt(Item_Attrib_Tenacity), client, data);
	if(item->GetFieldInt(Item_Attrib_Parry)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_ParryAddons, item->GetFieldInt(Item_Attrib_Parry), client, data);
	if(item->GetFieldInt(Item_Attrib_Treat)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_TreatAddons, item->GetFieldInt(Item_Attrib_Treat), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseHpMax)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_HpMaxAddons, item->GetFieldInt(Item_Attrib_BaseHpMax), client, data);
	if(item->GetFieldInt(Item_Attrib_BasePhysiDamage)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_PhysiDamageAddons, item->GetFieldInt(Item_Attrib_BasePhysiDamage), client, data);
	if(item->GetFieldInt(Item_Attrib_BasePhysiDefense)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_PhysiDefenseAddons, item->GetFieldInt(Item_Attrib_BasePhysiDefense), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseMagicDamage)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_MagicDamageAddons, item->GetFieldInt(Item_Attrib_BaseMagicDamage), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseMagicDefense)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_MagicDefenseAddons, item->GetFieldInt(Item_Attrib_BaseMagicDefense), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseStuntDamage)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_StuntDamageAddons, item->GetFieldInt(Item_Attrib_BaseStuntDamage), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseStuntDefense)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_StuntDefenseAddons, item->GetFieldInt(Item_Attrib_BaseStuntDefense), client, data);
	
	//加上强化的属性加成...
	//m_parent->m_IntensifyItemUnit.InlayJadeOnAttr(item, true);
	//m_parent->m_IntensifyItemUnit.InlayCrystalOnAttr(item, true);

	return true;
}

bool CItemUnit::_UnEquipItemAttrib(CItem* item, bool client, bool data)
{
	if( !item || (!client && !data))
		return false;

	//属性附加值	
	if(item->GetFieldInt(Item_Attrib_Strength) > 0)
		m_parent->ChangeFieldInt(Role_Attrib_StrengthAddons, -item->GetFieldInt(Item_Attrib_Strength), client, data);
	if(item->GetFieldInt(Item_Attrib_Intellect)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_IntellectAddons, -item->GetFieldInt(Item_Attrib_Intellect), client, data);
	if(item->GetFieldInt(Item_Attrib_Technique)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_TechniqueAddons, -item->GetFieldInt(Item_Attrib_Technique), client, data);
	if(item->GetFieldInt(Item_Attrib_Agility)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_AgilityAddons, -item->GetFieldInt(Item_Attrib_Agility), client, data);
	if(item->GetFieldInt(Item_Attrib_Hit)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_HitAddons, -item->GetFieldInt(Item_Attrib_Hit), client, data);
	if(item->GetFieldInt(Item_Attrib_Jouk)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_JoukAddons, -item->GetFieldInt(Item_Attrib_Jouk), client, data);
	if(item->GetFieldInt(Item_Attrib_Crit)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_CritAddons, -item->GetFieldInt(Item_Attrib_Crit), client, data);
	if(item->GetFieldInt(Item_Attrib_Tenacity)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_TenacityAddons, -item->GetFieldInt(Item_Attrib_Tenacity), client, data);
	if(item->GetFieldInt(Item_Attrib_Parry)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_ParryAddons, -item->GetFieldInt(Item_Attrib_Parry), client, data);
	if(item->GetFieldInt(Item_Attrib_Treat)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_TreatAddons, -item->GetFieldInt(Item_Attrib_Treat), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseHpMax)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_HpMaxAddons, -item->GetFieldInt(Item_Attrib_BaseHpMax), client, data);
	if(item->GetFieldInt(Item_Attrib_BasePhysiDamage)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_PhysiDamageAddons, -item->GetFieldInt(Item_Attrib_BasePhysiDamage), client, data);
	if(item->GetFieldInt(Item_Attrib_BasePhysiDefense)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_PhysiDefenseAddons, -item->GetFieldInt(Item_Attrib_BasePhysiDefense), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseMagicDamage)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_MagicDamageAddons, -item->GetFieldInt(Item_Attrib_BaseMagicDamage), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseMagicDefense)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_MagicDefenseAddons, -item->GetFieldInt(Item_Attrib_BaseMagicDefense), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseStuntDamage)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_StuntDamageAddons, -item->GetFieldInt(Item_Attrib_BaseStuntDamage), client, data);
	if(item->GetFieldInt(Item_Attrib_BaseStuntDefense)> 0)
		m_parent->ChangeFieldInt(Role_Attrib_StuntDefenseAddons, -item->GetFieldInt(Item_Attrib_BaseStuntDefense), client, data);
	
	//减去强化的属性加成...
	//m_parent->m_IntensifyItemUnit.InlayJadeOnAttr(item, true, false);
	//m_parent->m_IntensifyItemUnit.InlayCrystalOnAttr(item, true, false);

	return true;
}

int CItemUnit::GetExpandBagCost(int expandCnt)
{
	int fabNum = 0;
	if (expandCnt <= 1) {
		fabNum = 0;
	}
	else if (expandCnt > 1 && expandCnt <= 5) {
		fabNum = 1;
	}
	else if (expandCnt > 5 && expandCnt <= 13) {
		fabNum = 2;
	}
	else {
		fabNum = 3;
	}

	int powNum = 1;
	for (int i = 0; i < fabNum; i++) {
		powNum *= 2;
	}

	return ITEMUNIT_EXPAND_BASECOST * powNum;
}

int CItemUnit::GetMaxCapacity()
{
	return 100;
}

int CItemUnit::CanSpareSeat(int itemTempId, int itemNum)
{
	CItemTemplate *itemTemp = ItemTemplateMgr.GetObj(itemTempId);
	if(!itemTemp) {
		return false;
	}

	int spareSeat = itemNum;
	if(itemTemp->m_StackMax > 1) {
		int modeSeat = (itemNum % itemTemp->m_StackMax > 0) ? 1 : 0;
		spareSeat = itemNum / itemTemp->m_StackMax + modeSeat;
	}

	if(GetSpareSeat() < spareSeat) {
		return 0;
	}
	else {
		return spareSeat;
	}
}

void CItemUnit::SendResultFlag(int iflag, int i, int64 l1, int64 l2)
{
	NoticeModule.SendErrorMsg(m_parent, iflag, i, 0, 0, l1, l2);
}
