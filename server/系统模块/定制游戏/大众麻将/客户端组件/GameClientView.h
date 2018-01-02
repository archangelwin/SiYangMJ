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
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_TRUSTEE_CONTROL			(WM_USER+102)						//�йܿ���
#define IDM_CARD_OPERATE			(WM_USER+106)						//��Ҳ�����
#define IDM_USER_ACTION_FINISH		(WM_USER+107)						//��������
#define IDM_CHAT_MESSAGE			WM_USER+208							//������Ϣ
#define IDM_USER_ACTION				(WM_USER+108)						//�û�����
#define IDM_GAME_CLOSE				(WM_USER+109)						//��Ϸ�ر�

//////////////////////////////////////////////////////////////////////////

//��ͼλ��
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#define MYSELF_VIEW_ID				1									//��ͼλ��
#else
#define MYSELF_VIEW_ID				2									//��ͼλ��
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
	CPoint			ptFrom;												//��ʼλ��
	CPoint			ptTo;												//����λ��
	WORD			wViewChairId;										//���ƻ�����ͼλ��
	BYTE			cbCardData;											//���ƻ�������
	BYTE			cbCardCount;										//����
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewD3D
{
	//��־����
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	bool							m_bHuangZhuang;						//��ׯ��־
	bool							m_bTrustee[GAME_PLAYER];			//�Ƿ��й�
	bool							m_bListenStatus[GAME_PLAYER];		//���Ʊ�־
	bool							m_bShowChatMessage;					//��ʾ����
	CBrush							m_brush;							//������ˢ
	INT								m_nHeight;							//���ڸ߶�
	INT								m_nWidth;							//���ڿ��	

	//��Ϸ����
protected:
	LONGLONG						m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbFengQuan;						//��Ȧ
	BYTE							m_cbLeftCardCount;					//ʣ���˿�
	BYTE							m_cbLianZhuangCount;				//��ׯ����

	//��������
protected:
	bool							m_bBombEffect;						//����Ч��
	BYTE							m_cbBombFrameIndex;					//֡������

	bool							m_cbSick[6];						//����
	BYTE							m_cbSicFrameIndex[6];				//֡������

	//����Ч��
	WORD							m_wDiscUser;						//�����û�
	BYTE							m_cbDiscFrameIndex;					//֡������	

	//���ƻ���ƶ�������
protected:
	bool							m_bEnableAnimate;					//�Ƿ���ʾ���ƶ���
	bool							m_bCardMoving;						//�Ƿ��ڶ���
	int								m_nXStep;							//X����
	int								m_nYStep;							//Y����
	int								m_nStepCount;						//�ƶ�����
	enDirection						m_enMoveDirection;					//�ƶ�����
	CRect							m_rcCardMove;						//�滭����
	tagMoveCardItem					m_MoveCardItem;						//������Ŀ

	//��ʷ����
public:
	bool							m_bShowScore;						//�ɼ���־
	bool							m_bShowControl;						//��ʾ�����ܴ���
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//�ɼ���Ϣ

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbUserAction[GAME_PLAYER];		//�û�����

	//λ�ñ���
protected:
	CPoint							m_UserFlagPos[GAME_PLAYER];			//��־λ��
	CPoint							m_UserListenPos[GAME_PLAYER];		//��־λ��
	CPoint							m_PointTrustee[GAME_PLAYER];		//�й�λ��

	//������ʾ
protected:
	//����
	CD3DFontEx						m_FontName;							//��������

	//λͼ����
protected:
	CD3DTexture						m_ImageCenter;						//LOGOͼ	
	CD3DTexture						m_PngFrameWindow;					//��Ϸ����
	CD3DTexture						m_PngTableChair;					//������Դ
	CD3DTexture						m_ImageHuangZhuang;					//��ׯ��־
	
	CD3DTexture						m_ImageWait;						//�ȴ���ʾ
	CD3DTexture						m_ImageOutCard;						//������ʾ
	CD3DTexture						m_ImageUserFlag;					//�û���־
	CD3DTexture						m_ImageActionBack;					//��������
	CD3DTexture						m_ImageTrustee;						//�йܱ�־
	CD3DTexture						m_PngFengQuan;						//Ȧ���־
	CD3DTexture						m_PngListenFlag;					//���Ʊ�־
	CD3DTexture						m_ImageActionAni;					//���ƶ�����Դ
	CD3DTexture						m_ImageDisc;						//����Ч��
	CD3DTexture						m_ImageArrow;						//��ʱ����ͷ	
	CD3DTexture						m_PngLeftBoy;						//����ͷ��
	CD3DTexture						m_PngLeftGirl;						//����ͷ��
	CD3DTexture						m_PngTopBoy;						//����ͷ��
	CD3DTexture						m_PngTopGirl;						//����ͷ��
	CD3DTexture						m_PngRightBoy;						//����ͷ��
	CD3DTexture						m_PngRightGirl;						//����ͷ��
	CD3DTexture						m_PngUserFramePlayer;				//��Ϣ���
	CD3DTexture						m_PngHeapBack;						//�ƶѱ���
	CD3DTexture						m_PngHeapCount;						//��Ŀ����
	CD3DTexture						m_PngHeapNumber;					//����ͼƬ
	CD3DTexture						m_PngScoreInfo;						//��Ϣ��ʾ
	CD3DTexture						m_PngLZhuangNummber;				//��ׯ����
	CD3DTexture						m_PngMyScoreNum;					//�Լ��������
	CD3DTexture						m_PngChatBack;						//���챳��
	CD3DTexture						m_PngGameLogo;						//��ϷLOGO
	CD3DTexture						m_PngHistoryScore;					//���ֱ���
	CD3DTexture						m_ImageSice1;						//����
	CD3DTexture						m_ImageSice2;						//����
	CD3DTexture						m_ImageSice3;						//����
	CD3DTexture						m_ImageSice4;						//����
	CD3DTexture						m_ImageSice5;						//����
	CD3DTexture						m_ImageSice6;						//����
	CD3DTexture						m_ImageControlBack;					//�����ܰ�ť��ͼ

	CD3DTexture		    			m_ImageUserReady;
	CD3DTexture						m_ImageTimeBack;
	CD3DTexture						m_ImageTimeNumber;

	//�˿˿ؼ�
public:
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	CHeapCard						m_HeapCard[GAME_PLAYER];			//�����˿�
#endif
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
	CUserCard						m_UserCard[1];						//�û��˿�
#else
	CUserCard						m_UserCard[3];						//�û��˿�
#endif
	CTableCard						m_TableCard[GAME_PLAYER];					//�����˿�
	CWeaveCard						m_WeaveCard[GAME_PLAYER][MAX_WEAVE];		//����˿�
	CDiscardCard					m_DiscardCard[GAME_PLAYER];					//�����˿�
	CCardControl					m_HandCardControl;					//�����˿�
	//CEdit							m_ChatInput;						//����ؼ�
	//CChatEdit						m_ChatDisplay;						//��ʾ�ؼ�
	CGameScore						m_GameScore;						//�ɼ���

	//���׹���
	CCardControl					m_HandCardCheat;					//�����˿�
	bool							m_bChearUser;						//�����û�
	CButton							m_btOpenAdmin;						//ϵͳ����
	CShowCard*						m_pClientControlDlg;				//���ƴ���

	//�ؼ�����
public:
	CVirtualButton					m_btStart;							//��ʼ��ť
	//CSkinButtonEx					m_btSendMessage;					//������Ϣ
	CVirtualButton					m_btStusteeControl;					//�Ϲܿ���
	//CSkinButtonEx					m_btHistoryChat;					//�鿴����
	CVirtualButton					m_btHistoryScore;					//�鿴����
	CVirtualButton					m_btPeng;							//���ư�ť
	CVirtualButton					m_btGang;							//���ư�ť
	CVirtualButton					m_btChiHu;							//���ư�ť
	CVirtualButton					m_btGiveUp;							//���ư�ť
	CVirtualButton					m_btChi;							//�ؼ���ť	
	CVirtualButton					m_btCloseScore;						//�رհ�ť
	CVirtualButton					m_btContiueGame;					//������Ϸ

	//���Ʊ���
public:
	//D3D 
	CD3DDevice *					m_pD3DDevice;						//D3D ָ��

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���غ���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, int nWidth, int nHeight);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
protected:
	//��������
	virtual VOID CartoonMovie();
	//�����豸
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//��������
	void SetCellScore(LONGLONG lCellScore);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//��ׯ����
	void SetHuangZhuang(bool bHuangZhuang);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//���û���
	VOID SetHistoryScore(WORD wChairID, tagHistoryScore * pHistoryScore);
	//���ö���
	bool SetBombEffect(bool bBombEffect);
	//�����û�
	void SetDiscUser(WORD wDiscUser);
	//��ʱ���
	void SetCurrentUser(WORD wCurrentUser);
	//�����й�
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);
	//ֹͣ���ƻ��ƶ���
	bool StopMoveCard();
	//�Ƿ񶯻�������
	bool IsMovingCard() { return m_bCardMoving; }
	//������
	void EnableAnimate( bool bAnimate );
	//�Ƿ�������
	bool IsEnableAnimate() { return m_bEnableAnimate; }
	//�˿˶���
	void OnMoveCardItem( const tagMoveCardItem &MoveCardItem );
	//����ʣ��
	VOID SetLeftCardCount(BYTE cbLeftCardCount);
	//������ׯ
	void SetLZhuangCount(BYTE cbCount);
	//ϵͳ��Ϣ
	void InsertSystemMessage(LPCTSTR pszString);
	//������Ϣ
	void InsertChatMessage(LPCTSTR pszString);
	//������Ϣ
	void InsertNormalMessage(LPCTSTR pszString);
	//������Ϣ
	void InsertAllChatMessage(LPCTSTR pszString, int nMessageType);
	//��Ϣ����
	void InsertMessage(LPCTSTR pszString,COLORREF color=RGB(193,167,108));
	//���תȫ��
	CString StringHalfToFull(CString strContent);
	//ȫ��ת���
	CString StringFullToHalf(CString strContent);
	//�滭����
	void DrawSick(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//��������
	bool SetSick(bool bSickEffect,BYTE bSickIndex);
	//����״̬
	VOID SetControlInfo(bool bShow,BYTE cbAcitonMask);	


	//��������
protected:
	//�滭ʱ��
	void DrawUserTimerEx(CD3DDevice * pD3DDevice, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//�滭׼��
	void DrawUserReadyEx(CD3DDevice * pD3DDevice, int nXPos, int nYPos);
	//�滭�˿˶���
	void DrawMoveCardItem( CD3DDevice * pD3DDevice );
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_RIGHT);
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_RIGHT);
	//��������
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos, UINT nFormat);
	//��������
	VOID DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect, UINT nFormat);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	afx_msg void OnSendMessage();
	//�ؼ���ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//�鿴����
	afx_msg void OnHideChatMessage();
	//���ư�ť
	afx_msg VOID OnBnClickedAdmin();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif