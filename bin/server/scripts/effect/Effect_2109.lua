--群攻，并降低所有目标10%命中持续2回合
Effect_2109 = {}
Effect_2109.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	local targets = { GetTargetTeamAllMember(attackerIdx, tarIdx) };
	local hits = 0;
	for k, v in pairs(targets) do
		if( v > 0) then
			local damage = CombatOnAttack(attackerIdx , skill , v , hits);
			if(damage > 0) then
				hits = hits + 1;
				AddCommbatBuff(v , value, false);
			end
		end
	end
	
	if( hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end