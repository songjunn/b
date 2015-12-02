--事件技能效果 战亡时按百分比增加队友的气血/士气
Effect_1024 = {}
Effect_1024.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local membermates = Effect_1024.GetAllTeamMembers(attackerIdx);
	for k, v in pairs(membermates) do
		local ival = GetMemberAttribInt(v, attr);
		local ivaladd = value * ival / 100;
		
		if(attr == 2) then
			GainMemberHp(v, ivaladd);
			SetCheckAttr(v, 2);
		elseif(attr == 1) then
			ChangeMemberAttribInt(v, attr, ivaladd);
		end
	end
end

Effect_1024.GetAllTeamMembers = function(attackerIdx)

	local allMembers = {};

	local minidx, maxidx;
	if(attackerIdx >= 0 and attackerIdx < 9) then
		minidx = 0;
		maxidx = 8;
	elseif(attackerIdx >= 9 and attackerIdx < 18) then
		minidx = 9;
		maxidx = 17;
	end

	for i = minidx, maxidx do
		if(IsFighter(i) and i ~= attackerIdx and not IsFighterDead(i)) then
			table.insert(allMembers, i);			
		end
	end

	return allMembers;
end


