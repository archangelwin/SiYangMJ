#include "StdAfx.h"
#include "TableFrameSink.h"

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_lSiceCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	ZeroMemory(m_lGangPaiScore,sizeof(m_lGangPaiScore));
	m_HistoryScore.ResetData();

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//堆立信息
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
#endif

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 0;
	m_cbLianZhuangCount = 0;

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//服务控制
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

//析构函数
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

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//配置桌子
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//错误判断
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("游戏桌子 CTableFrameSink 查询 ITableFrame 接口失败"),TraceLevel_Exception);
		return false;
	}
	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_lSiceCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	ZeroMemory(m_lGangPaiScore,sizeof(m_lGangPaiScore));

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//堆立信息
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
#endif

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 0;

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	return;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	//混乱扑克
	m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),0);

	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));	

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	if( m_wBankerUser == INVALID_CHAIR )
	{
		m_wBankerUser = rand()%GAME_PLAYER;
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//设置变量
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//堆立信息
	WORD wSice = WORD(m_lSiceCount&0xffff);
#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//计算数目
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//提取扑克
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//完成判断
		if (cbTakeCount==0)
		{
			m_wHeapHead=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
			break;
		}
		//切换索引
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}
#endif

	//杠牌判断
	tagGangCardResult GangCardResult;
	m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

	//胡牌判断
	CChiHuRight chr;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
	m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr);
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//构造数据
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

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//设置变量
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

		//防作弊
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

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = INVALID_CHAIR;

			//结束信息
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
				//权位过滤
				if( m_dwChiHuKind[i] == WIK_CHI_HU )
				{
					FiltrateRight( i,m_ChiHuRight[i] );
					m_ChiHuRight[i].GetRightData( GameEnd.dwChiHuRight[i],MAX_RIGHT_COUNT );
				}
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//统计积分
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//结束信息
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
				CopyMemory(GameEnd.lGangScore,m_lGangPaiScore,sizeof(m_lGangPaiScore));

				CalScore( GameEnd.lGameScore );
			}
			else
			{
				//流局结束
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
			}

			//统计积分
			LONGLONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//杠牌分
				GameEnd.lGameScore[i] += m_lGameScore[i];
				//累加积分
				GameEnd.lGameScore[i]*=m_pGameServiceOption->lCellScore;
				//设置积分
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
			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE  ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;
				
				ScoreInfo[i].cbType = ScoreKind;
				ScoreInfo[i].lRevenue=lGameTax[i];
				ScoreInfo[i].lScore = GameEnd.lGameScore[i];

				//发送信息
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//历史积分
				m_HistoryScore.OnEventUserScore(i, GameEnd.lGameScore[i]);
			}
			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//设置变量
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

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.wLeftUser = INVALID_CHAIR;

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;
			GameEnd.wLeftUser = wChairID;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//积分
			GameEnd.lGameScore[wChairID] = -1L*m_pGameServiceOption->lCellScore;

			//通知消息
#ifndef DEBUG
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("由于 [ %s ] 离开游戏，游戏结束"),pIServerUserItem->GetNickName());
			m_pITableFrame->SendGameMessage(szMessage,SMT_CHAT);
#endif
			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//写入积分
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
				//历史积分
				m_HistoryScore.OnEventUserScore(i, GameEnd.lGameScore[i]);
			}
			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//历史积分
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_SCENE_PLAY:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChairID]==false)?m_cbUserAction[wChairID]:WIK_NULL;

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			//堆立信息
			StatusPlay.wHeapHead = m_wHeapHead;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
