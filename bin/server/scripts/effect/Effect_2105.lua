--单体，吸血(自身造成伤害的50%)，并增加自身格挡概率20%，持续2回合
Effect_2105 = {}
Effect_2105.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	local hits = 0;
	local damage = CombatOnAttack(attackerIdx , skill , tarIdx, hits);
	if(damage > 0) then
		hits = hits + 1;
		--吸血
		GainMemberHp(attackerIdx, damage / 2);
	end
	
	AddCommbatBuff(attackerIdx , value , true);
	ProcMemberBuff(attackerIdx , value , true);

	if( hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
