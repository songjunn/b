#ifndef ODIN_COMMDEF_H
#define ODIN_COMMDEF_H


#define DAY_SEC (24*60*60)

#define	EQUIP_EFFECT_MAX		5	//装备效果数量
#define CHARGE					8  //手续费
#define ITEM_ARRTU					3   //魂晶属性条数
#define	ITEM_KONG					4	//装备上镶嵌的魂玉和魂晶个数
#define ARR_LENS  				6
#define LEVELS					5  //等级,  强化魂玉 时  等级判断

#define ZERO_ONE 	10
#define ONE_TWO  	20
#define TWO_THREE  	30
#define THREE_FOUR  40
#define FOUR_FIVE  	50
#define FIVE_SIX  	60
#define SIX_SEVEN  	70
#define SEVEN_EIGHT 80

#define ARR_LEN		(6+1)
#define MATER_LENS	(ARR_LEN-1)
#define MATER_LEN	10

#define SUN			1
#define MOON	  	2
#define SUN_MOON	3
#define STAR		4
#define SUN_STAR  	5
#define MOON_STAR  	6
#define SUN_MOON_STAR	7

#define	 EXPRESS	0
#define  SHOP       1
#define  MALL       2
#define  CAIBEI     3
#define  ZHANGONG   4
#define	 SHENGWANG	5

//背包
#define ITEMUNIT_INIT_PAGE	3		//初始页数
#define ITEMUNIT_BUG_PAGE	15		//可购买页数
#define ITEMUNIT_PER_PAGE	6		//每页格子数
#define ITEMUNIT_BAG_INIT	(ITEMUNIT_PER_PAGE * ITEMUNIT_INIT_PAGE)
#define ITEMUNIT_EXPAND_BASECOST	20	//第一次扩展背包消耗金币

enum Store_Group
{
	Group_Unknow = 0,
	Group_Equip ,			//装备组
	Group_Material,			//材料组
	Group_GiftBag,			//礼包组
	Group_Jade,				//曜石组
	Group_Crystal,			//曜晶组
	Group_Mithril,		    //秘银组
	Group_HotSell,			//热卖组
	Group_Stone,			//曜晶石
	Group_Berserker = 9,	//战士组
	Group_Mage,				//法师组
	Group_Robber,			//盗贼组
	Group_Reserved,			//其他组
	Group_All,				//全部道具
};


#define STAR_NUM	7	//星盘中星点的数量

#define ITEM_TEMPID_MIN		10000000	//最小道具模板ID


//国战
#define BATTLE_WP_LINE		7
#define BATTLE_WP_MAX	BATTLE_WP_LINE*BATTLE_WP_LINE

enum JadeType
{
	JADESUN = 1,	//日
	JADEMOON,		//月
	JADESTAR,		//星
};

enum MaterialType
{
	Material_Jade = 1,	//曜石
	Material_Crystal,	//曜晶
	Material_Mithril,	//秘银
};

enum CoinType
{
	SILVERCOIN = 1,	//银币
	GOLDCOIN,		//金币
	CAICOIN,		//彩币
	ZHANGONGCOIN,   //战功
	SHENGWANGCOIN,	//声望
};

enum Role_Type
{
	Role_Type_Player = 1,			//玩家
	Role_Type_Hero,					//英雄
	Role_Type_Monster,				//怪物
};

enum Role_Vocation
{
	Role_Vocation_Berserker = 1,	//战士
	Role_Vocation_Mage,				//法师
	Role_Vocation_Robbers,			//盗贼
	Role_Vocation_Tongyong,			//通用
};

enum Role_Race
{
	Role_Race_People = 0,			//人类
	Role_Race_Animal,				//动物
	Role_Race_Fairy,				//精灵
	Role_Race_Machine,				//机甲
	Role_Race_Devil,				//魔怪
};

enum Role_Sex
{
	Role_Sex_None = 0,
	Role_Sex_Male,					//男
	Role_Sex_Female,				//女
};

enum Role_Body
{
	Role_Body_Common = 0,			//普通体型
	Role_Body_Large,				//大体型
	Role_Body_Huge,					//巨大体型
};

enum Damage_Position
{
	Damage_Position_Head = 1,		//头（角） 
	Damage_Position_Chest,			//身体 （尾部）
	Damage_Position_Arm,			//臂 （翅膀）
	Damage_Position_Leg,			//腿
};

enum Combat_Score					//战斗评分
{
	Combat_Score_Null = 0,			//无效
	Combat_Score_D,					//D
	Combat_Score_C,					//C
	Combat_Score_B,					//B
	Combat_Score_A,					//A
	Combat_Score_S,					//S
	Combat_Score_Max,					
};

enum Intensify_Points					//强化等级区间, 用来做需求点数
{
	Intensify_Points_A= 0,   //0_1需求点数				
	Intensify_Points_B,		 //1_2需求点数			
	Intensify_Points_C,		//2_3需求点数			
	Intensify_Points_D,		//3_4需求点数			
	Intensify_Points_E,		//4_5需求点数			
	Intensify_Points_F,		//5_6需求点数	

	Intensify_Points_G,		 //6_7需求点数			
	Intensify_Points_H,		//7_8需求点数			
	Intensify_Points_ENDS,		//结束
};

enum HoleState{
    kTypeUnEmbeded = 0, //已开启，未镶嵌
    kTypeEmbeded,		//已镶嵌	
    kTypeUnopen,		//未开启		
};

enum KONG_FLAG
{
	KongZero = 0, 			//第0个孔, 标识魂晶孔位
	KongFirst, 				//第一个孔， 标识魂玉第一个孔位
	KongSecond,				//第二个孔， 标识魂玉第二个孔位
	KongThird,				//第三个孔， 标识魂玉第三个孔位
};

enum HeroPosition		//英雄的位置
{
	Hero_Pos_Inns = 0,	//客栈中
	Hero_Pos_Team,		//队伍中
	//Hero_Pos_PK,		//上阵中
};

enum Train_Type
{
	Train_Common = 0,	//普通训练
	Train_Advanced,		//高级训练
	Train_Vip,			//至尊洗点
};

enum Reset_Skill_Type
{
	Reset_Skill_Gold = 0,	//金币方式重置特性
	Reset_Skill_Silver,		//银币方式重置特性
};

enum Notice_Type
{
	Notice_Error = 0,	//系统提示
	Notice_Broadcast,	//广播消息
};

enum BattleCamp			//国战阵营
{
	BATTLE_CAMP_NULL = -1,	//中立
	BATTLE_CAMP_A = 0,	//参战阵营A
	BATTLE_CAMP_B,		//参战阵营B
	BATTLE_CAMP,
};

enum WpType				//国战路点类型
{
	WP_NORMAL = 0,		//普通路点
	WP_BASE,			//大本营
};

enum RewardType			//奖励类型
{
	REWARD_BATTLE_WINNER	= 1,	//国战获胜奖励
	REWARD_BATTLE_RANKING,			//国战排名奖励
	REWARD_ARENA_RANKING,			//竞技场奖励
	REWARD_BOSS_RANKING		= 10,	//世界boss排名奖励	
	REWARD_BATTLE_WPNUM     = 20,	//国战 占有路点个数奖励
};

enum EOrderData
{
	EARENA_DATA_ARENASCORE,		// score
	EARENA_DATA_CHALLENGENUM,		// challenge number
	EARENA_DATA_BUYCHALLENGENUM, 	// bought challenge number
	EARENA_DATA_CHANGENUM, 		// change Opponent number
	EARENA_DATA_CURSTAGE,		// current stage
	EARENA_DATA_LASTREFRESHTIME,	// last refresh time
	EARENA_DATA_KNIGHTID,			// knight
};

enum EOrderType
{
	EORDER_ARENA,
	EORDER_END,
};

enum EChatType {
	ECHAT_PRIVATE, 
	ECHAT_WORLD,
	ECHAT_CAMP,
	ECHAT_KNIGHT,

	ECHAT_MAX,
};

enum City_Scene_Type
{
	Scene_Common = 1,		//普通副本
	Scene_Tavern_Battle,	//酒馆/国战
	Scene_Elite,			//精英副本
};

enum Challenge_Mode
{
	Mode_Attack = 1,		//进击模式
	Mode_Kill,				//绝杀模式
	Mode_Wipe,				//扫荡模式
};

enum Enum_ActivityType
{
    ActivityType_None = -1,     //未知
    ActivityType_LJDL = 0,      //累积登陆类型
    ActivityType_CZ = 1,        //充值
    ActivityType_DownLoad = 2,  //下载奖励
    ActivityType_RechargeA = 3, //单笔充值一次返还
    ActivityType_RechargeB = 4, //首次充值多次返还
    ActivityType_LJRecharge = 5,//累积充值
    ActivityType_MRcharge  =6,//每日充值
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int Role_Inherit		= 1<<0;		// 继承
const int Role_BeInherit	= 1<<1;		// 被继承

inline bool g_isInHerit(int flag)		{return (flag & Role_Inherit) > 0;}
inline bool g_isBeInHerit(int flag)		{return (flag & Role_BeInherit) > 0;}

inline int g_setInherit(int& flag)		{return (flag |= Role_Inherit);}		
inline int g_setBeInherit(int& flag)	{return (flag |= Role_BeInherit);}	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//属性Flag
const int Flag_OnEffect			= 1 << 0;
const int Flag_MoralCost		= 1 << 1;
const int Flag_HaveRevive		= 1 << 2;
const int Flag_ShareDamage		= 1 << 3;
const int Flag_Invincible		= 1 << 4;
const int Flag_FixStuntDamage	= 1 << 5;
const int Flag_FixedKill		= 1 << 6;
const int Flag_CanRevive		= 1 << 7;
const int Flag_Silent           = 1 << 8;

inline bool g_isOnEffect(int flag)			{ return (flag & Flag_OnEffect) > 0; }
inline bool g_isMoralCost(int flag)			{ return (flag & Flag_MoralCost) > 0; }
inline bool g_isHaveRevive(int flag)		{ return (flag & Flag_HaveRevive) > 0; }
inline bool g_isShareDamage(int flag)		{ return (flag & Flag_ShareDamage) > 0; }
inline bool g_isInvincible(int flag)		{ return (flag & Flag_Invincible) > 0; }
inline bool g_isFixStuntDamage(int flag)	{ return (flag & Flag_FixStuntDamage) > 0; }
inline bool g_isFixedKill(int flag)			{ return (flag & Flag_FixedKill) > 0; }
inline bool g_isCanRevive(int flag)			{ return (flag & Flag_CanRevive) > 0; }
inline bool g_isSilent(int flag)            { return (flag & Flag_Silent) > 0; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//战斗攻击
const int Attack_Kill		= 1<<0;		// 击杀
const int Attack_Bekill		= 1<<1;		// 被击杀
const int Attack_Jouk		= 1<<2;		// 闪避
const int Attack_Crit		= 1<<3;		// 暴击
const int Attack_Parry		= 1<<4;		// 格挡
const int Attack_Counter	= 1<<5;		// 反击
const int Attack_Floated	= 1<<6;		// 浮空
const int Attack_Destroy	= 1<<7;		// 破坏 7-10共4个破坏位置
const int Attack_Destroy_All= (1<<7) + (1<<8) + (1<<9) + (1<<10);
const int Attack_Combo		= 1<<11;	//连击
const int Attack_MyBuff		= 1<<12;	//给自己加buff
const int Attack_TaBuff		= 1<<13;	//给目标加buff
const int Attack_MyRevive	= 1<<14;	//自己复活
const int Attack_TaRevive	= 1<<15;	//目标复活

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//战报相关函数
inline void g_setKill(int& flag)	{flag |= Attack_Kill;}		//设置击杀
inline void g_setBekill(int& flag)	{flag |= Attack_Bekill;}	//设置被击杀
inline void g_setJouk(int& flag)	{flag |= Attack_Jouk;}		//设置闪避
inline void g_setCrit(int& flag)	{flag |= Attack_Crit;}		//设置暴击
inline void g_setParry(int& flag)	{flag |= Attack_Parry;}		//设置格挡
inline void g_setCounter(int& flag)	{flag |= Attack_Counter;}	//设置反击
inline void	g_setFloated(int& flag)	{flag |= Attack_Floated;}	//设置浮空
inline void	g_setCombo(int& flag)	{flag |= Attack_Combo;}		//设置连击
inline void	g_setMyBuff(int& flag)	{flag |= Attack_MyBuff;}	//设置给自己加buff
inline void	g_setTaBuff(int& flag)	{flag |= Attack_TaBuff;}	//设置给目标加buff
inline void g_setMyRevive(int& flag){flag |= Attack_MyRevive;}	//设置自己复活
inline void g_setTaRevive(int& flag){flag |= Attack_TaRevive;}	//设置目标复活
inline void g_setDestroy(int& flag, int i)	{flag |= (Attack_Destroy<<i);}	//设置破坏

inline bool	g_isKill(int flag)		{return (flag & Attack_Kill) > 0;}		//判断击杀
inline bool	g_isBekill(int flag)	{return (flag & Attack_Bekill) > 0;}	//判断被击杀
inline bool	g_isJouk(int flag)		{return (flag & Attack_Jouk) > 0;}		//判断闪避
inline bool	g_isCrit(int flag)		{return (flag & Attack_Crit) > 0;}		//判断暴击
inline bool	g_isParry(int flag)		{return (flag & Attack_Parry) > 0;}		//判断格挡
inline bool	g_isCounter(int flag)	{return (flag & Attack_Counter) > 0;}	//判断反击
inline bool	g_isFloated(int flag)	{return (flag & Attack_Floated) > 0;}	//判断浮空
inline bool	g_isCombo(int flag)		{return (flag & Attack_Combo) > 0;}		//判断连击
inline bool	g_isMyBuff(int flag)	{return (flag & Attack_MyBuff) > 0;}	//判断给自己加buff
inline bool	g_isTaBuff(int flag)	{return (flag & Attack_TaBuff) > 0;}	//判断给目标加buff
inline bool g_isMyRevive(int flag)	{return (flag & Attack_MyRevive) > 0;}	//判断自己复活
inline bool g_isTaRevive(int flag)	{return (flag & Attack_TaRevive) > 0;}	//判断目标复活
inline bool	g_isDestroy(int flag, int& i)									//判断破坏
{
	int bit = flag & Attack_Destroy_All;
	if( bit <= 0 )
		return false;
	i = 1;
	int t = Attack_Destroy;
	while( bit != t )
	{
		t = t<<1;
		i++;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//技能战斗相关参数
const int Skill_Stunt_Cost = 100;		//使用绝技的士气值
const int Skill_Anger_Cost = 100;		//使用终结技的怒气值
const int Combat_Place_Destory = 4;		//部位破坏位置数量

#define FIGHT_ROUND_MAX		30			//战斗的最大回合数

//技能攻击范围
enum SKILL_CAST_DISTANCE
{
	SKILL_CAST_CLOSE = 0,	//近程
	SKILL_CAST_LONG,		//远程
};

//技能伤害类型
enum SKILL_DAMAGE_TYPE
{
	SKILL_DAMAGE_PHYSICAL = 1,	//物理伤害
	SKILL_DAMAGE_MAGIC,			//魔法伤害
	SKILL_DAMAGE_STUNT,			//绝技伤害
};

struct CEffect
{
    int	effectID;			//效果ID
    int	effectValue;		//效果值
    int	effectProbability;	//效果几率
    int	effectAttr;			//关联属性
    
    void operator=(const CEffect& eff) {
        effectID = eff.effectID;
        effectValue = eff.effectValue;
        effectProbability = eff.effectProbability;
        effectAttr = eff.effectAttr;
    }
};

enum Buff_Type
{
	Buff_Self_Single = 0,		//本身所加buff单次效果
	Buff_Self_Repeated,			//本身所加buff重复/叠加效果
	Buff_Target_Single,			//敌人所加buff单次效果
	Buff_Target_Repeated		//敌人所加buff重复/叠加效果
};

//国战相关参数
const int g_Number_BattleBuyActionCost		= 5;	//购买挑战次数花费5金币
const int g_ClearCDTimePerGold  = 3; //国战多少分钟需要1金币来清除CD

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//obj属性定义
enum Role_Attrib
{
	///////////////int属性/////////////////////////
	Role_Attrib_Null = 0,

	Role_Attrib_ID,					//角色id
	Role_Attrib_UserID,				//账号ID
	Role_Attrib_Parent,				//拥有者	
	Role_Attrib_CreateTime,			//角色创建时间
	Role_Attrib_LoginTime,			//上次登录时间
	Role_Attrib_LogoutTime,			//上次下线时间
	Role_Attrib_TemplateID,			//模板
	Role_Attrib_Name,				//名字
	Role_Attrib_Vocation,			//职业
	Role_Attrib_Type,				//类型
	Role_Attrib_Sex,				//性别
	Role_Attrib_Camp,				//阵营
	Role_Attrib_Level,				//等级
	Role_Attrib_Vip,				//vip等级
	Role_Attrib_GoldCoin,			//金币
	Role_Attrib_SilverCoin,			//银币
	Role_Attrib_Fighting,			//战斗力
	Role_Attrib_SceneID,			//场景ID
	Role_Attrib_Position,			//场景坐标
	Role_Attrib_Exp,				//当前经验
	Role_Attrib_Hp,					//当前生命值

	//属性：当前值
	Role_Attrib_Strength,			//力量
	Role_Attrib_Intellect,			//智力
	Role_Attrib_Technique,			//技巧
	Role_Attrib_Agility,			//敏捷
	Role_Attrib_Hit,				//命中值
	Role_Attrib_Jouk,				//闪避值
	Role_Attrib_Crit,				//暴击值
	Role_Attrib_Tenacity,			//韧性值
	Role_Attrib_Parry,				//格挡值
	Role_Attrib_Treat,				//治疗力
	Role_Attrib_HpMax,				//生命值上限
	Role_Attrib_PhysiDamage,		//物理攻击
	Role_Attrib_PhysiDefense,		//物理防御
	Role_Attrib_MagicDamage,		//魔法攻击
	Role_Attrib_MagicDefense,		//魔法防御
	Role_Attrib_StuntDamage,		//绝技攻击
	Role_Attrib_StuntDefense,		//绝技防御

	//属性：基础值
	Role_Attrib_BaseStrength,		//基础力量
	Role_Attrib_BaseIntellect,		//基础智力
	Role_Attrib_BaseTechnique,		//基础技巧
	Role_Attrib_BaseAgility,		//基础敏捷
	Role_Attrib_BaseHit,			//基础命中值
	Role_Attrib_BaseJouk,			//基础闪避值
	Role_Attrib_BaseCrit,			//基础暴击值
	Role_Attrib_BaseTenacity,		//基础韧性值
	Role_Attrib_BaseParry,			//基础格挡值
	Role_Attrib_BaseTreat,			//基础治疗力
	Role_Attrib_BaseHpMax,			//基础生命值上限
	Role_Attrib_BasePhysiDamage,	//基础物理攻击
	Role_Attrib_BasePhysiDefense,	//基础物理防御
	Role_Attrib_BaseMagicDamage,	//基础魔法攻击
	Role_Attrib_BaseMagicDefense,	//基础魔法防御
	Role_Attrib_BaseStuntDamage,	//基础绝技攻击
	Role_Attrib_BaseStuntDefense,	//基础绝技防御

	//属性：附加值
	Role_Attrib_StrengthAddons,		//力量附加值
	Role_Attrib_IntellectAddons,	//智力附加值
	Role_Attrib_TechniqueAddons,	//技巧附加值
	Role_Attrib_AgilityAddons,		//敏捷附加值
	Role_Attrib_HitAddons,			//命中值附加值
	Role_Attrib_JoukAddons,			//闪避值附加值
	Role_Attrib_CritAddons,			//暴击值附加值
	Role_Attrib_TenacityAddons,		//韧性值附加值
	Role_Attrib_ParryAddons,		//格挡值附加值
	Role_Attrib_TreatAddons,		//治疗力附加值
	Role_Attrib_HpMaxAddons,		//生命值上限附加值
	Role_Attrib_PhysiDamageAddons,	//物理攻击附加值
	Role_Attrib_PhysiDefenseAddons,	//物理防御附加值
	Role_Attrib_MagicDamageAddons,	//魔法攻击附加值
	Role_Attrib_MagicDefenseAddons,	//魔法防御附加值
	Role_Attrib_StuntDamageAddons,	//绝技攻击附加值
	Role_Attrib_StuntDefenseAddons,	//绝技防御附加值

	//属性：附加百分比
	Role_Attrib_StrengthPercent,		//力量附加百分比
	Role_Attrib_IntellectPercent,		//智力附加百分比
	Role_Attrib_TechniquePercent,		//技巧附加百分比
	Role_Attrib_AgilityPercent,			//敏捷附加百分比
	Role_Attrib_HitPercent,				//命中值附加百分比
	Role_Attrib_JoukPercent,			//闪避值附加百分比
	Role_Attrib_CritPercent,			//暴击值附加百分比
	Role_Attrib_TenacityPercent,		//韧性值附加百分比
	Role_Attrib_ParryPercent,			//格挡值附加百分比
	Role_Attrib_TreatPercent,			//治疗力附加百分比
	Role_Attrib_HpMaxPercent,			//生命值上限附加百分比
	Role_Attrib_PhysiDamagePercent,		//物理攻击附加百分比
	Role_Attrib_PhysiDefensePercent,	//物理防御附加百分比
	Role_Attrib_MagicDamagePercent,		//魔法攻击附加百分比
	Role_Attrib_MagicDefensePercent,	//魔法防御附加百分比
	Role_Attrib_StuntDamagePercent,		//绝技攻击附加百分比
	Role_Attrib_StuntDefensePercent,	//绝技防御附加百分比

	Role_Attrib_PhysiRealDamageIncr,		//物理增伤值
	Role_Attrib_PhysiRealDamageIncrPer,		//物理增伤百分比
	Role_Attrib_PhysiRealDamageReducePer,	//物理减伤百分比

	Role_Attrib_MagicRealDamageIncr,		//魔法增伤值
	Role_Attrib_MagicRealDamageIncrPer,		//魔法增伤百分比
	Role_Attrib_MagicRealDamageReducePer,	//魔法减伤百分比

	Role_Attrib_StuntRealDamageIncr,		//绝技增伤值
	Role_Attrib_StuntRealDamageIncrPer,		//绝技增伤百分比
	Role_Attrib_StuntRealDamageReducePer,	//绝技减伤百分比

	//加成属性
	Role_Attrib_ExpAddition,		//经验加成百分比
	Role_Attrib_MeritAddition,		//战功加成百分比
	Role_Attrib_SilverAddition,		//银币加成百分比
	Role_Attrib_StateExpAddition,	//位阶经验加成百分比

	Role_Attrib_End,
};

//物品属性定义
enum Item_Attrib
{
	Item_Attrib_Null = 0,
	Item_Attrib_ID,					//实体ID
	Item_Attrib_Type,				//物品类型
	Item_Attrib_Career,				//职业限制
	Item_Attrib_Sex,				//性别限制
	Item_Attrib_MinLevel,			//最低等级限制
	Item_Attrib_Quatily,			//品质
	Item_Attrib_StackSize,			//堆叠数量
	Item_Attrib_StackMax,			//最大堆叠数量
	Item_Attrib_SellPrice,			//出售价格
	Item_Attrib_Parent,				//拥有者ID
	Item_Attrib_Position,			//装备位置
	Item_Attrib_EquipID,			//英雄id，role_id
	//基础属性
	Item_Attrib_Strength,			//力量
	Item_Attrib_Intellect,			//智力
	Item_Attrib_Technique,			//技巧
	Item_Attrib_Agility,			//敏捷

	//衍生属性
	Item_Attrib_Hit,				//命中值
	Item_Attrib_Jouk,				//闪避值
	Item_Attrib_Crit,				//暴击值
	Item_Attrib_Tenacity,			//韧性值
	Item_Attrib_Parry,				//格挡值
	Item_Attrib_Treat,				//治疗力

	Item_Attrib_BaseHpMax,			//基础生命值上限
	Item_Attrib_BasePhysiDamage,	//基础物理攻击
	Item_Attrib_BasePhysiDefense,	//基础物理防御
	Item_Attrib_BaseMagicDamage,	//基础魔法攻击
	Item_Attrib_BaseMagicDefense,	//基础魔法防御
	Item_Attrib_BaseStuntDamage,	//基础绝技攻击
	Item_Attrib_BaseStuntDefense,	//基础绝技防御



	Item_Attrib_Intensify,			//当前装备强化等级
	Item_Attrib_MaxLevelIntensify,	//装备强化上限
	Item_Attrib_IntensifyPoints,	//从当前强化等级强化到下一级需求点数数组[9]

	Item_Attrib_SoulType,			//种类, 日1， 月2， 星3， -1
	Item_Attrib_SoulPoints, 		//魂玉点数
	Item_Attrib_CHARGE, 			//强化手续费

	Item_Attrib_KongStoreZero,			// 孔1  存储
	Item_Attrib_KongStoreFirst, 		//孔2   存储
	Item_Attrib_KongStoreSecond, 		//孔3   存储
	Item_Attrib_KongStoreThird, 		//孔4   存储

	Item_Attrib_NextGradeTemplateId,//下一等级模板id
	Item_Attrib_UpGradeCharge,  // 装备升级手续费
	Item_Attrib_IntensifyJadePoints,
	Item_Attrib_IntensifyJadeBasePoints,
	Item_Attrib_LayCharge,
    
    Item_Attrib_SceneId,
	Item_Attrib_TemplateID,
    Item_Attrib_Name,
    
    Item_Attrib_GroupId,
    Item_Attrib_SellType,
};

//装备位置定义
enum Equip_Position
{
	Equip_Position_MainWeapon = 0,	//主手武器
	Equip_Position_AssiWeapon,		//副手武器
	Equip_Position_Chest,			//衣服
	Equip_Position_Hands,			//护手
	Equip_Position_Feets,			//靴子
	Equip_Position_Necklace,		//项链
	Equip_Position_Bracelet,		//手镯
	Equip_Position_Rings,			//戒指

	Equip_Position_End,
};

enum Equip_Quality
{
	Quality_White = 1,
	Quality_Green,
	Quality_Blue,
	Quality_Purple,
	Quality_Orange,
};

enum TYPE_COLOR
{
    COLOR_White = 1,
    COLOR_Green,
    COLOR_Blue,
    COLOR_Purple,
    COLOR_Orange,
    COLOR_Red,
};

enum Friend_State
{
	FRIEND_STATE_ONLINE	= 1,
	FRIEND_STATE_OFFLINE,
};

enum EOpponentState
{
	EOPPONENT_NORMAL = 0,
	EOPPONENT_OVER
};

// 奖励类型
enum ETaskReward {
	REWARD_NONE		= 0,		// 无
	REWARD_EXP,					// 经验
	REWARD_PRESTIGE,			// 声望
	REWARD_SILVER,				// 银币
	REWARD_ITEM,				// 道具
	REWARD_GOLD,				// gold
	REWARD_HERO,
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//obj类型定义
const int OBJTYPE_ITEM										= 200000000;	//物品
const int OBJTYPE_ITEM_EQUIPMENT							= 201000000;	  //装备
const int OBJTYPE_ITEM_EQUIPMENT_WEAPON						= 201010000;	    //武器
const int OBJTYPE_ITEM_EQUIPMENT_WEAPON_MAIN				= 201010100;	      //主手武器
const int OBJTYPE_ITEM_EQUIPMENT_WEAPON_ASSI				= 201010200;	      //副手武器
const int OBJTYPE_ITEM_EQUIPMENT_ARMOR						= 201020000;	    //防具
const int OBJTYPE_ITEM_EQUIPMENT_ARMOR_CHEST				= 201020100;	      //衣服
const int OBJTYPE_ITEM_EQUIPMENT_ARMOR_HANDS				= 201020200;	      //护手
const int OBJTYPE_ITEM_EQUIPMENT_ARMOR_FEETS				= 201020300;	      //靴子
const int OBJTYPE_ITEM_EQUIPMENT_JEWELRY					= 201030000;	    //饰品
const int OBJTYPE_ITEM_EQUIPMENT_JEWELRY_NECKLACE			= 201030100;	      //项链
const int OBJTYPE_ITEM_EQUIPMENT_JEWELRY_RINGS				= 201030200;	      //戒指
const int OBJTYPE_ITEM_EQUIPMENT_JEWELRY_BRACELET			= 201030300;	      //手镯
const int OBJTYPE_ITEM_STUFF								= 202000000;	  //材料
const int OBJTYPE_ITEM_HUNJING_WEI							= 202010000;	    //伪魂晶
const int OBJTYPE_ITEM_STUFF_LEVEVUP						= 202030000;	    //装备升级材料
const int OBJTYPE_ITEM_STUFF_STAR							= 202040000;		//星盘强化材料
const int OBJTYPE_ITEM_FUNCTION_RECUITCOMM					= 202050000;		//招募英雄的通用道具（全能碎片）
const int OBJTYPE_ITEM_FUNCTION_RECUITHERO					= 202060000;		//招募英雄的英雄碎片
const int OBJTYPE_ITEM_FUNCTION								= 203000000;	  //功能道具
const int OBJTYPE_ITEM_FUNCTION_BOX							= 203010000;		//宝箱
const int OBJTYPE_ITEM_FUNCTION_DRUGS						= 203020000;		//药剂
const int OBJTYPE_ITEM_FUNCTION_SPEAKER						= 203030000;		//小喇叭


const int OBJTYPE_ITEM_HUNJING								= 204000000;	    //魂晶
const int OBJTYPE_ITEM_HUNYU								= 205000000;	    //魂玉
const int OBJTYPE_ITEM_MIYIN								= 206000000;	    //秘银

const int OBJTYPE_ITEM_FUNCTION_WIPE						= 207000000;		//扫荡道具

#define GetLeftSingle(n)	(n/100000000)
#define GetLeftTwo(n)		(n/1000000)
#define GetLeftThree(n)		(n/10000)
#define GetLeftFour(n)		(n/100)	
#define GetLeftFive(n)		(n/1)


inline bool g_IsItem(int t)								{return GetLeftSingle(t) == GetLeftSingle(OBJTYPE_ITEM);}
inline bool g_IsItemEquipment(int t)					{return GetLeftTwo(t) == GetLeftTwo(OBJTYPE_ITEM_EQUIPMENT);}
inline bool g_IsItemEquipmentWeapon(int t)				{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_EQUIPMENT_WEAPON);}
inline bool g_IsItemEquipmentWeaponMain(int t)			{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_WEAPON_MAIN);}
inline bool g_IsItemEquipmentWeaponAssi(int t)			{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_WEAPON_ASSI);}
inline bool g_IsItemEquipmentArmor(int t)				{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_EQUIPMENT_ARMOR);}
inline bool g_IsItemEquipmentArmorChest(int t)			{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_ARMOR_CHEST);}
inline bool g_IsItemEquipmentArmorHands(int t)			{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_ARMOR_HANDS);}
inline bool g_IsItemEquipmentArmorFeets(int t)			{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_ARMOR_FEETS);}
inline bool g_IsItemEquipmentJewelry(int t)				{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_EQUIPMENT_JEWELRY);}
inline bool g_IsItemEquipmentJewelryNecklace(int t)		{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_JEWELRY_NECKLACE);}
inline bool g_IsItemEquipmentJewelryRings(int t)		{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_JEWELRY_RINGS);}
inline bool g_IsItemEquipmentJewelryBracelet(int t)		{return GetLeftFour(t) == GetLeftFour(OBJTYPE_ITEM_EQUIPMENT_JEWELRY_BRACELET);}
inline bool g_IsItemStuff(int t)						{return GetLeftTwo(t) == GetLeftTwo(OBJTYPE_ITEM_STUFF);}
inline bool g_IsItemStuffStarAct(int t)					{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_STUFF_STAR);}
inline bool g_IsItemFunctionRecuitcom(int t)			{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_FUNCTION_RECUITCOMM);}
inline bool g_IsItemFunctionRecuithero(int t)			{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_FUNCTION_RECUITHERO);}
inline bool g_IsItemFunctionWipe(int t)                 {return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_FUNCTION_WIPE);}
inline bool g_IsItemHunjing(int t)						{return GetLeftTwo(t) == GetLeftTwo(OBJTYPE_ITEM_HUNJING);}
inline bool g_IsItemHunyu(int t)						{return GetLeftTwo(t) == GetLeftTwo(OBJTYPE_ITEM_HUNYU);}
inline bool g_IsItemMiyin(int t)						{return GetLeftTwo(t) == GetLeftTwo(OBJTYPE_ITEM_MIYIN);}

inline bool g_IsItemWeihunjing(int t)					{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_HUNJING_WEI);}
inline bool g_IsItemStuffLevelUp(int t)					{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_STUFF_LEVEVUP);}
inline bool g_IsItemBox(int t)							{return GetLeftThree(t) == GetLeftThree(OBJTYPE_ITEM_FUNCTION_BOX);}

inline bool g_IsItemFunction(int t)						{return GetLeftTwo(t) == GetLeftTwo(OBJTYPE_ITEM_FUNCTION);}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	//ODIN_COMMDEF_H
