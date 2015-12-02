--群攻，额外增加以自身生命上限为15%比例的固定伤害
Effect_2123 = {}
Effect_2123.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	
	local hits = 0;
	local extraHp = GetMemberAttribInt(attackerIdx , 16) * 0.15;
	--循环遍历数组
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;

				ChangeMemberAttribInt(v, 2, -extraHp);
			end
		end
	end

	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
