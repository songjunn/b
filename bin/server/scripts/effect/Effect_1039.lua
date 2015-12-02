--非战斗中按百分比改变本体int属性
Effect_1039 = {}
Effect_1039.OnEffect = function(role, attr, value)

	local ival = GetRoleFieldInt(role, attr);
	ival = ival * value / 100;
	ChangeRoleFieldInt(role, attr, ival);
	
end

Effect_1039.UnEffect = function(role, attr, value)

	local ival = GetRoleFieldInt(role, attr);
	ival = ival * value / (100 + value);
	ChangeRoleFieldInt(role, attr, -ival)
	
end
