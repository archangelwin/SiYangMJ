#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "math.h"
#include "HistoryScore.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////////////
//Ч������
enum enEstimatKind
{
  EstimatKind_OutCard,      //����Ч��
  EstimatKind_GangCard,     //����Ч��
};
//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
  //��Ϸ����
protected:
  LONG              m_lSiceCount;             //���ӵ���
  WORD              m_wBankerUser;              //ׯ���û�
  LONGLONG            m_lGameScore[GAME_PLAYER];        //��Ϸ�÷�
  BYTE              m_cbCardIndex[GAME_PLAYER][MAX_INDEX];  //�û��˿�
  bool              m_bTrustee[GAME_PLAYER];        //�Ƿ��й�
  BYTE              m_cbListenStatus[GAME_PLAYER];      //����״̬
  BYTE              m_cbQuanFeng;             //Ȧ��
  BYTE              m_cbQuanCounter;            //Ȧ�����
  BYTE              m_cbLianZhuangCount;          //��ׯ����
  WORD              m_wReplaceUser;             //�����û�
  bool              m_bTianDiHu[GAME_PLAYER];       //���״̬
  bool              m_bTuoPai[GAME_PLAYER];         //����״̬
  BYTE              m_bMaiDi[GAME_PLAYER];          //���״̬ (0 û�� 1 ��һ 2 ��� other ����)
  bool              m_bHaveMaiDi[GAME_PLAYER];        //�Ƿ����

  //������Ϣ
protected:
  WORD              m_wOutCardUser;             //�����û�
  BYTE              m_cbOutCardData;            //�����˿�
  BYTE              m_cbOutCardCount;           //������Ŀ
  BYTE              m_cbDiscardCount[GAME_PLAYER];      //������Ŀ
  BYTE              m_cbDiscardCard[GAME_PLAYER][60];   //������¼

  //������Ϣ
protected:
  BYTE              m_cbSendCardData;           //�����˿�
  BYTE              m_cbSendCardCount;            //������Ŀ
  BYTE              m_cbLeftCardCount;            //ʣ����Ŀ
  BYTE              m_cbRepertoryCard[MAX_REPERTORY];   //����˿�
  BYTE              m_cbEndLeftCount;           //��ׯ����

  //��������
protected:
  WORD              m_wHeapHead;              //����ͷ��
  WORD              m_wHeapTail;              //����β��
  BYTE              m_cbHeapCardInfo[4][2];         //������Ϣ

  //�������
protected:
  BYTE              m_bMagicIndex;              //������ֵ
  WORD              m_wMagicPos;              //����λ
  BYTE              m_cbHeapMagic;              //�����λ  (��β������

  //���б���
protected:
  WORD              m_wResumeUser;              //��ԭ�û�
  WORD              m_wCurrentUser;             //��ǰ�û�
  WORD              m_wProvideUser;             //��Ӧ�û�
  BYTE              m_cbProvideCard;            //��Ӧ�˿�

  //״̬����
protected:
  bool              m_bSendStatus;              //����״̬
  bool              m_bGangStatus;              //����״̬
  bool              m_bEnjoinChiHu[GAME_PLAYER];      //��ֹ�Ժ�
  bool              m_bEnjoinChiPeng[GAME_PLAYER];      //��ֹ����

  //�û�״̬
public:
  bool              m_bResponse[GAME_PLAYER];       //��Ӧ��־
  BYTE              m_cbUserAction[GAME_PLAYER];      //�û�����
  BYTE              m_cbOperateCard[GAME_PLAYER][3];    //�����˿�
  BYTE              m_cbPerformAction[GAME_PLAYER];     //ִ�ж���

  //����˿�
protected:
  BYTE              m_cbWeaveItemCount[GAME_PLAYER];      //�����Ŀ
  tagWeaveItem          m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE]; //����˿�

  //������Ϣ
protected:
  BYTE              m_cbChiHuCard;              //�Ժ��˿�
  DWORD             m_dwChiHuKind[GAME_PLAYER];       //�Ժ����
  CChiHuRight           m_ChiHuRight[GAME_PLAYER];        //�Ժ�Ȩλ

  //�������
protected:
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  ITableFrame           * m_pITableFrame;           //��ܽӿ�
  const tagGameServiceOption    * m_pGameServiceOption;         //���ò���

  //�������
protected:
  HINSTANCE           m_hControlInst;
  IServerControl*         m_pServerControl;

  //���Ա���
protected:
  static const WORD       m_wPlayerCount;             //��Ϸ����

  //��������
public:
  //���캯��
  CTableFrameSink();
  //��������
  virtual ~CTableFrameSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID Release()
  {
    delete this;
  }
  //�ӿڲ�ѯ
  virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);
  //�����ӿ�
public:
  //���û���
  virtual void SetGameBaseScore(LONG lBaseScore) {};

  //����ӿ�
public:
  //��ʼ��
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //��λ����
  virtual VOID RepositionSink();

  //��ѯ�Ƿ�۷����
  virtual bool QueryBuckleServiceCharge(WORD wChairID)
  {
    return true;
  }

  //��Ϸ�¼�
public:
  //��Ϸ��ʼ
  virtual bool OnEventGameStart();
  //��Ϸ����
  virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //���ͳ���
  virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

  //�¼��ӿ�
public:
  //ʱ���¼�
  virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
  //��Ϸ��Ϣ
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����Ϣ
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

  //�û��¼�
public:
  //�û�����
  virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�����
  virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�ͬ��
  virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }
  //��ѯ�ӿ�
public:
  //��ѯ�޶�
  virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //���ٻ���
  virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //�����¼�
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return false;
  }
  //�����¼�
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
  {
    return false;
  }
  //��Ϸ�¼�
protected:
  //�û�����
  bool OnUserListenCard(WORD wChairID,BYTE cbListen);
  //�û�����
  bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
  //�û�����
  bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3]);
  //�û�����
  bool OnUserReplaceCard(WORD wChairID, BYTE cbCardData);
  //�û����
  bool OnUserChip(WORD wChairID, BYTE cbMaidi);

  //��������
protected:
  //���Ͳ���
  bool SendOperateNotify();
  //�ɷ��˿�
  bool DispatchCardData(WORD wCurrentUser,bool bTail=false);
  //��Ӧ�ж�
  bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
  //���
  BYTE CalScore(WORD wWinner, WORD wBanker, LONGLONG lScore[GAME_PLAYER]);
  //��ϵͳ���ƻ�ȡ��������
  BYTE GetMagicIndex(BYTE cbFanCard);
  //��ȡ��������
  BYTE GetDiscardCount(BYTE cbCardData);
  //Ȩλ����
  void FiltrateRight(WORD wWinner, CChiHuRight &chr);
  //״̬���
  bool IsAllState(bool bState[GAME_PLAYER], WORD wIgnorePos = INVALID_CHAIR, bool bCheckTrue = true);
  //�Ƿ�˫��
  bool IsBaoUser(WORD wUserFir, WORD wUserSec);
  //�����û�
  bool OnCheatCard(IServerUserItem * pIServerUserItem);
  //����������Ϣ
  void SendCheatCard();
};

//////////////////////////////////////////////////////////////////////////////////

#endif