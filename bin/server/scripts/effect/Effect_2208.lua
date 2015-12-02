--攻击敌方目标所在一行的所有目标并回复造成总伤害的20%的自身气血。
Effect_2208 = {}
Effect_2208.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取一行目标
	local targets = CommonFuncs.GetRowTargets(tarIdx);
	
	local totalDamage = 0;
	local hits = 0;
	--循环对每个目标进行伤害计算
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
				totalDamage = totalDamage + damage;
			end
		end
	end

	local addHp = totalDamage * value / 100;
	GainMemberHp(attackerIdx, addHp);
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
