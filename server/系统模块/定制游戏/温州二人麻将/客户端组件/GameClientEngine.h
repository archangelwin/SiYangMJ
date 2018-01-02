#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

#include "MoveCardItem.h"

//////////////////////////////////////////////////////////////////////////

typedef CWHArray<CMoveCardItem *>   MoveCardItemArray;      //������Ŀ

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
  //�û�����
protected:
  BYTE              m_bMagicIndex;            //������ֵ
  WORD              m_wMagicPos;            //����λ
  BYTE              m_cbHeapMagic;            //�����λ  (��β������
  WORD              m_wBankerUser;            //ׯ���û�
  BYTE              m_cbLianZhuangCount;            //��ׯ��Ŀ
  WORD              m_wCurrentUser;           //��ǰ�û�
  WORD              m_wReplaceUser;           //�����û�
  BYTE              m_cbActionMask;           //��Ҷ���
  BYTE              m_cbActionCard;           //������
  BYTE              m_cbListenStatus;         //����״̬,0ָδ����,1ָֻ����,2ָֻ����,3ָ���ƺ�����
  bool              m_bWillHearStatus;          //��������
  LONG              m_lSiceCount;           //ɫ����ֵ
  bool              m_bTuoPai;              //����״̬
  bool              m_bUserCheat[GAME_PLAYER];      //���ױ�־

  //��������
  TCHAR             m_szAccounts[GAME_PLAYER][LEN_NICKNAME];  //�������

  //��������
  MoveCardItemArray       m_MoveCardItemArray;        //�˿˻���

  //�йܱ���
protected:
  bool              m_bStustee;             //�йܱ�־
  WORD              m_wTimeOutCount;          //��ʱ����

  //�˿˱���
protected:
  BYTE              m_cbLeftCardCount;          //ʣ����Ŀ
  BYTE              m_cbCardIndex[MAX_INDEX];     //�����˿�
  WORD              m_wHeapHead;            //�ƶ�ͷ��
  WORD              m_wHeapTail;            //�ƶ�β��
  BYTE              m_cbHeapCardInfo[4][2];       //������Ϣ

  //�������
protected:
  BYTE              m_cbZiCardCount;          //�������Ƹ���
  BYTE              m_cbZiCardData[7];          //7������

  //�����˿�
protected:
  BYTE              m_cbCheatCardCount;         //�˿˸���
  BYTE              m_cbCheatCardData[MAX_COUNT];   //�����˿�


  //����˿�
protected:
  BYTE              m_cbWeaveCount[GAME_PLAYER];    //�����Ŀ
  tagWeaveItem          m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE]; //����˿�

  //�ؼ�����
protected:
  CGameLogic            m_GameLogic;            //��Ϸ�߼�
  CGameClientView         m_GameClientView;         //��Ϸ��ͼ


  //��������
public:
  //���캯��
  CGameClientEngine();
  //��������
  virtual ~CGameClientEngine();

  //ϵͳ�¼�
public:
  //��������
  virtual bool OnInitGameEngine();
  //���ú���
  virtual bool OnResetGameEngine();

  //ʱ���¼�
public:
  //ʱ��ɾ��
  virtual bool OnEventGameClockKill(WORD wChairID);
  //ʱ����Ϣ
  virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

  //��Ϸ�¼�
public:
  //�Թ���Ϣ
  virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
  //��Ϸ��Ϣ
  virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
  //������Ϣ
  virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

  //��Ϣ����
protected:
  //�û����
  bool OnSubChip(const void * pBuffer, WORD wDataSize);
  //��Ϸ��ʼ
  bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
  //�û�����
  bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
  //������Ϣ
  bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
  //������ʾ
  bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
  //�������
  bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
  //��Ϸ����
  bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
  //�û��й�
  bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
  //�û�����
  bool OnSubListen(const void * pBuffer,WORD wDataSize);
  //������Ϣ
  bool OnSubReplaceCard(const void * pBuffer, WORD wDataSize);
  //��׽��
  bool OnSubChipResult(const void * pBuffer, WORD wDataSize);
  //��������
  bool OnSubCheatCard(VOID * pData, WORD wDataSize);

  //��������
protected:
  //��������
  void PlayCardSound(WORD wChairID,BYTE cbCardData);
  //��������
  void PlayActionSound(WORD wChairID,BYTE cbAction);
  //У�����
  bool VerdictOutCard(BYTE cbPreCardData,BYTE cbAlreadyCardData[7],BYTE cbZiCardCount);
  //�Ƿ�ɳ�
  BOOL CheckOutCard(BYTE cbPreCardData,BYTE cbAlreadyCardData[7],BYTE cbZiCardCount);
  //�����˿�
  void SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard);
  //��ȡ������Ϣ
  BYTE GetSelectCardInfo(WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_COUNT]);
  //�˿˶���
  bool BeginMoveCard();
  //ֹͣ����
  void StopMoveCard();
  //���ƶ���
  bool BeginMoveOutCard(const CMoveCardItem *pMoveCardItem);
  //���ƶ���
  bool BeginMoveSendCard(const CMoveCardItem *pMoveCardItem);
  //��������
  bool BeginMoveReplaceCard(const CMoveCardItem *pMoveCardItem);
  //���ַ��ƶ���
  bool BeginMoveStartCard(const CMoveCardItem *pMoveCardItem);
  //���ƶ������
  bool OnMoveOutCardFinish(const COutCardItem *pOutCardItem);
  //���ƶ������
  bool OnMoveSendCardFinish(const CSendCardItem *pSendCardItem);
  //�����������
  bool OnMoveReplaceCardFinish(const CReplaceCardItem *pReplaceCardItem);
  //���ֶ������
  bool OnMoveStartCardFinish(const CStartCardItem *pStartCardItem);
  //�ƶ�λ��
  WORD SwitchToViewHeap(WORD wChairID);

  //��Ϣӳ��
protected:
  //ʱ����Ϣ
  VOID OnTimer(UINT nIDEvent);
  //��ʼ��Ϣ
  LRESULT OnStart(WPARAM wParam, LPARAM lParam);
  //���Ʋ���
  LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
  //�˿˲���
  LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
  //�Ϲܿ���
  LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
  //�����Ϣ
  LRESULT OnChip(WPARAM wParam, LPARAM lParam);
  //�ڶ���ҡɫ����Ϣ
  LRESULT OnSiceTwo(WPARAM wParam, LPARAM lParam);
  //ҡɫ�ӽ�����Ϣ
  LRESULT OnSiceFinish(WPARAM wParam, LPARAM lParam);
  //��Ҳ���
  LRESULT OnUserAction(WPARAM wParam, LPARAM lParam);
  //���������Ϣ
  LRESULT OnMoveCardFinish(WPARAM wParam, LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif