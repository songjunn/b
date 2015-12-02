Server = {}

Server.Startup = function()

	--取当前时间
	local secNow, minNow, hourNow, mdayNow, monNow, yearNow, wdayNow = GetNowTime();
	
	--检查国战时间
	if( wdayNow ~= 1 or hourNow >= 10 ) then
		BattleStartup();
	end

end