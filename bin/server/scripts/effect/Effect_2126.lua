--攻击对方全体目标,20%概率使所有目标眩晕持续2回合
Effect_2126 = {}
Effect_2126.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	
	local hits = 0;
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			
				local per = RandomInt(1, 100);
				if (per <= 20) then
					AddCommbatBuff(v, value, false);
				end
				
			end
		end
	end
	
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
