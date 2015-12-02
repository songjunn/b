--单体，若使目标死亡则额外获得一次技能攻击(可秒杀全场的补刀英雄)。
Effect_2134 = {}
Effect_2134.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local hits = 0;
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	if (damage > 0) then
		hits = hits + 1;
	end

	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end

	if(IsFighterDead(tarIdx) == true) then
	--攻击者额外获得一次技能
		Effect_2134.AttackAnotherAgain(attackerIdx, skill);
	end
end

Effect_2134.AttackAnotherAgain = function(attackerIdx, skill)
	local minIdx = 0;
	local maxIdx = 8;
	if(tarIdx > 8) then
		minIdx = 9;
		maxIdx = 17;
	end

	for i = minIdx, maxIdx do
		if(IsFighter(i) and IsFighterDead(i) == false) then
			AddAction();
			Effect_2134.OnEffect(attackerIdx, skill, 0, 0, i);
			return;
		end
	end
end
