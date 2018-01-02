#include "StdAfx.h"
#include "GameLogic.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

#define IDC_START           100               //��ʼ��ť

#define IDC_SCORENONE         101               //����ť
#define IDC_SCOREONE          102               //��һ��ť


#define IDC_TRUSTEE_CONTROL       104               //�йܿ���

//������ʶ
#define IDI_BOMB_EFFECT         101               //������ʶ
#define IDI_DISC_EFFECT         102               //����Ч��
#define IDI_MOVE_CARD         103               //

//������Ŀ
#define BOMB_EFFECT_COUNT       12                //������Ŀ
#define DISC_EFFECT_COUNT       8               //����Ч��    

#define MOVE_STEP_COUNT         6               //�ƶ�����
#define TIME_MOVE_CARD          30                //�ƶ�����ʱʱ��

#define NAME_WIDTH            120
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
  ON_WM_CREATE()
  ON_WM_SETCURSOR()
  ON_WM_LBUTTONDOWN()
  ON_CONTROL_RANGE(BN_CLICKED, IDC_SCORENONE, IDC_SCOREONE, OnChip)
  ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
  //��־����
  m_bOutCard=false;
  m_bWaitOther=false;
  m_bHuangZhuang=false;
  m_bMaiDiState=false;
  ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

  //��Ϸ����
  m_lCellScore=0L;
  m_wBankerUser=INVALID_CHAIR;
  m_cbLianZhuang = 0;
  m_wCurrentUser=INVALID_CHAIR;
  m_cbFengQuan = GAME_PLAYER;
  m_bMagicCard = 0;
  ZeroMemory(m_cbHuaCardCount,sizeof(m_cbHuaCardCount));
  ZeroMemory(m_cbHuaCardData,sizeof(m_cbHuaCardData));
  ZeroMemory(m_bMaiDi,sizeof(m_bMaiDi));

  //��������
  m_bBombEffect=false;
  m_cbBombFrameIndex=0;

  //����Ч��
  m_wDiscUser=INVALID_CHAIR;
  m_cbDiscFrameIndex=0;

  //�û�״̬
  m_cbCardData=0;
  m_wOutCardUser=INVALID_CHAIR;
  ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
  ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

  //���ƶ�������
  m_bEnableAnimate = true;
  m_bCardMoving = false;

  m_bShowHeapCard=false;

  return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }

  //������Դ
  g_CardResource.LoadResource(this);

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_ImageCenter.LoadFromResource(this,hInstance,IDB_VIEW_CENTER);
  m_ImageBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);



  m_ImageOutCard.LoadImage(this,hInstance,TEXT("OUT_CARD_TIP"));


  m_ImageTrustee.LoadImage(this,hInstance,TEXT("TRUSTEE"));
  m_ImageActionAni.LoadImage(this,hInstance,TEXT("ACTIONANI"));
  m_ImageDisc.LoadImage(this,hInstance,TEXT("DISC"));
  m_ImageArrow.LoadImage(this,hInstance,TEXT("ARROW"));
  m_PngListenFlag.LoadImage(this,hInstance,TEXT("LISTEN_FLAG"));

  //������Դ

  ImageUserReady.LoadFromResource(this,hInstance,IDB_USER_READY);
  ImageTimeBack.LoadFromResource(this,hInstance,IDB_TIME_BACK);
  ImageTimeNumber.LoadFromResource(this,hInstance,IDB_TIME_NUMBER);

  m_ImageActionBack.LoadFromResource(this,hInstance,IDB_ACTION_BACK);
  m_PngHuangZhuang.LoadFromResource(this,hInstance,IDB_HUANG_ZHUANG);
  m_ImageWait.LoadFromResource(this,hInstance,IDB_WAIT_TIP);
  m_ImageMagicCardBack.LoadFromResource(this,hInstance,IDB_MAGIC_CARD_BACK);
  m_ImageMaiDiFlag.LoadFromResource(this,hInstance,IDB_MAI_DI_FLAG);

  m_ImageUserFlag.LoadFromResource(this,hInstance,IDB_USER_FLAG);



  m_HandCardControl.LoadResource(this);
  //��������
  enDirection Direction[]= {Direction_North,Direction_West,Direction_South,Direction_East};

  //���ÿؼ�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //�û��˿�
    m_TableCard[i].SetDirection(Direction[i*2]);
    m_DiscardCard[i].SetDirection(Direction[i*2]);

    //����˿�
    for(BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++)
    {
      m_WeaveCard[i][j].SetDisplayItem(true);
      m_WeaveCard[i][j].SetDirection(Direction[i*2]);
      m_WeaveCard[i][j].LoadResource(this);
    }
  }
  for(int i = 0; i < 4; i++)
  {
    m_HeapCard[i].SetDirection(Direction[i]);
  }

  //���ÿؼ�
  m_UserCard[0].SetDirection(Direction_North);

  //�����ؼ�
  CRect rcCreate(0,0,0,0);
  m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);
  m_DrawSiceWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,11,NULL);

  m_GameScore.Create(IDD_GAME_SCORE,this);
  /*BYTE byCard[] = {
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
  };
    tagScoreInfo ScoreInfo;
    ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
    tagWeaveInfo WeaveInfo;
    ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));
    ScoreInfo.wChiHuUser = 0;
    ScoreInfo.wBankerUser = 0;
    ScoreInfo.wProvideUser = 0;
    ScoreInfo.cbFanCount = 88;
    ScoreInfo.cbCardCount = 2;
    memset(ScoreInfo.bMaidi,1,sizeof(ScoreInfo.bMaidi));
    CopyMemory( ScoreInfo.cbCardData,byCard,sizeof(BYTE)*ScoreInfo.cbCardCount );
    for( WORD i = 0; i < GAME_PLAYER; i++ )
    {
      lstrcpy( ScoreInfo.szUserName[i],(i%2==1)?TEXT("�������ð�"):TEXT("�������Ұ�") );
      ScoreInfo.lGameScore[i] = 123L*((i%2==1)?-1:1);
    }
    WeaveInfo.cbWeaveCount = 5;
    for (int i = 0; i < WeaveInfo.cbWeaveCount; i++)
    {
      WeaveInfo.cbCardCount[i] = 3;
      WeaveInfo.cbCardData[i][0] = 0x01;
      WeaveInfo.cbCardData[i][1] = 0x01;
      WeaveInfo.cbCardData[i][2] = 0x01;
    }
    CChiHuRight chr;
    chr |= 0x0480;
    ScoreInfo.cbHuaCardCount = 3;
    ScoreInfo.cbFanCount = 23;
    m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
  m_GameScore.ShowWindow( SW_SHOW );*/

  //�����ؼ�
  m_btStart.Create(NULL,WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
  m_btStart.SetButtonImage(IDB_BT_START,hInstance,false,false);

  //�йܰ�ť
//  m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
//  m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,hInstance,false,false);

  //��װ�ť
  m_btMaiDi.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCOREONE);
  m_btMaiDi.SetButtonImage(IDB_BT_MAI_DI,hInstance,false,false);

  //���װ�ť
  m_btDingDi.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCOREONE);
  m_btDingDi.SetButtonImage(IDB_BT_DING_DI,hInstance,false,false);

  //����ť
  m_btBuMai.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCORENONE);
  m_btBuMai.SetButtonImage(IDB_BT_BU_MAI,hInstance,false,false);

  //������ť
  m_btBuDing.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCORENONE);
  m_btBuDing.SetButtonImage(IDB_BT_BU_DING,hInstance,false,false);

  return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
  switch(LOWORD(wParam))
  {
    case IDC_START:
    {
      SendEngineMessage(IDM_START,0,0);
      return TRUE;
    }
    case IDC_TRUSTEE_CONTROL:
    {
      SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
      return TRUE;
    }
  }
  return __super::OnCommand(wParam, lParam);
}

