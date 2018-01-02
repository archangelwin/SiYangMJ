#include "stdafx.h"
#include "Resource.h"
#include "GlobalFuncUnits.h"
#include "UpdateConfig.h"
#include "UpdateConfigDlg.h"

//////////////////////////////////////////////////////////////////////////////////////////
//��ʶ����
#define IDC_LIST_VIEW								100

//��������
#define UPDATE_CONFIG_NAME							TEXT("Update.ini")

//��Ϣ����
#define WM_UPDATE_PROGRESS							(WM_USER+100)

//��������
#define PROGRESS_CMD_SETUP							0
#define PROGRESS_CMD_UPDATE							1	

//////////////////////////////////////////////////////////////////////////////////////////
//��Ϣӳ��
BEGIN_MESSAGE_MAP(CUpdateConfigDlg, CDialog)

	//ϵͳ��Ϣ
	ON_WM_ERASEBKGND()

	ON_MESSAGE(WM_UPDATE_PROGRESS,OnUpdateProgress)

	//�ؼ���Ϣ
	ON_BN_CLICKED(IDC_RADIO_ALL, OnBnClickedRadioAll)
	ON_BN_CLICKED(IDC_RADIO_PLATFORM, OnBnClickedRadioPlatform)
	ON_BN_CLICKED(IDC_RADIO_MULTI_GAME, OnBnClickedRadioGames)
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnBnClickedButtonDone)
	ON_BN_CLICKED(IDC_BROWSE_INPUTPATH, OnBnClickedBrowseInputpath)
	ON_BN_CLICKED(IDC_BROWSE_OUTPUTPATH, OnBnClickedBrowseOutputpath)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CUpdateConfigDlg::CUpdateConfigDlg(CWnd* pParent) : CDialog(IDD_DIALOG_UPDATEMAIN, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//���ñ���
	m_szGameExpandName[0]=0;
	m_szPlazaConfigName[0]=0;	
}

//���ݽ���
void CUpdateConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_COMBO_MULTI_GAME,m_ComboGameList);
}

//������Ϣ
BOOL CUpdateConfigDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	DWORD dwCommandID = HIWORD(wParam);
	if(m_ComboGameList.m_hWnd == (HWND)lParam)
	{
		switch(dwCommandID)	
		{
		case CBN_SELCHANGE:
			{
				//��ȡ�ؼ�
				CButton * pRadioButton=static_cast<CButton *>(GetDlgItem(IDC_RADIO_MULTI_GAME));
				if(pRadioButton && pRadioButton->GetCheck()==BST_CHECKED)
				{
					//��ȡ·��
					TCHAR szInputPath[MAX_PATH]=TEXT("");
					GetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath,CountArray(szInputPath));

					//ɸѡ�ļ�
					FilterFiles(szInputPath,FILE_KIND_GAME,m_ComboGameList.GetText());
				}

				return TRUE;
			}
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//��ʼ�Ի���
BOOL CUpdateConfigDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//����ͼ��
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//�����ؼ�
	CRect rcCreate(12,215,560,543);
	m_FileListView.Create(WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LIST_VIEW);

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//��������
	CString szConfigFile;
	szConfigFile.Format(TEXT("%s\\%s"),szWorkDirectory,UPDATE_CONFIG_NAME);

	//��������
	CFileFind filefind;
	if(filefind.FindFile(szConfigFile)==TRUE) LoadConfig(szConfigFile);

	return TRUE;
}

//��������
bool CUpdateConfigDlg::LoadConfig(LPCTSTR pszConfigFile)
{
	//��ȡ����
	CWHIniData IniData;
	IniData.SetIniFilePath(pszConfigFile);

	//���·��
	TCHAR szInputPath[MAX_PATH]=TEXT("");
	IniData.ReadString(TEXT("PATHCONFIG"),TEXT("INPUT_PATH"),TEXT(""),szInputPath,sizeof(szInputPath));

	//����·��
	TCHAR szInputNetAddr[MAX_PATH]=TEXT("");
	IniData.ReadString(TEXT("PATHCONFIG"),TEXT("INPUT_NET_ADDR"),TEXT(""),szInputNetAddr,sizeof(szInputNetAddr));

	//���·��
	TCHAR szOutPutPath[MAX_PATH]=TEXT("");
	IniData.ReadString(TEXT("PATHCONFIG"),TEXT("OUTPUT_PATH"),TEXT(""),szOutPutPath,sizeof(szOutPutPath));

	//ƽ̨����
	IniData.ReadString(TEXT("NAMECONFIG"),TEXT("PLAZA_CONFIG_NAME"),TEXT("Client.txt"),m_szPlazaConfigName,sizeof(m_szPlazaConfigName));

	//��Ϸ��չ��
	IniData.ReadString(TEXT("NAMECONFIG"),TEXT("GAME_EXPAND_NAME"),TEXT(".txt"),m_szGameExpandName,sizeof(m_szGameExpandName));	

	//���ÿؼ�
	SetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath);
	SetDlgItemText(IDC_EDIT_INPUT_NET_ADDR,szInputNetAddr);
	SetDlgItemText(IDC_EDIT_OUTPUT_PATH,szOutPutPath);

	//��ȡ��ť
	CButton * pRadioButton = static_cast<CButton *>(GetDlgItem(IDC_RADIO_ALL));
	if(pRadioButton != NULL) pRadioButton->SetCheck(BST_CHECKED);

	//ɸѡ�ļ�
	FilterFiles(szInputPath,FILE_KIND_GAME|FILE_KIND_PLATFORM,NULL);

	return true;
}

//��������
bool CUpdateConfigDlg::SaveConfig(LPCTSTR pszConfigFile)
{
	//���·��
	TCHAR szInputPath[MAX_PATH]=TEXT("");
	GetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath,CountArray(szInputPath));

	//����·��
    TCHAR szInputNetAddr[MAX_PATH]=TEXT("");
	GetDlgItemText(IDC_EDIT_INPUT_NET_ADDR,szInputNetAddr,CountArray(szInputNetAddr));	

	//���·��
	TCHAR szOutPutPath[MAX_PATH]=TEXT("");
	GetDlgItemText(IDC_EDIT_OUTPUT_PATH,szOutPutPath,CountArray(szOutPutPath));

	//���ļ�
	CFile ConfigFile;
	if (ConfigFile.Open(pszConfigFile,CFile::modeReadWrite|CFile::modeCreate,NULL)==FALSE)
	{
		return false;
	}

	//��������
	TCHAR szResult[10240]=TEXT("");
	TCHAR szFileData[10240]=TEXT("");
	LPCTSTR pszInfo=TEXT("-----------------�������� 6603s ���������ļ�  ��Ȩ�������������Ƽ����޹�˾------------------ \r\n\r\n");

	//UNICODE
#ifdef _UNICODE
	BYTE cbUnicodeHead[]={0xFF,0xFE};
	ConfigFile.Write(cbUnicodeHead,sizeof(cbUnicodeHead));
#endif

	//��ʼ�ֶ�
	ConfigFile.Write(pszInfo,lstrlen(pszInfo)*sizeof(TCHAR));

	//·������
	_sntprintf(szFileData,CountArray(szFileData),TEXT("[PATHCONFIG]\r\n") TEXT("INPUT_PATH=%s\r\n") TEXT("INPUT_NET_ADDR=%s\r\n") TEXT("OUTPUT_PATH=%s\r\n\r\n"), szInputPath, szInputNetAddr, szOutPutPath);
	ConfigFile.Write(szFileData,lstrlen(szFileData)*sizeof(TCHAR));

	//��������
	_sntprintf(szFileData, CountArray(szFileData), TEXT("[NAMECONFIG]\r\n") TEXT("PLAZA_CONFIG_NAME=%s\r\n") TEXT("GAME_EXPAND_NAME=%s\r\n"), m_szPlazaConfigName, m_szGameExpandName);
	ConfigFile.Write(szFileData,lstrlen(szFileData)*sizeof(TCHAR));

	return true;
}

//ͳ���ļ�
void CUpdateConfigDlg::CensusFileCount(LPCTSTR pszFilePath,int & nFileCount)
{
	//��������	
	size_t pathlen = 0;
	TCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;

	//��������
	TCHAR szFilename[MAX_PATH]=TEXT(""); 	

	//�����ַ�
	lstrcpy(szFind,pszFilePath);
	lstrcat(szFind,TEXT("\\*.*"));

	//�����ļ�
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind) return;

	//�����ļ�
	while(TRUE)
	{
		//�����ļ�
		if (FindFileData.cFileName[0] != '.') 
		{
			//��ʽ�ַ�
			_sntprintf(szFilename, sizeof(szFilename),TEXT("%s\\%s"), pszFilePath, FindFileData.cFileName);			

			//�ݹ�Ŀ¼
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CensusFileCount(szFilename, nFileCount);
			}
			else if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0)
			{
				//��������
				FILE *fHandle = NULL;

				//�����ж�
				fHandle=fopen(CT2CA(szFilename),"r");
                if(fHandle==NULL) return;

				//�ۼ���Ŀ
				++nFileCount;
                
				//�ر��ļ�
				fclose(fHandle);
			}
		}

		//������һ��
		if(!FindNextFile(hFind,&FindFileData)) break;
	}

	//��������
	FindClose(hFind);
}

//�����ļ�
void CUpdateConfigDlg::SearchFiles(LPCTSTR pszFilePath,LPCTSTR pszPrefix,CStringArray & StringArray)
{
	//��������	
	size_t pathlen = 0;
	TCHAR szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;

	//��������
	TCHAR szPrefix[MAX_PATH]=TEXT("");
	TCHAR szFilename[MAX_PATH]=TEXT(""); 	

	//�����ַ�
	lstrcpy(szFind,pszFilePath);
	lstrcat(szFind,TEXT("\\*.*"));

	//�����ļ�
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind) return;

	//�����ļ�
	while(TRUE)
	{
		//�����ļ�
		if (FindFileData.cFileName[0] != '.') 
		{
			//��ʽ�ַ�
			_sntprintf(szPrefix, sizeof(szPrefix),TEXT("%s%s"),  pszPrefix, FindFileData.cFileName);
			_sntprintf(szFilename, sizeof(szFilename),TEXT("%s\\%s"), pszFilePath, FindFileData.cFileName);			

			//�ݹ�Ŀ¼
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				lstrcat(szPrefix, TEXT("\\"));
				SearchFiles(szFilename, szPrefix, StringArray);
			}
			else if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0)
			{
				//��������
				FILE *fHandle = NULL;
				TCHAR pszFileName[MAX_PATH]=TEXT(""); 

				//�����ж�
				fHandle=fopen(CT2CA(szFilename),"r");
                if(fHandle==NULL) return;
				
				//��ʽ�ַ�
				_sntprintf(pszFileName, sizeof(pszFileName), TEXT("%s%s"), pszPrefix, FindFileData.cFileName);
		
				//�����б�
				StringArray.Add(pszFileName);
                
				//�ر��ļ�
				fclose(fHandle);
			}
		}

		//������һ��
		if(!FindNextFile(hFind,&FindFileData)) break;
	}

	//��������
	FindClose(hFind);
}

