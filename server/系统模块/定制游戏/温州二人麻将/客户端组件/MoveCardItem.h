#ifndef MOVE_CARD_ITEM_HEAD_FILE
#define MOVE_CARD_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�ƶ�����
enum MoveKind
{
  MK_NULL         = 0,              //
  MK_OutCard,                       //����
  MK_SendCard,                      //����
  MK_ReplaceCard,                     //����
  MK_StartCard                      //���ַ���
};

//////////////////////////////////////////////////////////////////////////

//�ƶ��˿���
class CMoveCardItem
{
  //�ƶ�����
protected:
  MoveKind            m_MoveKind;         //�ƶ�����

public:
  //���캯��
  CMoveCardItem() {}

  //��������
  virtual ~CMoveCardItem() {}

public:
  //��ȡ�ƶ�����
  virtual MoveKind GetMoveKind()
  {
    return m_MoveKind;
  }
};


//������
class COutCardItem:public CMoveCardItem
{
public:
  WORD              m_wOutCardUser;           //�����û�
  BYTE              m_cbOutCardData;          //�����˿�

public:
  //���캯��
  COutCardItem();
  //���캯��
  COutCardItem(const CMD_S_OutCard *pOutCard);

  //��������
  virtual ~COutCardItem();
};

//������
class CSendCardItem:public CMoveCardItem
{
public:
  BYTE              m_cbCardData;           //�˿�����
  BYTE              m_cbActionMask;           //��������
  WORD              m_wCurrentUser;           //��ǰ�û�
  WORD              m_wReplaceUser;           //�����û�
  WORD              m_wSendCardUser;          //�����û�
  bool              m_bTail;              //�Ƿ��β������

public:
  //���캯��
  CSendCardItem();
  //���캯��
  CSendCardItem(const CMD_S_SendCard *pSendCard);

  //��������
  virtual ~CSendCardItem();
};

//������
class CReplaceCardItem:public CMoveCardItem
{
public:
  WORD              m_wReplaceUser;           //�����û�
  BYTE              m_cbReplaceCard;          //�����˿�

public:
  //���캯��
  CReplaceCardItem();
  //���캯��
  CReplaceCardItem(const CMD_S_ReplaceCard *pReplaceCard);

  //��������
  virtual ~CReplaceCardItem();
};

//���ַ�����
class CStartCardItem:public CMoveCardItem
{
public:
  WORD              m_wChairId;             //�������
  BYTE              m_cbCardCount;            //������
  BYTE              m_cbCardData[4];          //����ֵ
  WORD              m_wHeapId;              //����λ��
  WORD              m_wHeapCardIndex;         //����λ��
  bool              m_bLastItem;            //�Ƿ����һ����

public:
  //���캯��
  CStartCardItem();
  //��������
  virtual ~CStartCardItem();
};

//////////////////////////////////////////////////////////////////////////

#endif