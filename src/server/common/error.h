#pragma once

enum Error_Define
{
	Error_Null = 0,

	Error_Login_ConnectTimeout =1,	//连接超时
	Error_Login_CheckFailed,		//账号验证失败
	Error_Create_NameInvalid,		//名字非法
	Error_Create_NameRepeat,		//重名
	Error_User_Displace,			//顶号

	// 道具升级
	Error_ItemUpGra_Unknow = 100,	//未知错误
	Error_ItemUpGra_NoItem,			//没有该道具
	Error_ItemUpGra_OverRoleLevel,	//大于角色等级
	Error_ItemUpGra_LackMoney,		//钱币不足
	Error_ItemUpGra_LackMaterial,	//材料不足
	Error_ItemUpGra_LackGold,		//元宝不足
	Error_ItemUpGra_UnloadSoul,		//_unloadSouls fail
	Error_ItemUpGra_FailCreate,		//创建新道具失败
	Error_ItemUpGra_Success,		//升级成功
	Error_ItemUpGra_OverLevel,		//升级后装备等级不能大于英雄等级
	Error_ItemUpGra_PackageFull,	//背包已满

	// 道具强化
	Error_ItemUp_Unknow = 200,		//未知错误
	Error_ItemUp_NoItem,            //没找到该道具
	Error_ItemUp_OverMinLevel,      //大于最低等级限制
	Error_ItemUp_OverRoleLevel,		//大于角色等级
	Error_ItemUp_LackMoney,			//缺少钱币
	Error_ItemUp_LackPoint,			//缺少点数
	Error_ItemUp_LackMaterial,		//材料不足
	Error_ItemUp_OutPresent,		//概率失败
	Error_ItemUp_Success,			//强化成功
    Error_ItemUpOnce_Success,       //一键强化成功

	// 魂晶强化
	Error_CrystalUp_Unknow = 300,	//未知错误
	Error_CrystalUp_NoCrystal,      //没找到该魂晶
	Error_CrystalUp_NotCrystal,		//该道具不是魂晶
	Error_CrystalUp_LackMaterial,	//材料不足
	Error_CrystalUp_MaxLevel,       //魂晶已满级
	Error_CrystalUp_LackMoney,		//缺少钱币
	Error_CrystalUp_DifMinLevel,    //材料和魂晶最小限制等级不同
	Error_CrystalUp_FailCreate,		//创建强化后的魂晶失败
	Error_CrystalUp_Success,		//强化成功

	// 魂玉强化
	Error_JadeUp_Unknow = 400,		//未知错误
	Error_JadeUp_NoJade,			//没找到该魂玉
	Error_JadeUp_NotJade,			//该道具不是魂玉
	Error_JadeUp_LackMaterial,		//材料不足
	Error_JadeUp_MaxLevel,			//魂玉已满级
	Error_JadeUp_LackMoney,			//缺少钱币
	Error_JadeUp_FailCreate,		//创建强化后的魂玉失败
	Error_JadeUp_MatNotJade,		//材料不是魂玉
	Error_JadeUp_LackPoint,			//点数不足
	Error_JadeUp_OutPresent,		//概率失败
	Error_JadeUp_Success,			//强化成功

	// 魂晶重铸
	Error_CrystalRecast_Unknow = 500,//未知错误
	Error_CrystalRecast_FadeCrystal,//有伪魂晶
	Error_CrystalRecast_NotCrystal, //有不是魂晶
	Error_CrystalRecast_DifLevel,	//两魂晶等级不同
	Error_CrystalRecast_LackMoney,	//钱币不足
	Error_CrystalRecast_NoCrystal,	//背包中没有这两个魂晶
	Error_CrystalRecast_FailCreate,	//创建新魂晶失败
	Error_CrystalRecast_Success,	//重铸成功

	// 打孔
	Error_Slotting_Unknow = 600,	//未知错误
	Error_Slotting_NotThird,		//不是第三个孔
	Error_Slotting_NoItem,			//没找到要打孔的道具
	Error_Slotting_NotEquipment,	//不是装备
	Error_Slotting_Slotted,			//已经打过孔
	Error_Slotting_LackGold,		//元宝不足
	Error_Slotting_Success,			//打孔成功

