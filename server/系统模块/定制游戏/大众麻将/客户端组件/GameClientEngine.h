#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "UserActionItem.h"

//////////////////////////////////////////////////////////////////////////

typedef CWHArray<tagUserAction *> CUserActionArray;				//������Ŀ
//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
	//�û�����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbActionMask;						//��Ҷ���
	BYTE							m_cbActionCard;						//������
	LONG							m_lSiceCount;						//ɫ����ֵ

	//��������
	TCHAR							m_szAccounts[GAME_PLAYER][LEN_NICKNAME];	//�������

	//�йܱ���
protected:
	bool							m_bStustee;							//�йܱ�־
	WORD							m_wTimeOutCount;					//��ʱ����

	//��������
protected:
	CUserActionArray				m_UserActionArray;					//������Ŀ
	CUserActionArray				m_UserActionStorage;				//������Ŀ

	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�

	BYTE							m_cbChearCardIndex[MAX_INDEX];		//�����˿�

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	WORD							m_wHeapHead;						//�ƶ�ͷ��
	WORD							m_wHeapTail;						//�ƶ�β��
	BYTE							m_cbHeapCardInfo[GAME_PLAYER][2];	//������Ϣ
#endif

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//����˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CHistoryScore					m_HistoryScore;						//��ʷ����

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
	//������Ϣ
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubOtherCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubAdminUpdate(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//������Ϣ
	bool OnGameMsg(bool bNextMsg);
	//��������
	void PlayCardSound(WORD wChairID,BYTE cbCardData);
	//��������
	void PlayActionSound(WORD wChairID,BYTE cbAction);
	//�����ж�
	bool VerdictOutCard(BYTE cbCardData);
	//�����˿�
	void SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard);
	//��ȡ������Ϣ
	BYTE GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] );

	//������������
protected:
	//��ʼ��Ҷ���
	bool BeginUserAction();
	//ֹͣ��Ҷ���
	bool StopUserAction();
	//�Ƴ���ǰ����
	bool RemoveCurrentAction();
	//�����
	tagUserAction *ActiveUserAction(enmActionKind enActionKind);
	//���ƶ���
	bool BeginActionStartCard();
	//���ƶ���
	bool BeginActionOutCard();
	//���ƶ���
	bool BeginActionSendCard();
	//��ʾ����
	bool BeginActionActionNotify();
	//�������
	bool BeginActionActionResult();
	//��ɫ��
	bool BeginActionSice();
	//��Ϸ����
	bool BeginActionGameEnd();
	//�������
	bool OnActionStartCard();
	//�������
	bool OnActionOutCard();
	//ץ�����
	bool OnActionSendCard();
	//��ʾ���
	bool OnActionActionNotify();
	//�������
	bool OnActionActionResult();
	//��ɫ�����
	bool OnActionSice();
	//��Ϸ����
	bool OnActionGameEnd();

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
	//�ڶ���ҡɫ����Ϣ
	LRESULT OnSiceTwo(WPARAM wParam, LPARAM lParam);
	//��Ҳ���
	LRESULT OnUserAction( WPARAM wParam, LPARAM lParam );
	//�������
	LRESULT OnUserActionFinish(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnChatMessage(WPARAM wParam, LPARAM lParam);
	//��Ϸ�ر�
	LRESULT OnMessageGameClose(WPARAM wParam, LPARAM lParam);
	//����Ա����
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif