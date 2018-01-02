#include "Stdafx.h"
#include "GlobalFuncUnits.h"

////////////////////////////////////////////////////////////////////////////////////////////////

//������Ϣ
VOID DoEvents()
{
	MSG msg;

	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}

//��ȡͼ������
int GetDiskIconIndex()
{
	static int nIconIndex=-1;

	if(nIconIndex==-1)
	{
		//��ȡĿ¼
		TCHAR szCurrDirectory[MAX_PATH]=TEXT("");
		GetCurrentDirectory(MAX_PATH,szCurrDirectory);

		//��������
		CString strCurrDirectory(szCurrDirectory);
		int nPos = strCurrDirectory.Find(TEXT("\\"));
		strCurrDirectory = strCurrDirectory.Left(nPos+1);

		//��ȡ����
		nIconIndex = GetIconIndex(strCurrDirectory,TRUE,FALSE);
	}

	return nIconIndex;
}

//��ȡͼ������
int GetDirectoryIconIndex()
{
	static int nIconIndex=-1;

	if(nIconIndex==-1)
	{
		//��ȡĿ¼
		TCHAR szCurrDirectory[MAX_PATH]=TEXT("");
		GetCurrentDirectory(MAX_PATH,szCurrDirectory);

		//��ȡ����
		nIconIndex = GetIconIndex(szCurrDirectory,TRUE,FALSE);
	}	

	return nIconIndex;
}

//��ȡͼ������
int GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected)
{
	int _nDirIconIdx = -1;
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	if (bIsDir)
	{
		//if (_nDirIconIdx == -1)
		{
			SHGetFileInfo(lpszPath,FILE_ATTRIBUTE_DIRECTORY,&sfi,sizeof(sfi), 
				SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | (bSelected ? SHGFI_OPENICON : 0)
			);
			_nDirIconIdx = sfi.iIcon;
		}

		return _nDirIconIdx;
	}
	else
	{
		SHGetFileInfo (lpszPath,FILE_ATTRIBUTE_NORMAL,&sfi,sizeof(sfi), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | (bSelected ? SHGFI_OPENICON : 0)
		);
		return sfi.iIcon;
	}
	return -1;
}

//�ȴ��������
BOOL WaitOperateResult(HANDLE hEventHandle,BOOL * pOperateResult,int nTimeout)
{
	//��������
	if(pOperateResult==NULL) return FALSE;

	//�����¼�
	*pOperateResult=FALSE;
	ResetEvent(hEventHandle);

	//��������
	DWORD dwRet;
    DWORD dwMaxTick = GetTickCount() + nTimeout;

	while(TRUE)
	{
		dwRet = MsgWaitForMultipleObjects(1, &hEventHandle, FALSE, dwMaxTick - GetTickCount(), QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
		{			
			return (BOOL)*pOperateResult;      
		}   
		else
		if (dwRet == WAIT_OBJECT_0 + 1)
		{			
			DoEvents();
		}  
		else
		{
			return FALSE;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////