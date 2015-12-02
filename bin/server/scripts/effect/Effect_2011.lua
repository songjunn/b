--绝技技能 攻击目标并随机选取另一个敌对目标造成伤害
Effect_2011 = {}
Effect_2011.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local hits = 0;
	local damage1 = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	if(damage1 > 0) then
		hits = hits + 1;
	end

	local tarRole = Effect_2011.GetRandomTarget(tarIdx);
	local damage2 = 0;
	if(tarRole >= 0) then
		damage2 = CombatOnAttack(attackerIdx, skill, tarRole, hits);
	end

	--增长怒气
	if (damage1 + damage2 > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end

Effect_2011.GetRandomTarget = function(tarIdx)

	local minidx, maxidx;
	if(tarIdx >= 0 and tarIdx < 9) then
		minidx = 0;
		maxidx = 8;
	elseif(tarIdx >= 9 and tarIdx < 18) then
		minidx = 9;
		maxidx = 17;
	end

	local irand = RandomInt(minidx, maxidx);

	local another;
	for i = 1, 9 do
		another = (irand + i) % 9 + minidx;
		if(IsFighter(another) and not IsFighterDead(another)) then
			return another;
		end
	end

	return -1;
end
