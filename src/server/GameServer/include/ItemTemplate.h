#pragma once
#include "commdata.h"
#include "Singleton.h"
#include "ObjMgr.h"

struct SoulCrystal	//魂晶
{
	int	soulEffectID;			//效果ID
	int	soulEffectValue;		//效果值
	int	soulEffectProbability;	//效果几率
	int	soulEffectAttr;			//关联类型
	int soulInspirType;//激活所需要的魂玉种类
	int	soulInspirGradeCounts;//激活所需要的魂玉等级和
	bool isSpir;	//是否激活，默认是false未激活， true激活
};

struct UpGradeNeeds//装备升级  需要
{
	int templateId;
	int nums;
};


class CItemTemplate
{
public:
	int	GetFieldInt(int i);

public:
	int		m_Id;			//编号
	//char	m_Name[OBJ_NAME_LEN];	//名字
	int		m_Type;			//类型
	int		m_Career;		//职业
	int		m_Sex;			//性别
	int		m_MinLevel;		//最低使用等级
	int		m_Quatily;		//品质
	int		m_StackMax;		//堆叠上限
	int		m_SellPrice;	//出售价格
	CEffect	m_EffectUse;	//使用效果
	CEffect	m_EffectEquip[EQUIP_EFFECT_MAX];	//装备效果

	int 		m_Strength;             //力量
	int 		m_Intellect;            //智力
	int 		m_Technique;			//技巧
	int 		m_Agility;				//敏捷

	int 		m_Hit;					//命中值
	int			m_Jouk;					//闪避值
	int 		m_Crit;					//暴击值
	int 		m_Tenacity;				//韧性值
	int 		m_Parry;				//格挡值
	int 		m_Treat;				//治疗力

	int 		m_BaseHpMax;			//基础生命值上限
	int 		m_BasePhysiDamage;      //基础物理攻击
	int 		m_BasePhysiDefense;		//基础物理防御
	int 		m_BaseMagicDamage;		//基础魔法攻击
	int 		m_BaseMagicDefense;		//基础魔法防御
	int			m_BaseStuntDamage;		//基础绝技攻击
	int			m_BaseStuntDefense;		//基础绝技防御

	int 		m_Intensify;			//当前装备强化等级, 初始0
	int 		m_MaxLevel_intensify;	//装备强化上限
	//int 		m_Intensify_points[Intensify_Points_ENDS];		//从当前强化等级强化到下一级需求点数

	int 		m_SoulType;				//种类, 日1， 月2， 星3， -1
	int 		m_SoulPoints; 			//魂玉点数

	//int 		m_charge[CHARGE];		//手续费


	//魂晶
	SoulCrystal  m_crystal[ITEM_ARRTU];


	UpGradeNeeds m_UpGrade[ARR_LENS];//升级需要
	int			m_NextGradeTemplateId;  //下一等级模板id
	int			m_UpGradeCharge;       // 升级手续费

	int 		m_IntensifyJadePoints;
	int 		m_IntensifyJadeBasePoints;	
	int 		m_LayCharge;  //镶嵌手续费
};

class CItemTemplateMgr : public CObjMgr< CItemTemplate, int >, public Singleton< CItemTemplateMgr >
{
public:
	CItemTemplateMgr();
	~CItemTemplateMgr();

	bool	LoadCSVData(const char* csvfile);

protected:
	
};

#define ItemTemplateMgr CItemTemplateMgr::getSingleton()
