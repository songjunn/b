#pragma once
#include "commdata.h"
#include "Singleton.h"
#include "ObjMgr.h"
#include "utlmap.h"

#define ROLE_SPEC_SKILL		5	//英雄特性技能数量

struct CRoleTemplate
{
	int		m_Id;			//编号
	//char	m_Name[OBJ_NAME_LEN];	//名字
	//char	m_Title[OBJ_NAME_LEN];	//称号
	int		m_Quality;		//品质
	int		m_Sex;			//性别
	int		m_Career;		//职业
	int		m_Strength;		//力量
	int		m_Intelli;		//智力
	int		m_Technic;		//技巧
	int		m_Agility;		//敏捷
	int		m_Hp;			//生命值
	int		m_PhysiDamage;	//物理攻击
	int		m_MagicDamage;	//魔法攻击
	int		m_StuntDamage;	//绝技攻击
	int		m_PhysiDefense;	//物理防御
	int		m_MagicDefense;	//魔法防御
	int		m_StuntDefense;	//绝技防御
	int		m_Hit;			//命中值
	int		m_Jouk;			//闪避值
	int		m_Crit;			//暴击值
	int		m_Tenacity;		//韧性值
	int		m_Parry;		//格挡值
	int		m_Treat;		//治疗力
	int		m_Counter;		//反击率
	int		m_MoraleRevert;	//士气回复
	int		m_HpRevert;		//生命回复
	int		m_StarsID;		//星盘
	int		m_Hotohori;		//星宿:日/月/星
	SkillID	m_AttackID;		//普攻ID
	SkillID	m_StuntID;		//绝技ID
	SkillID	m_FinalID;		//终结技ID
	SkillID	m_SpecSkill[ROLE_SPEC_SKILL];			//特性技能
	SkillID	m_PrivateSkill;							//私有特性技能
	int		m_MoraleAttack;	//恢复士气值
	int		m_MoraleBeAttack;
	int		m_MoraleRound;

};

class CRoleTemplateMgr : public CObjMgr< CRoleTemplate, int >, public Singleton< CRoleTemplateMgr >
{
public:
	CRoleTemplateMgr();
	~CRoleTemplateMgr();

	bool	LoadCSVData(const char* csvfile);

	bool	IsPrivateSkill(SkillID skillId);
	int		GetPrivateSkillCount();

private:
	CUtlMap<SkillID, SkillID> m_mapPrivateSkills;
};

#define RoleTemplateMgr CRoleTemplateMgr::getSingleton()
