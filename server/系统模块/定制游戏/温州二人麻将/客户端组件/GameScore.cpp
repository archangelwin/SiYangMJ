#include "Stdafx.h"
#include "GameScore.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CLOSE_SCORE       100                 //�رճɼ�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScore, CDialog)
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScore::CGameScore() : CDialog(IDD_GAME_SCORE)
{
  //��ʼ������
  ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));
  m_cbWeaveCount = 0;
  m_ChiHuRight.SetEmpty();

  HINSTANCE hInst = AfxGetInstanceHandle();
  m_ImageBack.LoadFromResource(hInst ,IDB_GAME_SCORE);
  m_ImageFangPao.LoadImage(hInst,TEXT("FANG_PAO"));
  m_ImageZiMo.LoadImage(hInst,TEXT("ZI_MO"));
  m_ImageGou.LoadImage(hInst,TEXT("GOU"));
}

//��������
CGameScore::~CGameScore()
{
}

//�ؼ���
void CGameScore::DoDataExchange(CDataExchange * pDX)
{
  __super::DoDataExchange(pDX);
  DDX_Control(pDX, IDOK, m_btCloseScore);
}

//��ʼ������
BOOL CGameScore::OnInitDialog()
{
  __super::OnInitDialog();

  //ɾ�����ڱ���ͻ�ȡ����
  ModifyStyle(WS_CAPTION,0,0);
  SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

  //��������
  CRgn RgnControl;
  m_ImageBack.CreateImageRegion(RgnControl,RGB(255,0,255));

  //���ô���
  if(RgnControl.GetSafeHandle()!=NULL)
  {
    //�ƶ�����
    SetWindowRgn(RgnControl,TRUE);
    SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);
  }

  m_btCloseScore.SetButtonImage(IDB_BT_SCORE_CLOSE,AfxGetInstanceHandle(),false,false);

  //������ť
  CRect rcClient;
  GetClientRect(&rcClient);
  CRect rcBtClose;
  m_btCloseScore.GetWindowRect(&rcBtClose);
  m_btCloseScore.SetWindowPos(NULL,(rcClient.Width()-rcBtClose.Width())/2,rcClient.Height()-rcBtClose.Height()-10,0,0,SWP_NOSIZE|SWP_NOZORDER);

  //��������
  //LOGFONT lg;
  //ZeroMemory( &lg,sizeof(lg) );
  //lg.lfCharSet = GB2312_CHARSET;
  //strcpy( lg.lfFaceName,"����" );
  //lg.lfHeight = -20;
  //if( !m_InfoFont.CreateFontIndirect( &lg ) )
  //  m_InfoFont.Attach(CSkinResourceManager::GetDefaultFont());

  //����͸��
  ModifyStyleEx(0,0x00080000);
  HINSTANCE hInst = LoadLibrary(TEXT("User32.DLL"));
  if(hInst)
  {
    typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);
    MYFUNC fun = NULL;
    //ȡ��SetLayeredWindowAttributes��������ָ��
    fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
    if(fun)
    {
      fun(this->GetSafeHwnd(),0,220,2);
    }
    FreeLibrary(hInst);
  }

  return TRUE;
}

//��λ����
void CGameScore::RestorationData()
{
  //���ñ���
  m_cbWeaveCount=0;
  ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

  //���ش���
  if(m_hWnd!=NULL)
  {
    ShowWindow(SW_HIDE);
  }

  return;
}

//���û���
void CGameScore::SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo, const CChiHuRight &ChiHuRight)
{
  //���ñ���
  m_ScoreInfo=ScoreInfo;
  m_cbWeaveCount=WeaveInfo.cbWeaveCount;
  m_ChiHuRight = ChiHuRight;

  //��ϱ���
  for(BYTE i=0; i<m_cbWeaveCount; i++)
  {
    bool bPublicWeave=(WeaveInfo.cbPublicWeave[i]==TRUE);
    m_WeaveCard[i].SetCardData(WeaveInfo.cbCardData[i],WeaveInfo.cbCardCount[i]);
    m_WeaveCard[i].SetDisplayItem(true);
  }

  //��ʾ����
  ShowWindow(SW_SHOW);

  return;
}

//�رհ�ť
void CGameScore::OnOK()
{
  //���ش���
  RestorationData();

  __super::OnOK();
}

//�ػ�����
void CGameScore::OnPaint()
{
  CPaintDC dc(this);

  //��ȡλ��
  CRect rcClient;
  GetClientRect(&rcClient);

  //��������
  CDC DCBuffer;
  CBitmap ImageBuffer;
  DCBuffer.CreateCompatibleDC(&dc);
  ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

  //���� DC
  DCBuffer.SetBkMode(TRANSPARENT);
  DCBuffer.SelectObject(&ImageBuffer);
  //DCBuffer.SelectObject(m_InfoFont);
  DCBuffer.SetTextColor(RGB(250,250,58));

  //�滭����
  if(m_ImageBack.IsNull()==false)
  {
    m_ImageBack.BitBlt(DCBuffer,0,0);
  }

  //λ�ñ���
  int nCardSpace=2;
  int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
  int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

  //����λ��
  int nYCardPos=40;
  int nXCardPos=(rcClient.Width()-nTotalWidth)/2;

  //�滭���
  for(BYTE i=0; i<m_cbWeaveCount; i++)
  {
    //�滭�˿�
    m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos,true);

    //����λ��
    nXCardPos+=(nCardSpace+nItemWidth*3);
  }

  //�滭�˿�
  for(BYTE i=0; i<m_ScoreInfo.cbCardCount; i++)
  {
    //�滭�˿�
    g_CardResource.m_ImageTableBottom.DrawCardItem(&DCBuffer,m_ScoreInfo.cbCardData[i],nXCardPos,nYCardPos);

    //����λ��
    nXCardPos+=nItemWidth;
    if((i+2)==m_ScoreInfo.cbCardCount)
    {
      nXCardPos+=nCardSpace;
    }
  }

  TCHAR szBuffer[32] = TEXT("");
  //������Ϣ
  if(m_ScoreInfo.wProvideUser != INVALID_CHAIR)
  {
    CRect rcDraw;
    CString strTotal;
    strTotal.Format(TEXT("��ң�%s ���ˣ�"), m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]);
    rcDraw.SetRect(100,8,370,27);
    DCBuffer.DrawText(strTotal,&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM);
    CRect rcChiHuRight;
    rcChiHuRight.SetRect(131,97,175,119);
    DCBuffer.SetTextColor(RGB(0,0,0));
    if(!(m_ChiHuRight&CHR_RUAN_HU).IsEmpty())
    {
      DCBuffer.DrawText(TEXT("���"),&rcChiHuRight,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER);
    }
    if(!(m_ChiHuRight&CHR_YING_HU).IsEmpty())
    {
      DCBuffer.DrawText(TEXT("���"),&rcChiHuRight,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER);
    }
    CRect rcFanXing(355,97,410,119);
    BYTE pcbFanCount[] = {1,1,1,1,1, 1,1,1,1};
    //������Ϣ
    LPCTSTR pszKind[] =
    {
      TEXT("������"),TEXT("�˶�"),TEXT("�غ�"),TEXT("���"),TEXT("����"),
      TEXT("����"),TEXT("˫��"),TEXT("��һɫ"),TEXT("Ӳ�˶�")
    };
    //����Ȩλ
    DWORD pdwRight[] = {CHR_PENG_PENG_HU,CHR_BA_DUI,CHR_DI_HU,CHR_TIAN_HU,CHR_DAN_DIAO,
                        CHR_THREE_KING,CHR_SHUANG_FAN,CHR_QING_YI_SE,CHR_BA_DUI_YING
                       };
    for(BYTE i = 0; i < CountArray(pcbFanCount); i++)
    {
      if(!(m_ChiHuRight&pdwRight[i]).IsEmpty())
      {
        DCBuffer.DrawText(pszKind[i],lstrlen(pszKind[i]),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
      }
    }
    //����
    //if( m_ScoreInfo.wProvideUser != m_ScoreInfo.wChiHuUser )
    //{
    //  rcDraw.SetRect(15,148,190,175);
    //  DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
    //    &rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
    //  m_ImageFangPao.DrawImage( &DCBuffer,205,150 );
    //  rcDraw.SetRect(295,148,482,175);
    //  if (m_ScoreInfo.bMulWinner)
    //  {
    //    CString strName,strTotal;
    //    for (int i = 0; i < GAME_PLAYER; i++)
    //    {
    //      if (m_ScoreInfo.dwChiHuKind[i] == WIK_CHI_HU)
    //      {
    //        if (strTotal.GetLength() == 0)
    //          strTotal.Format(TEXT("%s"), m_ScoreInfo.szUserName[i]);
    //        else
    //        {
    //          strName.Format(TEXT("��%s"), m_ScoreInfo.szUserName[i]);
    //          strTotal += strName;
    //        }
    //      }
    //    }
    //    DCBuffer.DrawText(strTotal,&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
    //  }
    //  else
    //    DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]),
    //      &rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
    //}
    ////����
    //else
    //{
    //  rcDraw.SetRect(15,148,190,175);
    //  DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
    //    &rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
    //  m_ImageZiMo.DrawImage( &DCBuffer,215,150 );
    //}
  }

  //�÷���Ϣ
  DCBuffer.SetTextColor(RGB(0,0,0));
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    CRect rcDrawPlayer(134+i*137,123,272+i*137,147);
    //���
    DCBuffer.DrawText(m_ScoreInfo.szUserName[i],lstrlen(m_ScoreInfo.szUserName[i]),
                      &rcDrawPlayer,DT_SINGLELINE|DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER);
    if(m_ScoreInfo.bMaidi[i])
    {
      if(i==m_ScoreInfo.wBankerUser)
      {
        m_ImageGou.DrawImage(&DCBuffer,188+i*137,156);
      }
      else
      {
        m_ImageGou.DrawImage(&DCBuffer,188+i*137,182);
      }
    }
    if(i!=1)
    {
      continue;
    }

    //����
    CRect rcDrawScore(165,206,400,240);
    _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),m_ScoreInfo.lGameScore[i]);
    DCBuffer.DrawText(szBuffer,lstrlen(szBuffer),&rcDrawScore,DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS);
  }

  //̨��
  //BYTE pcbFanCount[] = {0,1,1,1,1,0, 1,1,1,1,0,1
  //};
  //
  ////��������
  //CRect rcFanXing(34,301,159,327);
  ////��������
  //CRect rcFanShu(160,301,190,327);
  ////������
  //CRect rcFan(195,301,215,327);
  //BYTE cbDrawCount = 0;
  //for( BYTE i = 0; i < CountArray(pcbFanCount); i++ )
  //{
  //  if( !(m_ChiHuRight&pdwRight[i]).IsEmpty() && pcbFanCount[i] != 0)
  //  {
  //    DCBuffer.DrawText( pszKind[i],lstrlen(pszKind[i]),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
  //    _sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),pcbFanCount[i] );
  //    //DCBuffer.SetTextColor( RGB(252,255,0) );
  //    //DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
  //    //DCBuffer.SetTextColor( RGB(250,250,250) );
  //    //DCBuffer.DrawText( CString(TEXT("̨")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );

  //    //�ƶ��滭����
  //    if( ++cbDrawCount%2 == 0 )
  //    {
  //      rcFanXing.OffsetRect( -245,26 );
  //      rcFanShu.OffsetRect( -245,26 );
  //      rcFan.OffsetRect( -245,26 );
  //    }
  //    else
  //    {
  //      rcFanXing.OffsetRect( 245,0 );
  //      rcFanShu.OffsetRect( 245,0 );
  //      rcFan.OffsetRect( 245,0 );
  //    }
  //  }
  //}
  //if( m_ScoreInfo.cbHuaCardCount > 0 )
  //{
  //  DCBuffer.DrawText( TEXT("��"),lstrlen(TEXT("��")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
  //  _sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbHuaCardCount );
  //  DCBuffer.SetTextColor( RGB(252,255,0) );
  //  DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
  //  DCBuffer.SetTextColor( RGB(250,250,250) );
  //  DCBuffer.DrawText( CString(TEXT("̨")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
  //  //�ƶ��滭����
  //  if( ++cbDrawCount%2 == 0 )
  //  {
  //    rcFanXing.OffsetRect( -245,26 );
  //    rcFanShu.OffsetRect( -245,26 );
  //    rcFan.OffsetRect( -245,26 );
  //  }
  //  else
  //  {
  //    rcFanXing.OffsetRect( 245,0 );
  //    rcFanShu.OffsetRect( 245,0 );
  //    rcFan.OffsetRect( 245,0 );
  //  }
  //}
  //�ܼ�
  /*if( m_ScoreInfo.cbFanCount > 0 )
  {
    DCBuffer.DrawText( TEXT("�ܼ�"),lstrlen(TEXT("�ܼ�")),rcFanXing,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
    _sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),m_ScoreInfo.cbFanCount );
    DCBuffer.SetTextColor( RGB(252,255,0) );
    DCBuffer.DrawText( szBuffer,lstrlen(szBuffer),rcFanShu,DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
    DCBuffer.SetTextColor( RGB(250,250,250) );
    DCBuffer.DrawText( CString(TEXT("̨")),rcFan,DT_SINGLELINE|DT_LEFT|DT_BOTTOM );
  }*/

  //�滭����
  dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

  //������Դ
  DCBuffer.DeleteDC();
  ImageBuffer.DeleteObject();

  return;
}

//�����Ϣ
void CGameScore::OnLButtonDown(UINT nFlags, CPoint Point)
{
  __super::OnLButtonDown(nFlags,Point);

  //��Ϣģ��
  PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

  return;
}

//��������
BOOL CGameScore::OnEraseBkgnd(CDC * pDC)
{
  Invalidate(FALSE);
  UpdateWindow();
  return TRUE;
}

//////////////////////////////////////////////////////////////////////////
