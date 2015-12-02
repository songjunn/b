#pragma once
#include "Item.h"
#include "utlmap.h"
#include "utllinkedlist.h"
#include "eventdef.h"

class CFighter;
class Packet;
class CItemUnit
{
public:
	CItemUnit();
	~CItemUnit();

	void	Initialize(CFighter* parent);
	void	Release();
	bool	OnMsg(Packet* pack);

	//道具操作接口
	bool	GainItem(CItem* item, ITEM_REASON reason);								//获得物品
	bool	GainItem(int templateid, ITEM_REASON reason, int num = 1);				//获得物品
	bool	DeleteItem(CItem* item, ITEM_REASON reason);							//销毁物品
	bool	DeleteItem(int templateid, ITEM_REASON reason, int num = 1);			//销毁物品
	bool	DeleteItemByType(int type, ITEM_REASON reason, int num = 1);			//销毁物品
	bool	CostItem(CItem* item, int num = 1);										//消耗物品
	bool	UseItem(CItem* item, int num = 1);										//使用物品
	bool	SellItem(CItem* item);													//出售物品
	bool	LoadItem(ItemID itemid, string jsonstr);								//加载道具

	//穿装/脱装
	bool	EquipItem(CItem* item, CFighter* role, bool flag);						//穿装
	bool	UnEquipItem(int idx, CFighter* role);									//卸装

	//同步到client
	void	SyncBagItemsInfo();														//同步包裹中的道具
	void	SyncEquipItemsInfo(CPlayer* toPlayer = NULL);							//同步英雄身上的道具

	//取得已有道具
	int		GetItemCountByTemplate(int templateid);									//通过模板获取道具数量
	int		GetItemCountByType(int type);											//通过类型获取道具数量
	CItem*	GetItem(ItemID itemid);													//获得包裹中的道具
	CItem*	GetItemByTemplate(int templateid);										//通过模板获得道具
	CItem*	GetItemByType(int type);												//通过类型获得道具
	int		GetItemCountById(int templateId);										//根据模板ID取得道具数量
	
	//道具ID
	ItemID	MakeItemID();															//产生道具实例ID

	//背包容量
	int CanSpareSeat(int itemTempId, int itemNum);									//判断是否可容得下一定数量的道具
	inline int GetSpareSeat() {	return GetMaxCapacity() - m_ItemList.Count(); }		//获取包裹剩余位置数量

protected:
	bool	_AddItem(CItem* item, bool client = false, bool data = false);			//加入包裹
	bool	_RemoveItem(CItem* item);												//移除包裹
	bool	_StackItem(CItem* item);												//堆叠处理

	bool	_Equip(CItem* item, bool client = false, bool data = false);
	bool	_UnEquip(int idx, ItemID &oldId, bool client = false, bool data = false);

	bool	_GetEquipPostion(CItem* item, int& idx);								//获取穿装的位置
	bool    _EquipItemAttrib(CItem* item, bool client = false, bool data = false);	//穿装后的属性计算
	bool	_UnEquipItemAttrib(CItem* item, bool client = false, bool data = false);//卸装后的属性计算

	inline void	_SetItemFactID(ItemID id) {m_FactID = id > m_FactID ? id : m_FactID;}

protected:
	bool	_HandlePacket_UseItem(Packet* pack);
	bool	_HandlePacket_SellItem(Packet* pack);
	bool	_HandlePacket_EquipItem(Packet* pack);
	bool	_HandlePacket_UnequipItem(Packet* pack);

private:
	void	SendResultFlag(int iflag, int i = 0, int64 l1 = 0, int64 l2 = 0);

	//扩充背包
	int		GetExpandBagCost(int expandCnt);	//扩充背包消耗金币
	int		GetMaxCapacity();					//获得背包最大容量

protected:
	CFighter* m_parent;
	ItemID	m_FactID;
	CUtlMap<ItemID, CItem*>	m_ItemList;			//包裹物品集合
	CUtlLinkedList<ItemID>	m_StackList;		//堆叠物品集合
	CItem*	m_EquipList[Equip_Position_End];	//装备栏物品集合

};
