// PhraseSoundWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PhraseSoundWnd.h"
#include ".\phrasesoundwnd.h"

//�궨��
///////////////////////////////////////////////////////////////////

#define IDC_PAGE_UP						108            //��һҳ
#define IDC_PAGE_DOWN					109            //��һҳ

#define ITEM_WIDTH                      176            //������
#define ITEM_HEIGHT                     22             //����߶�
#define ITEM_DISS                       1              //������
#define TOP_DISS                        13              //������� 

///////////////////////////////////////////////////////////////////


// CPhraseSoundWnd

//Message Map
BEGIN_MESSAGE_MAP(CPhraseSoundWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()

	ON_BN_CLICKED(IDC_PAGE_UP, OnBnClickSoundPageUp)
	ON_BN_CLICKED(IDC_PAGE_DOWN, OnBnClickSoundPageDown)
END_MESSAGE_MAP()


//���캯��
CPhraseSoundWnd::CPhraseSoundWnd()
{
	//��ʼ������
	m_cbCurrPageIndex=0;
	m_cbCountPerPage=10;
	m_cbCurrSelected=255;
	m_cbLastSelected=255;
	m_cbCurrItemType=0;

	m_BrushNormal.CreateSolidBrush(RGB(39,63,99));
    m_BrushSelected.CreateSolidBrush(RGB(59,93,139));

	//������Դ
	HINSTANCE hInstance= AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_SOUND_BACK,hInstance,true);
	//m_ImageNormal.LoadImage(hInstance,"ITEM_NORMAL");
	//m_ImageSelected.LoadImage(hInstance,"ITEM_SELECTED");

}

//��������
CPhraseSoundWnd::~CPhraseSoundWnd()
{
//	ASSERT(m_PhraseSoundList!=NULL);
	if(m_PhraseSoundList.IsEmpty()) return;

	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
		delete m_PhraseSoundList[i];
	m_PhraseSoundList.RemoveAll();
}


//�������
VOID  CPhraseSoundWnd::AddPhraseSoundItem(LPCTSTR lpItemText,BYTE cbItemType)
{
	//�½�����
	tagPhraseSoundItem *  PhraseSoundItem = new tagPhraseSoundItem();
    PhraseSoundItem->bDisplay=true;
	PhraseSoundItem->bSelected=false;
	PhraseSoundItem->cbItemType=cbItemType;
	CopyMemory(PhraseSoundItem->DisplayText,lpItemText,sizeof(PhraseSoundItem->DisplayText));

    return AddPhraseSoundItem(PhraseSoundItem);
}

//�������
VOID  CPhraseSoundWnd::AddPhraseSoundItem(tagPhraseSoundItem * pPhraseSoundItem)
{
	//������֤
	ASSERT(pPhraseSoundItem!=NULL);

	m_PhraseSoundList.Add(pPhraseSoundItem);
}

//ɾ������
VOID  CPhraseSoundWnd::DeletePhraseSoundItem(LPCTSTR lpItemText)
{
	/*if(lpItemText==NULL) return;
	for(int i=0;i<m_PhraseSoundList.)*/
}

//�Ƴ�����
VOID  CPhraseSoundWnd::RemovePhraseSoundItem(const tagPhraseSoundItem * pPhraseSoundItem)
{

}

//����ת��
tagPhraseSoundItem *  CPhraseSoundWnd::TransPointToPhraseSoundItem(const CPoint  point)
{
	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		tagPhraseSoundItem * pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList[i];
		if((pPhraseSoundItem->crDisplayRect.PtInRect(point)) && (pPhraseSoundItem->bDisplay==TRUE))
		{
			UpdateItemsSelectedStatus(true,i);
			m_cbLastSelected=m_cbCurrSelected;
			m_cbCurrSelected=i;
			return  pPhraseSoundItem;
		}
	}
	return NULL;
}

//��ȡѡ���ı�
VOID CPhraseSoundWnd::GetSelectedText(CString & csWindowText,int nSelectIndex) 
{
	csWindowText.Empty();

	if(nSelectIndex!=255) 
		csWindowText=((tagPhraseSoundItem *)m_PhraseSoundList.GetAt(nSelectIndex))->DisplayText;
};


//����״̬
VOID  CPhraseSoundWnd::UpdateItemsDisplayStatus(bool bShow,int nStartIndex,int nEndIndex)
{
	if(m_PhraseSoundList.GetCount()==0) return;

	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		tagPhraseSoundItem * pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList[i];
		ASSERT(pPhraseSoundItem!=NULL);
		if(i >=nStartIndex && i <=nEndIndex)
			pPhraseSoundItem->bDisplay=bShow;
		else
			pPhraseSoundItem->bDisplay=!bShow;
	}

}

//����״̬
VOID  CPhraseSoundWnd::UpdateItemsSelectedStatus(bool bSelected,int nSlectedIndex)
{
//	ASSERT(m_PhraseSoundList!=NULL);
	if(m_PhraseSoundList.GetCount()==0) return;

	//��������ѡ��״̬
	BYTE cbItemCount=0;
	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		tagPhraseSoundItem * pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList[i];
		ASSERT(pPhraseSoundItem!=NULL);
		if(pPhraseSoundItem->bDisplay==TRUE)
		{
			if(cbItemCount>=m_cbCountPerPage) break;
			if(i ==nSlectedIndex)
				pPhraseSoundItem->bSelected=bSelected;
			else
				pPhraseSoundItem->bSelected=!bSelected;
			++cbItemCount;
		}
	}
}

//��������
VOID  CPhraseSoundWnd::AdjustItemDisplayRect()
{
//	ASSERT(m_PhraseSoundList!=NULL);
	if(m_PhraseSoundList.GetCount()==0) return;

	//������������
	BYTE cbItemCount=0;
	BYTE cbItemTypeStart=255;
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CRect rcItemDislplay((ClientRect.Width() - ITEM_WIDTH)/2,TOP_DISS,(ClientRect.Width() - ITEM_WIDTH)/2+ITEM_WIDTH,TOP_DISS+ITEM_HEIGHT);
	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		//��������
		tagPhraseSoundItem * pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList.GetAt(i);
		ASSERT(pPhraseSoundItem!=NULL);
		if(pPhraseSoundItem->cbItemType==m_cbCurrItemType)
		{
			if(cbItemTypeStart==255) cbItemTypeStart=i;
		    pPhraseSoundItem->crDisplayRect=rcItemDislplay;

			//�������
			++cbItemCount;

			//���µ�������
			if(i >0 && cbItemCount > 0 && cbItemCount%m_cbCountPerPage==0) 
				rcItemDislplay.OffsetRect(0,-(ITEM_HEIGHT+ITEM_DISS) * (m_cbCountPerPage-1)); 
			else
	    		//����ƫ��
		    	rcItemDislplay.OffsetRect(0,ITEM_HEIGHT+ITEM_DISS);
		}
	}
    UpdateItemsDisplayStatus(TRUE,cbItemTypeStart,__min(cbItemTypeStart+__min(m_cbCountPerPage-1,cbItemCount-1),m_PhraseSoundList.GetCount()));
	UpdateButtonsStatus(cbItemCount<=m_cbCountPerPage);
}


// CPhraseSoundWnd ��Ϣ�������

//��ҳ��ť
VOID CPhraseSoundWnd::OnBnClickSoundPageDown()
{
	BYTE cbItemCount=0;
	BYTE cbCurrTypeStartIndex=255;
	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		tagPhraseSoundItem * pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList.GetAt(i);
		ASSERT(pPhraseSoundItem!=NULL);
		if(pPhraseSoundItem->cbItemType==m_cbCurrItemType)
		{
			if(cbCurrTypeStartIndex==255) cbCurrTypeStartIndex=i;
			++cbItemCount;
		}
    }

	++m_cbCurrPageIndex;

	//���㵱ǰҳ��β����
	BYTE cbStartIndex=cbCurrTypeStartIndex+m_cbCurrPageIndex * m_cbCountPerPage;
	BYTE cbEndIndex=cbStartIndex+__min(m_cbCountPerPage-1,cbItemCount-cbStartIndex-1);

	//ѡ��ʧЧ
	if(m_cbCurrSelected!=255) ((tagPhraseSoundItem *)m_PhraseSoundList.GetAt(m_cbCurrSelected))->bSelected=false;
	m_cbCurrSelected=255;

	//������ʾ״̬
	UpdateItemsDisplayStatus(true,cbStartIndex,cbEndIndex);
	
	//���°�ť״̬
	UpdateButtonsStatus(cbEndIndex>=cbCurrTypeStartIndex+cbItemCount-1);
    
	//�����������
	CRect crUpdateRect(12,10,190,242);
    
	//���½���
	InvalidateRect(crUpdateRect,false);
}

//��ҳ��ť
VOID CPhraseSoundWnd::OnBnClickSoundPageUp()
{
	//��һҳû������
	if(m_cbCurrPageIndex==0) return;

	BYTE cbItemCount=0;
	BYTE cbCurrTypeStartIndex=255;
	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		tagPhraseSoundItem * pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList.GetAt(i);
		ASSERT(pPhraseSoundItem!=NULL);
		if(pPhraseSoundItem->cbItemType==m_cbCurrItemType)
		{
			if(cbCurrTypeStartIndex==255) cbCurrTypeStartIndex=i;
			++cbItemCount;
		}
	}
	--m_cbCurrPageIndex;
	BYTE cbStartIndex=cbCurrTypeStartIndex+m_cbCurrPageIndex * m_cbCountPerPage;
	BYTE cbEndIndex=cbStartIndex+m_cbCountPerPage-1;

	//ѡ��ʧЧ
	if(m_cbCurrSelected!=255) ((tagPhraseSoundItem *)m_PhraseSoundList.GetAt(m_cbCurrSelected))->bSelected=false;
	m_cbCurrSelected=255;

	//������ʾ״̬
	UpdateItemsDisplayStatus(true,cbStartIndex,cbEndIndex);
	
	//���°�ť״̬
	UpdateButtonsStatus(cbEndIndex>=cbCurrTypeStartIndex+cbItemCount-1);

	//�����������
	CRect crUpdateRect(12,10,190,242);
    
	//���½���
	InvalidateRect(crUpdateRect,false);
}

//���°�ť״̬
VOID  CPhraseSoundWnd::UpdateButtonsStatus(bool lLastPage)
{
	if(m_cbCurrPageIndex==0)
	{
		m_btPageUp.EnableWindow(false);
		if(lLastPage==TRUE)
		    m_btPageDown.EnableWindow(false);
		else
			m_btPageDown.EnableWindow(true);
	}
	else
	{
		m_btPageUp.EnableWindow(true);
		if(lLastPage==TRUE)
		    m_btPageDown.EnableWindow(false);
		else
			m_btPageDown.EnableWindow(true);
	}
}

//������������
VOID  CPhraseSoundWnd::ClacUniteRect(CRect & UpdateRect,int nStartIndex,int nEndIndex)
{
	//�������
	UpdateRect.SetRectEmpty();

	tagPhraseSoundItem * pPhraseSoundItem=NULL;
    for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		if(i>=nStartIndex && i< nEndIndex)
		{
			pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList[i];
			UpdateRect.UnionRect(UpdateRect,pPhraseSoundItem->crDisplayRect);
		}
	}
}

void CPhraseSoundWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	m_ImageBack.AlphaDrawImage(&dc,0,0,RGB(255,0,255));

	//�����������豸
	SetBkMode(dc,TRANSPARENT);

	//��������
	tagPhraseSoundItem * pPhraseSoundItem=NULL;
	BYTE cbDisplayerCount=0;
	CFont Newfont;
	CFont * Oldfont;
	Newfont.CreatePointFont(100,TEXT("����"));
	Oldfont = dc.SelectObject(&Newfont);
	for(int i=0;i<m_PhraseSoundList.GetCount();i++)
	{
		if(cbDisplayerCount>=m_cbCountPerPage) break;
		pPhraseSoundItem = (tagPhraseSoundItem *)m_PhraseSoundList[i];
		if(pPhraseSoundItem->bDisplay==TRUE)
		{
			if(pPhraseSoundItem->bSelected==TRUE)
			{
				dc.FillRect(pPhraseSoundItem->crDisplayRect,&m_BrushSelected);
				//m_ImageSelected.DrawImage(&dc,pPhraseSoundItem->crDisplayRect.left,pPhraseSoundItem->crDisplayRect.top);
			}
			else
			{
				dc.FillRect(pPhraseSoundItem->crDisplayRect,&m_BrushNormal);
				//m_ImageNormal.DrawImage(&dc,pPhraseSoundItem->crDisplayRect.left,pPhraseSoundItem->crDisplayRect.top);
			}
				
			SetTextColor(dc,RGB(255,255,0));
			DrawText(dc,pPhraseSoundItem->DisplayText,strlen(pPhraseSoundItem->DisplayText),pPhraseSoundItem->crDisplayRect,DT_LEFT|DT_VCENTER| DT_SINGLELINE          |DT_END_ELLIPSIS);		

			++cbDisplayerCount;
		}
	}
	dc.SelectObject(Oldfont);
	Newfont.DeleteObject();    
	return;
}

void CPhraseSoundWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	tagPhraseSoundItem * pPhraseSoundItem;
	if((pPhraseSoundItem=TransPointToPhraseSoundItem(point))!=NULL)
	{
		CRect crUpdate(0,0,0,0);
		if(m_cbLastSelected!=255)
		  crUpdate.UnionRect(crUpdate,((tagPhraseSoundItem *)m_PhraseSoundList[m_cbLastSelected])->crDisplayRect);
      	crUpdate.UnionRect(crUpdate,pPhraseSoundItem->crDisplayRect);
		
		if(m_cbLastSelected!=m_cbCurrSelected)
		  InvalidateRect(crUpdate,false);

	}         
	CWnd::OnMouseMove(nFlags, point);
}

void CPhraseSoundWnd::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	//�ػ�����
	//Invalidate();
}

BOOL CPhraseSoundWnd::OnEraseBkgnd(CDC* pDC)
{
	//if(!(m_ImageBack.IsNull))
	//{
	   //m_ImageBack.AlphaDrawImage(pDC,0,0,RGB(255,0,255));
	//}
	return CWnd::OnEraseBkgnd(pDC);
}

int CPhraseSoundWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	//��һҳ��ť
	CRect  PageUpRect(30, 254, 25+205/5, 254+18);
	HINSTANCE hInstance=AfxGetInstanceHandle();
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btPageUp.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,PageUpRect,this,IDC_PAGE_UP);
	m_btPageUp.SetButtonImage(IDB_PAGE_UP,hInstance,false);
	m_btPageUp.EnableWindow(false);
	m_btPageUp.ShowWindow(SW_SHOW);

	//��һҳ��ť
	CRect  PageDownRect(115, 254, 25+205/5, 254+18);
	m_btPageDown.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,PageDownRect,this,IDC_PAGE_DOWN);
	m_btPageDown.SetButtonImage(IDB_PAGE_DOWN,hInstance,false);
	m_btPageDown.EnableWindow(false);
	m_btPageDown.ShowWindow(SW_SHOW);

	//�������
	//����
	AddPhraseSoundItem(_T("���Щ"),1);
	AddPhraseSoundItem(_T("�������"),1);
	AddPhraseSoundItem(_T("��㰡���ϰ���"),1);
	AddPhraseSoundItem(_T("��㰡�ϰ���"),1);
	AddPhraseSoundItem(_T("��㰡����Ҫ������"),1);
	AddPhraseSoundItem(_T("��㰡����Ҫȥ�����"),1);
	AddPhraseSoundItem(_T("������ư����춼Ҫ����"),1);
	AddPhraseSoundItem(_T("��㰡���һ�Ҫȥ�����"),1);
	AddPhraseSoundItem(_T("�����������°� ��Ҫ�˷�ʱ��"),1);

	//Ů��
	AddPhraseSoundItem(_T("�������"),2);	
	AddPhraseSoundItem(_T("��㰡����Ů"),2);
	AddPhraseSoundItem(_T("��㰡���һ����µ�"),2);
	AddPhraseSoundItem(_T("������������춼Ҫ����"),2);
	AddPhraseSoundItem(_T("����㣬Ҫ���ڴ���"),2);
	AddPhraseSoundItem(_T("�������������������ȱһ��Ҫ�˷�ʱ��"),2);


	SetWindowPos(NULL,0,0,200,276,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);

	//��������
	AdjustItemDisplayRect();

	return 0;
}

void CPhraseSoundWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	tagPhraseSoundItem * pPhraseSoundItem;
	if((pPhraseSoundItem=TransPointToPhraseSoundItem(point))!=NULL)
	{
		AfxGetMainWnd()->SendMessage(WM_USER+110,m_cbCurrSelected,0);
		ShowWindow(FALSE);
	}
	//CWnd::OnLButtonDblClk(nFlags, point);
}
