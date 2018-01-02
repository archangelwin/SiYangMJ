#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////
// global modify flag
#define GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG						1
//////////////////////////////////////////////////////////////////////////////////
//服务定义

//游戏属性
#define KIND_ID						300									//游戏 I D
#define GAME_NAME					TEXT("大众麻将")					//游戏名字

//组件属性
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define GAME_PLAYER					2									//游戏人数
#else
#define GAME_PLAYER					4									//游戏人数
#endif
#define VERSION_SERVER				PROCESS_VERSION(7,0,2)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,2)				//程序版本

//////////////////////////////////////////////////////////////////////////////////
//状态定义

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//游戏进行


//常量定义
#define MAX_WEAVE					4									//最大组合
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define MAX_INDEX					16									//最大索引
#else
#define MAX_INDEX					34									//最大索引
#endif
#define MAX_COUNT					14									//最大数目
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define MAX_REPERTORY				64									//最大库存
#else
#define MAX_REPERTORY				136									//最大库存
#endif
#define MAX_HUA_CARD				0									//花牌个数

//扑克定义
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define HEAP_FULL_COUNT				16									//堆立全牌
#else
#define HEAP_FULL_COUNT				34									//堆立全牌
#endif

#define MAX_RIGHT_COUNT				1									//最大权位DWORD个数			

//////////////////////////////////////////////////////////////////////////

//组合子项
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户
	BYTE							cbCardData[4];						//组合数据
};

//请求数据
struct CMD_C_AdminReq
{
	BOOL bBit;															//占位符号
	BYTE cbReqType;														//请求类型
#define RQ_ASK_CARD_DATA				1
#define RQ_SET_CARD_DATA				2
	BYTE cbSelectCount;													//选择数目
	BYTE cbHandSelectData[MAX_COUNT];									//选择数据
	BYTE cbHeapSelectData[MAX_COUNT];									//选择数据
};

//请求回复
struct CMD_S_CommandResult
{
	BOOL bBit;															//占位符号
	BYTE cbAckType;														//回复类型
#define ACK_ASK_CARD_DATA				1
#define ACK_SET_CARD_DATA				2
	BYTE cbResult;														//返回结果
#define CR_ACCEPT						3								//接受
#define CR_REFUSAL						4								//拒绝
	BYTE cbRepertoryCardCount;											//牌堆数目
	BYTE cbRepertoryCard[MAX_REPERTORY];								//牌堆数据
};

//更新数据
struct CMD_C_CommandUpdate
{
	BYTE cbCardCount;													//选择数目
	BYTE cbHandCardData[MAX_COUNT];										//选择数据
	BYTE cbSelectCardData[MAX_COUNT];									//选择数据
};

//作弊数据
struct CMD_S_OtherCard
{
	BYTE cbCardCount;													//选择数目
	BYTE cbCardData[MAX_COUNT];											//选择数据
};

#define IDM_ADMIN_COMMDN				WM_USER+1000
#define IDM_ADMIN_UPDATE				WM_USER+1001

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//出牌命令
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_OPERATE_NOTIFY		104									//操作提示
#define SUB_S_OPERATE_RESULT		105									//操作命令
#define SUB_S_GAME_END				106									//游戏结束
#define SUB_S_TRUSTEE				107									//用户托管
#define SUB_S_REPLACE_CARD			108									//用户补牌
#define SUB_S_AMDIN_COMMAND			180									//系统控制
#define SUB_S_OTHER_CARD			181									//其他人牌
#define SUB_S_UPDATE_CARD			182									//更新手牌

//游戏状态
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//基础金币
	WORD							wBankerUser;						//庄家用户
	bool							bTrustee[GAME_PLAYER];				//是否托管

	//历史积分
	LONGLONG						lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG						lCollectScore[GAME_PLAYER];			//积分信息
};

//游戏状态
struct CMD_S_StatusPlay
{
	//游戏变量
	LONGLONG						lCellScore;									//单元积分
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户

	//状态变量
	BYTE							cbActionCard;								//动作扑克
	BYTE							cbActionMask;								//动作掩码
	BYTE							cbLeftCardCount;							//剩余数目
	bool							bTrustee[GAME_PLAYER];						//是否托管

	//出牌信息
	WORD							wOutCardUser;								//出牌用户
	BYTE							cbOutCardData;								//出牌扑克
	BYTE							cbDiscardCount[GAME_PLAYER];				//丢弃数目
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//丢弃记录

	//扑克数据
	BYTE							cbCardCount;								//扑克数目
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbSendCardData;								//发送扑克

	//组合扑克
	BYTE							cbWeaveCount[GAME_PLAYER];					//组合数目
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//组合扑克

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//堆立信息
	WORD							wHeapHead;									//堆立头部
	WORD							wHeapTail;									//堆立尾部
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆牌信息
#endif

	//历史积分
	LONGLONG						lTurnScore[GAME_PLAYER];					//积分信息
	LONGLONG						lCollectScore[GAME_PLAYER];					//积分信息
};

//游戏开始
struct CMD_S_GameStart
{	
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户
	BYTE							cbUserAction;								//用户动作
	BYTE							cbLianZhuangCount;							//连庄计数
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//扑克列表
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	WORD							wHeapHead;									//堆立牌头
	WORD							wHeapTail;									//堆立牌尾
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆立信息
#endif
	//骰子
	BYTE							cbSick[2];							//骰子
};

//出牌命令
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
};

//发送扑克
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//扑克数据
	BYTE							cbActionMask;						//动作掩码
	WORD							wCurrentUser;						//当前用户
	WORD							wSendCardUser;						//发牌用户
	bool							bTail;								//末尾发牌
	BYTE							cbLeftCardCount;					//剩余数目
};

//操作提示
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//还原用户
	BYTE							cbActionMask;						//动作掩码
	BYTE							cbActionCard;						//动作扑克
};

//操作命令
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//操作用户
	BYTE							cbActionMask;						//动作掩码
	WORD							wProvideUser;						//供应用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard[3];					//操作扑克
};

//游戏结束
struct CMD_S_GameEnd
{
	LONGLONG						lGameTax;							//游戏税收
	//结束信息
	WORD							wProvideUser;						//供应用户
	BYTE							cbProvideCard;						//供应扑克
	DWORD							dwChiHuKind[GAME_PLAYER];			//胡牌类型
	DWORD							dwChiHuRight[GAME_PLAYER][MAX_RIGHT_COUNT];		//胡牌类型

	//积分信息
	LONGLONG						lGameScore[GAME_PLAYER];			//游戏积分
	LONGLONG						lGangScore[GAME_PLAYER];			//杠牌得分

	//扑克信息
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//扑克数据

	WORD							wLeftUser;							//玩家逃跑
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

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			3									//操作扑克
#define SUB_C_TRUSTEE				4									//用户托管
#define SUB_C_REPLACE_CARD			5									//用户补牌
#define SUB_C_AMDIN_COMMAND			8									//系统控制

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

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
