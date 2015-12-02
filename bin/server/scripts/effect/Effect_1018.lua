--战斗中处理目标的某一个buff
Effect_1018 = {}
Effect_1018.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	ProcMemberBuff(tarIdx, value, false);

end
