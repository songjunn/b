--攻击对方全体目标,提高15%暴击率
Effect_2114 = {}
Effect_2114.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	ChangeMemberAttribInt(attackerIdx, 63, 15);
	
	local hits = 0;
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end
		end
	end
	
	ChangeMemberAttribInt(attackerIdx, 63, -15);
	
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
