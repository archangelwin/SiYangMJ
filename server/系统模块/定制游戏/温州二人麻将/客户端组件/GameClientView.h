#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "DrawSiceWnd.h"
#include "DlgListen.h"
#include "GameScore.h"
#include "MoveCardItem.h"
//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START         (WM_USER+100)           //��ʼ��Ϣ
#define IDM_OUT_CARD        (WM_USER+101)           //������Ϣ
#define IDM_TRUSTEE_CONTROL     (WM_USER+102)           //�йܿ���
#define IDM_CHIP          (WM_USER+103)           //��׿���
#define IDM_CARD_OPERATE      (WM_USER+106)           //��Ҳ�����
#define IDM_MOVE_CARD_FINISH    (WM_USER+107)           //�˿˶���������Ϣ

//////////////////////////////////////////////////////////////////////////

//��ͼλ��
#define MYSELF_VIEW_ID        1                 //��ͼλ��

//////////////////////////////////////////////////////////////////////////

struct tagMoveCardItem
{
  CPoint      ptFrom;                       //��ʼλ��
  CPoint      ptTo;                       //����λ��
  WORD      wViewChairId;                   //���ƻ�����ͼλ��
  BYTE      cbCardData;                     //���ƻ�������
  BYTE      cbCardCount;                    //����
};


//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
  //��־����
protected:
  bool              m_bOutCard;             //���Ʊ�־
  bool              m_bWaitOther;           //�ȴ���־
  bool              m_bHuangZhuang;           //��ׯ��־
  bool              m_bTrustee[GAME_PLAYER];      //�Ƿ��й�
  bool              m_bListenStatus[GAME_PLAYER];   //���Ʊ�־
  bool              m_bMaiDiState;            //��ױ�־

  bool              m_bShowHeapCard;

  //��Ϸ����
protected:
  LONGLONG            m_lCellScore;           //��������
  WORD              m_wBankerUser;            //ׯ���û�
  BYTE              m_cbLianZhuang;           //��ׯ��Ŀ
  WORD              m_wCurrentUser;           //��ǰ�û�
  BYTE              m_cbFengQuan;           //��Ȧ
  BYTE              m_bMagicCard;           //������ֵ
  BYTE              m_cbHuaCardCount[GAME_PLAYER];    //����ͳ��
  BYTE              m_cbHuaCardData[GAME_PLAYER][8];  //������Ϣ
  BYTE              m_bMaiDi[GAME_PLAYER];        //��׷���

  //��������
protected:
  bool              m_bBombEffect;            //����Ч��
  BYTE              m_cbBombFrameIndex;         //֡������

  //����Ч��
  WORD              m_wDiscUser;            //�����û�
  BYTE              m_cbDiscFrameIndex;         //֡������

  //���ƻ���ƶ�������
protected:
  bool              m_bEnableAnimate;         //�Ƿ���ʾ���ƶ���
  bool              m_bCardMoving;            //�Ƿ��ڶ���
  int               m_nXStep;             //X����
  int               m_nYStep;             //Y����
  int               m_nStepCount;           //�ƶ�����
  enDirection           m_enMoveDirection;          //�ƶ�����
  CRect             m_rcCardMove;           //�滭����
  tagMoveCardItem         m_MoveCardItem;           //������Ŀ

  //�û�״̬
protected:
  BYTE              m_cbCardData;           //�����˿�
  WORD              m_wOutCardUser;           //�����û�
  BYTE              m_cbUserAction[GAME_PLAYER];    //�û�����

  //λ�ñ���
protected:
  CPoint              m_UserFlagPos[GAME_PLAYER];     //��־λ��
  CPoint              m_UserListenPos[GAME_PLAYER];   //��־λ��
  CPoint              m_PointTrustee[GAME_PLAYER];    //�й�λ��
  CPoint              m_PointHuaCard[GAME_PLAYER];    //����λ��

  //λͼ����
protected:
  CBitImageEx           m_ImageBack;            //����ͼ��
  CBitImageEx           m_ImageCenter;            //LOGOͼ


  CPngImageEx           m_ImageOutCard;           //������ʾ




  CPngImageEx           m_ImageTrustee;           //�йܱ�־

  CPngImageEx           m_PngListenFlag;          //���Ʊ�־
  CPngImageEx           m_ImageActionAni;         //���ƶ�����Դ
  CPngImageEx           m_ImageDisc;            //����Ч��
  CPngImageEx           m_ImageArrow;           //��ʱ����ͷ



  CBitImageEx           ImageUserReady;           //�û�׼��
  CBitImageEx           ImageTimeBack;            //ʱ�ӱ���
  CBitImageEx           ImageTimeNumber;          //ʱ������

  CBitImageEx           m_ImageActionBack;          //��������
  CBitImageEx           m_PngHuangZhuang;         //��ׯ��־
  CBitImageEx           m_ImageWait;            //�ȴ���ʾ
  CBitImageEx           m_ImageMagicCardBack;       //���񱳾�
  CBitImageEx           m_ImageMaiDiFlag;         //��ױ�־

  CBitImageEx           m_ImageUserFlag;          //�û���־



  //�˿˿ؼ�
public:
  CHeapCard           m_HeapCard[4];                //�����˿�
  CUserCard           m_UserCard[GAME_PLAYER-1];          //�û��˿�
  CTableCard            m_TableCard[GAME_PLAYER];         //�����˿�
  CWeaveCard            m_WeaveCard[GAME_PLAYER][MAX_WEAVE];    //����˿�
  CDiscardCard          m_DiscardCard[GAME_PLAYER];         //�����˿�
  CCardControl          m_HandCardControl;          //�����˿�

  //�ؼ�����
public:
  CSkinButton           m_btStart;              //��ʼ��ť
  //CSkinButton           m_btStusteeControl;         //�Ϲܿ���

  CSkinButton           m_btMaiDi;            //��װ�ť
  CSkinButton           m_btDingDi;           //���װ�ť
  CSkinButton           m_btBuMai;            //����ť
  CSkinButton           m_btBuDing;           //������ť

  CControlWnd           m_ControlWnd;           //���ƴ���
  CDrawSiceWnd          m_DrawSiceWnd;            //ɫ�ӿؼ�
  CDlgListen            m_DlgListen;            //���ƶԻ���
  CGameScore            m_GameScore;            //�ɼ���


  //��������
public:
  //���캯��
  CGameClientView();
  //��������
  virtual ~CGameClientView();

  //���غ���
private:
  //���ý���
  virtual VOID ResetGameView();
  //�����ؼ�
  virtual VOID RectifyControl(INT nWidth, INT nHeight);
  //�滭����
  virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
  //�����
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  //WIN7֧��
  virtual bool RealizeWIN7()
  {
    return true;
  }
  //���ܺ���
public:
  //��������
  void SetCellScore(LONGLONG lCellScore);
  //ׯ���û�
  void SetBankerUser(WORD wBankerUser,BYTE cbLianZhuang);
  //��ׯ����
  void SetHuangZhuang(bool bHuangZhuang);
  //״̬��־
  void SetStatusFlag(bool bOutCard, bool bWaitOther);
  //������Ϣ
  void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
  //������Ϣ
  void SetUserAction(WORD wViewChairID, BYTE bUserAction);
  //���ö���
  bool SetBombEffect(bool bBombEffect);
  //�����û�
  void SetDiscUser(WORD wDiscUser);
  //��ʱ���
  void SetCurrentUser(WORD wCurrentUser);
  //�����й�
  void SetTrustee(WORD wTrusteeUser,bool bTrustee);
  //���Ʊ�־
  void SetUserListenStatus(WORD wViewChairID, bool bListenStatus);
  //���÷�Ȧ
  void SetFengQuan(BYTE cbFengQuan);
  //���ò���
  void SetMagicCard(BYTE bMagicCard);
  //�������
  void SetMaiDiState(bool bMaiDi);
  //���ö��׷���
  void SetMaiDiScore(WORD wViewChairID,BYTE bMaiDi);
  //ֹͣ���ƻ��ƶ���
  bool StopMoveCard();
  //�Ƿ񶯻�������
  bool IsMovingCard()
  {
    return m_bCardMoving;
  }
  //������
  void EnableAnimate(bool bAnimate);
  //�Ƿ�������
  bool IsEnableAnimate()
  {
    return m_bEnableAnimate;
  }
  //�˿˶���
  void OnMoveCardItem(const tagMoveCardItem &MoveCardItem);
  //���û���
  void SetHuaCard(WORD wChair,BYTE bHuaCard, bool bClear);
  //����β��
  CPoint GetHuaCardTailPos(WORD wChair);

  void SetShowHeapCard(bool bshow);

  //��������
protected:
  //��������
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat=DT_LEFT);
  //�滭ʱ��
  void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
  //�滭׼��
  void DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos);
  //�滭�˿˶���
  void DrawMoveCardItem(CDC *pDC);

  //��Ϣӳ��
protected:
  //��׿���
  afx_msg void OnChip(UINT nCtrlID);
  //��������
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //�����Ϣ
  afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
  //�����Ϣ
  afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
  //��ʱ��
  afx_msg void OnTimer(UINT nIDEvent);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif