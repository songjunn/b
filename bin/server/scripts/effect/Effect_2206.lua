--攻击所有敌方目标并给自身增加一个护盾，在护盾破碎前不受伤害。护盾血量为自身气血上限20%。
Effect_2206 = {}
Effect_2206.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

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
		end
	end

	local addHp = GetMemberAttribInt(attackerIdx, 16) * value / 100;
	GainMemberHp(attackerIdx, addHp);
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
