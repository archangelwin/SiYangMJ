#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
 
//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
	bool							m_bRestrict;						//�Ƿ����
	bool							m_bTradeUser;						//�Ƿ�Ի�
	bool							m_bDoubleChess;						//�Ƿ�˫��

	//ʱ�����
public:
	DWORD							m_dwLastClock;						//�ϴ�ʱ��
	WORD							m_wLeftClock[2];					//ʣ��ʱ��

	//�û�״̬
protected:
	WORD							m_wBlackUser;						//�������
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_cbBegStatus[2];					//����״̬
	BYTE							m_cbPeaceCount[2];					//��ʹ���
	BYTE							m_cbRegretCount[2];					//�������

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	//static const enStartMode		m_GameStartMode;					//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();


	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//��Ϣ�ӿ�
public:
	//��Ϸ״̬
	virtual bool IsUserPlaying(WORD wChairID);


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
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�û�����
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }

	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }

	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser){ return true; }
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser){ return true; }
	//�û�ͬ��
	virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserPlaceChess(WORD wChairID, BYTE cbXPos, BYTE cbYPos);
	//��������
	bool OnUserRegretReq(WORD wChairID);
	//����Ӧ��
	bool OnUserRegretAnswer(WORD wChairID, BYTE cbApprove);
	//�������
	bool OnUserPeaceReq(WORD wChairID);
	//���Ӧ��
	bool OnUserPeaceAnswer(WORD wChairID, BYTE cbApprove);
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//�Ի�����
	bool OnUserTradeReq(WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif
