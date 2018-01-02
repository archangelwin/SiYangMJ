#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "UserActionItem.h"

//////////////////////////////////////////////////////////////////////////
//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			201									//������ʱ��
#define IDI_REPLACE_CARD			301									//���ƶ�ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_OPERATE_CARD			20									//������ʱ��		
#define TIME_REPLACE_CARD			1000								//���ƶ�ʱ��

#define MAX_TIME_OUT				3									//���ʱ����
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)	
	ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
	ON_MESSAGE(IDM_USER_ACTION_FINISH,OnUserActionFinish)
	ON_MESSAGE(IDM_CHAT_MESSAGE,OnChatMessage)
	ON_MESSAGE(IDM_GAME_CLOSE,OnMessageGameClose)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_lSiceCount = 0;

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbChearCardIndex,sizeof(m_cbChearCardIndex));
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory( m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );
#endif

}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��������
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	////TEST+
	//m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	////TEST+	
	
	return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_lSiceCount = 0;

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_cbChearCardIndex,sizeof(m_cbChearCardIndex));
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
#endif
	return true;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			//if( m_bStustee && nElapse < TIME_START_GAME )
			//{
			//	OnStart(0,0);
			//	return true;
			//}
			if (nElapse==0)
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}
			if (nElapse<=5)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//�Զ�����
			bool bAutoOutCard=false;
			if ((bAutoOutCard==true)&&(m_GameClientView.m_bShowControl))
				bAutoOutCard=false;
			if((bAutoOutCard==false)&&(m_bStustee==true))
			{
				bAutoOutCard=true;
			}

			//��ʱ�ж�
			if ( (IsLookonMode()==false)&&
				( nElapse==0 || bAutoOutCard ) )
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID)
				{
					//���δ����,��δ�й�,���ۼӳ�ʱ����
					if( m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
					{
						m_wTimeOutCount = 0;
						OnStusteeControl(TRUE,0);
#ifndef DEBUG
						if(m_pIStringMessage!=NULL)
							m_pIStringMessage->InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
#endif
					}

					if (m_wCurrentUser==wMeChairID)
					{
						//��ȡ�˿�
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();

						//����ڲ���ģʽ��
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						//����Ч��
						if (VerdictOutCard(cbCardData)==false)
						{
							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								//����Ч��
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//���ñ���
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
						}

						//���ƶ���
						ASSERT( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)] > 0 );
						OnOutCard(cbCardData,cbCardData);
					}
					else 
					{
						//����ڲ���ģʽ��
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						OnUserAction(WIK_NULL,0);
					}
				}

				return true;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}
	return false;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
// 	m_GameClientView.InvalidGameView(0,0,0,0);
	return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pData,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pData,wDataSize);
		}
	case SUB_S_AMDIN_COMMAND:	//��������
		{
			return OnSubAdminControl(pData,wDataSize);
		}
	case SUB_S_OTHER_CARD:		//��������
		{
			return OnSubOtherCard(pData,wDataSize);
		}
	case SUB_S_UPDATE_CARD:		//��������
		{
			return OnSubAdminUpdate(pData,wDataSize);
		}
	}

	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;



			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				if(GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
					//m_GameClientView.m_btStart.SetFocus();
					SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
				}

				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//��Ϸ����
			if (CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}
			m_GameClientView.m_btHistoryScore.ShowWindow(SW_SHOW);

			//����Ч��
			m_GameClientView.SetDiscUser(INVALID_CHAIR);

// 			//���½���
// 			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//��������
			WORD wMeChairID=GetMeChairID();
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
			WORD wViewChairID[GAME_PLAYER]={0,0};
#else
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
#endif
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//���ñ���
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			m_wHeapHead = pStatusPlay->wHeapHead;
			m_wHeapTail = pStatusPlay->wHeapTail;
#endif

			//�û�����
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				IClientUserItem *pUserItem = GetTableUserItem(i);
				ASSERT( pUserItem != NULL );
				if( pUserItem == NULL ) 
				{
					m_szAccounts[i][0] = 0;
					continue;
				}
				lstrcpy( m_szAccounts[i],pUserItem->GetNickName() );
			}
			m_GameClientView.SetLeftCardCount(m_cbLeftCardCount);

			//�Թ�
			if( IsLookonMode()==true )
				m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());

			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
			}

			if( pStatusPlay->bTrustee[wMeChairID] )
			{
				OnStusteeControl(0,0);
			}

			m_wTimeOutCount=0;
			if(pStatusPlay->bTrustee[wMeChairID])
				m_GameClientView.m_btStusteeControl.SetButtonImage(&m_GameClientView.m_D3DDevice,TEXT("BT_STOP_TRUSTEE"),TEXT("PNG"),AfxGetInstanceHandle());	

			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(&m_GameClientView.m_D3DDevice,TEXT("BT_START_TRUSTEE"),TEXT("PNG"),AfxGetInstanceHandle());	

			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wOperateViewID = SwitchViewChairID(i);
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(m_WeaveItemArray[i][j].cbCardData,cbWeaveCardCount);
					if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i)) 
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos((BYTE)wProviderViewID);

				}
			}

			//�û��˿�
			if (m_wCurrentUser==wMeChairID&&
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pStatusPlay->cbSendCardData)] > 0 )
			{
				SetHandCardControl(m_cbCardIndex,pStatusPlay->cbSendCardData);
			}
			else
			{
				SetHandCardControl(m_cbCardIndex,0x00);
			}

			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��˿�
				if (i!=wMeChairID)
				{
					BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[i]*3-1;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//�����˿�
				m_GameClientView.m_DiscardCard[wViewChairID[i]].SetCardData(pStatusPlay->cbDiscardCard[i],pStatusPlay->cbDiscardCount[i]);
			}

			//����Ч��
			if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
			{
				m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));
				m_GameClientView.SetOutCardInfo( wViewChairID[pStatusPlay->wOutCardUser],pStatusPlay->cbOutCardData );
				m_GameClientView.m_DiscardCard[wViewChairID[pStatusPlay->wOutCardUser]].AddCardItem( pStatusPlay->cbOutCardData );
			}

			//��������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetHeapCardInfo(pStatusPlay->cbHeapCardInfo[i][0],pStatusPlay->cbHeapCardInfo[i][1]);
#endif
			}

			//��������
			if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
			{
				//��ȡ����
				m_cbActionMask=pStatusPlay->cbActionMask;
				m_cbActionCard=pStatusPlay->cbActionCard;

				//���ý���
				if (m_wCurrentUser==INVALID_CHAIR)
					SetGameClock(wMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				if (IsLookonMode()==false) 
					m_GameClientView.SetControlInfo(true,m_cbActionMask);
			}

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}
			m_GameClientView.m_btHistoryScore.ShowWindow(SW_SHOW);

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR)
			{
				//����ʱ��
				WORD wTimeCount=TIME_OPERATE_CARD;

				//����ʱ��
				SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
			}

// 			//���½���
// 			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	}
	return false;
}


//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//�û�����
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IClientUserItem *pUserItem= GetTableUserItem(i);
		ASSERT( pUserItem != NULL );
		if( pUserItem == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserItem->GetNickName() );
	}

	//����״̬
	SetGameStatus(GAME_SCENE_PLAY);

	//���ñ���
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY/*-GAME_PLAYER*(MAX_COUNT-1)-1*/;
	m_cbActionMask = pGameStart->cbUserAction;	
	m_GameClientView.SetLZhuangCount(pGameStart->cbLianZhuangCount);
	m_GameClientView.SetLeftCardCount(m_cbLeftCardCount);

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//���ö���ͷβ
	m_wHeapHead = pGameStart->wHeapHead;
	m_wHeapTail = pGameStart->wHeapTail;
	CopyMemory( m_cbHeapCardInfo,pGameStart->cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );
#endif

	//�Թ۽���
	if (IsLookonMode()==true)
	{
		//����˿�
		ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
		ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

		m_GameClientView.m_GameScore.RestorationData();
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);

		//���ý���
		m_GameClientView.SetDiscUser(INVALID_CHAIR);
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

		//�˿�����
		m_GameClientView.m_UserCard[0].SetCardData(0,false);
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
		m_GameClientView.m_UserCard[1].SetCardData(0,false);
		m_GameClientView.m_UserCard[2].SetCardData(0,false);
#endif
		m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

		//�˿�����
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
				m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
		}

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			m_GameClientView.m_HeapCard[i].ResetCardData();
			m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
#endif
		}
	}

	//���ý���
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//���ſ�ʼ����
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	if( pGameStart->cbSick[0] != 0 )
	{
		//ҡɫ�Ӷ���
		tagActionSice *pActionSice = (tagActionSice *)ActiveUserAction(AK_Sice);
		ASSERT(pActionSice);
		if( !pActionSice ) return false;
		CopyMemory(pActionSice->cbSick , pGameStart->cbSick,sizeof(pActionSice->cbSick));

		m_UserActionArray.Add(pActionSice);
	}

	//���ƶ���
	WORD wMeChairID = GetMeChairID();
	BYTE cbHandCardData[MAX_COUNT];
	m_GameLogic.SwitchToCardData( m_cbCardIndex,cbHandCardData );
	//�����˿�
	m_GameLogic.RandCardData(cbHandCardData,(m_wBankerUser==wMeChairID)?MAX_COUNT:MAX_COUNT-1,cbHandCardData);
	//ȷ������ɾ��λ��
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
	WORD wHeapId = 0;
	WORD wHeapCardIndex = 0;
#else
	WORD wHeapId = m_wHeapTail;
	WORD wHeapCardIndex = m_cbHeapCardInfo[m_wHeapTail][1]-1;
#endif
	//��4Ȧ�����1Ȧ��һ��
	for( BYTE i = 0; i < 4; i++ )
	{
		//��ׯ�ҿ�ʼ
		WORD wId = m_wBankerUser;
		for( WORD j = 0; j < GAME_PLAYER; j++ )
		{
			WORD wChairId = (wId+GAME_PLAYER+j)%GAME_PLAYER;

			//����������Ŀ
			tagActionStartCard *pStartCardItem = (tagActionStartCard *)ActiveUserAction(AK_StartCard);
			ASSERT( pStartCardItem );
			if( !pStartCardItem ) return false;

			//������
			pStartCardItem->cbCardCount = i<3?4:1;
			//����ֵ
			if( wChairId == wMeChairID )
				CopyMemory( pStartCardItem->cbCardData,&cbHandCardData[i*4],sizeof(BYTE)*(i<3?4:1) );
			//�������
			pStartCardItem->wChairId = wChairId;
			//����λ��
			pStartCardItem->wHeapId = wHeapId;
			//����λ��
			pStartCardItem->wHeapCardIndex = wHeapCardIndex;

			pStartCardItem->bLastItem = false;
			//���붯������
			m_UserActionArray.Add(pStartCardItem);

			//�ƽ�
			if( i < 3 )
			{
				if( wHeapCardIndex <= 3 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-(3-wHeapCardIndex)-1;
				}
				else wHeapCardIndex -= 4;
			}
			else
			{
				if( wHeapCardIndex == 0 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-1;
				}
				else wHeapCardIndex--;
			}
		}
	}
	//���һ�ţ�����ׯ��
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)ActiveUserAction(AK_StartCard);
	ASSERT( pStartCardItem );
	if( !pStartCardItem ) return false;
	//������
	pStartCardItem->cbCardCount = 1;
	//����ֵ
	if( m_wBankerUser == wMeChairID )
		pStartCardItem->cbCardData[0] = cbHandCardData[MAX_COUNT-1];
	//�������
	pStartCardItem->wChairId = m_wBankerUser;
	//����λ��
	pStartCardItem->wHeapId = wHeapId;
	//����λ��
	pStartCardItem->wHeapCardIndex = wHeapCardIndex;
	//���һ��
	pStartCardItem->bLastItem = true;
	//���붯������
	m_UserActionArray.Add(pStartCardItem);

	//��ʼ����
	BeginUserAction();

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	if( IsLookonMode() || pOutCard->wOutCardUser != GetMeChairID() )
	{
		tagActionOutCard *pActionOutCard = (tagActionOutCard *)ActiveUserAction(AK_OutCard);
		ASSERT(pActionOutCard);
		if( !pActionOutCard ) return false;

		pActionOutCard->cbOutCardData = pOutCard->cbOutCardData;
		pActionOutCard->wOutCardUser = pOutCard->wOutCardUser;

		//��Ӷ���
		m_UserActionArray.Add(pActionOutCard);

		//��ʼ����
		BeginUserAction();
	}

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��Ϣ����
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	tagActionSendCard *pActionSendCard = (tagActionSendCard *)ActiveUserAction(AK_SendCard);
	ASSERT(pActionSendCard);
	if( !pActionSendCard ) return false;

	pActionSendCard->bTail = pSendCard->bTail;
	pActionSendCard->cbActionMask = pSendCard->cbActionMask;
	pActionSendCard->cbCardData = pSendCard->cbCardData;
	pActionSendCard->wCurrentUser = pSendCard->wCurrentUser;
	pActionSendCard->wSendCardUser = pSendCard->wSendCardUser;
	pActionSendCard->cbLeftCardCount = pSendCard->cbLeftCardCount;

	//��Ӷ���
	m_UserActionArray.Add(pActionSendCard);

	//��ʼ����
	BeginUserAction();

	return true;
}

//������ʾ
bool CGameClientEngine::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//��Ӷ���
	tagActionActionNotify *pActionActionNotify = (tagActionActionNotify *)ActiveUserAction(AK_ActionNotify);
	ASSERT( pActionActionNotify );
	if( !pActionActionNotify ) return false;

	pActionActionNotify->cbActionCard = pOperateNotify->cbActionCard;
	pActionActionNotify->cbActionMask = pOperateNotify->cbActionMask;

	m_UserActionArray.Add(pActionActionNotify);

	//��ʼ����
	BeginUserAction();

	return true;
}

//�������
bool CGameClientEngine::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��Ӷ���
	tagActionActionResult *pActionActionResult = (tagActionActionResult *)ActiveUserAction(AK_ActionResult);
	ASSERT( pActionActionResult );
	if( !pActionActionResult ) return false;

	pActionActionResult->wOperateUser = pOperateResult->wOperateUser;
	pActionActionResult->wProvideUser = pOperateResult->wProvideUser;
	pActionActionResult->cbActionMask = pOperateResult->cbActionMask;
	pActionActionResult->cbOperateCode = pOperateResult->cbOperateCode;
	CopyMemory( pActionActionResult->cbOperateCard,pOperateResult->cbOperateCard,
		sizeof(pOperateResult->cbOperateCard) );

	m_UserActionArray.Add(pActionActionResult);

	//��ʼ����
	BeginUserAction();

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//��������
	tagActionGameEnd *pActionGameEnd = (tagActionGameEnd *)ActiveUserAction(AK_GameEnd);
	CopyMemory( pActionGameEnd->cbCardCount,pGameEnd->cbCardCount,sizeof(pGameEnd->cbCardCount) );
	CopyMemory( pActionGameEnd->cbCardData,pGameEnd->cbCardData,sizeof(pGameEnd->cbCardData) );
	pActionGameEnd->cbProvideCard = pGameEnd->cbProvideCard;
	CopyMemory( pActionGameEnd->dwChiHuKind,pGameEnd->dwChiHuKind,sizeof(pGameEnd->dwChiHuKind) );
	CopyMemory( pActionGameEnd->dwChiHuRight,pGameEnd->dwChiHuRight,sizeof(pGameEnd->dwChiHuRight) );
	pActionGameEnd->wLeftUser = pGameEnd->wLeftUser;
	CopyMemory( pActionGameEnd->lGameScore,pGameEnd->lGameScore,sizeof(pGameEnd->lGameScore) );
	CopyMemory(pActionGameEnd->lGangScore,pGameEnd->lGangScore,sizeof(pGameEnd->lGangScore));
	pActionGameEnd->lGameTax = pGameEnd->lGameTax;
	pActionGameEnd->wProvideUser = pGameEnd->wProvideUser;

	m_UserActionArray.Add(pActionGameEnd);

	BeginUserAction();

	return true;
}

//�û��й�
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	IClientUserItem *pUserItem = GetTableUserItem(pTrustee->wChairID);
#ifndef DEBUG	
	if ((IsLookonMode()||pTrustee->wChairID!=GetMeChairID())&&m_pIStringMessage!=NULL&&pUserItem!=NULL)
	{	
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pUserItem->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pUserItem->GetNickName());
		m_pIStringMessage->InsertSystemString(szBuffer);
	}
#endif
	return true;
}

//����
bool CGameClientEngine::OnSubAdminControl(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_CommandResult));
	if (wDataSize!=sizeof(CMD_S_CommandResult)) return false;

	//��Ϣ����
	CMD_S_CommandResult * pCommandResult=(CMD_S_CommandResult *)pBuffer;

	if (pCommandResult->cbAckType==ACK_ASK_CARD_DATA)
	{
		//��������
		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
		m_GameClientView.m_pClientControlDlg->SetHandCardData(cbCardData,cbCardCount,0);
		m_GameClientView.m_pClientControlDlg->ReqResult(pBuffer,this);
	}

	return true;
}

//��������
bool CGameClientEngine::OnSubOtherCard(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_OtherCard));
	if (wDataSize!=sizeof(CMD_S_OtherCard)) return false;

	//��Ϣ����
	CMD_S_OtherCard * pOtherCard=(CMD_S_OtherCard *)pBuffer;

	m_GameClientView.m_HandCardCheat.SetCardData(pOtherCard->cbCardData,pOtherCard->cbCardCount,0x00);
	m_GameClientView.m_HandCardCheat.SetPositively(true);
	m_GameClientView.m_HandCardCheat.SetDisplayItem(true);
	m_GameClientView.m_bChearUser=true;

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubAdminUpdate(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_C_CommandUpdate));
	if (wDataSize!=sizeof(CMD_C_CommandUpdate)) return false;

	//��Ϣ����
	CMD_C_CommandUpdate * pUpdate=(CMD_C_CommandUpdate *)pBuffer;

	if (!m_GameLogic.RemoveCard(m_cbCardIndex, pUpdate->cbHandCardData, pUpdate->cbCardCount))
	{
		ASSERT(FALSE);
	}
	for (BYTE i = 0; i < pUpdate->cbCardCount; i++)
	{
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pUpdate->cbSelectCardData[i])]++;
	}

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	if((cbCardCount + 1) % 3 == 0)
	{
		SetHandCardControl(m_cbCardIndex,cbCardData[cbCardCount - 1]);
	}
	else
	{
		SetHandCardControl(m_cbCardIndex,0x00);
	}

	return true;
}

//���ų�������
void CGameClientEngine::PlayCardSound(WORD wChairID, BYTE cbCardData)
{
	if(m_GameLogic.IsValidCard(cbCardData) == false)
	{
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}

	//�ж��Ա�
	IClientUserItem *pUserItem = GetTableUserItem(wChairID);

	if(pUserItem == NULL)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}
	bool bBoy = (pUserItem->GetGender() == GENDER_MANKIND ? true : false);
	BYTE cbType= (cbCardData & MASK_COLOR);
	BYTE cbValue= (cbCardData & MASK_VALUE);
	CString strSoundName;
	switch(cbType)
	{
	case 0X30:	//��
		{
			switch(cbValue) 
			{
			case 1:
				{
					strSoundName = _T("F_1");
					break;
				}
			case 2:
				{
					strSoundName = _T("F_2");
					break;
				}
			case 3:
				{
					strSoundName = _T("F_3");
					break;
				}
			case 4:
				{
					strSoundName = _T("F_4");
					break;
				}
			case 5:
				{
					strSoundName = _T("F_5");
					break;
				}
			case 6:
				{
					strSoundName = _T("F_6");
					break;
				}
			case 7:
				{
					strSoundName = _T("F_7");
					break;
				}
			default:
				{
					strSoundName=_T("BU_HUA");
				}

			}
			break;
		}		
	case 0X20:	//Ͳ
		{
			strSoundName.Format(_T("T_%d"), cbValue);
			break;
		}

	case 0X10:	//��
		{
			strSoundName.Format(_T("S_%d"), cbValue);
			break;
		}
	case 0X00:	//��
		{
			strSoundName.Format(_T("W_%d"), cbValue);
			break;
		}
	}	

	if(bBoy)
	{
		strSoundName = _T("BOY_") +strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strSoundName;
	}
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
}

//��������
void CGameClientEngine::PlayActionSound(WORD wChairID,BYTE cbAction)
{	
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}
	//�ж��Ա�
	IClientUserItem *pUserItem = GetTableUserItem(wChairID);
	if(pUserItem == NULL)
	{
		assert(0 && "�ò��������Ϣ");
		return;
	}

	bool bBoy = (pUserItem->GetGender() == GENDER_MANKIND ? true : false);

	switch (cbAction)
	{
	case WIK_LEFT:
	case WIK_CENTER:
	case WIK_RIGHT:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI"));		
			break;
		}
	case WIK_PENG:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_PENG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_PENG"));	
			break;
		}
	case WIK_GANG:		//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_GANG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_GANG"));		
			break;
		}
	case WIK_CHI_HU:	//�Ժ�
		{

			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI_HU"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI_HU"));		
			break;
		}
	case WIK_REPLACE:	//�滻����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_REPLACE"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_REPLACE"));		
			break;
		}
	case WIK_LISTEN:	//����
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_TING"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_TING"));		
			break;
		}
	}

	return;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard(BYTE cbCardData)
{
	return m_GameLogic.IsValidCard(cbCardData);
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	////TEST+
	//��������
	//tagScoreInfo ScoreInfo;
	//tagWeaveInfo WeaveInfo[4];
	//ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	//ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));
	//BYTE cbCardData[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x01};

	//WeaveInfo[0].cbWeaveCount = 1;
	//WeaveInfo[0].cbPublicWeave[0] = true;
	//WeaveInfo[0].cbCardCount[0] = 4;
	//for (BYTE i=0; i<4; i++)
	//{
	//	WeaveInfo[0].cbCardData[0][i] = 0x01;
	//}

	//ScoreInfo.cbCardCount[0] = 10;
	//CopyMemory(ScoreInfo.cbCardData[0], cbCardData, sizeof(BYTE)*10);
	//WeaveInfo[1].cbWeaveCount = 1;
	//WeaveInfo[1].cbPublicWeave[0] = true;
	//WeaveInfo[1].cbCardCount[0] = 4;
	//for (BYTE i=0; i<4; i++)
	//{
	//	WeaveInfo[1].cbCardData[0][i] = 0x01;
	//}

	//ScoreInfo.cbCardCount[1] = 10;
	//CopyMemory(ScoreInfo.cbCardData[1], cbCardData, sizeof(BYTE)*10);
	//ScoreInfo.lGameScore[0]=120;
	//ScoreInfo.lGameScore[1]=-20;
	//ScoreInfo.lGameScore[2]=-20;
	//ScoreInfo.lGameScore[3]=-20;

	//ScoreInfo.lGangScore[0]=20;
	//ScoreInfo.lGangScore[1]=-20;
	//ScoreInfo.lGangScore[2]=20;
	//ScoreInfo.lGangScore[3]=-20;

	//CChiHuRight chr;
	//chr |= CHR_PING_HU;

	//ScoreInfo.dwChiHuKind[0]=WIK_CHI_HU;
	//ScoreInfo.dwChiHuKind[1]=WIK_CHI_HU;

	//chr.GetRightData(ScoreInfo.dwChiHuRight[0], MAX_RIGHT_COUNT);
	//chr.GetRightData(ScoreInfo.dwChiHuRight[1], MAX_RIGHT_COUNT);
	//ScoreInfo.wLeftUser = INVALID_CHAIR;

	//TCHAR szAccounts[GAME_PLAYER][32];
	//lstrcpy(szAccounts[0],TEXT("һ�����������߰˾�ʮ"));
	//lstrcpy(szAccounts[1],TEXT("abcdefghijklmnopqrst"));
	//lstrcpy(szAccounts[2],TEXT("333333333"));
	//lstrcpy(szAccounts[3],TEXT("44444444444"));
	//CopyMemory(ScoreInfo.szUserName, szAccounts, sizeof(ScoreInfo.szUserName));

	//m_GameClientView.m_GameScore.SetScoreInfo(ScoreInfo, WeaveInfo);
	//KillGameClock(IDI_START_GAME);

	////���½���
	//m_GameClientView.InvalidGameView(0,0,0,0);
	//return 0;
	////TEST+

	//��������
	KillGameClock(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.SetControlInfo(false,WIK_NULL);
	m_GameClientView.m_GameScore.RestorationData();
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//���ý���
	m_GameClientView.SetDiscUser(INVALID_CHAIR);
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
#endif
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
		m_GameClientView.m_HeapCard[i].ResetCardData();
		m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
#endif
	}

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
#endif
	m_GameClientView.SetLeftCardCount(MAX_REPERTORY);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//��Ϸ�ر�
LRESULT CGameClientEngine::OnMessageGameClose(WPARAM wParam, LPARAM lParam)
{
	if(IsLookonMode() == true)
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return 0;
	}
	IClientUserItem * pIClientUserItem=GetTableUserItem(GetMeChairID());
	if(pIClientUserItem!=NULL)
		if(pIClientUserItem->GetUserStatus() == US_PLAYING)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			INT nRes=0;
			nRes=Information.ShowMessageBox(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),MB_ICONQUESTION|MB_YESNO,0);
			if (nRes!=IDYES)
			{
				return TRUE;
			}
		}
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return 0;
}

//���Ʋ���
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
		return 0;

	KillGameClock(IDI_OPERATE_CARD);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	BYTE cbOutCardData=(BYTE)wParam;

	//���ý���
	m_GameClientView.SetControlInfo(false,WIK_NULL);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//��Ӷ���
	tagActionOutCard *pActionOutCard = (tagActionOutCard *)ActiveUserAction(AK_OutCard);
	ASSERT( pActionOutCard );
	if( !pActionOutCard ) return 0;

	pActionOutCard->cbOutCardData = cbOutCardData;
	pActionOutCard->wOutCardUser = GetMeChairID();

	m_UserActionArray.Add(pActionOutCard);

	BeginUserAction();

	return 0;
}

//�˿˲���
LRESULT CGameClientEngine::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode;
	BYTE cbOperateCard[3] = {0,0,0};

	//ɾ��ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//��ȡѡ����Ϣ
	tagSelectCardInfo si;
	m_GameClientView.m_HandCardControl.GetUserSelectResult( si );

	//���������Ϣ
	cbOperateCode = (BYTE)si.wActionMask;
	cbOperateCard[0] = si.cbActionCard;
	CopyMemory( &cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2 );

	m_wCurrentUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//ȡ������ģʽ
	m_GameClientView.m_HandCardControl.SetSelectMode( false );

	//��������
	m_GameClientView.SetStatusFlag(false,true);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientEngine::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;

	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		//m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,true);
		m_GameClientView.m_btStusteeControl.SetButtonImage(&m_GameClientView.m_D3DDevice,TEXT("BT_START_TRUSTEE"),TEXT("PNG"),AfxGetInstanceHandle());	

#ifndef DEBUG
		if( FALSE == (BOOL)wParam && m_pIStringMessage!=NULL ) 
			m_pIStringMessage->InsertSystemString(_T("��ȡ�����йܹ���."));
#endif
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	else
	{
		m_bStustee=true;
		//m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,true);
		m_GameClientView.m_btStusteeControl.SetButtonImage(&m_GameClientView.m_D3DDevice,TEXT("BT_STOP_TRUSTEE"),TEXT("PNG"),AfxGetInstanceHandle());	

#ifndef DEBUG
		if( FALSE == (BOOL)wParam && m_pIStringMessage!=NULL )
			m_pIStringMessage->InsertSystemString(_T("��ѡ�����йܹ���."));
#endif
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}

	return 0;
}

//�ڶ���ҡɫ����Ϣ
LRESULT CGameClientEngine::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	return 0;
}


//������Ϣ
LRESULT CGameClientEngine::OnChatMessage( WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.InsertAllChatMessage((LPCTSTR)wParam,int(lParam));

	return 0;
}

//����Ա����
LRESULT CGameClientEngine::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_AMDIN_COMMAND, (CMD_C_AdminReq*)wParam, sizeof(CMD_C_AdminReq));
	return 0;
}

//�����˿�
void CGameClientEngine::SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard)
{
	//�����Ŀ
	WORD wMeChairID=GetMeChairID();
	BYTE cbWeaveCardCount=m_cbWeaveCount[wMeChairID]*3;

	//ת���˿�
	BYTE cbHandCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(cbCardIndex,cbHandCardData);

	//�����˿�
	if ( (cbWeaveCardCount+cbCardCount)==MAX_COUNT )
	{
		if( cbAdvanceCard != 0x00 )
		{
			//ɾ���˿�
			BYTE cbRemoveCard[]={cbAdvanceCard};
			VERIFY( m_GameLogic.RemoveCard(cbHandCardData,cbCardCount,cbRemoveCard,1) );

			//�����˿�
			cbHandCardData[cbCardCount-1]=cbAdvanceCard;
		}

		m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount-1,cbHandCardData[cbCardCount-1]);
	}
	else m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount,0x00);

	//���ÿ���
	if (m_GameClientView.m_pClientControlDlg != NULL)
		m_GameClientView.m_pClientControlDlg->SetHandCardData(cbHandCardData, cbCardCount, cbAdvanceCard);

	return;
}

//��ʱ����Ϣ
VOID CGameClientEngine::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

//��Ҳ����ؼ�
LRESULT CGameClientEngine::OnUserAction( WPARAM wParam, LPARAM lParam )
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard[3] = {0,0,0};

	//���ز����ؼ�
	m_GameClientView.SetControlInfo(false,WIK_NULL);

	//״̬�ж�
	if (cbOperateCode==WIK_NULL)
	{
		//���ñ���
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;
		if( m_wCurrentUser == INVALID_CHAIR )
		{
			//ɾ����ʱ��
			KillGameClock( IDI_OPERATE_CARD );
			//������Ϣ
			CMD_C_OperateCard OperateCard;
			OperateCard.cbOperateCode = WIK_NULL;
			ZeroMemory( OperateCard.cbOperateCard,sizeof(OperateCard.cbOperateCard) );
			SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
		}
		return 0;
	}

	bool bDone = false;
	//����
	if( cbOperateCode & WIK_CHI_HU )
		bDone = true;
	else
	{
		//��ȡѡ�����
		tagSelectCardInfo sci[MAX_WEAVE];
		BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

		//���ò����¼�
		bDone = m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );

		//�����ɲ���
		if( bDone )
		{
			//���ò������
			tagSelectCardInfo si;
			m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
			cbOperateCode = (BYTE)si.wActionMask;
			cbOperateCard[0] = si.cbActionCard;
			CopyMemory( &cbOperateCard[1],si.cbCardData,2*sizeof(BYTE) );

			m_GameClientView.m_HandCardControl.SetSelectMode( false );
		}
		//�������õȴ�ѡ��
		else m_GameClientView.SetStatusFlag( true,false );
	}

	//���������ɣ�ֱ�ӷ��Ͳ�������
	if( bDone )
	{
		//ɾ����ʱ��
		KillGameClock( IDI_OPERATE_CARD );

		//���ñ���
		m_wCurrentUser = INVALID_CHAIR;
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		CMD_C_OperateCard OperateCard;
		OperateCard.cbOperateCode = cbOperateCode;
		CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
		SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
	}

	return 0;
}

//��ȡ������Ϣ
BYTE CGameClientEngine::GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] )
{
	//��ʼ��
	BYTE cbSelectCount = 0;

	if( wOperateCode == WIK_NULL ) return 0;

	//����
	if( wOperateCode & WIK_LISTEN )
	{
		//��ȡ�ɶ�����
		BYTE cbCardData[MAX_COUNT],cbCardCount=0;
		BYTE cbCardIndex[MAX_INDEX];
		CopyMemory( cbCardIndex,m_cbCardIndex,sizeof(cbCardIndex) );
		WORD wMeChairId = GetMeChairID();
		CChiHuRight chr;
		for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		{
			if( cbCardIndex[i] == 0 ) continue;

			cbCardIndex[i]--;
			for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
			{
				BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(j);
				if( WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard( cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],
					cbCurrentCard,chr) )
				{
					cbCardData[cbCardCount++] = m_GameLogic.SwitchToCardData(i);
					break;
				}
			}
			cbCardIndex[i]++;
		}
		for( BYTE i = 0; i < cbCardCount; i++ )
		{
			SelectInfo[cbSelectCount].cbActionCard = cbCardData[i];
			SelectInfo[cbSelectCount].wActionMask = WIK_LISTEN;
			SelectInfo[cbSelectCount].cbCardCount = 1;
			SelectInfo[cbSelectCount++].cbCardData[0] = cbCardData[i];
		}
	}
	//����
	else if( wOperateCode&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_LEFT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_LEFT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard+1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+2;
		}
		if( m_cbActionMask & WIK_CENTER )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_CENTER;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+1;
		}
		if( m_cbActionMask & WIK_RIGHT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_RIGHT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-2;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard-1;
		}
	}
	//����
	else if( wOperateCode & WIK_PENG )
	{
		//Ч��
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
		SelectInfo[cbSelectCount].wActionMask = WIK_PENG;
		SelectInfo[cbSelectCount].cbCardCount = 2;
		SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
		SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard;
	}
	//����
	else if( wOperateCode & WIK_GANG )
	{
		//������Լ�����
		if( m_wCurrentUser == GetMeChairID() )
		{
			//Ѱ���Ƿ��ж������
			WORD wMeChairId = GetMeChairID();
			tagGangCardResult gcr;
			ZeroMemory( &gcr,sizeof(gcr) );
			m_GameLogic.AnalyseGangCard( m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],gcr );
			ASSERT( gcr.cbCardCount > 0 );
			for( BYTE i = 0; i < gcr.cbCardCount; i++ )
			{
				SelectInfo[cbSelectCount].cbActionCard = gcr.cbCardData[i];
				SelectInfo[cbSelectCount].wActionMask = WIK_GANG;				
				if( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(gcr.cbCardData[i])] == 1 )
				{
					SelectInfo[cbSelectCount].cbCardCount = 1;
					SelectInfo[cbSelectCount].cbCardData[0] = gcr.cbCardData[i];
				}
				else
				{
					SelectInfo[cbSelectCount].cbCardCount = m_GameLogic.GetWeaveCard(WIK_GANG,gcr.cbCardData[i],
						SelectInfo[cbSelectCount].cbCardData);
				}
				cbSelectCount++;
			}
		}
		else
		{
			ASSERT( m_cbActionCard != 0 );
			if( m_cbActionCard == 0 ) return 0;
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_GANG;
			SelectInfo[cbSelectCount].cbCardCount = 3;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
			SelectInfo[cbSelectCount].cbCardData[1] = m_cbActionCard;
			SelectInfo[cbSelectCount++].cbCardData[2] = m_cbActionCard;
		}
	}

	return cbSelectCount;
}

//��ʼ��Ҷ���
bool CGameClientEngine::BeginUserAction()
{
	if( m_UserActionArray.GetCount() == 0 )
		return false;

	//ö�ٶ���
	switch( m_UserActionArray[0]->enActionKind )
	{
	case AK_StartCard:				//����
		return BeginActionStartCard();
	case AK_OutCard:				//����
		return BeginActionOutCard();
	case AK_SendCard:				//ץ��
		return BeginActionSendCard();
	case AK_ActionNotify:			//��ʾ����
		return BeginActionActionNotify();
	case AK_ActionResult:			//�������
		return BeginActionActionResult();
	case AK_Sice:					//��ɫ��
		return BeginActionSice();	
	case AK_GameEnd:				//������Ϸ
		return BeginActionGameEnd();
	default:
		ASSERT(FALSE);
		return false;
	}

	return false;
}

