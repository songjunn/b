--连续型绝技效果：发动后士气变为x(可以持续发动绝技攻击)，造成的伤害为绝技伤害+普通伤害
Effect_1042 = {}
Effect_1042.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	local curMoral = GetMemberAttribInt(attackerIdx, 1);
	ChangeMemberAttribInt(attackerIdx, 1, value - curMoral);
	SetBitFlag(attackerIdx, 138, 1);
	SetBitFlag(attackerIdx, 161, 1);
end