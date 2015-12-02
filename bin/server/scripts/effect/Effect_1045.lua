--改变自身x%所有攻击
Effect_1045 = {}
Effect_1045.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	ChangeMemberAttribInt(attackerIdx, 68, value);
	ChangeMemberAttribInt(attackerIdx, 70, value);
	ChangeMemberAttribInt(attackerIdx, 72, value);
end

Effect_1045.UnEffect = function(role, attr, value)
	
	ChangeMemberAttribInt(attackerIdx, 68, -value);
	ChangeMemberAttribInt(attackerIdx, 70, -value);
	ChangeMemberAttribInt(attackerIdx, 72, -value);
end
