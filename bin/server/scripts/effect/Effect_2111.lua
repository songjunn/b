--单体，释放时增加15%暴击概率，并暴击时使目标混乱攻击其己方目标，持续1回合
Effect_2111 = {}
Effect_2111.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	ChangeMemberAttribInt(attackerIdx, 63, 15);
	
	local hits = 0;
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, hits);
	if (damage > 0) then
		hits = hits + 1;
	end

	ChangeMemberAttribInt(attackerIdx, 63, -15);
	
	--暴击?
	if GetCombatFlag(8) ~= 0 then
		--增加一个buffer，产生混乱
		AddCommbatBuff(tarIdx, value, false);
	end
	
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
