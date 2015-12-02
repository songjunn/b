--群攻，释放时使所有在吟唱技能的目标被打断
Effect_2124 = {}
Effect_2124.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local targets = { GetTargetTeamAllMember(attackerIdx, tarIdx) };
	local hits = 0;
	for k, v in pairs(targets) do
	    if(v >= 0) then
		local damage = CombatOnAttack(attackerIdx, skill, v, hits);
		if damage > 0 then
			hits = hits + 1;
			--中断吟唱
			--InterruptChant(attackerIdx , v);
		end
	    end
	end
	--增长怒气
	if hits > 0 then
		AddTargetTeamAnger(attackerIdx);
	end
end