	// 镶嵌魂晶
	Error_InlayCrystal_Unknow = 700,//未知错误
	Error_InlayCrystal_NoItem,		//没找到该道具
	Error_InlayCrystal_NotEquipment,//不是装备
	Error_InlayCrystal_NotCrystal,	//材料不是魂晶
	Error_InlayCrystal_LackMoeny,	//钱币不足
	Error_InlayCrystal_Unload,		//卸载已有的魂晶失败
	Error_InlayCrystal_FailInlay,	//镶嵌失败
	Error_InlayCrystal_Success,		//镶嵌成功

	// 镶嵌魂玉
	Error_InlayJade_Unknow = 800,	//未知错误
	Error_InlayJade_NoItem,			//没有找到该道具
	Error_InlayJade_NotEquipment,	//不是装备
	Error_InlayJade_NotJade,		//材料不是魂玉
	Error_InlayJade_NoSlotting,		//此孔未开
	Error_InlayJade_Unload,			//卸载已有的魂玉失败
	Error_InlayJade_LockMoney,		//钱币不足
	Error_InlayJade_FailInlay,		//镶嵌失败
	Error_InlayJade_Success,		//镶嵌成功
	Error_InlayItemAll_FailInlay,	//一键镶嵌失败
	Error_InlayItemAll_Success,		//一键镶嵌成功

	// 穿装/卸装
	Error_Equipment_Success  = 900,	//穿装成功
	Error_UnEquipment_Success,		//卸装成功
	Error_Equipment_UnKnow,			//未知错误
	Error_Equipment_NoItem,			//没有此装备
	Error_Equipment_HaveEquiped,	//该装备已穿戴
	Error_Equipment_MisMatchCareer, //职业不匹配
	Error_Equipment_NoLocation,		//没有穿戴位置
	Error_Equipment_UnEquip,		//卸掉该位置已有装备失败
	Error_Equipment_RemoveFromBag,	//从包裹中移除失败
	Error_Equipment_PutToBag,		//该位置已有装备归包失败
	Error_UnEquipment_WrongLocation,//装备位置错误
	Error_UnEquipment_PutToBag,		//卸下装备归包失败
	Error_UnEquipment_NoEquip,		//该位置无装备
	Error_UnEquipment_FullBag,		//背包已满，请先清理背包
	Error_Equipment_OverLevel,		//装备等级大于角色等级

	// 祭神
	Error_Worship_Closed = 1000,	//祭神系统已关闭
	Error_Worship_Inition,			//发送初始化界面所需的数据(不做错误文本定义)
	Error_Worship_Unknow,			//未知错误
	Error_Worship_BagFull,			//背包已满，请先清理背包
	Error_Worship_LowLevel,			//等级不足
	Error_Worship_Unfinished,		//没完成相关的新手教学内容
	Error_Worship_NotLighted,		//该祭品没有点亮
	Error_Worship_CountsUp,			//免费次数已用尽
	Error_Worship_LackGold,			//金币不足
	Error_Worship_CreateFall,		//创建新道具失败
	Error_Worship_ItemNotGet,		//道具未领取，无法继续进行祭神
	Error_Worship_FailPutBag,		//领取失败
	Error_Worship_HavePutBag,		//道具已领取，无法重复领取
	Error_Worship_HaveLighted,		//道具已点亮，无法重复点亮
	Error_Worship_LightOrange,		//点亮橙色祭品成功
	Error_Worship_Success,			//祭神成功
	Error_Worship_PutSuccess,		//领取成功

	// 签到
	Error_SignIn_Closed = 1100,		//签到系统已关闭
	Error_SignIn_OpenPanel,			//每天登录没签到的请求打开签到面板(不做错误文本定义)
	Error_SignIn_Inition,			//发送初始化数据(不做错误文本定义)
	Error_SignIn_Unknow,			//未知错误
	Error_SignIn_HaveSigned,		//该日期已签到
	Error_SignIn_LowLevel,			//等级不足
	Error_SignIn_LackGold,		    //金币不足
	Error_SignIn_Unfinished,		//未完成相关的新手教学内容
	Error_SignIn_NoSignDay,			//没有可补签的日期
	Error_SignIn_CanSignIn,			//该日期可签到，无需补签
	Error_SignIn_LackBadage,		//徽章不足，不能兑换该物品
	Error_SignIn_BagFull,			//背包已满，请先清理背包
	Error_SignIn_ExchangeFall,		//兑换失败
	Error_SignIn_HaveExchged,		//该物品今日已兑换过
	Error_SignIn_LowKniState,		//骑士位阶偏低，不能兑换该物品
	Error_SignIn_Success,			//签到成功
	Error_SignIn_SharedSuccess,		//分享成功
	Error_SignIn_CompSuccess,		//补签成功
	Error_SignIn_ExchgSuccess,		//兑换成功
	Error_SignIn_FlushSuccess,		//刷新成功

	// 扩充背包
	Error_ExpandBag_UnKnow = 1200,	//未知错误
	Error_ExpandBag_LackGold,		//金币不足
	Error_ExpandBag_ToMax,			//背包已扩充最大
	Error_ExpandBag_Success,		//扩充成功

	// 商城
	Error_Store_Unknow = 1300,		//未知错误
	Error_Store_LockMoney,			//钱币不足
	Error_Store_Success,			//购买成功
	Error_Store_LackGold,			//金币不足
	Error_Store_LackZhanGong,		//战功不足
	Error_Store_UnenoughBag,		//背包空间不足
	Error_Store_LackShengWang,		//声望不足

	// 国战
	Error_Battle_Join = 1400,		//报名成功
	Error_Battle_DefendResult,		//防守战果
	Error_Battle_DoubleReward,		//奖励翻倍失败
	Error_Battle_NotStart,			//国战未开启
	Error_Battle_WpNoDefend,		//该点不能防御
	Error_Battle_MoveFailed,		//移动失败

	// Arena
	Error_Arena_NoChange = 1500,	// change player is none
	Error_Arena_BuyChallengeMax,	// buy challenge num is max
	Error_Arena_BuyChallengeSuccess,// add challenge success
	Error_Arena_ChallengeWin,		// challenge win
	Error_Arena_ChallengeLose,		// challenge lose 
	Error_Arena_NoEnoughPlayer,		// no enough player ,can't join arena
	Error_Arena_CurChallengeMax,	// current challenge is max times

	// ItemUnit Error
	Error_ItemUnit_UnKnow = 1600,	// 未知错误
	Error_ItemUnit_HaveNo,			// 没有该道具
	Error_ItemUnit_SellFail,		// 出售失败
	Error_ItemUnit_SellSuccess,		// 出售成功


	// chat
	Error_Chat_Is_Null			= 1700,
	Error_Chat_Limit_Level_Camp,
	Error_Chat_Limit_Time_Camp,
	Error_Chat_No_Knight,
	Error_Chat_Private_No_Target,
	Error_Chat_Limit_Level_World,
	Error_Chat_Limit_Level_Private,
	Error_Chat_Limit_Time_World,
	Error_Chat_Limit_Time_Private,
    Error_Chat_Limit_Not_In_Camp,


	// friend
	Error_Friend_Max			= 1800,
	Error_Friend_ADD_SUCCESS,
	Error_Friend_DEL_SUCCESS,

	// 打开宝箱
	Error_Box_UnKnow = 1900,		// 未知错误
	Error_Box_UnEnoughBag,			// 背包空间不足，请先清理背包
	Error_Box_UseSuccess,			// 使用成功
	Error_Box_UseFail,				// 使用失败
	Error_Box_OverMerit,			// 战功超出上限

	// 世界boss
	Error_Boss_AttackCD = 2000,		// 战斗cd

	// 拆卸魂石
	Error_UnloadSoul_Unknow	= 2100,	//未知错误
	Error_UnloadSoul_FullBag,		//背包已满，请先清理背包
	Error_UnloadSoul_NoSoul,		//孔位上没有魂石
	Error_UnloadSoul_Fail,			//拆卸失败
	Error_UnloadSoul_Success,		//拆卸成功

