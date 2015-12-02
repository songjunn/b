Battle = {}

Battle.Startup = function()

	BattleStartup();

end

Battle.End = function()

	BattleEnd();

end

Battle.StartInspire = function()

	BattleInspire(true);

end

Battle.StopInspire = function()

	BattleInspire(false);

end

Battle.BattleRefresh = function()

	local players = {GetOnlinePlayer()};

	for k, v in pairs(players) do
		BattleRefresh(v);
	end
	
end

Battle.WpNumReward = function()
	WpNumReward();
end

