--绝技技能 当场上有两个及以上的己方角色拥有该技能时，其中一个发动技能攻击，其他有该技能的角色有x%概率(不消耗士气)以技能同时攻击相同目标(目标死亡时不触发)
Effect_2014 = {}
Effect_2014.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	local damage = CombatOnAttack(attackerIdx, skill, tarIdx, 0);
	if(damage > 0) then
		AddTargetTeamAnger(attackerIdx);
	end

	local teamRoles = Effect_2014.GetSameSkillRoles(attackerIdx);
	for k, v in pairs(teamRoles) do
		if(IsFighterDead(tarIdx)) then
			break;
		end		
			
		if (v >= 0) then
			local rand = RandomInt(1, 100);
			if(rand <= value) then
				AddAction();
				local damage = CombatOnAttack(v, skill, tarIdx, 0);
				if(damage > 0) then
					AddTargetTeamAnger(v);
				end
				SetCheckAttr(v, 1);
			end
		end
	end
end

Effect_2014.GetSameSkillRoles = function(attackerIdx)

	local skillRoles = {};
	local minidx, maxidx;
	if(attackerIdx >= 0 and attackerIdx < 9) then
		minidx = 0;
		maxidx = 8;
	elseif(attackerIdx >= 9 and attackerIdx < 18) then
		minidx = 9;
		maxidx = 17;
	end

	local mySkill = GetMemberAttribInt(attackerIdx, 88);
	for i = minidx, maxidx do
		local hisSkill = GetMemberAttribInt(i, 88);
		if(IsFighter(i) and not IsFighterDead(i) and i ~= attackerIdx and hisSkill == mySkill) then
			table.insert(skillRoles, i);
		end
	end

	return skillRoles;
end
