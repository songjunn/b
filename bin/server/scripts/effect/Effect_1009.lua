--Change非战斗中的本体int属性
Effect_1009 = {}
Effect_1009.OnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, attr, value)
	
end

Effect_1009.UnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, attr, -value)
	
end
