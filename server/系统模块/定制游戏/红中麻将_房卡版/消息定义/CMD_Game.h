#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////

//配牌器开关
#ifdef _DEBUG
#define CARD_DISPATCHER_CONTROL
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//服务定义

//游戏属性
#define KIND_ID						389									//游戏 I D
#define GAME_NAME					TEXT("泗阳麻将")					//游戏名字

//组件属性
#define GAME_PLAYER					4									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define MAX_ZUI						9									//嘴最大数

#define HUANG_ZHUANG_HEAP_CNT		7									// 黄庄剩余堆数

#define MAX_WEAVE					4									//最大组合
#define MAX_INDEX					42									//最大索引
#define MAX_HAND_COUNT					14									//最大数目
#define MAX_REPERTORY				144									//最大库存
#define FENG_INDEX					27									//风牌索引
#define FLOWER_INDEX				31									//花牌索引
#define MAX_FLOWER_CNT				20									//花牌个数
#define MAX_COLOR					5									//最大花色
#define WEN_QIAN_INDEX				18									//文钱索引

#define MAX_HUA_INDEX				11									//花牌索引

#define MAX_WEAVE_CNT				4									//最大碰杠数

//扑克定义
#define HEAP_FULL_COUNT				38									//堆立全牌
#define MAX_RIGHT_COUNT				1									//最大权位DWORD个数	

//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////////////
//状态定义

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_CHOOSE			GAME_SCENE_FREE + 1				//等待选嘴
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//游戏进行

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
#define CARD_COLOR_FENG			4

struct CMD_S_RECORD
{
	int		 nCount;
	BYTE cbHuCount[GAME_PLAYER];			//胡牌次数
	BYTE cbZimoCount[GAME_PLAYER];			//自摸次数
	BYTE cbJiePaoCount[GAME_PLAYER];		//自摸次数
	BYTE cbDiaoPaoCount[GAME_PLAYER];		//自摸次数
	BYTE cbAnGang[GAME_PLAYER];				//暗杠次数
	BYTE cbMingGang[GAME_PLAYER];			//明杠次数
	SCORE lAllScore[GAME_PLAYER];			//总结算分
	SCORE lDetailScore[GAME_PLAYER][32];	//单局结算分
};

//空闲状态
struct CMD_S_StatusFree
{
	//游戏属性
	LONG							lCellScore;									//基础积分

	//时间信息
	BYTE							cbTimeOutCard;								//出牌时间
	BYTE							cbTimeOperateCard;						//操作时间
	BYTE							cbTimeStartGame;							//开始时间

	//历史积分
	SCORE							lTurnScore[GAME_PLAYER];					//积分信息
	SCORE							lCollectScore[GAME_PLAYER];					//积分信息

	//规则					
	BYTE							cbPlayerCount;							//玩家人数
	BYTE							cbMaxZuiCount;							//最大嘴数量 
	BYTE							cbZuiScore;								//嘴的分数
};

//组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;								//组合类型
	BYTE							cbCenterCard;								//中心扑克
	BYTE							cbParam;									//类型标志
	WORD							wProvideUser;								//供应用户
	BYTE							cbCardData[MAX_WEAVE_CNT];								//麻将数据
};

//游戏状态
struct CMD_S_StatusChooseZui
{
	BYTE							cbPlayerCount;								//玩家人数
	BYTE							cbMaxZuiCount;								//最大嘴数量 
	BYTE							cbZuiScore;									//嘴分数
};

//游戏状态
struct CMD_S_StatusPlay
{
	//时间信息
	BYTE							cbTimeOutCard;								//出牌时间
	BYTE							cbTimeOperateCard;							//叫分时间
	BYTE							cbTimeStartGame;							//开始时间

	//游戏变量
	LONGLONG						lCellScore;									//单元积分
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户
	BYTE							cbMagicIndex;								//财神索引

	//规则					
	BYTE							cbPlayerCount;								//玩家人数
	BYTE							cbMaxZuiCount;								//最大嘴数量 
	BYTE							cbZuiScore;									//嘴分数

	//状态变量
	BYTE							cbActionCard;								//动作扑克
	BYTE							cbActionMask;								//动作掩码
	BYTE							cbLeftCardCount;							//剩余数目
	bool							bTrustee[GAME_PLAYER];						//是否托管
	bool							bTing[GAME_PLAYER];							//是否听牌
	bool							bIsGuoZhuang;
	BYTE							cbZuiInfo[GAME_PLAYER][MAX_ZUI];        // 每个玩家选嘴情况

	//出牌信息
	WORD							wOutCardUser;								//出牌用户
	BYTE							cbOutCardData;								//出牌扑克
	BYTE							cbDiscardCount[GAME_PLAYER];				//丢弃数目
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//丢弃记录

