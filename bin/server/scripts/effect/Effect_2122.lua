--群攻，造成伤害为绝技伤害+普攻魔法伤害
Effect_2122 = {}
Effect_2122.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	
	--获取魔法的伤害
	local magicDamage = GetMemberAttribInt(attackerIdx , 19);
	--绝技伤害 额外 增加一个魔法的伤害
	ChangeMemberAttribInt(attackerIdx, 21, magicDamage);

	--循环遍历数组
	local hits = 0;
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end
		end
	end

	ChangeMemberAttribInt(attackerIdx, 21, -magicDamage);

	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
