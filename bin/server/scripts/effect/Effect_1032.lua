--国战中同时改变经验、金钱、位阶经验、战功、攻击防御增益
Effect_1032 = {}
Effect_1032.OnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, Role_Attrib_ExpAddition, value)
	ChangeRoleFieldInt(role, Role_Attrib_MeritAddition, value)
	ChangeRoleFieldInt(role, Role_Attrib_SilverAddition, value)
	ChangeRoleFieldInt(role, Role_Attrib_StateExpAddition, value)
	ChangeRoleFieldInt(role, Role_Attrib_PhysiDamagePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_PhysiDefensePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDamagePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDefensePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDamagePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDefensePercent, value)
	
end

Effect_1032.UnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, Role_Attrib_ExpAddition, -value)
	ChangeRoleFieldInt(role, Role_Attrib_MeritAddition, -value)
	ChangeRoleFieldInt(role, Role_Attrib_SilverAddition, -value)
	ChangeRoleFieldInt(role, Role_Attrib_StateExpAddition, -value)
	ChangeRoleFieldInt(role, Role_Attrib_PhysiDamagePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_PhysiDefensePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDamagePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDefensePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDamagePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDefensePercent, -value)
	
end
