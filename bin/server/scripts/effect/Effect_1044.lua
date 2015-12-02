--buffer 使降低10%所有防御，持续2回合
Effect_1044 = {}
Effect_1044.OnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	--所有防御:包括物理防御、魔法防御、技能防御
	ChangeMemberAttribInt(attackerIdx, 69, value);
	ChangeMemberAttribInt(attackerIdx, 71, value);
	ChangeMemberAttribInt(attackerIdx, 73, value);
	
end

Effect_1044.UnEffect = function(attackerIdx, skill, attr, value, tarIdx)

	ChangeMemberAttribInt(attackerIdx, 69, -value);
	ChangeMemberAttribInt(attackerIdx, 71, -value);
	ChangeMemberAttribInt(attackerIdx, 73, -value);
	
end
