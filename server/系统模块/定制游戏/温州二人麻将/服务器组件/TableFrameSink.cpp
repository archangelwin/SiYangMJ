#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////
const WORD      CTableFrameSink::m_wPlayerCount=GAME_PLAYER;      //��Ϸ����

//���캯��
CTableFrameSink::CTableFrameSink()
{
  //��Ϸ����
  m_lSiceCount=0;
  m_wBankerUser=INVALID_CHAIR;
  ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
  ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
  ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
  m_cbQuanFeng = 0;
  m_cbQuanCounter = 1;
  m_cbLianZhuangCount = 0;
  m_wReplaceUser = INVALID_CHAIR;
  ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
  ZeroMemory(m_bTianDiHu,sizeof(m_bTianDiHu));
  ZeroMemory(m_bTuoPai,sizeof(m_bTuoPai));
  ZeroMemory(m_bMaiDi, sizeof(m_bMaiDi));
  ZeroMemory(m_bHaveMaiDi, sizeof(m_bHaveMaiDi));

  //������Ϣ
  m_cbOutCardData=0;
  m_cbOutCardCount=0;
  m_wOutCardUser=INVALID_CHAIR;
  ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
  ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

  //������Ϣ
  m_wHeapHead = INVALID_CHAIR;
  m_wHeapTail = INVALID_CHAIR;
  ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

  //������Ϣ
  m_bMagicIndex = MAX_INDEX;
  m_wMagicPos = INVALID_CHAIR;
  m_cbHeapMagic = 0;

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

  //�������
  m_pITableFrame=NULL;
  m_pGameServiceOption=NULL;

  //�������
  m_hControlInst = NULL;
  m_pServerControl = NULL;
  m_hControlInst = LoadLibrary(TEXT("SparrowWZExServerControl.dll"));
  if(m_hControlInst)
  {
    typedef void * (*CREATE)();
    CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl");
    if(ServerControl)
    {
      m_pServerControl = static_cast<IServerControl*>(ServerControl());
    }
  }

  return;

}

//��������
CTableFrameSink::~CTableFrameSink()
{
  if(m_pServerControl)
  {
    delete m_pServerControl;
    m_pServerControl = NULL;
  }

  if(m_hControlInst)
  {
    FreeLibrary(m_hControlInst);
    m_hControlInst = NULL;
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
  if(m_pITableFrame==NULL)
  {
    CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
    return false;
  }
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
  ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
  m_wReplaceUser = INVALID_CHAIR;
  ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
  ZeroMemory(m_bTianDiHu,sizeof(m_bTianDiHu));
  ZeroMemory(m_bTuoPai,sizeof(m_bTuoPai));
  ZeroMemory(m_bMaiDi, sizeof(m_bMaiDi));
  ZeroMemory(m_bHaveMaiDi, sizeof(m_bHaveMaiDi));

  //������Ϣ
  m_cbOutCardData=0;
  m_cbOutCardCount=0;
  m_wOutCardUser=INVALID_CHAIR;
  ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
  ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

  //������Ϣ
  m_wHeapHead = INVALID_CHAIR;
  m_wHeapTail = INVALID_CHAIR;
  ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

  //������Ϣ
  m_bMagicIndex = MAX_INDEX;
  m_wMagicPos = INVALID_CHAIR;
  m_cbHeapMagic = 0;

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
  //����״̬
  m_pITableFrame->SetGameStatus(GAME_SCENE_CHIP);

  //����ׯ��
  if(m_wBankerUser == INVALID_CHAIR)
  {
    m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),MAKEWORD(rand()%6+1,rand()%6+1));
  }
  else
  {
    m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),0);
  }

  //JJWillDo ע��
  //m_lSiceCount = MAKELONG(MAKEWORD(3,1),MAKEWORD(3,1));

  if(m_wBankerUser == INVALID_CHAIR)
  {
    WORD wSice = (WORD)(m_lSiceCount>>16);
    m_wBankerUser = (HIBYTE(wSice)+LOBYTE(wSice))%GAME_PLAYER;
  }

  //��������
  CMD_S_Chip  GameChip ;
  ZeroMemory(&GameChip,sizeof(GameChip));
  GameChip.wBankerUser = m_wBankerUser;
  GameChip.cbLianZhuangCount = m_cbLianZhuangCount;

  //��������
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP,&GameChip,sizeof(GameChip));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHIP,&GameChip,sizeof(GameChip));

  return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_NORMAL:    //�������
    {
      //��������
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //������Ϣ
      WORD wWinner = INVALID_CHAIR;
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
        if(m_dwChiHuKind[i] == WIK_CHI_HU)
        {
          wWinner = i;
        }
        GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
      }

      //�����������
      if(wWinner != INVALID_CHAIR)
      {
        //Ȩλ����
        FiltrateRight(wWinner,m_ChiHuRight[wWinner]);
      }

      //ͳ�ƻ���
      if(m_wProvideUser!=INVALID_CHAIR)
      {
        //������Ϣ
        GameEnd.wProvideUser=m_wProvideUser;
        GameEnd.cbProvideCard=m_cbProvideCard;

        m_ChiHuRight[wWinner].GetRightData(GameEnd.dwChiHuRight,MAX_RIGHT_COUNT);

        GameEnd.cbFanCount = CalScore(wWinner, m_wBankerUser, GameEnd.lGameScore);
      }
      else
      {
        //���ֽ���
        GameEnd.wProvideUser=m_wProvideUser;
        GameEnd.cbProvideCard=m_cbProvideCard;
      }

      //�û����ϻ���
      SCORE lUserScore[GAME_PLAYER];
      ZeroMemory(lUserScore,sizeof(lUserScore));
      CopyMemory(GameEnd.bMaidi,m_bMaiDi,sizeof(GameEnd.bMaidi));

      //ͳ�ƻ���
      LONGLONG lGameTax[GAME_PLAYER];
      ZeroMemory(lGameTax,sizeof(lGameTax));
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
        ASSERT(pUserItem != NULL);
        //�ۼӻ���
        GameEnd.lGameScore[i]*=m_pGameServiceOption->lCellScore;
        lUserScore[i] = pUserItem->GetUserScore();
      }

      // ��ҷ����� (���㷽��ֻ�������˷�)
      if(m_pGameServiceOption->wServerType &(GAME_GENRE_GOLD|SCORE_GENRE_POSITIVE))
      {
        //�Ǹ�
        for(WORD i=0; i<GAME_PLAYER; i++)
        {
          if(GameEnd.lGameScore[i]<0L)
          {
            if(lUserScore[i]+GameEnd.lGameScore[i]<0)
            {
              GameEnd.lGameScore[i] = -lUserScore[i];
              GameEnd.lGameScore[(i+1)%GAME_PLAYER] =lUserScore[i];
              break;
            }
          }
        }

        //ӮǮ���ó�������
        for(WORD i=0; i<GAME_PLAYER; i++)
        {
          if(GameEnd.lGameScore[i]>0L && GameEnd.lGameScore[i]>lUserScore[i])
          {
            GameEnd.lGameScore[i] = lUserScore[i];
            GameEnd.lGameScore[(i+1)%GAME_PLAYER] =-lUserScore[i];
            break;
          }
        }
      }


      tagScoreInfo ScoreInfoArray[GAME_PLAYER];
      ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

      //ͳ�ƻ���
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        BYTE ScoreKind;
        if(GameEnd.lGameScore[i] > 0L)
        {
          ScoreKind = SCORE_TYPE_WIN;
        }
        else if(GameEnd.lGameScore[i] < 0L)
        {
          ScoreKind = SCORE_TYPE_LOSE;
        }
        else
        {
          ScoreKind = SCORE_TYPE_DRAW;
        }
        //���û���
        if((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0)
        {
          if(GameEnd.lGameScore[i]>0L)
          {
            lGameTax[i] =m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
            GameEnd.lGameScore[i] -= lGameTax[i];
          }
        }

        ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];
        ScoreInfoArray[i].lRevenue = lGameTax[i];
        ScoreInfoArray[i].cbType   = ScoreKind;

        //������Ϣ
        m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
        m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      }

      //д�����
      m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

      //���ñ���
      if(wWinner != m_wBankerUser)
      {
        m_wBankerUser = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;
        m_cbLianZhuangCount = 0;
        if(++m_cbQuanCounter == 5)
        {
          m_cbQuanCounter = 1;
          if(++m_cbQuanFeng == 4)
          {
            m_cbQuanFeng = 0;
          }
        }
      }
      else
      {
        m_cbLianZhuangCount++;
        if(m_cbLianZhuangCount >= 4)
        {
          m_cbLianZhuangCount = 0;
          m_wBankerUser = INVALID_CHAIR;
        }
      }

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

      return true;
    }
    case GER_DISMISS:   //��Ϸ��ɢ
    {
      //��������
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //���ñ���
      GameEnd.wProvideUser=INVALID_CHAIR;

      //�����˿�
      for(WORD i=0; i<GAME_PLAYER; i++)
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
    case GER_USER_LEAVE:    //�û�ǿ��
    case GER_NETWORK_ERROR:
    {
      //��������
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //���ñ���
      GameEnd.wProvideUser=INVALID_CHAIR;

      //�����˿�
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
      }

      //����
      GameEnd.lGameScore[wChairID] = -30L*m_pGameServiceOption->lCellScore;

      // ��ҷǸ�
      if(m_pGameServiceOption->wServerType &(GAME_GENRE_GOLD|SCORE_GENRE_POSITIVE))
      {
        IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(wChairID);
        ASSERT(pUserItem != NULL);
        if(pUserItem->GetUserScore()+GameEnd.lGameScore[wChairID]<0)
        {
          GameEnd.lGameScore[wChairID] = -pUserItem->GetUserScore();
        }
      }
      //֪ͨ��Ϣ
      TCHAR szMessage[128]=TEXT("");
      _sntprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetNickName());
      m_pITableFrame->SendGameMessage(szMessage,SMT_CHAT);

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

      //д�����
      tagScoreInfo ScoreInfoArray;
      ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
      ScoreInfoArray.lScore=GameEnd.lGameScore[wChairID];
      ScoreInfoArray.cbType=SCORE_TYPE_FLEE;
      m_pITableFrame->WriteUserScore(wChairID,ScoreInfoArray);

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
  switch(cbGameStatus)
  {
    case GAME_SCENE_FREE: //����״̬
    {
      //��������
      CMD_S_StatusFree StatusFree;
      ZeroMemory(&StatusFree,sizeof(StatusFree));

      //��������
      StatusFree.wBankerUser=m_wBankerUser;
      StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
      CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
    }
    case GAME_SCENE_CHIP: //���״̬
    {
      //��������
      CMD_S_StatusChip StatusChip;
      ZeroMemory(&StatusChip,sizeof(StatusChip));

      //��������
      StatusChip.wBankerUser=m_wBankerUser;
      StatusChip.lCellScore=m_pGameServiceOption->lCellScore;
      StatusChip.cbLianZhuangCount=m_cbLianZhuangCount;
      CopyMemory(StatusChip.bTrustee,m_bTrustee,sizeof(m_bTrustee));
      CopyMemory(StatusChip.bMaiDi,m_bMaiDi,sizeof(m_bMaiDi));
      CopyMemory(StatusChip.bHaveMaiDi,m_bHaveMaiDi,sizeof(m_bHaveMaiDi));

      if(pIServerUserItem)
      {
        //�����û�
        if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
        {
          OnCheatCard(pIServerUserItem);
        }
      }

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusChip,sizeof(StatusChip));

    }
    case GAME_SCENE_PLAY: //��Ϸ״̬
    {
      //��������
      CMD_S_StatusPlay StatusPlay;
      ZeroMemory(&StatusPlay,sizeof(StatusPlay));

      //��Ϸ����
      StatusPlay.wBankerUser=m_wBankerUser;
      StatusPlay.cbLianZhuangCount = m_cbLianZhuangCount;
      StatusPlay.wCurrentUser=m_wCurrentUser;
      StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
      StatusPlay.wReplaceUser = m_wReplaceUser;
      CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
      StatusPlay.cbQuanFeng = m_cbQuanFeng;
      StatusPlay.bTuoPai = (wChairID!=m_wCurrentUser?false:m_bTuoPai[m_wCurrentUser]);

      //״̬����
      StatusPlay.cbActionCard=m_cbProvideCard;
      StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
      StatusPlay.cbActionMask=(m_bResponse[wChairID]==false)?m_cbUserAction[wChairID]:WIK_NULL;

      //����״̬
      CopyMemory(StatusPlay.cbHearStatus,m_cbListenStatus,sizeof(m_cbListenStatus));

      //��ʷ��¼
      StatusPlay.wOutCardUser=m_wOutCardUser;
      StatusPlay.cbOutCardData=m_cbOutCardData;
      CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
      CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

      //����˿�
      CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
      CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

      //������Ϣ
      StatusPlay.wHeapHead = m_wHeapHead;
      StatusPlay.wHeapTail = m_wHeapTail;
      CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

      //�˿�����
      StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],StatusPlay.cbCardData);
      if(m_wReplaceUser == wChairID)
      {
        StatusPlay.cbSendCardData = m_cbSendCardData;
      }
      else
      {
        StatusPlay.cbSendCardData=(m_wProvideUser==wChairID)?m_cbProvideCard:0x00;
      }

      //�������
      StatusPlay.bMagicIndex = m_bMagicIndex;
      StatusPlay.wMagicPos = m_wMagicPos;
      StatusPlay.cbHeapMagic = m_cbHeapMagic;

      if(pIServerUserItem)
      {
        //�����û�
        if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
        {
          OnCheatCard(pIServerUserItem);
        }
      }

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

    }
  }

  //�������
  ASSERT(FALSE);

  return false;
}


//�����û�
bool CTableFrameSink::OnCheatCard(IServerUserItem * pIServerUserItem)
{
  if(m_pServerControl)
  {
    CMD_S_CheatCard CheatCard ;
    ZeroMemory(&CheatCard, sizeof(CheatCard));

    BYTE bCardData[MAX_COUNT];
    BYTE bCardCount=0;
    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      WORD wShowPlayer=(i+1)%GAME_PLAYER;

      CheatCard.wCardUser[CheatCard.cbUserCount] = i;
      bCardCount = m_GameLogic.SwitchToCardData(m_cbCardIndex[wShowPlayer],bCardData);
      CopyMemory(CheatCard.cbCardIndex[wShowPlayer],m_cbCardIndex[wShowPlayer],MAX_INDEX);

      CheatCard.cbCardCount[CheatCard.cbUserCount] = bCardCount;
      CheatCard.cbUserCount++;
    }
    //��������
    //WB�� ����������������BUG
    //m_pITableFrame->SendTableData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
    //m_pITableFrame->SendLookonData(wChairID,SUB_S_CHEAT_CARD,&CheatCard,sizeof(CheatCard));
    m_pServerControl->ServerControl(&CheatCard, m_pITableFrame, pIServerUserItem);
  }

  return true;
}
//����������Ϣ
void CTableFrameSink::SendCheatCard()
{
  if(m_pServerControl)
  {
    CMD_S_CheatCard CheatCard ;
    ZeroMemory(&CheatCard, sizeof(CheatCard));

    BYTE bCardData[MAX_COUNT];
    BYTE bCardCount=0;
    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      WORD wShowPlayer=(i+1)%GAME_PLAYER;

      CheatCard.wCardUser[CheatCard.cbUserCount] = i;
      bCardCount = m_GameLogic.SwitchToCardData(m_cbCardIndex[wShowPlayer],bCardData);
      CopyMemory(CheatCard.cbCardIndex[wShowPlayer],m_cbCardIndex[wShowPlayer],MAX_INDEX);

      CheatCard.cbCardCount[CheatCard.cbUserCount] = bCardCount;
      CheatCard.cbUserCount++;
    }
    m_pServerControl->ServerControl(&CheatCard, m_pITableFrame);
  }
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
  return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  switch(wSubCmdID)
  {
    case SUB_C_OUT_CARD:    //������Ϣ
    {
      //Ч����Ϣ
      ASSERT(wDataSize==sizeof(CMD_C_OutCard));
      if(wDataSize!=sizeof(CMD_C_OutCard))
      {
        return false;
      }

      //�û�Ч��
      if(pIServerUserItem->GetUserStatus()!=US_PLAYING)
      {
        return true;
      }

      //��Ϣ����
      CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
      return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
    }
    case SUB_C_OPERATE_CARD:  //������Ϣ
    {
      //Ч����Ϣ
      ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
      if(wDataSize!=sizeof(CMD_C_OperateCard))
      {
        return false;
      }

      //�û�Ч��
      if(pIServerUserItem->GetUserStatus()!=US_PLAYING)
      {
        return true;
      }

      //��Ϣ����
      CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pData;
      return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
    }
    case SUB_C_REPLACE_CARD:  //�û�����
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_C_ReplaceCard));
      if(wDataSize!=sizeof(CMD_C_ReplaceCard))
      {
        return false;
      }

      //�û�Ч��
      if(pIServerUserItem->GetUserStatus()!=US_PLAYING)
      {
        return true;
      }

      //��������
      CMD_C_ReplaceCard * pReplaceCard=(CMD_C_ReplaceCard *)pData;

      //��Ϣ����
      return OnUserReplaceCard(pIServerUserItem->GetChairID(),pReplaceCard->cbCardData);
    }
    case SUB_C_TRUSTEE:
    {
      CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pData;
      if(wDataSize != sizeof(CMD_C_Trustee))
      {
        return false;
      }

      m_bTrustee[pIServerUserItem->GetChairID()]=pTrustee->bTrustee;
      CMD_S_Trustee Trustee;
      Trustee.bTrustee=pTrustee->bTrustee;
      Trustee.wChairID = pIServerUserItem->GetChairID();
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

      return true;
    }
    case SUB_C_LISTEN:
    {
      //Ч����Ϣ
      ASSERT(wDataSize==sizeof(CMD_C_Listen));
      if(wDataSize!=sizeof(CMD_C_Listen))
      {
        return false;
      }

      //�û�Ч��
      if(pIServerUserItem->GetUserStatus()!=US_PLAYING)
      {
        return true;
      }

      CMD_C_Listen *pListen = (CMD_C_Listen *)pData;
      return OnUserListenCard(pIServerUserItem->GetChairID(),pListen->cbListen);
    }
    case SUB_C_CHIP:
    {
      //Ч����Ϣ
      ASSERT(wDataSize==sizeof(CMD_C_Chip));
      if(wDataSize!=sizeof(CMD_C_Chip))
      {
        return false;
      }

      //�û�Ч��
      if(pIServerUserItem->GetUserStatus()!=US_PLAYING)
      {
        return true;
      }

      //����У��
      if(m_pITableFrame->GetGameStatus()!=GAME_SCENE_CHIP)
      {
        return true;
      }

      CMD_C_Chip *pChip = (CMD_C_Chip *)pData;
      return OnUserChip(pIServerUserItem->GetChairID(),pChip->bMaiDi);
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

  return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{

  //WORD wChairID=pIServerUserItem->GetChairID();
  ASSERT(wChairID!=INVALID_CHAIR);
  //ׯ������
  if(bLookonUser==false)
  {
    m_wBankerUser = INVALID_CHAIR;

    m_cbQuanCounter = 1;
    m_cbLianZhuangCount = 0;

    m_bTrustee[wChairID]=false;
    CMD_S_Trustee Trustee;
    Trustee.bTrustee=false;
    Trustee.wChairID = wChairID;
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
  }
  return true;
}

//�û�����
bool CTableFrameSink::OnUserListenCard(WORD wChairID,BYTE cbListen)
{
  //Ч��״̬
  ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
  if(m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY)
  {
    return true;
  }

  //Ч�����
  ASSERT((wChairID==m_wCurrentUser)&&(m_cbListenStatus[wChairID]==0));
  if((wChairID!=m_wCurrentUser)||(m_cbListenStatus[wChairID]!=0))
  {
    return false;
  }

  //���ñ���
  m_cbListenStatus[wChairID] = cbListen;
  m_bEnjoinChiPeng[wChairID] = true;

  //��������
  CMD_S_Listen ListenCard;
  ListenCard.wChairId=wChairID;

  //������Ϣ
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));

  return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
  //Ч��״̬
  ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
  if(m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY)
  {
    return true;
  }

  //�������
  ASSERT(wChairID==m_wCurrentUser);
  ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

  //Ч�����
  if(wChairID!=m_wCurrentUser)
  {
    return false;
  }
  if(m_GameLogic.IsValidCard(cbCardData)==false)
  {
    return false;
  }

  //�����ж�
  //if (m_bTuoPai[wChairID] && m_GameLogic.IsTuoPai(cbCardData, &m_WeaveItemArray[wChairID][m_cbWeaveItemCount[wChairID]-1]))
  //  return false;

  //ɾ���˿�
  if(m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
  {
    ASSERT(FALSE);
    return false;
  }

  //���ñ���
  m_bSendStatus=true;
  m_bGangStatus = false;
  m_cbUserAction[wChairID]=WIK_NULL;
  m_cbPerformAction[wChairID]=WIK_NULL;
  m_bTianDiHu[wChairID] = true;
  ZeroMemory(m_bTuoPai, sizeof(m_bTuoPai));

  //���Ƽ�¼
  m_cbOutCardCount++;
  m_wOutCardUser=wChairID;
  m_cbOutCardData=cbCardData;

  //��������
  CMD_S_OutCard OutCard;
  OutCard.wOutCardUser=wChairID;
  OutCard.cbOutCardData=cbCardData;



  //������Ϣ
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
    ASSERT(pUserItem != NULL);

    ZeroMemory(OutCard.cbCheatCardData,sizeof(OutCard.cbCheatCardData));
    OutCard.cbCheatCardCount=0;

    //�����û�
    if(CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))
    {
      BYTE bCardData[MAX_COUNT];
      BYTE bCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[(i+1)%GAME_PLAYER],bCardData);
      CopyMemory(OutCard.cbCheatCardData,bCardData,bCardCount);
      OutCard.cbCheatCardCount=bCardCount;
    }

    //������Ϣ
    m_pITableFrame->SendTableData(i,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
    m_pITableFrame->SendLookonData(i,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
  }


  m_wProvideUser=wChairID;
  m_cbProvideCard=cbCardData;

  //�û��л�
  m_wCurrentUser=(wChairID+m_wPlayerCount-1)%GAME_PLAYER;

  //��Ӧ�ж�
  bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

  //�ɷ��˿�
  if(bAroseAction==false)
  {
    DispatchCardData(m_wCurrentUser);
  }

  return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
  //Ч��״̬
  ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
  if(m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY)
  {
    return true;
  }

  //Ч���û�
  ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
  if((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR))
  {
    return true;
  }

  //��������
  if(cbOperateCode != WIK_CHI_HU)
  {
    m_bTianDiHu[wChairID] = true;
  }

  //��������
  if(m_wCurrentUser==INVALID_CHAIR)
  {
    //Ч��״̬
    ASSERT(m_bResponse[wChairID]==false);
    ASSERT(m_cbUserAction[wChairID] != WIK_NULL);
    ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

    //Ч��״̬
    if(m_bResponse[wChairID]==true)
    {
      return true;
    }
    if(m_cbUserAction[wChairID] == WIK_NULL)
    {
      return true;
    }
    if((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0))
    {
      return true;
    }

    //��������
    WORD wTargetUser=wChairID;
    BYTE cbTargetAction=cbOperateCode;

    //���ñ���
    m_bResponse[wChairID]=true;
    m_cbPerformAction[wChairID]=cbOperateCode;
    if(cbOperateCard[0]==0)
    {
      m_cbOperateCard[wChairID][0] = m_cbProvideCard;
    }
    else
    {
      CopyMemory(m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]));
    }

    //ִ���ж�
    for(WORD i=0; i<m_wPlayerCount; i++)
    {
      //��ȡ����
      BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

      //���ȼ���
      BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
      BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

      //�����ж�
      if(cbUserActionRank>cbTargetActionRank)
      {
        wTargetUser=i;
        cbTargetAction=cbUserAction;
      }
    }
    if(m_bResponse[wTargetUser]==false)
    {
      return true;
    }

    //�Ժ��ȴ�
    if(cbTargetAction==WIK_CHI_HU)
    {
      for(WORD i=0; i<m_wPlayerCount; i++)
      {
        if((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
        {
          return true;
        }
      }
    }

    //��������
    if(cbTargetAction==WIK_NULL)
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
    if(cbTargetAction==WIK_CHI_HU)
    {
      //������Ϣ
      m_cbChiHuCard=cbTargetCard;

      for(WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER; i!=m_wProvideUser; i = (i+m_wPlayerCount-1)%GAME_PLAYER)
      {
        //�����ж�
        if((m_cbPerformAction[i]&WIK_CHI_HU)==0)
        {
          continue;
        }

        //�����ж�
        BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
        tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
        m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i]);

        //�����˿�
        if(m_dwChiHuKind[i]!=WIK_NULL)
        {
          m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
          wTargetUser = i;
//          break;
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
    if(cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
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
        m_WeaveItemArray[wTargetUser][wIndex].cbCardData[3] = cbTargetCard;
      }
    }

    //ɾ���˿�
    switch(cbTargetAction)
    {
      case WIK_LEFT:    //���Ʋ���
      {
        //ɾ���˿�
        if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2))
        {
          ASSERT(FALSE);
          return false;
        }

        break;
      }
      case WIK_RIGHT:   //���Ʋ���
      {
        //ɾ���˿�
        if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2))
        {
          ASSERT(FALSE);
          return false;
        }

        break;
      }
      case WIK_CENTER:  //���Ʋ���
      {
        //ɾ���˿�
        if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2))
        {
          ASSERT(FALSE);
          return false;
        }

        break;
      }
      case WIK_PENG:    //���Ʋ���
      {
        //ɾ���˿�
        BYTE cbRemoveCard[]= {cbTargetCard,cbTargetCard};
        if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2))
        {
          ASSERT(FALSE);
          return false;
        }

        break;
      }
      case WIK_GANG:    //���Ʋ���
      {
        //ɾ���˿�,��������ֻ���ڷŸ�
        BYTE cbRemoveCard[]= {cbTargetCard,cbTargetCard,cbTargetCard};
        if(!m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)))
        {
          ASSERT(FALSE);
          return false;
        }

        break;
      }
      default:
        ASSERT(FALSE);
        return false;
    }

    //������
    CMD_S_OperateResult OperateResult;
    ZeroMemory(&OperateResult,sizeof(OperateResult));
    OperateResult.wOperateUser=wTargetUser;
    OperateResult.cbOperateCode=cbTargetAction;
    OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
    OperateResult.cbOperateCard[0] = cbTargetCard;
    OperateResult.bTuoPai = m_bTuoPai[wTargetUser];
    if(cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
    {
      CopyMemory(&OperateResult.cbOperateCard[1],&m_cbOperateCard[wTargetUser][1],2*sizeof(BYTE));
    }
    else if(cbTargetAction&WIK_PENG)
    {
      OperateResult.cbOperateCard[1] = cbTargetCard;
      OperateResult.cbOperateCard[2] = cbTargetCard;
    }

    //������Ϣ
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

    //�û�״̬
    ZeroMemory(m_bResponse,sizeof(m_bResponse));
    ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
    ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
    ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

    //�����û�
    m_wCurrentUser=wTargetUser;

    //���ƴ���
    if(cbTargetAction==WIK_GANG)
    {
      m_bGangStatus = true;
      DispatchCardData(wTargetUser,true);
    }

    return true;
  }

  //��������
  if(m_wCurrentUser==wChairID)
  {
    //Ч�����
    ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
    if((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
    {
      return false;
    }

    //�˿�Ч��
    ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
    if((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false))
    {
      return false;
    }

    //���ñ���
    m_bSendStatus=true;
    m_cbUserAction[m_wCurrentUser]=WIK_NULL;
    m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

    bool bPublic=false;

    //ִ�ж���
    switch(cbOperateCode)
    {
      case WIK_GANG:      //���Ʋ���
      {
        //��������
        BYTE cbWeaveIndex=0xFF;
        BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

        //���ƴ���
        if(m_cbCardIndex[wChairID][cbCardIndex]==1)
        {
          //Ѱ�����
          for(BYTE i=0; i<m_cbWeaveItemCount[wChairID]; i++)
          {
            BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
            BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
            if((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
            {
              bPublic=true;
              cbWeaveIndex=i;
              break;
            }
          }

          //Ч�鶯��
          ASSERT(cbWeaveIndex!=0xFF);
          if(cbWeaveIndex==0xFF)
          {
            return false;
          }

          //����˿�
          m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
          m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
          m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
          m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];
        }
        else
        {
          //�˿�Ч��
          ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
          if(m_cbCardIndex[wChairID][cbCardIndex]!=4)
          {
            return false;
          }

          //���ñ���
          bPublic=false;
          cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
          m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
          m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
          m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
          m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
          for(BYTE j = 0; j < 4; j++)
          {
            m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];
          }
        }

        //ɾ���˿�
        m_cbCardIndex[wChairID][cbCardIndex]=0;

        m_bGangStatus = true;

        //������
        CMD_S_OperateResult OperateResult;
        ZeroMemory(&OperateResult,sizeof(OperateResult));
        OperateResult.wOperateUser=wChairID;
        OperateResult.wProvideUser=wChairID;
        OperateResult.cbOperateCode=cbOperateCode;
        OperateResult.cbOperateCard[0]=cbOperateCard[0];

        //������Ϣ
        m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
        m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

        //Ч�鶯��
        bool bAroseAction=false;
        if(bPublic==true)
        {
          bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);
        }

        //�����˿�
        if(bAroseAction==false)
        {
          DispatchCardData(wChairID,true);
        }

        return true;
      }
      case WIK_CHI_HU:    //�Ժ�����
      {
        //����Ȩλ
        if(m_cbOutCardCount==0)
        {
          m_wProvideUser = m_wCurrentUser;
          m_cbProvideCard = m_cbSendCardData;
        }

        //��ͨ����
        BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
        tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
        if(!m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1))
        {
          ASSERT(FALSE);
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

//�û�����
bool CTableFrameSink::OnUserReplaceCard(WORD wChairID, BYTE cbCardData)
{
  //�������
  ASSERT(wChairID==m_wReplaceUser);
  ASSERT(m_GameLogic.IsHuaCard(cbCardData)==true);
  ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

  //Ч�����
  if(wChairID!=m_wReplaceUser)
  {
    return false;
  }
  if(m_GameLogic.IsHuaCard(cbCardData)==false)
  {
    return false;
  }
  if(m_GameLogic.IsValidCard(cbCardData)==false)
  {
    return false;
  }

  //ɾ���˿�
  if(m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
  {
    ASSERT(FALSE);
    return false;
  }

  //״̬����
  m_bSendStatus=true;
  m_cbSendCardData=0x00;

  //�����˿�
  //m_cbDiscardCount[wChairID]++;
  //m_cbDiscardCard[wChairID][m_cbDiscardCount[wChairID]-1]=cbCardData;

  //��������
  CMD_S_ReplaceCard ReplaceCard;
  ReplaceCard.wReplaceUser=wChairID;
  ReplaceCard.cbReplaceCard=cbCardData;

  //������Ϣ
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

  //�ɷ��˿�
  DispatchCardData(wChairID, true);

  return true;
}

//�û����
bool CTableFrameSink::OnUserChip(WORD wChairID, BYTE cbMaidi)
{
  //��������
  ASSERT(!m_bHaveMaiDi[wChairID]);
  if(m_bHaveMaiDi[wChairID])
  {
    return true;
  }

  //���ñ���
  m_bMaiDi[wChairID] = cbMaidi;
  m_bHaveMaiDi[wChairID] = true;

  //������Ϣ
  CMD_S_Chip_Result chipResult;
  ZeroMemory(&chipResult,sizeof(chipResult));
  chipResult.bTotal = false;
  chipResult.wMaiDiUser = wChairID;
  chipResult.bMaidi[wChairID] = cbMaidi;

  //������Ϣ
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_RESULT,&chipResult,sizeof(chipResult));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHIP_RESULT,&chipResult,sizeof(chipResult));

  if(IsAllState(m_bHaveMaiDi))
  {
    //������Ϣ
    CMD_S_Chip_Result chipResultTotal;
    ZeroMemory(&chipResultTotal,sizeof(chipResultTotal));
    chipResultTotal.bTotal = true;
    memcpy(chipResultTotal.bMaidi, m_bMaiDi, sizeof(m_bMaiDi));

    //������Ϣ
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHIP_RESULT,&chipResultTotal,sizeof(chipResultTotal));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHIP_RESULT,&chipResultTotal,sizeof(chipResultTotal));

    //���㶥��
    if(m_bMaiDi[m_wBankerUser] != 0)
    {
      for(int i = 0; i < GAME_PLAYER; i++)
      {
        if(i != m_wBankerUser && m_bMaiDi[i] != 0)
        {
          m_bMaiDi[i] += m_bMaiDi[m_wBankerUser];
        }
      }
    }

    //����״̬
    m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

    m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
    m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

    //�ַ��˿�
    for(WORD i=0; i<m_wPlayerCount; i++)
    {
      m_cbLeftCardCount-=(MAX_COUNT-1);
      m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
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
    BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
    WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
    while(cbSiceTakeCount*2 > HEAP_FULL_COUNT)
    {
      wTakeChairID = (wTakeChairID+1)%4;
      cbSiceTakeCount -= HEAP_FULL_COUNT/2;
    }
    m_wHeapTail = wTakeChairID;
    BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
    for(WORD i=0; i<4; i++)
    {
      //������Ŀ
      BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
      BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

      //��ȡ�˿�
      cbTakeCount-=cbRemoveCount;
      m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

      //����ж�
      if(cbTakeCount==0)
      {
        m_wHeapHead=wTakeChairID;
        break;
      }
      //�л�����
      wTakeChairID=(wTakeChairID+1)%4;
    }

    //���ò���
    WORD wMagicSice = wSice;
    BYTE cbMagicSiceCount = min(HIBYTE(wMagicSice),LOBYTE(wMagicSice));
    BYTE bMagicData =  m_cbRepertoryCard[cbMagicSiceCount*2-1];

    m_bMagicIndex = m_GameLogic.SwitchToCardIndex(bMagicData);
    m_GameLogic.SetMagicIndex(m_bMagicIndex);

    if(cbMagicSiceCount > cbSiceTakeCount)
    {
      m_wMagicPos = (m_wHeapTail+3) % 4;
      m_cbHeapMagic = (cbMagicSiceCount-1-cbSiceTakeCount) * 2;
    }
    else
    {
      m_wMagicPos = m_wHeapTail;
      m_cbHeapMagic = m_cbHeapCardInfo[m_wHeapTail][1] + cbMagicSiceCount*2 - 2;
    }

    //��������
    bool bAroseAction=false;

    //ׯ���ж�
    if(m_wReplaceUser==INVALID_CHAIR)
    {
      //�����ж�
      tagGangCardResult GangCardResult;
      m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

      //�����ж�
      CChiHuRight chr;
      m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
      m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr);
      m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
    }

    //��������
    CMD_S_GameStart GameStart;
    GameStart.lSiceCount=m_lSiceCount;
    GameStart.wBankerUser=m_wBankerUser;
    GameStart.wReplaceUser = m_wReplaceUser;
    GameStart.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
    GameStart.cbQuanFeng = m_cbQuanFeng;
    GameStart.wHeapHead = m_wHeapHead;
    GameStart.wHeapTail = m_wHeapTail;
    GameStart.bMagicIndex = m_bMagicIndex;
    GameStart.wMagicPos = m_wMagicPos;
    GameStart.cbHeapMagic = m_cbHeapMagic;
    GameStart.cbLianZhuangCount = m_cbLianZhuangCount;
    CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

    //��������
    for(WORD i=0; i<m_wPlayerCount; i++)
    {
      //���ñ���
      GameStart.cbUserAction=m_cbUserAction[i];
      m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

      //��������
      m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
      m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
    }

    //WB��
    SendCheatCard();
  }

  return true;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
  //������ʾ
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    if(m_cbUserAction[i]!=WIK_NULL)
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
  if(wCurrentUser==INVALID_CHAIR)
  {
    return false;
  }
  ASSERT(m_bSendStatus);
  if(m_bSendStatus == false)
  {
    return false;
  }

  //�����˿�
  if((m_wReplaceUser==INVALID_CHAIR)&&(m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
  {
    m_cbDiscardCount[m_wOutCardUser]++;
    m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
  }

  //��ׯ����
  if(m_cbLeftCardCount==m_cbEndLeftCount)
  {
    m_cbChiHuCard=0;
    m_wProvideUser=INVALID_CHAIR;
    OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

    return true;
  }

  //���ñ���
  if(m_wReplaceUser == INVALID_CHAIR)
  {
    m_cbOutCardData=0;
    m_wCurrentUser=wCurrentUser;
    m_wOutCardUser=INVALID_CHAIR;
  }

  //�����˿�
  m_cbSendCardCount++;
  m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];

  //����λ��
  BYTE bMagicCount = min(HIBYTE(WORD(m_lSiceCount&0xffff)),LOBYTE(WORD(m_lSiceCount&0xffff)));
  if(m_cbLeftCardCount == bMagicCount*2-1)
  {
    m_cbSendCardCount++;
    m_cbSendCardData = m_cbRepertoryCard[--m_cbLeftCardCount];
  }

  //����
  m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

  m_wReplaceUser=INVALID_CHAIR;

  if(m_wCurrentUser == wCurrentUser)
  {
    //���ñ���
    m_wProvideUser=wCurrentUser;
    m_cbProvideCard=m_cbSendCardData;
  }

  if(m_wReplaceUser == INVALID_CHAIR)
  {
    if(!m_bEnjoinChiHu[m_wCurrentUser])
    {
      //�����ж�
      CChiHuRight chr;
      m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
      BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
                      m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr);
      m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

      //���4̨
      if(cbAction&WIK_CHI_HU)
      {
        FiltrateRight(m_wCurrentUser, chr);
        BYTE bFanshu = m_GameLogic.GetChiHuActionRank(chr, true);
      }

      m_cbUserAction[m_wCurrentUser] |= cbAction;
    }

    //�����ж�
    if((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
    {
      tagGangCardResult GangCardResult;
      m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
                                      m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
    }
  }

  //������Ϣ
  ASSERT(m_wHeapHead != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR);
  if(!bTail)
  {
    //�л�����
    BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHead][0]+m_cbHeapCardInfo[m_wHeapHead][1];
    if(cbHeapCount==HEAP_FULL_COUNT)
    {
      m_wHeapHead=(m_wHeapHead+1)%4;
    }
    m_cbHeapCardInfo[m_wHeapHead][0]++;
  }
  else
  {
    //�л�����
    BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
    if(cbHeapCount==HEAP_FULL_COUNT)
    {
      m_wHeapTail=(m_wHeapTail+3)%4;
    }
    m_cbHeapCardInfo[m_wHeapTail][1]++;
  }

  //��������
  CMD_S_SendCard SendCard;
  SendCard.wSendCardUser = wCurrentUser;
  SendCard.wReplaceUser = m_wReplaceUser;
  SendCard.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
  SendCard.bTail = bTail;
  SendCard.cbActionMask=m_wReplaceUser==INVALID_CHAIR?m_cbUserAction[m_wCurrentUser]:WIK_NULL;
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
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    //�û�����
    if(wCenterUser==i)
    {
      continue;
    }

    //��������
    if(EstimatKind==EstimatKind_OutCard)
    {
      //�����ж�
      if(m_bEnjoinChiPeng[i]==false)
      {
        //�����ж�
        m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

        //�����ж�
        WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
        if(wEatUser==i)
        {
          m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
        }

        //�����ж�
        if(m_cbLeftCardCount>m_cbEndLeftCount)
        {
          m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
        }
      }
    }

    //�����ж�
    if(m_bEnjoinChiHu[i]==false)
    {
      //�Ժ��ж�
      CChiHuRight chr;
      BYTE cbWeaveCount=m_cbWeaveItemCount[i];
      BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr);

      //���4̨
      if(cbAction&WIK_CHI_HU)
      {
        FiltrateRight(i, chr);
        BYTE bFanshu = m_GameLogic.GetChiHuActionRank(chr, true);
      }

      m_cbUserAction[i] |= cbAction;
    }

    //����ж�
    if(m_cbUserAction[i]!=WIK_NULL)
    {
      bAroseAction=true;
    }
  }

  //�������
  if(bAroseAction==true)
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
BYTE CTableFrameSink::CalScore(WORD wWinner, WORD wBanker, LONGLONG lScore[GAME_PLAYER])
{
  //��ʼ��
  ZeroMemory(lScore,sizeof(lScore[0])*GAME_PLAYER);

  //��������
  WORD wFanShu = 0;
  LONGLONG lMagicScore[GAME_PLAYER];
  ZeroMemory(lMagicScore,sizeof(lMagicScore));
  LONGLONG lLianZhuangScore = 2 * (m_cbLianZhuangCount + 1);
  LONGLONG lMaidiMul[GAME_PLAYER] = {0};

  //���Ʒ���
  wFanShu = m_GameLogic.GetChiHuActionRank(m_ChiHuRight[wWinner]);
  ASSERT(wFanShu!=0);
  if(wFanShu == 0)
  {
    wFanShu = 1;
  }

  //�������
  for(int i = 0; i < GAME_PLAYER; i++)
  {
    LONGLONG lMagicCount = m_cbCardIndex[i][m_bMagicIndex];
    if(i == wWinner && !(m_ChiHuRight[wWinner]&CHR_THREE_KING).IsEmpty())
    {
      lMagicCount *= 2;
    }
    for(int j = 0; j < GAME_PLAYER; j++)
    {
      if(j == i)
      {
        lMagicScore[j] += lMagicCount * (GAME_PLAYER-1);
      }
      else
      {
        lMagicScore[j] -= lMagicCount;
      }
    }
  }

  //��ױ���
  if(wWinner == wBanker)
  {
    for(int i = 0; i < GAME_PLAYER; i++)
    {
      if(i == wWinner)
      {
        continue;
      }
      lMaidiMul[i] = (m_bMaiDi[i]==0?m_bMaiDi[wBanker]:m_bMaiDi[i]);
    }
  }
  else
  {
    for(int i = 0; i < GAME_PLAYER; i++)
    {
      if(i != wBanker)
      {
        continue;
      }
      lMaidiMul[i] = m_bMaiDi[wWinner];
    }
  }

  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(i != wWinner)
    {
      BYTE bZhuang = ((wWinner==wBanker||i==wBanker)?2:1);
      LONGLONG lGameScore = wFanShu * (lLianZhuangScore/2*lMaidiMul[i] + lLianZhuangScore*bZhuang) - lMagicScore[i];
      lScore[i] = -lGameScore;
      lScore[wWinner] += lGameScore;
    }
  }

  return BYTE(wFanShu);
}

//��ϵͳ���ƻ�ȡ��������
BYTE CTableFrameSink::GetMagicIndex(BYTE cbFanCard)
{
  BYTE cbZPaiIndex = m_GameLogic.SwitchToCardIndex(cbFanCard);
  if(cbZPaiIndex < 27)
  {
    if((cbZPaiIndex+1)%9 == 0)
    {
      cbZPaiIndex -= 8;
    }
    else
    {
      cbZPaiIndex++;
    }
  }
  else
  {
    if(cbZPaiIndex + 1 == MAX_INDEX)
    {
      cbZPaiIndex = 27;
    }
    else
    {
      cbZPaiIndex++;
    }
  }
  return cbZPaiIndex;
}

//��ȡ��������
BYTE CTableFrameSink::GetDiscardCount(BYTE cbCardData)
{
  BYTE cbCount = 0;
  //����������
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    for(BYTE j = 0; j < m_cbDiscardCount[i]; j++)
    {
      if(m_cbDiscardCard[i][j] == cbCardData)
        if(++cbCount == 4)
        {
          break;
        }
    }
  }
  if(cbCount < 4)
  {
    //������
    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      for(BYTE j = 0; j < m_cbWeaveItemCount[i]; j++)
      {
        if(!m_WeaveItemArray[i][j].cbPublicCard)
        {
          continue;
        }
        BYTE cbCardBuffer[4];
        BYTE cbCardCount = m_GameLogic.GetWeaveCard(m_WeaveItemArray[i][j].cbWeaveKind,
                           m_WeaveItemArray[i][j].cbCenterCard,cbCardBuffer);
        for(BYTE k = 0; k < cbCardCount; k++)
          if(cbCardBuffer[k] == cbCardData)
            if(++cbCount == 4)
            {
              break;
            }
      }
    }
  }

  return cbCount;
}

//Ȩλ����
void CTableFrameSink::FiltrateRight(WORD wWinner, CChiHuRight &chr)
{
  /*  ����Ȩλ  */

  //����
  if(wWinner == m_wProvideUser)
  {
    chr |= CHR_ZI_MO;
    chr |= CHR_YING_HU;
  }

  //��غ�
  if(!m_bTianDiHu[wWinner])
  {
    if(wWinner == m_wBankerUser)
    {
      chr |= CHR_TIAN_HU;
    }
    else
    {
      chr |= CHR_DI_HU;
    }
  }

  /*  �޸�Ȩλ  */


  /*  ����Ȩλ  */

  //Ӳ�˶Բ��ư˶�
  if(!(chr&CHR_BA_DUI_YING).IsEmpty())
  {
    chr &= ~CHR_BA_DUI;
  }

  //˫������Ӳ�� ���
  if(!(chr&CHR_BA_DUI_YING).IsEmpty() || !(chr&CHR_THREE_KING).IsEmpty() || !(chr&CHR_TIAN_HU).IsEmpty() ||
     !(chr&CHR_DI_HU).IsEmpty()       || !(chr&CHR_DAN_DIAO).IsEmpty())
  {
    chr |= CHR_SHUANG_FAN;
    chr &= ~CHR_YING_HU;
    chr &= ~CHR_RUAN_HU;
  }
  else if(!(chr&CHR_YING_HU).IsEmpty())    //Ӳ���������
  {
    chr &= ~CHR_RUAN_HU;
  }
  else
  {
    chr |= CHR_RUAN_HU;
  }
}

//״̬���
bool CTableFrameSink::IsAllState(bool bState[GAME_PLAYER], WORD wIgnorePos/* = INVALID_CHAIR*/, bool bCheckTrue/* = true*/)
{
  for(int i = 0; i < GAME_PLAYER; i++)
  {
    if(i == wIgnorePos)
    {
      continue;
    }

    if(bState[i] != bCheckTrue)
    {
      return false;
    }
  }
  return true;
}

//�Ƿ�˫��
bool CTableFrameSink::IsBaoUser(WORD wUserFir, WORD wUserSec)
{
  if(m_cbWeaveItemCount[wUserFir] >= 3)
  {
    BYTE cbProWeave = 0;
    for(int i = 0; i < m_cbWeaveItemCount[wUserFir]; i++)
    {
      if(m_WeaveItemArray[wUserFir][i].wProvideUser == wUserSec)
      {
        cbProWeave++;
      }
      if(cbProWeave >= 3)
      {
        return true;
      }
    }
  }

  if(m_cbWeaveItemCount[wUserSec] >= 3)
  {
    BYTE cbProWeave = 0;
    for(int i = 0; i < m_cbWeaveItemCount[wUserSec]; i++)
    {
      if(m_WeaveItemArray[wUserSec][i].wProvideUser == wUserFir)
      {
        cbProWeave++;
      }
      if(cbProWeave >= 3)
      {
        return true;
      }
    }
  }


  return false;
}

//////////////////////////////////////////////////////////////////////////////////
