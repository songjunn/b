#include "Fighter.h"
#include "Event.h"
#include "RoleTemplate.h"
#include "LuaEngine.h"
#include "Player.h"

CFighter::CFighter()
{
	
}

CFighter::~CFighter()
{
}

void CFighter::Release()
{
	m_ItemUnit.Release();
}

void CFighter::Init()
{
	IBaseObj::Init();

	m_Level = 0;
	m_SceneID = 0;		
	m_Position = 0;	
	m_TeamPos = 0;
	m_Hp = 0;			
	m_BaseStrength = 0;
	m_BaseIntellect = 0;
	m_BaseTechnique = 0;
	m_BaseAgility = 0;
	m_BaseHit = 0;			
	m_BaseJouk = 0;			
	m_BaseCrit = 0;		
	m_BaseTenacity = 0;		
	m_BaseParry = 0;		
	m_BaseTreat = 0;		
	m_Morale = 0;		
	m_Counter = 0;	
	m_FloatProb = 0;
	m_Sun = 0;
	m_Moon = 0;
	m_Stars = 0;
	m_CommSkill = -1;
	m_Fighting = 0;
	m_Body = 0;
	m_StarsID = 0;
	m_Quality = 0;
	m_Fighting = 0;
	m_QualityProgress = 0;

	m_TrainStrength = 0;
	m_TrainIntellect = 0;
	m_TrainTechnique = 0;
	m_TrainAgility = 0;

	m_BaseHpMax = 0;		
	m_BasePhysiDamage = 0;
	m_BasePhysiDefense = 0;
	m_BaseMagicDamage = 0;
	m_BaseMagicDefense = 0;
	m_BaseStuntDamage = 0;
	m_BaseStuntDefense = 0;

	m_PhysiRealDamageIncr = 0;	
	m_PhysiRealDamageIncrPer = 0;
	m_PhysiRealDamageReducePer = 0;
	m_MagicRealDamageIncr = 0;	
	m_MagicRealDamageIncrPer = 0;
	m_MagicRealDamageReducePer = 0;
	m_StuntRealDamageIncr = 0;	
	m_StuntRealDamageIncrPer = 0;
	m_StuntRealDamageReducePer = 0;

	for(int i=0; i<Attrib_Size; ++i)
	{
		m_Strength[i] = 0;		
		m_Intellect[i] = 0;		
		m_Technique[i] = 0;		
		m_Agility[i] = 0;		
		m_Hit[i] = 0;		
		m_Jouk[i] = 0;		
		m_Crit[i] = 0;		
		m_Tenacity[i] = 0;		
		m_Parry[i] = 0;		
		m_Treat[i] = 0;		

		m_HpMax[i] = 0;		
		m_PhysiDamage[i] = 0;	
		m_PhysiDefense[i] = 0;	
		m_MagicDamage[i] = 0;	
		m_MagicDefense[i] = 0;	
		m_StuntDamage[i] = 0;	
		m_StuntDefense[i] = 0;	
	}

	m_Dead = false;
	m_Dizzy = false;;
	m_Confusion = false;

	m_Hotohori = 0;

	m_bHit = true;
	m_CombatAllPer = 0;
	m_CombatCrossPer = 0;
	m_CombatRowPer = 0;
	m_CombatLinePer = 0;
	m_HotohoriDamageIncPer = 0;
	m_SunDamageIncPer = 0;
	m_MoonDamageIncPer = 0;
	m_StarDamageIncPer = 0;
	m_HotohoriDamageRedPer = 0;
	m_SunDamageRedPer = 0;
	m_MoonDamageRedPer = 0;
	m_StarDamageRedPer = 0;
	m_BodyDamageIncPer = 0;
	m_CommonDamageIncPer = 0;
	m_LargeDamageIncPer = 0;
	m_HugeDamageIncPer = 0;
	m_BodyDamageRedPer = 0;
	m_CommonDamageRedPer = 0;
	m_LargeDamageRedPer = 0;
	m_HugeDamageRedPer = 0;
	m_ReducePlace = 0;
	m_ReduceFloat = 0;
	m_ReduceCounter = 0;
	m_IncreaseCounter = 0;

	m_InitStuntID = 0;

	m_iFlag = 0;
	m_HpCombatInit = 0;

	m_MoraleAttack = 0;
	m_MoraleBeAttack = 0;
	m_MoraleRound = 0;

	m_ItemUnit.Initialize(this);
}

void CFighter::Copy(const CFighter& fighter)
{
	IBaseObj::Copy(fighter);

	m_Vocation = fighter.m_Vocation;	
	m_Sex = fighter.m_Sex;		
	m_Level = fighter.m_Level;
	m_SceneID = fighter.m_SceneID;		
	m_Position = fighter.m_Position;	
	m_TeamPos = fighter.m_TeamPos;
	m_BaseStrength = fighter.m_BaseStrength;		
	m_BaseIntellect = fighter.m_BaseIntellect;	
	m_BaseTechnique = fighter.m_BaseTechnique;	
	m_BaseAgility = fighter.m_BaseAgility;		
	m_Hp = fighter.m_Hp;			
	m_BaseHit = fighter.m_BaseHit;			
	m_BaseJouk = fighter.m_BaseJouk;			
	m_BaseCrit = fighter.m_BaseCrit;		
	m_BaseTenacity = fighter.m_BaseTenacity;		
	m_BaseParry = fighter.m_BaseParry;		
	m_BaseTreat = fighter.m_BaseTreat;		
	m_Morale = fighter.m_Morale;		
	m_Counter = fighter.m_Counter;		
	m_FloatProb = fighter.m_FloatProb;		
	m_Sun = fighter.m_Sun;
	m_Moon = fighter.m_Moon;
	m_Stars = fighter.m_Stars;
	m_CommSkill = fighter.m_CommSkill;
	m_StuntSkill = fighter.m_StuntSkill;
	m_FinalSkill = fighter.m_FinalSkill;
	m_Fighting = fighter.m_Fighting;
	m_Body = fighter.m_Body;
	m_StarsID = fighter.m_StarsID;
	m_Quality = fighter.m_Quality;
	m_Fighting = fighter.m_Fighting;
	m_QualityProgress = fighter.m_QualityProgress;

	m_TrainStrength = fighter.m_TrainStrength;		
	m_TrainIntellect = fighter.m_TrainIntellect;		
	m_TrainTechnique = fighter.m_TrainTechnique;		
	m_TrainAgility = fighter.m_TrainAgility;		

	m_BaseHpMax = fighter.m_BaseHpMax;		
	m_BasePhysiDamage = fighter.m_BasePhysiDamage;
	m_BasePhysiDefense = fighter.m_BasePhysiDefense;
	m_BaseMagicDamage = fighter.m_BaseMagicDamage;
	m_BaseMagicDefense = fighter.m_BaseMagicDefense;
	m_BaseStuntDamage = fighter.m_BaseStuntDamage;
	m_BaseStuntDefense = fighter.m_BaseStuntDefense;

	m_PhysiRealDamageIncr = fighter.m_PhysiRealDamageIncr;	
	m_PhysiRealDamageIncrPer = fighter.m_PhysiRealDamageIncrPer;
	m_PhysiRealDamageReducePer = fighter.m_PhysiRealDamageReducePer;
	m_MagicRealDamageIncr = fighter.m_MagicRealDamageIncr;	
	m_MagicRealDamageIncrPer = fighter.m_MagicRealDamageIncrPer;
	m_MagicRealDamageReducePer = fighter.m_MagicRealDamageReducePer;
	m_StuntRealDamageIncr = fighter.m_StuntRealDamageIncr;	
	m_StuntRealDamageIncrPer = fighter.m_StuntRealDamageIncrPer;
	m_StuntRealDamageReducePer = fighter.m_StuntRealDamageReducePer;

	for(int i=0; i<Attrib_Size; ++i)
	{
		m_Strength[i] = fighter.m_Strength[i];	
		m_Intellect[i] = fighter.m_Intellect[i];	
		m_Technique[i] = fighter.m_Technique[i];	
		m_Agility[i] = fighter.m_Agility[i];	
		m_Hit[i] = fighter.m_Hit[i];	
		m_Jouk[i] = fighter.m_Jouk[i];	
		m_Crit[i] = fighter.m_Crit[i];	
		m_Tenacity[i] = fighter.m_Tenacity[i];	
		m_Parry[i] = fighter.m_Parry[i];	
		m_Treat[i] = fighter.m_Treat[i];	

		m_HpMax[i] = fighter.m_HpMax[i];		
		m_PhysiDamage[i] = fighter.m_PhysiDamage[i];	
		m_PhysiDefense[i] = fighter.m_PhysiDefense[i];	
		m_MagicDamage[i] = fighter.m_MagicDamage[i];	
		m_MagicDefense[i] = fighter.m_MagicDefense[i];	
		m_StuntDamage[i] = fighter.m_StuntDamage[i];	
		m_StuntDefense[i] = fighter.m_StuntDefense[i];	
	}

	m_Dead = fighter.m_Dead;
	m_Dizzy = fighter.m_Dizzy;
	m_Confusion = fighter.m_Confusion;

	m_Hotohori = fighter.m_Hotohori;

	m_bHit = fighter.m_bHit;
	m_CombatAllPer = fighter.m_CombatAllPer;
	m_CombatCrossPer = fighter.m_CombatCrossPer;
	m_CombatRowPer = fighter.m_CombatRowPer;
	m_CombatLinePer = fighter.m_CombatLinePer;
	m_HotohoriDamageIncPer = fighter.m_HotohoriDamageIncPer;
	m_SunDamageIncPer = fighter.m_SunDamageIncPer;
	m_MoonDamageIncPer = fighter.m_MoonDamageIncPer;
	m_StarDamageIncPer = fighter.m_StarDamageIncPer;
	m_HotohoriDamageRedPer = fighter.m_HotohoriDamageRedPer;
	m_SunDamageRedPer = fighter.m_SunDamageRedPer;
	m_MoonDamageRedPer = fighter.m_MoonDamageRedPer;
	m_StarDamageRedPer = fighter.m_StarDamageRedPer;
	m_BodyDamageIncPer = fighter.m_BodyDamageIncPer;
	m_CommonDamageIncPer = fighter.m_CommonDamageIncPer;
	m_LargeDamageIncPer = fighter.m_LargeDamageIncPer;
	m_HugeDamageIncPer = fighter.m_HugeDamageIncPer;
	m_BodyDamageRedPer = fighter.m_BodyDamageRedPer;
	m_CommonDamageRedPer = fighter.m_CommonDamageRedPer;
	m_LargeDamageRedPer = fighter.m_LargeDamageRedPer;
	m_HugeDamageRedPer = fighter.m_HugeDamageRedPer;
	m_ReducePlace = fighter.m_ReducePlace;
	m_ReduceFloat = fighter.m_ReduceFloat;
	m_ReduceCounter = fighter.m_ReduceCounter;
	m_IncreaseCounter = fighter.m_IncreaseCounter;
	m_HpCombatInit = fighter.m_HpCombatInit;

	m_MoraleAttack = fighter.m_MoraleAttack;
	m_MoraleBeAttack = fighter.m_MoraleBeAttack;
	m_MoraleRound = fighter.m_MoraleRound;
}

int* CFighter::_FindFieldInt(int i)
{
	switch(i)
	{
	case Role_Attrib_TemplateID:	return &m_templateId;
	case Role_Attrib_Vocation:		return &m_Vocation;
	case Role_Attrib_Sex:			return &m_Sex;
	case Role_Attrib_Level:			return &m_Level;
	case Role_Attrib_Hp:			return &m_Hp;
	case Role_Attrib_SceneID:		return &m_SceneID;
	case Role_Attrib_Position:		return &m_Position;
	case Role_Attrib_Fighting:		return &m_Fighting;

	case Role_Attrib_BaseStrength:		return &m_BaseStrength;
	case Role_Attrib_BaseIntellect:		return &m_BaseIntellect;
	case Role_Attrib_BaseTechnique:		return &m_BaseTechnique;
	case Role_Attrib_BaseAgility:		return &m_BaseAgility;

	case Role_Attrib_Strength:		return &m_Strength[Attrib_Value];
	case Role_Attrib_Intellect:		return &m_Intellect[Attrib_Value];
	case Role_Attrib_Technique:		return &m_Technique[Attrib_Value];
	case Role_Attrib_Agility:		return &m_Agility[Attrib_Value];

	case Role_Attrib_StrengthAddons:	return &m_Strength[Attrib_Addons];
	case Role_Attrib_IntellectAddons:	return &m_Intellect[Attrib_Addons];
	case Role_Attrib_TechniqueAddons:	return &m_Technique[Attrib_Addons];
	case Role_Attrib_AgilityAddons:		return &m_Agility[Attrib_Addons];

	case Role_Attrib_StrengthPercent:	return &m_Strength[Attrib_Percent];
	case Role_Attrib_IntellectPercent:	return &m_Intellect[Attrib_Percent];
	case Role_Attrib_TechniquePercent:	return &m_Technique[Attrib_Percent];
	case Role_Attrib_AgilityPercent:	return &m_Agility[Attrib_Percent];
	
	case Role_Attrib_BaseHit:			return &m_BaseHit;
	case Role_Attrib_BaseJouk:			return &m_BaseJouk;
	case Role_Attrib_BaseCrit:			return &m_BaseCrit;
	case Role_Attrib_BaseTenacity:		return &m_BaseTenacity;
	case Role_Attrib_BaseParry:			return &m_BaseParry;
	case Role_Attrib_BaseTreat:			return &m_BaseTreat;

	case Role_Attrib_Hit:			return &m_Hit[Attrib_Value];
	case Role_Attrib_Jouk:			return &m_Jouk[Attrib_Value];
	case Role_Attrib_Crit:			return &m_Crit[Attrib_Value];
	case Role_Attrib_Tenacity:		return &m_Tenacity[Attrib_Value];
	case Role_Attrib_Parry:			return &m_Parry[Attrib_Value];
	case Role_Attrib_Treat:			return &m_Treat[Attrib_Value];

	case Role_Attrib_HitAddons:			return &m_Hit[Attrib_Addons];
	case Role_Attrib_JoukAddons:		return &m_Jouk[Attrib_Addons];
	case Role_Attrib_CritAddons:		return &m_Crit[Attrib_Addons];
	case Role_Attrib_TenacityAddons:	return &m_Tenacity[Attrib_Addons];
	case Role_Attrib_ParryAddons:		return &m_Parry[Attrib_Addons];
	case Role_Attrib_TreatAddons:		return &m_Treat[Attrib_Addons];

	case Role_Attrib_HitPercent:		return &m_Hit[Attrib_Percent];
	case Role_Attrib_JoukPercent:		return &m_Jouk[Attrib_Percent];
	case Role_Attrib_CritPercent:		return &m_Crit[Attrib_Percent];
	case Role_Attrib_TenacityPercent:	return &m_Tenacity[Attrib_Percent];
	case Role_Attrib_ParryPercent:		return &m_Parry[Attrib_Percent];
	case Role_Attrib_TreatPercent:		return &m_Treat[Attrib_Percent];

	case Role_Attrib_BaseHpMax:			return &m_BaseHpMax;
	case Role_Attrib_BasePhysiDamage:	return &m_BasePhysiDamage;
	case Role_Attrib_BasePhysiDefense:	return &m_BasePhysiDefense;
	case Role_Attrib_BaseMagicDamage:	return &m_BaseMagicDamage;
	case Role_Attrib_BaseMagicDefense:	return &m_BaseMagicDefense;
	case Role_Attrib_BaseStuntDamage:	return &m_BaseStuntDamage;
	case Role_Attrib_BaseStuntDefense:	return &m_BaseStuntDefense;

	case Role_Attrib_HpMax:			return &m_HpMax[Attrib_Value];
	case Role_Attrib_PhysiDamage:	return &m_PhysiDamage[Attrib_Value];
	case Role_Attrib_PhysiDefense:	return &m_PhysiDefense[Attrib_Value];
	case Role_Attrib_MagicDamage:	return &m_MagicDamage[Attrib_Value];
	case Role_Attrib_MagicDefense:	return &m_MagicDefense[Attrib_Value];
	case Role_Attrib_StuntDamage:	return &m_StuntDamage[Attrib_Value];
	case Role_Attrib_StuntDefense:	return &m_StuntDefense[Attrib_Value];

	case Role_Attrib_HpMaxAddons:			return &m_HpMax[Attrib_Addons];
	case Role_Attrib_PhysiDamageAddons:		return &m_PhysiDamage[Attrib_Addons];
	case Role_Attrib_PhysiDefenseAddons:	return &m_PhysiDefense[Attrib_Addons];
	case Role_Attrib_MagicDamageAddons:		return &m_MagicDamage[Attrib_Addons];
	case Role_Attrib_MagicDefenseAddons:	return &m_MagicDefense[Attrib_Addons];
	case Role_Attrib_StuntDamageAddons:		return &m_StuntDamage[Attrib_Addons];
	case Role_Attrib_StuntDefenseAddons:	return &m_StuntDefense[Attrib_Addons];

	case Role_Attrib_HpMaxPercent:			return &m_HpMax[Attrib_Percent];
	case Role_Attrib_PhysiDamagePercent:	return &m_PhysiDamage[Attrib_Percent];
	case Role_Attrib_PhysiDefensePercent:	return &m_PhysiDefense[Attrib_Percent];
	case Role_Attrib_MagicDamagePercent:	return &m_MagicDamage[Attrib_Percent];
	case Role_Attrib_MagicDefensePercent:	return &m_MagicDefense[Attrib_Percent];
	case Role_Attrib_StuntDamagePercent:	return &m_StuntDamage[Attrib_Percent];
	case Role_Attrib_StuntDefensePercent:	return &m_StuntDefense[Attrib_Percent];

	case Role_Attrib_PhysiRealDamageIncr:		return &m_PhysiRealDamageIncr;
	case Role_Attrib_PhysiRealDamageIncrPer:	return &m_PhysiRealDamageIncrPer;
	case Role_Attrib_PhysiRealDamageReducePer:	return &m_PhysiRealDamageReducePer;
	case Role_Attrib_MagicRealDamageIncr:		return &m_MagicRealDamageIncr;
	case Role_Attrib_MagicRealDamageIncrPer:	return &m_MagicRealDamageIncrPer;
	case Role_Attrib_MagicRealDamageReducePer:	return &m_MagicRealDamageReducePer;
	case Role_Attrib_StuntRealDamageIncr:		return &m_StuntRealDamageIncr;
	case Role_Attrib_StuntRealDamageIncrPer:	return &m_StuntRealDamageIncrPer;
	case Role_Attrib_StuntRealDamageReducePer:	return &m_StuntRealDamageReducePer;

	default:	return NULL;
	}
	return NULL;
}

