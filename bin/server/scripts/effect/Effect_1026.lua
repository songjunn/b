--特性技能效果，月属性克制造成的伤害增加/减少x%
Effect_1026 = {}
Effect_1026.OnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 144, value);
	else
		ChangePercent(role, 148, -value);
	end
end

Effect_1026.UnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 144, -value);
	else
		ChangePercent(role, 148, value);
	end
end
