#ifndef GAMESERVER_ITEMMGR_H
#define GAMESERVER_ITEMMGR_H

#include "commdata.h"
#include "ObjMgr.h"
#include "Item.h"


class CItemMgr : public CObjMgr< CItem, ItemID >, public Singleton< CItemMgr >
{
public:
	CItemMgr()	{};
	~CItemMgr()	{};

	inline CItem* Create(int templateid, ItemID itemid)
	{
		CItem* item = CObjMgr< CItem, ItemID >::Create( itemid );
		if( !item )
			return NULL;

		item->Init();
		item->SetID(itemid);
		if( !item->OnCreate(templateid) )
		{
			CObjMgr< CItem, ItemID >::Delete( item->GetID() );
			return NULL;
		}

		return item;
	}

	inline void Delete(ItemID id)
	{
		CItem* item = GetObj(id);
		if( item )
			item->Release();
		CObjMgr< CItem, ItemID >::Delete(id, item);

		LOGGER_DEBUG("[CItemMgr] Delete Item:"INT64_FMT, id);
	}

};

#define ItemMgr CItemMgr::getSingleton()

#endif	//GAMESERVER_ITEMMGR_H
