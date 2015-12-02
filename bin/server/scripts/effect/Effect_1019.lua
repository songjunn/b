--战斗中按百分比改变自身的Int属性
Effect_1019 = {}
Effect_1019.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local ival = GetMemberAttribInt(attackerIdx, attr);
	ival = value * ival / 100;
	ChangeMemberAttribInt(attackerIdx, attr, ival);
	
end

Effect_1019.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local ival = GetMemberAttribInt(attackerIdx, attr);
	ival = value * ival / (100 + value);
	ChangeMemberAttribInt(attackerIdx, attr, -ival);
	
end

