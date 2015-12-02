--战斗中按百分比改变目标的Int属性
Effect_1020 = {}
Effect_1020.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local ival = GetMemberAttribInt(tarIdx, attr);
	ival = value * ival / 100;
	ChangeMemberAttribInt(tarIdx, attr, ival);
	
end

Effect_1020.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local ival = GetMemberAttribInt(tarIdx, attr);
	ival = value * ival / (100 + value);
	ChangeMemberAttribInt(tarIdx, attr, -ival);
	
end
