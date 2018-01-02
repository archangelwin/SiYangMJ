#pragma once

#include "Stdafx.h"
#include "Resource.h"

//�ṹ����
//////////////////////////////////////////////////////////////////
struct  tagPhraseSoundItem
{
	BOOL                              bDisplay;                //�Ƿ���ʾ
	BOOL                              bSelected;               //�Ƿ�ѡ�� 
	BYTE                              cbItemType;              //��������
	CRect                             crDisplayRect;           //��ʾ����
	TCHAR                             DisplayText[128];        //��ʾ�ı�
public:
	tagPhraseSoundItem()
	{
		//��ʼ������
		crDisplayRect.SetRectEmpty();
		ZeroMemory(DisplayText,sizeof(DisplayText));
		bDisplay=false;
		bSelected=false;
	}
	~tagPhraseSoundItem()
	{
		crDisplayRect.SetRectEmpty();
		ZeroMemory(DisplayText,sizeof(DisplayText));
	}
	
};
//////////////////////////////////////////////////////////////////


// CPhraseSoundWnd

//�������ﴰ��
class CPhraseSoundWnd : public CWnd
{
	//���Ʊ���
private:
	CPtrArray                        m_PhraseSoundList;        //�����б�
	BYTE                             m_cbCurrPageIndex;        //��ǰҳ��
	BYTE                             m_cbCountPerPage;         //ҳ��ʾ��
	BYTE                             m_cbLastSelected;         //�ϴ�ѡ��
	BYTE                             m_cbCurrSelected;         //��ǰѡ��
	BYTE                             m_cbCurrItemType;         //��������

	//��Դ����
private:
	CSkinImage                       m_ImageBack;              //���屳��
	CSkinButton                      m_btPageUp;               //��ҳ��ť
	CSkinButton                      m_btPageDown;             //��ҳ��ť
	//CPngImage                        m_ImageNormal;            //��ͨ״̬
//	CPngImage                        m_ImageSelected;          //ѡ��״̬
	CBrush                           m_BrushNormal;            //��ͨ��ˢ
	CBrush                           m_BrushSelected;          //ѡ�л�ˢ  

public:
	//���캯��
	CPhraseSoundWnd();
	//��������
	virtual ~CPhraseSoundWnd();

	//���ܺ���
public:
	//�������
	VOID  AddPhraseSoundItem(LPCTSTR lpItemText,BYTE cbItemType);
	//�������
	VOID  AddPhraseSoundItem(tagPhraseSoundItem * pPhraseSoundItem);
	//ɾ������
	VOID  DeletePhraseSoundItem(LPCTSTR lpItemText);
	//�Ƴ�����
	VOID  RemovePhraseSoundItem(const tagPhraseSoundItem * pPhraseSoundItem);
	//����ת��
    tagPhraseSoundItem *  TransPointToPhraseSoundItem(const CPoint  point);
	//����״̬
	VOID  UpdateItemsDisplayStatus(bool bShow,int nStartIndex,int nEndIndex);
	//����״̬
	VOID  UpdateItemsSelectedStatus(bool bSelected,int nSlectedIndex);
	//��������
	VOID  AdjustItemDisplayRect();
	//������������
	VOID  SetItemType(BYTE cbItemType) { m_cbCurrItemType=cbItemType; AdjustItemDisplayRect(); };
	//���°�ť״̬
	VOID  UpdateButtonsStatus(bool lLastPage);
	//������������
	VOID  ClacUniteRect(CRect & UpdateRect,int nStartIndex,int nEndIndex);
	//��ȡѡ���ı�
	VOID GetSelectedText(CString & csWindowText,int nSelectIndex);

protected:
	DECLARE_MESSAGE_MAP()
public:
	//���ƽ���
	afx_msg void OnPaint();
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�����ƶ�
	afx_msg void OnMove(int x, int y);
	//���Ʊ���
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ҳ��ť
	VOID OnBnClickSoundPageDown();
    //��ҳ��ť
	VOID OnBnClickSoundPageUp();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


