#pragma once
//////////////////////////////////////////////////////////////////////////
//
enum Event_ID
{
	Event_Invalid			= 0,			//无效事件

	//主角事件
	Event_Player_Login		= 1,			//角色登陆：无
	Event_Player_Logout,					//角色登出：无
	Event_Player_Levelup,					//角色升级：等级
	Event_Player_Loadover,					//角色信息加载完毕
	Event_Player_LoadModule,				//角色加载完，通知相关模块
	Event_Player_Create,					//角色创建：账号ID
	Event_Player_Stateup,					//角色位阶升级：位阶等级
	Event_Player_MeritObtain,				//角色获得战功：战功值，原值，当前值，位阶等级
	Event_Player_VipLevelUp,				//vip等级提升
	Event_Player_FightingChange,			//主角战斗力变化

	//道具事件
	Event_Item_Gain			= 100,			//获得道具
	Event_Item_Delete,						//销毁道具:主角ID，模板ID，数量
	Event_Item_Obtain,						//道具产出:主角ID，模板ID，数量，道具ID，原因
	Event_Item_Consume,						//道具消耗:主角ID，用户ID，道具ID，模板ID，数量，原因
	Event_Item_Intensify,					//装备强化:主角ID，角色ID，道具ID，模板ID，品质，强化目标等级，成功与否
	Event_Item_Slotting,					//装备打孔:角色ID，用户ID，道具ID，模板ID，品质，孔位
	Event_Item_Inlay,						//装备镶嵌:角色ID，用户ID，道具ID，模板ID，品质，材料模板ID，材料等级
	Event_Item_UpGrade,						//装备升级:角色ID，用户ID，道具ID，模板ID，品质，花费金币，目标等级
	Event_Item_MatIntensify,				//材料强化:主角ID，用户ID，道具ID，模板ID，品质，强化目标等级，成功与否
	Event_Item_RecastCrystal,				//魂晶重铸:主角ID，用户ID，道具ID，模板ID，品质，魂晶等级

	//战斗事件
	Event_Combat_Dead		= 200,			//死亡
	Event_Combat_KillMonster,				//击杀怪物
	
	//副本
	Event_City_Active		= 300,			//副本激活：副本ID
	Event_City_Combat,						//副本战斗：副本ID，战斗次数
	Event_City_Finish,						//副本通关：副本ID，评分，副本模式

	//英雄
	Event_Hero_LeaveTeam	= 400,			//英雄离队：英雄ID，英雄模板
	Event_Hero_Recruit,						//英雄招募：英雄ID，英雄模板，英雄品质
	Event_Hero_Recall,						//英雄召回：英雄ID，英雄模板
	Event_Hero_Inherit,						//英雄继承：英雄ID，被继承英雄ID
	Event_Hero_UpQuality,					//英雄提升品质：英雄ID，英雄品质
	Event_Hero_Levelup,						//英雄升级：英雄ID，等级

	// order 
	Event_Order_Change_Field = 500,
	Event_Order_Join		 = 501,

	//技能
	Event_Stars_Active		= 600,			//星点激活：星点位置，消耗银币，消耗材料数
	Event_Stars_Uplevel,					//星点升级：星点位置，升级后的等级，消耗银币，消耗材料数
	Event_Stars_Inlay,						//星点镶嵌：角色ID，星点位置，镶嵌道具模板，消耗银币
	Event_Stars_Unlay,						//星点拆除：星点位置，镶嵌道具模板
	Event_Skill_Reset,						//重置特性：旧技能，新技能

	//money
	Event_Gold_Trade		= 700,			//获取/消耗金币:用户ID，数量，原因，角色名
	Event_Silver_Trade,						//获取/消耗银币:用户ID，数量，原因，角色名

	//商城
	Event_Store_Buy			= 800,			//商城日志:用户ID，物品模板ID，单价，物品数量，钱种类，消费钱币，角色名

	//阵型
	Event_Format_Uplevel	= 900,			//阵型升级：阵型id，升级后等级

	//世界boss
	Event_Boss_Attack		= 1000,			//世界boss参战：本次伤害，总伤害，战斗结果

	//任务
	Event_Task_Receive		= 1100,			//获得任务：任务ID，角色等级
	Event_Task_Finish,						//完成任务：任务ID，角色等级

	//国战
	Event_Battle_Fight		= 1200,			//主动战斗：攻或防，路点ID，耐久度A，耐久度B

	//训练
	Event_Train_Do			= 1300,			//训练：类型，力量，智力，技巧，敏捷
	Event_Train_Save,						//保存：力量，智力，技巧，敏捷
	Event_Train_UnSave,						//取消：力量，智力，技巧，敏捷

