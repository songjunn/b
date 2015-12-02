--特性技能效果，星属性克制造成的伤害增加/减少x%
Effect_1027 = {}
Effect_1027.OnEffect = function(role, attr, value)
	
	if(value > 0) then
		ChangePercent(role, 145, value);
	else
		ChangePercent(role, 149, -value);
	end
end

Effect_1027.UnEffect = function(role, attr, value)
	
	if(value > 0) then
		ChangePercent(role, 145, -value);
	else
		ChangePercent(role, 149, value);
	end
end
