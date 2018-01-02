#include "StdAfx.h"
#include "TableFrameSink.h"

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_lSiceCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	ZeroMemory(m_lGangPaiScore,sizeof(m_lGangPaiScore));
	m_HistoryScore.ResetData();

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//������Ϣ
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
#endif

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 0;
	m_cbLianZhuangCount = 0;

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_hInst = NULL;
	m_pServerContro = NULL;
	m_hInst = LoadLibrary(TEXT("SparrowDZServerControl.dll"));
	if (m_hInst != NULL)
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if (ServerControl != NULL)
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
	if (m_pServerContro != NULL)
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if (m_hInst != NULL)
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
		return false;
	}
	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_lSiceCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	ZeroMemory(m_lGangPaiScore,sizeof(m_lGangPaiScore));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//������Ϣ
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
#endif

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 0;

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	//�����˿�
	m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),0);

	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));	

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	if( m_wBankerUser == INVALID_CHAIR )
	{
		m_wBankerUser = rand()%GAME_PLAYER;
	}

	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//������Ϣ
	WORD wSice = WORD(m_lSiceCount&0xffff);
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//��ȡ�˿�
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//����ж�
		if (cbTakeCount==0)
		{
			m_wHeapHead=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
			break;
		}
		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}
#endif

	//�����ж�
	tagGangCardResult GangCardResult;
	m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

	//�����ж�
	CChiHuRight chr;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
	m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr);
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//��������
	CMD_S_GameStart GameStart;
	GameStart.cbSick[0]=HIBYTE(wSice);
	GameStart.cbSick[1]=LOBYTE(wSice);
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser= m_wCurrentUser;
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	GameStart.wHeapHead = m_wHeapHead;
	GameStart.wHeapTail = m_wHeapTail;
