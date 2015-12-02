LoginUnit = {}

--RegMessage(106, "LoginUnit", "OnPlayerLogin")
RegMessage(303, "LoginUnit", "OnUserLogin")
RegMessage(404, "LoginUnit", "OnPlayerCount")
RegMessage(406, "LoginUnit", "OnLoadover")

LoginUnit.Logic = function()
end

LoginUnit.Event = function()
end

LoginUnit.OnUserLogin = function(socket, uid, key, server, ip, port)
	ConnectGate(socket, uid, key, server, ip, port)
end

LoginUnit.OnPlayerCount = function(socket, count)
	if count <= 0 then
		CreatePlayer(socket, 1, "robot"..socket, 1, 1, 1)
	end
end

LoginUnit.OnPlayerLogin = function(socket, pid)
	--UserLogout(socket)
end

LoginUnit.OnLoadover = function(socket, pid)
	
end
