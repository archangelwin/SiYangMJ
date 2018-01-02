#pragma once

#include "Stdafx.h"
#include "ChessBorad.h"
#include "ChessManual.h"
#include "GameScoreWnd.h"
#include "DlgUserRequest.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_REGRET					(WM_USER+101)						//������Ϣ
#define IDM_PEACE					(WM_USER+102)						//�����Ϣ
#define IDM_GIVEUP					(WM_USER+103)						//������Ϣ
#define IDM_TRADE					(WM_USER+104)						//�Ի���Ϣ
#define IDM_SHOW_ID					(WM_USER+105)						//������Ϣ
#define IDM_PRESERVE				(WM_USER+106)						//������Ϣ
#define IDM_STUDY					(WM_USER+107)						//�о���Ϣ
#define IDM_REGRET_ANSWER			(WM_USER+108)						//����Ӧ��
#define IDM_PEACE_ANSWER			(WM_USER+109)						//���Ӧ��

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��Ԫ����
	friend class CGameClientEngine;

	//��������
protected:
	WORD							m_wBlackUser;						//�����û�
	WORD							m_wGameClock;						//��ʱʱ��
	WORD							m_wLeftClock[2];					//ʣ��ʱ��

	//λ����Ϣ
protected:
	CSize							m_UserInfoSize;						//λ����Ϣ
	CSize							m_ButtonBackSize;					//λ����Ϣ
	CPoint							m_ChessFlagPoint[2];				//��־λ��
	int								m_nYBorder ;						//
	int								m_nXBorder ;						//
	//�������
protected:
	CBitImageEx						m_ImageBack;						//����ͼ��
	CBitImageEx						m_ImageFlag;						//��־��ͼ
	CBitImageEx						m_ImageClock;						//ʱ�ӱ�־
	CBitImageEx						m_ImageUserInfo;					//�û���Ϣ
	CBitImageEx						m_ImageDecorate;					//װ��λͼ
	CBitImageEx						m_ImageButtonBack;					//��ť��ͼ

	//�ؼ�����
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btRegret;							//���尴ť
	CSkinButton						m_btPeace;							//��Ͱ�ť
	CSkinButton						m_btGiveUp;							//���䰴ť
	CSkinButton						m_btShowID;							//������ť
	//CSkinButton						m_btTrade;							//�Ի���ť
	CSkinButton						m_btPreserve;						//���水ť
	CSkinButton						m_btStudy;							//�о���ť
	CChessBorad						m_ChessBorad;						//��Ϸ����
	CChessManual					m_ChessManual;						//���״���
	CGameScoreWnd					m_GameScoreWnd;						//�ɼ�����
	CDlgUserRequest					m_PeaceRequest;						//��������
	CDlgUserRequest					m_RegretRequest;					//��������

#ifdef VIDEO_GAME

	//��Ƶ���
private:
	CVideoServiceManager			m_VedioServiceManager;				//��Ƶ����
	CVideoServiceControl			m_DlgVedioService[GAME_PLAYER];		//��Ƶ���

#endif


	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();


	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }


	//���ܺ���
public:
	//��������
	void ShowPeaceRequest();
	//��������
	void ShowRegretRequest();
	//���ú���
	void SetBlackUser(WORD wBlackUser);
	//����ʱ��
	void SetGameClock(WORD wGameClock);
	//����ʱ��
	void SetUserClock(WORD wViewChairID, WORD wLeftClock);
	//������ͼ
	void RefreshGameView();
	//������ͼ
	void RefreshGameView(CRect rect);
	//�ڲ����� 
private:
	//����״̬
	void UpdateUserInfoArea();
	//�û�ʱ��
	void DrawUserClock(CDC * pDC, int nXPos, int nYPos, WORD wClockCount);

	//�ؼ���Ϣ
protected:
	//�������
	LRESULT OnHitChessBorad(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ��
protected:
	//��ʼ��ť
	afx_msg void OnStart();
	//���尴ť
	afx_msg void OnRegret();
	//��Ͱ�ť
	afx_msg void OnPeace();
	//���䰴ť
	afx_msg void OnGiveUp();
	//������ť
	afx_msg void OnShowID();
	//�Ի���ť
	afx_msg void OnTrade();
	//���水ť
	afx_msg void OnPreserve();
	//�о���ť
	afx_msg void OnStudy();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
