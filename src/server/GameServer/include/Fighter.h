#pragma once
#include "IBaseObj.h"
#include "utlmap.h"
#include "eventdef.h"
#include "ItemUnit.h"

#define FIGHTER_LEVEL_MAX	99
#define ROLE_SKILL_MAX		10

struct FlagBit
{
	unsigned int bitOnEffect		: 1;
	unsigned int bitMoralCost		: 1;
	unsigned int bitHaveRevive		: 1;
	unsigned int bitShareDamage		: 1;
	unsigned int bitInvincible		: 1;
	unsigned int bitFixStuntDamage	: 1;
	unsigned int bitFixedKill		: 1;
	unsigned int bitCanRevive		: 1;
	unsigned int bitSilent			: 1;
	//...
};

class CPlayer;
class CFighter : public IBaseObj
{
public:
	CFighter();
	virtual ~CFighter();

	void	Init();
	void	Release();

	inline	bool	IsPlayer()						{ return m_type == Role_Type_Player; }
	inline	bool	IsHero()						{ return m_type == Role_Type_Hero; }
	inline	bool	IsMonster()						{ return m_type == Role_Type_Monster; }

	inline	bool	IsBerserker()					{return Role_Vocation_Berserker == m_Vocation;}
	inline	bool	IsMage()						{return Role_Vocation_Mage == m_Vocation;}
	inline	bool	IsRobbers()						{return Role_Vocation_Robbers == m_Vocation;}

	inline	bool	IsMale()						{return Role_Sex_Male == m_Sex;}
	inline	bool	IsFemale()						{return Role_Sex_Female == m_Sex;}

	inline	bool	IsCommonBody()					{return Role_Body_Common == m_Body;}
	inline	bool	IsLargeBody()					{return Role_Body_Large == m_Body;}
	inline	bool	IsHugeBody()					{return Role_Body_Huge == m_Body;}

	inline	bool	CanAttack()						{return !IsDizzy();}

	inline	bool	IsDizzy()						{return m_Dizzy;}
	inline	void	SetDizzy(bool flag)				{m_Dizzy = flag;}

	inline	bool	IsConfusion()					{return m_Confusion;}
	inline	void	SetConfusion(bool flag)			{m_Confusion = flag;}

	inline	bool	IsDead()						{return m_Dead; /*return GetFieldInt(Role_Attrib_Hp) <= 0;*/}
	inline	void	Die()							{m_Dead = true;}
	inline  void	Revive()						{m_Dead = false;}

	virtual void	Copy(const CFighter& fighter);

	virtual void	GainExp(int64 value) { return; }
	virtual void	TeamGainExp(int64 value) { return; }
	virtual void	GainStateExp(int64 value) { return; }
	virtual bool	GainGold(int nGold, GOLD_REASON reason) { return true; }
	virtual bool	GainSilver(int nSilver, SILVER_REASON reason) { return true; }
	virtual void	GainApLimit(int value) { return; }
	virtual void	GainMerit(int value, MERIT_REASON reason) { return; }

	void			GainHp(int value);

	void	OnLevelup(int up = 1);

	void	InitBaseAttr(bool client = false);

	virtual CPlayer* GetPlayer()	{return NULL;}

	void	SyncBaseAttrInfo(CPlayer* toplayer) { return; }	//同步战斗所需属性

protected:
	virtual int*	_FindFieldInt(int i);
	virtual int64*	_FindFieldI64(int i);

	virtual void	_ChangeRelatedField(int i, bool client = false, bool data = false);

protected:
	int		m_Vocation;			//职业
	int		m_Sex;				//性别
	int		m_Level;			//等级
	int		m_SceneID;			//场景ID
	int		m_Position;			//场景坐标
	int		m_TeamPos;			//阵型坐标
	int		m_Hp;				//当前生命值
	int		m_Morale;			//士气
	int		m_Counter;			//反击率
	int		m_FloatProb;		//造成浮空几率
	int		m_Sun;				//星宿属性：日
	int		m_Moon;				//星宿属性：月
	int		m_Stars;			//星宿属性：星
	SkillID	m_CommSkill;		//普通攻击技能
	SkillID	m_StuntSkill;		//使用的绝技
	SkillID	m_FinalSkill;		//使用的终结技
	int		m_Fighting;			//战斗力
	int		m_Body;				//体型
	int		m_StarsID;			//星盘ID
	int		m_Quality;			//品质
	int		m_QualityProgress;	//品质进度

	int		m_BaseStrength;		//基础力量
	int		m_BaseIntellect;	//基础智力
	int		m_BaseTechnique;	//基础技巧
	int		m_BaseAgility;		//基础敏捷