//ɸѡ�ļ�
void CUpdateConfigDlg::FilterFiles(LPCTSTR pszFilePath,BYTE cbFileKind,LPCTSTR pszGameNames)
{
	//��������
	CFileFind FileFind;	
	CString strFilePath;
	CString szDirectory(pszFilePath);	

	//�ļ���Ϣ
	WORD wFileCount=0;
	tagFileInfo FileInfo={0};
	 
	//����б��
	if(szDirectory.Right(1)!= TEXT("\\"))
	{
		szDirectory += TEXT("\\");
	}
	 
	//���ñ���
	szDirectory += TEXT("*.*");

	//ɾ������	
	m_FileListView.DeleteAllItems();	

	try
	{
		//�����ļ�
		BOOL bFind = FileFind.FindFile(szDirectory);
		while(bFind)
		{
			//������һ��
			bFind = FileFind.FindNextFile();		  

			//�ļ�����
			if(FileFind.IsHidden()) continue;

			//Ŀ¼���ļ���
			if(FileFind.IsDirectory())
			{
				//Ŀ¼����
				if(FileFind.IsDots()) continue;

				//���ñ���
				FileInfo.bIsDirectory=TRUE;	
				FileInfo.dwFileSize=0;
				FileInfo.szFileVersion[0]=0;
			}
			else
			{
				//��������
				strFilePath = FileFind.GetFilePath();

				//���ñ���
				FileInfo.bIsDirectory=FALSE;
				FileInfo.dwFileSize=GetFileSize(strFilePath);

				//��ȡ�汾
				GetFileVersion(strFilePath,FileInfo.szFileVersion,CountArray(FileInfo.szFileVersion));
			}

			//�޸�ʱ��
			FileFind.GetLastWriteTime(&FileInfo.ftModifyDate);

			//�ļ�����		
			lstrcpyn(FileInfo.szFileName,FileFind.GetFileName(),CountArray(FileInfo.szFileName));

			//�������
			MarkFileKind(pszFilePath,FileInfo);
			if(FileInfo.cbFileKind == FILE_KIND_GAME && FileInfo.bIsDirectory && pszGameNames == NULL)
			{
				m_ComboGameList.AddString(FileInfo.szFileName);
			}

			if((FileInfo.cbFileKind&cbFileKind)!=0)
			{
				//��ʶ����
				bool bAddItem=true;

				if((cbFileKind&FILE_KIND_GAME) != 0 && pszGameNames != NULL)
				{	
					//��������
					CString strGameNames;
					strGameNames.Format(TEXT(", %s,"),pszGameNames);

					//��������
					CString strFileName(FileInfo.szFileName);
					int nPos = strFileName.Find(TEXT('.'));
					if(nPos != -1) strFileName = strFileName.Left(nPos);

					//��������
					CString strSearchName;
					strSearchName.Format(TEXT(", %s,"),strFileName);

					//�����ִ�
					if(strGameNames.Find(strSearchName) == -1) 
					{
						bAddItem=false;
					}
				}

				//�������
				if(bAddItem == true)
				{
					m_FileListView.AddItem(wFileCount++,&FileInfo);
				}
			}
		}

		//�رվ��
		FileFind.Close();

	}
	catch(...){}

	//����������
	m_FileListView.Sort(0);

	return;
}

//ʹ������
VOID CUpdateConfigDlg::EnableControls(bool bEnabled)
{
	//���ÿؼ�	
	GetDlgItem(IDC_EDIT_INPUT_PATH)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_OUTPUT_PATH)->EnableWindow(bEnabled);
	GetDlgItem(IDC_EDIT_INPUT_NET_ADDR)->EnableWindow(bEnabled);

	//���ÿؼ�
	GetDlgItem(IDC_RADIO_ALL)->EnableWindow(bEnabled);
	GetDlgItem(IDC_RADIO_PLATFORM)->EnableWindow(bEnabled);
	GetDlgItem(IDC_RADIO_MULTI_GAME)->EnableWindow(bEnabled);
	GetDlgItem(IDC_COMBO_MULTI_GAME)->EnableWindow(bEnabled);

	//���ÿؼ�
	GetDlgItem(IDC_BUTTON_DONE)->EnableWindow(bEnabled);
	GetDlgItem(IDC_BROWSE_INPUTPATH)->EnableWindow(bEnabled);
	GetDlgItem(IDC_BROWSE_OUTPUTPATH)->EnableWindow(bEnabled);
}

//�ļ���С
DWORD CUpdateConfigDlg::GetFileSize(LPCTSTR pszFileName)
{
	//��������
	WIN32_FIND_DATA fileData;
    HANDLE hFind;
    DWORD dwFileSize=0;

	//�����ļ�
    hFind = FindFirstFile(pszFileName,&fileData);
    if(hFind != INVALID_HANDLE_VALUE)
	{
        dwFileSize = fileData.nFileSizeLow;
	}
   
	//�رվ��
    FindClose(hFind);

    return dwFileSize;
}

//�ļ�����
BOOL CUpdateConfigDlg::IsFileExist(LPCTSTR pszFileName)
{
	//��������
	CFileFind FileFind;
    return FileFind.FindFile(pszFileName);
}

//Ŀ¼����
BOOL CUpdateConfigDlg::IsDirectoryExist(LPCTSTR pszDirectory)
{
	//��������
	WIN32_FIND_DATA wfd;
    BOOL bResult = FALSE;

	//�����ļ�
    HANDLE hFind = FindFirstFile(pszDirectory, &wfd);
    if ((hFind!=INVALID_HANDLE_VALUE) &&(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
    {
        bResult = TRUE;
    }

	//�رվ��
    FindClose(hFind);

    return bResult;
}

//��Ϸ����
BOOL CUpdateConfigDlg::IsGameProcess(LPCTSTR pszFileName)
{
	//������Դ	
	bool bGameProcess=false;
	HINSTANCE hInstance=AfxLoadLibrary(pszFileName);

	//����ͼ��	
	CBitmap GameLogo;
	AfxSetResourceHandle(hInstance);
	if (GameLogo.LoadBitmap(TEXT("GAME_LOGO"))) bGameProcess=true;
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//�ͷ���Դ
	AfxFreeLibrary(hInstance);

	return bGameProcess;
}

//�������
VOID CUpdateConfigDlg::MarkFileKind(LPCTSTR pszWorkPath,tagFileInfo & FileInfo)
{
	//��������
	FileInfo.cbFileKind=FILE_KIND_PLATFORM;

	//Ŀ¼�ж�
	if(FileInfo.bIsDirectory==TRUE)
	{
		//��������
		CString strFilePath;
		strFilePath.Format(TEXT("%s\\%s.exe"),pszWorkPath,FileInfo.szFileName);

		//����ͬ��Ŀ¼
		if(IsFileExist(strFilePath) == TRUE && IsGameProcess(strFilePath) == TRUE)
		{
			FileInfo.cbFileKind=FILE_KIND_GAME;
		}
	}
	else
	{
		//��������		
		CString strFileName(FileInfo.szFileName);

		//ִ���ļ�
		if(strFileName.Find(TEXT(".exe")) > 0)
		{
			//��������
			CString strFilePath,strFileName1;

			//��ʽ��·��
			strFilePath.Format(TEXT("%s\\%s"),pszWorkPath,strFileName.Left(strFileName.Find(TEXT('.'))));
			strFileName1.Format(TEXT("%s\\%s"),pszWorkPath,strFileName);
			
			//����ͬ��Ŀ¼
			if(IsDirectoryExist(strFilePath) == TRUE && IsGameProcess(strFileName1) == TRUE)
			{
				FileInfo.cbFileKind=FILE_KIND_GAME;
			}
		}
	}
}

//�������
BOOL CUpdateConfigDlg::MakeUpdateConfig(CStringArray & FileNameArray,LPCTSTR pszConfigName)
{
	//��������	
	CString szCheckSum;
	CString szFileCount;
	CString szConfigFile;
	CString szInputPath,szInputNetAddr,szOutputPath;

	//��ȡ·��
	GetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath);
	GetDlgItemText(IDC_EDIT_INPUT_NET_ADDR,szInputNetAddr);
	GetDlgItemText(IDC_EDIT_OUTPUT_PATH,szOutputPath);

	//������֤
	if(szInputPath==TEXT("")) return FALSE;
	if(szInputNetAddr==TEXT("")) return FALSE;
	if(szOutputPath==TEXT("")) return FALSE;

	//��ʽ�ַ�
	szConfigFile.Format(TEXT("%s/%s"),szOutputPath,pszConfigName);

	//�����С
	WORD wFileCount=(WORD)FileNameArray.GetCount();
	DWORD dwDataSize=sizeof(tagUpdateInfo)+sizeof(tagUpdateItem)*wFileCount;

	//�����ڴ�
	DWORD dwDataIndex=0;
	BYTE * pFileData=new BYTE[dwDataSize];
	if(pFileData==NULL) throw TEXT("�ڴ治�㣡");

	//��ʼ����
	ZeroMemory(pFileData,dwDataSize);

	//������Ϣ
	tagUpdateInfo UpdateInfo;
	UpdateInfo.wUpdateCount=wFileCount;
	lstrcpynA(UpdateInfo.szDownLoadAddress, CT2CA(szInputNetAddr), CountArray(UpdateInfo.szDownLoadAddress));
	UpdateInfo.wStructSize=sizeof(UpdateInfo)-sizeof(UpdateInfo.szDownLoadAddress);
	UpdateInfo.wStructSize+=CountStringBufferA(UpdateInfo.szDownLoadAddress);

	//��������
	CopyMemory(pFileData, &UpdateInfo, UpdateInfo.wStructSize);
	dwDataIndex+=UpdateInfo.wStructSize;

	//��������
	tagUpdateItem UpdateItem;

	//�����ļ�
	for(INT_PTR nIndex=0;nIndex<FileNameArray.GetCount();nIndex++)
	{
		//�¼�����
		DoEvents();

		//��������
		CString  szFilePath;		

		//��ȡУ��
		szFilePath.Format(TEXT("%s\\%s"),szInputPath,FileNameArray[nIndex]);
		szCheckSum=CWHMD5Checksum::GetMD5(szFilePath);

		ZeroMemory(&UpdateItem,sizeof(UpdateItem));

        //���ñ���		
		lstrcpynA(UpdateItem.szFileName,CT2CA(FileNameArray[nIndex]),CountArray(UpdateItem.szFileName));
		lstrcpynA(UpdateItem.szFileCheckSum,CT2CA(szCheckSum),CountArray(UpdateItem.szFileCheckSum));

		//�����С
		UpdateItem.wStructSize=sizeof(UpdateItem)-sizeof(UpdateItem.szFileName);
		UpdateItem.wStructSize+=CountStringBufferA(UpdateItem.szFileName);

		//��������
		CopyMemory(&pFileData[dwDataIndex],&UpdateItem,UpdateItem.wStructSize);
		dwDataIndex+=UpdateItem.wStructSize;

		//���½���
		PostMessage(WM_UPDATE_PROGRESS,PROGRESS_CMD_UPDATE);
	}

	//�����ļ�
	CFile FileDownLoad;
	if (FileDownLoad.Open(szConfigFile,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone)==FALSE)
	{
		::AfxMessageBox(TEXT("�����ļ�����ʧ�ܣ�"));

		//�ͷ���Դ
		SafeDeleteArray(pFileData);

		return FALSE;
	}

	//ѹ������
	DWORD dwResultSize=0;
	BYTE * pResultData=new BYTE[dwDataIndex];
	ZeroMemory(pResultData,sizeof(pResultData));
	dwResultSize=CWHService::CompressData(pFileData,dwDataIndex,pResultData,dwDataIndex);

	//д���ļ�
	FileDownLoad.Write(pResultData,dwResultSize);
	FileDownLoad.Close();

	//�ͷ���Դ
	SafeDeleteArray(pFileData);
	SafeDeleteArray(pResultData);

	return TRUE;
}

//�ļ��汾
BOOL CUpdateConfigDlg::GetFileVersion(LPCTSTR pszFileName, TCHAR szFileVersion[],WORD wBufferCount)
{
	//���ñ���
	szFileVersion[0]=0;

	//��ȡ�汾
	DWORD dwFileVersion;
	if(CWHService::GetModuleVersion(pszFileName,dwFileVersion))
	{
		//��ʽת��
		BYTE * pcbFileVersion = (BYTE *)&dwFileVersion;
		_sntprintf(szFileVersion,wBufferCount,TEXT("%d.%d.%d.%d"),pcbFileVersion[3],pcbFileVersion[2],pcbFileVersion[1],pcbFileVersion[0]);

		return TRUE;
	}

	return FALSE;
}

//�滭�߿�
VOID CUpdateConfigDlg::DrawControlFrame(CDC * pDC,CRect & rcFrame)
{
	//���Ʊ߿�
	pDC->MoveTo(rcFrame.left,rcFrame.top);
	pDC->LineTo(rcFrame.right,rcFrame.top);
	pDC->MoveTo(rcFrame.right,rcFrame.top);
	pDC->LineTo(rcFrame.right,rcFrame.bottom);
	pDC->MoveTo(rcFrame.right,rcFrame.bottom);
	pDC->LineTo(rcFrame.left,rcFrame.bottom);
	pDC->MoveTo(rcFrame.left,rcFrame.bottom);
	pDC->LineTo(rcFrame.left,rcFrame.top);
}

//���Ŀ¼
CString CUpdateConfigDlg::GetBrowseFolder(HWND hParent,CString strTitle)
{
	//��������   
    BROWSEINFO bi;
    ITEMIDLIST *pidl=NULL;
	TCHAR szDirectory[MAX_PATH]={0};

	//����ṹ
    bi.hwndOwner=hParent;
    bi.pidlRoot=NULL;    
    bi.lpszTitle=strTitle;   
    bi.lpfn=NULL;
    bi.lParam=0;
    bi.iImage=0;
	bi.pszDisplayName=szDirectory;
	bi.ulFlags=BIF_RETURNONLYFSDIRS;

	//��Ŀ¼
    pidl = SHBrowseForFolder(&bi);
    if(NULL == pidl) return TEXT("");

	//��ȡĿ¼	
    if(!SHGetPathFromIDList(pidl,szDirectory)) return TEXT("");

    return CString(szDirectory);
}

//�滭����
BOOL CUpdateConfigDlg::OnEraseBkgnd(CDC* pDC)
{
	__super::OnEraseBkgnd(pDC);

	//��ȡ����
	CRect rcControl;

	//��ȡ����
	m_FileListView.GetWindowRect(&rcControl);    

	//����ת��
	ScreenToClient(&rcControl);

	//��������
	rcControl.InflateRect(1,1,1,1);

	//�滭���
	DrawControlFrame(pDC,rcControl);

	return TRUE;
}

//���½���
LRESULT CUpdateConfigDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	CProgressCtrl * pProgress = static_cast<CProgressCtrl * >(GetDlgItem(IDC_PROGRESS));
	if(pProgress == NULL) return 0;

	//��������
	WORD wProgressCmd=(WORD)wParam;
	WORD wProgressValue=(WORD)lParam;

	//���ý���
	if(PROGRESS_CMD_SETUP == wProgressCmd)
	{
		pProgress->SetPos(0);
		pProgress->SetRange(0,wProgressValue);		
	}

	//���½���
	if(PROGRESS_CMD_UPDATE == wProgressCmd)
	{		
		pProgress->SetPos(pProgress->GetPos()+1);
	}

	return 1;
}

