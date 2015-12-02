--恢复己方所有目标气血。
Effect_2204 = {}
Effect_2204.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

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
			local maxHp = GetMemberAttribInt(i, 16);
			local addHp = maxHp * value / 100;
			GainMemberHp(i, addHp);
		end
	end
	
end
