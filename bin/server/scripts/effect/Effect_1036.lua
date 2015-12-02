--非战斗中设置自身一些状态标志(是否受到buff/debuff、是否消耗士气、是否无敌状态...)
Effect_1036 = {}
Effect_1036.OnEffect = function(role, attr, value)

	SetRoleFlag(role, attr, value);
	
end

Effect_1036.UnEffect = function(role, attr, value)

	SetRoleFlag(role, attr, 0);
	
end