	//扑克数据
	BYTE							cbCardCount[GAME_PLAYER];					//扑克数目
	BYTE							cbCardData[MAX_HAND_COUNT];					//扑克列表
	BYTE							cbFlowerCount[GAME_PLAYER];
	BYTE							cbFlowerCardData[GAME_PLAYER][MAX_FLOWER_CNT];
	BYTE							cbTatalFlowerCount[GAME_PLAYER];
	BYTE							cbSendCardData;								//发送扑克

	//组合扑克
	BYTE							cbWeaveItemCount[GAME_PLAYER];				//组合数目
	tagWeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//组合扑克

	//堆立信息
	WORD							wHeapHead;									//堆立头部
	WORD							wHeapTail;									//堆立尾部
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆牌信息

	BYTE							cbHuCardCount[MAX_HAND_COUNT];
	BYTE							cbHuCardData[MAX_HAND_COUNT][28];
	BYTE							cbOutCardCount;
	BYTE							cbOutCardDataEx[MAX_HAND_COUNT];


	//历史积分
	SCORE							lTurnScore[GAME_PLAYER];					//积分信息
	SCORE							lCollectScore[GAME_PLAYER];					//积分信息
};

//////////////////////////////////////////////////////////////////////////////////
//命令定义

#define SUB_S_SHOW_FLOWER			98									//亮花消息
#define SUB_S_GAME_CHOOSE			99									//游戏选嘴
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//用户出牌
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_OPERATE_NOTIFY		103									//操作提示
#define SUB_S_OPERATE_RESULT		104									//操作命令
#define SUB_S_LISTEN_CARD			105									//用户听牌
#define SUB_S_TRUSTEE				106									//用户托管
#define SUB_S_REPLACE_CARD			107									//用户补牌
#define SUB_S_GAME_CONCLUDE			108									//游戏结束
#define SUB_S_SET_BASESCORE			109									//设置基数
#define SUB_S_HU_CARD				110									//听牌胡牌数据
#define SUB_S_RECORD				111									//游戏记录
#define SUB_S_FLOWER_CNT			112									//花牌张数

//发送选嘴命令
struct CMD_S_GameChoose
{
	bool							bChooseZui;
};

//发送扑克
struct CMD_S_GameStart
{
	WORD				 			wBankerUser;						//当前庄家
	WORD				 			wReplaceUser;						//补花用户
	WORD							wSiceCount;							//骰子点数
	WORD							wHeapHead;							//牌堆头部
	WORD							wHeapTail;							//牌堆尾部
	BYTE                            cbMagicIndex;                       //财神索引
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];		//堆立信息
	BYTE							cbUserAction;						//用户动作
	BYTE							cbCardData[MAX_HAND_COUNT];				//麻将列表
	BYTE							cbOutCardCount;
	BYTE							cbOutCardData[MAX_HAND_COUNT][28];
	BYTE							cbZuiInfo[GAME_PLAYER][MAX_ZUI];	// 玩家选嘴情况
};

//亮花消息
struct CMD_S_SHOW_FLOWER
{
	WORD							wChairID;								//亮花玩家
	BYTE							cbCardCount;							//亮花张数
	BYTE							cbCardData[MAX_HAND_COUNT];					//花牌
	BYTE							cbGetCardData[MAX_HAND_COUNT];				//补牌
};


//机器人扑克
struct CMD_S_AndroidCard
{
	BYTE							cbHandCard[GAME_PLAYER][MAX_HAND_COUNT];//手上扑克
	WORD							wCurrentUser ;						//当前玩家
};

//用户出牌
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
	bool							bSysOut;							//托管系统出牌
};

//发送扑克
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//扑克数据
	BYTE							cbActionMask;						//动作掩码
	WORD							wCurrentUser;						//当前用户
	WORD							wSendCardUser;						//发牌用户
	WORD							wReplaceUser;						//补牌用户
	bool							bTail;								//末尾发牌
	bool							bIsGuoZhuang;
};

//操作提示
struct CMD_S_OperateNotify
{
	BYTE							cbActionMask;						//动作掩码
	BYTE							cbActionCard;//[3];					//动作扑克
};

//操作命令
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//操作用户
	BYTE							cbActionMask;						//动作掩码
	WORD							wProvideUser;						//供应用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard[3];					//操作扑克
	bool							bIsGuoZhuang;
};

struct CMD_S_FlowerResult
{
	WORD							wOperateUser;						//操作用户
	BYTE							cbFlowerCnt;						//动作掩码
};

struct CMD_S_Hu_Data
{
	BYTE							cbOutCardCount;								  //出哪几张能听
	BYTE							cbOutCardData[MAX_HAND_COUNT];
	BYTE							cbHuCardCount[MAX_HAND_COUNT];				  //听后能胡哪几张牌
	BYTE							cbHuCardData[MAX_HAND_COUNT][28];
	BYTE							cbHuCardRemainingCount[MAX_HAND_COUNT][28];   //胡牌剩余数
};

