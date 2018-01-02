#include "Stdafx.h"
#include "ShowCard.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CShowCard, CDialog)
	ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CShowCard::CShowCard() : CDialog(IDD_DLG_SHOW_CARD)
{
	//加载位图
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCard.LoadImage(hInstance, _T("GAME_CARD"));

	m_nWidth = m_ImageCard.GetWidth()/43;
	m_nHeight= m_ImageCard.GetHeight();
	m_cbCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	ZeroMemory(m_rcCard,sizeof(m_rcCard));
	ZeroMemory(m_bSelectCard,sizeof(m_bSelectCard));

	m_cbSelCardCount=0;
	ZeroMemory(m_cbSelCardData,sizeof(m_cbSelCardData));

	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_rcHandCard,sizeof(m_rcHandCard));
	ZeroMemory(m_bHandSelectCard,sizeof(m_bHandSelectCard));

	m_cbHandSelCardCount=0;
	ZeroMemory(m_cbHandSelCardData,sizeof(m_cbHandSelCardData));

	//m_cbCardCount=20;
	//for (int i =0;i<m_cbCardCount;i+=2)
	//{
	//	m_cbRepertoryCard[i] = 0x01;
	//	m_cbRepertoryCard[i+1] = 0x0a;
	//}
	////生成区域
	//for (BYTE i = 0; i<m_cbCardCount; i++)
	//{
	//	m_rcCard[i] = CRect((i%15)*m_nWidth,(i/15)*m_nHeight,(i%15)*m_nWidth+m_nWidth,(i/15)*m_nHeight+m_nHeight);
	//}

	//m_cbHandCardCount=14;
	//for (int i =0;i<m_cbHandCardCount;i+=2)
	//{
	//	m_cbHandCardData[i] = 0x01;
	//	m_cbHandCardData[i+1] = 0x0a;
	//}
	////生成区域
	//for (BYTE i = 0; i<m_cbHandCardCount; i++)
	//{
	//	m_rcHandCard[i] = CRect((i%15)*m_nWidth,340+(i/15)*m_nHeight,(i%15)*m_nWidth+m_nWidth,340+(i/15)*m_nHeight+m_nHeight);
	//}
	return;
}

//析构函数
CShowCard::~CShowCard()
{
}

//控件绑定
void CShowCard::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//初始化函数
BOOL CShowCard::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("配牌设置"));

	return TRUE;
}

//确定消息
void CShowCard::OnOK()
{
	if (m_cbSelCardCount != m_cbHandSelCardCount)
	{
		return;
	}
	CMD_C_AdminReq AdminReq;
	AdminReq.cbReqType=RQ_SET_CARD_DATA;
	AdminReq.cbSelectCount=m_cbSelCardCount;
	CopyMemory(AdminReq.cbHeapSelectData,m_cbSelCardData,sizeof(AdminReq.cbHeapSelectData));
	CopyMemory(AdminReq.cbHandSelectData,m_cbHandSelCardData,sizeof(AdminReq.cbHandSelectData));
	m_pClientEngine->SendSocketData(SUB_C_AMDIN_COMMAND, &AdminReq, sizeof(CMD_C_AdminReq));

	ShowWindow(SW_HIDE);
	m_cbCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	ZeroMemory(m_rcCard,sizeof(m_rcCard));
	ZeroMemory(m_bSelectCard,sizeof(m_bSelectCard));

	m_cbSelCardCount=0;
	ZeroMemory(m_cbSelCardData,sizeof(m_cbSelCardData));

	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_rcHandCard,sizeof(m_rcHandCard));
	ZeroMemory(m_bHandSelectCard,sizeof(m_bHandSelectCard));

	m_cbHandSelCardCount=0;
	ZeroMemory(m_cbHandSelCardData,sizeof(m_cbHandSelCardData));
}

//取消消息
void CShowCard::OnCancel()
{
	ShowWindow(SW_HIDE);
	m_cbCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	ZeroMemory(m_rcCard,sizeof(m_rcCard));
	ZeroMemory(m_bSelectCard,sizeof(m_bSelectCard));

	m_cbSelCardCount=0;
	ZeroMemory(m_cbSelCardData,sizeof(m_cbSelCardData));

	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_rcHandCard,sizeof(m_rcHandCard));
	ZeroMemory(m_bHandSelectCard,sizeof(m_bHandSelectCard));

	m_cbHandSelCardCount=0;
	ZeroMemory(m_cbHandSelCardData,sizeof(m_cbHandSelCardData));
}

void CShowCard::OnPaint()
{
	CClientDC dc(this);
	for (BYTE i = 0; i<m_cbCardCount; i++)
	{
		BYTE cbCardData = m_cbRepertoryCard[i];

		//计算位置
		BYTE cbValue=cbCardData&0X0F;
		BYTE cbColor=(cbCardData&0XF0)>>4;
		BYTE cbPos  =(cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);

		m_ImageCard.DrawImage(&dc,(i%15)*m_nWidth,(i/15)*m_nHeight,m_nWidth,m_nHeight,m_nWidth*cbPos,0,m_nWidth,m_nHeight);
		dc.SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
		dc.Rectangle(m_rcCard[i]);
	}

	//选中的牌
	for (BYTE i = 0 ;i<m_cbSelCardCount; i ++)
	{
		BYTE cbCardData = m_cbSelCardData[i];

		//计算位置
		BYTE cbValue=cbCardData&0X0F;
		BYTE cbColor=(cbCardData&0XF0)>>4;
		BYTE cbPos  =(cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);

		m_ImageCard.DrawImage(&dc,(i%15)*m_nWidth,250+(i/15)*m_nHeight,m_nWidth,m_nHeight,m_nWidth*cbPos,0,m_nWidth,m_nHeight);
	}

	//手中的牌
	for (BYTE i = 0 ;i<m_cbHandCardCount; i ++)
	{
		BYTE cbCardData = m_cbHandCardData[i];

		//计算位置
		BYTE cbValue=cbCardData&0X0F;
		BYTE cbColor=(cbCardData&0XF0)>>4;
		BYTE cbPos  =(cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);

		m_ImageCard.DrawImage(&dc,(i%15)*m_nWidth,340+(i/15)*m_nHeight,m_nWidth,m_nHeight,m_nWidth*cbPos,0,m_nWidth,m_nHeight);
		dc.SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
		dc.Rectangle(m_rcHandCard[i]);
	}
	
	//选中的手牌
	for (BYTE i = 0 ;i<m_cbHandSelCardCount; i ++)
	{
		BYTE cbCardData = m_cbHandSelCardData[i];

		//计算位置
		BYTE cbValue=cbCardData&0X0F;
		BYTE cbColor=(cbCardData&0XF0)>>4;
		BYTE cbPos  =(cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);

		m_ImageCard.DrawImage(&dc,(i%15)*m_nWidth,430+(i/15)*m_nHeight,m_nWidth,m_nHeight,m_nWidth*cbPos,0,m_nWidth,m_nHeight);
	}
}

//擦除背景
BOOL CShowCard::OnEraseBkgnd(CDC* pDC)
{
	Invalidate(FALSE);
	return FALSE;
}

//鼠标消息
void CShowCard::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);
	
	for (BYTE i = 0; i<m_cbCardCount; i++)
	{
		if (m_bSelectCard[i]==false&&m_rcCard[i].PtInRect(Point))
		{
			m_bSelectCard[i]=true;
			m_cbSelCardData[m_cbSelCardCount++]=m_cbRepertoryCard[i];

			Invalidate(FALSE);
		}
	}

	//手牌判断
	for (BYTE i = 0; i<m_cbHandCardCount; i++)
	{
		if (m_bHandSelectCard[i]==false&&m_rcHandCard[i].PtInRect(Point))
		{
			m_bHandSelectCard[i]=true;
			m_cbHandSelCardData[m_cbHandSelCardCount++]=m_cbHandCardData[i];

			Invalidate(FALSE);
		}
	}

	return;
}

//申请结果
bool __cdecl CShowCard::ReqResult(const void * pBuffer,CGameClientEngine* pClient)
{
	//消息处理
	CMD_S_CommandResult * pResultCard=(CMD_S_CommandResult *)pBuffer;

	m_pClientEngine = pClient;

	CopyMemory(m_cbRepertoryCard,pResultCard->cbRepertoryCard,sizeof(pResultCard->cbRepertoryCard));
	m_cbCardCount = pResultCard->cbRepertoryCardCount;

	//生成区域
	for (BYTE i = 0; i<m_cbCardCount; i++)
	{
		m_rcCard[i] = CRect((i%15)*m_nWidth,(i/15)*m_nHeight,(i%15)*m_nWidth+m_nWidth,(i/15)*m_nHeight+m_nHeight);
	}
	Invalidate(FALSE);
	return true;
}

//设置数据
void __cdecl CShowCard::SetLeftCardCount(BYTE cbRepertoryCardCount)
{
	return;
}

//设置手牌
void __cdecl CShowCard::SetHandCardData(BYTE cbHandCardData[], BYTE cbCardCount, BYTE cbCurrentCard)
{
	CopyMemory(m_cbHandCardData,cbHandCardData,sizeof(m_cbHandCardData));
	m_cbHandCardCount=cbCardCount;

	//生成区域
	for (BYTE i = 0; i<m_cbHandCardCount; i++)
	{
		m_rcHandCard[i] = CRect((i%15)*m_nWidth,340+(i/15)*m_nHeight,(i%15)*m_nWidth+m_nWidth,340+(i/15)*m_nHeight+m_nHeight);
	}
	Invalidate(FALSE);
	return;
}

//设置当前
void __cdecl CShowCard::SetCurrentCard(BYTE cbCurrentCard)
{
	return;
}

//////////////////////////////////////////////////////////////////////////