#endif

			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],StatusPlay.cbCardData);
			StatusPlay.cbSendCardData=(m_wProvideUser==wChairID)?m_cbProvideCard:0x00;

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//错误断言
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
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

				//权限判断
				if (CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;

				// 获取信息
				const CMD_C_AdminReq* AdminReq = static_cast<const CMD_C_AdminReq*>(pData);

				//请求剩余牌
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

					//移除手牌
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

								//添加手牌
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

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (!bLookonUser)
	{
		m_HistoryScore.OnEventUserEnter(wChairID);
	}
	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
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

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

	//错误断言
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置变量
	m_bSendStatus=true;
	m_bGangStatus = false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//出牌记录
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//用户切换
	m_wCurrentUser=(wChairID+m_wPlayerCount+1)%m_wPlayerCount;

	//响应判断
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//派发扑克
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY)
		return true;

	//效验用户 注意：机器人有可能发生此断言
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return true;

	//被动动作
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//效验状态
		//ASSERT(m_bResponse[wChairID]==false);
		//ASSERT( m_cbUserAction[wChairID] != WIK_NULL );
		//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//效验状态
		if (m_bResponse[wChairID]==true) return true;
		if( m_cbUserAction[wChairID] == WIK_NULL ) return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

		//变量定义
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//设置变量
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		if(cbOperateCard[0]==0)
			m_cbOperateCard[wChairID][0] = m_cbProvideCard;
		else CopyMemory( m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]) );

		//执行判断
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//获取动作
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//优先级别
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//动作判断
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//吃胡等待
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//放弃操作
		if (cbTargetAction==WIK_NULL)
		{
			//用户状态
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//发送扑克
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//变量定义
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser][0];

		//出牌变量
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//胡牌操作
		if (cbTargetAction==WIK_CHI_HU)
		{
			//结束信息
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+m_wPlayerCount-1)%GAME_PLAYER)
			{
				//过虑判断
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//胡牌判断
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i]);

				//插入扑克
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					wTargetUser = i;
				}
			}

			//结束游戏
			ASSERT(m_dwChiHuKind[wTargetUser]!=WIK_NULL);
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//组合扑克
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

		//删除扑克
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_CENTER:	//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_PENG:		//碰牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_GANG:		//杠牌操作
			{
				//删除扑克,被动动作只存在放杠
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

		//构造结果
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

		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//如果非杠牌
		if( cbTargetAction != WIK_GANG )
		{
			tagGangCardResult gcr;
			m_cbUserAction[wTargetUser] |= m_GameLogic.AnalyseGangCard( m_cbCardIndex[wTargetUser],m_WeaveItemArray[wTargetUser],m_cbWeaveItemCount[wTargetUser],
				gcr );
			OperateResult.cbActionMask |= m_cbUserAction[wTargetUser];
		}

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//设置用户
		m_wCurrentUser=wTargetUser;

		//杠牌处理
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus = true;
			//杠牌得分
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

	//主动动作
	if (m_wCurrentUser==wChairID)
	{
		//效验操作
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return false;

		//扑克效验
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false)) 
			return false;

		//设置变量
		m_bSendStatus=true;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_GANG:			//杠牌操作
			{
				//变量定义
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

				//杠牌处理
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//寻找组合
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

					//效验动作
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//组合扑克
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];

					//杠牌得分
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
					//扑克效验
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//设置变量
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					for( BYTE j = 0; j < 4; j++ ) m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];

					//杠牌得分
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

				//删除扑克
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;

				//构造结果
				CMD_S_OperateResult OperateResult;
				ZeroMemory( &OperateResult,sizeof(OperateResult) );
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard[0]=cbOperateCard[0];

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//效验动作
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);

				//发送扑克
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}

				return true;
			}
		case WIK_CHI_HU:		//吃胡操作
			{
				//吃牌权位
				if (m_cbOutCardCount==0)
				{
					m_wProvideUser = m_wCurrentUser;
					m_cbProvideCard = m_cbSendCardData;
				}

				//普通胡牌
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

				//结束信息
				m_cbChiHuCard=m_cbProvideCard;

				//结束游戏
				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送提示
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;
	ASSERT( m_bSendStatus );
	if( m_bSendStatus == false ) return false;

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//荒庄结束
	if (m_cbLeftCardCount==m_cbEndLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//杠牌得分
	if( m_bGangStatus )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
			m_lGameScore[i] += m_lGangScore[i];
		ZeroMemory( m_lGangScore,sizeof(m_lGangScore) );
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];

	//设置变量
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;
	m_wProvideUser=wCurrentUser;
	m_cbProvideCard=m_cbSendCardData;

	//加牌
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	if( !m_bEnjoinChiHu[m_wCurrentUser] )
	{
		//胡牌判断
		CChiHuRight chr;
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
		m_cbUserAction[m_wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
			m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr);
		m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
	}

	//杠牌判断
	if ((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
	{
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
			m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
	}

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
	//堆立信息
	ASSERT( m_wHeapHead != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHead][0]+m_cbHeapCardInfo[m_wHeapHead][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHead=(m_wHeapHead+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHead][0]++;
	}
	else
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}
#endif

	//构造数据
	CMD_S_SendCard SendCard;
	SendCard.wSendCardUser = wCurrentUser;
	SendCard.wCurrentUser=m_wCurrentUser;
	SendCard.bTail = bTail;
	SendCard.cbLeftCardCount = m_cbLeftCardCount;
	SendCard.cbActionMask=m_cbUserAction[m_wCurrentUser];
	SendCard.cbCardData=(m_wCurrentUser==wCurrentUser)?m_cbProvideCard:m_cbSendCardData;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction=false;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//动作判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户过滤
		if (wCenterUser==i) continue;

		//出牌类型
		if (EstimatKind==EstimatKind_OutCard)
		{
			//吃碰判断
			if (m_bEnjoinChiPeng[i]==false)
			{
				//碰牌判断
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//吃牌判断
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				//杠牌判断
				if (m_cbLeftCardCount>m_cbEndLeftCount) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
		}

		//胡牌判断
		if (m_bEnjoinChiHu[i]==false)	
		{
			//吃胡判断
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr);
			m_cbUserAction[i] |= cbAction;
		}

		//结果判断
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}

	//结果处理
	if (bAroseAction==true) 
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//发送提示
		SendOperateNotify();

		return true;
	}

	return false;
}

