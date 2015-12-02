--绝技技能 发动后，己方所有角色士气增加到x(已经到达或者超过x的无效果)
Effect_2012 = {}
Effect_2012.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, 0);
	
	if(damage > 0) then
		AddTargetTeamAnger(attackerIdx);
	end

	local teamRoles = Effect_2012.GetMyAllMembers(attackerIdx);

	for k, v in pairs(teamRoles) do
		if (v >= 0) then
			local moral = GetMemberAttribInt(v, 1);
			if(moral < value) then
				ChangeMemberAttribInt(v, 1, value - moral);
			end
		end
	end
end

Effect_2012.GetMyAllMembers = function(attackerIdx)

	local allMembers = {};
	local minidx, maxidx;
	if(attackerIdx >= 0 and attackerIdx < 9) then
		minidx = 0;
		maxidx = 8;
	elseif(attackerIdx >= 9 and attackerIdx < 18) then
		minidx = 9;
		maxidx = 17;
	end

	for i = minidx, maxidx do
		if(IsFighter(i) and not IsFighterDead(i)) then
			table.insert(allMembers, i);
		end
	end

	return allMembers;
end