//ȫ���ļ�
void CUpdateConfigDlg::OnBnClickedRadioAll()
{
	//��ȡ·��
	TCHAR szInputPath[MAX_PATH]=TEXT("");
	GetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath,CountArray(szInputPath));

	//�Ƴ��б�
	while(m_ComboGameList.GetCount()) m_ComboGameList.DeleteString(0);

	//ɸѡ�ļ�
	FilterFiles(szInputPath,FILE_KIND_GAME|FILE_KIND_PLATFORM,NULL);
}

//ƽ̨�ļ�
void CUpdateConfigDlg::OnBnClickedRadioPlatform()
{
	//��ȡ·��
	TCHAR szInputPath[MAX_PATH]=TEXT("");
	GetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath,CountArray(szInputPath));

	//�Ƴ��б�
	while(m_ComboGameList.GetCount()) m_ComboGameList.DeleteString(0);

	//ɸѡ�ļ�
	FilterFiles(szInputPath,FILE_KIND_PLATFORM,NULL);
}

//�����Ϸ
void CUpdateConfigDlg::OnBnClickedRadioGames()
{
	//��ȡ·��
	TCHAR szInputPath[MAX_PATH]=TEXT("");
	GetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath,CountArray(szInputPath));
	
	//�Ƴ��б�
	while(m_ComboGameList.GetCount()) m_ComboGameList.DeleteString(0);

	//ɸѡ�ļ�
	FilterFiles(szInputPath,FILE_KIND_GAME,NULL);

	//ȫ��ѡ��
	m_ComboGameList.SelectAll(TRUE);
}