	int		m_TrainStrength;	//训练的力量成长
	int		m_TrainIntellect;	//训练的智力成长
	int		m_TrainTechnique;	//训练的技巧成长
	int		m_TrainAgility;		//训练的敏捷成长

	int		m_Strength[Attrib_Size];	//力量
	int		m_Intellect[Attrib_Size];	//智力
	int		m_Technique[Attrib_Size];	//技巧
	int		m_Agility[Attrib_Size];		//敏捷

	int		m_BaseHit;			//基础命中值
	int		m_BaseJouk;			//基础闪避值
	int		m_BaseCrit;			//基础暴击值
	int		m_BaseTenacity;		//基础韧性值
	int		m_BaseParry;		//基础格挡值
	int		m_BaseTreat;		//基础治疗力

	int		m_Hit[Attrib_Size];		//命中值
	int		m_Jouk[Attrib_Size];	//闪避值
	int		m_Crit[Attrib_Size];	//暴击值
	int		m_Tenacity[Attrib_Size];//韧性值
	int		m_Parry[Attrib_Size];	//格挡值
	int		m_Treat[Attrib_Size];	//治疗力

	int		m_BaseHpMax;		//基础生命值上限
	int		m_BasePhysiDamage;	//基础物理攻击
	int		m_BasePhysiDefense;	//基础物理防御
	int		m_BaseMagicDamage;	//基础魔法攻击
	int		m_BaseMagicDefense;	//基础魔法防御
	int		m_BaseStuntDamage;	//基础绝技攻击
	int		m_BaseStuntDefense;	//基础绝技防御

	int		m_HpMax[Attrib_Size];			//生命值上限
	int		m_PhysiDamage[Attrib_Size];		//物理攻击
	int		m_PhysiDefense[Attrib_Size];	//物理防御
	int		m_MagicDamage[Attrib_Size];		//魔法攻击
	int		m_MagicDefense[Attrib_Size];	//魔法防御
	int		m_StuntDamage[Attrib_Size];		//绝技攻击
	int		m_StuntDefense[Attrib_Size];	//绝技防御

	int		m_PhysiRealDamageIncr;		//物理增伤值
	int		m_PhysiRealDamageIncrPer;	//物理增伤百分比
	int		m_PhysiRealDamageReducePer;	//物理减伤百分比
	int		m_MagicRealDamageIncr;		//魔法增伤值
	int		m_MagicRealDamageIncrPer;	//魔法增伤百分比
	int		m_MagicRealDamageReducePer;	//魔法减伤百分比
	int		m_StuntRealDamageIncr;		//绝技增伤值
	int		m_StuntRealDamageIncrPer;	//绝技增伤百分比
	int		m_StuntRealDamageReducePer;	//绝技减伤百分比

	int		m_MoraleRevert;		//士气回复
	int		m_HpRevert;			//生命回复

	bool	m_Dizzy;			//眩晕状态
	bool	m_Confusion;		//混乱状态

	int		m_Hotohori;			//星宿:日/月/星
	bool	m_Dead;				//死亡标志

	int		m_InitStuntID;		//天生的绝技技能

	//战斗时存储被动技能的作用效果(不存盘，不读表)
	int		m_CombatAllPer;
	int		m_CombatCrossPer;
	int		m_CombatRowPer;
	int		m_CombatLinePer;
	int		m_HotohoriDamageIncPer;
	int		m_SunDamageIncPer;
	int		m_MoonDamageIncPer;
	int		m_StarDamageIncPer;
	int		m_HotohoriDamageRedPer;
	int		m_SunDamageRedPer;
	int		m_MoonDamageRedPer;
	int		m_StarDamageRedPer;
	int		m_BodyDamageIncPer;
	int		m_CommonDamageIncPer;
	int		m_LargeDamageIncPer;
	int		m_HugeDamageIncPer;
	int		m_BodyDamageRedPer;
	int		m_CommonDamageRedPer;
	int		m_LargeDamageRedPer;
	int		m_HugeDamageRedPer;
	int		m_ReducePlace;
	int		m_ReduceFloat;
	int		m_ReduceCounter;
	int		m_IncreaseCounter;

	FlagBit m_BitFlag;					//战斗中需要的标志
	int		m_iFlag;
	int		m_HpCombatInit;				//战斗初始血值

	int			m_MoraleAttack;				//恢复士气值
	int			m_MoraleBeAttack;
	int			m_MoraleRound;

public:
	bool	m_bHit;						//是否击中敌人

	SkillID				m_Skills[ROLE_SKILL_MAX];
	CUtlMap<int, int>	m_Buffs;	//战斗中buff
	CUtlMap<int, int>	m_ExBuffs;  //非战斗中添加的buff

public:
	CItemUnit	m_ItemUnit;

};

