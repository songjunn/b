--战斗中处理本体的某一个buff
Effect_1017 = {}
Effect_1017.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	ProcMemberBuff(attackerIdx, value, true)
	
end
