--攻击所有敌方目标并各有20%概率使其沉默一回和(无法使用技能)。
Effect_2210 = {}
Effect_2210.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};

	local hits = 0;
	--循环对每个目标进行伤害计算
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end

			local ranNum = RandomInt(1, 100);
			if(ranNum <= 20) then
				AddCommbatBuff(v, value, false);
			end
		end
	end
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
