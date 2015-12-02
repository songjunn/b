--攻击对方全体目标,提高自身攻击力,最高12%
Effect_2101 = {}
Effect_2101.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--获取全部攻击目标
	local targets = {GetTargetTeamAllMember(attackerIdx, tarIdx)};

	local hits = 0;
	local attDamage = GetMemberAttribInt(attackerIdx, 21);  --21是Role_Attrib_StuntDamage
	local attDefense = GetMemberAttribInt(attackerIdx, 22); --22是Role_Attrib_StuntDefense
	--循环对每个目标进行伤害计算
	for k, v in pairs(targets) do
		if (v >= 0) then
			local defDefense = GetMemberAttribInt(v, 22);
			local maxAddons = 12;
			if (defDefense > 0) then 
				maxAddons = 100 * (attDefense - defDefense) / defDefense;
				if (maxAddons > 12) then 
					maxAddons = 12;
				elseif (maxAddons < 0) then
					maxAddons = 0;
				end
			end
			
			local addition = attDamage * maxAddons / 100;
			ChangeMemberAttribInt(attackerIdx, 21, addition);
			
			local damage = CombatOnAttack(attackerIdx, skill, v, hits);
			if (damage > 0) then
				hits = hits + 1;
			end
			
			ChangeMemberAttribInt(attackerIdx, 21, -addition);
		end
	end
	
	--增长怒气
	if (hits > 0) then
		AddTargetTeamAnger(attackerIdx);
	end
	
end
