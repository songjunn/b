--攻击目标及其横向一行
Effect_2002 = {}
Effect_2002.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	--获取全部攻击目标
	local targetRole = CommonFuncs.GetRowTargets(tarIdx);

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
