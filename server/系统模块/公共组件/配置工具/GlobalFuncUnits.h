#ifndef GLOBAL_FUNC_UNITS_HEAD_FILE
#define GLOBAL_FUNC_UNITS_HEAD_FILE
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////

//������Ϣ
extern VOID DoEvents();
//��ȡͼ������
extern int GetDiskIconIndex();
//��ȡͼ������
extern int GetDirectoryIconIndex();
//��ȡͼ������
extern int GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected = FALSE);
//�ȴ��������
extern BOOL WaitOperateResult(HANDLE hEventHandle,BOOL * pOperateResult,int nTimeout);

////////////////////////////////////////////////////////////////////////////////////////////////

#endif