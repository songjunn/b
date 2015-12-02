--特性技能效果，如果目标是大体型，伤害增加/减少x%
Effect_1022 = {}
Effect_1022.OnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 152, value);
	else
		ChangePercent(role, 156, -value);
	end
end

Effect_1022.UnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 152, -value);
	else
		ChangePercent(role, 156, value);
	end
end
