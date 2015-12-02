--战斗中给目标添加buff
Effect_1004 = {}
Effect_1004.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

		AddCommbatBuff(tarIdx, value, false);

end
