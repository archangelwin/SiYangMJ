#pragma once

#include "Stdafx.h"
#include "Resource.h"
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//游戏配置
class CShowCard : public CDialog
{
	//控件变量
public:
	CSkinButton						m_btOK;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮

	CPngImage						m_ImageCard;						//扑克资源
	int								m_nWidth;							//扑克宽度
	int								m_nHeight;							//扑克高度

	CGameClientEngine *				m_pClientEngine;					//结构指针

	//选中扑克
private:
	BYTE							m_cbSelCardData[MAX_COUNT];			//牌堆数据
	BYTE							m_cbSelCardCount;					//扑克数量
private:
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];	//牌堆数据
	BYTE							m_cbCardCount;						//扑克数量
	CRect							m_rcCard[MAX_REPERTORY];			//记录区域
	bool							m_bSelectCard[MAX_REPERTORY];		//是否选中

	//手牌设置	
private:
	BYTE							m_cbHandSelCardData[MAX_INDEX];		//牌堆数据
	BYTE							m_cbHandSelCardCount;				//扑克数量

	BYTE							m_cbHandCardData[MAX_COUNT];		//手中扑克
	BYTE							m_cbHandCardCount;					//扑克数量
	CRect							m_rcHandCard[MAX_COUNT];			//记录区域
	bool							m_bHandSelectCard[MAX_COUNT];		//是否选中


	//函数定义
public:
	//构造函数
	CShowCard();
	//析构函数
	virtual ~CShowCard();

	//申请结果
	virtual bool __cdecl ReqResult(const void * pBuffer,CGameClientEngine* pClient);
	//设置数据
	virtual void __cdecl SetLeftCardCount(BYTE cbRepertoryCardCount);
	//设置手牌
	virtual void __cdecl SetHandCardData(BYTE cbHandCardData[], BYTE cbCardCount, BYTE cbCurrentCard);
	//设置当前
	virtual void __cdecl SetCurrentCard(BYTE cbCurrentCard);

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK();
	//取消消息
	virtual void OnCancel();

	//绘画消息
public:
	//绘画函数
	afx_msg void OnPaint();
	//擦除背景
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
