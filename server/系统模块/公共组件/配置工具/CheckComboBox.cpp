#include "stdafx.h"
#include "CheckComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////////////

//��̬����
static WNDPROC m_pWndProc = 0;
static CCheckComboBox *m_pComboBox = 0;

///////////////////////////////////////////////////////////////////////////////////////

//��Ϣӳ��
BEGIN_MESSAGE_MAP(CCheckComboBox, CComboBox)	
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)	
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////////////
//
// The subclassed COMBOLBOX message handler
//
extern "C" LRESULT FAR PASCAL ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

	switch (nMsg) 
	{		
		case WM_RBUTTONDOWN: 
			{
				if (m_pComboBox != 0) 
				{
					INT nCount = m_pComboBox->GetCount();
					INT nSelCount = 0;

					for (INT i = 0; i < nCount; i++) 
					{
						if (m_pComboBox->GetCheck(i))
							nSelCount++;
					}

					m_pComboBox->SelectAll(nSelCount != nCount);

					InvalidateRect(hWnd, 0, FALSE);
					m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);
				}

			break;
		}
		case LB_GETCURSEL: 
			{
				return -1;
			}

		case WM_CHAR: 
			{
				if (wParam == VK_SPACE) 
				{
					// Get the current selection
					INT nIndex = CallWindowProcA(m_pWndProc, hWnd, LB_GETCURSEL, wParam, lParam);

					CRect rcItem;
					SendMessage(hWnd, LB_GETITEMRECT, nIndex, (LONG)(VOID *)&rcItem);
					InvalidateRect(hWnd, rcItem, FALSE);

					// Invert the check mark
					m_pComboBox->SetCheck(nIndex, !m_pComboBox->GetCheck(nIndex));

					// Notify that selection has changed
					m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);
					return 0;
				}
				break;
			}
		case WM_LBUTTONDOWN: 
			{
				CRect rcClient;
				GetClientRect(hWnd, rcClient);

				CPoint pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);

				if (PtInRect(rcClient, pt)) 
				{
					INT nItemHeight = SendMessage(hWnd, LB_GETITEMHEIGHT, 0, 0);
					INT nTopIndex   = SendMessage(hWnd, LB_GETTOPINDEX, 0, 0);

					INT nIndex = nTopIndex + pt.y / nItemHeight;

					CRect rcItem;
					SendMessage(hWnd, LB_GETITEMRECT, nIndex, (LONG)(VOID *)&rcItem);

					if (PtInRect(rcItem, pt)) 
					{
						InvalidateRect(hWnd, rcItem, FALSE);
						m_pComboBox->SetCheck(nIndex, !m_pComboBox->GetCheck(nIndex));

						m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);
					}
				}
				break;
			}
		case WM_LBUTTONUP: 
			{			
				return 0;
			}
		}

	return CallWindowProc(m_pWndProc, hWnd, nMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////
//���캯��
CCheckComboBox::CCheckComboBox()
{
	//���ñ���
	m_hListBox= 0;
	m_bTextUpdated= FALSE;
	m_bItemHeightSet= FALSE;
}

//��������
CCheckComboBox::~CCheckComboBox()
{
}

//��������
BOOL CCheckComboBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	//������ʽ
	dwStyle &= ~0xF;
	dwStyle |= CBS_DROPDOWNLIST;

	// Make sure to use the CBS_OWNERDRAWVARIABLE style
	dwStyle |= CBS_OWNERDRAWVARIABLE;

	// Use default strings. We need the itemdata to store checkmarks
	dwStyle |= CBS_HASSTRINGS;

	return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}

//�ؼ���ɫ
LRESULT CCheckComboBox::OnCtlColorListBox(WPARAM wParam, LPARAM lParam) 
{	
	if (m_hListBox == 0) 
	{
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd) 
		{			
			m_hListBox = hWnd;

			//���໯�ؼ�
			m_pWndProc = (WNDPROC)GetWindowLong(m_hListBox, GWL_WNDPROC);
			SetWindowLong(m_hListBox, GWL_WNDPROC, (LONG)ComboBoxListBoxProc);
		}
	}
	
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

//��������
void CCheckComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	HDC dc = lpDrawItemStruct->hDC;

	CRect rcBitmap = lpDrawItemStruct->rcItem;
	CRect rcText   = lpDrawItemStruct->rcItem;

	CString strText;

	// 0 - No check, 1 - Empty check, 2 - Checked
	INT nCheck = 0;

	// Check if we are drawing the static portion of the combobox
	if ((LONG)lpDrawItemStruct->itemID < 0) 
	{		
		RecalcText();
		strText = m_strText;
		nCheck = 0;
	}
	else 
	{
		GetLBText(lpDrawItemStruct->itemID, strText);
		nCheck = 1 + (GetItemData(lpDrawItemStruct->itemID) != 0);

		TEXTMETRIC metrics;
		GetTextMetrics(dc, &metrics);

		rcBitmap.left    = 0;
		rcBitmap.right   = rcBitmap.left + metrics.tmHeight + metrics.tmExternalLeading + 6;
		rcBitmap.top    += 1;
		rcBitmap.bottom -= 1;

		rcText.left = rcBitmap.right;
	}
	 

	if (nCheck > 0) 
	{
		SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));

		UINT nState = DFCS_BUTTONCHECK;

		if (nCheck > 1) nState |= DFCS_CHECKED;

		DrawFrameControl(dc, rcBitmap, DFC_BUTTON, nState);
	}

	if (lpDrawItemStruct->itemState & ODS_SELECTED) 
	{
		SetBkColor(dc, GetSysColor(COLOR_HIGHLIGHT));
		SetTextColor(dc, GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else 
	{
		SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));
	}

	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rcText, 0, 0, 0);
	DrawText(dc, ' ' + strText, strText.GetLength() + 1, &rcText, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	if ((lpDrawItemStruct->itemState & (ODS_FOCUS|ODS_SELECTED)) == (ODS_FOCUS|ODS_SELECTED))
	{
		DrawFocusRect(dc, &rcText);
	}	
}

//��������
void CCheckComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	CClientDC dc(this);
	CFont *pFont = dc.SelectObject(GetFont());

	if (pFont != 0) 
	{
		TEXTMETRIC metrics;
		dc.GetTextMetrics(&metrics);

		lpMeasureItemStruct->itemHeight = metrics.tmHeight + metrics.tmExternalLeading;

		lpMeasureItemStruct->itemHeight += 2;

		if (!m_bItemHeightSet) 
		{
			m_bItemHeightSet = TRUE;
			SetItemHeight(-1, lpMeasureItemStruct->itemHeight);
		}

		dc.SelectObject(pFont);
	}
}

//������Ϣ
void CCheckComboBox::OnDropDown() 
{
	m_pComboBox = this;	
}

//ѡ��ȫ��
void CCheckComboBox::SelectAll(BOOL bCheck)
{
	INT nCount = GetCount();

	for (INT i = 0; i < nCount; i++)
		SetCheck(i, bCheck);

}

//��ȡ�ı�
LRESULT CCheckComboBox::OnGetText(WPARAM wParam, LPARAM lParam)
{
	// Make sure the text is updated
	RecalcText();

	if (lParam == 0)
		return 0;

	// Copy the 'fake' window text
	lstrcpyn((LPTSTR)lParam, m_strText, (INT)wParam);
	return m_strText.GetLength();
}

//��ȡ�ı�����
LRESULT CCheckComboBox::OnGetTextLength(WPARAM, LPARAM)
{
	// Make sure the text is updated
	RecalcText();
	return m_strText.GetLength();
}

//�����ı�
void CCheckComboBox::RecalcText()
{
	if (!m_bTextUpdated) 
	{
		CString strText;
		
		INT nCount=GetCount();

		TCHAR szBuffer[10] = {0};
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, szBuffer, sizeof(szBuffer));

		CString strSeparator = szBuffer;

		if (strSeparator.GetLength() == 0)
		{
			strSeparator = ';';
		}

		strSeparator.TrimRight();
		strSeparator += ' ';

		for (INT i = 0; i < nCount; i++) 
		{
			if (GetItemData(i)) 
			{
				CString strItem;
				GetLBText(i, strItem);

				if (!strText.IsEmpty())
					strText += strSeparator;

				strText += strItem;
			}
		}

		m_strText = strText;

		m_bTextUpdated = TRUE;
	}
}

//����ѡ��
INT CCheckComboBox::SetCheck(INT nIndex, BOOL bFlag)
{
	INT nResult = SetItemData(nIndex, bFlag);

	if (nResult < 0)
		return nResult;

	m_bTextUpdated = FALSE;
	RecalcText();
	Invalidate(FALSE);	

	return nResult;
}

//��ȡѡ��
BOOL CCheckComboBox::GetCheck(INT nIndex)
{
	return (BOOL)GetItemData(nIndex);
}
///////////////////////////////////////////////////////////////////////////////////////
