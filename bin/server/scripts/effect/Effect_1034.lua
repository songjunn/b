--国战中同时改变物理、魔法、绝技防御增益
Effect_1034 = {}
Effect_1034.OnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, Role_Attrib_PhysiDefensePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDefensePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDefensePercent, value)
	
end

Effect_1034.UnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, Role_Attrib_PhysiDefensePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDefensePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDefensePercent, -value)
	
end
