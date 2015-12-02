
---------------------活动参数配置---------------------

--注：StartTime和EndTime为"0000-00-00-00:00:00"时StartDay和ContinueDay起作用
--注：StartDay和continueDay起作用时StartDay=0表示开服当天开启
--注：ContinueDay=-1表示无限周期持续下去；ContinueDay=0表示活动停止

--连续登录活动参数
ActivityLogin = {}
ActivityLogin.StartTime		= "2015-04-17-00:00:00"
ActivityLogin.EndTime		= "2015-07-29-23:59:00"
ActivityLogin.StartDay		= 0
ActivityLogin.ContinueDay	= 30
ActivityLogin.Recycle		= 7

--首充双倍
ActivityFirstCharge = {}
ActivityFirstCharge.StartTime	= "2015-04-17-00:00:00"
ActivityFirstCharge.EndTime	= "2015-07-29-23:59:00"
ActivityFirstCharge.StartDay	= 0
ActivityFirstCharge.ContinueDay	= 30
ActivityFirstCharge.Recycle	= 1

--单笔充值
ActivityRecharge1 = {}
ActivityRecharge1.StartTime	= "2015-04-17-00:00:00"
ActivityRecharge1.EndTime	= "2015-07-29-23:59:00"
ActivityRecharge1.StartDay	= 0
ActivityRecharge1.ContinueDay	= 30
ActivityRecharge1.Recycle	= 1

--首次充值多次返还
ActivityRecharge2 = {}
ActivityRecharge2.StartTime	= "2015-04-17-00:00:00"
ActivityRecharge2.EndTime	= "2015-07-29-23:59:00"
ActivityRecharge2.StartDay	= 0
ActivityRecharge2.ContinueDay	= 30
ActivityRecharge2.Recycle	= 1

--累计充值
ActivityRecharge3 = {}
ActivityRecharge3.StartTime	= "2015-04-17-00:00:00"
ActivityRecharge3.EndTime	= "2015-07-29-23:59:00"
ActivityRecharge3.StartDay	= 0
ActivityRecharge3.ContinueDay	= 30
ActivityRecharge3.Recycle	= 1

--每日充值
ActivityDayCharge = {}
ActivityDayCharge.StartTime	= "2015-04-17-00:00:00"
ActivityDayCharge.EndTime	= "2015-07-29-23:59:00"
ActivityDayCharge.StartDay	= 0
ActivityDayCharge.ContinueDay	= 30
ActivityDayCharge.Recycle	= 7

--签到活动
ActivitySignIn = {}
ActivitySignIn.ContDaysMax = 10
ActivitySignIn.ContDays0 = 5
ActivitySignIn.ContDays1 = 10
ActivitySignIn.ContDays2 = 15
ActivitySignIn.ContDays3 = 20
ActivitySignIn.ContDays4 = 25
ActivitySignIn.ContDays5 = 30
ActivitySignIn.ContDays6 = 35
ActivitySignIn.ContDays7 = 40
ActivitySignIn.ContDays8 = 45
ActivitySignIn.ContDays9 = 50
ActivitySignIn.ContDays10 = 55
ActivitySignIn.Knight1_ExtraBadage = 2
ActivitySignIn.Knight2_ExtraBadage = 3
ActivitySignIn.Knight3_ExtraBadage = 4
ActivitySignIn.Knight4_ExtraBadage = 5
ActivitySignIn.Knight5_ExtraBadage = 6
ActivitySignIn.Knight6_ExtraBadage = 7
ActivitySignIn.Knight7_ExtraBadage = 9
ActivitySignIn.Knight8_ExtraBadage = 11
ActivitySignIn.Knight9_ExtraBadage = 13
ActivitySignIn.Knight10_ExtraBadage = 15
ActivitySignIn.Knight11_ExtraBadage = 15
ActivitySignIn.Knight12_ExtraBadage = 15
ActivitySignIn.Knight1_FreeFlush = 1
ActivitySignIn.Knight2_FreeFlush = 1
ActivitySignIn.Knight3_FreeFlush = 2
ActivitySignIn.Knight4_FreeFlush = 2
ActivitySignIn.Knight5_FreeFlush = 3
ActivitySignIn.Knight6_FreeFlush = 3
ActivitySignIn.Knight7_FreeFlush = 4
ActivitySignIn.Knight8_FreeFlush = 4
ActivitySignIn.Knight9_FreeFlush = 4
ActivitySignIn.Knight10_FreeFlush = 5
ActivitySignIn.Knight11_FreeFlush = 5
ActivitySignIn.Knight12_FreeFlush = 5

--斗酒
ActivityRandHero = {}
ActivityRandHero.DrawOneCost = 198
ActivityRandHero.DrawTenCost = 1800
ActivityRandHero.DrawTenCost2 = 1700
ActivityRandHero.DrawInnormalCount = 3
ActivityRandHero.DrawPeriod = 22		--小时
ActivityRandHero.FreeOne_RandValue1 = 70	--白
ActivityRandHero.FreeOne_RandValue2 = 25	--绿
ActivityRandHero.FreeOne_RandValue3 = 5		--蓝
ActivityRandHero.FreeOne_RandValue4 = 0		--紫
ActivityRandHero.FreeOne_RandValue5 = 0		--橙
ActivityRandHero.CostOne_RandValue1 = 70
ActivityRandHero.CostOne_RandValue2 = 25
ActivityRandHero.CostOne_RandValue3 = 5
ActivityRandHero.CostOne_RandValue4 = 0
ActivityRandHero.CostOne_RandValue5 = 0
ActivityRandHero.CostTen_RandValue1 = 70
ActivityRandHero.CostTen_RandValue2 = 25
ActivityRandHero.CostTen_RandValue3 = 5
ActivityRandHero.CostTen_RandValue4 = 0
ActivityRandHero.CostTen_RandValue5 = 0

--女王调教
ActivityQueenTease = {}
ActivityQueenTease.TeasePeriod = 8		--小时

-- -----
SetActivityParams()