	// 副本
	Error_City_LackGold = 2200,		//金币不足
	Error_City_ResetOverUp,			//重置次数已用完
	Error_City_ChalgLeft,			//挑战次数未用尽，无需重置
	Error_City_WipeElite,			//精英副本不能扫荡
	Error_City_LowLevel,			//主角等级不足，无法扫荡
	Error_City_LackEngage,			//雇佣令不足
	Error_City_OverEngage,			//超过最大扫荡数
	Error_City_BagUnenough,			//背包空间不足，请先清理背包
	Error_City_BagFulling,			//扫荡过程背包已满，扫荡中断(剩余雇佣令已返还)
	Error_City_WipeFinish,			//扫荡完成
	Error_City_ZoneUnOpen,			//该区域未开启
	Error_City_HaveNoBox,			//该宝箱不存在
	Error_City_LackStar,			//星级数量不足
	Error_City_HaveGot,				//该宝箱已领取过
	Error_City_GetSuccess,			//宝箱领取成功

	// 秘银强化
	Error_MithrilUp_Unknow = 2300,	//未知错误
	Error_MithrilUp_NotMithril,		//该道具不是秘银
	Error_MithrilUp_LackMaterial,	//材料不足
	Error_MithrilUp_LackPoint,		//点数不足
	Error_MithrilUp_MaxLevel,		//秘银已满级
	Error_MithrilUp_LackMoney,		//钱币不足
	Error_MithrilUp_MatNotMithril,	//材料类型错误
	Error_MithrilUp_OutPresent,		//强化失败
	Error_MithrilUp_Success,		//强化成功

	// 连续登录活动
	Error_LoginAct_Unknow = 2400,	//未知错误
	Error_LoginAct_Closed,			//该活动已关闭
	Error_LoginAct_HaveGet,			//今天已领取奖励
	Error_LoginAct_FullBag,			//背包已满，无法领取奖励
	Error_LoginAct_Success,			//领取成功

	// 添加的强化模块的开关
	Error_IntenItem_Closed = 2500,	//道具强化系统已关闭
	Error_IntenJade_Closed,			//魂玉强化系统已关闭
	Error_IntenCrystal_Closed,		//魂晶强化系统已关闭
	Error_IntenMithril_Closed,		//秘银强化系统已关闭
	Error_InlayJade_Closed,			//镶嵌魂玉系统已关闭
	Error_InlayCrystal_Closed,		//镶嵌魂晶系统已关闭
	Error_UpgradeItem_Closed,		//道具升级系统已关闭
	Error_SlottingItem_Closed,		//道具打孔系统已关闭
	Error_RecastCrystal_Closed,		//魂晶重铸系统已关闭
	Error_UnloadSoul_Closed,		//卸载魂石系统已关闭
	Error_UnloadAllSoul_Success,	//卸载全部曜石成功
	Error_UnloadAllSoul_Failed,		//卸载全部曜石失败

	// 充值相关活动
	Error_ChargeAct_Unknow = 2600,  //未知错误
	Error_ChargeAct_Closed,			//该活动已关闭
	Error_ChargeAct_GetSuccess,		//领取成功
	Error_ChargeAct_UnenoughBag,	//背包空间不足，请先清理背包
	Error_ChargeAct_UnOpened,		//该活动没开启
	Error_ChargeAct_HaveGot,		//今日已领取
	Error_ChargeAct_LowVip,			//Vip等级不足
	Error_ChargeAct_Unreached,		//未达到充值额度，不能领取
    
    //
    Tip_Zhuangbei_shuxing = 2709,
    Tip_Zhuangbei_xiangqian = 2701,
    Tip_Zhuangbei_shengji =2702,
    Tip_Zhuangbei_qianghua =2708,

    Tip_Zhuangbei_xiangqiankong =2703,
    Tip_hecheng_yaoshiqianghua =2710,
    Tip_hecheng_miyinqianghua =2704,
    Tip_hecheng_yaojingqianghua =2705,
    Tip_hecheng_yaojingchongzhu =2706,
    
    Tip_role_xunlian =2711,
    Tip_role_tipin =2712,
    Tip_role_jueji =2713,
    Tip_role_texing =2714,
    Tip_Battle_juesha = 2715,
    
    //提品
	Error_UpQuality_Unknow = 2800,	//未知错误
	Error_UpQuality_TopQuality,		//已达到最高品级
	Error_UpQuality_LowProgress,	//小提品进度没达到

