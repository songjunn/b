--绝技技能 以概率100%多攻击一次， 释放时加20%暴击
Effect_2010 = {}
Effect_2010.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	ChangeMemberAttribInt(attackerIdx, 63, 20);

	local hits = 0;
	local damage1 = CombatOnAttack(attackerIdx, skill, tarIdx, hits);

	--增长怒气
	if (damage1 > 0) then
		hits = hits + 1;

		AddTargetTeamAnger(attackerIdx);
	end

	local damage2 = 0;
	if(IsFighterDead(tarIdx) == false) then
		AddAction();
		damage2 = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	end

	--增长怒气
	if (damage2 > 0) then
		AddTargetTeamAnger(attackerIdx);
	end

	ChangeMemberAttribInt(attackerIdx, 63, -20);
end

