// DlgResponseLeave.cpp : implementation file
//

#include "stdafx.h"
#include "DlgListen.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgListen, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////


//���캯��
CDlgListen::CDlgListen(CWnd * pParentWnd):CSkinDialog(IDD_RESPONSE_LISTEN)
{
  m_strCaption = TEXT("�������");

  m_bZhuaPao = true;
  m_bZiMo = true;
}

//��������
CDlgListen::~CDlgListen()
{
}

//�ؼ���
void CDlgListen::DoDataExchange(CDataExchange* pDX)
{
  __super::DoDataExchange(pDX);
  DDX_Control(pDX, IDOK, m_btOk);
  DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CDlgListen::OnInitDialog()
{
  __super::OnInitDialog();

  //���ñ���
  SetWindowText(m_strCaption);

  ((CButton *)GetDlgItem(IDC_ZHUA_PAO))->SetCheck(BST_CHECKED);
  ((CButton *)GetDlgItem(IDC_ZI_MO))->SetCheck(BST_CHECKED);

  return FALSE;
}

//ȷ������
VOID CDlgListen::OnOK()
{
  m_bZhuaPao = (((CButton *)GetDlgItem(IDC_ZHUA_PAO))->GetCheck()==BST_CHECKED);
  m_bZiMo = (((CButton *)GetDlgItem(IDC_ZI_MO))->GetCheck()==BST_CHECKED);

  __super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