//ֹͣ��Ҷ���
bool CGameClientEngine::StopUserAction()
{
	//ֹͣɫ�Ӷ���

	if( m_GameClientView.IsMovingCard() )
		m_GameClientView.StopMoveCard();

	while( BeginUserAction() )
	{
		if( m_GameClientView.IsMovingCard() )
			m_GameClientView.StopMoveCard();
	}

	return true;
}

//�Ƴ���ǰ����
bool CGameClientEngine::RemoveCurrentAction()
{
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//ɾ����ǰ����
	tagUserAction *pUserAction = m_UserActionArray[0];
	m_UserActionArray.RemoveAt(0);
	m_UserActionStorage.Add(pUserAction);
	pUserAction->ResetAction();

	return true;
}

//�����
tagUserAction *CGameClientEngine::ActiveUserAction(enmActionKind enActionKind)
{
	//��������
	tagUserAction *pUserAction = NULL;

	//Ѱ�ҿ��
	for( INT_PTR i = 0; i < m_UserActionStorage.GetCount(); i++ )
	{
		if( m_UserActionStorage[i]->enActionKind == enActionKind )
		{
			pUserAction = m_UserActionStorage[i];
			m_UserActionStorage.RemoveAt(i);
			return pUserAction;
		}
	}

	//�´���
	switch( enActionKind )
	{
	case AK_StartCard:
		{
			pUserAction = new tagActionStartCard;
			return pUserAction;
		}
	case AK_OutCard:
		{
			pUserAction = new tagActionOutCard;
			return pUserAction;
		}
	case AK_SendCard:
		{
			pUserAction = new tagActionSendCard;
			return pUserAction;
		}
	case AK_ActionNotify:
		{
			pUserAction = new tagActionActionNotify;
			return pUserAction;
		}
	case AK_ActionResult:
		{
			pUserAction = new tagActionActionResult;
			return pUserAction;
		}
	case AK_GameEnd:
		{
			pUserAction = new tagActionGameEnd;
			return pUserAction;
		}
	case AK_Sice:
		{
			pUserAction = new tagActionSice;
			return pUserAction;
		}
	default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	return pUserAction;
}

//���ƶ���
bool CGameClientEngine::BeginActionStartCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)m_UserActionArray[0];

	pStartCardItem->bHandleStatus = TRUE;

	//��ʼ,������
	CPoint ptFrom,ptTo;

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
	ptFrom.x = HEAP_START_POINT_X;
	ptFrom.y = HEAP_START_POINT_Y;
#else
	//����λ��
	WORD wHeapId = pStartCardItem->wHeapId;
	WORD wHeapViewId = SwitchViewChairID(wHeapId);
	//����λ��
	WORD wHeapCardIndex = pStartCardItem->wHeapCardIndex;
	if( pStartCardItem->cbCardCount > wHeapCardIndex+1 )
	{
		ASSERT( pStartCardItem->cbCardCount == 4 );
		//ɾ��ǰ��Ķ���
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,wHeapCardIndex+1 );

		wHeapId = (wHeapId+1)%GAME_PLAYER;
		wHeapViewId = SwitchViewChairID(wHeapId);
		wHeapCardIndex = HEAP_FULL_COUNT-(4-(wHeapCardIndex+1))-1;
		m_cbLeftCardCount -= pStartCardItem->cbCardCount;

		//��ȡ��ʼ��
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(WORD(wHeapCardIndex-1));

		//ɾ������
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( HEAP_FULL_COUNT-1,
			pStartCardItem->cbCardCount-pStartCardItem->wHeapCardIndex-1 );
	}
	else
	{
		WORD wHeapPos = wHeapCardIndex;
		if( pStartCardItem->cbCardCount == 4 )
			wHeapPos -= 3;
		else
		{
			ASSERT( pStartCardItem->cbCardCount == 1 );
			if( wHeapPos%2 != 0 ) wHeapPos--;
			else wHeapPos++;
		}
		m_cbLeftCardCount -= pStartCardItem->cbCardCount;
		//��ȡ��ʼ��
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(wHeapPos);

		//ɾ������
		if( pStartCardItem->cbCardCount == 4 )
			m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,4 );
		else
		{
			ASSERT( pStartCardItem->cbCardCount == 1 );
			if( wHeapCardIndex%2 != 0 )
				m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex-1,pStartCardItem->cbCardCount );
			else m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex+1,pStartCardItem->cbCardCount );
		}
	}
#endif

	//��ȡ������
	WORD wViewChairId = SwitchViewChairID(pStartCardItem->wChairId);
	if( wViewChairId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		ptTo = m_GameClientView.m_UserCard[wUserIndex].GetDispatchCardPos();
	}
	
	m_GameClientView.SetLeftCardCount(m_cbLeftCardCount);
	//���ŷ�������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

	//���춯��
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = pStartCardItem->cbCardCount;
	mci.cbCardData = 0;
	mci.wViewChairId = wViewChairId;
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//���ƶ���
bool CGameClientEngine::BeginActionOutCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionOutCard *pOutCardItem = (tagActionOutCard *)m_UserActionArray[0];

	pOutCardItem->bHandleStatus = TRUE;

	//�������
	WORD wMeChairId = GetMeChairID();
	WORD wOutCardUser = pOutCardItem->wOutCardUser;
	WORD wViewOutCardUser = SwitchViewChairID(wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->cbOutCardData;

	//��������
	KillGameClock(IDI_OPERATE_CARD); 

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//������ʼ,������
	CPoint ptFrom,ptTo;
	if( wOutCardUser == wMeChairId )
		ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wViewId = wViewOutCardUser>=3?2:wViewOutCardUser;
		ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
	}
	ptTo = m_GameClientView.m_DiscardCard[wViewOutCardUser].GetDispatchCardPos();

	//��������
	PlayCardSound(wOutCardUser,cbOutCardData);

	//���ý���
	m_GameClientView.SetCurrentUser( INVALID_CHAIR );
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(wViewOutCardUser,cbOutCardData);
	m_GameClientView.SetStatusFlag(false,false);

	//�����˿�
	if (wViewOutCardUser==MYSELF_VIEW_ID)
	{
		//ɾ���˿�
		if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
		{
			ASSERT( FALSE );
			return false;
		}

		SetHandCardControl( m_cbCardIndex,0x00 );
	}
	else
	{
		WORD wUserIndex=(wViewOutCardUser>2)?2:wViewOutCardUser;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
	}

	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = 1;
	mci.cbCardData = cbOutCardData;
	mci.wViewChairId = wViewOutCardUser;
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//���ƶ���
bool CGameClientEngine::BeginActionSendCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionSendCard *pSendCardItem = (tagActionSendCard *)m_UserActionArray[0];

	m_cbLeftCardCount--;
	pSendCardItem->bHandleStatus = TRUE;
	if (pSendCardItem->cbLeftCardCount != m_cbLeftCardCount)
	{
		m_cbLeftCardCount = pSendCardItem->cbLeftCardCount;
	}
	m_GameClientView.SetLeftCardCount(m_cbLeftCardCount);

	WORD wSendUserViewId = SwitchViewChairID(pSendCardItem->wSendCardUser);

	//���ÿ���
	if (m_GameClientView.m_pClientControlDlg != NULL)
		m_GameClientView.m_pClientControlDlg->SetLeftCardCount(m_cbLeftCardCount);

	//������ʼ,������
	CPoint ptFrom,ptTo;
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
	ptFrom.x = HEAP_START_POINT_X;
	ptFrom.y = HEAP_START_POINT_Y;
