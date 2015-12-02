--战斗中死亡时有p%概率以x%气血复活
Effect_1038 = {}
Effect_1038.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	MemberRevive(attackerIdx, value);
end

