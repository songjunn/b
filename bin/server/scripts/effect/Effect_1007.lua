--Set非战斗中的本体int属性
Effect_1007 = {}
Effect_1007.OnEffect = function(role, attr, value)

	SetRoleFieldInt(role, attr, value, false, false)
	
end

Effect_1007.UnEffect = function(role, attr, value)

	SetRoleFieldInt(role, attr, -value, false, false)
	
end