int64* CFighter::_FindFieldI64(int i)
{
	switch(i)
	{
	case Role_Attrib_ID:	return &m_ObjID;
	default:	return NULL;
	}
	return NULL;
}

void CFighter::_ChangeRelatedField(int i, bool client, bool data)
{
	switch(i)
	{
	case Role_Attrib_BaseHpMax:
	case Role_Attrib_HpMaxAddons:
	case Role_Attrib_HpMaxPercent:
		{
			float percent = 1.00f + (float)m_HpMax[Attrib_Percent] / 100;
			int value = (m_BaseHpMax + m_HpMax[Attrib_Addons]) * percent;
			SetFieldInt(Role_Attrib_HpMax, value, client);
			SetFieldInt(Role_Attrib_Hp, value, client);
		}
		break;
	case Role_Attrib_BasePhysiDamage:
	case Role_Attrib_PhysiDamageAddons:
	case Role_Attrib_PhysiDamagePercent:
		{
			int value = (m_BasePhysiDamage + m_PhysiDamage[Attrib_Addons]) * (100 + m_PhysiDamage[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_PhysiDamage, value, client);
		}
		break;
	case Role_Attrib_BasePhysiDefense:
	case Role_Attrib_PhysiDefenseAddons:
	case Role_Attrib_PhysiDefensePercent:
		{
			int value = (m_BasePhysiDefense + m_PhysiDefense[Attrib_Addons]) * (100 + m_PhysiDefense[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_PhysiDefense, value, client);
		}
		break;
	case Role_Attrib_BaseMagicDamage:
	case Role_Attrib_MagicDamageAddons:
	case Role_Attrib_MagicDamagePercent:
		{
			int value = (m_BaseMagicDamage + m_MagicDamage[Attrib_Addons]) * (100 + m_MagicDamage[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_MagicDamage, value, client);
		}
		break;
	case Role_Attrib_BaseMagicDefense:
	case Role_Attrib_MagicDefenseAddons:
	case Role_Attrib_MagicDefensePercent:
		{
			int value = (m_BaseMagicDefense + m_MagicDefense[Attrib_Addons]) * (100 + m_MagicDefense[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_MagicDefense, value, client);
		}
		break;
	case Role_Attrib_BaseStuntDamage:
	case Role_Attrib_StuntDamageAddons:
	case Role_Attrib_StuntDamagePercent:
		{
			int value = (m_BaseStuntDamage + m_StuntDamage[Attrib_Addons]) * (100 + m_StuntDamage[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_StuntDamage, value, client);
		}
		break;
	case Role_Attrib_BaseStuntDefense:
	case Role_Attrib_StuntDefenseAddons:
	case Role_Attrib_StuntDefensePercent:
		{
			int value = (m_BaseStuntDefense + m_StuntDefense[Attrib_Addons]) * (100 + m_StuntDefense[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_StuntDefense, value, client);
		}
		break;
	case Role_Attrib_BaseStrength:
	case Role_Attrib_StrengthAddons:
	case Role_Attrib_StrengthPercent:
		{
			int value = (m_BaseStrength + m_Strength[Attrib_Addons]) * (100 + m_Strength[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Strength, value, client);
		}
		break;
	case Role_Attrib_BaseIntellect:
	case Role_Attrib_IntellectAddons:
	case Role_Attrib_IntellectPercent:
		{
			int value = (m_BaseIntellect + m_Intellect[Attrib_Addons]) * (100 + m_Intellect[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Intellect, value, client);
		}
		break;
	case Role_Attrib_BaseTechnique:
	case Role_Attrib_TechniqueAddons:
	case Role_Attrib_TechniquePercent:
		{
			int value = (m_BaseTechnique + m_Technique[Attrib_Addons]) * (100 + m_Technique[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Technique, value, client);
		}
		break;
	case Role_Attrib_BaseAgility:
	case Role_Attrib_AgilityAddons:
	case Role_Attrib_AgilityPercent:
		{
			int value = (m_BaseAgility + m_Agility[Attrib_Addons]) * (100 + m_Agility[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Agility, value, client);
		}
		break;
	case Role_Attrib_BaseHit:
	case Role_Attrib_HitAddons:
	case Role_Attrib_HitPercent:
		{
			int value = (m_BaseHit + m_Hit[Attrib_Addons]) * (100 + m_Hit[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Hit, value, client);
		}
		break;
	case Role_Attrib_BaseJouk:
	case Role_Attrib_JoukAddons:
	case Role_Attrib_JoukPercent:
		{
			int value = (m_BaseJouk + m_Jouk[Attrib_Addons]) * (100 + m_Jouk[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Jouk, value, client);
		}
		break;
	case Role_Attrib_BaseCrit:
	case Role_Attrib_CritAddons:
	case Role_Attrib_CritPercent:
		{
			int value = (m_BaseCrit + m_Crit[Attrib_Addons]) * (100 + m_Crit[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Crit, value, client);
		}
		break;
	case Role_Attrib_BaseTenacity:
	case Role_Attrib_TenacityAddons:
	case Role_Attrib_TenacityPercent:
		{
			int value = (m_BaseTenacity + m_Tenacity[Attrib_Addons]) * (100 + m_Tenacity[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Tenacity, value, client);
		}
		break;
	case Role_Attrib_BaseParry:
	case Role_Attrib_ParryAddons:
	case Role_Attrib_ParryPercent:
		{
			int value = (m_BaseParry + m_Parry[Attrib_Addons]) * (100 + m_Parry[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Parry, value, client);
		}
		break;
	case Role_Attrib_BaseTreat:
	case Role_Attrib_TreatAddons:
	case Role_Attrib_TreatPercent:
		{
			int value = (m_BaseTreat + m_Treat[Attrib_Addons]) * (100 + m_Treat[Attrib_Percent]) / 100;
			SetFieldInt(Role_Attrib_Treat, value, client);
		}
		break;
	case Role_Attrib_Strength:
	case Role_Attrib_Intellect:
	case Role_Attrib_Technique:
	case Role_Attrib_Agility:
	case Role_Attrib_Hit:
	case Role_Attrib_Jouk:
	case Role_Attrib_Crit:
	case Role_Attrib_Tenacity:
	case Role_Attrib_Parry:
	case Role_Attrib_Treat:
	case Role_Attrib_HpMax:
	case Role_Attrib_PhysiDamage:
	case Role_Attrib_PhysiDefense:
	case Role_Attrib_MagicDamage:
	case Role_Attrib_MagicDefense:
	case Role_Attrib_StuntDamage:
	case Role_Attrib_StuntDefense:
		{
		}
		break;
	default:
		break;
	}
}

void CFighter::GainHp(int value)
{
	if(value == 0) {
		return;
	}

	int curHp = GetFieldInt(Role_Attrib_Hp);
	int maxHp = GetFieldInt(Role_Attrib_HpMax);

	if(curHp + value <= 0) {
		SetFieldInt(Role_Attrib_Hp, 0);
	}
	else if(curHp + value >= maxHp) {
		SetFieldInt(Role_Attrib_Hp, maxHp);
	}
	else {
		ChangeFieldInt(Role_Attrib_Hp, value);
	}
}

void CFighter::OnLevelup(int up)
{
	ChangeFieldInt(Role_Attrib_Level, up, true);
	InitBaseAttr(true);
	//SyncFieldToData("attr");

	if( this->IsPlayer() )
	{
		CEvent* ev = MakeEvent(Event_Player_Levelup, GetID(), (int64)GetFieldInt(Role_Attrib_Level), NULL, true);
		OnEvent(ev);
	}
	else
	{
		CPlayer* player = this->GetPlayer();
		if( player )
		{
			CEvent* ev = MakeEvent(Event_Hero_Levelup, player->GetID(), GetID(), (int64)GetFieldInt(Role_Attrib_Level), NULL, true);
			player->OnEvent(ev);
		}
	}
}

void CFighter::InitBaseAttr(bool client)
{
	CRoleTemplate* role = RoleTemplateMgr.GetObj( GetTemplateID() );
	if( !role )
		return;
}


