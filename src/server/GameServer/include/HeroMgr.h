#ifndef GAMESERVER_HEROMGR_H
#define GAMESERVER_HEROMGR_H

#include "commdata.h"
#include "ObjMgr.h"
#include "Hero.h"


class CHeroMgr : public CObjMgr< CHero, PersonID >, public Singleton< CHeroMgr >
{
public:
	CHeroMgr()	{};
	~CHeroMgr()	{};

	inline CHero* Create(int templateid, PersonID heroid)
	{
		CHero* hero = CObjMgr< CHero, PersonID >::Create(heroid);
		if (!hero)
			return NULL;

		hero->Init();
		hero->SetID(heroid);
		if (!hero->OnCreate(templateid))
		{
			CObjMgr< CHero, PersonID >::Delete(hero->GetID());
			return NULL;
		}

		return hero;
	}

	inline void Delete(PersonID id)
	{
		CHero* hero = GetObj(id);
		if (hero)
			hero->Release();
		CObjMgr< CHero, PersonID >::Delete(id, hero);

		LOGGER_DEBUG("[CHeroMgr] Delete Hero:"INT64_FMT, id);
	}

};

#define HeroMgr CHeroMgr::getSingleton()

#endif	//GAMESERVER_HEROMGR_H
