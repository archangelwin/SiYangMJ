#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "GameScore.h"
#include "SkinButtonEx.h"
#include "ChatEdit.h"
#include "HistoryScore.h"
#include "ClientControl.h"
#include "ShowCard.h"
//////////////////////////////////////////////////////////////////////////
//消息定义

#define IDM_START					(WM_USER+100)						//开始消息
#define IDM_OUT_CARD				(WM_USER+101)						//出牌消息
#define IDM_TRUSTEE_CONTROL			(WM_USER+102)						//托管控制
#define IDM_CARD_OPERATE			(WM_USER+106)						//玩家操作牌
#define IDM_USER_ACTION_FINISH		(WM_USER+107)						//动画结束
#define IDM_CHAT_MESSAGE			WM_USER+208							//聊天消息
#define IDM_USER_ACTION				(WM_USER+108)						//用户动作
#define IDM_GAME_CLOSE				(WM_USER+109)						//游戏关闭

//////////////////////////////////////////////////////////////////////////

//视图位置
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define MYSELF_VIEW_ID				1									//视图位置
#else
#define MYSELF_VIEW_ID				2									//视图位置
#endif
#define NAME_WIDTH						85

//////////////////////////////////////////////////////////////////////////////////
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
// heap card is removed, start-card and send-card need a fixed start-point to move card
#define		HEAP_START_POINT_X		50
#define		HEAP_START_POINT_Y		50
#else
#endif
struct tagMoveCardItem
{
	CPoint			ptFrom;												//开始位置
	CPoint			ptTo;												//到达位置
	WORD			wViewChairId;										//出牌或发牌视图位置
	BYTE			cbCardData;											//出牌或发牌数据
	BYTE			cbCardCount;										//牌数
};

//游戏视图
class CGameClientView : public CGameFrameViewD3D
{
	//标志变量
protected:
	bool							m_bOutCard;							//出牌标志
	bool							m_bWaitOther;						//等待标志
	bool							m_bHuangZhuang;						//荒庄标志
	bool							m_bTrustee[GAME_PLAYER];			//是否托管
	bool							m_bListenStatus[GAME_PLAYER];		//听牌标志
	bool							m_bShowChatMessage;					//显示聊天
	CBrush							m_brush;							//背景画刷
	INT								m_nHeight;							//窗口高度
	INT								m_nWidth;							//窗口宽度	

	//游戏属性
protected:
	LONGLONG						m_lCellScore;						//基础积分
	WORD							m_wBankerUser;						//庄家用户
	WORD							m_wCurrentUser;						//当前用户
	BYTE							m_cbFengQuan;						//风圈
	BYTE							m_cbLeftCardCount;					//剩余扑克
	BYTE							m_cbLianZhuangCount;				//连庄计数

	//动作动画
protected:
	bool							m_bBombEffect;						//动作效果
	BYTE							m_cbBombFrameIndex;					//帧数索引

	bool							m_cbSick[6];						//骰子
	BYTE							m_cbSicFrameIndex[6];				//帧数索引

	//丢弃效果
	WORD							m_wDiscUser;						//丢弃用户
	BYTE							m_cbDiscFrameIndex;					//帧数索引	

	//发牌或出牌动画变量
protected:
	bool							m_bEnableAnimate;					//是否显示发牌动画
	bool							m_bCardMoving;						//是否在动画
	int								m_nXStep;							//X步长
	int								m_nYStep;							//Y步长
	int								m_nStepCount;						//移动步数
	enDirection						m_enMoveDirection;					//移动方向
	CRect							m_rcCardMove;						//绘画区域
	tagMoveCardItem					m_MoveCardItem;						//动画项目

	//历史积分
public:
	bool							m_bShowScore;						//成绩标志
	bool							m_bShowControl;						//显示吃碰杠窗口
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//成绩信息

	//用户状态
protected:
	BYTE							m_cbCardData;						//出牌扑克
	WORD							m_wOutCardUser;						//出牌用户
	BYTE							m_cbUserAction[GAME_PLAYER];		//用户动作

	//位置变量
protected:
	CPoint							m_UserFlagPos[GAME_PLAYER];			//标志位置
	CPoint							m_UserListenPos[GAME_PLAYER];		//标志位置
	CPoint							m_PointTrustee[GAME_PLAYER];		//托管位置

	//文字显示
protected:
	//字体
	CD3DFontEx						m_FontName;							//名字字体

	//位图变量
protected:
	CD3DTexture						m_ImageCenter;						//LOGO图	
	CD3DTexture						m_PngFrameWindow;					//游戏构架
	CD3DTexture						m_PngTableChair;					//椅子资源
	CD3DTexture						m_ImageHuangZhuang;					//荒庄标志
	
	CD3DTexture						m_ImageWait;						//等待提示
	CD3DTexture						m_ImageOutCard;						//出牌提示
	CD3DTexture						m_ImageUserFlag;					//用户标志
	CD3DTexture						m_ImageActionBack;					//动作背景
	CD3DTexture						m_ImageTrustee;						//托管标志
	CD3DTexture						m_PngFengQuan;						//圈风标志
	CD3DTexture						m_PngListenFlag;					//听牌标志
	CD3DTexture						m_ImageActionAni;					//吃牌动画资源
	CD3DTexture						m_ImageDisc;						//丢弃效果
	CD3DTexture						m_ImageArrow;						//定时器箭头	
	CD3DTexture						m_PngLeftBoy;						//人物头像
	CD3DTexture						m_PngLeftGirl;						//人物头像
	CD3DTexture						m_PngTopBoy;						//人物头像
	CD3DTexture						m_PngTopGirl;						//人物头像
	CD3DTexture						m_PngRightBoy;						//人物头像
	CD3DTexture						m_PngRightGirl;						//人物头像
	CD3DTexture						m_PngUserFramePlayer;				//信息框架
	CD3DTexture						m_PngHeapBack;						//牌堆背景
	CD3DTexture						m_PngHeapCount;						//数目背景
	CD3DTexture						m_PngHeapNumber;					//数字图片
	CD3DTexture						m_PngScoreInfo;						//信息提示
	CD3DTexture						m_PngLZhuangNummber;				//连庄次数
	CD3DTexture						m_PngMyScoreNum;					//自己金币数字
	CD3DTexture						m_PngChatBack;						//聊天背景
	CD3DTexture						m_PngGameLogo;						//游戏LOGO
	CD3DTexture						m_PngHistoryScore;					//积分背景
	CD3DTexture						m_ImageSice1;						//骰子
	CD3DTexture						m_ImageSice2;						//骰子
	CD3DTexture						m_ImageSice3;						//骰子
	CD3DTexture						m_ImageSice4;						//骰子
	CD3DTexture						m_ImageSice5;						//骰子
	CD3DTexture						m_ImageSice6;						//骰子
	CD3DTexture						m_ImageControlBack;					//吃碰杠按钮底图

	CD3DTexture		    			m_ImageUserReady;
	CD3DTexture						m_ImageTimeBack;
	CD3DTexture						m_ImageTimeNumber;

	//扑克控件
public:
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	CHeapCard						m_HeapCard[GAME_PLAYER];			//堆立扑克
#endif
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
	CUserCard						m_UserCard[1];						//用户扑克
#else
	CUserCard						m_UserCard[3];						//用户扑克
#endif
	CTableCard						m_TableCard[GAME_PLAYER];					//桌面扑克
	CWeaveCard						m_WeaveCard[GAME_PLAYER][MAX_WEAVE];		//组合扑克
	CDiscardCard					m_DiscardCard[GAME_PLAYER];					//丢弃扑克
	CCardControl					m_HandCardControl;					//手上扑克
	//CEdit							m_ChatInput;						//输入控件
	//CChatEdit						m_ChatDisplay;						//显示控件
	CGameScore						m_GameScore;						//成绩框

	//作弊功能
	CCardControl					m_HandCardCheat;					//作弊扑克
	bool							m_bChearUser;						//作弊用户
	CButton							m_btOpenAdmin;						//系统控制
	CShowCard*						m_pClientControlDlg;				//控制窗口

