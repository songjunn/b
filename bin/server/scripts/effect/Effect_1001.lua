--改变战斗中的自身属性
Effect_1001 = {}
Effect_1001.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	ChangeMemberAttribInt(attackerIdx, attr, value)

end

Effect_1001.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	ChangeMemberAttribInt(attackerIdx, attr, -value)

end

