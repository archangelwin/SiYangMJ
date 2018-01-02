#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"
#include "../../third/dump/ExceptionReport.h"
//////////////////////////////////////////////////////////////////////////////////

//�������
CGameServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerApp::CGameServerApp()
{
}

//��������
BOOL CGameServerApp::InitInstance()
{
	CExceptionReport dmp;
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CGameServerDlg GameServerDlg;
	m_pMainWnd=&GameServerDlg;
	GameServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