//���ý���
VOID CGameClientView::ResetGameView()
{
  //��־����
  m_bOutCard=false;
  m_bWaitOther=false;
  m_bHuangZhuang=false;
  m_bMaiDiState=false;
  ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

  //��Ϸ����
  m_lCellScore=0L;
  m_wBankerUser=INVALID_CHAIR;
  m_cbLianZhuang = 0;
  m_wCurrentUser=INVALID_CHAIR;
  m_cbFengQuan = GAME_PLAYER;
  m_bMagicCard = 0;
  ZeroMemory(m_cbHuaCardCount,sizeof(m_cbHuaCardCount));
  ZeroMemory(m_cbHuaCardData,sizeof(m_cbHuaCardData));
  ZeroMemory(m_bMaiDi,sizeof(m_bMaiDi));

  //��������
  m_bBombEffect=false;
  m_cbBombFrameIndex=0;

  //����Ч��
  m_wDiscUser=INVALID_CHAIR;
  m_cbDiscFrameIndex=0;

  //�û�״̬
  m_cbCardData=0;
  m_wOutCardUser=INVALID_CHAIR;
  ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

  //���ƶ�������
  m_bEnableAnimate = true;
  m_bCardMoving = false;

  //��������
  m_btStart.ShowWindow(SW_HIDE);
  m_ControlWnd.ShowWindow(SW_HIDE);
  m_btMaiDi.ShowWindow(SW_HIDE);
  m_btDingDi.ShowWindow(SW_HIDE);
  m_btBuMai.ShowWindow(SW_HIDE);
  m_btBuDing.ShowWindow(SW_HIDE);

  m_GameScore.RestorationData();

  //���ÿؼ�
//  m_btStusteeControl.EnableWindow(FALSE);

  //�˿�����
  m_UserCard[0].SetCardData(0,false);
  m_HandCardControl.SetPositively(false);
  m_HandCardControl.SetDisplayItem(false);
  m_HandCardControl.SetCardData(NULL,0,0);

  //�˿�����
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_TableCard[i].SetCardData(NULL,0);
    m_DiscardCard[i].SetCardData(NULL,0);
    for(BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++)
    {
      m_WeaveCard[i][j].SetCardData(NULL,0);
    }
  }
  for(int i = 0; i < 4; i++)
  {
    m_HeapCard[i].SetHeapCardInfo(0,0);
  }

  //���ٶ�ʱ��
  KillTimer(IDI_MOVE_CARD);
  KillTimer(IDI_DISC_EFFECT);
  KillTimer(IDI_BOMB_EFFECT);
  m_DrawSiceWnd.StopSicing(false);

  m_bShowHeapCard=false;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
  int m_nXFace= 50, m_nYFace= 50;
  int m_nXBorder=0,m_nYBorder=0;
  int m_nXTimer=50,m_nYTimer=50;
  //��������
  m_ptAvatar[0].x=nWidth/2-m_nXFace;
  m_ptAvatar[0].y=5+m_nYBorder;
  m_ptNickName[0].x=nWidth/2-60-120;
  m_ptNickName[0].y=10;
  m_ptClock[0].x=nWidth/2-m_nXFace-m_nXTimer-2;
  m_ptClock[0].y=17+m_nYBorder;
  m_ptReady[0].x=m_ptNickName[0].x-30;
  m_ptReady[0].y=m_nYBorder+20;
  m_UserFlagPos[0].x=nWidth/2+30;
  m_UserFlagPos[0].y=5;
  m_PointTrustee[0].x=nWidth/2-m_nXFace-69;
  m_PointTrustee[0].y=5+m_nYBorder;
  m_UserListenPos[0].x=nWidth/2-m_nXFace+5;
  m_UserListenPos[0].y=m_nYBorder+m_nYFace+20;
  //SetFlowerControlInfo( 0,m_ptAvatar[0].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[0].y+m_nYFace );

  m_ptAvatar[1].x=nWidth/2-m_nXFace;
  m_ptAvatar[1].y=nHeight-m_nYBorder-m_nYFace-5;
  m_ptNickName[1].x=nWidth/2-60-120;
  m_ptNickName[1].y=nHeight-10-16;
  m_ptClock[1].x=nWidth/2-m_nXFace/2-m_nXTimer-2;
  m_ptClock[1].y=nHeight-m_nYBorder-m_nYTimer-8+40;
  m_ptReady[1].x=m_ptNickName[1].x-30;;
  m_ptReady[1].y=nHeight-m_nYBorder-20;
  m_UserFlagPos[1].x=nWidth/2+30;
  m_UserFlagPos[1].y=nHeight-5-35;
  m_PointTrustee[1].x=nWidth/2-m_nXFace-69;
  m_PointTrustee[1].y=nHeight-m_nYBorder-35;
  m_UserListenPos[1].x=nWidth/2-m_nXFace;
  m_UserListenPos[1].y=nHeight-m_nYBorder-140;
  //SetFlowerControlInfo( 2,m_ptAvatar[1].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[1].y-BIG_FACE_HEIGHT );

  //�û��˿�
  m_UserCard[0].SetControlPoint(nWidth/2-240,m_nYBorder+m_nYFace+10);
  m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-9,enXCenter,enYBottom);

  //�����˿�
  m_TableCard[0].SetControlPoint(nWidth/2+200,m_nYBorder+30+20);
  m_TableCard[1].SetControlPoint(nWidth/2-200,nHeight-m_nYFace-m_nYBorder-63);

  //�ѻ��˿�
  int nXCenter=nWidth/2;
  int nYCenter=nHeight/2-40+10;
  m_HeapCard[0].SetControlPoint(nXCenter-189+36,nYCenter-220);
  m_HeapCard[1].SetControlPoint(nXCenter+252,nYCenter-110);
  m_HeapCard[2].SetControlPoint(nXCenter+190-36,nYCenter+222);
  m_HeapCard[3].SetControlPoint(nXCenter-289,nYCenter+165);

  //�����˿�
  m_DiscardCard[0].SetControlPoint(nXCenter+217,nYCenter-160);
  m_DiscardCard[1].SetControlPoint(nXCenter-215,nYCenter+170);

  //����˿�
  int nXControl = nWidth/2+360;
  int nYControl = m_nYBorder+53;
  for(BYTE i = 0; i < CountArray(m_WeaveCard[0]); i++)
  {
    m_WeaveCard[0][i].SetControlPoint(nXControl,nYControl);
    nXControl -= 115;
  }

  //����˿�
  nXControl = nWidth/2-370;
  nYControl = nHeight-m_nYFace-m_nYBorder-63;
  for(BYTE i = 0; i < CountArray(m_WeaveCard[1]); i++)
  {
    m_WeaveCard[1][i].SetControlPoint(nXControl,nYControl);
    nXControl += 125;
  }

  //�����˿�
  m_PointHuaCard[0].SetPoint(nWidth/2+55, 2);
  m_PointHuaCard[1].SetPoint(nWidth/2-285, nHeight-38);

  //ɫ�Ӵ���
  CRect rcSice;
  m_DrawSiceWnd.GetWindowRect(&rcSice);
  m_DrawSiceWnd.SetBenchmarkPos((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2);
  m_DrawSiceWnd.MoveWindow((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2,rcSice.Width(),rcSice.Height());

  //�ƶ���ť
  CRect rcButton;
  HDWP hDwp=BeginDeferWindowPos(32);
  m_btStart.GetWindowRect(&rcButton);
  const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

  //�ƶ�����
  DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-m_nYBorder-175,0,0,uFlags);
//  DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-rcButton.Height(),0,0,uFlags);

  DeferWindowPos(hDwp,m_btMaiDi,NULL,nWidth/2-10-rcButton.Width(),nHeight-195-m_nYBorder,0,0,uFlags);
  DeferWindowPos(hDwp,m_btDingDi,NULL,nWidth/2-10-rcButton.Width(),nHeight-195-m_nYBorder,0,0,uFlags);
  DeferWindowPos(hDwp,m_btBuMai,NULL,nWidth/2+10,nHeight-195-m_nYBorder,0,0,uFlags);
  DeferWindowPos(hDwp,m_btBuDing,NULL,nWidth/2+10,nHeight-195-m_nYBorder,0,0,uFlags);

  //���ƴ���
  CRect rcControlWnd;
  m_ControlWnd.GetWindowRect(&rcControlWnd);
  DeferWindowPos(hDwp,m_ControlWnd,NULL,nWidth-rcControlWnd.Width()-30,nHeight-rcControlWnd.Height()-132,0,0,uFlags);

  //�����ƶ�
  EndDeferWindowPos(hDwp);

  CRect rcGameScore;
  m_GameScore.GetWindowRect(&rcGameScore);
  CPoint ptPos((nWidth-rcGameScore.Width())/2,(nHeight-rcGameScore.Height())*2/5-30);
  ClientToScreen(&ptPos);
  m_GameScore.SetWindowPos(NULL,ptPos.x,ptPos.y,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOCOPYBITS);

  return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
  //�滭����
  DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
  DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

  CString szBuffer;

  //�滭�û�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
//#ifdef DEBUG
//    CRect rc (m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+16);
//    CDFontEx::DrawText(this, pDC, 14, 400,TEXT("���ǵǼǿ��ϵ����Ӱ�����˹������˵����������˹�ӵ£�������Ҷ��ǿ������׶Σ�"),rc, RGB(255,255,255),(i==1?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS);
//    if ( i== m_wBankerUser)
//    {
//      szBuffer.Format(TEXT("���%d"),m_bMaiDi[i]);
//      DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y+14,14,400);
//    }
//    if ( i!= m_wBankerUser)
//    {
//      szBuffer.Format(TEXT("����%d"),m_bMaiDi[i]);
//      DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x+60,m_ptNickName[i].y+14,14,400);
//    }
//      int nMoveTimeX = ((m_DiscardCard[0].GetCardCount()>42||m_DiscardCard[1].GetCardCount()>42)?200:0);
//    DrawUserTimerEx(pDC,nWidth/2-nMoveTimeX,nHeight/2,22);
//    if(i==0)
//      m_ImageArrow.DrawImage(pDC,nWidth/2-15-nMoveTimeX,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i*2,0);
//    else if(i==1)
//      m_ImageArrow.DrawImage(pDC,nWidth/2-15-nMoveTimeX,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i*2,0);
//    DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
//    DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,0);
//    m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
//#endif
    IClientUserItem *pUserItem =GetClientUserItem(i);
    if(pUserItem!=NULL)
    {
      //�û�����
      CRect rc(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+16);
      CDFontEx::DrawText(this, pDC, 14, 400,pUserItem->GetNickName(),rc, RGB(255,255,255),DT_RIGHT|DT_END_ELLIPSIS);
      //DrawTextString(pDC,pUserItem->GetNickName(),RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y,14,400);

      //�û����
      TCHAR szBuffer[64]=TEXT("");
      LONGLONG lUserScore=pUserItem->GetUserScore();
      _sntprintf(szBuffer,sizeof(szBuffer),TEXT("�Ƹ���%I64d"),lUserScore);
      rc.SetRect(nWidth/2+140,m_ptNickName[i].y,nWidth/2+140+100,m_ptNickName[i].y+16);
      CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(255,255,0),DT_LEFT|DT_END_ELLIPSIS);

      //�滭���
      if(m_bMaiDi[i])
      {
        m_ImageMaiDiFlag.TransDrawImage(pDC,nWidth/2+100,m_ptNickName[i].y-5,RGB(255,0,255));
      }

      /*if ( i== m_wBankerUser)
      {
        szBuffer.Format(TEXT("���%d"),m_bMaiDi[i]);
        DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y+14,14,400);
      }
      if ( i!= m_wBankerUser)
      {
        szBuffer.Format(TEXT("����%d"),m_bMaiDi[i]);
        DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y+14,14,400);
      }*/

      //������Ϣ
      WORD wUserTimer=GetUserClock(i);

      int nMoveTimeX = ((m_DiscardCard[0].GetCardCount()>42||m_DiscardCard[1].GetCardCount()>42)?200:0);
      if((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
      {
        DrawUserTimerEx(pDC,nWidth/2-nMoveTimeX,nHeight/2,wUserTimer);
        if(m_wCurrentUser==0)
        {
          m_ImageArrow.DrawImage(pDC,nWidth/2-15-nMoveTimeX,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser*2,0);
        }
        else if(m_wCurrentUser==1)
        {
          m_ImageArrow.DrawImage(pDC,nWidth/2-15-nMoveTimeX,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser*2,0);
        }
      }
      if((wUserTimer!=0)&&(m_wCurrentUser==INVALID_CHAIR))
      {
        DrawUserTimerEx(pDC,nWidth/2-nMoveTimeX,nHeight/2,wUserTimer);
        if(i==0)
        {
          m_ImageArrow.DrawImage(pDC,nWidth/2-15-nMoveTimeX,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i*2,0);
        }
        else if(i==1)
        {
          m_ImageArrow.DrawImage(pDC,nWidth/2-15-nMoveTimeX,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i*2,0);
        }
      }

      if(pUserItem->GetUserStatus()==US_READY)
      {
        DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
      }
      //if(!CD2DEngine::GetD2DEngine())
      //DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserItem);

      //�й�
      if(m_bTrustee[i])
      {
        m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
      }
    }

  }

//#ifdef DEBUG
//  m_wBankerUser = (m_wBankerUser+1)%GAME_PLAYER;
//  m_bOutCard =true;
//  m_bWaitOther =true;
//  //��ׯ��־
//  m_bHuangZhuang=true;
//  //���Ʊ�־
//  for (WORD i=0;i<GAME_PLAYER;i++)
//  {
//    m_bListenStatus[i]=true;
//
//  }
//  m_bMaiDiState=true;
//#endif
  //�û���־
  if(m_wBankerUser!=INVALID_CHAIR)
  {
    //����λͼ
    int nImageWidth=m_ImageUserFlag.GetWidth()/4;
    int nImageHeight=m_ImageUserFlag.GetHeight();

    //�滭��־
    for(BYTE i =0; i<GAME_PLAYER; i++)
    {
      if(i == m_wBankerUser)
      {
        WORD wIndex=i/*(m_wBankerUser+GAME_PLAYER-i)%GAME_PLAYER*/;
        BYTE cbImagePos = m_cbLianZhuang%4;
        m_ImageUserFlag.TransDrawImage(pDC,m_UserFlagPos[wIndex].x,m_UserFlagPos[wIndex].y,
                                       nImageWidth,nImageHeight,nImageWidth*cbImagePos,0,RGB(255,0,255));
      }
    }
  }

  //�����˿�
  for(int i = 0; i < GAME_PLAYER; i++)
  {
    m_DiscardCard[i].DrawCardControl(pDC);
  }

  //�û��˿�
  for(int i = 0; i < GAME_PLAYER-1; i++)
  {
    m_UserCard[i].DrawCardControl(pDC);
  }

  if(m_bShowHeapCard)
  {
    //�ѻ��˿�
    for(int i = 0; i < 4; i++)
    {
      m_HeapCard[i].DrawCardControl(pDC);
    }
  }

  //�����˿�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_TableCard[i].DrawCardControl(pDC);
    for(BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++)
    {
      m_WeaveCard[i][j].DrawCardControl(pDC,false);
    }
  }

  //�����˿�
  //test
  /*for (int i = 0; i < 4; i++)
  {
    m_cbHuaCardCount[i] = 4;
    for (int j = 0; j < 8; j++)
      m_cbHuaCardData[i][j] = 0x38+j;
  }*/
  //test end
  //��
  if(m_cbHuaCardCount[0] > 0)
  {
    int nXPos = m_PointHuaCard[0].x+(8-m_cbHuaCardCount[0])*24;
    int nYPos = m_PointHuaCard[0].y;

    //�滭�˿�
    for(int i=int(m_cbHuaCardCount[0])-1; i>=0; i--)
    {
      g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbHuaCardData[0][i],nXPos,nYPos,24,36);
      nXPos += 24;
    }
  }
  //��
  if(m_cbHuaCardCount[1] > 0)
  {
    int nXPos = m_PointHuaCard[1].x;
    int nYPos = m_PointHuaCard[1].y;

    //�滭�˿�
    for(int i=0; i<m_cbHuaCardCount[1]; i++)
    {
      g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbHuaCardData[1][i],nXPos,nYPos,24,36);
      nXPos += 24;
    }
  }

  //�����˿�
  m_HandCardControl.DrawCardControl(pDC);

  //�滭����
  m_ImageMagicCardBack.TransDrawImage(pDC,10,10,RGB(255,0,255));
  if(0 != m_bMagicCard)
  {
    //�滭����
    g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_bMagicCard,65,22);
  }

  //������ʾ
  if(m_bOutCard==true)
  {
    m_ImageOutCard.DrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-165);
  }

  //�ȴ���ʾ
  if(m_bWaitOther==true)
  {
    m_ImageWait.TransDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-165,RGB(255,0,255));
  }

  //��ׯ��־
  if(m_bHuangZhuang==true)
  {
    m_PngHuangZhuang.TransDrawImage(pDC, (nWidth-m_PngHuangZhuang.GetWidth())/2, nHeight/2-90,RGB(255,0,255));
  }

  //���Ʊ�־
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(m_bListenStatus[i]==true)
    {
      m_PngListenFlag.DrawImage(pDC,m_UserListenPos[i].x,m_UserListenPos[i].y);
    }
  }

  //�û�״̬
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
    {
      //����λ��
      int nXPos=0,nYPos=0;
      switch(i)
      {
        case 0: //����
        {
          nXPos=nWidth/2-58;
          nYPos=95;
          break;
        }
        case 1: //����
        {
          nXPos=nWidth/2-58;
          nYPos=nHeight-220;
          break;
        }
      }

      //�滭����
      if(m_cbUserAction[i]!=WIK_NULL)
      {
        //�滭����
        if(m_bBombEffect==true && i == 0)
        {
          //�滭Ч��
          INT nItemHeight=m_ImageActionAni.GetHeight();
          INT nItemWidth=m_ImageActionAni.GetWidth()/3;

          //�滭����
          int nYImagePos=-1;
          if(m_cbUserAction[i]&WIK_LEFT || m_cbUserAction[i]&WIK_CENTER || m_cbUserAction[i]&WIK_RIGHT)
          {
            nYImagePos=0;
          }
          else if(m_cbUserAction[i]&WIK_PENG)
          {
            nYImagePos=nItemWidth;
          }
          else if(m_cbUserAction[i]&WIK_GANG)
          {
            nYImagePos=nItemWidth*2;
          }
          /*else if( m_cbUserAction[i]&WIK_LISTEN ) nYImagePos = nItemHeight*3;
          else if (m_cbUserAction[i]&WIK_CHI_HU) nYImagePos=nItemHeight*4;
          else if( m_cbUserAction[i]&WIK_REPLACE ) nYImagePos = nItemHeight*6;*/
          else
          {
            nYImagePos=-1;
          }
          if(-1 != nYImagePos)
          {
            //��������
            m_ImageActionBack.TransDrawImage(pDC,nXPos,nYPos,RGB(255,255,255));

            m_ImageActionAni.DrawImage(pDC,nXPos+29,nYPos+29,nItemWidth,nItemHeight,
                                       /*nItemWidth**/nYImagePos/*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT)*/,0,nItemWidth,nItemHeight);
          }
        }
      }
      else
      {
        //��������
        m_ImageActionBack.TransDrawImage(pDC,nXPos,nYPos,RGB(255,255,255));

        //�滭�˿�
        g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+39,nYPos+30);
      }

    }
  }

  //����Ч��
  if(m_wDiscUser!=INVALID_CHAIR)
  {
    CSize SizeDisc(m_ImageDisc.GetWidth()/DISC_EFFECT_COUNT,m_ImageDisc.GetHeight());
    CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();
    pt.Offset(-SizeDisc.cx/2,-SizeDisc.cy);
    //�滭��Ϣ
    m_ImageDisc.DrawImage(pDC,pt.x,pt.y,SizeDisc.cx,SizeDisc.cy,
                          m_cbDiscFrameIndex*SizeDisc.cx,0,SizeDisc.cx,SizeDisc.cy);
  }

  //���ƻ��ƶ���
  DrawMoveCardItem(pDC);

  ////�滭���
  //if (m_bMaiDiState)
  //{
  //  DrawTextString(pDC,TEXT("������/����"),RGB(255,255,255),RGB(201,229,133),nWidth/2-5,nHeight/2-63,36,400,DT_CENTER);
  //}

  return;
}

//�滭�˿˶���
void CGameClientView::DrawMoveCardItem(CDC *pDC)
{
  if(m_bCardMoving)
  {
    int nXDraw = m_MoveCardItem.ptFrom.x, nYDraw = m_MoveCardItem.ptFrom.y;
    switch(m_enMoveDirection)
    {
      case Direction_East:
      case Direction_West:
      {
        //����
        if(m_MoveCardItem.cbCardData != 0)
        {
          if(m_enMoveDirection == Direction_East)
          {
            g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw);
          }
          else
          {
            g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw);
          }
        }
        //����
        else
        {
          if(m_MoveCardItem.cbCardCount == 1)
          {
            g_CardResource.m_ImageHeapSingleV.TransDrawImage(pDC,nXDraw,nYDraw ,RGB(255,0,255));
          }
          else
          {
            ASSERT(m_MoveCardItem.cbCardCount == 4);
            g_CardResource.m_ImageHeapDoubleV.TransDrawImage(pDC,nXDraw,nYDraw ,RGB(255,0,255));
            nYDraw += Y_HEAP_DOUBLE_V_EXCUSION;
            g_CardResource.m_ImageHeapDoubleV.TransDrawImage(pDC,nXDraw,nYDraw ,RGB(255,0,255));
          }
        }
      }
      break;
      case Direction_South:
      case Direction_North:
      {
        //����
        if(m_MoveCardItem.cbCardData != 0)
        {
          if(m_enMoveDirection == Direction_South)
          {
            g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw);
          }
          else
          {
            g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw);
          }
        }
        //����
        else
        {
          if(m_MoveCardItem.cbCardCount == 1)
          {
            g_CardResource.m_ImageHeapSingleH.TransDrawImage(pDC,nXDraw,nYDraw ,RGB(255,0,255));
          }
          else
          {
            ASSERT(m_MoveCardItem.cbCardCount == 4);
            g_CardResource.m_ImageHeapDoubleH.TransDrawImage(pDC,nXDraw,nYDraw ,RGB(255,0,255));
            nXDraw += g_CardResource.m_ImageHeapDoubleH.GetWidth();
            g_CardResource.m_ImageHeapDoubleH.TransDrawImage(pDC,nXDraw,nYDraw ,RGB(255,0,255));
          }
        }
      }
      break;
      default:
        ASSERT(FALSE);
    }
  }
}

//��������
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
  //�����˿�
  if(lCellScore!=m_lCellScore)
  {
    //���ñ���
    m_lCellScore=lCellScore;

    //���½���
    InvalidGameView(0,0,0,0);
  }

  return;
}

//�����˿�
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
  //�����˿�
  if(bHuangZhuang!=m_bHuangZhuang)
  {
    //���ñ���
    m_bHuangZhuang=bHuangZhuang;

    //���½���
    InvalidGameView(0,0,0,0);
  }

  return;
}

//ׯ���û�
void CGameClientView::SetBankerUser(WORD wBankerUser,BYTE cbLianZhuang)
{
  //�����û�
  if(wBankerUser!=m_wBankerUser)
  {
    //���ñ���
    m_wBankerUser=wBankerUser;
    m_cbLianZhuang = cbLianZhuang;

    //���½���
    InvalidGameView(0,0,0,0);
  }

  return;
}

//״̬��־
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
  //���ñ���
  m_bOutCard=bOutCard;
  m_bWaitOther=bWaitOther;

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//������Ϣ
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
  //���ñ���
  m_cbCardData=cbCardData;
  m_wOutCardUser=wViewChairID;

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
  //���ñ���
  if(wViewChairID<GAME_PLAYER)
  {
    m_cbUserAction[wViewChairID]=bUserAction;
    SetBombEffect(true);
  }
  else
  {
    ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
    if(m_bBombEffect)
    {
      SetBombEffect(false);
    }
  }

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//���ö���
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
  if(bBombEffect==true)
  {
    //���ñ���
    m_bBombEffect=true;
    m_cbBombFrameIndex=0;

    //����ʱ��
    SetTimer(IDI_BOMB_EFFECT,250,NULL);
  }
  else
  {
    //ֹͣ����
    if(m_bBombEffect==true)
    {
      //ɾ��ʱ��
      KillTimer(IDI_BOMB_EFFECT);

      //���ñ���
      m_bBombEffect=false;
      m_cbBombFrameIndex=0;

      //���½���
      InvalidGameView(0,0,0,0);
    }
  }

  return true;
}

//�����û�
void CGameClientView::SetDiscUser(WORD wDiscUser)
{
  if(m_wDiscUser != wDiscUser)
  {
    //���±���
    m_wDiscUser=wDiscUser;

    if(m_wDiscUser != INVALID_CHAIR)
    {
      SetTimer(IDI_DISC_EFFECT,250,NULL);
    }
    else
    {
      KillTimer(IDI_DISC_EFFECT);
    }

    //���½���
    InvalidGameView(0,0,0,0);
  }
  return;
}

//��ʱ���
void CGameClientView::SetCurrentUser(WORD wCurrentUser)
{
  if(m_wCurrentUser != wCurrentUser)
  {
    //���±���
    m_wCurrentUser=wCurrentUser;

    //���½���
    InvalidGameView(0,0,0,0);
  }
  return;
}
//�����й�
void CGameClientView::SetTrustee(WORD wTrusteeUser,bool bTrustee)
{
  //У������
  ASSERT(wTrusteeUser>=0&&wTrusteeUser<GAME_PLAYER);

  if(m_bTrustee[wTrusteeUser] !=bTrustee)
  {
    //��������
    m_bTrustee[wTrusteeUser]=bTrustee;

    //���½���
    InvalidGameView(0,0,0,0);
  }
  return;
}

//���Ʊ�־
void CGameClientView::SetUserListenStatus(WORD wViewChairID, bool bListenStatus)
{
  //���ñ���
  if(wViewChairID<GAME_PLAYER)
  {
    SetBombEffect(true);
    m_cbUserAction[wViewChairID]=WIK_LISTEN;
    m_bListenStatus[wViewChairID]=bListenStatus;
  }
  else
  {
    ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));
  }

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat)
{
  //��������
  int nStringLength=lstrlen(pszString);
  int nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  int nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  //�滭�߿�
  for(int i=0; i<CountArray(nXExcursion); i++)
  {
    CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nFormat);
  }

  //�滭����
  CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos,nYPos, crText, nFormat);

  return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
  //��ȡ���
  CPoint MousePoint;
  GetCursorPos(&MousePoint);
  ScreenToClient(&MousePoint);

  //�������
  CRect rcRePaint;
  bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,rcRePaint);

  //�ػ�����
  if(rcRePaint.IsRectEmpty()==false)
  {
    InvalidGameView(rcRePaint.left,rcRePaint.top,rcRePaint.Width(),rcRePaint.Height());
  }

  //������
  if(bHandle==false)
  {
    return __super::OnSetCursor(pWnd,nHitTest,uMessage);
  }

  return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
  __super::OnLButtonDown(nFlags, Point);

  //�˿��¼�
  m_HandCardControl.OnEventLeftHitCard();

  //test
  //for (int j = 0; j < 4; j++)
  ////for (int i = 0; i < 8; i++)
  //{
  //  CPoint pt = GetHuaCardTailPos(j);
  //  m_cbHuaCardCount[j]++;
  //}
  //BYTE byCard[] = {
  //  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
  //  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
  //  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
  //};
  //BYTE byCardCount = 17;
  //static int n = 0;
  //if( n==0 )
  //{
  //  m_HandCardControl.SetDisplayItem(true);
  //  m_HandCardControl.SetPositively(true);
  //  m_HandCardControl.SetMagicCard(0x38);
  //  static BYTE bWeaveCount = 0;
  //  m_HandCardControl.SetCardData(byCard,MAX_COUNT-1-bWeaveCount*3,byCard[MAX_COUNT-1]);
  //  for( BYTE i = 0; i < GAME_PLAYER; i++ )
  //  {
  //    m_HeapCard[i].SetHeapCardInfo(0,0);
  //    if (i == 2) m_HeapCard[i].SetCardData( 4,0x01 );
  //    //m_HeapCard[i].SetCardEmpty( 19,4 );
  //    m_TableCard[i].SetCardData(byCard,byCardCount-bWeaveCount*3);
  //    m_DiscardCard[i].SetCardData(byCard,27);
  //
  //    for (int j = 0; j < bWeaveCount; j++)
  //    {
  //      m_WeaveCard[i][j].SetCardData(byCard,3);
  //      m_WeaveCard[i][j].SetDirectionCardPos( 1 );
  //    }
  //    if( i < 3 )
  //      m_UserCard[i].SetCardData(MAX_COUNT-1-bWeaveCount*3,true);
  //    m_bTrustee[i] = true;
  //    m_cbUserAction[i] = WIK_REPLACE;
  //    m_bListenStatus[i] = true;
  //  }
  //  m_bWaitOther = true;
  //  m_bHuangZhuang = true;
  //  m_bOutCard = true;
  //  m_wOutCardUser = 3;
  //  m_cbCardData = 0x01;
  //  m_cbUserAction[3] = WIK_NULL;
  //  bWeaveCount++;
  //}
  //InvalidGameView(0,0,0,0);
  //bool bCgMagicPos = false;
  //if (!m_HeapCard[2].DeductionHeapCard(true, bCgMagicPos))
  //{
  //  m_HeapCard[1].DeductionHeapCard(true, bCgMagicPos);
  //}
  //if (bCgMagicPos)
  //  m_HeapCard[1].SetCardData(0, 0x01);

  //InvalidGameView(0,0,0,0);
  //  SetDiscUser( 1 );
  //  SetBombEffect(true);
  //  tagScoreInfo ScoreInfo;
  //  ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
  //  tagWeaveInfo WeaveInfo;
  //  ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));
  //  ScoreInfo.wChiHuUser = 0;
  //  ScoreInfo.wProvideUser = 0;
  //  ScoreInfo.cbFanCount = 88;
  //  ScoreInfo.cbCardCount = 2;
  //  CopyMemory( ScoreInfo.cbCardData,byCard,sizeof(BYTE)*ScoreInfo.cbCardCount );
  //  for( WORD i = 0; i < GAME_PLAYER; i++ )
  //  {
  //    lstrcpy( ScoreInfo.szUserName[i],TEXT("SSSSDFSDFSDF") );
  //    ScoreInfo.lGameScore[i] = 123L*((i%2)?-1:1);
  //  }
  //  WeaveInfo.cbWeaveCount = 5;
  //  for (int i = 0; i < WeaveInfo.cbWeaveCount; i++)
  //  {
  //    WeaveInfo.cbCardCount[i] = 3;
  //    WeaveInfo.cbCardData[i][0] = 0x01;
  //    WeaveInfo.cbCardData[i][1] = 0x01;
  //    WeaveInfo.cbCardData[i][2] = 0x01;
  //  }
  //  CChiHuRight chr;
  //  chr |= CHR_ZI_MO;
  //  ScoreInfo.cbHuaCardCount = 3;
  //  ScoreInfo.cbFanCount = 23;
  //  m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
  //  m_GameScore.ShowWindow( SW_SHOW );
  //  SetHuangZhuang(true);
  //  m_cbFengQuan = 0;
  //  m_btStart.ShowWindow( SW_SHOW );

  //  tagSelectCardInfo sci[MAX_COUNT];
  //  ZeroMemory( sci,sizeof(sci) );
  //  sci[0].cbActionCard = 0x03;
  //  sci[0].wActionMask = WIK_LEFT;
  //  sci[0].cbCardCount = 2;
  //  sci[0].cbCardData[0] = 0x04;
  //  sci[0].cbCardData[1] = 0x05;
  //  sci[1].cbActionCard = 0x03;
  //  sci[1].wActionMask = WIK_CENTER;
  //  sci[1].cbCardCount = 2;
  //  sci[1].cbCardData[0] = 0x02;
  //  sci[1].cbCardData[1] = 0x04;
  //  sci[2].cbActionCard = 0x03;
  //  sci[2].wActionMask = WIK_RIGHT;
  //  sci[2].cbCardCount = 2;
  //  sci[2].cbCardData[0] = 0x01;
  //  sci[2].cbCardData[1] = 0x02;
  //  m_HandCardControl.OnEventUserAction( sci,3 );
  //  m_ControlWnd.SetControlInfo( WIK_PENG|WIK_CHI_HU|WIK_LEFT|WIK_LISTEN|WIK_GANG );

  //  //m_DrawSiceWnd.SetSiceInfo( GetDC(),250,MAKEWORD(4,5),MAKEWORD(2,6) );
  //  //m_DrawSiceWnd.ShowWindow( SW_SHOW );
  //}
  //else
  //{
  //  tagMoveCardItem mci;
  //  mci.cbCardCount = 4;
  //  mci.cbCardData = 0;
  //  mci.wViewChairId = 0;
  //  mci.ptFrom = m_HeapCard[0].GetDispatchCardPos( (WORD)6 );
  //  mci.ptTo = CPoint(500,500);
  //  m_HeapCard[0].SetCardEmpty( 6+1,4 );
  //  OnMoveCardItem( mci );
  //}
  //end test

  return;
}

//��׿���
void CGameClientView::OnChip(UINT nCtrlID)
{
  UINT nCurPort = nCtrlID - IDC_SCORENONE;

  //������Ϣ
  SendEngineMessage(IDM_CHIP,nCurPort,0);

  return;
}

//��ʱ��
void CGameClientView::OnTimer(UINT nIDEvent)
{
  //��������
  if(nIDEvent==IDI_BOMB_EFFECT)
  {
    //ֹͣ�ж�
    if(m_bBombEffect==false)
    {
      KillTimer(IDI_BOMB_EFFECT);
      return;
    }

    //���ñ���
    if((m_cbBombFrameIndex+1)>=BOMB_EFFECT_COUNT)
    {
      //ɾ��ʱ��
      KillTimer(IDI_BOMB_EFFECT);

      //���ñ���
      m_bBombEffect=false;
      m_cbBombFrameIndex=0;
    }
    else
    {
      m_cbBombFrameIndex++;
    }

    //���½���
    InvalidGameView(0,0,0,0);

    return;
  }
  else if(nIDEvent==IDI_DISC_EFFECT)
  {
    //���ñ���
    if((m_cbDiscFrameIndex+1)>=DISC_EFFECT_COUNT)
    {
      m_cbDiscFrameIndex=0;
    }
    else
    {
      m_cbDiscFrameIndex++;
    }

    //���½���
    InvalidGameView(0,0,0,0);

    return;

  }
  else if(nIDEvent == IDI_MOVE_CARD)
  {
    //�жϵ�ǰ�����Ƿ����
    if(m_nStepCount == 0)
    {
      //ɾ����ʱ��
      KillTimer(IDI_MOVE_CARD);

      //��FALSE
      m_bCardMoving = false;

      //���Ͷ���������Ϣ
      PostEngineMessage(IDM_MOVE_CARD_FINISH,0,0);

      //������ͼ
      InvalidGameView(0,0,0,0);
    }
    //������û����
    else
    {
      //���ٲ���
      m_nStepCount--;

      //���û滭����
      CSize size(m_MoveCardItem.cbCardCount==4?80:60,m_MoveCardItem.cbCardCount==4?80:60);
      m_rcCardMove = CRect(m_MoveCardItem.ptFrom,size);

      //�ƶ�
      m_MoveCardItem.ptFrom.x += m_nXStep;
      m_MoveCardItem.ptFrom.y += m_nYStep;

      //���ϻ滭����
      size.SetSize(m_MoveCardItem.cbCardCount==4?80:60,m_MoveCardItem.cbCardCount==4?80:60);
      CRect rcDraw(m_MoveCardItem.ptFrom,size);
      m_rcCardMove.UnionRect(&m_rcCardMove,&rcDraw);

      //������ͼ
      InvalidGameView(m_rcCardMove.left,m_rcCardMove.top,m_rcCardMove.Width(),m_rcCardMove.Height());
    }
    return;
  }

  __super::OnTimer(nIDEvent);
}

//���÷�Ȧ
void CGameClientView::SetFengQuan(BYTE cbFengQuan)
{
  if(m_cbFengQuan != cbFengQuan)
  {
    m_cbFengQuan = cbFengQuan;
    InvalidGameView(0,0,0,0);
  }
}

//���ò���
void CGameClientView::SetMagicCard(BYTE bMagicCard)
{
  if(m_bMagicCard != bMagicCard)
  {
    m_bMagicCard = bMagicCard;
    InvalidGameView(0,0,0,0);
  }
}

//�������
void CGameClientView::SetMaiDiState(bool bMaiDi)
{
  if(m_bMaiDiState != bMaiDi)
  {
    m_bMaiDiState = bMaiDi;
    InvalidGameView(0,0,0,0);
  }
}

//��׷���
void CGameClientView::SetMaiDiScore(WORD wViewChairID,BYTE bMaiDi)
{
  //���ñ���
  if(wViewChairID < GAME_PLAYER)
  {
    m_bMaiDi[wViewChairID] = bMaiDi;
  }
  else
  {
    ZeroMemory(m_bMaiDi,sizeof(m_bMaiDi));
  }

  //���½���
  InvalidGameView(0,0,0,0);
}

//������
void CGameClientView::EnableAnimate(bool bAnimate)
{
  m_bEnableAnimate = bAnimate;

  if(!m_bEnableAnimate && m_bCardMoving)
  {
    StopMoveCard();
  }
}

//��������,�����Ͷ���������Ϣ
bool CGameClientView::StopMoveCard()
{
  //�ж�
  if(!m_bCardMoving)
  {
    return false;
  }

  KillTimer(IDI_MOVE_CARD);

  m_bCardMoving = false;

  //������Ϣ
  SendEngineMessage(IDM_MOVE_CARD_FINISH,0,0);

  InvalidGameView(0,0,0,0);

  return true;
}

//�˿˶���
void CGameClientView::OnMoveCardItem(const tagMoveCardItem &MoveCardItem)
{
  if(m_bCardMoving)
  {
    return;
  }

  //�Ƿ�������
  if(!m_bEnableAnimate)
  {
    //������Ϣ
    SendEngineMessage(IDM_MOVE_CARD_FINISH,0,0);
    return;
  }

  m_bCardMoving = true;
  //���ñ���
  m_MoveCardItem = MoveCardItem;
  m_nStepCount = MOVE_STEP_COUNT;
  m_nXStep = (MoveCardItem.ptTo.x-MoveCardItem.ptFrom.x)/m_nStepCount;
  m_nYStep = (MoveCardItem.ptTo.y-MoveCardItem.ptFrom.y)/m_nStepCount;
  switch(MoveCardItem.wViewChairId)
  {
    case 0:
      m_enMoveDirection = Direction_North;
      break;
    case 2:
      m_enMoveDirection = Direction_West;
      break;
    case 1:
      m_enMoveDirection = Direction_South;
      break;
    case 3:
      m_enMoveDirection = Direction_East;
  }

  //���û滭����
  CSize size(MoveCardItem.cbCardCount==4?80:50,MoveCardItem.cbCardCount==4?80:50);
  CRect rcDraw(MoveCardItem.ptFrom,size);
  m_rcCardMove = rcDraw;

  //���ö�ʱ��
  SetTimer(IDI_MOVE_CARD,TIME_MOVE_CARD,NULL);
  InvalidGameView(m_rcCardMove.left,m_rcCardMove.top,m_rcCardMove.Width(),m_rcCardMove.Height());
}

//���û���
void CGameClientView::SetHuaCard(WORD wChair,BYTE bHuaCard, bool bClear)
{
  //�Ƿ�ȫ�����
  if(bClear)
  {
    ZeroMemory(m_cbHuaCardCount, sizeof(m_cbHuaCardCount));
    ZeroMemory(m_cbHuaCardData, sizeof(m_cbHuaCardData));
  }
  else
  {
    if(m_cbHuaCardCount[wChair] < 8)
    {
      m_cbHuaCardData[wChair][m_cbHuaCardCount[wChair]] = bHuaCard;
      m_cbHuaCardCount[wChair]++;
    }
  }
  InvalidGameView(0,0,0,0);
}

//����β��
CPoint CGameClientView::GetHuaCardTailPos(WORD wChair)
{
  CPoint ptTail;
  switch(wChair)
  {
    case 0:
    {
      ptTail.SetPoint(m_PointHuaCard[0].x+(7-m_cbHuaCardCount[0])*24, m_PointHuaCard[0].y-3);
      break;
    }
    case 1:
    {
      ptTail.SetPoint(m_PointHuaCard[2].x+m_cbHuaCardCount[2]*24, m_PointHuaCard[2].y-3);
      break;
    }
  }

  return ptTail;
}

void CGameClientView::SetShowHeapCard(bool bshow)
{
  m_bShowHeapCard = bshow;
  InvalidGameView(0,0,0,0);
}
//�滭ʱ��
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
  //��ȡ����
  const INT nNumberHeight=ImageTimeNumber.GetHeight();
  const INT nNumberWidth=ImageTimeNumber.GetWidth()/11;

  //������Ŀ
  LONG lNumberCount=0;
  WORD wNumberTemp=wTime;
  do
  {
    lNumberCount++;
    wNumberTemp/=10;
  }
  while(wNumberTemp>0L);

  //λ�ö���
  INT nYDrawPos=nYPos-nNumberHeight/2+1;
  INT nXDrawPos=nXPos/*+(lNumberCount*nNumberWidth)/2-nNumberWidth*/;

  //�滭����
  CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
  ImageTimeBack.TransDrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2,RGB(255,0,255));

  //�滭����
  for(int i=0; i<lNumberCount; i++)
  {
    //�滭����
    WORD wCellNumber=wTime%10;
    ImageTimeNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0,RGB(255,0,255));

    //���ñ���
    wTime/=10;
    nXDrawPos-=nNumberWidth;
  };

  return;
}

//�滭׼��
void CGameClientView::DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos)
{
  //�滭׼��
  CSize SizeImage(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());
  ImageUserReady.TransDrawImage(pDC,nXPos-SizeImage.cx/2,nYPos-SizeImage.cy/2,RGB(255,0,255));
  return;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
