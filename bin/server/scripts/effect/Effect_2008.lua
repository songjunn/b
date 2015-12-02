--绝技技能 绝技普通一击
Effect_2008 = {}
Effect_2008.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, 0);
	
	if(damage > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end