//算分
void CTableFrameSink::CalScore( LONGLONG lScore[GAME_PLAYER] )
{
	//初始化
	ZeroMemory( lScore,sizeof(LONGLONG)*GAME_PLAYER );

	for( WORD wWinner = 0; wWinner < GAME_PLAYER; wWinner++ )
	{
		if( m_dwChiHuKind[wWinner]==WIK_NULL ) continue;

		//胡牌番数
		WORD wFanShuo = m_GameLogic.GetChiHuActionRank( m_ChiHuRight[wWinner] );

		//花牌个数
		ASSERT( wFanShuo > 0 );
		if( wFanShuo == 0 ) return ;

		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( i != wWinner )
			{
				LONGLONG lGameScore = wFanShuo;

				//点炮包三家
				if( wWinner != m_wProvideUser ) lScore[m_wProvideUser] += -lGameScore;
				else lScore[i] += -lGameScore;

				lScore[wWinner] += lGameScore;
			}
		}
	}

	return ;
}

//权位过滤
void CTableFrameSink::FiltrateRight( WORD wWinner, CChiHuRight &chr )
{
	/*	增加权位	*/

	//自摸
	if( wWinner == m_wProvideUser )
		chr |= CHR_ZI_MO;
}

//是否作弊
bool CTableFrameSink::IsCheatUser(DWORD dwUserID)
{
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\CheatUserConfig.ini"),szPath);

	//定义子项
	TCHAR szSubItem[MAX_PATH]=TEXT("");
	_sntprintf(szSubItem,CountArray(szSubItem),TEXT("%d"),dwUserID);

	//读取用户
	bool bFindUser=GetPrivateProfileInt(TEXT("UserList"),szSubItem,0,szFileName);

	return bFindUser;
}
//////////////////////////////////////////////////////////////////////////