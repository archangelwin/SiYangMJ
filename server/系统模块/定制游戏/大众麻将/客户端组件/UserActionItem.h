#ifndef USER_ACTION_ITEM_HEAD_FILE
#define USER_ACTION_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�ƶ�����
enum enmActionKind
{
	AK_NULL					= 0,							//
	AK_Sice,												//��ɫ��
	AK_StartCard,											//����
	AK_OutCard,												//����
	AK_SendCard,											//����
	AK_ActionNotify,										//��ʾ����
	AK_ActionResult,										//�������
	AK_GameEnd												//����
};

//////////////////////////////////////////////////////////////////////////

//��Ҷ���
struct tagUserAction
{
	enmActionKind					enActionKind;						//��������
	BOOL							bHandleStatus;						//����״̬

	//���캯��
	tagUserAction()
	{
		enActionKind = AK_NULL;
		ResetAction();
	}
	//��������
	virtual ~tagUserAction() {}
	//���ú���
	virtual VOID ResetAction()
	{
		bHandleStatus = FALSE;
	}
};

//��ɫ��
struct tagActionSice: public tagUserAction
{
	BYTE							cbSick[2];							//����

	//���캯��
	tagActionSice()
	{
		enActionKind = AK_Sice;
		ResetAction();
	}
	//���ú���
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		ZeroMemory(cbSick,sizeof(cbSick));
	}
};

//���ַ���
struct tagActionStartCard: public tagUserAction
{
	WORD							wChairId;							//�������
	BYTE							cbCardCount;						//������
	BYTE							cbCardData[4];						//����ֵ
	WORD							wHeapId;							//����λ��
	WORD							wHeapCardIndex;						//����λ��
	bool							bLastItem;							//�Ƿ����һ����

	//���캯��
	tagActionStartCard()
	{
		enActionKind = AK_StartCard;
		ResetAction();
	}
	//���ú���
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		wChairId = INVALID_CHAIR;
		cbCardCount = 0;
		ZeroMemory( cbCardData,sizeof(cbCardData) );
		wHeapId = INVALID_CHAIR;
		wHeapCardIndex = 0;
		bLastItem = false;
	}
};

//����
struct tagActionOutCard: public tagUserAction
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�

	//���캯��
	tagActionOutCard()
	{
		enActionKind = AK_OutCard;
		ResetAction();
	}
	//���ú���
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		wOutCardUser = INVALID_CHAIR;
		cbOutCardData = 0;
	}
};

//����
struct tagActionSendCard: public tagUserAction
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wSendCardUser;						//�����û�
	bool							bTail;								//�Ƿ��β������
	BYTE							cbLeftCardCount;					//ʣ����Ŀ

	//���캯��
	tagActionSendCard()
	{
		enActionKind = AK_SendCard;
		ResetAction();
	}
	//���ú���
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		cbCardData = 0;
		cbActionMask = WIK_NULL;
		wCurrentUser = INVALID_CHAIR;
		wSendCardUser = INVALID_CHAIR;
		bTail = false;
	}
};

//��ʾ����
struct tagActionActionNotify: public tagUserAction
{
	BYTE							cbActionCard;						//������
	BYTE							cbActionMask;						//������

	//���캯��
	tagActionActionNotify()
	{
		enActionKind = AK_ActionNotify;
		ResetAction();
	}
	//���ú���
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		cbActionCard = 0;
		cbActionMask = WIK_NULL;
	}
};

//�������
struct tagActionActionResult: public tagUserAction
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�

	//���캯��
	tagActionActionResult()
	{
		enActionKind = AK_ActionResult;
		ResetAction();
	}
	//���ú���
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		wOperateUser = INVALID_CHAIR;
		wProvideUser = INVALID_CHAIR;
		cbActionMask = WIK_NULL;
		cbOperateCode = WIK_NULL;
		ZeroMemory( cbOperateCard,sizeof(cbOperateCard) );
	}
};

//��Ϸ����
struct tagActionGameEnd: public tagUserAction
{
	LONGLONG						lGameTax;							//��Ϸ˰��
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbProvideCard;						//��Ӧ�˿�
	DWORD							dwChiHuKind[GAME_PLAYER];			//��������
	DWORD							dwChiHuRight[GAME_PLAYER][MAX_RIGHT_COUNT];		//��������
	WORD							wLeftUser;							//�������

	//������Ϣ
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
	LONGLONG						lGangScore[GAME_PLAYER];			//���Ƶ÷�

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿�����

	//���캯��
	tagActionGameEnd()
	{
		enActionKind = AK_GameEnd;
		ResetAction();
	}
	//���ú���
	VOID ResetAction()
	{
		bHandleStatus = FALSE;
		lGameTax = 0;
		wProvideUser = INVALID_CHAIR;
		cbProvideCard = 0;
		ZeroMemory( dwChiHuKind,sizeof(dwChiHuKind) );
		ZeroMemory( dwChiHuRight,sizeof(dwChiHuRight) );
		wLeftUser = INVALID_CHAIR;
		ZeroMemory( lGameScore,sizeof(lGameScore) );
		ZeroMemory( cbCardCount,sizeof(cbCardCount) );
		ZeroMemory( cbCardData,sizeof(cbCardData) );
	}
};

//////////////////////////////////////////////////////////////////////////

#endif