//操作命令
struct CMD_S_ListenCard
{
	WORD							wListenUser;				//听牌用户
	bool							bListen;					//是否听牌
	BYTE							cbHuCardCount;				//胡几张牌
	BYTE							cbHuCardData[MAX_INDEX];    //胡牌数据
};

//游戏结束
struct CMD_S_GameConclude
{
	//积分变量
	LONG							lCellScore;										//单元积分
	SCORE							lGameScore[GAME_PLAYER];						//游戏积分
	SCORE							lRevenue[GAME_PLAYER];							//税收积分
	SCORE							lGangScore[GAME_PLAYER];						//本局杠输赢分
	SCORE							lZuiScore[GAME_PLAYER][MAX_ZUI];        // 每个

																					//结束信息
	WORD							wProvideUser;									//供应用户
	BYTE							cbProvideCard;									//供应扑克
	BYTE							cbSendCardData;									//最后发牌
	DWORD							dwChiHuKind[GAME_PLAYER];						//胡牌类型
	DWORD							dwChiHuRight[GAME_PLAYER][MAX_RIGHT_COUNT];		//胡牌类型	
	WORD							wLeftUser;										//玩家逃跑
	WORD							wLianZhuang;									//连庄
	
	//游戏信息
	BYTE							cbCardCount[GAME_PLAYER];						//扑克数目
	BYTE							cbHandCardData[GAME_PLAYER][MAX_HAND_COUNT];	//扑克列表

	BYTE							cbWeaveItemCount[GAME_PLAYER];					//组合数目
	tagWeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];			//组合扑克

	BYTE							cbHandFlowerCard[GAME_PLAYER][MAX_FLOWER_CNT];		//花牌
	BYTE							cbFlowerCount[GAME_PLAYER];							//花朵个数
	BYTE							cbTotalFlowerCount[GAME_PLAYER];							//花朵个数
	BYTE							cbChooseZuiInfo[GAME_PLAYER][MAX_ZUI];        // 每个玩家选嘴情况
};

//用户托管
struct CMD_S_Trustee
{
	bool							bTrustee;							//是否托管
	WORD							wChairID;							//托管用户
};

//补牌命令
struct CMD_S_ReplaceCard
{
	WORD							wReplaceUser;						//补牌用户
	BYTE							cbReplaceCard;						//补牌扑克 
};

//////////////////////////////////////////////////////////////////////////////////
//命令定义

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			2									//操作扑克
#define SUB_C_LISTEN_CARD			3									//用户听牌
#define SUB_C_TRUSTEE				4									//用户托管
#define SUB_C_REPLACE_CARD			5									//用户补牌
#define SUB_C_SEND_CARD             6                                   //发送扑克 
#define SUB_C_CHOOSE_ZUI            7                                   //发送扑克 

//用户选嘴
struct CMD_C_ChooseZuiInfo
{
	BYTE							cbZui[MAX_ZUI];						// 玩家选嘴情况
};


//出牌命令
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//扑克数据
};

//操作命令
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard[3];					//操作扑克
};

//用户听牌
struct CMD_C_ListenCard
{
	bool							bListenCard;						//是否听牌	
};


//用户托管
struct CMD_C_Trustee
{
	bool							bTrustee;							//是否托管	
};

//补牌命令
struct CMD_C_ReplaceCard
{
	BYTE							cbCardData;							//扑克数据
};


//发送扑克
struct CMD_C_SendCard
{
	BYTE							cbControlGameCount;				   //控制次数
	BYTE                            cbCardCount;                       //扑克数目
	WORD                            wBankerUser;                       //控制庄家
	BYTE                            cbCardData[MAX_REPERTORY];         //扑克数据
};

//听牌分析结果
struct tagTingCardResult
{
	BYTE bAbandonCount;													//可丢弃牌数
	BYTE bAbandonCard[MAX_HAND_COUNT];									//可丢弃牌
	BYTE bTingCardCount[MAX_HAND_COUNT];								//听牌数目
																		// 第一个Max_count是打什么牌出去 第二个是打什么牌出去后可以胡的牌
	BYTE bTingCard[MAX_HAND_COUNT][MAX_HAND_COUNT - 1];					//听牌
	BYTE bRemainCount[MAX_HAND_COUNT];									//听的牌的剩余数
};

//////////////////////////////////////////////////////////////////////////////////

//杠牌得分
struct tagGangScore
{
	bool		bMingGang;							//杠个数
	BYTE		cbGangCount;						//杠个数
	LONGLONG	lScore[MAX_WEAVE][GAME_PLAYER];		//每个杠得分
};


#pragma pack()

#endif