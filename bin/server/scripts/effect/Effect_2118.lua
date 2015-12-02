--自由之翼   单体，并随机以30%生命复活一个阵亡友方英雄
Effect_2118 = {}
Effect_2118.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取攻击目标
	local hits = 0;
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	if (damage > 0) then
		hits = hits + 1;
	end
		
	MemberRevive(attackerIdx, 30);

	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end