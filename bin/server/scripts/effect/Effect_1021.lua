--特性技能效果，如果目标是普通体型，伤害增加/减少x%
Effect_1021 = {}
Effect_1021.OnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 151, value);
	else
		ChangePercent(role, 155, -value);
	end
end

Effect_1021.UnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 151, -value);
	else
		ChangePercent(role, 155, value);
	end
end
