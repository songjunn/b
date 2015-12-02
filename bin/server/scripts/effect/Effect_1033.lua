--国战中同时改变物理、魔法、绝技攻击增益
Effect_1033 = {}
Effect_1033.OnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, Role_Attrib_PhysiDamagePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDamagePercent, value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDamagePercent, value)
	
end

Effect_1033.UnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, Role_Attrib_PhysiDamagePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_MagicDamagePercent, -value)
	ChangeRoleFieldInt(role, Role_Attrib_StuntDamagePercent, -value)
	
end
