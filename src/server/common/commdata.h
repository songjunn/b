#ifndef COMMON_COMMDATA_H
#define COMMON_COMMDATA_H

#include "types.h"
#include "gamedef.h"

typedef int64	UserID;							// 游戏内部帐号ID
typedef int64	OpUserID;						// 运营平台账号ID
typedef int64	GuestID;						// 游客账号ID
typedef int64	PersonID;						// 玩家角色的ID	-1 为无效值
typedef int64	ItemID;							// 道具ID
typedef int64	TMV;							// 时间类型
typedef int		SkillID;						// 技能ID
typedef int		EffectID;						// 效果ID
typedef int		BuffID;							// buffer
typedef int		EventID;						// event


//无效值定义
#define INVALID_TIME				0xFFFFFFFFFFFFFFFFULL			// 无效的时间
#define INVALID_VALUE				-1								// 无效的值
#define INVALID_VALUE64				(int64)0xFFFFFFFFFFFFFFFFULL			//
#define INVALID_UINT64				0xFFFFFFFFFFFFFFFFULL			//
#define INVALID_PID					0xFFFFFFFFFFFFFFFFULL			// 无效的PID
#define INVALID_UID					0xFFFFFFFFFFFFFFFFULL			// 无效的UID

#define DEVICE_STRING_LEN			128
#define	OBJ_NAME_LEN				64

#define	FORMAT_TILE_XY		3
#define FORMAT_TILE_MAX		(FORMAT_TILE_XY * FORMAT_TILE_XY)	//阵型格子

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Obj流水号生成规则

//worldId最多为三位数1-999
enum OBJID_KEY
{
	GUEST_KEY = 1,
	PLAYER_KEY,
	ITEM_KEY,
	HERO_KEY,
};
//ID规则：服编号（3位）基础Obj类型（player, guest）（2位）基础Obj自增索引（7位）扩展Obj类型（item, hero）（2位）扩展Obj自增索引（5位）
#define OBJ_BIT		100					//2bit
#define EXTOBJ_BIT	100000				//5bit
#define BASEOBJ_BIT	100000000000000		//14bit

inline PersonID g_MakeInitGuestID(int worldId)		{ return (worldId * OBJ_BIT + GUEST_KEY) * BASEOBJ_BIT; }
inline PersonID g_MakeInitPlayerID(int worldId)		{ return (worldId * OBJ_BIT + PLAYER_KEY) * BASEOBJ_BIT; }
inline PersonID	g_MakeInitHeroID(PersonID player)	{ return player + HERO_KEY * EXTOBJ_BIT; }
inline ItemID	g_MakeInitItemID(PersonID player)	{ return player + ITEM_KEY * EXTOBJ_BIT; }

inline PersonID g_MakePlayerID(PersonID player)		{ return player + 1 * OBJ_BIT * EXTOBJ_BIT; }

inline bool	g_IsPlayerId(PersonID id)	{ return (PLAYER_KEY == id / BASEOBJ_BIT % OBJ_BIT); }
inline bool	g_IsHeroId(PersonID id)		{ return (HERO_KEY == id / EXTOBJ_BIT % OBJ_BIT); }

//游客账号与运营平台账号向游戏内部账号ID的转换
inline UserID MakeUserIdFromOpUser( OpUserID id )	{ return 1000000000000000000 + id; }
inline UserID MakeUserIdFromGuest( GuestID id )		{ return 2000000000000000000 + id; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//player在线标记类型
enum Online_Flag
{
	Online_Flag_Null = 0,
	Online_Flag_Load,		//加载中
	Online_Flag_On,			//在线
	Online_Flag_Off,		//离线
};

enum Attrib_Param
{
	Attrib_Value,
	Attrib_Addons,
	Attrib_Percent,

	Attrib_Size,
};

#define GET_FIGHTER( id, fighter )			\
	{										\
		if( g_IsHeroId(id) )				\
			fighter = HeroMgr.GetObj(id);	\
		else if( g_IsPlayerId(id) )			\
			fighter = PlayerMgr.GetObj(id);	\
	}										\

#endif	//COMMON_COMMDATA_H