	//祭神
	Event_Worship_Do		= 1400,			//祭神：主角ID，品质(0-绿 1-蓝 2-紫 3-橙)

	//签到
	Event_SignIn_Do			= 1500,			//签到：主角ID，当前日期(1-31)

	// arena
	Event_Arena_Join		= 1600,			// join arena

	//新增每日任务
	Event_Daily_RandHero	= 1700,			//斗酒N次
	Event_Daily_WipeOut,					//扫荡任意副本N次
	Event_Daily_FinishElite,				//通过N个试炼之词
	Event_Daily_Expedition,					//通过N个省界讨伐
	Event_Daily_QueenTease,					//完成N次女王调教
	Event_Daily_CompandCrystal,				//完成N次曜晶合成

	//新增支线任务
	Event_Branch_ZoneBox		= 1800,			//领取区域宝箱
	Event_Branch_QualityItem,					//任意角色获得N件M品质的装备
};

//道具产出和销毁原因
enum ITEM_REASON
{
	Item_Reason_Null = -1,					
	Item_Reason_UnKonw = 0,					//未知原因
	Item_Reason_Sell,						//出售道具
	Item_Reason_OpenBox,					//打开宝箱获得道具
	Item_Reason_DebugComm,					//debug命令获得道具
	Item_Reason_DoTask,						//做任务获得道具
	Item_Reason_CityCombat,					//城战获得道具
	Item_Reason_DropItem,					//打怪掉落道具
	Item_Reason_UnLoadSoul,					//卸载道具上的魂石
	Item_Reason_Exchange,					//签到界面兑换物品
	Item_Reason_StoreBuy,					//商城购买道具
	Item_Reason_Worship,					//祭神获得道具
	Item_Reason_RecastMat,					//重铸时消耗材料
	Item_Reason_Recurit,					//招募英雄时消耗碎片
	Item_Reason_Material,					//道具升级时消耗材料
	Item_Reason_Inlay,						//镶嵌时消耗魂石
	Item_Reason_IntensifyMat,				//强化时消耗材料
	Item_Reason_UpFormat,					//阵型升级消耗材料
	Item_Reason_ActiveStar,					//激活星点消耗材料
	Item_Reason_UpStar,						//升级星点消耗材料
	Item_Reason_FinishTask,					//完成收集任务时消耗道具
	Item_Reason_Use,						//使用道具
	Item_Reason_HeroQuality,				//英雄提品
	Item_Reason_UnloadSoul,					//卸载魂石
	Item_Reason_ActLogin,					//登录活动奖励
	Item_Reason_RechargeActivity1,			//单次充值返赠A
	Item_Reason_RechargeActivity2,			//单次充值返赠B
	Item_Reason_RechargeActivity3,			//累积充值返赠
	Item_Reason_Init,						//角色初始获得的装备
	Item_Reason_StarInlay,					//镶嵌星点消耗材料
	Item_Reason_WipeMonster,				//扫荡获得道具
	Item_Reason_ConsumeEngage,				//扫荡时消耗雇佣令
	Item_Reason_ZoneBoxAward,				//区域宝箱奖励
	Item_Reason_Expedition,					//远征获得道具
	Item_Reason_DayChargeGift,				//每日充值礼包获得道具

};

//金币获取/消耗原因
enum GOLD_REASON
{
	Gold_Reason_Null = -1,					
	Gold_Reason_UnKonw = 0,					//未知原因
	Gold_Reason_DebugComm,					//debug命令获取/消耗
	Gold_Reason_BuyAction,					//购买battle次数
	Gold_Reason_BuyTime,					//购买battle时间
	Gold_Reason_WPNum,					    //国战占有路点数奖励
	Gold_Reason_NDouble,					//翻倍消耗的金币
	Gold_Reason_BossInspire,				//boss inspire消耗
	Gold_Reason_BossCoolDown,				//boss 减少冷却时间消耗
	Gold_Reason_ArenaChgOpp,				//Arena change opp
	Gold_Reason_ArenaChallenge,				//竞技场发起挑战消耗
	Gold_Reason_ArenaAddChalge,				//竞技场增加挑战次数
	Gold_Reason_CityGetPrize,				//领取抽奖消耗
	Gold_Reason_RecuritHero,				//招募英雄消耗
	Gold_Reason_Intensify,					//强化道具消耗
	Gold_Reason_Slotting,					//道具打孔消耗
	Gold_Reason_ExpandBag,					//扩展背包消耗
	Gold_Reason_SignComplement,				//补签消耗
	Gold_Reason_FlushExchage,				//签到界面刷新物品
	Gold_Reason_StoreBuy,					//商城购买
	Gold_Reason_HeroTrain,					//训练英雄
	Gold_Reason_Worship,					//祭神消耗
	Gold_Reason_LightOrange,				//祭神时直接点亮橙色祭品
	Gold_Reason_SkillReset,					//重置特性
	Gold_Reason_CityResetCnt1,				//重置普通副本绝杀模式挑战次数
	Gold_Reason_CityResetCnt2,				//重置精英副本进击模式挑战次数
	Gold_Reason_CityResetCnt3,				//重置精英副本绝杀模式挑战次数
	Gold_Reason_Recharge,					//充值
	Gold_Reason_RechargeActivity1,			//单次充值返赠A
	Gold_Reason_RechargeActivity2,			//单次充值返赠B
	Gold_Reason_RechargeActivity3,			//累积充值返赠
	Gold_Reason_FinishTask,					//finish task
	Gold_Reason_BuyStamina,					//购买体力
	Gold_Reason_LoginActivity,				//连续登录活动奖励
	Gold_Reason_Expedition,					//远征
	Gold_Reason_RandHero,					//斗酒消耗金币
	Gold_Reason_QueenTease,					//女王调教消耗金币
	Gold_Reason_EraseItem,					//消耗道具产生金币(一般没有，有消耗钱袋子产生银币的)
	Gold_Reason_CarnageReset,				//屠戳秘境重置次数消耗金币
	Gold_Reason_GMSend,						//GM后台发放
	Gold_Reason_DayChargeGift,				//每日充值礼包获得金币
	Gold_Reason_ResetGiftBag,				//重置每日充值礼包消耗金币
};

