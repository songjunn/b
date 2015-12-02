--注册定时器
RegistTimer(-1,-1,-1,-1,-1,0,-1,"ActionOnHalfHour", "Player")		--每整点
RegistTimer(-1,-1,-1,-1,-1,30,-1,"ActionOnHalfHour", "Player")	--每半小时
RegistTimer(-1,-1,-1,-1,0,-1,-1,"ActionOnOclock", "Player")			--每天零点

RegistTimer(-1,-1,-1,1,10,0,-1,"Startup", "Battle")						--周一10点开始
RegistTimer(-1,-1,-1,0,23,59,-1,"End", "Battle")									--周日24点结束
RegistTimer(-1,-1,-1,-1,0,0,-1,"WpNumReward", "Battle")									--每天零点

RegistTimer(-1,-1,-1,-1,12,-1,-1,"StartInspire", "Battle")			--每天12点
RegistTimer(-1,-1,-1,-1,13,-1,-1,"StopInspire", "Battle")				--每天13点
RegistTimer(-1,-1,-1,-1,21,-1,-1,"StartInspire", "Battle")			--每天21点
RegistTimer(-1,-1,-1,-1,22,-1,-1,"StopInspire", "Battle")				--每天22点
RegistTimer(-1,-1,-1,0,23,59,-1,"BattleRefresh", "Battle")			--周日零点刷新国战

RegistTimer(-1,-1,-1,-1,15,50,-1,"Ready", "Boss")							  --每天15点50分
RegistTimer(-1,-1,-1,-1,16,00,-1,"Startup", "Boss")							--每天16点00分
RegistTimer(-1,-1,-1,-1,16,20,-1,"End", "Boss")									--每天16点20分

RegistTimer(-1,-1,-1,-1,0,-1,-1,"Refresh", "Arena")					-- arena refresh
RegistTimer(-1,-1,-1,-1,0,-1,-1,"Refresh", "Task")					-- task refresh
