--攻击目标及以目标为中心的十字单位
Effect_2004 = {}
Effect_2004.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	--获取全部攻击目标
	local targetRole = CommonFuncs.GetCrossTargets(tarIdx);

	local hits = 0;
	--循环对每个目标进行伤害计算
	for k, v in pairs(targetRole) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end
		end
	end
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
