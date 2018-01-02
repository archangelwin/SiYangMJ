#include "StdAfx.h"
#include "GameLogic.h"
#include "GameClient.h"
#include "ControlWnd.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CHI_LEFT        100                 //���ư�ť
#define IDC_CHI_CENTER        101                 //���ư�ť
#define IDC_CHI_RIGHT       102                 //���ư�ť
#define IDC_PENG          103                 //���ư�ť
#define IDC_GANG          104                 //���ư�ť
#define IDC_CHI_HU          105                 //���ư�ť
#define IDC_GIVEUP          106                 //���ư�ť

#define IDC_HEAR          107                 //���ư�ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
  ON_WM_PAINT()
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_PENG, OnBnClickedPeng)
  ON_BN_CLICKED(IDC_GANG, OnBnClickedGang)
  ON_BN_CLICKED(IDC_HEAR, OnBnClickedHear)
  ON_BN_CLICKED(IDC_CHI_HU, OnBnClickedChiHu)
  ON_BN_CLICKED(IDC_GIVEUP, OnBnClickedGiveUp)
  ON_BN_CLICKED(IDC_CHI_LEFT, OnBnClickedChiLeft)
  ON_BN_CLICKED(IDC_CHI_CENTER, OnBnClickedChiCenter)
  ON_BN_CLICKED(IDC_CHI_RIGHT, OnBnClickedChiRight)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlWnd::CControlWnd()
{
  //������Դ
  HINSTANCE hResInstance=AfxGetInstanceHandle();
  m_ImageControlBack.LoadFromResource(hResInstance,IDB_CONTROL_BACK);

  return;
}

//��������
CControlWnd::~CControlWnd()
{
}

//����״̬
VOID CControlWnd::SetControlInfo(BYTE cbAcitonMask)
{
  //���ư�ť
  m_btPeng.EnableWindow(((cbAcitonMask&WIK_PENG)!=0)?TRUE:FALSE);
  m_btGang.EnableWindow(((cbAcitonMask&WIK_GANG)!=0)?TRUE:FALSE);
  m_btChiHu.EnableWindow(((cbAcitonMask&WIK_CHI_HU)!=0)?TRUE:FALSE);
  m_btChiLeft.EnableWindow((cbAcitonMask&WIK_LEFT)?TRUE:FALSE);
  m_btChiCenter.EnableWindow((cbAcitonMask&WIK_CENTER)?TRUE:FALSE);
  m_btChiRight.EnableWindow((cbAcitonMask&WIK_RIGHT)?TRUE:FALSE);

  //���ƴ���
  ShowWindow((cbAcitonMask!=WIK_NULL)?SW_SHOW:SW_HIDE);

  return;
}

//�����ؼ�
VOID CControlWnd::RectifyControl(INT nWidth, INT nHeight)
{
  //��ȡλ��
  CRect rcButton;

  //��������
  CSkinButton * ControlButton[]= {&m_btChiLeft,&m_btChiCenter,&m_btChiRight,&m_btPeng,&m_btGang,&m_btChiHu};

  //���ư�ť
  INT nXPos=24;
  for(INT i=0; i<CountArray(ControlButton); i++)
  {
    ControlButton[i]->SetWindowPos(NULL,nXPos,39,0,0,SWP_NOZORDER|SWP_NOSIZE);
    ControlButton[i]->GetWindowRect(&rcButton);
    nXPos += rcButton.Width()+1;
  }

  //������ť
  m_btGiveUp.GetWindowRect(&rcButton);
  m_btGiveUp.SetWindowPos(NULL,nWidth-rcButton.Width()-12,43,0,0,SWP_NOZORDER|SWP_NOSIZE);

  return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedPeng()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_PENG,0);
  }

  return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedGang()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_GANG,0);
  }

  return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedHear()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_LISTEN,0);
  }

  return;
}

//�Ժ���ť
VOID CControlWnd::OnBnClickedChiHu()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_CHI_HU,0);
  }

  return;
}

//������ť
VOID CControlWnd::OnBnClickedGiveUp()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_NULL,0);
  }

  return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedChiLeft()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_LEFT,0);
  }

  return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedChiCenter()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_CENTER,0);
  }

  return;
}

//���ư�ť
VOID CControlWnd::OnBnClickedChiRight()
{
  //������Ϣ
  CGameFrameView *pWnd=CGameFrameView::GetInstance();
  if(pWnd)
  {
    pWnd->PostEngineMessage(IDM_USER_ACTION,WIK_RIGHT,0);
  }

  return;
}

//�ػ�����
VOID CControlWnd::OnPaint()
{
  CPaintDC dc(this);

  //�滭����
  m_ImageControlBack.TransDrawImage(&dc,0,0,RGB(255,0,255));

  return;
}

//������Ϣ
INT CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }

  //������ť
  CRect rcCreate(0,0,0,0);
  m_btGiveUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_GIVEUP);
  m_btPeng.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_PENG);
  m_btGang.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_GANG);
  m_btChiHu.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI_HU);

  m_btChiLeft.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI_LEFT);
  m_btChiCenter.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI_CENTER);
  m_btChiRight.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CHI_RIGHT);

  //����λͼ
  HINSTANCE hResInstance=AfxGetInstanceHandle();
  m_btPeng.SetButtonImage(IDB_BT_PENG,hResInstance,false,false);
  m_btGang.SetButtonImage(IDB_BT_GANG,hResInstance,false,false);

  m_btChiHu.SetButtonImage(IDB_BT_CHIHU,hResInstance,false,false);
  m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,hResInstance,false,false);
  m_btChiLeft.SetButtonImage(IDB_BT_CHI_LEFT,hResInstance,false,false);
  m_btChiCenter.SetButtonImage(IDB_BT_CHI_CENTER,hResInstance,false,false);
  m_btChiRight.SetButtonImage(IDB_BT_CHI_RIGHT,hResInstance,false,false);

  //��������
  CRgn WindowRgn;
  m_ImageControlBack.CreateImageRegion(WindowRgn,RGB(255,0,255));

  //��������
  SetWindowRgn(WindowRgn,FALSE);
  SetWindowPos(NULL,0,0,m_ImageControlBack.GetWidth(),m_ImageControlBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

  //�����ؼ�
  CRect rcClient;
  GetClientRect(&rcClient);
  RectifyControl(rcClient.Width(),rcClient.Height());

  return 0;
}

//////////////////////////////////////////////////////////////////////////