#endif
	GameStart.cbLianZhuangCount = m_cbLianZhuangCount;
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
#endif

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		GameStart.cbUserAction=m_cbUserAction[i];

		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		if( m_pITableFrame->GetTableUserItem(i)->IsAndroidUser() )
		{
			BYTE bIndex = 1;
			for( WORD j=0; j<GAME_PLAYER; j++ )
			{
				if( j == i ) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j],&GameStart.cbCardData[MAX_COUNT*bIndex++]);
			}
		}

		//������
		for( WORD j=0; j<GAME_PLAYER; j++ )
		{
			if( j == i ) continue;
			IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(j);

			//if (IsCheatUser(pServerUserItem->GetGameID()))
			{
				CMD_S_OtherCard OtherCard;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[i],OtherCard.cbCardData);
				OtherCard.cbCardCount=(i==m_wBankerUser?MAX_COUNT:MAX_COUNT-1);
				m_pITableFrame->SendTableData(j,SUB_S_OTHER_CARD,&OtherCard,sizeof(OtherCard));
			}
		}

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = INVALID_CHAIR;

			//������Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
				//Ȩλ����
				if( m_dwChiHuKind[i] == WIK_CHI_HU )
				{
					FiltrateRight( i,m_ChiHuRight[i] );
					m_ChiHuRight[i].GetRightData( GameEnd.dwChiHuRight[i],MAX_RIGHT_COUNT );
				}
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//ͳ�ƻ���
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//������Ϣ
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
				CopyMemory(GameEnd.lGangScore,m_lGangPaiScore,sizeof(m_lGangPaiScore));

				CalScore( GameEnd.lGameScore );
			}
			else
			{
				//���ֽ���
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
			}

			//ͳ�ƻ���
			LONGLONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//���Ʒ�
				GameEnd.lGameScore[i] += m_lGameScore[i];
				//�ۼӻ���
				GameEnd.lGameScore[i]*=m_pGameServiceOption->lCellScore;
				//���û���
				if((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0)
				{
					if (GameEnd.lGameScore[i]>0L)
					{
						lGameTax[i] = GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenueRatio/1000L;
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE  ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;
				
				ScoreInfo[i].cbType = ScoreKind;
				ScoreInfo[i].lRevenue=lGameTax[i];
				ScoreInfo[i].lScore = GameEnd.lGameScore[i];

				//������Ϣ
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//��ʷ����
				m_HistoryScore.OnEventUserScore(i, GameEnd.lGameScore[i]);
			}
			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//���ñ���
			if( m_wProvideUser != INVALID_CHAIR )
			{
				WORD wBankerUser = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;
				for( ; wBankerUser != m_wBankerUser; wBankerUser = (wBankerUser+GAME_PLAYER-1)%GAME_PLAYER )
					if( m_dwChiHuKind[wBankerUser] != WIK_NULL ) break;
				if (wBankerUser==m_wBankerUser)
				{
					m_cbLianZhuangCount++;
				}
				else
					m_cbLianZhuangCount=0;
				m_wBankerUser = wBankerUser;
			}
			else m_wBankerUser = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = INVALID_CHAIR;

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.wLeftUser = wChairID;

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//����
			GameEnd.lGameScore[wChairID] = -1L*m_pGameServiceOption->lCellScore;

			//֪ͨ��Ϣ
#ifndef DEBUG
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetNickName());
			m_pITableFrame->SendGameMessage(szMessage,SMT_CHAT);
#endif
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//д�����
			//tagScoreInfo ScoreInfoArray;
			//ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
			//ScoreInfoArray.lScore=GameEnd.lGameScore[wChairID];
			//ScoreInfoArray.cbType=SCORE_TYPE_FLEE;
			//m_pITableFrame->WriteUserScore(wChairID,ScoreInfoArray);
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				BYTE  ScoreKind;
				if(i==wChairID)
					ScoreKind = SCORE_TYPE_FLEE;
				else 
					ScoreKind = SCORE_TYPE_DRAW;

				ScoreInfoArray[i].cbType = ScoreKind;
				ScoreInfoArray[i].lRevenue=0;
				ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];
				//��ʷ����
				m_HistoryScore.OnEventUserScore(i, GameEnd.lGameScore[i]);
			}
			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//��ʷ����
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//��������
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���ñ���
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChairID]==false)?m_cbUserAction[wChairID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			//������Ϣ
			StatusPlay.wHeapHead = m_wHeapHead;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
#endif

			//�˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],StatusPlay.cbCardData);
			StatusPlay.cbSendCardData=(m_wProvideUser==wChairID)?m_cbProvideCard:0x00;

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���ñ���
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pData;
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pData;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;


			m_bTrustee[pIServerUserItem->GetChairID()]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pIServerUserItem->GetChairID();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

			return true;
		}
	case SUB_C_AMDIN_COMMAND:
		{
			if (IsCheatUser(pIServerUserItem->GetGameID()))
			{
				ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
				if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;

				//Ȩ���ж�
				if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;

				// ��ȡ��Ϣ
				const CMD_C_AdminReq* AdminReq = static_cast<const CMD_C_AdminReq*>(pData);

				//����ʣ����
				if (AdminReq->cbReqType==RQ_ASK_CARD_DATA)
				{
					CMD_S_CommandResult Result;
					CopyMemory(Result.cbRepertoryCard,m_cbRepertoryCard,sizeof(Result.cbRepertoryCard));
					Result.cbRepertoryCardCount=m_cbLeftCardCount;
					Result.cbAckType=ACK_ASK_CARD_DATA;	
					Result.cbResult=CR_ACCEPT;

					m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_S_AMDIN_COMMAND,&Result,sizeof(Result));
					return true;
				}
				if (AdminReq->cbReqType==RQ_SET_CARD_DATA)
				{
					CMD_C_CommandUpdate CommandUpdate;
					CommandUpdate.cbCardCount=AdminReq->cbSelectCount;
					CopyMemory(CommandUpdate.cbHandCardData,AdminReq->cbHandSelectData,sizeof(AdminReq->cbHandSelectData));
					WORD wCharID = pIServerUserItem->GetChairID();

					//�Ƴ�����
					for (int j = 0;j < AdminReq->cbSelectCount; j++)
					{
						m_cbCardIndex[wCharID][m_GameLogic.SwitchToCardIndex(CommandUpdate.cbHandCardData[j])]--;
					}

					for (int j = 0;j < AdminReq->cbSelectCount; j++)
					{
						for (int i = 0; i < m_cbLeftCardCount ; i++)
						{
							if (AdminReq->cbHeapSelectData[j] == m_cbRepertoryCard[i])
							{
								CommandUpdate.cbSelectCardData[j] = AdminReq->cbHeapSelectData[j];
								m_cbRepertoryCard[i]=AdminReq->cbHandSelectData[j];

								//�������
								m_cbCardIndex[wCharID][m_GameLogic.SwitchToCardIndex(CommandUpdate.cbSelectCardData[j])]++;
								break;
							}
						}
					}

					m_pITableFrame->SendTableData(wCharID,SUB_S_UPDATE_CARD,&CommandUpdate,sizeof(CommandUpdate));
					return true;
				}
			
				return true;
			}
		}
	}


	return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (!bLookonUser)
	{
		m_HistoryScore.OnEventUserEnter(wChairID);
	}
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if (bLookonUser==false&&pIServerUserItem!=NULL)
	{		
		m_HistoryScore.OnEventUserLeave(wChairID);
		m_wBankerUser = INVALID_CHAIR;

		m_bTrustee[pIServerUserItem->GetChairID()]=false;
		CMD_S_Trustee Trustee;
		Trustee.bTrustee=false;
		Trustee.wChairID = pIServerUserItem->GetChairID();
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//Ч�����
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bSendStatus=true;
	m_bGangStatus = false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//�û��л�
	m_wCurrentUser=(wChairID+m_wPlayerCount+1)%m_wPlayerCount;

	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//�ɷ��˿�
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY)
		return true;

	//Ч���û� ע�⣺�������п��ܷ����˶���
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return true;

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		//ASSERT(m_bResponse[wChairID]==false);
		//ASSERT( m_cbUserAction[wChairID] != WIK_NULL );
		//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true) return true;
		if( m_cbUserAction[wChairID] == WIK_NULL ) return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

		//��������
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		if(cbOperateCard[0]==0)
			m_cbOperateCard[wChairID][0] = m_cbProvideCard;
		else CopyMemory( m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]) );

		//ִ���ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//��������
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//�����˿�
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser][0];

		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+m_wPlayerCount-1)%GAME_PLAYER)
			{
				//�����ж�
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//�����ж�
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i]);

				//�����˿�
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					wTargetUser = i;
				}
			}

			//������Ϸ
			ASSERT(m_dwChiHuKind[wTargetUser]!=WIK_NULL);
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//����˿�
		ASSERT(m_cbWeaveItemCount[wTargetUser]<MAX_WEAVE);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		m_WeaveItemArray[wTargetUser][wIndex].cbCardData[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = m_cbOperateCard[wTargetUser][1];
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = m_cbOperateCard[wTargetUser][2];
		}
		else
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = cbTargetCard;
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = cbTargetCard;
			if( cbTargetAction & WIK_GANG )
				m_WeaveItemArray[wTargetUser][wIndex].cbCardData[3] = cbTargetCard;
		}

		//ɾ���˿�
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���˿�
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_GANG:		//���Ʋ���
			{
				//ɾ���˿�,��������ֻ���ڷŸ�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		default:
			ASSERT( FALSE );
			return false;
		}

		//������
		CMD_S_OperateResult OperateResult;
		ZeroMemory( &OperateResult,sizeof(OperateResult) );
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		OperateResult.cbOperateCard[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			CopyMemory( &OperateResult.cbOperateCard[1],&m_cbOperateCard[wTargetUser][1],2*sizeof(BYTE) );
		else if( cbTargetAction&WIK_PENG )
		{
			OperateResult.cbOperateCard[1] = cbTargetCard;
			OperateResult.cbOperateCard[2] = cbTargetCard;
		}

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//����Ǹ���
		if( cbTargetAction != WIK_GANG )
		{
			tagGangCardResult gcr;
			m_cbUserAction[wTargetUser] |= m_GameLogic.AnalyseGangCard( m_cbCardIndex[wTargetUser],m_WeaveItemArray[wTargetUser],m_cbWeaveItemCount[wTargetUser],
				gcr );
			OperateResult.cbActionMask |= m_cbUserAction[wTargetUser];
		}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus = true;
			//���Ƶ÷�
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			m_lGangScore[wTargetUser] += 1;
			m_lGangScore[m_wProvideUser] -= 1;
#endif

			CopyMemory(m_lGangPaiScore,m_lGangScore,sizeof(m_lGangPaiScore));

			DispatchCardData(wTargetUser,true);
		}

		return true;
	}

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return false;

		//�˿�Ч��
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false)) 
			return false;

		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_GANG:			//���Ʋ���
			{
				//��������
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

				//���ƴ���
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//Ѱ�����
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
						{
							bPublic=true;
							cbWeaveIndex=i;
							break;
						}
					}

					//Ч�鶯��
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//����˿�
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];

					//���Ƶ÷�
					WORD wProvideUser = m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser;
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
					m_lGangScore[wChairID] += 1;
					m_lGangScore[wProvideUser] -= 1;
#endif
					CopyMemory(m_lGangPaiScore,m_lGangScore,sizeof(m_lGangPaiScore));
				}
				else
				{
					//�˿�Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//���ñ���
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					for( BYTE j = 0; j < 4; j++ ) m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];

					//���Ƶ÷�
					for( WORD i = 0; i < GAME_PLAYER; i++ )
					{
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
						if( i == wChairID ) m_lGangScore[i] += 3;
						else m_lGangScore[i] -= 1;
#endif
					}
					CopyMemory(m_lGangPaiScore,m_lGangScore,sizeof(m_lGangPaiScore));
				}

				//ɾ���˿�
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;

				//������
				CMD_S_OperateResult OperateResult;
				ZeroMemory( &OperateResult,sizeof(OperateResult) );
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard[0]=cbOperateCard[0];

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//Ч�鶯��
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);

				//�����˿�
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}

				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				if (m_cbOutCardCount==0)
				{
					m_wProvideUser = m_wCurrentUser;
					m_cbProvideCard = m_cbSendCardData;
				}

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1) )
				{
					ASSERT( FALSE );
					return false;
				}
				m_dwChiHuKind[wChairID] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,
					m_ChiHuRight[wChairID]);
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;

				//������Ϸ
				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;
	ASSERT( m_bSendStatus );
	if( m_bSendStatus == false ) return false;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//��ׯ����
	if (m_cbLeftCardCount==m_cbEndLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//���Ƶ÷�
	if( m_bGangStatus )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
			m_lGameScore[i] += m_lGangScore[i];
		ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	}

	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];

	//���ñ���
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;
	m_wProvideUser=wCurrentUser;
	m_cbProvideCard=m_cbSendCardData;

	//����
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	if( !m_bEnjoinChiHu[m_wCurrentUser] )
	{
		//�����ж�
		CChiHuRight chr;
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
		m_cbUserAction[m_wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
			m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr);
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
	}

	//�����ж�
	if ((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
	{
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
			m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
	}

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//������Ϣ
	ASSERT( m_wHeapHead != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHead][0]+m_cbHeapCardInfo[m_wHeapHead][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHead=(m_wHeapHead+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHead][0]++;
	}
	else
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}
#endif

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wSendCardUser = wCurrentUser;
	SendCard.wCurrentUser=m_wCurrentUser;
	SendCard.bTail = bTail;
	SendCard.cbLeftCardCount = m_cbLeftCardCount;
	SendCard.cbActionMask=m_cbUserAction[m_wCurrentUser];
	SendCard.cbCardData=(m_wCurrentUser==wCurrentUser)?m_cbProvideCard:m_cbSendCardData;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�û�����
		if (wCenterUser==i) continue;

		//��������
		if (EstimatKind==EstimatKind_OutCard)
		{
			//�����ж�
			if (m_bEnjoinChiPeng[i]==false)
			{
				//�����ж�
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				if (m_cbLeftCardCount>m_cbEndLeftCount) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false)	
		{
			//�Ժ��ж�
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr);
			m_cbUserAction[i] |= cbAction;
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}

	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}

