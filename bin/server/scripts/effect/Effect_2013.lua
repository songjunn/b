--绝技技能 发动后不减少士气，可以持续发动绝技攻击
Effect_2013 = {}
Effect_2013.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, 0);
	
	if(damage > 0) then
		AddTargetTeamAnger(attackerIdx);
	end

	SetBitFlag(attackerIdx, 138, 1);
	SetCheckAttr(attackerIdx, 1);
end

