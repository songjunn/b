--战斗中设置自身一些状态标志(是否受到buff/debuff、是否消耗士气、是否无敌状态...)
Effect_1037 = {}
Effect_1037.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	SetBitFlag(attackerIdx, attr, 1);
	
end

Effect_1037.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	SetBitFlag(attackerIdx, attr, 0);
	
end

