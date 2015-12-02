--道具使用效果 非战斗中按百分比同时改变气血最大值和当前气血值 + x%
Effect_1029 = {}
Effect_1029.OnEffect = function(role, attr, value)

	local ival = GetRoleFieldInt(role, 2);
	local iaddval = value * ival / 100;

	ChangeRoleFieldInt(role, 2, iaddval);
	ChangeRoleFieldInt(role, 16, iaddval);
end

Effect_1029.UnEffect = function(role, attr, value)

	local ival = GetRoleFieldInt(role, 2);
	local iaddval = value * ival / (100 + value);

	ChangeRoleFieldInt(role, 2, -iaddval);
	ChangeRoleFieldInt(role, 16, -iaddval);
end

