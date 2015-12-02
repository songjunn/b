--Change非战斗中的本体int64属性
Effect_1010 = {}
Effect_1010.OnEffect = function(role, attr, value)

	ChangeRoleFieldI64(role, attr, value)
	
end

Effect_1010.UnEffect = function(role, attr, value)

	ChangeRoleFieldI64(role, attr, -value)
	
end
