Player = {}

Player.OnCreate = function(id)

	--[[--���ó�ʼ�ȼ�
	SetRoleFieldInt(id, Role_Attrib_Level, 1, false, true);
	--���ó�ʼvip�ȼ�
	SetRoleFieldInt(id, Role_Attrib_Vip, 0, false, true);
	--���������
	GainFullAction(id);
	--���ó�ʼ����
	AddTask(id, 10000001);
	AddTask(id, 20000099);
	--�����ʼ����
	ActiveCity(id, 1000, false, true);
	ActiveCity(id, 1100, false, true);
	--��ó�ʼ����
	GetNewFormat(id);
	--ѡ��ʹ�õ�����
	SetFormation(id, 10000, false, true);
	--���ó�ʼ���
	--SetRoleFieldInt(id, Role_Attrib_GoldCoin, 100000, false, true);
	--SetRoleFieldInt(id, Role_Attrib_SilverCoin, 100000, false, true);
	
	--arena
	SetArenaFieldInt(id, EARENA_DATA_ARENASCORE, 1000, false, true);
	SetArenaFieldInt(id, EARENA_DATA_CHALLENGENUM, 0, false, true);
	SetArenaFieldInt(id, EARENA_DATA_BUYCHALLENGENUM, 0, false, true);
	SetArenaFieldInt(id, EARENA_DATA_CHANGENUM, 3, false, true);
	SetArenaFieldInt(id, EARENA_DATA_CURSTAGE, 0, false, true);
	SetArenaFieldInt(id, EARENA_DATA_KNIGHTID, 0, false, true);
	
	--��ó�ʼװ��
	--Player.GetInitEquip(id, id);]]
	
end

Player.OnHeroCreate = function(playerid, heroid)
	--��ó�ʼװ��
	--Player.GetInitEquip(heroid, playerid);
end

Player.OnLogin = function(id)
	
	--[[local nowTimeSec = GetTimeSec();
	local logoutTimeSec = GetRoleFieldI64(id, Role_Attrib_LogoutTime);
	--每6分钟增加一点体力
	local ap = (nowTimeSec - logoutTimeSec) / 60 / 6;
	GainActionPoint(id, ap);

	local secNow, minNow, hourNow, mdayNow, monNow, yearNow, wdayNow = GetNowTime();
	local secOut, minOut, hourOut, mdayOut, monOut, yearOut, wdayOut = GetDataTime(logoutTimeSec);

	--ÿ�������ָ������� wenc 
	if (mdayNow ~= mdayOut or monNow ~= monOut or yearNow ~= yearOut) then
		ResetWorshipNum(id);
		ResetSignInAttr(id);
		ResetArenaData(id);
		ResetDailyTask(id);
		BattleRefresh(id);
		ResetCityCounts(id);
		DayFirstLogin(id);

		SetRoleFieldInt(id, Role_Attrib_BuyStamina, 0, true, true);
		SetRoleFieldInt(id, Role_Attrib_AdvancedTrain, 0, true, true);
		SetRoleFieldInt(id, Role_Attrib_BossInspireFree, 0, true, true);
		SetRoleFieldInt(id, Role_Attrib_SignInFree, 0, true, true);
		SetRoleFieldInt(id, Role_Attrib_SkillResetFree, 0, true, true);
	end]]

end

Player.OnLogout = function(id)
end

Player.OnReward = function(id)

end

Player.ActionOnHalfHour = function()

	local players = {GetOnlinePlayer()};
	
	--�ظ�5���ж���
	for k, v in pairs(players) do
		GainActionPoint(v, 5);
	end

end

Player.ActionOnOclock = function()

	local players = {GetOnlinePlayer()};
	
	for k, v in pairs(players) do
		--�ظ��ж��������ֵ
		--GainFullAction(v);

		--ÿ�������ָ������� wenc 
		ResetWorshipNum(v);
		ResetSignInAttr(v);
		ResetArenaData(v);
		ResetDailyTask(v);
		BattleRefresh(v);
		ResetCityCounts(v);
		DayFirstLogin(v);
		
		SetRoleFieldInt(v, Role_Attrib_BuyStamina, 0, true, true);
		SetRoleFieldInt(v, Role_Attrib_AdvancedTrain, 0, true, true);
		SetRoleFieldInt(v, Role_Attrib_BossInspireFree, 0, true, true);
		SetRoleFieldInt(v, Role_Attrib_SignInFree, 0, true, true);
		SetRoleFieldInt(v, Role_Attrib_SkillResetFree, 0, true, true);
	end

end

Player.GetInitEquip = function(id, parent)

	local vocation = GetRoleFieldInt(id, Role_Attrib_Vocation);
	if (vocation == Vocation_Berserker) then
		EquipItem(parent, id, 10101001);
		EquipItem(parent, id, 10102001);
		EquipItem(parent, id, 10103001);
		EquipItem(parent, id, 10104001);
		EquipItem(parent, id, 10105001);
		EquipItem(parent, id, 10001001);
		EquipItem(parent, id, 10002001);
		EquipItem(parent, id, 10003001);
	elseif (vocation == Vocation_Mage) then
		EquipItem(parent, id, 10201001);
		EquipItem(parent, id, 10202001);
		EquipItem(parent, id, 10203001);
		EquipItem(parent, id, 10204001);
		EquipItem(parent, id, 10205001);
		EquipItem(parent, id, 10001001);
		EquipItem(parent, id, 10002001);
		EquipItem(parent, id, 10003001);
	elseif (vocation == Vocation_Robbers) then
		EquipItem(parent, id, 10301001);
		EquipItem(parent, id, 10302001);
		EquipItem(parent, id, 10303001);
		EquipItem(parent, id, 10304001);
		EquipItem(parent, id, 10305001);
		EquipItem(parent, id, 10001001);
		EquipItem(parent, id, 10002001);
		EquipItem(parent, id, 10003001);
	end
	
end