//���ɸ���
void CUpdateConfigDlg::OnBnClickedButtonDone()
{
	//��������
	CStringArray GameFileArray;
	CStringArray PlatformFileArray;	

	//��������
	CWHArray<tagItemInfo *,tagItemInfo *> GameItemArray;
	CWHArray<tagItemInfo *,tagItemInfo *> PlatformItemArray;

	//��ȡ·��
	TCHAR szInputPath[MAX_PATH]=TEXT("");
	GetDlgItemText(IDC_EDIT_INPUT_PATH,szInputPath,CountArray(szInputPath));

	//��������
	tagItemInfo * pItemInfo=NULL;
	CString strInputPath,strPrefix,strConfigName;

	//���ÿؼ�
	EnableControls(false);

	//ͳ���ļ���
	int nFileCount=0; 
	
	//�������
	for(int i = 0; i < m_FileListView.GetItemCount(); i++)
	{
		pItemInfo = reinterpret_cast<tagItemInfo*>(m_FileListView.GetItemData(i));
		if(pItemInfo != NULL)
		{
			//ƽ̨����
			if(pItemInfo->cbFileKind == FILE_KIND_PLATFORM)
			{
				PlatformItemArray.Add(pItemInfo);

				//�ļ�ͳ��				
				if(pItemInfo->bIsDirectory == TRUE)
				{
					strInputPath.Format(TEXT("%s\\%s"),szInputPath,pItemInfo->strFileName);
					CensusFileCount(strInputPath,nFileCount);
				}
				else
				{
					++nFileCount;
				}
			}

			//��Ϸ����
			if(pItemInfo->cbFileKind == FILE_KIND_GAME && pItemInfo->bIsDirectory == TRUE)
			{
				GameItemArray.Add(pItemInfo);

				//�ļ�ͳ��
				strInputPath.Format(TEXT("%s\\%s"),szInputPath,pItemInfo->strFileName);
				CensusFileCount(strInputPath,++nFileCount);
			}
		}
	}	

	//���ý���
	PostMessage(WM_UPDATE_PROGRESS,PROGRESS_CMD_SETUP,nFileCount);

	//ƽ̨�ļ�
	if(PlatformItemArray.GetCount() > 0)
	{
		//�����ļ�
		for(INT_PTR i = 0; i < PlatformItemArray.GetCount(); i++)
		{
			//�¼�����
			DoEvents();

			pItemInfo=PlatformItemArray[i];
			if(pItemInfo->bIsDirectory == TRUE)
			{
				//��ʽ����
				strPrefix.Format(TEXT("%s\\"),pItemInfo->strFileName);
				strInputPath.Format(TEXT("%s\\%s"),szInputPath,pItemInfo->strFileName);

				//�����ļ�
				SearchFiles(strInputPath,strPrefix,PlatformFileArray);
			}
			else
			{
				PlatformFileArray.Add(pItemInfo->strFileName);
			}
		}

		//����ƽ̨����
		if(PlatformFileArray.GetCount() > 0)
		{
			//��������
			MakeUpdateConfig(PlatformFileArray, m_szPlazaConfigName);
		}
	}

	//��Ϸ�ļ�
	if(GameItemArray.GetCount() > 0)
	{
		//������Ϸ�ļ�
		for(INT_PTR i = 0; i < GameItemArray.GetCount(); i++)
		{
			//�¼�����
			DoEvents();

			//�Ƴ�Ԫ��
			GameFileArray.RemoveAll();

			//��ȡ����
			pItemInfo=GameItemArray[i];

			//�����Ϸ����
			strInputPath.Format(TEXT("%s.exe"),pItemInfo->strFileName);
			GameFileArray.Add(strInputPath);

			//��ʽ����
			strPrefix.Format(TEXT("%s\\"),pItemInfo->strFileName);
			strInputPath.Format(TEXT("%s\\%s"),szInputPath,pItemInfo->strFileName);

			//�����ļ�
			SearchFiles(strInputPath,strPrefix,GameFileArray);

			//������Ϸ����
			if(GameFileArray.GetCount() > 0)
			{
				//��������
				strConfigName.Format(TEXT("%s%s"),pItemInfo->strFileName,m_szGameExpandName);
				MakeUpdateConfig(GameFileArray, strConfigName);
			}
		}
	}

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//��������
	CString szConfigFile;
	szConfigFile.Format(TEXT("%s\\%s"),szWorkDirectory,UPDATE_CONFIG_NAME);

	//��������	
	SaveConfig(szConfigFile);

	//���ÿؼ�
	EnableControls(true);
}

//���Ŀ¼
void CUpdateConfigDlg::OnBnClickedBrowseInputpath()
{
	//��ʾ���
	CString strInputPath=GetBrowseFolder(m_hWnd,TEXT("ѡ������ļ����Ŀ¼"));
	if(strInputPath.IsEmpty() == false)	
	{
		SetDlgItemText(IDC_EDIT_INPUT_PATH,strInputPath);
	}
}

//���Ŀ¼
void CUpdateConfigDlg::OnBnClickedBrowseOutputpath()
{
	//��ʾ���
	CString strOutPutPath=GetBrowseFolder(m_hWnd,TEXT("ѡ������������Ŀ¼"));
	if(strOutPutPath.IsEmpty() == false)	
	{
		SetDlgItemText(IDC_EDIT_OUTPUT_PATH,strOutPutPath);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
