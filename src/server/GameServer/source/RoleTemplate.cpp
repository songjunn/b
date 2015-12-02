#include "RoleTemplate.h"
#include "CSVParser.h"


CRoleTemplateMgr::CRoleTemplateMgr()
{
	SetDefLessFunc(m_mapPrivateSkills);
}

CRoleTemplateMgr::~CRoleTemplateMgr()
{
}

bool CRoleTemplateMgr::LoadCSVData(const char* csvfile)
{
	CCSVParser file;
	if( !csvfile || !file.Load(csvfile) )
	{
		LOGGER_ERROR("[InitData] Load File Failed:%s", csvfile);
		return false;
	}

	int index = 0;
	int count = file.Line();

	if( !Initialize("RoleTemplate", count) )
		return false;

	for(int i = 0;i<count; ++i, index=0)
	{
		int id = 0;
		file.GetAttribute(i, index++, id);

		CRoleTemplate* role = Create( id );
		if( !role )
			return false;

		role->m_Id = id;
		//index++;	//name
		//index++;	//title
		//file.GetAttribute(i, index++, role->m_Name);
		//file.GetAttribute(i, index++, role->m_Title);
		file.GetAttribute(i, index++, role->m_Quality);
		file.GetAttribute(i, index++, role->m_Sex);
		file.GetAttribute(i, index++, role->m_Career);
		file.GetAttribute(i, index++, role->m_Strength);
		file.GetAttribute(i, index++, role->m_Intelli);
		file.GetAttribute(i, index++, role->m_Technic);
		file.GetAttribute(i, index++, role->m_Agility);
		file.GetAttribute(i, index++, role->m_Hp);
		file.GetAttribute(i, index++, role->m_PhysiDamage);
		file.GetAttribute(i, index++, role->m_MagicDamage);
		file.GetAttribute(i, index++, role->m_StuntDamage);
		file.GetAttribute(i, index++, role->m_PhysiDefense);
		file.GetAttribute(i, index++, role->m_MagicDefense);
		file.GetAttribute(i, index++, role->m_StuntDefense);
		file.GetAttribute(i, index++, role->m_Hit);
		file.GetAttribute(i, index++, role->m_Jouk);
		file.GetAttribute(i, index++, role->m_Crit);
		file.GetAttribute(i, index++, role->m_Tenacity);
		file.GetAttribute(i, index++, role->m_Parry);
		file.GetAttribute(i, index++, role->m_Treat);
		file.GetAttribute(i, index++, role->m_Counter);
		file.GetAttribute(i, index++, role->m_MoraleRevert);
		file.GetAttribute(i, index++, role->m_HpRevert);
		file.GetAttribute(i, index++, role->m_AttackID);
		file.GetAttribute(i, index++, role->m_StuntID);
		file.GetAttribute(i, index++, role->m_FinalID);
		
		for(int x=0; x<ROLE_SPEC_SKILL; ++x)
		{
			file.GetAttribute(i, index++, role->m_SpecSkill[x]);
		}
		file.GetAttribute(i, index++, role->m_PrivateSkill);

		file.GetAttribute(i, index++, role->m_StarsID);
		file.GetAttribute(i, index++, role->m_Hotohori);
		file.GetAttribute(i, index++, role->m_MoraleAttack);
		file.GetAttribute(i, index++, role->m_MoraleBeAttack);
		file.GetAttribute(i, index++, role->m_MoraleRound);

		m_mapPrivateSkills.Insert(role->m_PrivateSkill, i);
	}

	file.Release();

	return true;
}

bool CRoleTemplateMgr::IsPrivateSkill(SkillID skillId)
{
	int idx = m_mapPrivateSkills.Find(skillId);
	return m_mapPrivateSkills.IsValidIndex(idx);
}

int	CRoleTemplateMgr::GetPrivateSkillCount()
{
	return m_mapPrivateSkills.Count();
}

