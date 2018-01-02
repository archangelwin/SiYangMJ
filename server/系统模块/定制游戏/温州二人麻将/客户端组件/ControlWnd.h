#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_USER_ACTION       (WM_USER+300)           //�û�����

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
  //�ؼ�����
protected:
  CSkinButton           m_btChiLeft;            //�ؼ���ť
  CSkinButton           m_btChiCenter;            //�ؼ���ť
  CSkinButton           m_btChiRight;           //�ؼ���ť
  CSkinButton           m_btPeng;             //���ư�ť
  CSkinButton           m_btGang;             //���ư�ť
  CSkinButton           m_btChiHu;              //���ư�ť
  CSkinButton           m_btGiveUp;             //���ư�ť


  //��Դ����
protected:
  CBitImage           m_ImageControlBack;         //���ڱ���


  //��������
public:
  //���캯��
  CControlWnd();
  //��������
  virtual ~CControlWnd();

  //���ܺ���
public:
  //����״̬
  VOID SetControlInfo(BYTE cbAcitonMask);
  //�ڲ�����
protected:
  //�����ؼ�
  VOID RectifyControl(INT nWidth, INT nHeight);

  //��ť��Ϣ
protected:
  //���ư�ť
  VOID OnBnClickedPeng();
  //���ư�ť
  VOID OnBnClickedGang();
  //���ư�ť
  VOID OnBnClickedHear();
  //�Ժ���ť
  VOID OnBnClickedChiHu();
  //������ť
  VOID OnBnClickedGiveUp();
  //���ư�ť
  VOID OnBnClickedChiLeft();
  //���ư�ť
  VOID OnBnClickedChiCenter();
  //���ư�ť
  VOID OnBnClickedChiRight();

  //��Ϣӳ��
protected:
  //�ػ�����
  VOID OnPaint();
  //������Ϣ
  INT OnCreate(LPCREATESTRUCT lpCreateStruct);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
