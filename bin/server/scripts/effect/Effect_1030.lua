--非战斗中给自身添加buff(需存档的buff，一般为使用道具产生的buff)
Effect_1030 = {}
Effect_1030.OnEffect = function(role, attr, value)

	AddExBuff(role, value)
	
end
