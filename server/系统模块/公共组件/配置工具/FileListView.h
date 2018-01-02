#ifndef FILE_LIST_VIEW_HEAD_FILE
#define FILE_LIST_VIEW_HEAD_FILE
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////
//�ļ���Ϣ
struct tagFileInfo
{
	TCHAR							szFileName[64];						//�ļ�����
	TCHAR							szFileType[32];						//�ļ�����
	TCHAR							szFileVersion[32];					//�ļ��汾
	BYTE							cbFileKind;							//�ļ�����
	BOOL							bIsDirectory;						//Ŀ¼��ʶ
	DWORD							dwFileSize;							//�ļ���С
	FILETIME						ftModifyDate;						//�޸�����
};

//������Ϣ
struct tagItemInfo 
{
	CString							strFileName;						//�ļ�����
	CString							strFileVersion;						//�ļ��汾
	CString							strType;							//�ļ�����
	BYTE							cbFileKind;							//�ļ�����
	BOOL							bIsDirectory;						//�Ƿ��ļ���
	DWORD							nFileSize;							//�ļ���С	
	FILETIME						ftLastWriteTime;					//�޸�����
};

//�ļ�����
#define FILE_KIND_GAME				0x01								//��Ϸ�ļ�
#define FILE_KIND_PLATFORM			0x02								//ƽ̨�ļ�

///////////////////////////////////////////////////////////////////////////////////////////
//�б���ͼ
class CFileListView : public CListCtrl
{
	//��������
protected:
	int											m_nSortedCol;						//����������

	//��������
public: 
	//���캯��
	CFileListView();
	//��������
	virtual ~CFileListView();	

	//���ܺ���
public:
	//�������
	tagItemInfo * AddItem(int nIndex, tagFileInfo * pFileInfo);
	//������ļ���
	tagItemInfo * AddNewFolder(int nIndex, LPCTSTR lpszFolder);
	//������ļ�
	tagItemInfo * AddNewFile(int nIndex, LPCTSTR lpszFileName, DWORD dwFileSize);

	//��д����
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//���ܺ���
public:
	//�����б�
	VOID ResetListView();
	//�����ļ���
	VOID SetFileName(int nIndex, LPCTSTR lpszFileName);
	//ִ������
	VOID Sort(int nCol = -1);
	//�ļ��б�ʶ
	BOOL IsDirectory(int nItem);
	//��ȡ�ļ���С
	DWORD GetFileSize(int nItem);
	//��ȡ�޸�ʱ��
	FILETIME GetLastWriteTime(int nItem);
	//��ʽ����С
	LPCTSTR FormatSize(DWORD dwSizeLow, DWORD dwSizeHigh = 0);

	//��������
protected:
	//�ͷ�����
	VOID FreeItemMemory();
	//�ݹ��б�
	VOID RecursiveFileList(LPCTSTR lpszPath, CStringArray &strFileNameArray);
	//��ȡ����
	LPCTSTR GetTypeName(CString strPath);
	//��ʼ�б�
	BOOL InitListViewImageLists();	

	//��̬����
public:
	//�ȽϺ���
	static int CALLBACK CompareFunc (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	//��Ϣ����
protected:	
	//������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	//��ȡ��Ϣ
	VOID OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	//��ͷ���
	VOID OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//ɾ������
	VOID OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////////////
#endif
