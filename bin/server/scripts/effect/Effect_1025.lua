--特性技能效果，日属性克制造成的伤害增加/减少x%
Effect_1025 = {}
Effect_1025.OnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 143, value);
	else
		ChangePercent(role, 147, -value);
	end
end

Effect_1025.UnEffect = function(role, attr, value)

	if(value > 0) then
		ChangePercent(role, 143, -value);
	else
		ChangePercent(role, 147, value);
	end
end
