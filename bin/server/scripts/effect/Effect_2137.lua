--群攻，随机攻击三下(先选定目标，可能攻击到后排目标，也可能三发命中同一目标即使第一发时已经死亡)
Effect_2137 = {}
Effect_2137.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};

	local hits = 0;
	local ran = 0;
	local damage = 0;
	--随机攻击三个
	for i = 1, 3 do
		ran = RandomInt(1, table.getn(targets));
		local defenderIdx = targets[ran];
		if(defenderIdx >= 0 and IsFighterDead(defenderIdx) == false) then
			damage = CombatOnAttack(attackerIdx, skill, defenderIdx, hits);
			if(damage > 0) then
				hits = hits + 1;
			end
		end
	end

	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
