--群攻，对每个目标20%概率使其士气归零。
Effect_2205 = {}
Effect_2205.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

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

			local ranVal = RandomInt(1, 100);
			if(ranVal <= value) then
				ChangeMemberAttribInt(v, 1, -GetMemberAttribInt(v, 1));
			end
		end
	end
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
