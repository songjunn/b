--绝技技能 攻击成功时恢复自身气血 数值为造成伤害的x%
Effect_2007 = {}
Effect_2007.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, 0);
	
	if(damage > 0) then
		local hpval = value * damage / 100;
		GainMemberHp(attackerIdx, hpval);
		SetCheckAttr(attackerIdx, 2);

		AddTargetTeamAnger(attackerIdx);
	end
end

