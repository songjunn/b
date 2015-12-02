#pragma once
#include "attrs.h"
#include "gamedef.h"

const std::string GROUP_ATTRS = "attrs";
const std::string GROUP_ITEMS = "items";

//CPlayer
static const attrs::CAttr player_attrs[] =
{
	{ Role_Attrib_Null,			"null" },
	{ Role_Attrib_ID,			"playerid" },
	{ Role_Attrib_UserID,		"userid" },
	{ Role_Attrib_Name,			"name" },
	{ Role_Attrib_SceneID,		"sceneid" },
	{ Role_Attrib_Position,		"position" },
	{ Role_Attrib_TemplateID,	"templateid" },
	{ Role_Attrib_Vocation,		"vocation" },
	{ Role_Attrib_Sex,			"sex" },
	{ Role_Attrib_Level,		"level" },
	{ Role_Attrib_Exp,			"exp" },
	{ Role_Attrib_GoldCoin,		"goldcoin" },
	{ Role_Attrib_SilverCoin,	"silvercoin" },
	{ Role_Attrib_Fighting,		"fighting" },
	{ Role_Attrib_CreateTime,	"createtime" },
	{ Role_Attrib_LoginTime,	"logintime"},
	{ Role_Attrib_LogoutTime,	"logouttime" },
};

//CItem
static const attrs::CAttr item_attrs[] =
{
	{ Item_Attrib_Null,			"null" },
	{ Item_Attrib_ID,			"itemid" },
	{ Item_Attrib_Parent,		"parent" },
	{ Item_Attrib_TemplateID,	"templateid" },
	{ Item_Attrib_Position,		"position" },
	{ Item_Attrib_StackSize,	"stack" },
	{ Item_Attrib_EquipID,		"equipid" },
	{ Item_Attrib_Intensify,	"intensify" },
};