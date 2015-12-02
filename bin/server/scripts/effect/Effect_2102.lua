--单体，减免自身受到伤害50%持续2回合
Effect_2102 = {}
Effect_2102.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local hits = 0;
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	if (damage > 0) then
		hits = hits + 1;
	end
	
	AddCommbatBuff(attackerIdx, value, true);
	ProcMemberBuff(attackerIdx, value, true);
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
