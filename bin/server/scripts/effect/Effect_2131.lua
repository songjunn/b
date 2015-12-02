--单体，65%使目标混乱攻击其己方目标(若无己方目标则呆立不动)，持续1回合
Effect_2131 = {}
Effect_2131.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local hits = 0;
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	if (damage > 0) then
		hits = hits + 1;
	end
	
	local x = RandomInt(1, 100);
	if x <= 65 then
		if GetCombatFlag(4) == 0 then
			AddCommbatBuff(tarIdx, value, false);
		end
	
	end
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
