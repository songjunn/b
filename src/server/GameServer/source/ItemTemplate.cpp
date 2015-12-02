#include "ItemTemplate.h"
#include "CSVParser.h"


CItemTemplateMgr::CItemTemplateMgr()
{
}

CItemTemplateMgr::~CItemTemplateMgr()
{
}


int CItemTemplate::GetFieldInt(int i)
{
	switch(i)
	{
		case Item_Attrib_Strength:	//力量
			return m_Strength; 				
									break;
		case Item_Attrib_Intellect: //智力
			return m_Intellect;
									break;
		case Item_Attrib_Technique: //技巧
			return m_Technique;
									break;		
		case Item_Attrib_Agility:		//敏捷
			return m_Agility;
									break;		
		case Item_Attrib_Hit:			//命中值
			return m_Hit;
									break;		
		case Item_Attrib_Jouk:		//闪避值
			return m_Jouk;
									break;		
		case Item_Attrib_Crit:		//暴击值
			return m_Crit;
									break;		
		case Item_Attrib_Tenacity:	//韧性值
			return m_Tenacity;
									break;		
		case Item_Attrib_Parry: 	//格挡值
			return m_Treat;
									break;		
		case Item_Attrib_Treat: 	//治疗力
			return m_Treat;
									break;		
		case Item_Attrib_BaseHpMax: 	//生命值上限
			return m_BaseHpMax;
									break;		
		case Item_Attrib_BasePhysiDamage:	//物理攻击
			return m_BasePhysiDamage;
									break;		
		case Item_Attrib_BasePhysiDefense: //物理防御
			return m_BasePhysiDefense;
									break;		
		case Item_Attrib_BaseMagicDamage:	//魔法攻击
			return m_BaseMagicDamage;
									break;		
		case Item_Attrib_BaseMagicDefense: //魔法防御
			return m_BaseMagicDefense;
									break;		
		case Item_Attrib_BaseStuntDamage:	//绝技攻击
			return m_BaseStuntDamage;		
									break;		
		case Item_Attrib_BaseStuntDefense: //绝技防御
			return m_BaseStuntDefense;
									break;			
		default:
			return NULL;
			break;
	}
	return NULL;
}

bool CItemTemplateMgr::LoadCSVData(const char* csvfile)
{
	CCSVParser file;
	if( !csvfile || !file.Load(csvfile) )
	{
		LOGGER_ERROR("[InitData] Load File Failed:%s", csvfile);
		return false;
	}

	int index = 0;
	int count = file.Line();

	if( !Initialize("ItemTemplate", count) )
		return false;

	for(int i = 0;i<count; ++i, index=0)
	{
		int id = 0;
		file.GetAttribute(i, index++, id);

		CItemTemplate* item = Create( id );
		if( !item )
			return false;

		item->m_Id = id;
		index++;	//name
		//file.GetAttribute(i, index++, item->m_Name);
		file.GetAttribute(i, index++, item->m_Type);
		file.GetAttribute(i, index++, item->m_Career);
		file.GetAttribute(i, index++, item->m_Sex);
		file.GetAttribute(i, index++, item->m_MinLevel);
		file.GetAttribute(i, index++, item->m_Quatily);
		file.GetAttribute(i, index++, item->m_StackMax);
		file.GetAttribute(i, index++, item->m_SellPrice);

		file.GetAttribute(i, index++, item->m_EffectUse.effectID);
		file.GetAttribute(i, index++, item->m_EffectUse.effectValue);
		file.GetAttribute(i, index++, item->m_EffectUse.effectProbability);
		file.GetAttribute(i, index++, item->m_EffectUse.effectAttr);

		for(int n=0; n<EQUIP_EFFECT_MAX; ++n)
		{
			file.GetAttribute(i, index++, item->m_EffectEquip[n].effectID);
			file.GetAttribute(i, index++, item->m_EffectEquip[n].effectValue);
			file.GetAttribute(i, index++, item->m_EffectEquip[n].effectProbability);
			file.GetAttribute(i, index++, item->m_EffectEquip[n].effectAttr);
		}

		file.GetAttribute(i, index++, item->m_Strength);
		file.GetAttribute(i, index++, item->m_Intellect);
		file.GetAttribute(i, index++, item->m_Technique);
		file.GetAttribute(i, index++, item->m_Agility);
		
		file.GetAttribute(i, index++, item->m_Hit);
		file.GetAttribute(i, index++, item->m_Jouk);
		file.GetAttribute(i, index++, item->m_Crit);
		file.GetAttribute(i, index++, item->m_Tenacity);		
		file.GetAttribute(i, index++, item->m_Parry);
		file.GetAttribute(i, index++, item->m_Treat);

		file.GetAttribute(i, index++, item->m_BaseHpMax);
		file.GetAttribute(i, index++, item->m_BasePhysiDamage);
		file.GetAttribute(i, index++, item->m_BasePhysiDefense);
		file.GetAttribute(i, index++, item->m_BaseMagicDamage);		
		file.GetAttribute(i, index++, item->m_BaseMagicDefense);
		file.GetAttribute(i, index++, item->m_BaseStuntDamage);
		file.GetAttribute(i, index++, item->m_BaseStuntDefense);

		file.GetAttribute(i, index++, item->m_Intensify);
		file.GetAttribute(i, index++, item->m_MaxLevel_intensify);
		//for(int n=0; n<Intensify_Points_ENDS; n++)		
		//	file.GetAttribute(i, index++, item->m_Intensify_points[n]);
		file.GetAttribute(i, index++, item->m_SoulType);
		file.GetAttribute(i, index++, item->m_SoulPoints);

		//for(int h=0; h<CHARGE; h++)
		//	file.GetAttribute(i, index++, item->m_charge[h]);//手续费


		//魂晶
		for(int k=0; k<ITEM_ARRTU; k++)
		{
			file.GetAttribute(i, index++, item->m_crystal[k].soulEffectID);
			file.GetAttribute(i, index++, item->m_crystal[k].soulEffectValue);
			file.GetAttribute(i, index++, item->m_crystal[k].soulEffectProbability);
			file.GetAttribute(i, index++, item->m_crystal[k].soulEffectAttr);
			file.GetAttribute(i, index++, item->m_crystal[k].soulInspirType);
			file.GetAttribute(i, index++, item->m_crystal[k].soulInspirGradeCounts);			
		}

		for(int k=0; k<ARR_LENS; k++)  //装备升级材料
		{
			file.GetAttribute(i, index++, item->m_UpGrade[k].templateId);
			file.GetAttribute(i, index++, item->m_UpGrade[k].nums);			
		}

		file.GetAttribute(i, index++, item->m_NextGradeTemplateId);
		file.GetAttribute(i, index++, item->m_UpGradeCharge);
		
		file.GetAttribute(i, index++, item->m_IntensifyJadePoints);
		file.GetAttribute(i, index++, item->m_IntensifyJadeBasePoints);
		file.GetAttribute(i, index++, item->m_LayCharge);
	}

	file.Release();

	return true;
}
