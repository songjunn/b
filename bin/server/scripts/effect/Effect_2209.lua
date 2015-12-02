--攻击所有敌方目标并使其士气减少20(不能低于0)。
Effect_2209 = {}
Effect_2209.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

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

			local curMorle = GetMemberAttribInt(v, 1);
			if(value > curMorle) then
				value = curMorle
			end
			ChangeMemberAttribInt(v, 1, -value);
		end
	end
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
