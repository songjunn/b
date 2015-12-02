--群攻，释放时额外增加20%部位破坏概率
Effect_2136 = {}
Effect_2136.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	
	ChangeMemberAttribInt(attackerIdx, 139, 20);

	local hits = 0;
	--循环遍历数组
	for k, v in pairs(targets) do
		if (v >= 0) then		
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end
		end
	end

	ChangeMemberAttribInt(attackerIdx, 139, -20);

	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end