	//远征活动
	Error_Expedition_Unknow = 2900,	//未知错误
	Error_Expedition_Closed,		//远征活动已关闭
	Error_Expedition_NonAuto,		//当前是第一个关卡，不能自动战斗
	Error_Expedition_Autoed,		//今天已自动挑战过，明天再试
	Error_Expedition_BagUnenough,	//背包空间不足，请先清理背包
	Error_Expedition_FailMax,		//已尝试挑战到最大次数
	Error_Expedition_LowLevel,		//主角等级未达到，无法挑战

	//斗酒
	Error_RandHero_Unknow = 3000,	//未知错误
	Error_RandHero_Closed,			//斗酒系统已关闭
	Error_RandHero_UnreachTime,		//冷却时间未到，不能斗酒
	Error_RandHero_LackGold,		//金币不足，不能斗酒

	//角色
	Error_Role_ProgressSuccess = 3100,//小提品成功
	Error_Role_QualitySuccess,		  //大提品成功
	Error_Role_TrainSuccess,		  //训练成功
	Error_Role_ResetSuccess,		  //重置特性成功
	Error_Role_InheritSuccess,		  //继承成功
	Error_Role_LeaveSuccess,		  //离队成功
	Error_Role_LevelUpSuccess,		  //升级成功

	//继承
	Error_Inherit_Unknow = 3200,	  //未知错误
	Error_Inherit_LowLevel,			  //被继承者等级不足
	Error_Inherit_OnlyOnce,			  //每个英雄只能继承或被继承一次
	Error_Inherit_DiffCarrer,		  //两个英雄职业不同
	Error_Inherit_Success,			  //继承成功

	//女王调教
	Error_QueenTease_Unknow = 3300,	  //未知错误
	Error_QueenTease_Closed,		  //该活动已关闭
	Error_QueenTease_TimeLeft,		  //未到冷却时间
	Error_QueenTease_MaxCount,		  //调教次数已用尽
	Error_QueenTease_LackGold,		  //金币不足，无法调教

	//屠戳秘境
	Error_Carnage_Unknow = 3400,	  //未知错误
	Error_Carnage_Closed,			  //该活动已关闭
	Error_Carnage_ChlgCntMax,		  //挑战次数已用尽
	Error_Carnage_LackStamina,		  //体力不足
	Error_Carnage_SceneNotOpen,		  //当前秘境未开启
	Error_Carnage_ResetCntMax,		  //重置次数已用尽
	Error_Carnage_LockGold,			  //金币不足，无法重置
	Error_Carnage_ChlgSuccess,		  //挑战成功
	Error_Carnage_ResetSuccess,		  //重置成功
	Error_Carnage_LeftChlgCnt,		  //挑战次数未用完，可继续挑战

	//每日充值
	Error_DayCharge_Unknow = 3500,	  //未知错误
	Error_DayCharge_Closed,			  //该活动已关闭
	Error_DayCharge_LackGold,		  //金币不足，无法重置
	Error_DayCharge_HaveGot,		  //该礼包已领取
	Error_DayCharge_Unreached,		  //未达到充值额度
	Error_DayCharge_FullBag,		  //背包空间不足，无法领取
	Error_DayCharge_GetSuccess,		  //领取礼包成功
	Error_DayCharge_ResetSuccess,	  //重置礼包成功
	Error_DayCHarge_ChargeSuccess,	  //充值成功
	Error_DayCHarge_ChargeFailed,	  //充值失败
};

enum Boardcast_Define
{
    Boardcast_Null = 0,
    
    // 国战
    Boardcast_Battle_Startup  = 1000,	//国战开始
    Boardcast_Battle_WpChangeHand,		//占领路点
    Boardcast_Battle_Draw,				//国战平局
    Boardcast_Battle_Settle,			//国战结算
	Boardcast_Battle_WpCapture,		//围困路点
    
    // 世界boss
    BoardCast_Boss_Startup = 1100,		//世界boss活动开始
    BoardCast_Boss_End,					//世界boss活动结束（未击杀）
	BoardCast_Boss_Settle,				//世界boss活动结算（已击杀）
	BoardCast_Boss_Killer,				//世界boss击杀者
	BoardCast_Boss_Ready,				//世界boss活动准备

};
