--道具使用效果 非战斗中同时改变气血最大值和当前气血值 + x
Effect_1028 = {}
Effect_1028.OnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, 2, value);
	ChangeRoleFieldInt(role, 16, value);
end

Effect_1028.UnEffect = function(role, attr, value)

	ChangeRoleFieldInt(role, 2, -value);
	ChangeRoleFieldInt(role, 16, -value);
end
