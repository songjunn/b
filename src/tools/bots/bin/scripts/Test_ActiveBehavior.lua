require("config")

ActiveUnit = {}

ActiveUnit[robotaction.nettest] = function (sock)
	DoNetTest(sock)
end
ActiveUnit[robotaction.logintest] = function (sock)
	DoLoginTest(sock)
end

ActiveUnit.DoActiveBehavior = function (sock)
	ActiveUnit[robot.actiontype](sock)
end
