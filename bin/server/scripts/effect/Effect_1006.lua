--改变战斗中的自身属性+队伍中女性队友数量*n%
Effect_1006 = {}
Effect_1006.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local num = GetFemaleNumInTeam(attackerIdx);
	if( num > 0 ) then
		ChangeMemberAttribInt(attackerIdx, attr, num * value);
	end
	
end

Effect_1006.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local num = GetFemaleNumInTeam(attackerIdx);
	if( num > 0 ) then
		ChangeMemberAttribInt(attackerIdx, attr, -num * value);
	end
	
end
