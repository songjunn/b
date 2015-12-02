--特性技能效果，如果目标是巨大体型，伤害增加/减少x%
Effect_1023 = {}
Effect_1023.OnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 153, value);
	else
		ChangePercent(role, 157, -value);
	end
end

Effect_1023.UnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 153, -value);
	else
		ChangePercent(role, 157, value);
	end
end
