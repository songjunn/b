--特性技能效果，当场中有2个或2个以上己方拥有该技能的有效目标时，每个目标受到的伤害均分给其他友方目标
Effect_1041 = {}
Effect_1041.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	SetBitFlag(attackerIdx, 159, 1);
end

Effect_1041.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)
	
	SetBitFlag(attackerIdx, 159, 0);
end
