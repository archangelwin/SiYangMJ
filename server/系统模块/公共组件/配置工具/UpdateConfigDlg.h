#ifndef UPDATE_MAIN_HEAD_FILE
#define UPDATE_MAIN_HEAD_FILE
#pragma once

//�����ļ�
#include "FileListView.h"
#include "CheckComboBox.h"

///////////////////////////////////////////////////////////////////////////////////

// ��������
class CUpdateConfigDlg : public CDialog
{
	//��Դ����
protected:
	HICON							m_hIcon;						//����ͼ�� 	

	//���ñ���
protected:
	TCHAR							m_szPlazaConfigName[32];		//��������
	TCHAR							m_szGameExpandName[32];			//��չ����

	//�ؼ�����
protected:	
	CFileListView					m_FileListView;					//�ļ��б�
	CCheckComboBox					m_ComboGameList;				//��Ϸ������	

	//��������
public:
	//���캯��
	CUpdateConfigDlg(CWnd* pParent = NULL);	

	//�̳к���
protected:	
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���ݽ���
	virtual void DoDataExchange(CDataExchange* pDX);	
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�ļ�����
protected:
	//��������
	bool LoadConfig(LPCTSTR pszConfigFile);	
	//��������
	bool SaveConfig(LPCTSTR pszConfigFile);
	//ͳ���ļ�
	void CensusFileCount(LPCTSTR pszFilePath,int & nFileCount);
	//ɸѡ�ļ�
	void FilterFiles(LPCTSTR pszFilePath,BYTE cbFileKind,LPCTSTR pszGameNames);
	//�����ļ�
	void SearchFiles(LPCTSTR pszFilePath,LPCTSTR pszPrefix,CStringArray & StringArray);	

	//��������
protected:
	//ʹ������
	VOID EnableControls(bool bEnabled);
	//�ļ���С
	DWORD GetFileSize(LPCTSTR pszFileName);	
	//���Ʊ߿�
	VOID DrawControlFrame(CDC * pDC,CRect & rcFrame);	
	//���Ŀ¼
	CString GetBrowseFolder(HWND hParent,CString strTitle);
	//�ļ��汾
	BOOL GetFileVersion(LPCTSTR pszFileName, TCHAR szFileVersion[],WORD wBufferCount);

	//���ܺ���
protected:
	//�ļ�����
	BOOL IsFileExist(LPCTSTR pszFileName);	
	//��Ϸ����
	BOOL IsGameProcess(LPCTSTR pszFileName);
	//Ŀ¼����
	BOOL IsDirectoryExist(LPCTSTR pszDirectory);	
	//�������
	VOID MarkFileKind(LPCTSTR pszWorkPath,tagFileInfo & FileInfo);
	//��������
	BOOL MakeUpdateConfig(CStringArray & FileNameArray,LPCTSTR pszConfigName);

	//��Ϣ����
protected:	
	//�滭����
	BOOL OnEraseBkgnd(CDC* pDC);

	//��Ϣ����
public:
	//���½���
	LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);

	//��Ϣ����
public:
	//ȫ���ļ�
	afx_msg void OnBnClickedRadioAll();
	//��Ϸ�ļ�
	afx_msg void OnBnClickedRadioGames();
	//ƽ̨�ļ�
	afx_msg void OnBnClickedRadioPlatform();	
	//���ɸ���
	afx_msg void OnBnClickedButtonDone();
	//���Ŀ¼
	afx_msg void OnBnClickedBrowseInputpath();
	//���Ŀ¼
	afx_msg void OnBnClickedBrowseOutputpath();

	DECLARE_MESSAGE_MAP()			
};

///////////////////////////////////////////////////////////////////////////////////
#endif
