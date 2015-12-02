--绝技技能 自己生命越少造成伤害越高 0生命时是满生命的两倍
Effect_2006 = {}
Effect_2006.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, 0);
	
	if(damage > 0) then
		local curhp = GetMemberAttribInt(attackerIdx, 2);
		local maxhp = GetMemberAttribInt(attackerIdx, 16);
		local dagval = (maxhp - curhp) * damage / maxhp;
		GainMemberHp(tarIdx, -dagval);

		AddTargetTeamAnger(attackerIdx);
	end
end

