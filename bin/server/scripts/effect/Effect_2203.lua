--恢复己方血量最低2个目标气血。
Effect_2203 = {}
Effect_2203.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local minidx, maxidx;
	if(attackerIdx >= 0 and attackerIdx < 9) then
		minidx = 0;
		maxidx = 8;
	elseif(attackerIdx >= 9 and attackerIdx < 18) then
		minidx = 9;
		maxidx = 17;
	end

	local minHp = 0;
	for i = 0, 1 do
		local roleIdx = -1;
		for i = minidx, maxidx do
			if(IsFighter(i) and not IsFighterDead(i)) then
				local curHp = GetMemberAttribInt(i, 2);
				if(curHp > minHp) then
					minHp = curHp
					roleIdx = i;
				end
			end
		end

		if(roleIdx >= 0) then
			local maxHp = GetMemberAttribInt(roleIdx, 16);
			local addHp = maxHp * value / 100;
			GainMemberHp(roleIdx, addHp);
		end
	end
	
end
