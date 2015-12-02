--单体，释放时使所有友方目标满士气，并增加自身20%闪避持续2回合
Effect_2107 = {}
Effect_2107.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local hits = 0;
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, hits);

	--设置队伍中所有角色满士气
	Effect_2107.AllMembersFullMorale(attackerIdx);

	if (damage > 0) then
		hits = hits + 1;
		--为自己增加buferr
		AddCommbatBuff(attackerIdx, attackerIdx, value);
		ProcMemberBuff(attackerIdx, attackerIdx, value);
	end
	
	if(hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end

Effect_2107.AllMembersFullMorale = function(attackerIdx)

	local minidx, maxidx;
	if(attackerIdx >= 0 and attackerIdx < 9) then
		minidx = 0;
		maxidx = 8;
	elseif(attackerIdx >= 9 and attackerIdx < 18) then
		minidx = 9;
		maxidx = 17;
	end

	for i = minidx, maxidx do
		if(IsFighter(i) and not IsFighterDead(i)) then
			ChangeMemberAttribInt(i, 1, 100 - GetMemberAttribInt(i, 1));
		end
	end
end

