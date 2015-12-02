--群攻，打列，并使目标降低10%所有攻击，持续2回合
Effect_2116 = {}
Effect_2116.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取一列的敌人
	local targets = CommonFuncs.GetColumnTargets(tarIdx);

	local hits = 0;
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if(damage > 0) then
				hits = hits + 1;
				--为敌人增加一个buffer
				AddCommbatBuff(v, value, false);
			end
		end
	end
	
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
