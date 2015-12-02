--战斗中按最大值的百分比改变自身相应的当前值
Effect_1012 = {}
Effect_1012.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local curAttr = 0;
	if(attr == 16) then
		curAttr = 2;
		
	--...

	end

	if(curAttr <= 0) then
		return;
	end

	local maxVal = GetMemberAttribInt(attackerIdx, attr);
	local curVal = GetMemberAttribInt(attackerIdx, curAttr);
	local addVal = maxVal * value / 100;
	if(addVal + curVal < 0) then
		addVal = -curVal;
	elseif(addVal + curVal > maxVal) then
		addVal = maxVal - curVal;
	end

	ChangeMemberAttribInt(attackerIdx, curAttr, addVal);
	
end

Effect_1012.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local curAttr = 0;
	if(attr == 16) then
		curAttr = 2;
		
	--...

	end

	if(curAttr <= 0) then
		return;
	end

	local maxVal = GetMemberAttribInt(attackerIdx, attr);
	local curVal = GetMemberAttribInt(attackerIdx, curAttr);
	local addVal = maxVal * value / (100 + value);

	addVal = -addVal;
	if(addVal + curVal < 0) then
		addVal = -curVal;
	elseif(addVal + curVal > maxVal) then
		addVal = maxVal - curVal;
	end

	ChangeMemberAttribInt(attackerIdx, curAttr, addVal);
	
end
