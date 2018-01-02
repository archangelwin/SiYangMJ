#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME        200                 //��ʼ��ʱ��
#define IDI_CHIP          201                 //��׶�ʱ��
#define IDI_OPERATE_CARD      202                 //������ʱ��
#define IDI_REPLACE_CARD      301                 //���ƶ�ʱ��

//��Ϸ��ʱ��
#ifndef _DEBUG
#define TIME_START_GAME       30                  //��ʼ��ʱ��
#define TIME_CHIP         5                 //��׶�ʱ��
#define TIME_OPERATE_CARD     20                  //������ʱ��    
#else
#define TIME_START_GAME       10                  //��ʼ��ʱ��
#define TIME_CHIP         20                  //��׶�ʱ��
#define TIME_OPERATE_CARD     99                  //������ʱ��  
#endif
#define TIME_REPLACE_CARD     1000                //���ƶ�ʱ��

#define MAX_TIME_OUT        3                 //���ʱ����

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
  ON_WM_TIMER()
  ON_MESSAGE(IDM_START,OnStart)
  ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
  ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
  ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
  ON_MESSAGE(IDM_CHIP,OnChip)
  ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
  ON_MESSAGE(IDM_SICE_FINISH,OnSiceFinish)
  ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
  ON_MESSAGE(IDM_MOVE_CARD_FINISH,OnMoveCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
  //��Ϸ����
  m_wBankerUser=INVALID_CHAIR;
  m_cbLianZhuangCount = 0;
  m_wCurrentUser=INVALID_CHAIR;
  m_wReplaceUser = INVALID_CHAIR;
  m_cbActionMask = 0;
  m_cbActionCard = 0;
  m_cbListenStatus = 0;
  m_bWillHearStatus=false;
  m_lSiceCount = 0;
  m_bMagicIndex = MAX_INDEX;
  m_wMagicPos = INVALID_CHAIR;
  m_cbHeapMagic = 0;
  m_bTuoPai = false;

  //�йܱ���
  m_bStustee=false;
  m_wTimeOutCount =0;

  //����˿�
  ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
  ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

  //�˿˱���
  m_cbLeftCardCount=0;
  ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
  m_wHeapHead = INVALID_CHAIR;
  m_wHeapTail = INVALID_CHAIR;
  ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

  //�������
  m_cbZiCardCount=0;
  ZeroMemory(m_cbZiCardData,sizeof(m_cbZiCardData));

  //�����˿�
  m_cbCheatCardCount=0;
  ZeroMemory(m_cbCheatCardData,sizeof(m_cbCheatCardData));

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



  return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
  //��Ϸ����
  m_wBankerUser=INVALID_CHAIR;
  m_cbLianZhuangCount = 0;
  m_wCurrentUser=INVALID_CHAIR;
  m_wReplaceUser = INVALID_CHAIR;
  m_cbActionMask = 0;
  m_cbActionCard = 0;
  m_cbListenStatus = 0;
  m_bWillHearStatus=false;
  m_lSiceCount = 0;
  m_bMagicIndex = MAX_INDEX;
  m_wMagicPos = INVALID_CHAIR;
  m_cbHeapMagic = 0;
  m_bTuoPai = false;
  ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

  //�йܱ���
  m_bStustee=false;
  m_wTimeOutCount =0;

  //����˿�
  ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
  ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

  //�˿˱���
  m_cbLeftCardCount=0;
  ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
  m_wHeapHead = INVALID_CHAIR;
  m_wHeapTail = INVALID_CHAIR;

  //�������
  m_cbZiCardCount=0;
  ZeroMemory(m_cbZiCardData,sizeof(m_cbZiCardData));

  //�����˿�
  m_cbCheatCardCount=0;
  ZeroMemory(m_cbCheatCardData,sizeof(m_cbCheatCardData));

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
  switch(wClockID)
  {
    case IDI_START_GAME:    //��ʼ��Ϸ
    {
      //if( m_bStustee && nElapse < TIME_START_GAME )
      //{
      //  OnStart(0,0);
      //  return true;
      //}
      if(nElapse==0)
      {
        AfxGetMainWnd()->PostMessage(WM_CLOSE);
        return true;
      }
      if(nElapse<=5)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
      }

      return true;
    }
    case IDI_CHIP:        //��׶�ʱ��
    {
      if(!IsLookonMode() && m_bStustee && nElapse < TIME_CHIP)
      {
        OnChip(0,0);
        return true;
      }
      if(!IsLookonMode() && nElapse == 0)
      {
        OnChip(0,0);
        return true;
      }
      if(nElapse <= 5)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
      }

      return true;
    }
    case IDI_OPERATE_CARD:    //������ʱ��
    {
      //�Զ�����
      bool bAutoOutCard=false;
      if((bAutoOutCard==true)&&(m_GameClientView.m_ControlWnd.IsWindowVisible()))
      {
        bAutoOutCard=false;
      }
      if((bAutoOutCard==false)&&(m_bStustee==true))
      {
        bAutoOutCard=true;
      }

      //��ʱ�ж�
      if((IsLookonMode()==false)&&
         (nElapse==0 || bAutoOutCard || (m_cbListenStatus&&!m_bWillHearStatus&&nElapse<TIME_OPERATE_CARD)))
      {
        //��ȡλ��
        WORD wMeChairID=GetMeChairID();

        //��������
        if(wChairID==wMeChairID)
        {
          ////���δ����,��δ�й�,���ۼӳ�ʱ����
          //if( m_cbListenStatus == 0 && m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
          //{
          //  m_wTimeOutCount = 0;
          //  OnStusteeControl(TRUE,0);
          //  if(m_pIStringMessage!=NULL)
          //    m_pIStringMessage->InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
          //}

          if(m_wCurrentUser==wMeChairID)
          {
            //��ȡ�˿�
            BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();

            //����״̬
            if(m_cbListenStatus >= 2)
            {
              if(m_cbActionMask & WIK_CHI_HU)
              {
                OnUserAction(WIK_CHI_HU,0);
                return true;
              }
            }
            //����ڲ���ģʽ��
            if(m_GameClientView.m_HandCardControl.GetSelectMode())
            {
              OnCardOperate(0,0);
              return true;
            }

            //
            ZeroMemory(m_cbZiCardData, sizeof(m_cbZiCardData));
            m_cbZiCardCount = 0;
            m_GameLogic.AnalyseZiCard(m_cbCardIndex, m_cbZiCardData, m_cbZiCardCount);

            //����Ч��
            if(VerdictOutCard(cbCardData,m_cbZiCardData,m_cbZiCardCount)==false)
            {
              for(BYTE i=0; i<MAX_INDEX; i++)
              {
                //����Ч��
                if(m_cbCardIndex[i]==0)
                {
                  continue;
                }
                if(CheckOutCard(m_GameLogic.SwitchToCardData(i),m_cbZiCardData,m_cbZiCardCount) != 0)
                {
                  continue;
                }

                //���ñ���
                cbCardData=m_GameLogic.SwitchToCardData(i);
              }
            }

            //���ƶ���
            ASSERT(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)] > 0);
            LRESULT bOutResult = OnOutCard(cbCardData, cbCardData);
            if(bOutResult == 2)
            {
              BYTE bCard[13];
              ZeroMemory(bCard,sizeof(bCard));
              BYTE bCardCount = m_GameLogic.SwitchToCardData(m_cbCardIndex, bCard);
              for(int i = bCardCount-1; i >= 0; i--)
              {
                bOutResult = OnOutCard(bCard[i], bCard[i]);
                if(bOutResult != 2)
                {
                  break;
                }
                ASSERT(i!=0);
              }
            }
          }
          else
          {
            //����״̬,���ڻ������͵���
            if(m_cbListenStatus == 1 || m_cbListenStatus == 3)
            {
              if(m_cbActionMask & WIK_CHI_HU)
              {
                OnUserAction(WIK_CHI_HU,0);
                return true;
              }
            }
            //����ڲ���ģʽ��
            if(m_GameClientView.m_HandCardControl.GetSelectMode())
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
      if((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
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
  m_GameClientView.InvalidGameView(0,0,0,0);
  return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
  //ֹͣɫ�Ӷ���
  m_GameClientView.m_DrawSiceWnd.StopSicing();
  switch(wSubCmdID)
  {
    case SUB_S_CHIP:      //�û����
    {
      return OnSubChip(pData,wDataSize);
    }
    case SUB_S_GAME_START:    //��Ϸ��ʼ
    {
      return OnSubGameStart(pData,wDataSize);
    }
    case SUB_S_OUT_CARD:    //�û�����
    {
      return OnSubOutCard(pData,wDataSize);
    }
    case SUB_S_SEND_CARD:   //������Ϣ
    {
      return OnSubSendCard(pData,wDataSize);
    }
    case SUB_S_OPERATE_NOTIFY:  //������ʾ
    {
      StopMoveCard();
      return OnSubOperateNotify(pData,wDataSize);
    }
    case SUB_S_OPERATE_RESULT:  //�������
    {
      StopMoveCard();
      return OnSubOperateResult(pData,wDataSize);
    }
    case SUB_S_GAME_END:    //��Ϸ����
    {
      StopMoveCard();
      return OnSubGameEnd(pData,wDataSize);
    }
    case SUB_S_TRUSTEE:     //�û��й�
    {
      return OnSubTrustee(pData,wDataSize);
    }
    case SUB_S_LISTEN:      //�������
    {
      return OnSubListen(pData,wDataSize);
    }
    case SUB_S_REPLACE_CARD:  //����
    {
      return OnSubReplaceCard(pData,wDataSize);
    }
    case SUB_S_CHIP_RESULT:   //��׽��
    {
      return OnSubChipResult(pData,wDataSize);
    }
    case SUB_S_CHEAT_CARD:    //��������
    {
      return OnSubCheatCard(pData,wDataSize);
    }
  }

  return true;
}

//������Ϣ
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
  switch(cbGameStatus)
  {
    case GAME_SCENE_FREE: //����״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;


      //��������
      m_wBankerUser=pStatusFree->wBankerUser;
      m_GameClientView.SetCellScore(pStatusFree->lCellScore);
      m_GameClientView.m_HandCardControl.SetDisplayItem(true);

      //�й�����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
      }

      //���ÿؼ�
      if(IsLookonMode()==false && GetMeUserItem()->GetUserStatus() != US_READY)
      {
        m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
        m_GameClientView.m_btStart.SetFocus();
        SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
      }

      if(IsLookonMode() == false)
      {
//        m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
      }

      if(m_pIStringMessage!=NULL)
      {
        TCHAR szCellScore[32]=TEXT("");
        _sntprintf(szCellScore,sizeof(szCellScore),TEXT("��Ϸ�׷֣�%I64d"),pStatusFree->lCellScore);
        m_pIStringMessage->InsertSystemString(szCellScore);
      }

      //����Ч��
      m_GameClientView.SetDiscUser(INVALID_CHAIR);

      //���½���
      m_GameClientView.InvalidGameView(0,0,0,0);

      return true;
    }
    case GAME_SCENE_CHIP: //���״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusChip))
      {
        return false;
      }
      CMD_S_StatusChip * pStatusChip=(CMD_S_StatusChip *)pData;


      //��������
      m_wBankerUser=pStatusChip->wBankerUser;
      m_GameClientView.SetCellScore(pStatusChip->lCellScore);
      m_GameClientView.m_HandCardControl.SetDisplayItem(true);

      //�й�����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusChip->bTrustee[i]);
      }

      //ģ����Ϣ (�����Ϣ)
      if(IsLookonMode()==false && !pStatusChip->bHaveMaiDi[GetMeChairID()])
      {
        CMD_S_Chip chip;
        ZeroMemory(&chip,sizeof(chip));
        chip.cbLianZhuangCount = pStatusChip->cbLianZhuangCount;
        chip.wBankerUser = pStatusChip->wBankerUser;

        if(!OnSubChip((void*)&chip, sizeof(chip)))
        {
          return false;
        }
      }

      //ģ����Ϣ (��׽����Ϣ)
      m_GameClientView.SetMaiDiState(true);
      for(int i = 0; i < GAME_PLAYER; i++)
      {
        if(pStatusChip->bHaveMaiDi)
        {
          //ģ����Ϣ
          CMD_S_Chip_Result chipResult;
          ZeroMemory(&chipResult,sizeof(chipResult));
          chipResult.bTotal = false;
          chipResult.wMaiDiUser = i;
          chipResult.bMaidi[i] = pStatusChip->bMaiDi[i];

          if(!OnSubChipResult((void*)&chipResult, sizeof(chipResult)))
          {
            return false;
          }
        }
      }

      //����Ч��
      m_GameClientView.SetDiscUser(INVALID_CHAIR);

      if(m_pIStringMessage!=NULL)
      {
        TCHAR szCellScore[32]=TEXT("");
        _sntprintf(szCellScore,sizeof(szCellScore),TEXT("��Ϸ�׷֣�%I64d"),pStatusChip->lCellScore);
        m_pIStringMessage->InsertSystemString(szCellScore);

      }
      //���½���
      m_GameClientView.InvalidGameView(0,0,0,0);

      return true;
    }
    case GAME_SCENE_PLAY: //��Ϸ״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusPlay))
      {
        return false;
      }
      CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

      //��������
      WORD wMeChairID=GetMeChairID();
      WORD wViewChairID[GAME_PLAYER];
      ZeroMemory(wViewChairID,sizeof(wViewChairID));
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        wViewChairID[i]=SwitchViewChairID(i);
      }

      //���ñ���
      m_wBankerUser=pStatusPlay->wBankerUser;
      m_wCurrentUser=pStatusPlay->wCurrentUser;
      m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
      m_cbListenStatus = pStatusPlay->cbHearStatus[wMeChairID];
      m_wReplaceUser = pStatusPlay->wReplaceUser;
      m_wHeapHead = pStatusPlay->wHeapHead;
      m_wHeapTail = pStatusPlay->wHeapTail;
      m_bMagicIndex = pStatusPlay->bMagicIndex;
      m_wMagicPos = pStatusPlay->wMagicPos;
      m_cbHeapMagic = pStatusPlay->cbHeapMagic;
      m_bTuoPai = pStatusPlay->bTuoPai;

      m_GameClientView.SetShowHeapCard(true);

      //���ò���
      m_GameLogic.SetMagicIndex(pStatusPlay->bMagicIndex);
      m_GameClientView.m_HandCardControl.SetMagicCard(m_GameLogic.SwitchToCardData(pStatusPlay->bMagicIndex));

      //�û�����
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        IClientUserItem *pUserItem =GetTableUserItem(i);

        ASSERT(pUserItem != NULL);
        if(pUserItem == NULL)
        {
          m_szAccounts[i][0] = 0;
          continue;
        }
        lstrcpy(m_szAccounts[i],pUserItem->GetNickName());
      }

      //�Թ�
      if(IsLookonMode()==true)
      {
        m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
      }

      //�й�����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
      }

      if(pStatusPlay->bTrustee[wMeChairID])
      {
        OnStusteeControl(0,0);
      }

      m_wTimeOutCount=0;
      /*if(pStatusPlay->bTrustee[wMeChairID])
        m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
      else
        m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);*/

      //�˿˱���
      CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
      CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
      m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

      //��������
      m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
      m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser],pStatusPlay->cbLianZhuangCount);
      m_GameClientView.SetMagicCard(m_GameLogic.SwitchToCardData(m_bMagicIndex));

      /*if (!m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wMagicPos)].IsEmpty())
        m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wMagicPos)].SetCardData(m_cbHeapMagic, m_GameLogic.SwitchToCardData(m_bMagicIndex));
      else
      {
        for (int i = 1; i < GAME_PLAYER; i++)
        {
          WORD wPos = (m_wMagicPos+i)%GAME_PLAYER;
          if  (!m_GameClientView.m_HeapCard[SwitchToViewHeap(wPos)].IsEmpty())
          {
            if (m_GameClientView.m_HeapCard[SwitchToViewHeap(wPos)].IsNotEmptyPos(m_cbHeapMagic))
              m_GameClientView.m_HeapCard[SwitchToViewHeap(wPos)].SetCardData(m_cbHeapMagic, m_GameLogic.SwitchToCardData(m_bMagicIndex));
          }
        }
      }*/
      m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wMagicPos)].SetCardData(m_cbHeapMagic, m_GameLogic.SwitchToCardData(m_bMagicIndex));

      //��һ���
      for(int i = 0; i < GAME_PLAYER; i++)
      {
        WORD wViewChair = SwitchViewChairID(i);
        for(int j = 0; j < 8; j++)
        {
          if(pStatusPlay->cbHuaCardData[i][j] != 0)
          {
            m_GameClientView.SetHuaCard(wViewChair, pStatusPlay->cbHuaCardData[i][j], false);
          }
        }
      }

      //����˿�
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        WORD wOperateViewID = SwitchViewChairID(i);
        for(BYTE j=0; j<m_cbWeaveCount[i]; j++)
        {
          BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
          BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
          BYTE cbWeaveCardCount=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
          m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(m_WeaveItemArray[i][j].cbCardData,cbWeaveCardCount);
          if((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i))
          {
            m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
          }
          WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
          m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos((BYTE)wProviderViewID);
        }
      }

      //�û��˿�
      if(m_wCurrentUser==wMeChairID)
      {
        SetHandCardControl(m_cbCardIndex,pStatusPlay->cbSendCardData);
      }
      else
      {
        SetHandCardControl(m_cbCardIndex,0x00);
      }

      //�˿�����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //�û��˿�
        if(i!=wMeChairID)
        {
          BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[i]*3-1;
          WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;

          if(m_bUserCheat[GetMeChairID()])
          {
            m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(0,0);
          }
          else
          {
            m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
          }

        }

        //�����˿�
        m_GameClientView.m_DiscardCard[wViewChairID[i]].SetCardData(pStatusPlay->cbDiscardCard[i],pStatusPlay->cbDiscardCount[i]);
      }

      //����Ч��
      if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
      {
        m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));
        m_GameClientView.SetOutCardInfo(wViewChairID[pStatusPlay->wOutCardUser],pStatusPlay->cbOutCardData);
        m_GameClientView.m_DiscardCard[wViewChairID[pStatusPlay->wOutCardUser]].AddCardItem(pStatusPlay->cbOutCardData);
      }

      //��������
      if(IsLookonMode()==false)
      {
        m_GameClientView.m_HandCardControl.SetPositively(true);
        m_GameClientView.m_HandCardControl.SetDisplayItem(true);
//        m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
      }

      //��������
      for(WORD i=0; i<4; i++)
      {
        m_GameClientView.m_HeapCard[SwitchToViewHeap(i)].SetHeapCardInfo(pStatusPlay->cbHeapCardInfo[i][0],pStatusPlay->cbHeapCardInfo[i][1]);
      }

      //����״̬
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(pStatusPlay->cbHearStatus[i] > 0)
        {
          m_GameClientView.SetUserListenStatus(wViewChairID[i],true);
        }
      }

      //��������
      if((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
      {
        //��ȡ����
        m_cbActionMask=pStatusPlay->cbActionMask;
        m_cbActionCard=pStatusPlay->cbActionCard;

        //���ý���
        if(m_wCurrentUser==INVALID_CHAIR)
        {
          SetGameClock(wMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
        }
        if(IsLookonMode()==false)
        {
          m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
        }
      }

      //����ʱ��
      if(m_wCurrentUser!=INVALID_CHAIR)
      {
        //����ʱ��
        WORD wTimeCount=TIME_OPERATE_CARD;

        //����ʱ��
        SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
      }
      else if(m_wReplaceUser == wMeChairID && !IsLookonMode())
      {
        SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
      }

      //����Ȧ��
      m_GameClientView.SetFengQuan(pStatusPlay->cbQuanFeng);

      if(m_pIStringMessage!=NULL)
      {
        TCHAR szCellScore[32]=TEXT("");
        _sntprintf(szCellScore,sizeof(szCellScore),TEXT("��Ϸ�׷֣�%I64d"),pStatusPlay->lCellScore);
        m_pIStringMessage->InsertSystemString(szCellScore);

      }
      //���½���
      m_GameClientView.InvalidGameView(0,0,0,0);

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
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }

  //��������
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  //����״̬
  SetGameStatus(GAME_SCENE_PLAY);
  m_GameClientView.SetMaiDiState(false);

  //���ñ���
  m_wBankerUser = pGameStart->wBankerUser;
  m_cbLianZhuangCount = pGameStart->cbLianZhuangCount;
  m_wCurrentUser=pGameStart->wCurrentUser;
  m_wReplaceUser = pGameStart->wReplaceUser;
  m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
  m_cbActionMask = pGameStart->cbUserAction;
  m_lSiceCount = pGameStart->lSiceCount;
  m_bMagicIndex = pGameStart->bMagicIndex;
  m_wMagicPos = pGameStart->wMagicPos;
  m_cbHeapMagic = pGameStart->cbHeapMagic;
  ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

  //���ò���
  m_GameLogic.SetMagicIndex(pGameStart->bMagicIndex);
  m_GameClientView.m_HandCardControl.SetMagicCard(m_GameLogic.SwitchToCardData(pGameStart->bMagicIndex));
  m_GameClientView.SetMagicCard(0);

  //�����˿�
  BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
  m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

  //���ö���ͷβ
  m_wHeapHead = pGameStart->wHeapHead;
  m_wHeapTail = pGameStart->wHeapTail;
  CopyMemory(m_cbHeapCardInfo,pGameStart->cbHeapCardInfo,sizeof(m_cbHeapCardInfo));
  m_GameClientView.SetShowHeapCard(true);

  //�Թ۽���
  if(IsLookonMode()==true)
  {
    //���ñ���
    m_cbListenStatus = 0;
    m_bWillHearStatus = false;

    //����˿�
    ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
    ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

    m_GameClientView.m_GameScore.RestorationData();
    m_GameClientView.SetCurrentUser(INVALID_CHAIR);

    //���ý���
    m_GameClientView.SetDiscUser(INVALID_CHAIR);
    m_GameClientView.SetHuangZhuang(false);
    m_GameClientView.SetStatusFlag(false,false);
    m_GameClientView.SetBankerUser(INVALID_CHAIR,0);
    m_GameClientView.SetUserAction(INVALID_CHAIR,0);
    m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
    m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);

    //�˿�����
    for(int i = 0; i < GAME_PLAYER-1; i++)
    {
      m_GameClientView.m_UserCard[i].SetCardData(0,false);
    }
    m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

    //�˿�����
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
      m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
      for(BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++)
      {
        m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
      }
    }

    //�����˿�
    for(WORD i=0; i<4; i++)
    {
      m_GameClientView.m_HeapCard[i].ResetCardData();
      m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
    }
  }

  //���ý���
  m_GameClientView.SetFengQuan(pGameStart->cbQuanFeng);
  if(WORD(pGameStart->lSiceCount>>16) == 0)
  {
    m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser),m_cbLianZhuangCount);
  }

  //���ſ�ʼ����
  PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

  //ҡɫ�Ӷ���
  m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(pGameStart->lSiceCount>>16),(WORD)pGameStart->lSiceCount);
  m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);

  return true;
}

//�û����
bool CGameClientEngine::OnSubChip(const void * pBuffer, WORD wDataSize)
{
  //Ч����Ϣ
  ASSERT(wDataSize==sizeof(CMD_S_Chip));
  if(wDataSize!=sizeof(CMD_S_Chip))
  {
    return false;
  }

  //����״̬
  SetGameStatus(GAME_SCENE_CHIP);
  m_GameClientView.SetMaiDiState(true);

  CMD_S_Chip * pChip = (CMD_S_Chip*)pBuffer;

  //���ñ���
  m_wBankerUser = pChip->wBankerUser;
  m_cbLianZhuangCount = pChip->cbLianZhuangCount;
  m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser),pChip->cbLianZhuangCount);

  //�û�����
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    IClientUserItem *pUserItem =GetTableUserItem(i);
    ASSERT(pUserItem != NULL);
    if(pUserItem == NULL)
    {
      m_szAccounts[i][0] = 0;
      continue;
    }
    lstrcpy(m_szAccounts[i],pUserItem->GetNickName());
  }

  //�������
  if(IsLookonMode()==false)
  {
    //��ť���
    if(m_wBankerUser == GetMeChairID())
    {
      m_GameClientView.m_btMaiDi.ShowWindow(SW_SHOW);
      m_GameClientView.m_btBuMai.ShowWindow(SW_SHOW);
    }
    else
    {
      m_GameClientView.m_btDingDi.ShowWindow(SW_SHOW);
      m_GameClientView.m_btBuDing.ShowWindow(SW_SHOW);
    }


  }

  //���ö�ʱ��
  SetGameClock(GetMeChairID(),IDI_CHIP,TIME_CHIP);

  //��ׯ���
  CString strZhuang;
  if(pChip->cbLianZhuangCount == 0)
  {
    strZhuang.Format(TEXT("��� %s ��ׯ��"), m_szAccounts[m_wBankerUser]);
  }
  else
  {
    strZhuang.Format(TEXT("��� %s ��ׯ����ׯ%d�Ρ�"), m_szAccounts[m_wBankerUser], pChip->cbLianZhuangCount);
  }
  if(m_pIStringMessage!=NULL)
  {
    m_pIStringMessage->InsertSystemString(strZhuang);
  }

  return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
  //Ч����Ϣ
  ASSERT(wDataSize==sizeof(CMD_S_OutCard));
  if(wDataSize!=sizeof(CMD_S_OutCard))
  {
    return false;
  }

  //��������
  m_bTuoPai = false;

  //��Ϣ����
  CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

  //�����˿�
  m_cbCheatCardCount=pOutCard->cbCheatCardCount;
  CopyMemory(m_cbCheatCardData,pOutCard->cbCheatCardData,m_cbCheatCardCount);

  if(IsLookonMode() || pOutCard->wOutCardUser != GetMeChairID())
  {
    COutCardItem *pOutCardItem = NULL;
    try
    {
      //����������Ŀ
      pOutCardItem = new COutCardItem(pOutCard);
    }
    catch(...)
    {
      ASSERT(FALSE);
      return false;
    }

    //��Ӷ���
    m_MoveCardItemArray.Add(pOutCardItem);

    //���Ŷ���
    BeginMoveCard();
  }

  return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_SendCard));
  if(wDataSize!=sizeof(CMD_S_SendCard))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

  CSendCardItem *pSendCardItem = NULL;
  try
  {
    //����������Ŀ
    pSendCardItem = new CSendCardItem(pSendCard);
  }
  catch(...)
  {
    ASSERT(FALSE);
    return false;
  }

  //��Ӷ���
  m_MoveCardItemArray.Add(pSendCardItem);

  //���Ŷ���
  BeginMoveCard();

  return true;
}

//������Ϣ
bool CGameClientEngine::OnSubReplaceCard(const void * pBuffer, WORD wDataSize)
{
  //Ч�����
  ASSERT(wDataSize==sizeof(CMD_S_ReplaceCard));
  if(wDataSize!=sizeof(CMD_S_ReplaceCard))
  {
    return false;
  }

  //��������
  CMD_S_ReplaceCard * pReplaceCard=(CMD_S_ReplaceCard *)pBuffer;

  if(IsLookonMode() || pReplaceCard->wReplaceUser != GetMeChairID())
  {
    CReplaceCardItem *pReplaceCardItem = NULL;
    try
    {
      //����������Ŀ
      pReplaceCardItem = new CReplaceCardItem(pReplaceCard);
    }
    catch(...)
    {
      ASSERT(FALSE);
      return false;
    }

    //��Ӷ���
    m_MoveCardItemArray.Add(pReplaceCardItem);

    //���Ŷ���
    BeginMoveCard();
  }

  return true;
}

//��׽��
bool CGameClientEngine::OnSubChipResult(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_Chip_Result));
  if(wDataSize!=sizeof(CMD_S_Chip_Result))
  {
    return false;
  }

  CMD_S_Chip_Result *pChipResult = (CMD_S_Chip_Result*)pBuffer;

  //�������
  CString strInfo;
  BYTE bTrueMaidi = 0;

  if(pChipResult->bTotal)
  {
    for(int i = 0; i < GAME_PLAYER; i++)
    {
      bTrueMaidi = pChipResult->bMaidi[i];
      m_GameClientView.SetMaiDiScore(SwitchViewChairID(i), bTrueMaidi);
      strInfo.Format(TEXT("��� %s %s"), m_szAccounts[i], (i==m_wBankerUser?TEXT("���"):TEXT("����")));
      if(m_pIStringMessage!=NULL)
      {
        m_pIStringMessage->InsertSystemString(strInfo);
      }
    }
  }
  else
  {
    bTrueMaidi = pChipResult->bMaidi[pChipResult->wMaiDiUser];

    //���ý���
    m_GameClientView.SetMaiDiScore(SwitchViewChairID(pChipResult->wMaiDiUser), bTrueMaidi);

    if(bTrueMaidi == 2 || bTrueMaidi == 4)
    {
      strInfo.Format(TEXT("��� %s %s��"), m_szAccounts[pChipResult->wMaiDiUser], (pChipResult->wMaiDiUser==m_wBankerUser?TEXT("���"):TEXT("����")));
      if(m_pIStringMessage!=NULL)
      {
        m_pIStringMessage->InsertSystemString(strInfo);
      }
    }
  }

  return true;
}

//������ʾ
bool CGameClientEngine::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
  if(wDataSize!=sizeof(CMD_S_OperateNotify))
  {
    return false;
  }

  //��������
  CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

  //�û�����
  if((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
  {
    //��ȡ����
    WORD wMeChairID=GetMeChairID();
    m_cbActionMask = pOperateNotify->cbActionMask;
    m_cbActionCard = pOperateNotify->cbActionCard;

    //������������ڲ���ģʽ,��ȡ��
    if(m_GameClientView.m_HandCardControl.GetSelectMode())
    {
      m_GameClientView.m_HandCardControl.SetSelectMode(false);
    }

    //���ý���
    ActiveGameFrame();
    m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);

    //����ʱ��
    m_GameClientView.SetCurrentUser(INVALID_CHAIR);
    SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
  }

  return true;
}

//�������
bool CGameClientEngine::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
  //Ч����Ϣ
  ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
  if(wDataSize!=sizeof(CMD_S_OperateResult))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

  //��������
  BYTE cbPublicCard=TRUE;
  WORD wOperateUser=pOperateResult->wOperateUser;
  BYTE cbOperateCard[3];
  CopyMemory(cbOperateCard,pOperateResult->cbOperateCard,sizeof(cbOperateCard));
  WORD wOperateViewID=SwitchViewChairID(wOperateUser);
  WORD wProviderViewID = SwitchViewChairID(pOperateResult->wProvideUser);

  //���ñ���
  m_wCurrentUser = pOperateResult->wOperateUser;
  m_bTuoPai = (m_wCurrentUser==GetMeChairID() && pOperateResult->bTuoPai);

  //�������
  if((pOperateResult->cbOperateCode&WIK_GANG)!=0)
  {
    //���ñ���
    m_wCurrentUser=INVALID_CHAIR;

    //����˿�
    BYTE cbWeaveIndex=0xFF;
    for(BYTE i=0; i<m_cbWeaveCount[wOperateUser]; i++)
    {
      BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
      BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
      if((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
      {
        cbWeaveIndex=i;
        m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
        m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
        m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[3] = cbCenterCard;
        break;
      }
    }

    //����˿�
    if(cbWeaveIndex==0xFF)
    {
      //�����ж�
      cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

      //�����˿�
      cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
      m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
      m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
      m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
      m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
      for(BYTE i = 0; i < 4; i++)
      {
        m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[i] = cbOperateCard[0];
      }

      m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);
    }

    //��Ͻ���
    BYTE cbWeaveKind=pOperateResult->cbOperateCode;
    BYTE cbWeaveCardCount=4;
    //����ֻ��ʾ����һ��
    if(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard==false)
    {
      BYTE cbTempCard[4]= {0};
      cbTempCard[3]=cbOperateCard[0];
      m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbTempCard,
          cbWeaveCardCount);
    }
    else
    {
      m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,
          cbWeaveCardCount);
    }

    if(pOperateResult->wProvideUser==GetMeChairID())
    {

      m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem(true);
    }
    else
    {
      m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);
    }

    //�˿�����
    if(GetMeChairID()==wOperateUser)
    {
      m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbOperateCard[0])]=0;
    }

    //�����˿�
    if(GetMeChairID()==wOperateUser)
    {
      SetHandCardControl(m_cbCardIndex,0x00);
    }
    else
    {
      WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
      BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;

      if(m_bUserCheat[GetMeChairID()])
      {
        m_GameClientView.m_UserCard[wUserIndex].SetCardData(0,0);
      }
      else
      {
        m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
      }

    }
  }
  else if(pOperateResult->cbOperateCode!=WIK_NULL)
  {
    //�������
    BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
    m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
    m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
    m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
    m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
    CopyMemory(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,cbOperateCard,sizeof(cbOperateCard));

    //��Ͻ���
    BYTE cbWeaveCard[3],cbWeaveKind=pOperateResult->cbOperateCode;
    BYTE cbWeaveCardCount=3;
    CopyMemory(cbWeaveCard,cbOperateCard,sizeof(cbWeaveCard));
    m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
    m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);

    //ɾ���˿�
    if(GetMeChairID()==wOperateUser)
    {
      VERIFY(m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1));
      if(!m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1))
      {
        ASSERT(FALSE);
        return false;
      }
    }

    //�����˿�
    if(GetMeChairID()==wOperateUser)
    {
      SetHandCardControl(m_cbCardIndex,0x00);
    }
    else
    {
      WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
      BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;

      if(m_bUserCheat[GetMeChairID()])
      {
        m_GameClientView.m_UserCard[wUserIndex].SetCardData(0,0);
      }
      else
      {
        m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
      }
    }
  }

  //���ý���
  //����ṩ��Ҳ����Լ�,��ȥ���ṩ��ҵ��ѳ���
  if(pOperateResult->wProvideUser != wOperateUser)
  {
    if(m_GameClientView.m_DiscardCard[wProviderViewID].GetCardCount() > 0)
    {
      m_GameClientView.m_DiscardCard[wProviderViewID].RemoveCard(1);
    }
  }
  m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
  m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
  m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
  m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
  //������������ڲ���ģʽ,��ȡ��
  if(m_GameClientView.m_HandCardControl.GetSelectMode())
  {
    m_GameClientView.m_HandCardControl.SetSelectMode(false);
  }

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  //��������
  PlayActionSound(wOperateUser,pOperateResult->cbOperateCode);

  //����ʱ��
  if(m_wCurrentUser!=INVALID_CHAIR)
  {
    //�����ж�
    if(!IsLookonMode() && m_cbListenStatus == 0 && m_wCurrentUser==GetMeChairID())
    {
      WORD wMeChairId = GetMeChairID();

      //m_cbActionMask = m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId]); //ȡ������

      if(m_cbActionMask != WIK_NULL)
      {
        m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
      }
    }

    //����ʱ��
    WORD wTimeCount=TIME_OPERATE_CARD;

    //����ʱ��
    m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
    SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
  }

  return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //����״̬
  SetGameStatus(GAME_SCENE_FREE);
  m_GameClientView.SetStatusFlag(false,false);
  m_GameClientView.SetMaiDiState(false);

  //ɾ����ʱ��
  KillGameClock(IDI_OPERATE_CARD);
  KillGameClock(IDI_CHIP);

  //���ÿؼ�
  m_GameClientView.SetStatusFlag(false,false);
  m_GameClientView.SetMaiDiScore(INVALID_CHAIR, 0);
  m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
  m_GameClientView.m_HandCardControl.SetPositively(false);
  m_GameClientView.SetCurrentUser(INVALID_CHAIR);
  m_GameClientView.m_btMaiDi.ShowWindow(SW_HIDE);
  m_GameClientView.m_btDingDi.ShowWindow(SW_HIDE);
  m_GameClientView.m_btBuMai.ShowWindow(SW_HIDE);
  m_GameClientView.m_btBuDing.ShowWindow(SW_HIDE);

  //�����˿�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    for(BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++)
    {
      m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
    }
  }

  //��������
  tagScoreInfo ScoreInfo;
  tagWeaveInfo WeaveInfo;
  ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
  ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

  //�ɼ�����
  ScoreInfo.wProvideUser=pGameEnd->wProvideUser;
  ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;
  CopyMemory(ScoreInfo.bMaidi,pGameEnd->bMaidi,sizeof(ScoreInfo.bMaidi));
  ScoreInfo.wBankerUser = m_wBankerUser;

  //���û���
  CString strTemp ,strEnd = TEXT("���ֽ���,�ɼ�ͳ��:\r\n");
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    strTemp.Format(TEXT("%s: %I64d\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
    strEnd += strTemp;
  }

  //��Ϣ����
  if(m_pIStringMessage!=NULL)
  {
    m_pIStringMessage->InsertSystemString(strEnd);
  }

  WORD wFirstWinner = INVALID_CHAIR;
  CChiHuRight chr;
  if(pGameEnd->wProvideUser != INVALID_CHAIR)
  {
    for(int i = 0; i < GAME_PLAYER; i++)
    {
      if(pGameEnd->dwChiHuKind[i] == WIK_CHI_HU)
      {
        if(wFirstWinner == INVALID_CHAIR)
        {
          wFirstWinner = i;
        }
        else
        {
          ScoreInfo.bMulWinner = true;
        }
      }
    }
    ASSERT(wFirstWinner!=INVALID_CHAIR);
    if(wFirstWinner==INVALID_CHAIR)
    {
      wFirstWinner = 0;
    }
    wFirstWinner = (pGameEnd->dwChiHuKind[GetMeChairID()]==WIK_CHI_HU?GetMeChairID():wFirstWinner);

    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��������
      ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];

      //���óɼ�
      ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
      lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));

      //�����˿�
      if((ScoreInfo.cbCardCount==0)&&(pGameEnd->dwChiHuKind[i]!=WIK_NULL))
      {
        ScoreInfo.wChiHuUser = i;
        //����˿�
        WeaveInfo.cbWeaveCount=m_cbWeaveCount[i];
        for(BYTE j=0; j<WeaveInfo.cbWeaveCount; j++)
        {
          BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
          BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
          WeaveInfo.cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
          WeaveInfo.cbCardCount[j]=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
          CopyMemory(WeaveInfo.cbCardData[j],m_WeaveItemArray[i][j].cbCardData,sizeof(m_WeaveItemArray[i][j].cbCardData));
        }

        //�����˿�
        ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
        CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

        //��ȡ����
        for(BYTE j=0; j<ScoreInfo.cbCardCount; j++)
        {
          if(ScoreInfo.cbCardData[j]==pGameEnd->cbProvideCard)
          {
            MoveMemory(&ScoreInfo.cbCardData[j],&ScoreInfo.cbCardData[j+1],(ScoreInfo.cbCardCount-j-1)*sizeof(BYTE));
            ScoreInfo.cbCardData[ScoreInfo.cbCardCount-1]=pGameEnd->cbProvideCard;
            break;
          }
        }
      }
    }

    ScoreInfo.cbFanCount = pGameEnd->cbFanCount;
    chr.SetRightData(pGameEnd->dwChiHuRight,CountArray(pGameEnd->dwChiHuRight));

    //�ɼ�����
    m_GameClientView.m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
  }
  else
  {
    m_GameClientView.SetHuangZhuang(true);
  }

  //�û��˿�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    WORD wViewChairID=SwitchViewChairID(i);
    if(pGameEnd->dwChiHuKind[i]!=WIK_NULL)
    {
      m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
    }
    m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
  }

  //�����˿�
  for(int i = 0; i < GAME_PLAYER-1; i++)
  {
    m_GameClientView.m_UserCard[i].SetCardData(0,false);
  }
  m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

  //��������
  if(!chr.IsEmpty())
  {
    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      if(pGameEnd->dwChiHuKind[i] != WIK_NULL)
      {
        PlayActionSound(i,WIK_CHI_HU);
        break;
      }
    }
  }
  else
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
  }

  //���ý���
  if(IsLookonMode()==false)
  {
    m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
    m_GameClientView.m_btStart.SetFocus();
    SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
  }

  //ȡ���й�
  if(m_bStustee)
  {
    OnStusteeControl(0,0);
  }

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
}

//�û��й�
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_Trustee));
  if(wDataSize!=sizeof(CMD_S_Trustee))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
  m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
  IClientUserItem *pUserItem =GetTableUserItem(pTrustee->wChairID);
  if((IsLookonMode()||(pTrustee->wChairID!=GetMeChairID()))&&pUserItem!=NULL)
  {
    CString szBuffer;
    if(pTrustee->bTrustee==true)
    {
      szBuffer.Format(TEXT("���[%s]ѡ�����йܹ���."),pUserItem->GetNickName());
    }
    else
    {
      szBuffer.Format(TEXT("���[%s]ȡ�����йܹ���."),pUserItem->GetNickName());
    }
    if(m_pIStringMessage!=NULL)
    {
      m_pIStringMessage->InsertSystemString(szBuffer);
    }
  }

  return true;
}

//�û�����
bool CGameClientEngine::OnSubListen(const void * pBuffer,WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_Listen));
  if(wDataSize!=sizeof(CMD_S_Listen))
  {
    return false;
  }

  CMD_S_Listen *pListen = (CMD_S_Listen *)pBuffer;
  WORD wViewId = SwitchViewChairID(pListen->wChairId);

  //��������״̬
  if(IsLookonMode() || pListen->wChairId != GetMeChairID())
  {
    m_GameClientView.SetUserListenStatus(wViewId,true);
  }

  //��������
  PlayActionSound(pListen->wChairId,WIK_LISTEN);

  m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
  m_GameClientView.SetUserAction(wViewId,WIK_LISTEN);

  //����״̬
  m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),m_wCurrentUser!=GetMeChairID());

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
  IClientUserItem *pUserItem =GetTableUserItem(wChairID);
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
    case 0X30:  //��
    {
      switch(cbValue)
      {
        case 1:
        {
          strSoundName = TEXT("F_1");
          break;
        }
        case 2:
        {
          strSoundName = TEXT("F_2");
          break;
        }
        case 3:
        {
          strSoundName = TEXT("F_3");
          break;
        }
        case 4:
        {
          strSoundName = TEXT("F_4");
          break;
        }
        case 5:
        {
          strSoundName = TEXT("F_5");
          break;
        }
        case 6:
        {
          strSoundName = TEXT("F_6");
          break;
        }
        case 7:
        {
          strSoundName = TEXT("F_7");
          break;
        }
        default:
        {
          strSoundName=TEXT("BU_HUA");
        }

      }
      break;
    }
    case 0X20:  //Ͳ
    {
      strSoundName.Format(TEXT("T_%d"), cbValue);
      break;
    }

    case 0X10:  //��
    {
      strSoundName.Format(TEXT("S_%d"), cbValue);
      break;
    }
    case 0X00:  //��
    {
      strSoundName.Format(TEXT("W_%d"), cbValue);
      break;
    }
  }

  if(bBoy)
  {
    strSoundName = TEXT("BOY_") +strSoundName;
  }
  else
  {
    strSoundName = TEXT("GIRL_") + strSoundName;
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
  IClientUserItem *pUserItem =GetTableUserItem(wChairID);
  if(pUserItem == NULL)
  {
    assert(0 && "�ò��������Ϣ");
    return;
  }

  bool bBoy = (pUserItem->GetGender() == GENDER_MANKIND);

  switch(cbAction)
  {
    case WIK_LEFT:
    case WIK_CENTER:
    case WIK_RIGHT:   //����
    {
      if(bBoy)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI"));
      }
      else
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI"));
      }
      break;
    }
    case WIK_PENG:    //����
    {
      if(bBoy)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_PENG"));
      }
      else
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_PENG"));
      }
      break;
    }
    case WIK_GANG:    //����
    {
      if(bBoy)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_GANG"));
      }
      else
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_GANG"));
      }
      break;
    }
    case WIK_CHI_HU:  //�Ժ�
    {

      if(bBoy)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI_HU"));
      }
      else
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI_HU"));
      }
      break;
    }
    case WIK_REPLACE: //�滻����
    {
      if(bBoy)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_REPLACE"));
      }
      else
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_REPLACE"));
      }
      break;
    }
    case WIK_LISTEN:  //����
    {
      if(bBoy)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_TING"));
      }
      else
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_TING"));
      }
      break;
    }
  }

  return;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard(BYTE cbPreCardData,BYTE cbAlreadyCardData[7],BYTE cbZiCardCount)
{
  BOOL bRet = CheckOutCard(cbPreCardData, cbAlreadyCardData, cbZiCardCount);

  ASSERT(bRet >= 0 && bRet <= 4);

  if(m_pIStringMessage!=NULL)
  {
    if(bRet == 1)
    {
      m_pIStringMessage->InsertSystemString(TEXT("���ɳ�����"));
    }
    else if(bRet == 2)
    {
      m_pIStringMessage->InsertSystemString(TEXT("�����ȳ�����ƣ�"));
    }
    else if(bRet == 3)
    {
      m_pIStringMessage->InsertSystemString(TEXT("�����ȳ�����ƣ�"));
    }
    else if(bRet == 4)
    {
      m_pIStringMessage->InsertSystemString(TEXT("�����ȳ��������еķ��ƺͼ��ƣ�"));
    }
  }


  return (bRet==0);
}

//�Ƿ�ɳ�  (0 �ɳ� 1 ���ɳ����� 2 �����ȳ������ 3 �����������)
BOOL CGameClientEngine::CheckOutCard(BYTE cbPreCardData,BYTE cbAlreadyCardData[7],BYTE cbZiCardCount)
{
  //�����ж�
  if(m_GameLogic.IsMagicCard(cbPreCardData, false))
  {
    return 1;
  }

  BYTE cbFengCardCount = 0;
  BYTE cbJianCardCount = 0;
  m_GameLogic.AnalyseFengAndJianCardCount(cbAlreadyCardData, cbFengCardCount, cbJianCardCount);

  //û�е�������
  if(cbZiCardCount == 0)
  {
    return 0;
  }
  else
  {
    //���ڷ���
    if(cbFengCardCount != 0 && cbJianCardCount == 0 && !m_GameLogic.IsFengCard(cbPreCardData))
    {
      return 2;
    }

    //���ڼ���
    if(cbJianCardCount != 0 && cbFengCardCount == 0 && !m_GameLogic.IsJianCard(cbPreCardData))
    {
      return 3;
    }

    if(cbJianCardCount != 0 && cbFengCardCount != 0)
    {
      if(!(m_GameLogic.IsFengCard(cbPreCardData) || m_GameLogic.IsJianCard(cbPreCardData)))
      {
        return 4;
      }
    }

  }

  return 0;
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
  //��������
  KillGameClock(IDI_START_GAME);
  m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
  m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
  m_GameClientView.m_GameScore.RestorationData();
  m_GameClientView.SetCurrentUser(INVALID_CHAIR);

  //���ý���
  m_GameClientView.SetDiscUser(INVALID_CHAIR);
  m_GameClientView.SetHuangZhuang(false);
  m_GameClientView.SetStatusFlag(false,false);
  m_GameClientView.SetBankerUser(INVALID_CHAIR,0);
  m_GameClientView.SetUserAction(INVALID_CHAIR,0);
  m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
  m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);
  m_GameClientView.SetHuaCard(0, 0, true);
  m_GameClientView.SetMagicCard(0);

  //�˿�����
  for(int i = 0; i < GAME_PLAYER-1; i++)
  {
    m_GameClientView.m_UserCard[i].SetCardData(0,false);
  }
  m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

  //�˿�����
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
    m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
    for(BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++)
    {
      m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
    }
  }

  //�����˿�
  for(WORD i=0; i<4; i++)
  {
    m_GameClientView.m_HeapCard[i].ResetCardData();
    m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
  }

  //��Ϸ����
  m_wCurrentUser=INVALID_CHAIR;
  m_wReplaceUser = INVALID_CHAIR;
  m_cbListenStatus = 0;
  m_bWillHearStatus = false;
  m_cbActionMask = 0;
  m_cbActionCard = 0;

  //����˿�
  ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
  ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

  //�˿˱���
  m_cbLeftCardCount=0;
  ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
  m_wHeapHead = INVALID_CHAIR;
  m_wHeapTail = INVALID_CHAIR;
  m_cbZiCardCount = 0;
  ZeroMemory(m_cbZiCardData, sizeof(m_cbZiCardData));

  //�����˿�
  m_cbCheatCardCount=0;
  ZeroMemory(m_cbCheatCardData,sizeof(m_cbCheatCardData));

  m_GameClientView.SetShowHeapCard(false);

  //������Ϣ
  SendUserReady(NULL,0);

  //OnSiceFinish(0, 0);
  /*m_GameLogic.SetMagicIndex(1);
  m_GameClientView.m_HandCardControl.SetMagicCard(0x02);
  m_cbCardIndex[1] = 3;
  m_cbCardIndex[2] = 3;
  m_cbCardIndex[3] = 3;
  m_cbCardIndex[4] = 3;
  m_cbCardIndex[5] = 3;
  m_cbCardIndex[33] = 2;
  SetHandCardControl(m_cbCardIndex, 2);*/

  return 0;
}

//���Ʋ���
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
  //�����ж�
  if((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
  {
    return 0;
  }

  //�����ж�
  BYTE cbOutCardData=(BYTE)wParam;

  ZeroMemory(m_cbZiCardData, sizeof(m_cbZiCardData));
  m_cbZiCardCount = 0;
  m_GameLogic.AnalyseZiCard(m_cbCardIndex, m_cbZiCardData, m_cbZiCardCount);

  if(VerdictOutCard(cbOutCardData,m_cbZiCardData,m_cbZiCardCount)==false)
  {
    if(m_pIStringMessage != NULL)
    {
      m_pIStringMessage->InsertSystemString(TEXT("�����Ʋ�������Ϸ����!"));
    }
    return 1;
  }

  KillGameClock(IDI_OPERATE_CARD);

  //���ñ���
  m_wCurrentUser=INVALID_CHAIR;
  m_cbActionMask = 0;
  m_cbActionCard = 0;
  if(!m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData))
  {
    ASSERT(FALSE);
    return 0;
  }

  //�����˿�
  BYTE cbCardData[MAX_COUNT];
  BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
  ASSERT((cbCardCount-1)%3==0);
  m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

  //���ý���
  m_GameClientView.SetCurrentUser(INVALID_CHAIR);
  m_GameClientView.SetStatusFlag(false,false);
  m_GameClientView.SetUserAction(INVALID_CHAIR,0);
  m_GameClientView.SetOutCardInfo(2,cbOutCardData);
  m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
  if(m_GameClientView.m_DlgListen.m_hWnd)
  {
    m_GameClientView.m_DlgListen.EndDialog(IDCANCEL);
  }

  //������ƶ���
  COutCardItem *pOutCardItem = NULL;
  try
  {
    pOutCardItem = new COutCardItem();
  }
  catch(...)
  {
    ASSERT(FALSE);
    return 0;
  }

  pOutCardItem->m_cbOutCardData = cbOutCardData;
  pOutCardItem->m_wOutCardUser = GetMeChairID();
  m_MoveCardItemArray.Add(pOutCardItem);

  tagMoveCardItem mci;
  mci.cbCardCount = 1;
  mci.cbCardData = cbOutCardData;
  mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
  mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
  mci.wViewChairId = MYSELF_VIEW_ID;
  m_GameClientView.OnMoveCardItem(mci);

  //��������
  CMD_C_OutCard OutCard;
  OutCard.cbCardData=cbOutCardData;
  SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

  //��������
  PlayCardSound(GetMeChairID(),cbOutCardData);

  m_GameClientView.InvalidGameView(0,0,0,0);

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
  m_GameClientView.m_HandCardControl.GetUserSelectResult(si);

  //���������Ϣ
  cbOperateCode = (BYTE)si.wActionMask;
  cbOperateCard[0] = si.cbActionCard;
  CopyMemory(&cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2);

  m_cbActionMask = 0;
  m_cbActionCard = 0;

  //ȡ������ģʽ
  m_GameClientView.m_HandCardControl.SetSelectMode(false);

  //���������
  if(m_bWillHearStatus && (cbOperateCode&WIK_LISTEN))
  {
    m_bWillHearStatus = false;
    return OnOutCard(cbOperateCard[0],0);
  }

  m_wCurrentUser = INVALID_CHAIR;

  //��������
  m_GameClientView.SetStatusFlag(false,true);

  //��������
  CMD_C_OperateCard OperateCard;
  OperateCard.cbOperateCode=cbOperateCode;
  CopyMemory(OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard));
  SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

  return 0;
}

//�Ϲܿ���
LRESULT CGameClientEngine::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
  ////���ñ���
  //m_wTimeOutCount=0;

  ////����״̬
  //if (m_bStustee==true)
  //{
  //  m_bStustee=false;
  //  m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
  //  if( FALSE == (BOOL)wParam && m_pIStringMessage!=NULL)
  //    m_pIStringMessage->InsertSystemString(TEXT("��ȡ�����йܹ���."));
  //  CMD_C_Trustee Trustee;
  //  Trustee.bTrustee = false;
  //  SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
  //}
  //else
  //{
  //  m_bStustee=true;
  //  m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
  //  if( FALSE == (BOOL)wParam && m_pIStringMessage!=NULL)
  //    m_pIStringMessage->InsertSystemString(TEXT("��ѡ�����йܹ���."));
  //  CMD_C_Trustee Trustee;
  //  Trustee.bTrustee = true;
  //  SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
  //}

  return 0;
}

//�����Ϣ
LRESULT CGameClientEngine::OnChip(WPARAM wParam, LPARAM lParam)
{
  if(IsLookonMode())
  {
    return 0;
  }

  KillGameClock(IDI_CHIP);

  m_GameClientView.m_btMaiDi.ShowWindow(SW_HIDE);
  m_GameClientView.m_btDingDi.ShowWindow(SW_HIDE);
  m_GameClientView.m_btBuMai.ShowWindow(SW_HIDE);
  m_GameClientView.m_btBuDing.ShowWindow(SW_HIDE);

  //������Ϣ
  CMD_C_Chip  chip ;
  ZeroMemory(&chip,sizeof(chip));
  chip.bMaiDi = (BYTE)wParam;

  SendSocketData(SUB_C_CHIP,&chip,sizeof(chip));

  return 0;
}

//�ڶ���ҡɫ����Ϣ
LRESULT CGameClientEngine::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

  //���ý���
  bool bPlayerMode=(IsLookonMode()==false);
  m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser),m_cbLianZhuangCount);

  return 0;
}

//ҡɫ�ӽ�����Ϣ
LRESULT CGameClientEngine::OnSiceFinish(WPARAM wParam, LPARAM lParam)
{
  //���ý���
  m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);
  m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wMagicPos)].SetCardData(m_cbHeapMagic, m_GameLogic.SwitchToCardData(m_bMagicIndex));
  m_GameClientView.SetMagicCard(m_GameLogic.SwitchToCardData(m_bMagicIndex));

  WORD wMeChairID = GetMeChairID();
  bool bPlayerMode = (IsLookonMode()==false);

  //���췢�ƶ���
  BYTE cbHandCardData[MAX_COUNT];
  m_GameLogic.SwitchToCardData(m_cbCardIndex,cbHandCardData);
  //�����˿�
  m_GameLogic.RandCardData(cbHandCardData,(m_wBankerUser==wMeChairID)?MAX_COUNT:MAX_COUNT-1,cbHandCardData);
  //ȷ������ɾ��λ��
  WORD wHeapId = m_wHeapTail;
  WORD wHeapCardIndex = 0;
  if(m_cbHeapCardInfo[m_wHeapTail][1] == 0)
  {
    wHeapId = (wHeapId+1)%4;
    wHeapCardIndex = HEAP_FULL_COUNT-1;
  }
  else
  {
    wHeapCardIndex = m_cbHeapCardInfo[m_wHeapTail][1]-1;
  }
  //��4Ȧ�����1Ȧ��һ��
  for(BYTE i = 0; i < 4; i++)
  {
    //��ׯ�ҿ�ʼ
    WORD wId = m_wBankerUser;
    for(WORD j = 0; j < GAME_PLAYER; j++)
    {
      WORD wChairId = (wId+GAME_PLAYER-j)%GAME_PLAYER;
      CStartCardItem *pStartCardItem = NULL;
      //����������Ŀ
      try
      {
        pStartCardItem = new CStartCardItem();
      }
      catch(...)
      {
        ASSERT(FALSE);
        return 0;
      }
      //������
      pStartCardItem->m_cbCardCount = i<3?4:1;
      //����ֵ
      if(wChairId == wMeChairID)
      {
        CopyMemory(pStartCardItem->m_cbCardData,&cbHandCardData[i*4],sizeof(BYTE)*(i<3?4:1));
      }
      //�������
      pStartCardItem->m_wChairId = wChairId;
      //����λ��
      pStartCardItem->m_wHeapId = wHeapId;
      //����λ��
      pStartCardItem->m_wHeapCardIndex = wHeapCardIndex;
      //���붯������
      m_MoveCardItemArray.Add(pStartCardItem);

      //�ƽ�
      if(i < 3)
      {
        if(wHeapCardIndex <= 3)
        {
          wHeapId = (wHeapId+1)%4;
          wHeapCardIndex = HEAP_FULL_COUNT-(3-wHeapCardIndex)-1;
        }
        else
        {
          wHeapCardIndex -= 4;
        }
      }
      else
      {
        if(wHeapCardIndex == 0)
        {
          wHeapId = (wHeapId+1)%4;
          wHeapCardIndex = HEAP_FULL_COUNT-1;
        }
        else
        {
          wHeapCardIndex--;
        }
      }
    }
  }
  //���һ�ţ�����ׯ��
  CStartCardItem *pStartCardItem = NULL;
  //����������Ŀ
  try
  {
    pStartCardItem = new CStartCardItem();
  }
  catch(...)
  {
    ASSERT(FALSE);
    return 0;
  }
  //������
  pStartCardItem->m_cbCardCount = 1;
  //����ֵ
  if(m_wBankerUser == wMeChairID)
  {
    pStartCardItem->m_cbCardData[0] = cbHandCardData[MAX_COUNT-1];
  }
  //�������
  pStartCardItem->m_wChairId = m_wBankerUser;
  //����λ��
  pStartCardItem->m_wHeapId = wHeapId;
  //����λ��
  pStartCardItem->m_wHeapCardIndex = wHeapCardIndex;
  //���һ��
  pStartCardItem->m_bLastItem = true;
  //���붯������
  m_MoveCardItemArray.Add(pStartCardItem);

  //��ʼ����
  BeginMoveCard();

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
  if((cbWeaveCardCount+cbCardCount)==MAX_COUNT)
  {
    if(cbAdvanceCard != 0x00)
    {
      //ɾ���˿�
      BYTE cbRemoveCard[]= {cbAdvanceCard};
      VERIFY(m_GameLogic.RemoveCard(cbHandCardData,cbCardCount,cbRemoveCard,1));

      //�����˿�
      cbHandCardData[cbCardCount-1]=cbAdvanceCard;
    }

    m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount-1,cbHandCardData[cbCardCount-1]);
  }
  else
  {
    m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount,0x00);
  }

  return;
}

//��ʱ����Ϣ
VOID CGameClientEngine::OnTimer(UINT nIDEvent)
{
  switch(nIDEvent)
  {
    case IDI_REPLACE_CARD:  //�û�����
    {
      //ɾ��ʱ��
      KillTimer(IDI_REPLACE_CARD);

      //���ý���
      m_GameClientView.SetUserAction(INVALID_CHAIR,0);

      //Ѱ���˿�
      for(BYTE i=MAX_INDEX-MAX_HUA_CARD; i<MAX_INDEX; i++)
      {
        if(m_cbCardIndex[i] > 0 && !m_GameLogic.IsMagicCard(i,true))
        {
          //��������
          BYTE cbReplaceCard=m_GameLogic.SwitchToCardData(i);
          if(!m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard))
          {
            ASSERT(FALSE);
            return;
          }

          //�����˿�
          SetHandCardControl(m_cbCardIndex,0x00);

          //��������
          PlayActionSound(GetMeChairID(),WIK_REPLACE);

          //���ý���
          m_GameClientView.SetUserAction(MYSELF_VIEW_ID,WIK_REPLACE);

          CReplaceCardItem *pReplaceCardItem = NULL;
          try
          {
            pReplaceCardItem = new CReplaceCardItem();
          }
          catch(...)
          {
            ASSERT(FALSE);
            return;
          }
          pReplaceCardItem->m_cbReplaceCard = cbReplaceCard;
          pReplaceCardItem->m_wReplaceUser = GetMeChairID();
          m_MoveCardItemArray.Add(pReplaceCardItem);

          //������ƶ���
          tagMoveCardItem mci;
          mci.cbCardCount = 1;
          mci.cbCardData = cbReplaceCard;
          mci.wViewChairId = MYSELF_VIEW_ID;
          mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
          //mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
          mci.ptTo = m_GameClientView.GetHuaCardTailPos(MYSELF_VIEW_ID);
          m_GameClientView.OnMoveCardItem(mci);

          //��������
          CMD_C_ReplaceCard ReplaceCard;
          ReplaceCard.cbCardData=cbReplaceCard;
          SendSocketData(SUB_C_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

          break;
        }
      }

      return;
    }
  }

  __super::OnTimer(nIDEvent);
}

//��Ҳ����ؼ�
LRESULT CGameClientEngine::OnUserAction(WPARAM wParam, LPARAM lParam)
{
  //��������
  BYTE cbOperateCode=(BYTE)(wParam);
  BYTE cbOperateCard[3] = {0,0,0};

  //���ز����ؼ�
  m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

  //״̬�ж�
  if(cbOperateCode==WIK_NULL)
  {
    //���ñ���
    m_cbActionMask = WIK_NULL;
    m_cbActionCard = 0;
    if(m_wCurrentUser == INVALID_CHAIR)
    {
      //ɾ����ʱ��
      KillGameClock(IDI_OPERATE_CARD);
      //������Ϣ
      CMD_C_OperateCard OperateCard;
      OperateCard.cbOperateCode = WIK_NULL;
      ZeroMemory(OperateCard.cbOperateCard,sizeof(OperateCard.cbOperateCard));
      SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
    }
    return 0;
  }

  if(cbOperateCode & WIK_LISTEN)
  {
    ASSERT(m_cbListenStatus == 0);
    if(m_cbListenStatus != 0)
    {
      return 0;
    }

    //����ѡ��
    if(m_GameClientView.m_DlgListen.DoModal() == IDOK)
    {
      //ѡ��������ץ��
      if(m_GameClientView.m_DlgListen.m_bZhuaPao && m_GameClientView.m_DlgListen.m_bZiMo)
      {
        m_cbListenStatus = 3;
      }
      //ֻ����
      else if(m_GameClientView.m_DlgListen.m_bZiMo)
      {
        m_cbListenStatus = 2;
      }
      //ֻץ��
      else if(m_GameClientView.m_DlgListen.m_bZhuaPao)
      {
        m_cbListenStatus = 1;
      }
      //���ѡ����,����������Ϣ
      if(m_cbListenStatus > 0)
      {
        //���ü������Ʊ�־
        m_bWillHearStatus = true;

        //���ñ���
        m_cbActionMask = WIK_NULL;
        m_cbActionCard = 0;

        //��������״̬
        m_GameClientView.SetUserListenStatus(2,true);

        //������������
        CMD_C_Listen Listen;
        Listen.cbListen = m_cbListenStatus;
        SendSocketData(SUB_C_LISTEN,&Listen,sizeof(Listen));

        //��ȡѡ�����
        tagSelectCardInfo sci[MAX_COUNT];
        BYTE cbInfoCount = GetSelectCardInfo(cbOperateCode,sci);

        //���ֻ��һ����
        if(cbInfoCount == 1)
        {
          m_bWillHearStatus = false;
          OnOutCard(sci[0].cbActionCard,0);
        }
        //���������ѡ��
        else
        {
          //���ò����¼�
          m_GameClientView.m_HandCardControl.OnEventUserAction(sci,cbInfoCount);
          //��ʾ��Ϣ
          if(m_pIStringMessage!=NULL)
          {
            m_pIStringMessage->InsertSystemString(TEXT("��ѡ�������"));
          }
        }
      }
    }

    return 0;
  }

  bool bDone = false;
  //����
  if(cbOperateCode & WIK_CHI_HU)
  {
    bDone = true;
  }
  else
  {
    //��ȡѡ�����
    tagSelectCardInfo sci[MAX_COUNT];
    BYTE cbInfoCount = GetSelectCardInfo(cbOperateCode,sci);

    //���ò����¼�
    bDone = m_GameClientView.m_HandCardControl.OnEventUserAction(sci,cbInfoCount);

    //�����ɲ���
    if(bDone)
    {
      //���ò������
      tagSelectCardInfo si;
      m_GameClientView.m_HandCardControl.GetUserSelectResult(si);
      cbOperateCode = (BYTE)si.wActionMask;
      cbOperateCard[0] = si.cbActionCard;
      CopyMemory(&cbOperateCard[1],si.cbCardData,2*sizeof(BYTE));

      m_GameClientView.m_HandCardControl.SetSelectMode(false);
    }
    //�������õȴ�ѡ��
    else
    {
      m_GameClientView.SetStatusFlag(true,false);
    }
  }

  //���������ɣ�ֱ�ӷ��Ͳ�������
  if(bDone)
  {
    //ɾ����ʱ��
    KillGameClock(IDI_OPERATE_CARD);

    //���ñ���
    m_wCurrentUser = INVALID_CHAIR;
    m_cbActionMask = WIK_NULL;
    m_cbActionCard = 0;

    CMD_C_OperateCard OperateCard;
    OperateCard.cbOperateCode = cbOperateCode;
    CopyMemory(OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard));
    SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
  }

  return 0;
}

//��ȡ������Ϣ
BYTE CGameClientEngine::GetSelectCardInfo(WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_COUNT])
{
  //��ʼ��
  BYTE cbSelectCount = 0;

  if(wOperateCode == WIK_NULL)
  {
    return 0;
  }

  //����
  if(wOperateCode & WIK_LISTEN)
  {
    //��ȡ�ɶ�����
    BYTE cbCardData[MAX_COUNT],cbCardCount=0;
    BYTE cbCardIndex[MAX_INDEX];
    CopyMemory(cbCardIndex,m_cbCardIndex,sizeof(cbCardIndex));
    WORD wMeChairId = GetMeChairID();
    CChiHuRight chr;
    for(BYTE i = 0; i < MAX_INDEX/*-MAX_HUA_CARD*/; i++)
    {
      //091231JJ
      if(cbCardIndex[i] == 0)
      {
        continue;
      }
      if(i >= MAX_INDEX-MAX_HUA_CARD && i < MAX_INDEX && !m_GameLogic.IsMagicCard(i,true))
      {
        continue;
      }

      cbCardIndex[i]--;
      for(BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++)
      {
        BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(j);
        if(WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard(cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],
            cbCurrentCard,chr))
        {
          cbCardData[cbCardCount++] = m_GameLogic.SwitchToCardData(i);
          break;
        }
      }
      cbCardIndex[i]++;
    }
    for(BYTE i = 0; i < cbCardCount; i++)
    {
      SelectInfo[cbSelectCount].cbActionCard = cbCardData[i];
      SelectInfo[cbSelectCount].wActionMask = WIK_LISTEN;
      SelectInfo[cbSelectCount].cbCardCount = 1;
      SelectInfo[cbSelectCount++].cbCardData[0] = cbCardData[i];
    }
  }
  //����
  else if(wOperateCode&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
  {
    //Ч��
    ASSERT(m_cbActionCard != 0);
    if(m_cbActionCard == 0)
    {
      return 0;
    }

    //�滻�װ� (���ɲ��������ֵ)
    if(m_cbActionCard == 0x37 && m_bMagicIndex != 33)
    {
      m_cbActionCard = m_GameLogic.SwitchToCardData(m_bMagicIndex);
    }

    if(wOperateCode & WIK_LEFT)
    {
      SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
      SelectInfo[cbSelectCount].wActionMask = WIK_LEFT;
      SelectInfo[cbSelectCount].cbCardCount = 2;
      SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard+1;
      SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+2;
    }
    else if(wOperateCode & WIK_CENTER)
    {
      SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
      SelectInfo[cbSelectCount].wActionMask = WIK_CENTER;
      SelectInfo[cbSelectCount].cbCardCount = 2;
      SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-1;
      SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+1;
    }
    else if(wOperateCode & WIK_RIGHT)
    {
      SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
      SelectInfo[cbSelectCount].wActionMask = WIK_RIGHT;
      SelectInfo[cbSelectCount].cbCardCount = 2;
      SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-2;
      SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard-1;
    }

    //�滻�װ�  (������)
    if(m_cbActionMask & (WIK_RIGHT|WIK_LEFT|WIK_CENTER))
    {
      for(int i = 0; i < cbSelectCount; i++)
      {
        if(SelectInfo[i].cbActionCard == m_GameLogic.SwitchToCardData(m_bMagicIndex))
        {
          SelectInfo[i].cbActionCard = 0x37;
        }

        for(int j = 0; j < 2; j++)
        {
          if(SelectInfo[i].cbCardData[j] == m_GameLogic.SwitchToCardData(m_bMagicIndex))
          {
            SelectInfo[i].cbCardData[j] = 0x37;
          }
        }
      }
    }
  }
  //����
  else if(wOperateCode & WIK_PENG)
  {
    //Ч��
    ASSERT(m_cbActionCard != 0);
    if(m_cbActionCard == 0)
    {
      return 0;
    }
    SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
    SelectInfo[cbSelectCount].wActionMask = WIK_PENG;
    SelectInfo[cbSelectCount].cbCardCount = 2;
    SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
    SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard;
  }
  //����
  else if(wOperateCode & WIK_GANG)
  {
    //������Լ�����
    if(m_wCurrentUser == GetMeChairID())
    {
      //Ѱ���Ƿ��ж������
      WORD wMeChairId = GetMeChairID();
      tagGangCardResult gcr;
      ZeroMemory(&gcr,sizeof(gcr));
      m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],gcr);
      ASSERT(gcr.cbCardCount > 0);
      for(BYTE i = 0; i < gcr.cbCardCount; i++)
      {
        SelectInfo[cbSelectCount].cbActionCard = gcr.cbCardData[i];
        SelectInfo[cbSelectCount].wActionMask = WIK_GANG;
        if(m_cbCardIndex[m_GameLogic.SwitchToCardIndex(gcr.cbCardData[i])] == 1)
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
      ASSERT(m_cbActionCard != 0);
      if(m_cbActionCard == 0)
      {
        return 0;
      }
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


//��������
bool CGameClientEngine::OnSubCheatCard(VOID * pData, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_CheatCard));
  if(wDataSize!=sizeof(CMD_S_CheatCard))
  {
    return false;
  }

  //��������
  CMD_S_CheatCard * pCheatCard=(CMD_S_CheatCard *)pData;

  ASSERT(pCheatCard->cbUserCount <= GAME_PLAYER);
  if(pCheatCard->cbUserCount > GAME_PLAYER)
  {
    return false;
  }

  //�û��˿�
  for(WORD i = 0; i < pCheatCard->cbUserCount ; i++)
  {
    if(GetMeChairID()==pCheatCard->wCardUser[i])
    {
      m_bUserCheat[pCheatCard->wCardUser[i]] = true;

      //for (WORD j=0;j<GAME_PLAYER;j++)
      //{
      //  if (j==GetMeChairID()) continue;
      WORD wShowPlayer=(i+1)%GAME_PLAYER;
      WORD wViewChairID=SwitchViewChairID(wShowPlayer);
      BYTE bCardData[MAX_COUNT];
      BYTE bCardCount=m_GameLogic.SwitchToCardData(pCheatCard->cbCardIndex[wShowPlayer],bCardData);

      //for (BYTE n=0;n<MAX_COUNT;n++)
      //{
      //  if(pCheatCard->cbCardData[(i+1)%GAME_PLAYER-1]!=0) bCardCount++;
      //}
      m_GameClientView.m_TableCard[wViewChairID].SetCardData(bCardData,bCardCount);
      m_GameClientView.m_UserCard[wViewChairID].SetCardData(0,0);
      //}
    }

  }

  //���½���
  CRect rcClient;
  GetClientRect(&rcClient);
  m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
  return true;
}

//�˿˶���
bool CGameClientEngine::BeginMoveCard()
{
  if(m_MoveCardItemArray.GetCount() == 0)
  {
    return false;
  }
  if(m_GameClientView.IsMovingCard())
  {
    return true;
  }

  MoveKind enMoveKind = m_MoveCardItemArray[0]->GetMoveKind();

  //���ַ���
  if(enMoveKind == MK_StartCard)
  {
    return BeginMoveStartCard(m_MoveCardItemArray[0]);
  }
  //����
  else if(enMoveKind == MK_OutCard)
  {
    return BeginMoveOutCard(m_MoveCardItemArray[0]);
  }
  //����
  else if(enMoveKind == MK_SendCard)
  {
    return BeginMoveSendCard(m_MoveCardItemArray[0]);
  }
  //����
  else if(enMoveKind == MK_ReplaceCard)
  {
    return BeginMoveReplaceCard(m_MoveCardItemArray[0]);
  }

  return false;
}

//ֹͣ����
void CGameClientEngine::StopMoveCard()
{
  if(m_GameClientView.IsMovingCard())
  {
    m_GameClientView.StopMoveCard();
  }

  while(BeginMoveCard())
  {
    m_GameClientView.StopMoveCard();
  }
}

//���ƶ���
bool CGameClientEngine::BeginMoveOutCard(const CMoveCardItem *pMoveCardItem)
{
  const COutCardItem *pOutCardItem = (const COutCardItem *)pMoveCardItem;

  //�������
  WORD wMeChairId = GetMeChairID();
  WORD wOutCardUser = pOutCardItem->m_wOutCardUser;
  WORD wViewOutCardUser = SwitchViewChairID(wOutCardUser);
  BYTE cbOutCardData = pOutCardItem->m_cbOutCardData;


  //�����˿�
  for(WORD i = 0; i < GAME_PLAYER ; i++)
  {
    if(m_bUserCheat[i])
    {
      WORD wViewOrtherChairID=SwitchViewChairID((i+1)%GAME_PLAYER);
      m_GameClientView.m_TableCard[wViewOrtherChairID].SetCardData(m_cbCheatCardData,m_cbCheatCardCount);
      m_GameClientView.m_UserCard[wViewOrtherChairID].SetCardData(0,0);
    }
  }

  if(IsLookonMode() || wOutCardUser != wMeChairId)
  {
    //��������
    KillGameClock(IDI_OPERATE_CARD);

    //���ñ���
    m_wCurrentUser=INVALID_CHAIR;

    //������ʼ,������
    CPoint ptFrom,ptTo;
    if(wOutCardUser == wMeChairId)
    {
      ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
    }
    else
    {
      WORD wViewId = 0;
      ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
    }
    ptTo = m_GameClientView.m_DiscardCard[wViewOutCardUser].GetDispatchCardPos();

    //��������
    PlayCardSound(wOutCardUser,cbOutCardData);

    //���ý���
    m_GameClientView.SetCurrentUser(INVALID_CHAIR);
    m_GameClientView.SetUserAction(INVALID_CHAIR,0);
    m_GameClientView.SetOutCardInfo(wViewOutCardUser,cbOutCardData);

    //�����˿�
    if(wViewOutCardUser==MYSELF_VIEW_ID)
    {
      //ɾ���˿�
      if(!m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData))
      {
        ASSERT(FALSE);
        return false;
      }

      SetHandCardControl(m_cbCardIndex,0x00);
    }
    else
    {
      WORD wUserIndex=0;
      m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
    }

    tagMoveCardItem mci;
    mci.ptFrom = ptFrom;
    mci.ptTo = ptTo;
    mci.cbCardCount = 1;
    mci.cbCardData = cbOutCardData;
    mci.wViewChairId = wViewOutCardUser;
    m_GameClientView.OnMoveCardItem(mci);
  }

  return true;
}

//���ƶ���
bool CGameClientEngine::BeginMoveSendCard(const CMoveCardItem *pMoveCardItem)
{
  const CSendCardItem *pSendCardItem = (const CSendCardItem *)pMoveCardItem;

  WORD wSendUserViewId = SwitchViewChairID(pSendCardItem->m_wSendCardUser);

  //������ʼ,������
  CPoint ptFrom,ptTo;
  bool bCgMagicPos = false;
  if(!pSendCardItem->m_bTail)
  {
    ASSERT(m_wHeapHead != INVALID_CHAIR);
    ptFrom = m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapHead)].GetDispatchCardPos(false);
    //ɾ����ͷ
    if(!m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapHead)].DeductionHeapCard(false, bCgMagicPos))
    {
      m_wHeapHead = (m_wHeapHead+1)%4;
      ptFrom = m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapHead)].GetDispatchCardPos(false);
      VERIFY(m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapHead)].DeductionHeapCard(false, bCgMagicPos));
    }
  }
  else
  {
    ASSERT(m_wHeapTail != INVALID_CHAIR);
    ptFrom = m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapTail)].GetDispatchCardPos(true);
    WORD wNextHeapTail = (m_wHeapTail+GAME_PLAYER-1)%4;
    //ɾ����β
    if(!m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapTail)].DeductionHeapCard(true, bCgMagicPos))
    {
      m_wHeapTail = (m_wHeapTail+GAME_PLAYER-1)%4;
      ptFrom = m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapTail)].GetDispatchCardPos(true);
      VERIFY(m_GameClientView.m_HeapCard[SwitchToViewHeap(m_wHeapTail)].DeductionHeapCard(true, bCgMagicPos));
    }
    bool bHaveMagic = m_GameClientView.m_HeapCard[SwitchToViewHeap(wNextHeapTail)].HaveMagicCard();
    if(!bHaveMagic && bCgMagicPos && !m_GameClientView.m_HeapCard[SwitchToViewHeap(wNextHeapTail)].IsEmpty())
    {
      m_GameClientView.m_HeapCard[SwitchToViewHeap(wNextHeapTail)].SetCardData(0, m_GameLogic.SwitchToCardData(m_bMagicIndex));
    }
  }
  if(wSendUserViewId == MYSELF_VIEW_ID)
  {
    ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
  }
  else
  {
    WORD wViewId = 0;
    ptTo = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
  }

  //���ŷ�������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

  //���춯��
  tagMoveCardItem mci;
  mci.ptFrom = ptFrom;
  mci.ptTo = ptTo;
  mci.cbCardCount = 1;
  mci.cbCardData = 0;
  mci.wViewChairId = SwitchViewChairID(pSendCardItem->m_wSendCardUser);
  m_GameClientView.OnMoveCardItem(mci);

  return true;
}

//��������
bool CGameClientEngine::BeginMoveReplaceCard(const CMoveCardItem *pMoveCardItem)
{
  const CReplaceCardItem *pReplaceCardItem = (const CReplaceCardItem *)pMoveCardItem;

  //�������
  WORD wMeChairId = GetMeChairID();
  WORD wReplaceUser = pReplaceCardItem->m_wReplaceUser;
  WORD wReplaceUserViewId = SwitchViewChairID(wReplaceUser);
  BYTE cbReplaceCard = pReplaceCardItem->m_cbReplaceCard;

  if(IsLookonMode() || wReplaceUser != wMeChairId)
  {
    //��������
    PlayActionSound(wReplaceUser,WIK_REPLACE);

    //������ʼ,������
    CPoint ptFrom,ptTo;
    if(wReplaceUser == wMeChairId)
    {
      ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
    }
    else
    {
      WORD wViewId = 0;
      ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
    }
    //ptTo = m_GameClientView.m_DiscardCard[wReplaceUserViewId].GetDispatchCardPos();
    ptTo = m_GameClientView.GetHuaCardTailPos(wReplaceUserViewId);

    //���ö���
    m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
    m_GameClientView.SetUserAction(INVALID_CHAIR,0);
    m_GameClientView.SetUserAction(wReplaceUserViewId,WIK_REPLACE);

    //�����˿�
    if(wReplaceUserViewId==MYSELF_VIEW_ID)
    {
      //ɾ���˿�
      if(!m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard))
      {
        ASSERT(FALSE);
        return false;
      }

      //�����˿�
      SetHandCardControl(m_cbCardIndex,0x00);
    }
    else
    {
      //�����˿�
      WORD wUserIndex=0;
      if(m_GameClientView.m_UserCard[wUserIndex].IsHaveCurrentCard())
      {
        m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
      }
      else
      {
        WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
        m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount-1,false);
      }
    }

    tagMoveCardItem mci;
    mci.ptFrom = ptFrom;
    mci.ptTo = ptTo;
    mci.cbCardCount = 1;
    mci.cbCardData = cbReplaceCard;
    mci.wViewChairId = wReplaceUserViewId;
    m_GameClientView.OnMoveCardItem(mci);
  }

  return true;
}

//���ַ��ƶ���
bool CGameClientEngine::BeginMoveStartCard(const CMoveCardItem *pMoveCardItem)
{
  const CStartCardItem *pStartCardItem = (const CStartCardItem *)pMoveCardItem;

  //��ʼ,������
  CPoint ptFrom,ptTo;

  //����λ��
  WORD wHeapId = pStartCardItem->m_wHeapId;
  WORD wHeapViewId = SwitchToViewHeap(wHeapId);
  //����λ��
  WORD wHeapCardIndex = pStartCardItem->m_wHeapCardIndex;
  if(pStartCardItem->m_cbCardCount > wHeapCardIndex+1)
  {
    ASSERT(pStartCardItem->m_cbCardCount == 4);
    //ɾ��ǰ��Ķ���
    m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty(wHeapCardIndex,wHeapCardIndex+1);

    wHeapId = (wHeapId+1)%4;
    wHeapViewId = SwitchToViewHeap(wHeapId);
    wHeapCardIndex = HEAP_FULL_COUNT-(4-(wHeapCardIndex+1))-1;

    //��ȡ��ʼ��
    ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(WORD(wHeapCardIndex-1));

    //ɾ������
    m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty(HEAP_FULL_COUNT-1,
        pStartCardItem->m_cbCardCount-pStartCardItem->m_wHeapCardIndex-1);
  }
  else
  {
    WORD wHeapPos = wHeapCardIndex;
    if(pStartCardItem->m_cbCardCount == 4)
    {
      wHeapPos -= 3;
    }
    else
    {
      ASSERT(pStartCardItem->m_cbCardCount == 1);
      if(wHeapPos%2 != 0)
      {
        wHeapPos--;
      }
      else
      {
        wHeapPos++;
      }
    }
    //��ȡ��ʼ��
    ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(wHeapPos);

    //ɾ������
    if(pStartCardItem->m_cbCardCount == 4)
    {
      m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty(wHeapCardIndex,4);
    }
    else
    {
      ASSERT(pStartCardItem->m_cbCardCount == 1);
      if(wHeapCardIndex%2 != 0)
      {
        m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty(wHeapCardIndex-1,pStartCardItem->m_cbCardCount);
      }
      else
      {
        m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty(wHeapCardIndex+1,pStartCardItem->m_cbCardCount);
      }
    }
  }

  //��ȡ������
  WORD wViewChairId = SwitchViewChairID(pStartCardItem->m_wChairId);
  if(wViewChairId == MYSELF_VIEW_ID)
  {
    ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
  }
  else
  {
    WORD wUserIndex = 0;
    ptTo = m_GameClientView.m_UserCard[wUserIndex].GetDispatchCardPos();
  }

  //���ŷ�������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

  //���춯��
  tagMoveCardItem mci;
  mci.ptFrom = ptFrom;
  mci.ptTo = ptTo;
  mci.cbCardCount = pStartCardItem->m_cbCardCount;
  mci.cbCardData = 0;
  mci.wViewChairId = wViewChairId;
  m_GameClientView.OnMoveCardItem(mci);

  return true;
}

//���������Ϣ
LRESULT CGameClientEngine::OnMoveCardFinish(WPARAM wParam, LPARAM lParam)
{
  //У��
  ASSERT(m_MoveCardItemArray.GetCount() > 0);
  if(m_MoveCardItemArray.GetCount() == 0)
  {
    return 0;
  }

  MoveKind enMoveKind = m_MoveCardItemArray[0]->GetMoveKind();

  //������Ϣ
  if(enMoveKind == MK_StartCard)
  {
    CStartCardItem StartCardItem = *((CStartCardItem *)m_MoveCardItemArray[0]);

    //ɾ��������Ŀ
    SafeDelete(m_MoveCardItemArray[0]);
    m_MoveCardItemArray.RemoveAt(0);

    VERIFY(OnMoveStartCardFinish(&StartCardItem));
  }
  else if(enMoveKind == MK_OutCard)
  {
    COutCardItem OutCardItem = *((COutCardItem *)m_MoveCardItemArray[0]);

    //ɾ��������Ŀ
    SafeDelete(m_MoveCardItemArray[0]);
    m_MoveCardItemArray.RemoveAt(0);

    VERIFY(OnMoveOutCardFinish(&OutCardItem));
  }
  else if(enMoveKind == MK_SendCard)
  {
    CSendCardItem SendCardItem = *((CSendCardItem *)m_MoveCardItemArray[0]);

    //ɾ��������Ŀ
    SafeDelete(m_MoveCardItemArray[0]);
    m_MoveCardItemArray.RemoveAt(0);

    VERIFY(OnMoveSendCardFinish(&SendCardItem));
  }
  else if(enMoveKind == MK_ReplaceCard)
  {
    CReplaceCardItem ReplaceCardItem = *((CReplaceCardItem *)m_MoveCardItemArray[0]);

    //ɾ��������Ŀ
    SafeDelete(m_MoveCardItemArray[0]);
    m_MoveCardItemArray.RemoveAt(0);

    VERIFY(OnMoveReplaceCardFinish(&ReplaceCardItem));
  }
  else
  {
    ASSERT(FALSE);
    return 0;
  }

  //������һ������
  BeginMoveCard();

  return 0;
}

//���ƶ������
bool CGameClientEngine::OnMoveOutCardFinish(const COutCardItem *pOutCardItem)
{
  WORD wOutUserViewId = SwitchViewChairID(pOutCardItem->m_wOutCardUser);
  BYTE cbOutCardData = pOutCardItem->m_cbOutCardData;

  //��ӵ��Ѷ�����
  m_GameClientView.m_DiscardCard[wOutUserViewId].AddCardItem(cbOutCardData);

  //���ó���ָʾ
  m_GameClientView.SetDiscUser(wOutUserViewId);

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
}

//���ƶ������
bool CGameClientEngine::OnMoveSendCardFinish(const CSendCardItem *pSendCardItem)
{
  //���ñ���
  WORD wMeChairID = GetMeChairID();
  WORD wViewChairID=SwitchViewChairID(pSendCardItem->m_wSendCardUser);
  WORD wChairId = pSendCardItem->m_wSendCardUser;
  BYTE cbSendCardData = pSendCardItem->m_cbCardData;
  m_wCurrentUser = pSendCardItem->m_wCurrentUser;
  m_wReplaceUser = pSendCardItem->m_wReplaceUser;

  //ȡ�ƽ���
  if(wViewChairID!=MYSELF_VIEW_ID)
  {
    if(m_bUserCheat[GetMeChairID()] == false)
    {
      WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
      BYTE cbCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount()+m_cbWeaveCount[wChairId]*3;
      m_GameClientView.m_UserCard[wUserIndex].AddCardData(cbCardCount==MAX_COUNT-1?true:false);
    }
  }
  else
  {
    //�����˿�
    m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbSendCardData)]++;

    //�����˿�
    SetHandCardControl(m_cbCardIndex,cbSendCardData);
  }

  //��ǰ�û�
  if((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
  {
    //������
    ActiveGameFrame();

    m_cbActionMask = pSendCardItem->m_cbActionMask;
    if(m_cbListenStatus == 0)
    {
      //m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID]); //ȡ������
    }
    m_cbActionCard = pSendCardItem->m_cbCardData;

    //��������
    if(m_cbActionMask!=WIK_NULL)
    {
      //���ý���
      m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
    }
  }

  //���Ʋ���
  if((IsLookonMode()==false)&&(pSendCardItem->m_wReplaceUser==wMeChairID))
  {
    //����ʱ��
    SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
  }

  //������ʾ
  m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  //����ʱ��
  WORD wTimeCount=TIME_OPERATE_CARD;

  //����ʱ��
  if(m_wCurrentUser != INVALID_CHAIR)
  {
    m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
    SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
  }
  else
  {
    m_GameClientView.SetCurrentUser(INVALID_CHAIR);
  }

  return true;
}

//�����������
bool CGameClientEngine::OnMoveReplaceCardFinish(const CReplaceCardItem *pReplaceCardItem)
{
  WORD wReplaceUserViewId = SwitchViewChairID(pReplaceCardItem->m_wReplaceUser);
  BYTE cbReplaceCardData = pReplaceCardItem->m_cbReplaceCard;

  //��ӵ��Ѷ�����
  //m_GameClientView.m_DiscardCard[wReplaceUserViewId].AddCardItem( cbReplaceCardData );

  //���ó���ָʾ
  m_GameClientView.SetDiscUser(wReplaceUserViewId);

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  m_GameClientView.SetHuaCard(wReplaceUserViewId, cbReplaceCardData, false);

  return true;
}

//���ֶ������
bool CGameClientEngine::OnMoveStartCardFinish(const CStartCardItem *pStartCardItem)
{
  //���ñ���
  WORD wMeChairId = GetMeChairID();
  WORD wChairId = pStartCardItem->m_wChairId;
  WORD wViewChairId = SwitchViewChairID(wChairId);

  //����˿�
  if(wViewChairId == MYSELF_VIEW_ID)
  {
    m_GameClientView.m_HandCardControl.AddCardData(pStartCardItem->m_cbCardData,pStartCardItem->m_cbCardCount);
  }
  else
  {

    if(m_bUserCheat[GetMeChairID()] == false)
    {
      WORD wUserIndex = wViewChairId>2?2:wViewChairId;
      if(pStartCardItem->m_bLastItem)
      {
        m_GameClientView.m_UserCard[wUserIndex].AddCardData(true);
      }
      else
      {
        for(BYTE i = 0; i < pStartCardItem->m_cbCardCount; i++)
        {
          m_GameClientView.m_UserCard[wUserIndex].AddCardData(false);
        }
      }
    }
  }

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

  //�ж��Ƿ����һ����
  if(pStartCardItem->m_bLastItem)
  {
    //������
    if(!IsLookonMode())
    {
      ActiveGameFrame();
    }

    //�����˿�
    for(WORD i=0; i<4; i++)
    {
      m_GameClientView.m_HeapCard[SwitchToViewHeap(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
    }

    //�����˿�
    SetHandCardControl(m_cbCardIndex,0x00);

    m_GameClientView.m_HandCardControl.SetPositively(!IsLookonMode());

    //���Ʋ���
    if(!IsLookonMode()&&(m_wReplaceUser==wMeChairId))
    {
      //����ʱ��
      SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
    }

    //������ʾ,��������
    if(!IsLookonMode()&&(m_wCurrentUser==wMeChairId))
    {
      m_GameClientView.SetStatusFlag(true,false);

      //m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);  //ȡ������

      if(m_cbActionMask != WIK_NULL)
      {
        m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
      }
    }
    else if(m_wCurrentUser != wMeChairId && m_wCurrentUser != INVALID_CHAIR)
    {
      m_GameClientView.SetStatusFlag(false,true);
    }

    //����ʱ��
    if(m_wCurrentUser!=INVALID_CHAIR)
    {
      m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
      SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
    }
  }

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
}

//λ��ת��
WORD CGameClientEngine::SwitchToViewHeap(WORD wPos)
{
  if(GetMeChairID() == 1)
  {
    return wPos;
  }
  else
  {
    return (wPos+2)%4;
  }

  ASSERT(FALSE);
  return wPos;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
