--单体，每次攻击有30%概率连击(可无限攻击一个敌人致死)
Effect_2135 = {}
Effect_2135.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local hits = 0;
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	if (damage > 0) then
		hits = hits + 1;
	end

	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
	local ran = RandomInt(1, 100);
	if(ran <= 30) then
		Effect_2135.AttackAgain(attackerIdx, skill, tarIdx);
	end
end

Effect_2135.AttackAgain = function(attackerIdx, skill, tarIdx)
	if(IsFighterDead(tarIdx) == false) then
		AddAction();
		Effect_2135.OnEffect(attackerIdx, skill, 0, 0, tarIdx);
	end
end
