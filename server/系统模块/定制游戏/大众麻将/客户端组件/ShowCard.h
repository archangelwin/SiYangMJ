#pragma once

#include "Stdafx.h"
#include "Resource.h"
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CShowCard : public CDialog
{
	//�ؼ�����
public:
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	CPngImage						m_ImageCard;						//�˿���Դ
	int								m_nWidth;							//�˿˿��
	int								m_nHeight;							//�˿˸߶�

	CGameClientEngine *				m_pClientEngine;					//�ṹָ��

	//ѡ���˿�
private:
	BYTE							m_cbSelCardData[MAX_COUNT];			//�ƶ�����
	BYTE							m_cbSelCardCount;					//�˿�����
private:
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];	//�ƶ�����
	BYTE							m_cbCardCount;						//�˿�����
	CRect							m_rcCard[MAX_REPERTORY];			//��¼����
	bool							m_bSelectCard[MAX_REPERTORY];		//�Ƿ�ѡ��

	//��������	
private:
	BYTE							m_cbHandSelCardData[MAX_INDEX];		//�ƶ�����
	BYTE							m_cbHandSelCardCount;				//�˿�����

	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�
	BYTE							m_cbHandCardCount;					//�˿�����
	CRect							m_rcHandCard[MAX_COUNT];			//��¼����
	bool							m_bHandSelectCard[MAX_COUNT];		//�Ƿ�ѡ��


	//��������
public:
	//���캯��
	CShowCard();
	//��������
	virtual ~CShowCard();

	//������
	virtual bool __cdecl ReqResult(const void * pBuffer,CGameClientEngine* pClient);
	//��������
	virtual void __cdecl SetLeftCardCount(BYTE cbRepertoryCardCount);
	//��������
	virtual void __cdecl SetHandCardData(BYTE cbHandCardData[], BYTE cbCardCount, BYTE cbCurrentCard);
	//���õ�ǰ
	virtual void __cdecl SetCurrentCard(BYTE cbCurrentCard);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//�滭��Ϣ
public:
	//�滭����
	afx_msg void OnPaint();
	//��������
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
