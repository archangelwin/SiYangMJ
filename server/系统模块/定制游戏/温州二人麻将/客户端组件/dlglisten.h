#ifndef DLG_RESPONSE_LEAVE_HEAD_FILE
#define DLG_RESPONSE_LEAVE_HEAD_FILE

#pragma once


#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�����뿪��Ӧ
class CDlgListen : public CSkinDialog
{
  //�������
protected:
  WORD              m_wTimer;             //��ʱʱ��
  CString             m_strCaption;           //�Ի������

public:
  bool              m_bZhuaPao;             //ץ��
  bool              m_bZiMo;              //����

  //�ؼ�����
public:
  CSkinButton           m_btOk;               //ȷ����ť
  CSkinButton           m_btCancel;             //ȡ����ť

  //��������
public:
  //���캯��
  CDlgListen(CWnd * pParentWnd=NULL);
  //��������
  virtual ~CDlgListen();

  //���غ���
protected:
  //�ؼ���
  virtual VOID DoDataExchange(CDataExchange * pDX);
  //��ʼ������
  virtual BOOL OnInitDialog();
  //ȷ������
  virtual VOID OnOK();

  DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////

#endif
