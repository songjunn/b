--使对方产生混乱，攻击已方目标
Effect_1046 = {}
Effect_1046.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	SetMemberConfusion(tarIdx, true);
	
end

Effect_1046.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	SetMemberConfusion(tarIdx, false);
	
end
