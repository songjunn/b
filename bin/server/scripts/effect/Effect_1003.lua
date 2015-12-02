--战斗中给本体添加buff
Effect_1003 = {}
Effect_1003.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	AddCommbatBuff(attackerIdx, value, true)
	
end
