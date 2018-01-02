#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "UserActionItem.h"

//////////////////////////////////////////////////////////////////////////

typedef CWHArray<tagUserAction *> CUserActionArray;				//操作项目
//////////////////////////////////////////////////////////////////////////////////

//游戏引擎
class CGameClientEngine : public CGameFrameEngine
{
	//用户变量
protected:
	WORD							m_wBankerUser;						//庄家用户
	WORD							m_wCurrentUser;						//当前用户
	BYTE							m_cbActionMask;						//玩家动作
	BYTE							m_cbActionCard;						//动作牌
	LONG							m_lSiceCount;						//色子数值

	//辅助变量
	TCHAR							m_szAccounts[GAME_PLAYER][LEN_NICKNAME];	//玩家名称

	//托管变量
protected:
	bool							m_bStustee;							//托管标志
	WORD							m_wTimeOutCount;					//超时次数

	//动作变量
protected:
	CUserActionArray				m_UserActionArray;					//操作项目
	CUserActionArray				m_UserActionStorage;				//操作项目

	//扑克变量
protected:
	BYTE							m_cbLeftCardCount;					//剩余数目
	BYTE							m_cbCardIndex[MAX_INDEX];			//手中扑克

	BYTE							m_cbChearCardIndex[MAX_INDEX];		//作弊扑克

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	WORD							m_wHeapHead;						//牌堆头部
	WORD							m_wHeapTail;						//牌堆尾部
	BYTE							m_cbHeapCardInfo[GAME_PLAYER][2];	//堆立信息
#endif

	//组合扑克
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//组合数目
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//组合扑克

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
	CHistoryScore					m_HistoryScore;						//历史积分

	//函数定义
public:
	//构造函数
	CGameClientEngine();
	//析构函数
	virtual ~CGameClientEngine();

	//系统事件
public:
	//创建函数
	virtual bool OnInitGameEngine();
	//重置函数
	virtual bool OnResetGameEngine();

	//时钟事件
public:
	//时钟删除
	virtual bool OnEventGameClockKill(WORD wChairID);
	//时钟信息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//游戏事件
public:
	//旁观消息
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户出牌
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//操作提示
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//操作结果
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//用户托管
	bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//控制消息
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);
	//其他人牌
	bool OnSubOtherCard(const void * pBuffer, WORD wDataSize);
	//控制消息
	bool OnSubAdminUpdate(const void * pBuffer, WORD wDataSize);

	//辅助函数
protected:
	//处理消息
	bool OnGameMsg(bool bNextMsg);
	//播放声音
	void PlayCardSound(WORD wChairID,BYTE cbCardData);
	//播放声音
	void PlayActionSound(WORD wChairID,BYTE cbAction);
	//出牌判断
	bool VerdictOutCard(BYTE cbCardData);
	//设置扑克
	void SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard);
	//获取操作信息
	BYTE GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] );

	//动作操作函数
protected:
	//开始玩家动作
	bool BeginUserAction();
	//停止玩家动作
	bool StopUserAction();
	//移除当前动作
	bool RemoveCurrentAction();
	//激活动作
	tagUserAction *ActiveUserAction(enmActionKind enActionKind);
	//发牌动作
	bool BeginActionStartCard();
	//出牌动作
	bool BeginActionOutCard();
	//发牌动作
	bool BeginActionSendCard();
	//提示操作
	bool BeginActionActionNotify();
	//操作结果
	bool BeginActionActionResult();
	//掷色子
	bool BeginActionSice();
	//游戏结束
	bool BeginActionGameEnd();
	//发牌完成
	bool OnActionStartCard();
	//出牌完成
	bool OnActionOutCard();
	//抓牌完成
	bool OnActionSendCard();
	//提示完成
	bool OnActionActionNotify();
	//操作完成
	bool OnActionActionResult();
	//掷色子完成
	bool OnActionSice();
	//游戏结束
	bool OnActionGameEnd();

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//出牌操作
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//扑克操作
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	//拖管控制
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//第二次摇色子消息
	LRESULT OnSiceTwo(WPARAM wParam, LPARAM lParam);
	//玩家操作
	LRESULT OnUserAction( WPARAM wParam, LPARAM lParam );
	//操作完成
	LRESULT OnUserActionFinish(WPARAM wParam, LPARAM lParam);
	//聊天消息
	LRESULT OnChatMessage(WPARAM wParam, LPARAM lParam);
	//游戏关闭
	LRESULT OnMessageGameClose(WPARAM wParam, LPARAM lParam);
	//管理员请求
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif