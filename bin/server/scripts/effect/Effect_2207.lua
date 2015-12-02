--攻击敌方目标所在一列的所有目标并给自身增加一个持续2回合不受负面效果的效果。
Effect_2207 = {}
Effect_2207.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取一列目标
	local targets = CommonFuncs.GetColumnTargets(tarIdx);
	
	local hits = 0;
	--循环对每个目标进行伤害计算
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end
		end
	end

	AddCommbatBuff(attackerIdx, value, true);
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
