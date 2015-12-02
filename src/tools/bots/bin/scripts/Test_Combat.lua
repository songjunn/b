CombatUnit = {}

RegMessage(104, "CombatUnit", "")
RegMessage(105, "CombatUnit", "")
RegMessage(111, "CombatUnit", "")
RegMessage(106, "CombatUnit", "OnPlayerLogin")
RegMessage(300, "CombatUnit", "OnCombatStart")
RegMessage(301, "CombatUnit", "OnCombatReport")
RegMessage(302, "CombatUnit", "OnCombatResult")


CombatUnit.Logic = function()
end

CombatUnit.Event = function()
end

CombatUnit.OnPlayerLogin = function(socket, pid)
	AttackMonster(socket, pid, 1000, 0)
end

CombatUnit.OnCombatStart = function()

end

CombatUnit.OnCombatReport = function()

end

CombatUnit.OnCombatResult = function()

end
