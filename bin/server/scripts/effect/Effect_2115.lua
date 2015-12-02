--群攻，打行，并使目标降低10%所有防御，持续2回合
Effect_2115 = {}
Effect_2115.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取一行敌人
	local targets = CommonFuncs.GetRowTargets(tarIdx);
	
	local hits = 0;
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
				--增加一个buffer
				AddCommbatBuff(v, value, false);
			end
		end
	end
	
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