#else
	if( !pSendCardItem->bTail )
	{
		ASSERT( m_wHeapHead != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
		//ɾ����ͷ
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) )
		{
			m_wHeapHead = (m_wHeapHead+1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) );
		}
	}
	else
	{
		ASSERT( m_wHeapTail != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
		//ɾ����β
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) )
		{
			m_wHeapTail = (m_wHeapTail+GAME_PLAYER-1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) );
		}
	}
#endif
	if( wSendUserViewId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wViewId = wSendUserViewId>=3?2:wSendUserViewId;
		ptTo = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
	}

	//���ÿ���
	if (m_GameClientView.m_pClientControlDlg != NULL)
		m_GameClientView.m_pClientControlDlg->SetCurrentCard(pSendCardItem->cbCardData);

	//���ŷ�������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

	//���춯��
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = 1;
	mci.cbCardData = 0;
	mci.wViewChairId = SwitchViewChairID(pSendCardItem->wSendCardUser);
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//��ʾ����
bool CGameClientEngine::BeginActionActionNotify()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionActionNotify *pOperateNotify = (tagActionActionNotify *)m_UserActionArray[0];

	pOperateNotify->bHandleStatus = TRUE;

	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//��ȡ����
		WORD wMeChairID=GetMeChairID();
		m_cbActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard = pOperateNotify->cbActionCard;

		//������������ڲ���ģʽ,��ȡ��
		if( m_GameClientView.m_HandCardControl.GetSelectMode() )
			m_GameClientView.m_HandCardControl.SetSelectMode(false);

		//���ý���
		ActiveGameFrame();
		m_GameClientView.SetControlInfo(true,m_cbActionMask);

		//����ʱ��
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);
		SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return OnActionActionNotify();
}

//�������
bool CGameClientEngine::BeginActionActionResult()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionActionResult *pOperateResult = (tagActionActionResult *)m_UserActionArray[0];

	pOperateResult->bHandleStatus = TRUE;

	//ɾ����ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard[3];
	CopyMemory( cbOperateCard,pOperateResult->cbOperateCard,sizeof(cbOperateCard) );
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);
	WORD wProviderViewID = SwitchViewChairID(pOperateResult->wProvideUser);

	//���ñ���
	m_wCurrentUser=pOperateResult->wOperateUser;

	//�������
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
	{
		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;

		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[3] = cbCenterCard;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//�����˿�
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
			for( BYTE i = 0; i < 4; i++ ) m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[i] = cbOperateCard[0];

			m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);
		}

		//��Ͻ���
		BYTE cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=4;
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,
			cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbOperateCard[0])]=0;
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}
	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		CopyMemory( m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,cbOperateCard,sizeof(cbOperateCard) );

		//��Ͻ���
		BYTE cbWeaveCard[3],cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=3;
		CopyMemory( cbWeaveCard,cbOperateCard,sizeof(cbWeaveCard) );
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			VERIFY( m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1) );
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1) )
			{
				ASSERT( FALSE );
				return false;
			}
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//���ý���
	//����ṩ��Ҳ����Լ�,��ȥ���ṩ��ҵ��ѳ���
	if( pOperateResult->wProvideUser != wOperateUser )
	{
		if( m_GameClientView.m_DiscardCard[wProviderViewID].GetCardCount() > 0 )
			m_GameClientView.m_DiscardCard[wProviderViewID].RemoveCard(1);
	}
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.SetControlInfo(false,WIK_NULL);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
	//������������ڲ���ģʽ,��ȡ��
	if( m_GameClientView.m_HandCardControl.GetSelectMode() )
		m_GameClientView.m_HandCardControl.SetSelectMode(false);

// 	//���½���
// 	m_GameClientView.InvalidGameView(0,0,0,0);

	//��������
	PlayActionSound(wOperateUser,pOperateResult->cbOperateCode);

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		if( IsLookonMode()==false && m_wCurrentUser == GetMeChairID() )
		{
			m_cbActionMask = pOperateResult->cbActionMask;

			if( m_cbActionMask != WIK_NULL )
				m_GameClientView.SetControlInfo(true,m_cbActionMask);
		}

		//����ʱ��
		WORD wTimeCount=TIME_OPERATE_CARD;

		//����ʱ��
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}

	return OnActionActionResult();
}

//��ɫ��
bool CGameClientEngine::BeginActionSice()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionSice *pActionSice = (tagActionSice *)m_UserActionArray[0];

	pActionSice->bHandleStatus = TRUE;

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));	

	//ҡɫ�Ӷ���
	m_GameClientView.SetSick(true,pActionSice->cbSick[0]);
	m_GameClientView.SetSick(true,pActionSice->cbSick[1]);

	return true;
}

//��Ϸ����
bool CGameClientEngine::BeginActionGameEnd()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//��������
	tagActionGameEnd *pGameEnd = (tagActionGameEnd *)m_UserActionArray[0];

	pGameEnd->bHandleStatus = TRUE;

	//����״̬
	SetGameStatus(GAME_SCENE_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//ɾ����ʱ��
	KillGameClock(IDI_OPERATE_CARD);

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetControlInfo(false,WIK_NULL);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
	}

	//��������
	tagScoreInfo ScoreInfo;
	tagWeaveInfo WeaveInfo[GAME_PLAYER];
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

	//�ɼ�����
	ScoreInfo.wProvideUser=pGameEnd->wProvideUser;
	ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;
	ScoreInfo.wBankerUser = m_wBankerUser;
	CopyMemory(ScoreInfo.lGangScore,pGameEnd->lGangScore,sizeof(pGameEnd->lGangScore));

	//���û���
	CString strTemp ,strEnd = TEXT("���ֽ���,�ɼ�ͳ��:\r\n");
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		strTemp.Format(TEXT("%s: %+ld\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}
	//��Ϣ����
#ifndef DEBUG
	if(m_pIStringMessage !=NULL)
		m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);
#endif
	if( pGameEnd->wProvideUser != INVALID_CHAIR || pGameEnd->wLeftUser != INVALID_CHAIR )
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];
			CopyMemory( ScoreInfo.dwChiHuRight[i],pGameEnd->dwChiHuRight[i],MAX_RIGHT_COUNT*sizeof(DWORD) );

			//���óɼ�
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));

			m_HistoryScore.OnEventUserScore(i,pGameEnd->lGameScore[i]);

			//�����˿�
			if ((ScoreInfo.cbCardCount[i]==0)&&(pGameEnd->dwChiHuKind[i]!=WIK_NULL))
			{
				//����˿�
				WeaveInfo[i].cbWeaveCount=m_cbWeaveCount[i];
				for (BYTE j=0;j<WeaveInfo[i].cbWeaveCount;j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					WeaveInfo[i].cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
					WeaveInfo[i].cbCardCount[j]=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					CopyMemory( WeaveInfo[i].cbCardData[j],m_WeaveItemArray[i][j].cbCardData,sizeof(m_WeaveItemArray[i][j].cbCardData) );
				}

				//�����˿�
				ScoreInfo.cbCardCount[i]=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData[i],&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount[i]*sizeof(BYTE));

				//��ȡ����
				for (BYTE j=0;j<ScoreInfo.cbCardCount[i];j++)
				{
					if (ScoreInfo.cbCardData[i][j]==pGameEnd->cbProvideCard)
					{
						MoveMemory(&ScoreInfo.cbCardData[i][j],&ScoreInfo.cbCardData[i][j+1],(ScoreInfo.cbCardCount[i]-j-1)*sizeof(BYTE));
						ScoreInfo.cbCardData[i][ScoreInfo.cbCardCount[i]-1]=pGameEnd->cbProvideCard;
						break;
					}
				}
			}
		}

		ScoreInfo.wLeftUser = pGameEnd->wLeftUser;
		//�ɼ�����
		m_GameClientView.m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo);
		m_GameClientView.m_btContiueGame.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCloseScore.ShowWindow(SW_SHOW);
	}
	else
	{
		m_GameClientView.SetHuangZhuang(true);

		//���ý���
		if (IsLookonMode()==false)
		{
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		}
	}

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->dwChiHuKind[i]!=WIK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
#endif
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//��������
	if( pGameEnd->wProvideUser != INVALID_CHAIR )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( pGameEnd->dwChiHuKind[i] != WIK_NULL )
			{
				PlayActionSound( i,WIK_CHI_HU );
				break;
			}
		}
	}
	else 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
	}

	//���ý���
	if (IsLookonMode()==false)
	{
		//m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		//m_GameClientView.m_btStart.SetFocus();
		m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//ȡ���й�
	if(m_bStustee)
		OnStusteeControl(0,0);

// 	//���½���
// 	m_GameClientView.InvalidGameView(0,0,0,0);

	return OnActionGameEnd();
}

