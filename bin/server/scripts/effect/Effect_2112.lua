--攻击所有敌方目标且额外造成相当于自身已损失生命值的伤害。
Effect_2112 = {}
Effect_2112.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--损失的生命值
	local maxhp = GetMemberAttribInt(attackerIdx , 16);
	local hp =  GetMemberAttribInt(attackerIdx , 2);
	local _hp = maxhp - hp;

	local hits = 0;
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	for k, v in pairs(targets) do
		if (v >= 0) then
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
				--额外造成的伤害:自身已经损失的生命值
				GainMemberHp(v, -_hp);
			end
		end
	end
	
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