//银币获取/消耗原因
enum SILVER_REASON
{
	Silver_Reason_Null = -1,					
	Silver_Reason_UnKonw = 0,				//未知原因
	Silver_Reason_DebugComm,				//debug命令获取/消耗
	Silver_Reason_StoreBuy,					//商城购买
	Silver_Reason_ArenaRank,				//竞技场排名奖励
	Silver_Reason_BattleWin,				//国战胜利者奖励
	Silver_Reason_BattleSingle,				//国战单次奖励
	Silver_Reason_WPNum,					//国战占有路点数奖励
	Silver_Reason_DoubleSingle,				//BattleModule::_DoubleSingleReward
	Silver_Reason_JoinerReward,				//BossModule::_JoinerReward
	Silver_Reason_BossKill,					//BossModule::_KilldownReward
	Silver_Reason_Obtain,					//RewardSilver::obtain
	Silver_Reason_AttackMonster,			//打怪所得
	Silver_Reason_RecuritHero,				//招募英雄消耗
	Silver_Reason_ItemRecast,				//道具重铸花费
	Silver_Reason_ItemUpGrade,				//道具升级花费
	Silver_Reason_ItemInlay,				//道具镶嵌消耗
	Silver_Reason_ItemIntensify,			//道具强化花费
	Silver_Reason_SellItem,					//出售道具所得
	Silver_Reason_ActiveStar,				//激活星点花费
	Silver_Reason_UpLeveStar,				//升级星点花费
	Silver_Reason_InlayStar,				//镶嵌星点花费
	Silver_Reason_UpFormation,				//升级阵型花费
	Silver_Reason_TrainHero,				//训练英雄花费
	Silver_Reason_BattleDialy,				//国战每日奖励
	Silver_Reason_RechargeActivity1,		//单次充值返赠A
	Silver_Reason_RechargeActivity2,		//单次充值返赠B
	Silver_Reason_RechargeActivity3,		//累积充值返赠
	Silver_Reason_VipLevelup,				//vip赠送
	Silver_Reason_LoginActivity,			//连续登录活动奖励
	Silver_Reason_WipeMonster,				//扫荡获得银币
	Silver_Reason_SkillReset,				//重置特性
	Silver_Reason_Expedition,				//远征获得
	Silver_Reason_QueenTease,				//女王调教获得银币
	Silver_Reason_EraseItem,				//消耗道具产生银币(如钱袋子)
	Silver_Reason_DayChargeGift,			//每日充值礼包获得银币

};

//战功产出途径
enum MERIT_REASON
{
	Merit_Reason_Null = -1,
	Merit_Reason_BattleCombat = 0,			//战场单场战斗奖励
	Merit_Reason_BattleRanking,				//战场排名奖励
	Merit_Reason_BattleWin,					//战场获胜奖励
	Merit_Reason_ArenaRanking,				//竞技场排名奖励
	Merit_Reason_VipLevelup,				//vip赠送
	Merit_Reason_Boss,						//世界boss
	Merit_Reason_BoxAward,					//宝箱奖励获得战功
	Merit_Reason_StoreBuy,					//商城购买消耗战功
	Merit_Reason_EraseItem,					//消耗道具产生的战功(一般没有，有消耗钱袋子产生银币的)
	Merit_Reason_DayChargeGift,				//每日充值获得战功
};
