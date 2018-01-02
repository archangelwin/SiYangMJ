#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////

#define IDI_CHECK_TABLE				1					//检查桌子
#define IDI_START_GAME				2									//开始定时器
#define IDI_OUT_CARD				3									//出牌定时器
#define IDI_OPERATE_CARD			4									//操作定时器
#define IDT_FLOWER					5									//操作定时器
#define IDT_FLOWER_START			6									//操作定时器


#define TIME_FLOWER					1200		//补花前停顿时间
#define TIME_CHECK_TABLE			3000		//检查桌子停顿时间
#define TIME_OUT_CARD				1000		//检查桌子停顿时间

#define  ZAN_ADD					1			// 庄家加一占
//构造函数
CTableFrameSink::CTableFrameSink()
{
	m_wPlayerCount = GAME_PLAYER;

	//组件变量
	m_pITableFrame = NULL;
	m_pGameCustomRule = NULL;
	m_pGameServiceOption = NULL;
	m_pGameServiceAttrib = NULL;	

	PersonalTableEnd();
	RepositionSink();
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{

}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_wSiceCount = 0;
	m_wCurrentUser = INVALID_CHAIR;
	m_cbMagicIndex = MAX_INDEX;
	m_cbMinusHeadCount = 0;
	m_cbMinusLastCount = 0;
	m_cbLeftCardCount = MAX_REPERTORY;
	m_cbEndLeftCount = 0;
	m_cbSendCardCount = 0;

	ZeroMemory(m_bTing, sizeof(m_bTing));
	ZeroMemory(m_bTrustee, sizeof(m_bTrustee));

	//堆立信息
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo, sizeof(m_cbHeapCardInfo));

	//运行变量
	m_cbProvideCard = 0;
	m_wResumeUser = INVALID_CHAIR;
	m_wProvideUser = INVALID_CHAIR;

	//状态变量
	m_bGangOutCard = false;
	m_enSendStatus = Not_Send;
	m_cbGangStatus = WIK_GANERAL;
	m_wProvideGangUser = INVALID_CHAIR;
	ZeroMemory(m_bEnjoinChiHu, sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng, sizeof(m_bEnjoinChiPeng));	
	ZeroMemory(m_bEnjoinGang, sizeof(m_bEnjoinGang));
	ZeroMemory(m_cbChiPengCount, sizeof(m_cbChiPengCount));
	ZeroMemory(m_bGangCard, sizeof(m_bGangCard));	
	ZeroMemory(m_cbGangCount, sizeof(m_cbGangCount));

	for(int i = 0; i < GAME_PLAYER; i++)
	{
		m_vecEnjoinChiHu[i].clear();
		m_vecEnjoinChiPeng[i].clear();	
	}

	//用户状态
	ZeroMemory(m_bResponse, sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray, sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount, sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind, sizeof(m_dwChiHuKind));
	
	//出牌信息
	m_cbOutCardData = 0;
	m_cbOutCardCount = 0;
	m_wOutCardUser = INVALID_CHAIR;

	ZeroMemory(m_cbDiscardCard, sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount, sizeof(m_cbDiscardCount));

	//扑克信息
	ZeroMemory(m_cbCardIndex, sizeof(m_cbCardIndex));
	ZeroMemory(m_cbHandCardCount, sizeof(m_cbHandCardCount));
	ZeroMemory(m_cbUserCatchCardCount,sizeof(m_cbUserCatchCardCount));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_GangScore, sizeof(m_GangScore));
	ZeroMemory(m_lUserGangScore,sizeof(m_lUserGangScore));
	ZeroMemory(m_lZuiScore, sizeof(m_lZuiScore));
	ZeroMemory(m_lDetailScore, sizeof(m_lDetailScore));
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));

	m_wLastCatchCardUser=INVALID_CHAIR;
	m_wLastBankerUser = INVALID_CHAIR;
	m_wLastBankerUserTemp = INVALID_CHAIR;

	ZeroMemory(m_cbHuCardCount,sizeof(m_cbHuCardCount));
	ZeroMemory(m_cbHuCardData,sizeof(m_cbHuCardData));
	ZeroMemory(m_cbUserMaCount, sizeof(m_cbUserMaCount));

	
	ZeroMemory(m_bNotFirstRepairFlowerRound, sizeof(m_bNotFirstRepairFlowerRound));
	ZeroMemory(m_cbHandFlowerCard, sizeof(m_cbHandFlowerCard));
	ZeroMemory(m_cbFlowerCount, sizeof(m_cbFlowerCount));
	ZeroMemory(m_cbTatalFlowerCount, sizeof(m_cbTatalFlowerCount));
	for (BYTE i = 0; i < GAME_PLAYER;++i)
	{
		m_wWinUser[i] = INVALID_CHAIR;
	}
	m_cbWinCount = 0;
	
	return;
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

	//游戏配置
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//自定规则
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	m_cbMaxZui = m_pGameCustomRule->cbMaCount;
	m_cbPlayerCount = m_pGameCustomRule->cbPlayerCount;

	ZeroMemory(&m_stRecord,sizeof(m_stRecord));
	//开始模式
	if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) !=0 )//房卡模式
		m_pITableFrame->SetStartMode(START_MODE_FULL_READY);
	else
		m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	return true;
}

//消费能力
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//最少积分
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//金币游戏
	if (m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|SCORE_GENRE_POSITIVE))
	{		
		return 64 * m_pITableFrame->GetCellScore();
	}
	return 0;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
	if (!m_bChooseZui && m_cbMaxZui > 0)
	{
		m_bChooseZui = true;
		m_pITableFrame->SetGameStatus(GAME_SCENE_CHOOSE);
		//构造变量
		ZeroMemory(&m_cbChooseZuiInfo, sizeof(m_cbChooseZuiInfo));
		ZeroMemory(&m_cbChooseZuiStatus, sizeof(m_cbChooseZuiStatus));
		CMD_S_GameChoose GameChoose;
		ZeroMemory(&GameChoose, sizeof(GameChoose));
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_CHOOSE, &GameChoose, sizeof(GameChoose));
		return true;
	}

	return startPlay();
}

bool CTableFrameSink::startPlay()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	//随机扑克
	BYTE Sice1 = rand() % 6 + 1;
	BYTE Sice2 = rand() % 6 + 1;
	BYTE minSice = min(Sice1, Sice2);
	m_wSiceCount = MAKEWORD(Sice1, Sice2);
	m_cbLeftCardCount = MAX_REPERTORY;
	m_cbSendCardCount = 0;
	m_bUserActionDone = false;
	m_enSendStatus = Not_Send;
	m_cbGangStatus = WIK_GANERAL;
	m_wProvideGangUser = INVALID_CHAIR;

	m_GameLogic.RandCardList(m_cbRepertoryCard, CountArray(m_cbRepertoryCard));

// #ifdef  CARD_DISPATCHER_CONTROL
// 
// 	TCHAR szPath[MAX_PATH] = TEXT("");
// 	GetCurrentDirectory(sizeof(szPath), szPath);
// 	CString strSaveFileName;
// 	strSaveFileName.Format(TEXT("%s\\SparrowHZData.dat"), szPath);
// 	//非控制状态
// 	if (m_cbControlGameCount == 0)
// 	{
// 		CFile file;
// 		if (file.Open(strSaveFileName, CFile::modeRead))
// 		{
// 			file.Read(&m_wBankerUser, sizeof(m_wBankerUser));
// 			file.Read(m_cbRepertoryCard, sizeof(m_cbRepertoryCard));
// 			file.Close();
// 		}
// 	}
// 	else
// 	{
// 		m_wBankerUser = m_wControBankerUser;
// 		CopyMemory(m_cbRepertoryCard, m_cbControlRepertoryCard, sizeof(m_cbRepertoryCard));
// 		m_cbControlGameCount--;
// 	}
// 
// #endif

	//分发扑克
	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		if (m_pITableFrame->GetTableUserItem(i) == NULL)
			continue;

		m_bPlayStatus[i] = true;
		m_cbLeftCardCount -= (MAX_HAND_COUNT - 1);
		m_cbMinusHeadCount += (MAX_HAND_COUNT - 1);
		m_cbHandCardCount[i] = (MAX_HAND_COUNT - 1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount], MAX_HAND_COUNT - 1, m_cbCardIndex[i]);
	}

	if (m_wBankerUser == INVALID_CHAIR && (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) != 0)//房卡模式下先把庄家给房主
	{
		DWORD OwnerId = m_pITableFrame->GetTableOwner();
		IServerUserItem *pOwnerItem = m_pITableFrame->SearchUserItem(OwnerId);
		if ( pOwnerItem && pOwnerItem->GetChairID() != INVALID_CHAIR)
			m_wBankerUser = pOwnerItem->GetChairID();
		else
			m_wBankerUser = INVALID_CHAIR;
	}
	//确定庄家
	if (m_wBankerUser == INVALID_CHAIR || m_bPlayStatus[m_wBankerUser] == false)
	{
		m_wBankerUser = rand() % m_cbPlayerCount;
		while (m_pITableFrame->GetTableUserItem(m_wBankerUser) == NULL)
		{
			m_wBankerUser = (m_wBankerUser + 1) % m_cbPlayerCount;
		}
	}

	//发送扑克
	m_cbMinusHeadCount++;
	m_cbSendCardData = m_cbRepertoryCard[--m_cbLeftCardCount];
	// m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
	m_cbUserCatchCardCount[m_wBankerUser]++;
	//设置变量
	m_cbProvideCard = m_cbSendCardData;
	m_wProvideUser = m_wBankerUser;
	m_wCurrentUser = m_wBankerUser;

	//堆立信息
	WORD wSiceCount = LOBYTE(m_wSiceCount) + HIBYTE(m_wSiceCount);
	WORD wTakeChairID = (m_wBankerUser + wSiceCount - 1) % GAME_PLAYER;
	BYTE cbTakeCount = MAX_REPERTORY - m_cbLeftCardCount;
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		//计算数目
		BYTE cbValidCount = HEAP_FULL_COUNT - m_cbHeapCardInfo[wTakeChairID][1] - ((i == 0) ? (minSice)* 2 : 0);
		BYTE cbRemoveCount = __min(cbValidCount, cbTakeCount);

		//提取扑克
		cbTakeCount -= cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i == 0) ? 1 : 0] += cbRemoveCount;

		//完成判断
		if (cbTakeCount == 0)
		{
			m_wHeapHead = wTakeChairID;
			m_wHeapTail = (m_wBankerUser + wSiceCount - 1) % GAME_PLAYER;
			break;
		}
		//切换索引
		wTakeChairID = (wTakeChairID + GAME_PLAYER - 1) % GAME_PLAYER;
	}

	//操作分析
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));

	//听牌判断
	BYTE cbCount = 0;
	BYTE cbOutCard[MAX_HAND_COUNT][28] = { 0 };
	

	//构造变量
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart, sizeof(GameStart));
	GameStart.wBankerUser = m_wBankerUser;
	GameStart.wSiceCount = m_wSiceCount;
	GameStart.wHeapHead = m_wHeapHead;
	GameStart.wHeapTail = m_wHeapTail;
	GameStart.cbMagicIndex = m_cbMagicIndex;
	CopyMemory(GameStart.cbHeapCardInfo, m_cbHeapCardInfo, sizeof(GameStart.cbHeapCardInfo));
	CopyMemory(GameStart.cbZuiInfo, m_cbChooseZuiInfo, sizeof(GameStart.cbZuiInfo));

	//发送数据
	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		if (m_pITableFrame->GetTableUserItem(i) == NULL)
			continue;
		GameStart.cbUserAction = m_cbUserAction[i];
		ZeroMemory(GameStart.cbCardData, sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i], GameStart.cbCardData);

		GameStart.cbOutCardCount = 0;
		ZeroMemory(GameStart.cbOutCardData, sizeof(GameStart.cbOutCardData));
		if (i == m_wBankerUser )
		{
			GameStart.cbCardData[MAX_HAND_COUNT - 1] = m_cbSendCardData;
			if (cbCount > 0)
			{
				GameStart.cbOutCardCount = cbCount;
				CopyMemory(GameStart.cbOutCardData, cbOutCard, sizeof(cbOutCard));
			}

		}

		logCardData(i);
		m_pITableFrame->SendTableData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
		m_pITableFrame->SendLookonData(i, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
	}

	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH) != 0)
	{
		m_pITableFrame->SetGameTimer(IDI_CHECK_TABLE, TIME_CHECK_TABLE, -1, NULL);
		m_cbWaitTime = 0;
	}


	if (m_GameLogic.IsHasFlowerCard(m_cbCardIndex[m_wBankerUser]))
	{
		m_cbSendCardData = IsShowFlower(m_wBankerUser);
	}

	EstimateUserOpertion(m_wBankerUser, true);
	if (m_cbUserAction[m_wBankerUser] != WIK_NULL)
	{
		SendOperateNotify(m_wBankerUser);
	}
	else
	{
		int a = 1;
		a = 3;
	}
	return true;
}

// 返回0代表牌完了
BYTE CTableFrameSink::IsShowFlower(WORD chairId)
{
	CMD_S_SHOW_FLOWER nShowFlower;
	ZeroMemory(&nShowFlower, sizeof(nShowFlower));
	WORD wChairId = chairId % GAME_PLAYER;
	BYTE cbCount = 0;

	BYTE lastSendCard = m_cbSendCardData;
	m_bNotFirstRepairFlowerRound[wChairId] = true;

	string fowerCardStr, repairCardStr;
	bool isRepairFlower = true;
	while (isRepairFlower)
	{
		isRepairFlower = false;
		for (int k = FLOWER_INDEX; k < MAX_INDEX; k++)
		{
			BYTE flowerCnt = m_cbCardIndex[wChairId][k];
			if (flowerCnt > 0)
			{
				isRepairFlower = true;
				for (int tempFlowerCnt = 0; tempFlowerCnt < flowerCnt; ++tempFlowerCnt)
				{
					BYTE flowerCard = m_GameLogic.SwitchToCardData(k);
					nShowFlower.cbCardData[cbCount] = flowerCard;
					if (m_cbFlowerCount[wChairId] >= MAX_FLOWER_CNT)
					{
						int a = 2;
						a = 3;
					}
					m_cbHandFlowerCard[wChairId][m_cbFlowerCount[wChairId]++] = flowerCard;
					if (m_cbFlowerCount[wChairId] >= MAX_FLOWER_CNT)
					{
						int a = 2;
						a = 3;
					}
					m_cbTatalFlowerCount[wChairId]++;
					fowerCardStr += " " + to_string(flowerCard);

					auto repairCard = GetSendCard(true);
					if (repairCard == 0)
					{
						return 0;
					}

					lastSendCard = repairCard;
					repairCardStr += " " + to_string(repairCard);
					nShowFlower.cbGetCardData[cbCount] = repairCard;
					cbCount++;
					auto repairIndex = m_GameLogic.SwitchToCardIndex(repairCard);
					m_cbCardIndex[wChairId][repairIndex]++;
					m_cbCardIndex[wChairId][k]--;
				}
			}
		}
	}
	
	if (cbCount > 0)
	{
		nShowFlower.wChairID = wChairId;
		//发送补花
		m_pITableFrame->SendTableData(wChairId, SUB_S_SHOW_FLOWER, &nShowFlower, sizeof(nShowFlower));

		//隐藏补进的牌
		ZeroMemory(&(nShowFlower.cbGetCardData), sizeof(nShowFlower.cbGetCardData));

		//发送给其他玩家
		//分发扑克
		for (WORD m = 0; m < m_cbPlayerCount; m++)
		{
			if (m_pITableFrame->GetTableUserItem(m) == NULL)
				continue;
			if (m == wChairId) continue;

			m_pITableFrame->SendTableData(m, SUB_S_SHOW_FLOWER, &nShowFlower, sizeof(nShowFlower));
		}

		m_cbGangStatus = WIK_AN_GANG;
		string str = "player: " + std::to_string(wChairId) + " BuHua: \n" ;
		str += "fowerCardStr " + fowerCardStr + "\n";
		str += "repairCardStr " + repairCardStr + "\n";

		int a = 1;

		for (int  i = 0; i < GAME_PLAYER;  ++i)
		{

		}
		string nShowFlowerFlowerStr, nShowFlowerRepairCard;

		for (int i = 0; i < nShowFlower.cbCardCount; ++i)
		{
			str = "player: " + std::to_string(i) + " huaCnt " + std::to_string(m_cbFlowerCount[i]) + "\n";
			for (int j = 0; j < MAX_FLOWER_CNT; ++j)
			{
				str += " " + m_cbHandFlowerCard[i][j];
			}
		}


		_bstr_t bstrTmp(str.c_str());
		LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
		// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);

		CString strInfo(pszDescribeString);
		// 		CString strInfo;
		// 		strInfo.Format(TEXT("[%s]"), str.c_str());
		CGameLogic::NcaTextOut(strInfo);

		return lastSendCard;
	}

	return lastSendCard;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	// add by yangyunlong 2017-9-21
// 	if(NULL == pIServerUserItem)
// 		return true;
	///////////////////////
	if((m_pGameServiceOption->wServerType & GAME_GENRE_MATCH) != 0)
		m_pITableFrame->KillGameTimer(IDI_CHECK_TABLE);
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude, sizeof(GameConclude));
			GameConclude.cbSendCardData = m_cbSendCardData;
			GameConclude.wLeftUser = INVALID_CHAIR;			

			//结束信息
			for (WORD i = 0; i < m_cbPlayerCount; i++)
			{
				if (m_pITableFrame->GetTableUserItem(i) == NULL)
					continue;

				if (!m_bPlayStatus[i])
					continue;

				GameConclude.dwChiHuKind[i] = m_dwChiHuKind[i];
				//权位过滤
				if(m_dwChiHuKind[i] == WIK_CHI_HU)
				{
					FiltrateRight(i, m_ChiHuRight[i]);
					m_ChiHuRight[i].GetRightData(GameConclude.dwChiHuRight[i], MAX_RIGHT_COUNT);
				}
				GameConclude.cbCardCount[i] = m_GameLogic.SwitchToCardData(m_cbCardIndex[i], GameConclude.cbHandCardData[i]);
			}

			//计算胡牌输赢分
			SCORE lUserGameScore[GAME_PLAYER]={0};
			CalHuPaiScore(lUserGameScore);

			SCORE lUserZuiScore[GAME_PLAYER] = { 0 };
			// if (m_cbMaxZui > 0)
			CalZuiScore(lUserZuiScore);

			CopyMemory(GameConclude.cbWeaveItemCount, m_cbWeaveItemCount, sizeof(m_cbWeaveItemCount));
			CopyMemory(GameConclude.WeaveItemArray, m_WeaveItemArray, sizeof(m_WeaveItemArray));

			//拷贝码数据
			CopyMemory(GameConclude.cbHandFlowerCard, m_cbHandFlowerCard, sizeof(m_cbHandFlowerCard));
			CopyMemory(GameConclude.cbFlowerCount, m_cbFlowerCount, sizeof(m_cbFlowerCount));
			CopyMemory(GameConclude.cbChooseZuiInfo, m_cbChooseZuiInfo,sizeof(m_cbChooseZuiInfo));
			CopyMemory(GameConclude.cbTotalFlowerCount, m_cbTatalFlowerCount, sizeof(m_cbTatalFlowerCount));
			CopyMemory(GameConclude.lZuiScore, m_lZuiScore, sizeof(m_lZuiScore));
			
			for (int i = 0; i < m_cbWinCount; i++)
			{
				WORD winUser = m_wWinUser[i];
				WORD sanBaoUser = GetSanBaoUser(winUser, m_ChiHuRight[winUser]);
				if (sanBaoUser == INVALID_CHAIR)
				{
					continue;
				}
			
				m_lDetailScore[winUser][sanBaoUser] = -m_lDetailScore[winUser][winUser];
				for (int j = 0; j < m_cbPlayerCount; ++j)
				{
					if (j == winUser)
					{
						continue;
					}

					if (j == sanBaoUser)
					{
						continue;
					}

					m_lDetailScore[winUser][j] = 0;
				}
			}

			SCORE lGameScore[GAME_PLAYER] = { 0 };
			for (int i = 0; i < m_cbPlayerCount; ++i)
			{
				for (int j = 0; j < m_cbPlayerCount; ++j)
				{
					lGameScore[i] += m_lDetailScore[j][i];
				}
			}
			

			//积分变量
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));

			GameConclude.wProvideUser = m_wProvideUser;
			GameConclude.cbProvideCard = m_cbProvideCard;

			//统计积分
			for (WORD i = 0; i < m_cbPlayerCount; i++)
			{
				if (m_pITableFrame->GetTableUserItem(i) == NULL)
					continue;

				if(!m_bPlayStatus[i])
					continue;

				GameConclude.lGameScore[i] = lGameScore[i];
				//胡牌分算完后再加上杠的输赢分就是玩家本轮最终输赢分
				// GameConclude.lGameScore[i] += lUserZuiScore[i];
				// GameConclude.lGangScore[i] = lUserZuiScore[i];

				//收税
				if (GameConclude.lGameScore[i]>0 && (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0)
				{
					GameConclude.lRevenue[i] = m_pITableFrame->CalculateRevenue(i,GameConclude.lGameScore[i]);
					GameConclude.lGameScore[i] -= GameConclude.lRevenue[i];
				}

				ScoreInfoArray[i].lRevenue = GameConclude.lRevenue[i];
				ScoreInfoArray[i].lScore = GameConclude.lGameScore[i];
				ScoreInfoArray[i].cbType = ScoreInfoArray[i].lScore > 0 ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;

				//历史积分
				m_HistoryScore.OnEventUserScore(i, GameConclude.lGameScore[i]);
				if(m_stRecord.nCount<32)
				{
					m_stRecord.lDetailScore[i][m_stRecord.nCount]=GameConclude.lGameScore[i];
					m_stRecord.lAllScore[i] += GameConclude.lGameScore[i];
				}
			}
			m_stRecord.nCount++;

			if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) !=0 )//房卡模式
			{
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_RECORD,&m_stRecord,sizeof(m_stRecord));
			}

			//发送数据
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_CONCLUDE, &GameConclude, sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_CONCLUDE, &GameConclude, sizeof(GameConclude));

			logConclude(GameConclude);


			//写入积分
			m_pITableFrame->WriteTableScore(ScoreInfoArray, m_cbPlayerCount);

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			// ZeroMemory(&m_stRecord, sizeof(m_stRecord));

			if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) !=0 )//房卡模式
			{
				if(m_pITableFrame->IsPersonalRoomDisumme() )//当前朋友局解散清理记录
				{
					ZeroMemory(&m_stRecord,sizeof(m_stRecord));
				}
			}
			
			return true;
		}
	case GER_NETWORK_ERROR:	//网络中断
		{
			return true;
		}
	case GER_USER_LEAVE:	//用户强退
		{
			if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) != 0 )//房卡模式
				return true;

			//自动托管
			OnUserTrustee(wChairID,true);

			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameConclude GameConclude;
			ZeroMemory(&GameConclude, sizeof(GameConclude));

			m_wBankerUser = INVALID_CHAIR;

			GameConclude.cbSendCardData = m_cbSendCardData;

			//用户扑克
			BYTE cbCardIndex=0;
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				GameConclude.cbCardCount[i] = m_GameLogic.SwitchToCardData(m_cbCardIndex[i], GameConclude.cbHandCardData[i]);
			}


			CopyMemory(GameConclude.cbWeaveItemCount, m_cbWeaveItemCount, sizeof(m_cbWeaveItemCount));
			CopyMemory(GameConclude.WeaveItemArray, m_WeaveItemArray, sizeof(m_WeaveItemArray));

			//拷贝码数据
			CopyMemory(GameConclude.cbHandFlowerCard, m_cbHandFlowerCard, sizeof(m_cbHandFlowerCard));
			CopyMemory(GameConclude.cbFlowerCount, m_cbFlowerCount, sizeof(m_cbFlowerCount));
			CopyMemory(GameConclude.cbChooseZuiInfo, m_cbChooseZuiInfo, sizeof(m_cbChooseZuiInfo));
			CopyMemory(GameConclude.cbTotalFlowerCount, m_cbTatalFlowerCount, sizeof(m_cbTatalFlowerCount));
			CopyMemory(GameConclude.lZuiScore, m_lZuiScore, sizeof(m_lZuiScore));


			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_CONCLUDE, &GameConclude, sizeof(GameConclude));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_CONCLUDE, &GameConclude, sizeof(GameConclude));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) !=0 )//房卡模式
			{
				if(m_pITableFrame->IsPersonalRoomDisumme() )//当前朋友局解散清理记录
				{
					ZeroMemory(&m_stRecord,sizeof(m_stRecord));
				}
			}
			
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

void CTableFrameSink::logConclude(const CMD_S_GameConclude &GameConclude)
{
	string str = "CMD_S_GameConclude: \n";
	str += "lCellScore: " + std::to_string(GameConclude.lCellScore);
	str += "lGameScore: ";

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		str += " ";
		str += std::to_string(GameConclude.lGameScore[i]);
	}

	str = "\n";
	str += "lRevenue: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		str += " ";
		str += std::to_string(GameConclude.lRevenue[i]);
	}

	str = "\n";
	str += "lGangScore: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		str += " ";
		str += std::to_string(GameConclude.lGangScore[i]);
	}

	str = "\n";
	str += "wProvideUser: ";
	str += std::to_string(GameConclude.wProvideUser);

	str = "\n";
	str += "cbProvideCard: ";
	str += std::to_string(GameConclude.cbProvideCard);

	str = "\n";
	str += "cbSendCardData: ";
	str += std::to_string(GameConclude.cbSendCardData);

	str = "\n";
	str += "dwChiHuKind: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		str += " ";
		str += std::to_string(GameConclude.dwChiHuKind[i]);
	}

	str = "\n";
	str += "dwChiHuRight: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		str += " ";
		str += std::to_string(GameConclude.dwChiHuRight[i][0]);
	}

	str = "\n";
	str += "wLeftUser: ";
	str += std::to_string(GameConclude.wLeftUser);

	str = "\n";
	str += "wLianZhuang: ";
	str += std::to_string(GameConclude.wLianZhuang);


	str = "\n";
	str += "cbCardCount: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		str += " ";
		str += std::to_string(GameConclude.cbCardCount[i]);
	}

	str = "\n";
	str += "cbHandCardData: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		for (int j = 0; j < MAX_HAND_COUNT; ++j)
		{
			str += " ";
			str += std::to_string(GameConclude.cbHandCardData[i][j]);
		}
	}

	str = "\n";
	str += "cbFlowerCount: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		str += " ";
		str += std::to_string(GameConclude.cbFlowerCount[i]);
	}

	str = "\n";
	str += "cbHandFlowerCard: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		for (int j = 0; j < MAX_FLOWER_CNT; ++j)
		{
			str += " ";
			str += std::to_string(GameConclude.cbHandFlowerCard[i][j]);
		}
	}

	str = "\n";
	str += "cbChooseZuiInfo: ";
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		for (int j = 0; j < MAX_ZUI; ++j)
		{
			str += " ";
			str += std::to_string(GameConclude.cbChooseZuiInfo[i][j]);
		}
	}

	_bstr_t bstrTmp(str.c_str());
	LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
	// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);
	CString strInfo(pszDescribeString);
	CGameLogic::NcaTextOut(strInfo);
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree, sizeof(StatusFree));

			//设置变量
			StatusFree.lCellScore = m_pITableFrame->GetCellScore();
			StatusFree.cbMaxZuiCount = m_cbMaxZui;
			StatusFree.cbZuiScore = m_cbScoreZui;
			StatusFree.cbPlayerCount = m_cbPlayerCount;
			//自定规则
			StatusFree.cbTimeOutCard = m_pGameCustomRule->cbTimeOutCard;		
			StatusFree.cbTimeOperateCard = m_pGameCustomRule->cbTimeOperateCard;
			StatusFree.cbTimeStartGame = m_pGameCustomRule->cbTimeStartGame;

			//历史积分
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore = m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusFree.lTurnScore[i] = pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i] = pHistoryScore->lCollectScore;
			}

			if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) !=0 )//房卡模式
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_RECORD,&m_stRecord,sizeof(m_stRecord));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));
		}
	case GAME_SCENE_CHOOSE:	
	{
		//构造数据
		CMD_S_StatusChooseZui StatusChooseZui;
		ZeroMemory(&StatusChooseZui, sizeof(StatusChooseZui));
		StatusChooseZui.cbMaxZuiCount = m_cbMaxZui;
		StatusChooseZui.cbZuiScore = m_cbScoreZui;
		StatusChooseZui.cbPlayerCount = m_cbPlayerCount;
		//发送场景
		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusChooseZui, sizeof(StatusChooseZui));
	}
	case GAME_SCENE_PLAY:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));

			//自定规则
			StatusPlay.cbTimeOutCard = m_pGameCustomRule->cbTimeOutCard;		
			StatusPlay.cbTimeOperateCard = m_pGameCustomRule->cbTimeOperateCard;
			StatusPlay.cbTimeStartGame = m_pGameCustomRule->cbTimeStartGame;
			
			OnUserTrustee(wChairID,false);//重入取消托管

			//规则
			StatusPlay.cbMaxZuiCount = m_cbMaxZui;
			StatusPlay.cbZuiScore = m_cbScoreZui;
			StatusPlay.cbPlayerCount = m_cbPlayerCount;
			//游戏变量
			StatusPlay.wBankerUser = m_wBankerUser;
			StatusPlay.wCurrentUser = m_wCurrentUser;
			StatusPlay.lCellScore = m_pGameServiceOption->lCellScore;
			StatusPlay.cbMagicIndex = m_cbMagicIndex;
			StatusPlay.bIsGuoZhuang = m_bIsGuoZhuang;
			//状态变量
			StatusPlay.cbActionCard = m_cbProvideCard;
			StatusPlay.cbLeftCardCount = m_cbLeftCardCount;
			StatusPlay.cbActionMask = !m_bResponse[wChairID] ? m_cbUserAction[wChairID] : WIK_NULL;
			
			CopyMemory(StatusPlay.bTrustee, m_bTrustee, sizeof(StatusPlay.bTrustee));
			CopyMemory(StatusPlay.bTing, m_bTing, sizeof(StatusPlay.bTing));
			CopyMemory(StatusPlay.cbZuiInfo, m_cbChooseZuiInfo, sizeof(StatusPlay.cbZuiInfo));


			//当前能胡的牌
			StatusPlay.cbOutCardCount = m_GameLogic.AnalyseTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbTatalFlowerCount[wChairID], m_bIsGuoZhuang, StatusPlay.cbOutCardCount, StatusPlay.cbOutCardDataEx, StatusPlay.cbHuCardCount, StatusPlay.cbHuCardData);
			
			//历史记录
			StatusPlay.wOutCardUser = m_wOutCardUser;
			StatusPlay.cbOutCardData = m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard, m_cbDiscardCard, sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount, m_cbDiscardCount, sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray, m_WeaveItemArray, sizeof(StatusPlay.WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveItemCount, m_cbWeaveItemCount, sizeof(StatusPlay.cbWeaveItemCount));

			//堆立信息
			StatusPlay.wHeapHead = m_wHeapHead;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo, m_cbHeapCardInfo, sizeof(m_cbHeapCardInfo));

			//扑克数据
			for(int i = 0; i < GAME_PLAYER; i++)
			{
				StatusPlay.cbCardCount[i] = m_GameLogic.GetCardCount(m_cbCardIndex[i]);
			}
			m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID], StatusPlay.cbCardData);

			CopyMemory(StatusPlay.cbFlowerCount, m_cbFlowerCount, sizeof(StatusPlay.cbFlowerCount));
			CopyMemory(StatusPlay.cbFlowerCardData, m_cbHandFlowerCard, sizeof(StatusPlay.cbFlowerCardData));
			CopyMemory(StatusPlay.cbTatalFlowerCount, m_cbTatalFlowerCount, sizeof(StatusPlay.cbTatalFlowerCount));

			//历史积分
			for (WORD i = 0; i < GAME_PLAYER; i++)
			{
				//变量定义
				tagHistoryScore * pHistoryScore = m_HistoryScore.GetHistoryScore(i);

				//设置变量
				StatusPlay.lTurnScore[i] = pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i] = pHistoryScore->lCollectScore;
			}

			if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) !=0 )//房卡模式
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_RECORD,&m_stRecord,sizeof(m_stRecord));
			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusPlay, sizeof(StatusPlay));
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

void CTableFrameSink::PersonalTableEnd()
{
	//规则变量
	m_cbMaxZui = 0;
	m_cbScoreZui = 2;
	m_cbPlayerCount = 4;

	m_bChooseZui = false;
	m_bIsGuoZhuang = false;
	m_wLastBankerUser = INVALID_CHAIR;
	ZeroMemory(&m_bChooseZui, sizeof(m_bChooseZui));
	ZeroMemory(m_cbChooseZuiStatus, sizeof(m_cbChooseZuiStatus));
	ZeroMemory(m_cbChooseZuiInfo, sizeof(m_cbChooseZuiInfo));
}

//时间事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0)
	{
		if(wTimerID==IDI_CHECK_TABLE)
		{
			m_cbWaitTime++;
			if(m_cbWaitTime>=3)
			{
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
			}
			else if(m_cbWaitTime>=2)
			{
				for(int i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(i);
					if(pServerUserItem!=NULL && pServerUserItem->GetUserStatus()==US_OFFLINE)return true;				
				}
				OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS);
			}
			return true;
		}
	}

	if(wTimerID == IDI_OUT_CARD)
	{
		m_pITableFrame->KillGameTimer(wTimerID);

		BYTE card = m_cbSendCardData;
        if(!m_GameLogic.IsValidCard(card))
		{
			for(int i=0;i<MAX_INDEX;i++)
			{
				if(m_cbCardIndex[m_wCurrentUser][i]>0)
				{
					card = m_GameLogic.SwitchToCardData(i);
					break;
				}
			}
		}

		OnUserOutCard(m_wCurrentUser,card,true);
		return true;
	}
	else if(wTimerID >= IDI_OPERATE_CARD && wTimerID < IDI_OPERATE_CARD+GAME_PLAYER)
	{
		m_pITableFrame->KillGameTimer(wTimerID);

		WORD wChair=wTimerID-IDI_OPERATE_CARD;
		BYTE cbOperateCard[3]={0};
		if(m_cbUserAction[wChair] != WIK_LISTEN)
			OnUserOperateCard(wTimerID-IDI_OPERATE_CARD,WIK_NULL,cbOperateCard);
		else
		{
			OnUserListenCard(wChair,false);
		}
	}

	else if (wTimerID == IDT_FLOWER_START)
	{
		m_pITableFrame->KillGameTimer(wTimerID);

		if (IsShowFlower(m_wCurrentUser) == 0)
		{
			return false;
		}
	}

	return false;
}

//数据事件
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}

//游戏消息
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	m_cbWaitTime = 0;
	switch (wSubCmdID)
	{
	case SUB_C_CHOOSE_ZUI:
	{
		//效验消息
		ASSERT(wDataSize == sizeof(CMD_C_ChooseZuiInfo));
		if (wDataSize != sizeof(CMD_C_ChooseZuiInfo)) return false;

		//用户效验
		ASSERT(pIServerUserItem->GetUserStatus() == US_PLAYING);
		if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

		//消息处理
		CMD_C_ChooseZuiInfo * pChooseZui = (CMD_C_ChooseZuiInfo *)pData;
		return OnUserChooseZui(pIServerUserItem->GetChairID(), pChooseZui->cbZui);
	}
	case SUB_C_OUT_CARD:
		{
			//效验消息
			ASSERT(wDataSize == sizeof(CMD_C_OutCard));
			if (wDataSize != sizeof(CMD_C_OutCard)) return false;

			//用户效验
			ASSERT(pIServerUserItem->GetUserStatus() == US_PLAYING);
			if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

			//消息处理
			CMD_C_OutCard * pOutCard = (CMD_C_OutCard *)pData;
			return OnUserOutCard(pIServerUserItem->GetChairID(), pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize == sizeof(CMD_C_OperateCard));
			if (wDataSize != sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard = (CMD_C_OperateCard *)pData;
			return OnUserOperateCard(pIServerUserItem->GetChairID(), pOperateCard->cbOperateCode, pOperateCard->cbOperateCard);
		}
	case SUB_C_LISTEN_CARD:
		{
			//效验消息
			ASSERT(wDataSize == sizeof(CMD_C_ListenCard));
			if (wDataSize != sizeof(CMD_C_ListenCard)) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

			//消息处理
			CMD_C_ListenCard * pOperateCard = (CMD_C_ListenCard *)pData;
			return OnUserListenCard(pIServerUserItem->GetChairID(), pOperateCard->bListenCard);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pData;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			return OnUserTrustee(pIServerUserItem->GetChairID(), pTrustee->bTrustee);
		}
	case SUB_C_REPLACE_CARD:	//用户补牌
		{
			//效验数据
			ASSERT(wDataSize == sizeof(CMD_C_ReplaceCard));
			if (wDataSize != sizeof(CMD_C_ReplaceCard)) return false;
			//用户效验
			if (pIServerUserItem->GetUserStatus() != US_PLAYING) return true;

			//变量定义
			CMD_C_ReplaceCard * pReplaceCard = (CMD_C_ReplaceCard *)pData;

			//消息处理
			return OnUserReplaceCard(pIServerUserItem->GetChairID(), pReplaceCard->cbCardData);
		}

#ifdef  CARD_DISPATCHER_CONTROL
	case SUB_C_SEND_CARD:
		{
			ASSERT(wDataSize==sizeof(CMD_C_SendCard));
			if(wDataSize!=sizeof(CMD_C_SendCard)) return false;

			CMD_C_SendCard * pSendCard = (CMD_C_SendCard *)pData;

			return OnUserSendCard(pSendCard->cbCardCount, pSendCard->wBankerUser, pSendCard->cbCardData, pSendCard->cbControlGameCount);
		}
#endif

	default:
		break;
	}

	return false;
}

//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//获取房卡规则
	BYTE* pSetInfo = m_pITableFrame->GetGameRule();
	if(pSetInfo[0] == 1)
	{
		m_cbPlayerCount = pSetInfo[1];
		m_cbMaxZui = pSetInfo[4];
		m_cbScoreZui = pSetInfo[5];
		
// 		m_bIsGuoZhuang = false;
// 		m_wLastBankerUser = INVALID_CHAIR;
// 		ZeroMemory(&m_bChooseZui, sizeof(m_bChooseZui));
// 		ZeroMemory(m_cbChooseZuiStatus, sizeof(m_cbChooseZuiStatus));
// 		ZeroMemory(m_cbChooseZuiInfo, sizeof(m_cbChooseZuiInfo));
	}

	//历史积分
	if (bLookonUser==false)
	{
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserEnter(wChairID);
		m_wBankerUser = INVALID_CHAIR;
		m_wLianZhuang = 1;
	}

	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false)
	{
		ASSERT(wChairID!=INVALID_CHAIR);
		m_HistoryScore.OnEventUserLeave(wChairID);
		m_wBankerUser = INVALID_CHAIR;
		m_wLianZhuang = 1;
	}

	if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) !=0 )//房卡模式
	{
		//byte testByte;
		//m_pITableFrame->GetSpeicalInfo(&testByte, 1);
		//m_pITableFrame->SetTableChairCount(3);

		auto mode = m_pITableFrame->GetDataBaseMode();
		auto dissmiss = m_pITableFrame->IsPersonalRoomDisumme();
		if(m_pITableFrame->IsPersonalRoomDisumme())//当前朋友局解散清理记录
		{
			ZeroMemory(&m_stRecord,sizeof(m_stRecord));
		}
	}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserChooseZui(WORD wChairID, BYTE cbChooseZuiInfo[])
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_CHOOSE);
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_CHOOSE) return true;
	CopyMemory(m_cbChooseZuiInfo[wChairID], cbChooseZuiInfo, sizeof(m_cbChooseZuiInfo[wChairID]));
	m_cbChooseZuiStatus[wChairID] = true;

// 	string str = "player: " + std::to_string(wChairID) + " zuiInfo: ";
// 	for (int i = 0; i < MAX_ZUI; ++i)
// 	{
// 		str += " " + std::to_string(cbChooseZuiInfo[i]);
// 	}
// 
// 	str = "player: " + std::to_string(wChairID) + " m_cbChooseZuiCnt: ";
// 	for (int i = 0; i < MAX_ZUI; ++i)
// 	{
// 		str += " " + std::to_string(m_cbChooseZuiCnt[wChairID][i]);
// 	}
// 
// 
// 	_bstr_t bstrTmp(str.c_str());
// 	LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
// 	CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);
// 
// 	CString strInfo(pszDescribeString);
// 	CGameLogic::NcaTextOut(strInfo);

	for (int i = 0; i < m_cbPlayerCount; ++i)
	{
		if (!m_cbChooseZuiStatus[i])
		{
			return true;
		}
	}

	startPlay();
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData,bool bSysOut/*=false*/)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_PLAY);
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_PLAY) return true;

	//错误断言
	ASSERT(wChairID == m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData));

	//效验参数
	if (wChairID != m_wCurrentUser) return false;
	if (!m_GameLogic.IsValidCard(cbCardData))
	{
		int a = 1;
		a = 3;
		return false;
	}

	//删除扑克
 	if (!m_GameLogic.RemoveCard(m_cbCardIndex[wChairID], cbCardData))
 	{
 		ASSERT(FALSE);
 		return false;
 	}

	//清除禁止
	m_bEnjoinChiHu[wChairID] = false;
	m_bEnjoinChiPeng[wChairID] = false;
	m_vecEnjoinChiPeng[wChairID].clear();
	m_vecEnjoinChiHu[wChairID].clear();

	//设置变量
	m_enSendStatus = OutCard_Send;
	m_cbSendCardData = 0;
	m_cbUserAction[wChairID] = WIK_NULL;
	m_cbPerformAction[wChairID] = WIK_NULL;

	//出牌记录
	m_wOutCardUser = wChairID;
	m_cbOutCardData = cbCardData;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser = wChairID;
	OutCard.cbOutCardData = cbCardData;
	OutCard.bSysOut=bSysOut;
	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OUT_CARD, &OutCard, sizeof(OutCard));

	m_wProvideUser = wChairID;
	m_cbProvideCard = cbCardData;

	//用户切换
	m_wCurrentUser = (wChairID + 1) % m_cbPlayerCount;
	for(int i=0;i<m_cbPlayerCount;i++)
	{
		if(m_bPlayStatus[m_wCurrentUser])
			break;
		m_wCurrentUser = (m_wCurrentUser+1)%m_cbPlayerCount;
	}

	//响应判断
 	bool bAroseAction = EstimateUserRespond(wChairID, cbCardData, EstimatKind_OutCard);
 
	if(m_cbGangStatus != WIK_GANERAL)
	{
		m_bGangOutCard = true;
		m_cbGangStatus = WIK_GANERAL;
		m_wProvideGangUser = INVALID_CHAIR;
	}	
	else
	{
		m_bGangOutCard = false;
	}

 	//派发扑克
 	if (!bAroseAction) 
	{
		DispatchCardData(m_wCurrentUser);
	}

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_PLAY);
	if (m_pITableFrame->GetGameStatus() != GAME_SCENE_PLAY) return true;

	//效验用户 注意：机器人有可能发生此断言
	ASSERT((wChairID == m_wCurrentUser) || (m_wCurrentUser == INVALID_CHAIR));
	if ((wChairID != m_wCurrentUser) && (m_wCurrentUser != INVALID_CHAIR))  return true;

 	//被动动作
 	if (m_wCurrentUser == INVALID_CHAIR)
 	{
 		//效验状态
 		ASSERT(!m_bResponse[wChairID]);
 		ASSERT(m_cbUserAction[wChairID] != WIK_NULL);
 		ASSERT((cbOperateCode == WIK_NULL) || ((m_cbUserAction[wChairID]&cbOperateCode) != 0));
 
 		//效验状态
 		if (m_bResponse[wChairID]) return true;
 		if(m_cbUserAction[wChairID] == WIK_NULL) return true;
 		if ((cbOperateCode!=WIK_NULL) && ((m_cbUserAction[wChairID]&cbOperateCode) == 0)) return true;
 
 		//变量定义
 		WORD wTargetUser = wChairID;
 		BYTE cbTargetAction = cbOperateCode;
 
 		//设置变量
		m_bEnjoinGang[wTargetUser] = false;
 		m_bResponse[wTargetUser] = true;
 		m_cbPerformAction[wTargetUser] = cbOperateCode;
 		if(cbOperateCard[0] == 0)
		{
			m_cbOperateCard[wTargetUser][0] = m_cbProvideCard;
		}
 		else 
		{
			CopyMemory(m_cbOperateCard[wTargetUser], cbOperateCard, sizeof(m_cbOperateCard[wTargetUser]));
		}
 
		//放弃操作
		if (cbTargetAction == WIK_NULL)
		{
			////禁止这轮吃胡
			if((m_cbUserAction[wTargetUser] & WIK_CHI_HU) != 0)
			{
				m_bEnjoinChiHu[wTargetUser]=true;
				m_vecEnjoinChiHu[wTargetUser].push_back(m_cbProvideCard);
			}

			////禁止吃碰杠这张牌
			if ((m_cbUserAction[wTargetUser] & WIK_PENG) != 0)
			{
				//m_bEnjoinChiPeng[wTargetUser] = true;
				m_vecEnjoinChiPeng[wTargetUser].push_back(m_cbProvideCard);
			}
		}

 		//执行判断
 		for (WORD i = 0; i < m_cbPlayerCount; i++)
 		{
 			//获取动作
 			BYTE cbUserAction = (!m_bResponse[i]) ? m_cbUserAction[i] : m_cbPerformAction[i];
 
 			//优先级别
 			BYTE cbUserActionRank = m_GameLogic.GetUserActionRank(cbUserAction);
 			BYTE cbTargetActionRank = m_GameLogic.GetUserActionRank(cbTargetAction);
 
 			//动作判断
 			if (cbUserActionRank > cbTargetActionRank)
 			{
 				wTargetUser = i;
 				cbTargetAction = cbUserAction;
 			}
 		}
 		if (!m_bResponse[wTargetUser]) 
 			return true;
 
 		//吃胡等待
 		if (cbTargetAction == WIK_CHI_HU)
 		{
 			for (WORD i = 0; i < GAME_PLAYER; i++)
 			{
 				if (!m_bResponse[i] && (m_cbUserAction[i]&WIK_CHI_HU))
 					return true;
 			}
 		}
 
 		//放弃操作
 		if (cbTargetAction == WIK_NULL)
 		{
			//用户状态
 			ZeroMemory(m_bResponse, sizeof(m_bResponse));
 			ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
 			ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
 			ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));
 
			DispatchCardData(m_wResumeUser,m_cbGangStatus != WIK_GANERAL);
 			return true;
 		}
 
 		//变量定义
 		BYTE cbTargetCard = m_cbOperateCard[wTargetUser][0];
 
 		//出牌变量
 		m_enSendStatus = Gang_Send;
		m_cbSendCardData = 0;
 		m_wOutCardUser = INVALID_CHAIR;
		m_cbOutCardData = 0;
 
 		//胡牌操作
 		if (cbTargetAction == WIK_CHI_HU)
 		{
 			//结束信息
 			m_cbChiHuCard = cbTargetCard;
 
			WORD wChiHuUser = m_wBankerUser;
			for(int i = 0; i < m_cbPlayerCount; i++)
 			{
 				wChiHuUser = (m_wBankerUser + i) % m_cbPlayerCount;
				//过虑判断
 				if ((m_cbPerformAction[wChiHuUser]&WIK_CHI_HU) == 0)
 					continue;
 
 				//胡牌判断
 				BYTE cbWeaveItemCount = m_cbWeaveItemCount[wChiHuUser];
 				tagWeaveItem * pWeaveItem = m_WeaveItemArray[wChiHuUser];
 				m_dwChiHuKind[wChiHuUser] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChiHuUser], pWeaveItem, cbWeaveItemCount, m_cbChiHuCard, m_ChiHuRight[wChiHuUser], m_cbTatalFlowerCount[wChiHuUser]);

 				//插入扑克
 				if (m_dwChiHuKind[wChiHuUser] != WIK_NULL) 
 				{
 					wTargetUser = wChiHuUser;
					//break;
 				}				
 			}
 

 			//结束游戏
 			ASSERT(m_dwChiHuKind[wTargetUser] != WIK_NULL);
 			OnEventGameConclude(m_wProvideUser, NULL, GER_NORMAL);
 
 			return true;
 		}
 
 		//组合扑克
 		ASSERT(m_cbWeaveItemCount[wTargetUser] < MAX_WEAVE);
 		WORD wIndex = m_cbWeaveItemCount[wTargetUser]++;
 		m_WeaveItemArray[wTargetUser][wIndex].cbParam = WIK_GANERAL;
 		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard = cbTargetCard;
 		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind = cbTargetAction;
 		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser = (m_wProvideUser == INVALID_CHAIR) ? wTargetUser : m_wProvideUser;
 		m_WeaveItemArray[wTargetUser][wIndex].cbCardData[0] = cbTargetCard;
 		if(cbTargetAction&(WIK_LEFT | WIK_CENTER | WIK_RIGHT))
 		{
 			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = m_cbOperateCard[wTargetUser][1];
 			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = m_cbOperateCard[wTargetUser][2];
 		}
 		else
 		{
 			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = cbTargetCard;
 			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = cbTargetCard;
 			if(cbTargetAction & WIK_GANG)
			{
				m_WeaveItemArray[wTargetUser][wIndex].cbParam = WIK_FANG_GANG;
				m_WeaveItemArray[wTargetUser][wIndex].cbCardData[3] = cbTargetCard;
			}
 		}
 
 		//删除扑克
 		switch (cbTargetAction)
 		{
 		case WIK_LEFT:		//上牌操作
 			{
 				//删除扑克
 				if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], &m_cbOperateCard[wTargetUser][1], 2))
 				{
 					ASSERT(FALSE);
 					return false;
 				}
				m_cbChiPengCount[wTargetUser]++;
 
 				break;
 			}
 		case WIK_RIGHT:		//上牌操作
 			{
 				//删除扑克
 				if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], &m_cbOperateCard[wTargetUser][1], 2))
 				{
 					ASSERT(FALSE);
 					return false;
 				}
				m_cbChiPengCount[wTargetUser]++;
 
 				break;
 			}
 		case WIK_CENTER:	//上牌操作
 			{
 				//删除扑克
 				if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], &m_cbOperateCard[wTargetUser][1], 2))
 				{
 					ASSERT(FALSE);
 					return false;
 				}
				m_cbChiPengCount[wTargetUser]++;
 
 				break;
 			}
 		case WIK_PENG:		//碰牌操作
 			{
 				//删除扑克
 				BYTE cbRemoveCard[] = {cbTargetCard,cbTargetCard};
 				if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], cbRemoveCard, 2))
 				{
 					ASSERT(FALSE);
 					return false;
 				}

				m_cbChiPengCount[wTargetUser]++;
				if (!m_bNotFirstRepairFlowerRound[wTargetUser])
				{
					if (m_GameLogic.IsHasFlowerCard(m_cbCardIndex[wTargetUser]))
					{
						m_cbSendCardData = IsShowFlower(wTargetUser);
						if (m_cbSendCardData == 0)
						{
							return true;
						}

						m_cbProvideCard = m_cbSendCardData;
						// return true;
					}
				}
 				break;
 			}
 		case WIK_GANG:		//杠牌操作
 			{
 				//删除扑克,被动动作只存在放杠
 				BYTE cbRemoveCard[] = {cbTargetCard, cbTargetCard, cbTargetCard};
 				if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser], cbRemoveCard, CountArray(cbRemoveCard)))
 				{
 					ASSERT(FALSE);
 					return false;
 				}
 
				if (!m_bNotFirstRepairFlowerRound[wTargetUser])
				{
					if (m_GameLogic.IsHasFlowerCard(m_cbCardIndex[wTargetUser]))
					{
						m_cbSendCardData = IsShowFlower(wTargetUser);
						if (m_cbSendCardData == 0)
						{
							return true;
						}

						m_cbProvideCard = m_cbSendCardData;
						//return true;
					}
				}
 				break;
 			}
 		default:
 			ASSERT(FALSE);
 			return false;
 		}

		if (!m_bIsGuoZhuang)
		{
			auto privideUser = getGuoZhuangId(m_wProvideUser);
			auto curId = getGuoZhuangId(wTargetUser);
			if (curId < privideUser)
			{
				m_bIsGuoZhuang = true;
			}
		}
 
 		//构造结果
 		CMD_S_OperateResult OperateResult;
 		ZeroMemory(&OperateResult, sizeof(OperateResult));
 		OperateResult.wOperateUser = wTargetUser;
 		OperateResult.cbOperateCode = cbTargetAction;
 		OperateResult.wProvideUser = (m_wProvideUser == INVALID_CHAIR) ? wTargetUser : m_wProvideUser;
 		OperateResult.cbOperateCard[0] = cbTargetCard;
		OperateResult.bIsGuoZhuang = m_bIsGuoZhuang;
 		if(cbTargetAction & (WIK_LEFT | WIK_CENTER | WIK_RIGHT))
		{
			CopyMemory(&OperateResult.cbOperateCard[1], &m_cbOperateCard[wTargetUser][1], 2 * sizeof(BYTE));
		}
 		else if(cbTargetAction&WIK_PENG)
 		{
 			OperateResult.cbOperateCard[1] = cbTargetCard;
 			OperateResult.cbOperateCard[2] = cbTargetCard;
 		}

		logCardData(wTargetUser);

 		//用户状态
 		ZeroMemory(m_bResponse, sizeof(m_bResponse));
 		ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
 		ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
 		ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));
 
 		//发送消息
 		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));
 		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));
		FlowerCntLogic(OperateResult.wOperateUser);

		//如果非杠牌
		if (cbTargetAction != WIK_GANG)
		{
			// EstimateUserOpertion(wTargetUser, false);
			m_wProvideUser = INVALID_CHAIR;
			m_cbProvideCard = 0;
			EstimateUserOpertion(wTargetUser, false);
			if (m_cbUserAction[wTargetUser] != WIK_NULL)
			{
				SendOperateNotify(wTargetUser);
			}
			// OperateResult.cbActionMask |= m_cbUserAction[wTargetUser];
		}

 		//设置用户
 		m_wCurrentUser = wTargetUser;
 
 		//杠牌处理
 		if (cbTargetAction == WIK_GANG)
 		{
 			m_cbGangStatus = WIK_FANG_GANG;
			m_wProvideGangUser = (m_wProvideUser == INVALID_CHAIR) ? wTargetUser : m_wProvideUser;
			m_bGangCard[wTargetUser] = true;
			m_cbGangCount[wTargetUser]++;
 			DispatchCardData(wTargetUser, true);
 		}

 		return true;
 	}
 
 	//主动动作
 	if (m_wCurrentUser == wChairID)
 	{
 		//效验操作
 		//ASSERT((cbOperateCode != WIK_NULL) && ((m_cbUserAction[wChairID]&cbOperateCode) != 0));
 		//if ((cbOperateCode == WIK_NULL) || ((m_cbUserAction[wChairID]&cbOperateCode) == 0)) return false;
 
 		//扑克效验
 		ASSERT((cbOperateCode == WIK_NULL) || (cbOperateCode == WIK_CHI_HU) || (m_GameLogic.IsValidCard(cbOperateCard[0])));
 		if ((cbOperateCode != WIK_NULL) && (cbOperateCode != WIK_CHI_HU) && (!m_GameLogic.IsValidCard(cbOperateCard[0]))) return false;
 
 		//设置变量
 		m_cbUserAction[m_wCurrentUser] = WIK_NULL;
 		m_cbPerformAction[m_wCurrentUser] = WIK_NULL;
 
 		//执行动作
 		switch (cbOperateCode)
 		{
// 		case WIK_LISTEN:
// 			{
// 				return listenLogic(wChairID, cbOperateCard, cbOperateCode);
// 
// 			}
// 			break;
		case WIK_NULL:			//过操作
			{
				// 报听后自动出牌
				if (m_bTing[m_wCurrentUser])
				{
					m_bUserActionDone = true;
					m_pITableFrame->SetGameTimer(IDI_OUT_CARD, 500, 1, 0);
				}
				return true;
			}
 		case WIK_GANG:			//杠牌操作
 			{
				m_enSendStatus = Gang_Send;

 				//变量定义
 				BYTE cbWeaveIndex = 0xFF;
 				BYTE cbCardIndex = m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);
				WORD wProvideUser = wChairID;
				BYTE cbGangKind = WIK_MING_GANG;
 				//杠牌处理
				BYTE cardCnt = m_cbCardIndex[wChairID][cbCardIndex];
 				if (m_cbCardIndex[wChairID][cbCardIndex] == 1)
 				{
 					//寻找组合
 					for (BYTE i = 0; i < m_cbWeaveItemCount[wChairID]; i++)
 					{
 						BYTE cbWeaveKind = m_WeaveItemArray[wChairID][i].cbWeaveKind;
 						BYTE cbCenterCard = m_WeaveItemArray[wChairID][i].cbCenterCard;
 						if ((cbCenterCard == cbOperateCard[0]) && (cbWeaveKind == WIK_PENG))
 						{
 							cbWeaveIndex = i;
 							break;
 						}
 					}
 
					cbGangKind = WIK_MING_GANG;
 					//组合扑克
 					m_WeaveItemArray[wChairID][cbWeaveIndex].cbParam = WIK_MING_GANG;
 					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind = cbOperateCode;
 					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard = cbOperateCard[0];
 					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3] = cbOperateCard[0];
 
 					//杠牌得分
 					wProvideUser = m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser;
 				}
 				else
 				{
 					//扑克效验
 					ASSERT(m_cbCardIndex[wChairID][cbCardIndex] == 4);
 					if (m_cbCardIndex[wChairID][cbCardIndex] != 4) 
 						return false;
 
 					//设置变量
 					cbGangKind = WIK_AN_GANG;
 					cbWeaveIndex = m_cbWeaveItemCount[wChairID]++;
 					m_WeaveItemArray[wChairID][cbWeaveIndex].cbParam = WIK_AN_GANG;
 					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser = wChairID;
 					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind = cbOperateCode;
 					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard = cbOperateCard[0];
 					for(BYTE j = 0; j < 4; j++) 
					{
						m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];
					}
 				}
 
 				//删除扑克
 				m_cbCardIndex[wChairID][cbCardIndex] = 0;
				m_cbGangStatus = cbGangKind;
				m_wProvideGangUser = wProvideUser;
				m_bGangCard[wChairID] = true;
				m_cbGangCount[wChairID]++;
 
 				//构造结果
 				CMD_S_OperateResult OperateResult;
 				ZeroMemory(&OperateResult, sizeof(OperateResult));
 				OperateResult.wOperateUser = wChairID;
 				OperateResult.wProvideUser = wProvideUser;
 				OperateResult.cbOperateCode = cbOperateCode;
 				OperateResult.cbOperateCard[0] = cbOperateCard[0];
 
 				//发送消息
 				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));
 				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_OPERATE_RESULT, &OperateResult, sizeof(OperateResult));
				FlowerCntLogic(wChairID);

 				//效验动作
 				bool bAroseAction = false;
 				if (cbGangKind == WIK_MING_GANG) 
				{
					bAroseAction=EstimateUserRespond(wChairID, cbOperateCard[0], EstimatKind_GangCard);
				}
 
 				//发送扑克
 				if (!bAroseAction)
 				{
 					DispatchCardData(wChairID, true);
 				}
 
 				return true;
 			}
 		case WIK_CHI_HU:		//自摸
 			{
 				//普通胡牌
 				BYTE cbWeaveItemCount = m_cbWeaveItemCount[wChairID];
 				tagWeaveItem * pWeaveItem = m_WeaveItemArray[wChairID];
 				if(!m_GameLogic.RemoveCard(m_cbCardIndex[wChairID], &m_cbSendCardData, 1))
 				{
 					ASSERT(FALSE);
 					return false;
 				}
 				m_dwChiHuKind[wChairID] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID], pWeaveItem, cbWeaveItemCount, m_cbSendCardData, m_ChiHuRight[wChairID], m_cbTatalFlowerCount[wChairID]);
 
 				//结束信息
				m_cbChiHuCard = m_cbSendCardData;
				m_cbProvideCard = m_cbSendCardData;
				
 				//结束游戏
 				OnEventGameConclude(m_wProvideUser, NULL, GER_NORMAL);
 
 				return true;
 			}
 		}
 
 		return true;
 	}

	return false;
}

void CTableFrameSink::logCardData(WORD wTargetUser)
{
	// return;
	string str = "player: " + std::to_string(wTargetUser) + " SendCard " + std::to_string(m_cbProvideCard) + " cardData: " + "\n";

	int cardCnt = 0;
	for (int cnt = 0; cnt < MAX_INDEX; ++cnt)
	{
		if (cnt % 9 == 0)
		{
			str += "\n";
		}
		str += " " + std::to_string(m_cbCardIndex[wTargetUser][cnt]);
	}

	_bstr_t bstrTmp(str.c_str());
	LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
	// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);
	CString strInfo(pszDescribeString);
	CGameLogic::NcaTextOut(strInfo);
}

void CTableFrameSink::FlowerCntLogic(WORD chairId)
{
	BYTE extralFlowerCnt = m_GameLogic.GetExtralFlowerCnt(m_cbCardIndex[chairId], m_WeaveItemArray[chairId], m_cbWeaveItemCount[chairId]);
	auto preFlowerCnt = m_cbTatalFlowerCount[chairId];
	m_cbTatalFlowerCount[chairId] = m_cbFlowerCount[chairId] + extralFlowerCnt;
	if (preFlowerCnt != m_cbTatalFlowerCount[chairId])
	{
		CMD_S_FlowerResult flowerResult;
		flowerResult.wOperateUser = chairId;
		flowerResult.cbFlowerCnt = m_cbTatalFlowerCount[chairId];

		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_FLOWER_CNT, &flowerResult, sizeof(flowerResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_FLOWER_CNT, &flowerResult, sizeof(flowerResult));
		
	}
}
// bool CTableFrameSink::listenLogic(WORD wChairID, BYTE cbOperateCard, BYTE cbOperateCode)
// {
// 	if (m_bBaoTing[wChairID] == 1)
// 	{
// 		ASSERT(false);
// 		return true;
// 	}
// 
// 	if (WIK_NULL == AnalyseTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_bGameTypeIdex))
// 	{
// 		ASSERT(false);
// 		return true;
// 	}
// 
// 	// 应该在这里判断胡牌张数
// 	m_bBaoTing[wChairID] = 1;
// 	m_bBaoTingCard[wChairID] = cbOperateCard;
// 	updateTingInfo(wChairID);
// 
// 	checkTingAction(wChairID, cbOperateCard);
// 
// 
// 	// 				m_cbHuCardCnt[wChairID] = TingCardResult.bTingCardCount[i];
// 	// 				CopyMemory(m_cbHuCard[wChairID], TingCardResult.bTingCard[i], sizeof(m_cbHuCard[wChairID]));
// 
// #ifdef DEBUG
// 	TCHAR szString[512] = TEXT("");
// 	ZeroMemory(szString, 512);
// 	_sntprintf(szString, CountArray(szString), TEXT("桌号：%d 玩家 %d, 听牌 "), m_pITableFrame->GetTableID(), wChairID);
// 	CTraceService::TraceString(szString, TraceLevel_Normal);
// #endif // DEBUG
// 
// 	return true;
// }
// 
// 
// void CTableFrameSink::checkTingAction(BYTE cbCharid, BYTE cbBaoTingCard)
// {
// 	if (WIK_LISTEN == AnalyseTingCard(m_cbCardIndex[cbCharid], m_WeaveItemArray[cbCharid], m_cbWeaveItemCount[cbCharid], m_bGameTypeIdex))
// 	{
// 		tagTingCardResult TingCardResult;
// 		tagTingCardResult AllTingCardResult;
// 		if (SearchTingCard(TingCardResult, AllTingCardResult, cbCharid))
// 		{
// 			m_pITableFrame->SendTableData(cbCharid, SUB_S_TING_RESULT, &TingCardResult, sizeof(TingCardResult));
// 
// 			// 			CMD_S_OperateResult OperateResult;
// 			// 			OperateResult.wOperateUser = cbCharid;
// 			// 			OperateResult.wProvideUser = cbCharid;
// 			// 			OperateResult.cbOperateCode = WIK_LISTEN;
// 			// 			OperateResult.cbOperateCard = cbBaoTingCard;
// 			// 			addGameRecordAction(OperateResult);
// 
// #ifdef DEBUG_LOG_INFO
// 
// 			IServerUserItem* pUser = m_pITableFrame->GetTableUserItem(cbCharid);
// 
// 			CTraceService::TraceString("====================", TraceLevel_Normal);
// 			std::string curStr = "";
// 			//构造提示
// 			TCHAR szString[512] = TEXT("当前手里的牌:");
// 			curStr += szString;
// 
// 			std::string mj;
// 			for (int i = 0; i < MAX_INDEX; i++)
// 			{
// 				if (m_cbCardIndex[cbCharid][i] <= 0)
// 					continue;
// 				BYTE cbCard = m_GameLogic.SwitchToCardData(i);
// 				mj = getMajingText(cbCard);
// 				ZeroMemory(szString, 512);
// 				_sntprintf(szString, CountArray(szString), TEXT("[%s,idx:%d, cardData:%d,个数:%d] "), mj.c_str(), i, cbCard, m_cbCardIndex[cbCharid][i]);
// 				curStr += szString;
// 			}
// 			CTraceService::TraceString(curStr.c_str(), TraceLevel_Normal);
// 
// 			ZeroMemory(szString, 512);
// 			_sntprintf(szString, CountArray(szString), TEXT("玩家:[%s] 座位号:[%d] 听牌结果 可丢弃牌数 %d"), pUser->GetNickName(), cbCharid, TingCardResult.bAbandonCount);
// 			CTraceService::TraceString(szString, TraceLevel_Normal);
// 			for (int i = 0; i < TingCardResult.bAbandonCount; i++)
// 			{
// 				BYTE color = m_GameLogic.GetCardColor(TingCardResult.bAbandonCard[i]);
// 				BYTE value = m_GameLogic.GetCardValue(TingCardResult.bAbandonCard[i]);
// 				std::string s = getMajingText(TingCardResult.bAbandonCard[i]);
// 				ZeroMemory(szString, 512);
// 				_sntprintf(szString, CountArray(szString), TEXT("丢弃牌：[%s] CardData:[%d]"), s.c_str(), TingCardResult.bAbandonCard[i]);
// 				CTraceService::TraceString(szString, TraceLevel_Normal);
// 			}
// 
// 			for (int i = 0; i < MAX_COUNT; i++)
// 			{
// 				if (TingCardResult.bTingCardCount[i] <= 0)
// 					continue;
// 				ZeroMemory(szString, 512);
// 				_sntprintf(szString, CountArray(szString), TEXT("听牌数量 %d"), TingCardResult.bTingCardCount[i]);
// 				CTraceService::TraceString(szString, TraceLevel_Normal);
// 
// 				for (int j = 0; j < MAX_COUNT - 1; j++)
// 				{
// 					if (TingCardResult.bTingCard[i][j] <= 0)
// 						continue;
// 
// 					std::string s = getMajingText(TingCardResult.bAbandonCard[i]);
// 					ZeroMemory(szString, 512);
// 					_sntprintf(szString, CountArray(szString), TEXT("听牌：[%s] CardData:[%d]"), s.c_str(), TingCardResult.bTingCard[i][j]);
// 					CTraceService::TraceString(szString, TraceLevel_Normal);
// 				}
// 			}
// 			CTraceService::TraceString("====================", TraceLevel_Normal);
// #endif // DEBUG
// 		}
// 	}
// 	else
// 	{
// 		m_bBaoTing[cbCharid] = 0;
// 		m_cbTingDiscardIdx[cbCharid] == 100;
// 	}
// }
// 
// void CTableFrameSink::updateTingInfo(BYTE charId)
// {
// 	CMD_S_TingInfo_Update updateInfo;
// 	updateInfo.wChairId = (BYTE)charId;
// 	updateInfo.bTing = m_bBaoTing[charId];
// 	updateInfo.bOutCard = m_cbTingDiscardIdx[charId] != 100 ? 1 : 0;
// 	m_pITableFrame->SendTableData(charId, SUB_S_TING_UPDATE, &updateInfo, sizeof(updateInfo));
// }
// 
// bool CTableFrameSink::SearchTingCard(tagTingCardResult &TingCardResult, tagTingCardResult &AllTingCardResult, BYTE cbCharId)
// {
// 	//变量定义
// 	ZeroMemory(&TingCardResult, sizeof(TingCardResult));
// 	ZeroMemory(&AllTingCardResult, sizeof(AllTingCardResult));
// 	BYTE bAbandonCardCount = 0;
// 	BYTE bAllAbandonCardCount = 0;
// 
// 	//构造扑克
// 	WORD wMeChairId = cbCharId;
// 	BYTE cbCardIndexTemp[MAX_INDEX];
// 	CopyMemory(cbCardIndexTemp, m_cbCardIndex[wMeChairId], sizeof(cbCardIndexTemp));
// 
// 	BYTE cbCardCount = m_GameLogic.GetCardCount(cbCardIndexTemp);
// 	if ((cbCardCount - 2) % 3 == 0)
// 	{
// 		//听牌分析
// 		for (BYTE i = 0; i < FENG_INDEX; i++)
// 		{
// 			//空牌过滤
// 			if (cbCardIndexTemp[i] == 0) continue;
// 
// 			//听牌处理
// 			cbCardIndexTemp[i]--;
// 
// 			//听牌判断
// 			bool bHuCard = false;
// 			bool bAllHuCard = false;
// 			bAbandonCardCount = TingCardResult.bAbandonCount;
// 			bAllAbandonCardCount = AllTingCardResult.bAbandonCount;
// 			CChiHuRight chr;
// 			for (BYTE j = 0; j < FENG_INDEX; j++)
// 			{
// 				//胡牌分析
// 				BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(j);
// 
// 
// 				BYTE bColor = m_GameLogic.GetCardColor(cbCurrentCard);
// 				BYTE bValue = m_GameLogic.GetCardValue(cbCurrentCard);
// 
// 				bool isNeedFil = false;
// 				BYTE cbHuCardKind = m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp, m_WeaveItemArray[wMeChairId], m_cbWeaveItemCount[wMeChairId], cbCurrentCard, chr, m_cbFlowerCount[wMeChairId]);
// 				if (!isNeedFil)
// 				{
// 					//结果判断
// 					if (cbHuCardKind != WIK_NULL)
// 					{
// 						bHuCard = true;
// 						TingCardResult.bTingCard[bAbandonCardCount][TingCardResult.bTingCardCount[bAbandonCardCount]++] = cbCurrentCard;
// 					}
// 				}
// 
// 				//结果判断
// 				if (cbHuCardKind != WIK_NULL)
// 				{
// 					bAllHuCard = true;
// 					AllTingCardResult.bTingCard[bAllAbandonCardCount][AllTingCardResult.bTingCardCount[bAllAbandonCardCount]++] = cbCurrentCard;
// 				}
// 			}
// 
// 			if (bHuCard == true)
// 			{
// 				TingCardResult.bAbandonCard[TingCardResult.bAbandonCount++] = m_GameLogic.SwitchToCardData(i);
// 			}
// 
// 			if (bAllHuCard == true)
// 			{
// 				AllTingCardResult.bAbandonCard[AllTingCardResult.bAbandonCount++] = m_GameLogic.SwitchToCardData(i);
// 			}
// 			//还原处理
// 			cbCardIndexTemp[i]++;
// 		}
// 	}
// 	else
// 	{
// 		//听牌判断
// 		CTraceService::TraceString("打印了就不敢删除。。。。", TraceLevel_Warning);
// 		bAbandonCardCount = TingCardResult.bAbandonCount;
// 		CChiHuRight chr;
// 		for (BYTE j = 0; j < FENG_INDEX; j++)
// 		{
// 			//胡牌分析
// 			BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(j);
// 
// 			BYTE bColor = m_GameLogic.GetCardColor(cbCurrentCard);
// 			BYTE bValue = m_GameLogic.GetCardValue(cbCurrentCard);
// 
// 			BYTE cbHuCardKind = m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp, m_WeaveItemArray[wMeChairId], m_cbWeaveItemCount[wMeChairId], cbCurrentCard, chr, m_cbFlowerCount[wMeChairId]);
// 
// 			//结果判断
// 			if (cbHuCardKind != WIK_NULL)
// 			{
// 				TingCardResult.bTingCard[bAbandonCardCount][TingCardResult.bTingCardCount[bAbandonCardCount]++] = cbCurrentCard;
// 			}
// 		}
// 	}
// 
// 	//计算剩余牌
// 	BYTE n = 0;
// 	while (TingCardResult.bTingCardCount[n] > 0)
// 	{
// 		TingCardResult.bRemainCount[n] = TingCardResult.bTingCardCount[n] * 4;
// 
// 		for (BYTE i = 0; i < TingCardResult.bTingCardCount[n]; i++)
// 		{
// 			BYTE bCardData = TingCardResult.bTingCard[n][i];
// 			//减自己牌
// 			if (cbCardIndexTemp[m_GameLogic.SwitchToCardIndex(bCardData)] > 0)
// 			{
// 				TingCardResult.bRemainCount[n] -= cbCardIndexTemp[m_GameLogic.SwitchToCardIndex(bCardData)];
// 				ASSERT(TingCardResult.bRemainCount[n] >= 0);
// 			}
// 
// 			for (BYTE j = 0; j < GAME_PLAYER; j++)
// 			{
// 				//减组合牌
// 				BYTE k = 0;
// 				for (k = 0; k < m_cbWeaveItemCount[j]; k++)
// 				{
// 					if (m_WeaveItemArray[j][k].cbCenterCard == bCardData)
// 					{
// 						TingCardResult.bRemainCount[n] -= m_WeaveItemArray[j][k].cbWeaveKind == WIK_GANG ? 4 : 3;
// 						ASSERT(TingCardResult.bRemainCount[n] >= 0);
// 					}
// 				}
// 				//减丢弃牌
// 				for (k = 0; k < m_cbDiscardCount[j]; k++)
// 				{
// 					if (bCardData == m_cbDiscardCard[j][k])
// 					{
// 						TingCardResult.bRemainCount[n]--;
// 						ASSERT(TingCardResult.bRemainCount[n] >= 0);
// 					}
// 				}
// 			}
// 		}
// 
// 		n++;
// 	}
// 
// 	return true;
// }

//用户听牌
bool CTableFrameSink::OnUserListenCard(WORD wChairID, bool bListenCard)
{
	if(bListenCard)
	{
		ASSERT(!m_bTing[wChairID]);
		if(WIK_LISTEN == m_GameLogic.AnalyseTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbTatalFlowerCount[wChairID]))
		{
			m_bTing[wChairID] = true;
			CMD_S_ListenCard ListenCard;
			ZeroMemory(&ListenCard,sizeof(ListenCard));

			ListenCard.wListenUser = wChairID;
			ListenCard.bListen = true;
			//发给消息//
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN_CARD,&ListenCard,sizeof(ListenCard));

			for(int i=0;i<m_cbPlayerCount;i++)
			{
				if (m_pITableFrame->GetTableUserItem(i) == NULL)
					continue;

				if( i != wChairID)
					m_pITableFrame->SendTableData(i,SUB_S_LISTEN_CARD,&ListenCard,sizeof(ListenCard));
			}

			
			//计算胡几张字
			m_cbHuCardCount[wChairID] = m_GameLogic.GetHuCard(m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID],m_cbWeaveItemCount[wChairID],m_cbHuCardData[wChairID], m_cbTatalFlowerCount[wChairID]);
			ASSERT(m_cbHuCardCount[wChairID]>0);
			ListenCard.cbHuCardCount = m_cbHuCardCount[wChairID];
			CopyMemory(ListenCard.cbHuCardData,m_cbHuCardData[wChairID],sizeof(ListenCard.cbHuCardData));

			//听牌后禁止碰牌
			m_bEnjoinChiPeng[wChairID] = true;

			// m_cbLastTingChuPaiIndex = m_cbChuPaiIndex;

			m_pITableFrame->SendTableData(wChairID,SUB_S_LISTEN_CARD,&ListenCard,sizeof(ListenCard));
			
// 			string str = "player: " + std::to_string(wChairID) + " ting============";
// 			
// 			_bstr_t bstrTmp(str.c_str());
// 			LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
// 			CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);
// 			CString strInfo(pszDescribeString);
// 			CGameLogic::NcaTextOut(strInfo);

			return true;
		}
		else
		{
			return false;
		}
	}
// 	else
// 	{
// 
// 		ASSERT(m_bTing[wChairID]);
// 		m_bTing[wChairID] = false;
// 
// 		CMD_S_ListenCard ListenCard;
// 		ListenCard.wListenUser = wChairID;
// 		ListenCard.bListen = false;
// 
// 		//发送消息
// 		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_LISTEN_CARD, &ListenCard, sizeof(ListenCard));
// 		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_LISTEN_CARD, &ListenCard, sizeof(ListenCard));
// 		return true;
// 	}
}

//用户托管
bool CTableFrameSink::OnUserTrustee(WORD wChairID, bool bTrustee)
{
	//效验状态
	ASSERT((wChairID < m_cbPlayerCount));
	if ((wChairID>=m_cbPlayerCount)) return false;

	m_bTrustee[wChairID]=bTrustee;

	CMD_S_Trustee Trustee;
	Trustee.bTrustee=bTrustee;
	Trustee.wChairID = wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

	if(bTrustee)
	{
		if(wChairID == m_wCurrentUser && m_bUserActionDone==false)
		{
			
			BYTE cardindex=INVALID_BYTE;
			if(m_cbSendCardData != 0)
				cardindex = m_GameLogic.SwitchToCardIndex(m_cbSendCardData);
			else
			{
				for(int i=0;i<MAX_INDEX;i++)
				{
					if(m_cbCardIndex[wChairID][i]>0)
					{
						cardindex = i;
						break;
					}
				}
			}

			ASSERT(cardindex != INVALID_BYTE);
			OnUserOutCard(wChairID,m_GameLogic.SwitchToCardData(cardindex));
		}
		else if(m_wCurrentUser == INVALID_CHAIR && m_bUserActionDone==false)
		{
			BYTE operatecard[3]={0};
			OnUserOperateCard(wChairID,WIK_NULL,operatecard);
		}
	}

	return true;
}

//用户补牌
bool CTableFrameSink::OnUserReplaceCard(WORD wChairID, BYTE cbCardData)
{
	//错误断言
	ASSERT(m_GameLogic.IsValidCard(cbCardData));
	//效验参数
	if(!m_GameLogic.IsValidCard(cbCardData))  return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID], cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//状态变量
	m_enSendStatus = BuHua_Send;
	m_cbGangStatus = WIK_GANERAL;
	m_wProvideGangUser = INVALID_CHAIR;

	//构造数据
	CMD_S_ReplaceCard ReplaceCard;
	ReplaceCard.wReplaceUser = wChairID;
	ReplaceCard.cbReplaceCard = cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_REPLACE_CARD, &ReplaceCard, sizeof(ReplaceCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_REPLACE_CARD, &ReplaceCard, sizeof(ReplaceCard));

	//派发扑克
	DispatchCardData(wChairID, true);

	return true;
}

//发送扑克
bool CTableFrameSink::OnUserSendCard(BYTE cbCardCount, WORD wBankerUser, BYTE cbCardData[], BYTE cbControlGameCount)
{
#ifdef  CARD_DISPATCHER_CONTROL
	ASSERT(cbCardCount == MAX_REPERTORY && wBankerUser < GAME_PLAYER);
	if(cbCardCount != MAX_REPERTORY || wBankerUser >= GAME_PLAYER) return false;

	//设置控制局数
	m_cbControlGameCount = cbControlGameCount;

	m_wControBankerUser = wBankerUser;

	//拷贝控制扑克
	CopyMemory(m_cbControlRepertoryCard, cbCardData, sizeof(m_cbControlRepertoryCard));
#endif
	return true;
}

//发送操作
bool CTableFrameSink::SendOperateNotify(WORD chairId /*= INVAID_CHAIR*/)
{
	if (chairId != INVALID_CHAIR)
	{
		//构造数据
		CMD_S_OperateNotify OperateNotify;
		ZeroMemory(&OperateNotify, sizeof(OperateNotify));

		OperateNotify.cbActionCard = m_cbOperateCard[chairId][0];
		// CopyMemory(OperateNotify.cbActionCard, m_cbOperateCard[chairId], sizeof(m_cbOperateCard[chairId]));
		OperateNotify.cbActionMask = m_cbUserAction[chairId];

		//发送数据
		m_pITableFrame->SendTableData(chairId, SUB_S_OPERATE_NOTIFY, &OperateNotify, sizeof(OperateNotify));
		m_pITableFrame->SendLookonData(chairId, SUB_S_OPERATE_NOTIFY, &OperateNotify, sizeof(OperateNotify));
		return true;
	}

	//发送提示
	for (WORD i=0;i<m_cbPlayerCount;i++)
	{
		if (m_pITableFrame->GetTableUserItem(i) == NULL)
			continue;

		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			ZeroMemory(&OperateNotify, sizeof(OperateNotify));
			//CopyMemory(OperateNotify.cbActionCard, m_cbOperateCard[i][0], sizeof(m_cbOperateCard[i][0]));
			OperateNotify.cbActionCard = m_cbOperateCard[i][0];
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			
// 			if(m_bTrustee[i])
// 			{
// 				m_pITableFrame->SetGameTimer(IDI_OPERATE_CARD+i,1000,1,0);
// 			}
		}
	}

	return true;
}

//取得扑克
BYTE CTableFrameSink::GetSendCard(bool bTail)
{
	//荒庄结束
	if ((m_cbLeftCardCount + 1) / 2 <= HUANG_ZHUANG_HEAP_CNT) // 7摞牌结束 //(m_cbMaCount>1?m_cbMaCount+1:m_cbMaCount) )//2-6码要多留一颗，一码全中不用
	{
		m_cbChiHuCard = 0;
		m_wProvideUser = INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser, NULL, GER_NORMAL);

		return 0;
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbLeftCardCount--;

	BYTE cbSendCardData = 0;
	
	BYTE cbIndexCard;
	if(bTail)
	{	
		cbSendCardData = m_cbRepertoryCard[m_cbMinusLastCount];
		m_cbMinusLastCount++;
	}
	else
	{
		m_cbMinusHeadCount++;
		cbIndexCard = MAX_REPERTORY - m_cbMinusHeadCount;
		cbSendCardData=m_cbRepertoryCard[cbIndexCard];
	}	

	//堆立信息
	ASSERT(m_wHeapHead != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR);
	if(!bTail)
	{
		//切换索引
		BYTE cbHeapCount = m_cbHeapCardInfo[m_wHeapHead][0] + m_cbHeapCardInfo[m_wHeapHead][1];
		if (cbHeapCount == HEAP_FULL_COUNT) 
		{
			m_wHeapHead = (m_wHeapHead + GAME_PLAYER - 1) % CountArray(m_cbHeapCardInfo);
		}
		m_cbHeapCardInfo[m_wHeapHead][0]++;
	}
	else
	{
		//切换索引
		BYTE cbHeapCount = m_cbHeapCardInfo[m_wHeapTail][0] + m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount == HEAP_FULL_COUNT) 
		{
			m_wHeapTail = (m_wHeapTail + 1) % CountArray(m_cbHeapCardInfo);
		}
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	if (cbSendCardData == 0)
	{
		string str = "GetSendCard is Zero m_cbMinusLastCount: " + std::to_string(m_cbMinusLastCount);
		
		_bstr_t bstrTmp(str.c_str());
		LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
		// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);

		CString strInfo(pszDescribeString);
		// 		CString strInfo;
		// 		strInfo.Format(TEXT("[%s]"), str.c_str());
		CGameLogic::NcaTextOut(strInfo);
		
	}
	return cbSendCardData;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wSendCardUser, bool bTail)
{
	//状态效验
	ASSERT(wSendCardUser != INVALID_CHAIR);
	if (wSendCardUser == INVALID_CHAIR) return false;

	ASSERT(m_enSendStatus != Not_Send);
	if(m_enSendStatus == Not_Send) return false;

	WORD wCurrentUser = wSendCardUser;

	//丢弃扑克
	if ((m_wOutCardUser != INVALID_CHAIR) && (m_cbOutCardData != 0))
	{
		m_cbOutCardCount++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]] = m_cbOutCardData;
		m_cbDiscardCount[m_wOutCardUser]++;
	}

	//荒庄结束
	if ((m_cbLeftCardCount + 1) / 2 <= HUANG_ZHUANG_HEAP_CNT) // 7摞牌结束 //(m_cbMaCount>1?m_cbMaCount+1:m_cbMaCount) )//2-6码要多留一颗，一码全中不用
	{
		m_cbChiHuCard = 0;
		m_wProvideUser = INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser, NULL, GER_NORMAL);

		return true;
	}

	//发送扑克
	m_cbProvideCard = GetSendCard(bTail);	
	m_cbSendCardData = m_cbProvideCard;
	m_wLastCatchCardUser = wSendCardUser;
	//清除禁止胡牌的牌
	m_bEnjoinChiHu[wCurrentUser] = false;
	m_vecEnjoinChiHu[wCurrentUser].clear();
	m_bEnjoinChiPeng[wCurrentUser] = false;		
	m_vecEnjoinChiPeng[wCurrentUser].clear();
	m_bEnjoinGang[wCurrentUser] = false;
	
	//设置变量	
	m_wOutCardUser = INVALID_CHAIR;
	m_cbOutCardData = 0;	
	m_wCurrentUser = wCurrentUser;
	if (!m_bIsGuoZhuang)
	{
		auto privideUser = getGuoZhuangId(m_wProvideUser);
		auto curId = getGuoZhuangId(wCurrentUser);
		if (curId < privideUser)
		{
			m_bIsGuoZhuang = true;
		}
	}

	m_wProvideUser = wCurrentUser;		
	m_bGangOutCard = false;
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));

	if(bTail)//从尾部取牌，说明玩家杠牌了,计算分数
	{
		CalGangScore();
	}

	//加牌
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
	m_cbUserCatchCardCount[wCurrentUser]++;

	//构造数据
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard, sizeof(SendCard));


	SendCard.wSendCardUser = wSendCardUser;
	SendCard.wCurrentUser = wCurrentUser;
	SendCard.bTail = bTail;
	SendCard.cbActionMask = WIK_NULL;// m_cbUserAction[wCurrentUser];
	SendCard.cbCardData = m_cbProvideCard;
	SendCard.bIsGuoZhuang = m_bIsGuoZhuang;
	// logCardData(wCurrentUser);

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD, &SendCard, sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD, &SendCard, sizeof(SendCard));

	if (m_GameLogic.IsHasFlowerCard(m_cbCardIndex[wCurrentUser]))
	{
		m_cbSendCardData = IsShowFlower(m_wCurrentUser);
		m_cbProvideCard = m_cbSendCardData;
	}
	m_bNotFirstRepairFlowerRound[m_wCurrentUser] = true;

	EstimateUserOpertion(wCurrentUser, true);
	if (m_cbUserAction[wCurrentUser] != WIK_NULL)
	{
		SendOperateNotify(wCurrentUser);
	}

	m_bUserActionDone=false;
	if(m_bTrustee[wCurrentUser] || (m_bTing[wCurrentUser] && m_cbUserAction[wCurrentUser] == WIK_NULL))
	{
		m_bUserActionDone=true;
		m_pITableFrame->SetGameTimer(IDI_OUT_CARD, TIME_OUT_CARD,1,0);
	}
	return true;
}

WORD CTableFrameSink::getGuoZhuangId(WORD wChairId)
{
	if (wChairId == m_wBankerUser)
	{
		return wChairId;
	}

	return wChairId + m_wBankerUser + 1;
}

void CTableFrameSink::EstimateUserOpertion(WORD wCurrentUser, bool isSendCard)
{
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
	m_cbOperateCard[wCurrentUser][0] = m_cbSendCardData;
	if (!m_bTrustee[wCurrentUser])
	{
		//胡牌判断
		if (!m_bEnjoinChiHu[wCurrentUser] && isSendCard)
		{
			CChiHuRight chr;
			m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
			m_cbUserAction[wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser],
				m_cbWeaveItemCount[wCurrentUser], m_cbSendCardData, chr, m_cbTatalFlowerCount[wCurrentUser]);
			m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
		}

		//杠牌判断
		if ((!m_bEnjoinGang[wCurrentUser]) && (m_cbLeftCardCount > m_cbEndLeftCount) && isSendCard)
		{
			userGangEstimate(wCurrentUser, isSendCard);
		}

		//听牌判断
		CMD_S_Hu_Data HuData;
		ZeroMemory(&HuData, sizeof(HuData));

		BYTE cbCount = 0;
		if (!m_bTing[wCurrentUser])
		{
			cbCount = m_GameLogic.AnalyseTingCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], m_cbTatalFlowerCount[wCurrentUser], m_bIsGuoZhuang, HuData.cbOutCardCount, HuData.cbOutCardData, HuData.cbHuCardCount, HuData.cbHuCardData);
			if (cbCount > 0)
			{
				m_cbUserAction[wCurrentUser] |= WIK_LISTEN;

				for (int i = 0; i < MAX_HAND_COUNT; i++)
				{
					if (HuData.cbHuCardCount[i] > 0)
					{
						for (int j = 0; j < HuData.cbHuCardCount[i]; j++)
						{
							HuData.cbHuCardRemainingCount[i][j] = GetRemainingCount(wCurrentUser, HuData.cbHuCardData[i][j]);
						}
					}
					else
						break;
				}
				m_pITableFrame->SendTableData(wCurrentUser, SUB_S_HU_CARD, &HuData, sizeof(HuData));
			}
		}
	}
}

void CTableFrameSink::userGangEstimate(WORD wCurrentUser, bool send)
{
	tagGangCardResult GangCardResult;
	ZeroMemory(&GangCardResult, sizeof(GangCardResult));
	if (m_bTing[wCurrentUser])
	{
		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp, m_cbCardIndex[wCurrentUser], sizeof(cbCardIndexTemp));
		BYTE cbHuCardData[28];
		ZeroMemory(cbHuCardData, sizeof(cbHuCardData));
		// 手上为1，代表组合杠
		cbCardIndexTemp[m_GameLogic.SwitchToCardIndex(m_cbProvideCard)] = 0;
		int nCount = m_GameLogic.GetHuCard(cbCardIndexTemp, m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], cbHuCardData, m_cbTatalFlowerCount[wCurrentUser]);
		if (nCount <= 0)
		{
			return;
		}

		bool isSame = true;
		if (m_cbHuCardCount[wCurrentUser] != nCount)
		{
			isSame = false;
		}
		else
		{
			for (int i = 0; i < nCount; ++i)
			{
				if (m_cbHuCardData[wCurrentUser][i] != cbHuCardData[i])
				{
					isSame = false;
					break;
				}
			}
		}

		if (isSame)
		{
			m_cbUserAction[wCurrentUser] |= m_GameLogic.AnalyseGangCardEx(m_cbCardIndex[wCurrentUser],
				m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], m_cbProvideCard, send, GangCardResult);
			for (int gangCnt = 0; gangCnt < GangCardResult.cbCardCount; ++gangCnt)
			{
				if (gangCnt >= 3)
				{
					break;
				}
				m_cbOperateCard[wCurrentUser][gangCnt] = GangCardResult.cbCardData[gangCnt];
			}
		}
	}
	else
	{
		m_cbUserAction[wCurrentUser] |= m_GameLogic.AnalyseGangCardEx(m_cbCardIndex[wCurrentUser],
			m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], m_cbProvideCard, send, GangCardResult);
		for (int gangCnt = 0; gangCnt < GangCardResult.cbCardCount; ++gangCnt)
		{
			if (gangCnt >= 3)
			{
				break;
			}
			m_cbOperateCard[wCurrentUser][gangCnt] = GangCardResult.cbCardData[gangCnt];
		}
	}
}

//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction = false;

	//用户状态
	ZeroMemory(m_bResponse, sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction, sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard, sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction, sizeof(m_cbPerformAction));

	//动作判断
	for (WORD i = 0; i < m_cbPlayerCount; i++)
	{
		//用户过滤
		if (wCenterUser == i || !m_bPlayStatus[i] || m_bTrustee[i]) continue;
		m_cbOperateCard[i][0] = cbCenterCard;

		//出牌类型
		if (EstimatKind == EstimatKind_OutCard)
		{
			//吃碰判断
			bool bPeng = true;
			if (m_bEnjoinChiPeng[i])
			{
				bPeng = false;
			}

			for (UINT j = 0; j < m_vecEnjoinChiPeng[i].size(); j++)
			{
				if (m_vecEnjoinChiPeng[i][j] == cbCenterCard)
				{
					bPeng = false;
				}
			}
			if (bPeng && !m_bTing[i])
			{
				//碰牌判断
				m_cbUserAction[i] |= m_GameLogic.EstimatePengCard(m_cbCardIndex[i], cbCenterCard);
			}
			
			//杠牌判断

			//杠牌判断
			if (m_cbLeftCardCount > m_cbEndLeftCount && !m_bEnjoinGang[i])
			{
				userGangEstimate(i, false);

// 				听牌后的杠,不能影响牌面
// 									if (m_bTing[i])
// 									{
// 										//听牌判断
// 										BYTE cbHuCardData[28];
// 										ZeroMemory(cbHuCardData, sizeof(cbHuCardData));
// 										int iCount = m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(cbCenterCard)];
// 										m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(cbCenterCard)] = 0;
// 										int nCount = m_GameLogic.GetHuCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], m_cbHuCardData[i], m_cbTatalFlowerCount[i]);
// 										m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(cbCenterCard)] = iCount;
// 										if (nCount > 0)
// 										{
// 											m_cbUserAction[i] |= m_GameLogic.EstimateGangCard(m_cbCardIndex[i], cbCenterCard);
// 										}
// 									}
// 									else
// 										m_cbUserAction[i] |= m_GameLogic.EstimateGangCard(m_cbCardIndex[i], cbCenterCard);
			}

			if (!m_bEnjoinChiHu[i])
			{
				//吃胡判断
				CChiHuRight chr;
				BYTE cbWeaveCount = m_cbWeaveItemCount[i];
				BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i], m_WeaveItemArray[i], cbWeaveCount, cbCenterCard, chr, m_cbTatalFlowerCount[i]);
				m_cbUserAction[i] |= cbAction;
			}
		}

		//结果判断
		if (m_cbUserAction[i] != WIK_NULL) 
			bAroseAction = true;
	}

	//结果处理
	if (bAroseAction) 
	{
		//设置变量
		m_wProvideUser = wCenterUser;
		m_cbProvideCard = cbCenterCard;
		m_wResumeUser = m_wCurrentUser;
		m_wCurrentUser = INVALID_CHAIR;

		//发送提示
		SendOperateNotify();

		return true;
	}

	return false;
}

//算分
void CTableFrameSink::CalHuPaiScore(LONGLONG lEndScore[GAME_PLAYER])
{
	//初始化
	ZeroMemory(lEndScore,sizeof(LONGLONG)*GAME_PLAYER);
	ZeroMemory(m_cbUserMaCount,sizeof(m_cbUserMaCount));

	bool bNoNegative = (m_pGameServiceOption->wServerType & GAME_GENRE_GOLD) != 0 || (m_pGameServiceOption->wServerType & SCORE_GENRE_POSITIVE) != 0;

	SCORE lCellScore = m_pITableFrame->GetCellScore();

	SCORE lUserScore[GAME_PLAYER] = {0};//玩家手上分
	SCORE lTempScore = 0;
	SCORE lTempAllScore = 0;
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if (!m_bPlayStatus[i])
			continue;

		if (m_pITableFrame->GetTableUserItem(i) == NULL)
			continue;

		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		ASSERT(pIServerUserItem);
		if(pIServerUserItem)
		{
			lUserScore[i] = pIServerUserItem->GetUserScore();
		}		
	}

	for (BYTE playerCnt = 0; playerCnt < GAME_PLAYER; ++playerCnt)
	{
		m_wWinUser[playerCnt] = INVALID_CHAIR;
	}
	m_cbWinCount = 0;
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		WORD user = m_wBankerUser + i;
		user %= GAME_PLAYER;
		if (!m_bPlayStatus[user])
			continue;

		if (m_pITableFrame->GetTableUserItem(user) == NULL)
			continue;

		if(WIK_CHI_HU == m_dwChiHuKind[(user) % GAME_PLAYER])
		{
			m_wWinUser[m_cbWinCount++] = (user) % GAME_PLAYER;

			//统计胡牌次数
			m_stRecord.cbHuCount[(user) % GAME_PLAYER]++;
		}
	}

	if(m_cbWinCount > 0)//有人胡牌
	{
		auto winUser = m_wWinUser[0];
		bool bZiMo= (m_wProvideUser == m_wWinUser[0]);
		if(bZiMo)//自摸
		{
			WORD score = m_GameLogic.GetChiHuScore(m_ChiHuRight[winUser], m_cbCardIndex[winUser], m_cbTatalFlowerCount[winUser]);
			m_stRecord.cbZimoCount[winUser]++;
			//m_pITableFrame->AddPrivateAction(m_wWinUser, PrivateAction_ZiMo);

			for(int i=0;i<GAME_PLAYER;i++)
			{
				if (!m_bPlayStatus[i])
					continue;

				if (m_pITableFrame->GetTableUserItem(i) == NULL)
					continue;

				if(i != m_wWinUser[0])
				{
					m_lDetailScore[m_wWinUser[0]][i] -= score;
					m_lDetailScore[m_wWinUser[0]][m_wWinUser[0]] += score;
					lEndScore[i] -= score;
					lEndScore[m_wWinUser[0]] += score;
				}
			}
		}
		else//放炮
		{
			for(int i=0;i<m_cbWinCount;i++)
			{
				BYTE winUser = m_wWinUser[i];
				WORD score = m_GameLogic.GetChiHuScore(m_ChiHuRight[winUser], m_cbCardIndex[winUser], m_cbTatalFlowerCount[winUser]);
				m_stRecord.cbJiePaoCount[winUser]++;

				m_lDetailScore[winUser][m_wProvideUser] -= score;
				m_lDetailScore[winUser][winUser] += score;

				lEndScore[winUser] += score;
				lEndScore[m_wProvideUser] -= score;
			}
			m_stRecord.cbDiaoPaoCount[m_wProvideUser]++;
		}

		//谁胡谁当庄
		m_wLastBankerUser = m_wBankerUser;
		m_wBankerUser = m_wWinUser[0];
	}
// 	else//荒庄
// 	{
// 		m_wBankerUser = m_wLastCatchCardUser;//最后一个摸牌的人当庄
// 	}
}

void CTableFrameSink::CalGangScore()
{
	SCORE lcell = m_pITableFrame->GetCellScore();
	WORD wTargetID = m_wCurrentUser;
	WORD wProvideID = m_wProvideGangUser;
	BYTE cbGangIndex = m_GangScore[wTargetID].cbGangCount++;
	if(m_cbGangStatus == WIK_FANG_GANG)//放杠一家扣分
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(!m_bPlayStatus[i])
				continue;
			if(i != m_wCurrentUser)
			{
				m_lUserGangScore[m_wProvideGangUser] -= lcell;
				m_lUserGangScore[m_wCurrentUser] += lcell;

				m_lDetailScore[m_wCurrentUser][m_wProvideGangUser] -= lcell;
				m_lDetailScore[m_wCurrentUser][m_wCurrentUser] += lcell;

				m_GangScore[wTargetID].lScore[cbGangIndex][i] = -lcell;
				m_GangScore[wTargetID].lScore[cbGangIndex][wTargetID] += lcell;
			}
		}
		//记录明杠次数
		m_stRecord.cbMingGang[m_wCurrentUser]++;
	}
	else if(m_cbGangStatus == WIK_MING_GANG)//明杠每家出1倍
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(!m_bPlayStatus[i])
				continue;
			if(i != m_wCurrentUser)
			{
				m_lUserGangScore[i] -= lcell;
				m_lUserGangScore[m_wCurrentUser] += lcell;


				m_lDetailScore[m_wCurrentUser][i] -= lcell;
				m_lDetailScore[m_wCurrentUser][m_wCurrentUser] += lcell;

				m_GangScore[wTargetID].lScore[cbGangIndex][i] = -lcell;
				m_GangScore[wTargetID].lScore[cbGangIndex][wTargetID] += lcell;
			}
		}
		//记录明杠次数
		m_stRecord.cbMingGang[m_wCurrentUser]++;
	}
	else if(m_cbGangStatus == WIK_AN_GANG)//暗杠每家出2倍
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(!m_bPlayStatus[i])
				continue;
			if(i != m_wCurrentUser)
			{
				m_lUserGangScore[i] -= 2*lcell;
				m_lUserGangScore[m_wCurrentUser] += 2*lcell;


				m_lDetailScore[m_wCurrentUser][i] -= 2 * lcell;
				m_lDetailScore[m_wCurrentUser][m_wCurrentUser] += 2 * lcell;

				m_GangScore[wTargetID].lScore[cbGangIndex][i] = -2 * lcell;
				m_GangScore[wTargetID].lScore[cbGangIndex][wTargetID] += 2 * lcell;
			}
		}
		//记录暗杠次数
		m_stRecord.cbAnGang[m_wCurrentUser]++;
	}
}

void CTableFrameSink::CalZuiScore(LONGLONG lZuiScore[GAME_PLAYER])
{
	ZeroMemory(lZuiScore, sizeof(lZuiScore));
	ZeroMemory(m_lZuiScore, sizeof(m_lZuiScore));
	for (int i = 0; i < m_cbWinCount; i++)
	{
		BYTE winUser = m_wWinUser[i];

		if (winUser == INVALID_CHAIR)
		{
			return;
		}

		if (m_cbScoreZui == 0)
		{
			m_cbScoreZui = 2;
		}
		winUser %= GAME_PLAYER;

		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp, m_cbCardIndex[winUser], sizeof(cbCardIndexTemp));

		//插入扑克
		if (m_cbProvideCard != 0)
		{
			cbCardIndexTemp[m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
		}

		int zuiCnt = 0;
		string strLog;
		for (int zuiIndex = 0; zuiIndex < MAX_ZUI; ++zuiIndex)
		{
			enZuiKind zuiKind;
			zuiKind = (enZuiKind)zuiIndex;
			BYTE winUserHandZuiCnt = 1;
			switch (zuiKind)
			{
			case TING:
				if (m_bTing[winUser])
				{
					winUserHandZuiCnt = 1;
				}
				else
				{
					winUserHandZuiCnt = 0;
				}
				break;
			case KAN:
				winUserHandZuiCnt = m_GameLogic.GetKanCnt(cbCardIndexTemp, m_WeaveItemArray[winUser], m_cbWeaveItemCount[winUser]);
				break;
			case Wen:
				winUserHandZuiCnt = m_GameLogic.GetWenQianCnt(cbCardIndexTemp, m_WeaveItemArray[winUser], m_cbWeaveItemCount[winUser]);
				break;
			case NIU:
				winUserHandZuiCnt = 1;
				break;
			case QIE:
				winUserHandZuiCnt = m_GameLogic.GetQueColorCnt(cbCardIndexTemp, m_WeaveItemArray[winUser], m_cbWeaveItemCount[winUser]);
				break;
			case DUI:
				winUserHandZuiCnt = m_GameLogic.GetDuiCnt(cbCardIndexTemp, m_WeaveItemArray[winUser], m_cbWeaveItemCount[winUser]);
				break;
			case BA:
				winUserHandZuiCnt = m_GameLogic.GetBaCnt(cbCardIndexTemp, m_WeaveItemArray[winUser], m_cbWeaveItemCount[winUser]);
				break;
			case ZAN:

				break;
			case JIE:
				if (winUser == m_wProvideUser)
				{
					winUserHandZuiCnt = 1;
				}
				else
				{
					winUserHandZuiCnt = 0;
				}
				break;
			default:
				break;
			}

			strLog += getZuiStr(zuiKind);
			BYTE winUserChooseZuiCnt = m_cbChooseZuiInfo[winUser][zuiIndex];
			//构造扑克
			BYTE cbCardIndexTempTing[MAX_INDEX];
			CopyMemory(cbCardIndexTempTing, m_cbCardIndex[winUser], sizeof(cbCardIndexTempTing));

			if (zuiKind == enZuiKind::TING)
			{
				winUserChooseZuiCnt += m_GameLogic.GetTingCnt(cbCardIndexTempTing, m_WeaveItemArray[winUser], m_cbWeaveItemCount[winUser], m_cbProvideCard);
			}
			// 自摸要算其他三家的嘴
			if (winUser == m_wProvideUser)
			{
				if (zuiKind == enZuiKind::ZAN)
				{
					if (m_wLastBankerUser == winUser)
					{
						winUserChooseZuiCnt += ZAN_ADD;
					}
					else
					{
						winUserChooseZuiCnt = 0;
					}
				}

				for (int palyerIndex = 0; palyerIndex < GAME_PLAYER; palyerIndex++)
				{
					if (!m_bPlayStatus[palyerIndex])
						continue;

					if (m_pITableFrame->GetTableUserItem(palyerIndex) == NULL)
						continue;

					if (palyerIndex == winUser)
					{
						continue;
					}

					BYTE loseUserChooseZuiCnt = m_cbChooseZuiInfo[palyerIndex][zuiIndex];
					if (zuiKind == enZuiKind::ZAN)
					{
						if (m_wLastBankerUser == palyerIndex)
						{
							loseUserChooseZuiCnt += ZAN_ADD;
						}
						else
						{
							loseUserChooseZuiCnt = 0;
						}
					}
					SCORE score = m_cbScoreZui * winUserHandZuiCnt * (loseUserChooseZuiCnt + winUserChooseZuiCnt);

					strLog += "player: " + std::to_string(palyerIndex) + " score: " + to_string(-score);
					strLog += " handZuiCnt: " + std::to_string(winUserHandZuiCnt) + " winUserChooseZuiCnt: " + to_string(winUserChooseZuiCnt);
					strLog += " userChooseZuiCnt: " + std::to_string(loseUserChooseZuiCnt) + "\n";

					strLog += "player: " + std::to_string(winUser) + " score: " + to_string(score);
					strLog += " handZuiCnt: " + std::to_string(winUserHandZuiCnt) + " winUserChooseZuiCnt: " + to_string(winUserChooseZuiCnt);
					strLog += " userChooseZuiCnt: " + std::to_string(loseUserChooseZuiCnt) + "\n";

					lZuiScore[palyerIndex] += -score;
					lZuiScore[winUser] += score;
					m_lZuiScore[palyerIndex][zuiIndex] = -score;
					m_lZuiScore[winUser][zuiIndex] = score;

					m_lDetailScore[winUser][palyerIndex] += -score;
					m_lDetailScore[winUser][winUser] += score;

					if (lZuiScore[0] + lZuiScore[1] != 0)
					{
						int a = 1;
						a = 2;
					}
				}
			}
			else
			{
				BYTE userChooseZuiCnt = m_cbChooseZuiInfo[m_wProvideUser][zuiIndex];
				if (zuiKind == enZuiKind::ZAN)
				{
					if (m_wLastBankerUser == m_wProvideUser)
					{
						userChooseZuiCnt += ZAN_ADD;
					}
					else
					{
						userChooseZuiCnt = 0;
					}

					if (winUser == m_wLastBankerUser)
					{
						winUserChooseZuiCnt += ZAN_ADD;
					}
					else
					{
						winUserChooseZuiCnt = 0;
					}
				}

				SCORE score = m_cbScoreZui * winUserHandZuiCnt * (userChooseZuiCnt + winUserChooseZuiCnt);
				lZuiScore[winUser] += score;
				lZuiScore[m_wProvideUser] += -score;

				m_lDetailScore[winUser][m_wProvideUser] += -score;
				m_lDetailScore[winUser][winUser] += score;

				m_lZuiScore[winUser][zuiIndex] = score;
				m_lZuiScore[m_wProvideUser][zuiIndex] = -score;
				strLog += std::to_string(m_wProvideUser) + " " + to_string(-score) + "		";


				strLog += "player: " + std::to_string(m_wProvideUser) + " score: " + to_string(-score);
				strLog += " handZuiCnt: " + std::to_string(winUserHandZuiCnt) + " winUserChooseZuiCnt: " + to_string(winUserChooseZuiCnt);
				strLog += " userChooseZuiCnt: " + std::to_string(userChooseZuiCnt) + "\n";

				strLog += "player: " + std::to_string(winUser) + " score: " + to_string(score);
				strLog += " handZuiCnt: " + std::to_string(winUserHandZuiCnt) + " winUserChooseZuiCnt: " + to_string(winUserChooseZuiCnt);
				strLog += " userChooseZuiCnt: " + std::to_string(userChooseZuiCnt) + "\n";
			}
			strLog += "\n";
		}

		_bstr_t bstrTmp(strLog.c_str());
		LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
		// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);

		CString strInfo(pszDescribeString);
		CGameLogic::NcaTextOut(strInfo);

		WORD times = m_GameLogic.GetZuiTimes(m_ChiHuRight[winUser]);

		string str = "CalZuiScore ";
		for (WORD i = 0; i < GAME_PLAYER; ++i)
		{
			if (!m_bPlayStatus[i])
				continue;

			if (m_pITableFrame->GetTableUserItem(i) == NULL)
				continue;

			lZuiScore[i] *= times;

			for (BYTE zuiIndex = 0; zuiIndex < MAX_ZUI; ++zuiIndex)
			{
				m_lZuiScore[i][zuiIndex] *= times;
			}

			str += "player " + std::to_string(i);
			str += "lZuiScore " + std::to_string(lZuiScore[i]);

			_bstr_t bstrTmp(str.c_str());
			LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
			// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);

			CString strInfo(pszDescribeString);
			// CGameLogic::NcaTextOut(strInfo);
		}
	}
}

string CTableFrameSink::getZuiStr(enZuiKind zuiKind)
{
	switch (zuiKind)
	{
	case TING:
		return "听 ";
	case KAN:
		return "坎 ";
		break;
	case Wen:
		return "文钱 ";
	case NIU:
		return "牛 ";
	case QIE:
		return "撇 ";
	case DUI:
		return "对 ";
	case BA:
		return "八 ";
	case ZAN:
		return "占 ";
	case JIE:
		return "接 ";
	default:
		break;
	}
	return " ";
}

WORD CTableFrameSink::GetSanBaoUser(WORD winUser, const CChiHuRight &chr)
{
	if (!(chr&CHR_PING_HU).IsEmpty())
	{
		return INVALID_CHAIR;
	}

	auto weaveCnt = m_cbWeaveItemCount[winUser];
	if (weaveCnt < 3)
	{
		return INVALID_CHAIR;
	}

	BYTE provideUserCnt[GAME_PLAYER] = { 0 };
	for (int i = 0; i < weaveCnt; ++i)
	{
		if (m_WeaveItemArray[winUser][i].cbWeaveKind != WIK_PENG)
		{
			continue;
		}

		provideUserCnt[m_WeaveItemArray[winUser][i].wProvideUser]++;
	}

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (provideUserCnt[i] >= 3)
		{
			return i;
		}
	}

	return INVALID_CHAIR;
}


//权位过滤
void CTableFrameSink::FiltrateRight(WORD wWinner, CChiHuRight &chr)
{
	//自摸
	if(wWinner == m_wProvideUser)
	{
		chr |= CHR_ZI_MO;

	}
// 	else if(m_cbGangStatus == WIK_MING_GANG)
// 	{
// 		chr |= CHR_QIANG_GANG_HU;
// 	}
// 	else
// 	{
// 		ASSERT(false);
// 	}

	if (m_wCurrentUser == INVALID_CHAIR && m_cbGangStatus == WIK_MING_GANG)
	{
		chr |= CHR_QIANG_GANG_HU;
	}

	//杠上花
	if (m_wCurrentUser == wWinner && m_cbGangStatus)
	{
		chr |= CHR_GANG_SHANG_HUA;
	}

	m_GameLogic.logChiHuRight(chr, "FiltrateRight");
}

//设置基数
void CTableFrameSink::SetGameBaseScore(LONG lBaseScore)
{
	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SET_BASESCORE,&lBaseScore,sizeof(lBaseScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SET_BASESCORE,&lBaseScore,sizeof(lBaseScore));
}

BYTE CTableFrameSink::GetRemainingCount(WORD wChairID,BYTE cbCardData)
{
	BYTE cbIndex = m_GameLogic.SwitchToCardIndex(cbCardData);
	BYTE nCount=0;
	for(int i=m_cbMinusLastCount;i<MAX_REPERTORY-m_cbMinusHeadCount;i++)
	{
		if(m_cbRepertoryCard[i] == cbCardData)
			nCount++;
	}
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if( i == wChairID)
			continue;

		nCount += m_cbCardIndex[i][cbIndex];
	}
	return nCount;
}

bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if( (m_pGameServiceOption->wServerType&GAME_GENRE_PERSONAL) != 0)//房卡模式
		return true;
	//自动托管
	OnUserTrustee(wChairID,true);


	return true;
}



//////////////////////////////////////////////////////////////////////////////////