//���
void CTableFrameSink::CalScore( LONGLONG lScore[GAME_PLAYER] )
{
	//��ʼ��
	ZeroMemory( lScore,sizeof(LONGLONG)*GAME_PLAYER );

	for( WORD wWinner = 0; wWinner < GAME_PLAYER; wWinner++ )
	{
		if( m_dwChiHuKind[wWinner]==WIK_NULL ) continue;

		//���Ʒ���
		WORD wFanShuo = m_GameLogic.GetChiHuActionRank( m_ChiHuRight[wWinner] );

		//���Ƹ���
		ASSERT( wFanShuo > 0 );
		if( wFanShuo == 0 ) return ;

		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( i != wWinner )
			{
				LONGLONG lGameScore = wFanShuo;

				//���ڰ�����
				if( wWinner != m_wProvideUser ) lScore[m_wProvideUser] += -lGameScore;
				else lScore[i] += -lGameScore;

				lScore[wWinner] += lGameScore;
			}
		}
	}

	return ;
}

//Ȩλ����
void CTableFrameSink::FiltrateRight( WORD wWinner, CChiHuRight &chr )
{
	/*	����Ȩλ	*/

	//����
	if( wWinner == m_wProvideUser )
		chr |= CHR_ZI_MO;
}

//�Ƿ�����
bool CTableFrameSink::IsCheatUser(DWORD dwUserID)
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\CheatUserConfig.ini"),szPath);

	//��������
	TCHAR szSubItem[MAX_PATH]=TEXT("");
	_sntprintf(szSubItem,CountArray(szSubItem),TEXT("%d"),dwUserID);

	//��ȡ�û�
	bool bFindUser=GetPrivateProfileInt(TEXT("UserList"),szSubItem,0,szFileName);

	return bFindUser;
}
//////////////////////////////////////////////////////////////////////////