--群攻，自身当前生命值越高造成伤害越高，即额外增加以自身当前生命/(2.5*生命上限)为系数的伤害
Effect_2110 = {}
Effect_2110.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	local hits = 0;

	local factor = GetMemberAttribInt(2) / (GetMemberAttribInt(16) * 2.5);

	--循环对每个目标进行伤害计算
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end

			GainMemberHp(v, -factor * damage);
		end
	end
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end