	//控件变量
public:
	CVirtualButton					m_btStart;							//开始按钮
	//CSkinButtonEx					m_btSendMessage;					//发送消息
	CVirtualButton					m_btStusteeControl;					//拖管控制
	//CSkinButtonEx					m_btHistoryChat;					//查看聊天
	CVirtualButton					m_btHistoryScore;					//查看积分
	CVirtualButton					m_btPeng;							//控制按钮
	CVirtualButton					m_btGang;							//控制按钮
	CVirtualButton					m_btChiHu;							//控制按钮
	CVirtualButton					m_btGiveUp;							//控制按钮
	CVirtualButton					m_btChi;							//控件按钮	
	CVirtualButton					m_btCloseScore;						//关闭按钮
	CVirtualButton					m_btContiueGame;					//继续游戏

	//控制变量
public:
	//D3D 
	CD3DDevice *					m_pD3DDevice;						//D3D 指针

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//重载函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CDC * pDC, int nWidth, int nHeight);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//界面函数
protected:
	//动画驱动
	virtual VOID CartoonMovie();
	//配置设备
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//绘画界面
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//功能函数
public:
	//基础积分
	void SetCellScore(LONGLONG lCellScore);
	//庄家用户
	void SetBankerUser(WORD wBankerUser);
	//荒庄设置
	void SetHuangZhuang(bool bHuangZhuang);
	//状态标志
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//出牌信息
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//动作信息
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//设置积分
	VOID SetHistoryScore(WORD wChairID, tagHistoryScore * pHistoryScore);
	//设置动作
	bool SetBombEffect(bool bBombEffect);
	//丢弃用户
	void SetDiscUser(WORD wDiscUser);
	//定时玩家
	void SetCurrentUser(WORD wCurrentUser);
	//设置托管
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);
	//停止出牌或发牌动画
	bool StopMoveCard();
	//是否动画进行中
	bool IsMovingCard() { return m_bCardMoving; }
	//允许动画
	void EnableAnimate( bool bAnimate );
	//是否允许动画
	bool IsEnableAnimate() { return m_bEnableAnimate; }
	//扑克动画
	void OnMoveCardItem( const tagMoveCardItem &MoveCardItem );
	//设置剩余
	VOID SetLeftCardCount(BYTE cbLeftCardCount);
	//设置连庄
	void SetLZhuangCount(BYTE cbCount);
	//系统消息
	void InsertSystemMessage(LPCTSTR pszString);
	//聊天消息
	void InsertChatMessage(LPCTSTR pszString);
	//常规消息
	void InsertNormalMessage(LPCTSTR pszString);
	//聊天消息
	void InsertAllChatMessage(LPCTSTR pszString, int nMessageType);
	//消息函数
	void InsertMessage(LPCTSTR pszString,COLORREF color=RGB(193,167,108));
	//半角转全角
	CString StringHalfToFull(CString strContent);
	//全角转半角
	CString StringFullToHalf(CString strContent);
	//绘画骰子
	void DrawSick(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//设置骰子
	bool SetSick(bool bSickEffect,BYTE bSickIndex);
	//设置状态
	VOID SetControlInfo(bool bShow,BYTE cbAcitonMask);	


	//辅助函数
protected:
	//绘画时间
	void DrawUserTimerEx(CD3DDevice * pD3DDevice, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//绘画准备
	void DrawUserReadyEx(CD3DDevice * pD3DDevice, int nXPos, int nYPos);
	//绘画扑克动画
	void DrawMoveCardItem( CD3DDevice * pD3DDevice );
	// 绘画数字
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_RIGHT);
	// 绘画数字
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_RIGHT);
	//艺术字体
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, UINT nFormat);
	//艺术字体
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect, UINT nFormat);

	//消息映射
protected:
	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//定时器
	afx_msg void OnTimer(UINT nIDEvent);
	//发送消息
	afx_msg void OnSendMessage();
	//控件颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//查看聊天
	afx_msg void OnHideChatMessage();
	//控制按钮
	afx_msg VOID OnBnClickedAdmin();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif