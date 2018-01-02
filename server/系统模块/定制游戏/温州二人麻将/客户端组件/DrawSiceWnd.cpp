#include "StdAfx.h"
#include "DrawSiceWnd.h"

//////////////////////////////////////////////////////////////////////////
#define IDI_SICE_POS        203                 //�ƶ���ʱ��
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDrawSiceWnd, CWnd)
  ON_WM_PAINT()
  ON_WM_CREATE()
  ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDrawSiceWnd::CDrawSiceWnd()
{
  //������Դ
  m_bSicePos=0;
  m_nXPos=0;
  m_nYPos=0;
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_ImageBack.LoadFromResource(hInstance,IDB_SICE_BACK);

  return;
}

//��������
CDrawSiceWnd::~CDrawSiceWnd()
{
}

//�ػ�����
void CDrawSiceWnd::OnPaint()
{
  CPaintDC dc(this);

  //��ȡλ��
  CRect rcClient;
  GetClientRect(&rcClient);

  //��������
  CDC BufferDC;
  CBitmap BufferImage;
  BufferDC.CreateCompatibleDC(&dc);
  BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
  BufferDC.SelectObject(&BufferImage);
  //�滭����

  m_ImageBack.BitBlt(BufferDC,0,0);

  if(m_bSicePos<9)
  {
    m_ImageDrawSiceFirst.TransDrawImage(&BufferDC,30,0,m_ImageDrawSiceFirst.GetWidth()/9,m_ImageDrawSiceFirst.GetHeight(),
                                        m_bSicePos*m_ImageDrawSiceFirst.GetWidth()/9,0,RGB(0,0,255));
    m_ImageDrawSiceSecond.TransDrawImage(&BufferDC,30,0,m_ImageDrawSiceSecond.GetWidth()/9,m_ImageDrawSiceSecond.GetHeight(),
                                         m_bSicePos*m_ImageDrawSiceSecond.GetWidth()/9,0,RGB(0,0,255));
  }
  else
  {

    m_ImageDrawSiceUserFirst.TransDrawImage(&BufferDC,30,0,m_ImageDrawSiceUserFirst.GetWidth()/9,m_ImageDrawSiceUserFirst.GetHeight(),
                                            m_bSicePos%9*m_ImageDrawSiceUserFirst.GetWidth()/9,0,RGB(0,0,255));
    m_ImageDrawSiceUserSecond.TransDrawImage(&BufferDC,30,0,m_ImageDrawSiceUserSecond.GetWidth()/9,m_ImageDrawSiceUserSecond.GetHeight(),
        m_bSicePos%9*m_ImageDrawSiceUserSecond.GetWidth()/9,0,RGB(0,0,255));
  }
  //�滭����

  dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

  //������Դ
  BufferDC.DeleteDC();
  BufferImage.DeleteObject();

  return;
}

//������Ϣ
int CDrawSiceWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }
  SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOREPOSITION);
  return 0;
}
//��ʱ����Ϣ
void CDrawSiceWnd::OnTimer(UINT nIDEvent)
{
  if(IDI_SICE_POS==nIDEvent)
  {

    if(m_bSicePos>=17)
    {
      KillTimer(IDI_SICE_POS);
      Sleep(1000);
      CGameFrameView *pWnd=CGameFrameView::GetInstance();
      if(pWnd)
      {
        pWnd->PostEngineMessage(IDM_SICE_FINISH,NULL,NULL);
      }
    }
    else
    {
      if(m_bSicePos==8)
      {
        Sleep(1000);
        CGameFrameView *pWnd=CGameFrameView::GetInstance();
        if(pWnd)
        {
          pWnd->SendEngineMessage(IDM_SICE_TWO,NULL,NULL);
        }
      }
      m_bSicePos++;
    }
    Invalidate();
  }

  __super::OnTimer(nIDEvent);
}

//ֹͣ����
void CDrawSiceWnd::StopSicing(bool bFinish)
{
  if(IsWindowVisible())
  {
    KillTimer(IDI_SICE_POS);

    if(m_bSicePos <= 8 && bFinish)
    {
      CGameFrameView *pWnd=CGameFrameView::GetInstance();
      if(pWnd)
      {
        pWnd->SendEngineMessage(IDM_SICE_TWO,0,0);
      }
    }

    if(bFinish)
    {
      CGameFrameView *pWnd=CGameFrameView::GetInstance();
      if(pWnd)
      {
        pWnd->SendEngineMessage(IDM_SICE_FINISH,NULL,NULL);
      }
    }

    if(!bFinish)
    {
      ShowWindow(SW_HIDE);
    }
  }

  return;
}

//����ɫ����Ϣ
void CDrawSiceWnd::SetSiceInfo(CDC *dc,BYTE bMoveTime,WORD wSice1,WORD wSice2)
{
  HINSTANCE hInstance=AfxGetInstanceHandle();
  ParentDC=dc;

  BYTE SiceFirst,SiceSecond;
  if(wSice1 > 0)
  {
    if(&m_ImageDrawSiceFirst!=NULL)
    {
      m_ImageDrawSiceFirst.Destroy();
    }
    if(&m_ImageDrawSiceSecond!=NULL)
    {
      m_ImageDrawSiceSecond.Destroy();
    }

    SiceFirst = HIBYTE(wSice1);
    SiceSecond = LOBYTE(wSice1);
    ASSERT(SiceFirst > 0 && SiceFirst < 7 && SiceSecond > 0 && SiceSecond < 7);
    m_ImageDrawSiceFirst.LoadFromResource(hInstance,IDB_DRAW_SICE1+SiceFirst-1);
    m_ImageDrawSiceSecond.LoadFromResource(hInstance,IDB_DRAW_SICE7+SiceSecond-1);
  }
  if(wSice2 > 0)
  {
    if(&m_ImageDrawSiceUserFirst!=NULL)
    {
      m_ImageDrawSiceUserFirst.Destroy();
    }
    if(&m_ImageDrawSiceUserSecond!=NULL)
    {
      m_ImageDrawSiceUserSecond.Destroy();
    }

    SiceFirst = HIBYTE(wSice2);
    SiceSecond = LOBYTE(wSice2);
    ASSERT(SiceFirst > 0 && SiceFirst < 7 && SiceSecond > 0 && SiceSecond < 7);
    m_ImageDrawSiceUserFirst.LoadFromResource(hInstance,IDB_DRAW_SICE1+SiceFirst-1);
    m_ImageDrawSiceUserSecond.LoadFromResource(hInstance,IDB_DRAW_SICE7+SiceSecond-1);
  }
  m_bSicePos = (wSice1>0)?0:9;
  SetTimer(IDI_SICE_POS,bMoveTime,NULL);
}

//��׼λ��
void CDrawSiceWnd::SetBenchmarkPos(int nXPos, int nYPos)
{
  m_nXPos=nXPos;
  m_nYPos=nYPos;
}



//////////////////////////////////////////////////////////////////////////