//�������
bool CGameClientEngine::OnActionStartCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)m_UserActionArray[0];

	//���ñ���
	WORD wMeChairId = GetMeChairID();
	WORD wChairId = pStartCardItem->wChairId;
	WORD wViewChairId = SwitchViewChairID(wChairId);

	//����˿�
	if( wViewChairId == MYSELF_VIEW_ID )
		m_GameClientView.m_HandCardControl.AddCardData( pStartCardItem->cbCardData,pStartCardItem->cbCardCount );
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		if( pStartCardItem->bLastItem )
			m_GameClientView.m_UserCard[wUserIndex].AddCardData(true);
		else
		{
			for( BYTE i = 0; i < pStartCardItem->cbCardCount; i++ )
				m_GameClientView.m_UserCard[wUserIndex].AddCardData(false);
		}
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

	//�ж��Ƿ����һ����
	if( pStartCardItem->bLastItem )
	{
		//������
		if (!IsLookonMode())
			ActiveGameFrame();

		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
#endif
		}

		//�����˿�
		SetHandCardControl(m_cbCardIndex,0x00);

		m_GameClientView.m_HandCardControl.SetPositively(!IsLookonMode());

		//������ʾ,��������
		if (!IsLookonMode()&&(m_wCurrentUser==wMeChairId))
		{
			m_GameClientView.SetStatusFlag(true,false);

			m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);

			if( m_cbActionMask != WIK_NULL )
				m_GameClientView.SetControlInfo(true,m_cbActionMask);
		}
		else if( m_wCurrentUser != wMeChairId && m_wCurrentUser != INVALID_CHAIR )
			m_GameClientView.SetStatusFlag(false,true);

		//����ʱ��
		if (m_wCurrentUser!=INVALID_CHAIR)
		{
			m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
			SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
	}

// 	//���½���
// 	m_GameClientView.InvalidGameView(0,0,0,0);
// 
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//�������
bool CGameClientEngine::OnActionOutCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionOutCard *pOutCardItem = (tagActionOutCard *)m_UserActionArray[0];

	WORD wOutUserViewId = SwitchViewChairID(pOutCardItem->wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->cbOutCardData;

	//��ӵ��Ѷ�����
	m_GameClientView.m_DiscardCard[wOutUserViewId].AddCardItem( cbOutCardData );

	//���ó���ָʾ
	m_GameClientView.SetDiscUser( wOutUserViewId );

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

// 	//���½���
// 	m_GameClientView.InvalidGameView(0,0,0,0);
// 
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//ץ�����
bool CGameClientEngine::OnActionSendCard()
{
	//Ч��
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//��������
	tagActionSendCard *pSendCardItem = (tagActionSendCard *)m_UserActionArray[0];

	//���ñ���
	WORD wMeChairID = GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pSendCardItem->wSendCardUser);
	WORD wChairId = pSendCardItem->wSendCardUser;
	BYTE cbSendCardData = pSendCardItem->cbCardData;
	m_wCurrentUser = pSendCardItem->wCurrentUser;

	//ȡ�ƽ���
	if (wViewChairID!=MYSELF_VIEW_ID)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		BYTE cbCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount()+m_cbWeaveCount[wChairId]*3;
		m_GameClientView.m_UserCard[wUserIndex].AddCardData(cbCardCount==MAX_COUNT-1?true:false);
	}
	else
	{
		//�����˿�
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbSendCardData)]++;

		//�����˿�
		SetHandCardControl(m_cbCardIndex,cbSendCardData);
	}

	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//������
		ActiveGameFrame();

		m_cbActionMask = pSendCardItem->cbActionMask;
		m_cbActionCard = pSendCardItem->cbCardData;

		//��������
		if (m_cbActionMask!=WIK_NULL)
		{
			//���ý���
			m_GameClientView.SetControlInfo(true,m_cbActionMask);
		}
	}

	//������ʾ
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

// 	//���½���
// 	m_GameClientView.InvalidGameView(0,0,0,0);

	//����ʱ��
	WORD wTimeCount=TIME_OPERATE_CARD;

	//����ʱ��
	if( m_wCurrentUser != INVALID_CHAIR )
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}
	else m_GameClientView.SetCurrentUser( INVALID_CHAIR );

	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//��ʾ���
bool CGameClientEngine::OnActionActionNotify()
{
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//�������
bool CGameClientEngine::OnActionActionResult()
{
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//��ɫ�����
bool CGameClientEngine::OnActionSice()
{
	//���ش���

	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnActionGameEnd()
{
	//ɾ������
	RemoveCurrentAction();

	//�¸�����
	BeginUserAction();

	return true;
}

//�������
LRESULT CGameClientEngine::OnUserActionFinish(WPARAM wParam, LPARAM lParam)
{
	//У��
	ASSERT( m_UserActionArray.GetCount() > 0 );		
	if( m_UserActionArray.GetCount() == 0 ) return 0;

	//ö�ٶ���
	switch( m_UserActionArray[0]->enActionKind )
	{
	case AK_StartCard:				//�������
		OnActionStartCard();
		break;
	case AK_OutCard:				//�������
		OnActionOutCard();
		break;
	case AK_SendCard:				//ץ�����
		OnActionSendCard();
		break;
	case AK_Sice:					//��ɫ�����
		OnActionSice();
		break;
	default:
		ASSERT(FALSE);
		return 0;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
