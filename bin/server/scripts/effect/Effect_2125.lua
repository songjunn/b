--全体攻击，无视目标防御。
Effect_2125 = {}
Effect_2125.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};
	
	local hits = 0;
	--循环遍历数组
	for k, v in pairs(targets) do
		if (v >= 0) then
			--将目标的防御置为0后，进入战斗计算
			local x = GetMemberAttribInt(v , 18);
			local y = GetMemberAttribInt(v , 20);
			local z = GetMemberAttribInt(v , 22);

			--将目标的物理、魔法、绝技防御都置为0
			ChangeMemberAttribInt(v, 18, -x);
			ChangeMemberAttribInt(v, 20, -y);
			ChangeMemberAttribInt(v, 22, -z);

			--进入战斗
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end

			--战斗结束后，敌人的防御值回归
			ChangeMemberAttribInt(v, 18, x);
			ChangeMemberAttribInt(v, 20, y);
			ChangeMemberAttribInt(v, 22, z);
		end
	end

	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
end
