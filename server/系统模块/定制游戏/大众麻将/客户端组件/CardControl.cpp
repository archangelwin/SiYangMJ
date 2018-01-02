#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//公共定义
#define POS_SHOOT					5									//弹起象素
#define POS_SPACE					8									//分隔间隔
#define ITEM_COUNT					43									//子项数目
#define INVALID_ITEM				0xFFFF								//无效索引

//扑克大小
#define CARD_WIDTH					51									//扑克宽度
#define CARD_HEIGHT					81									//扑克高度

//////////////////////////////////////////////////////////////////////////

//构造函数
CCardListImage::CCardListImage()
{
	//位置变量
	m_nItemWidth=0;
	m_nItemHeight=0;

	return;
}

//析构函数
CCardListImage::~CCardListImage()
{
}

//加载资源
bool CCardListImage::LoadResource(CD3DDevice * pD3DDevice,HINSTANCE hInstance,LPCTSTR pszResourceCard,LPCTSTR pszResourceShadow)
{
	//加载资源
	m_CardListImage.LoadImage(pD3DDevice, pszResourceCard,FALSE);
	m_ImageCardShadow.LoadImage( pD3DDevice,pszResourceShadow,FALSE);

	//设置变量
	m_nItemHeight=m_CardListImage.GetHeight();
	m_nItemWidth=m_CardListImage.GetWidth()/ITEM_COUNT;

	return true;
}

//释放资源
bool CCardListImage::DestroyResource()
{
	//设置变量
	m_nItemWidth=0;
	m_nItemHeight=0;

	//释放资源
	m_CardListImage.Destory();
	m_ImageCardShadow.Destory();

	return true;
}

//获取位置
int CCardListImage::GetImageIndex(BYTE cbCardData)
{
	//背景判断
	if (cbCardData==0) 
		return 0;

	//计算位置
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}

//绘画扑克
bool CCardListImage::DrawCardItem(CD3DDevice * pD3DDevice, BYTE cbCardData, int xDest, int yDest, bool bShadow)
{
	//效验状态
	ASSERT(m_CardListImage.IsNull()==false);
	ASSERT((m_nItemWidth!=0)&&(m_nItemHeight!=0));

	//绘画子项
	if(cbCardData<=0x3F)
	{
		int nImageXPos=GetImageIndex(cbCardData)*m_nItemWidth;
		//绘画阴影
		/*if( bShadow )
			m_ImageCardShadow.DrawImage( pDestDC,xDest,yDest );*/
		m_CardListImage.DrawImage( pD3DDevice,xDest,yDest,m_nItemWidth,m_nItemHeight,nImageXPos,0 );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CCardResource::CCardResource()
{
}

//析构函数
CCardResource::~CCardResource()
{
}

//加载资源
bool CCardResource::LoadResource(CD3DDevice* pD3DDevice)
{
	//变量定义
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//用户扑克
	m_ImageUserTop.LoadImage( pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/CARD_USER_TOP.png") ,FALSE);
	m_ImageUserLeft.LoadImage( pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/CARD_USER_LEFT.png"),FALSE );
	m_ImageUserRight.LoadImage(  pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/CARD_USER_RIGHT.png"),FALSE );
	m_ImageUserBottom.LoadResource(  pD3DDevice,hInstance,TEXT("SubGame/SparrowDZ/clientview/CARD_USER_BOTTOM.png"),TEXT("SubGame/SparrowDZ/clientview/USER_BOTTOM_SHADOW.png") );
	m_ImageUserBtGrayed.LoadResource( pD3DDevice, hInstance,TEXT("SubGame/SparrowDZ/clientview/CARD_USER_BOTTOM_GRAYED.png"),TEXT("SubGame/SparrowDZ/clientview/USER_BOTTOM_SHADOW.png") );

	//桌子扑克
	m_ImageTableTop.LoadResource(  pD3DDevice,hInstance,TEXT("SubGame/SparrowDZ/clientview/CARD_TABLE_TOP.png"),TEXT("SubGame/SparrowDZ/clientview/TABLE_TOP_SHADOW.png") );
	m_ImageTableLeft.LoadResource( pD3DDevice, hInstance,TEXT("SubGame/SparrowDZ/clientview/CARD_TABLE_LEFT.png"),TEXT("SubGame/SparrowDZ/clientview/TABLE_LEFT_SHADOW.png"));
	m_ImageTableRight.LoadResource( pD3DDevice, hInstance,TEXT("SubGame/SparrowDZ/clientview/CARD_TABLE_RIGHT.png"),TEXT("SubGame/SparrowDZ/clientview/TABLE_RIGHT_SHADOW.png") );
	m_ImageTableBottom.LoadResource( pD3DDevice, hInstance,TEXT("SubGame/SparrowDZ/clientview/CARD_TABLE_BOTTOM.png"),TEXT("SubGame/SparrowDZ/clientview/TABLE_BOTTOM_SHADOW.png") );
	m_ImageDisCard.LoadResource(pD3DDevice, hInstance,TEXT("SubGame/SparrowDZ/clientview/CARD_DISCARD.png"),TEXT("SubGame/SparrowDZ/clientview/TABLE_BOTTOM_SHADOW.png"));

	//牌堆扑克
	m_ImageHeapSingleV.LoadImage( pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/CARD_HEAP_SINGLE_V.png"),FALSE);
	m_ImageHeapSingleH.LoadImage( pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/CARD_HEAP_SINGLE_H.png"),FALSE);
	m_ImageHeapDoubleV.LoadImage( pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/CARD_HEAP_DOUBLE_V.png"),FALSE);
	m_ImageHeapDoubleH.LoadImage( pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/CARD_HEAP_DOUBLE_H.png"),FALSE);

	//扑克阴影
	m_ImageUserTopShadow.LoadImage(  pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/USER_TOP_SHADOW.png"),FALSE);
	m_ImageUserLeftShadow.LoadImage( pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/USER_LEFT_SHADOW.png"),FALSE);
	m_ImageUserRightShadow.LoadImage( pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/USER_RIGHT_SHADOW.png"),FALSE);
	m_ImageHeapSingleVShadow.LoadImage(  pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/HEAP_SINGLE_V_SHADOW.png"),FALSE);
	m_ImageHeapSingleHShadow.LoadImage(  pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/HEAP_SINGLE_H_SHADOW.png"),FALSE);
	m_ImageHeapDoubleHShadow.LoadImage(  pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/HEAP_DOUBLE_H_SHADOW.png"),FALSE);
	m_ImageHeapDoubleVShadow.LoadImage(  pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/HEAP_DOUBLE_V_SHADOW.png"),FALSE);

	return true;
}

//消耗资源
bool CCardResource::DestroyResource()
{
	//用户扑克
	m_ImageUserTop.Destory();
	m_ImageUserLeft.Destory();
	m_ImageUserRight.Destory();
	m_ImageUserBottom.DestroyResource();

	//桌子扑克
	m_ImageTableTop.DestroyResource();
	m_ImageTableLeft.DestroyResource();
	m_ImageTableRight.DestroyResource();
	m_ImageTableBottom.DestroyResource();

	//牌堆扑克
	m_ImageHeapSingleV.Destory();
	m_ImageHeapSingleH.Destory();
	m_ImageHeapDoubleV.Destory();
	m_ImageHeapDoubleH.Destory();

	//扑克阴影
	m_ImageUserTopShadow.Destory();
	m_ImageUserLeftShadow.Destory();
	m_ImageUserRightShadow.Destory();
	m_ImageHeapSingleVShadow.Destory();
	m_ImageHeapSingleHShadow.Destory();
	m_ImageHeapDoubleHShadow.Destory();
	m_ImageHeapDoubleVShadow.Destory();

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CHeapCard::CHeapCard()
{
	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//扑克变量
	m_wFullCount=HEAP_FULL_COUNT;
	m_wMinusHeadCount=0;
	m_wMinusLastCount=0;
	ZeroMemory( m_cbCardData,sizeof(m_cbCardData) );

	return;
}

//析构函数
CHeapCard::~CHeapCard()
{
}

//绘画扑克
void CHeapCard::DrawCardControl(CD3DDevice* pD3DDevice)
{
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//绘画扑克
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//变量定义
				int nXPos=0,nYPos=0;
				int nYExcusion = Y_HEAP_DOUBLE_V_EXCUSION;
				int nCardHeight = g_CardResource.m_ImageHeapDoubleV.GetHeight()-nYExcusion;

				//尾部扑克
				nXPos = m_ControlPoint.x;
				nYPos = m_ControlPoint.y-(m_wFullCount/2*nYExcusion+nCardHeight)+m_wMinusLastCount/2*nYExcusion;
				WORD wCardIndex = (m_wMinusLastCount/2)*2;
				if (m_wMinusLastCount%2!=0)
				{
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+10,true );
					}
					else
					{
						//g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos+10 );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos+10 );
					}
					wCardIndex += 2;
					nYPos += nYExcusion;
				}

				//中间扑克
				WORD wFinallyIndex=(m_wFullCount-m_wMinusHeadCount)/2;
				WORD wDoubleIndex = (m_wMinusLastCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					if( m_cbCardData[wCardIndex+1] != 255 || m_cbCardData[wCardIndex] != 255 )
						//g_CardResource.m_ImageHeapDoubleVShadow.DrawImage( pDC,nXPos,nYPos );
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 || m_cbCardData[wCardIndex] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
						{
							if( m_cbCardData[wCardIndex+1] != 255 )
								g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+10,false );
						}
						else g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos+10 );
						if( m_cbCardData[wCardIndex] != 0 )
						{
							if( m_cbCardData[wCardIndex] != 255 )
								g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						}
						else g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos );
					}
					else g_CardResource.m_ImageHeapDoubleV.DrawImage( pD3DDevice,nXPos,nYPos );
					wCardIndex += 2;
					nYPos += nYExcusion;
				}
				
				//头部扑克
				if (m_wMinusHeadCount%2!=0)
				{
					nYPos += 10;
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos,true );
					}
					else
					{
						//g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos );
					}
					if( wDoubleIndex < wFinallyIndex && m_cbCardData[wCardIndex+1] != 255 && m_cbCardData[wCardIndex-2] != 255 )
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleVShadow.GetWidth();
						int nShadowHeight = g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-g_CardResource.m_ImageHeapSingleV.GetHeight();
						/*g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos,nShadowWidth,nShadowHeight,
							0,g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-nShadowHeight );*/
					}
				}
			}
			break;
		}
	case Direction_South:	//南向
		{
			//绘画扑克
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//变量定义
				int nXPos=0,nYPos=0;

				//尾部扑克
				int nXExcusion = g_CardResource.m_ImageHeapDoubleH.GetWidth();
				int nYExcusion = 9;
				nYPos = m_ControlPoint.y;
				nXPos = m_ControlPoint.x-m_wFullCount/2*nXExcusion+m_wMinusLastCount/2*nXExcusion;
				WORD wCardIndex = (m_wMinusLastCount/2)*2;
				if (m_wMinusLastCount%2!=0)
				{
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableBottom.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+nYExcusion,true );
					}
					else
					{
						//g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos+nYExcusion );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos+nYExcusion );
					}
					wCardIndex += 2;
					nXPos += nXExcusion;
				}

				//中间扑克
				WORD wFinallyIndex=(m_wFullCount-m_wMinusHeadCount)/2;
				WORD wDoubleIndex = (m_wMinusLastCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					if( m_cbCardData[wCardIndex+1] != 255 || m_cbCardData[wCardIndex] != 255 )
						//g_CardResource.m_ImageHeapDoubleHShadow.DrawImage( pDC,nXPos,nYPos );
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 || m_cbCardData[wCardIndex] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
						{
							if( m_cbCardData[wCardIndex+1] != 255 )
								g_CardResource.m_ImageTableBottom.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+nYExcusion,false );
						}
						else g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos+nYExcusion );
						if( m_cbCardData[wCardIndex] != 0 )
						{
							if( m_cbCardData[wCardIndex] != 255 )
								g_CardResource.m_ImageTableBottom.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						}
						else g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos );
					}
					else g_CardResource.m_ImageHeapDoubleH.DrawImage( pD3DDevice,nXPos,nYPos );
					wCardIndex += 2;
					nXPos += nXExcusion;
				}

				//头部扑克
				if (m_wMinusHeadCount%2!=0)
				{
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableBottom.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+nYExcusion,true );
					}
					else
					{
						//g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos+nYExcusion );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos+nYExcusion );
					}
					if( wDoubleIndex < wFinallyIndex && m_cbCardData[wCardIndex+1] != 255 && m_cbCardData[wCardIndex-2] != 255)
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nXExcusion;
						int nShadowHeight = g_CardResource.m_ImageHeapSingleHShadow.GetHeight();
						/*g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos,nShadowWidth,nShadowHeight,
							g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nShadowWidth,0 );*/ 
					}
				}
			}
			
			break;
		}
	case Direction_West:	//西向
		{
			//绘画扑克
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//变量定义
				int nXPos=0,nYPos=0;

				//头部扑克
				int nYExcusion = Y_HEAP_DOUBLE_V_EXCUSION;
				nXPos = m_ControlPoint.x;
				nYPos = m_ControlPoint.y+m_wMinusHeadCount/2*nYExcusion;
				WORD wCardIndex = m_wFullCount-(m_wMinusHeadCount/2)*2-2;
				if (m_wMinusHeadCount%2!=0)
				{
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+10,true );
					}
					else
					{
						//g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos+10 );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos+10 );
					}
					wCardIndex -= 2;
					nYPos += nYExcusion;
				}

				//中间扑克
				WORD wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;
				WORD wDoubleIndex = (m_wMinusHeadCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					if( m_cbCardData[wCardIndex+1] != 255 || m_cbCardData[wCardIndex] != 255 )
						//g_CardResource.m_ImageHeapDoubleVShadow.DrawImage( pDC,nXPos,nYPos );
					//如果有翻牌
					if( m_cbCardData[wCardIndex] != 0 || m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
						{
							if( m_cbCardData[wCardIndex+1] != 255 )
								g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+10,false );
						}
						else g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos+10 );
						if( m_cbCardData[wCardIndex] != 0 )
						{
							if( m_cbCardData[wCardIndex] != 255 )
								g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						}
						else g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos );
					}
					else g_CardResource.m_ImageHeapDoubleV.DrawImage( pD3DDevice,nXPos,nYPos );
					nYPos += nYExcusion;
					wCardIndex -= 2;
				}

				//尾部扑克
				if (m_wMinusLastCount%2!=0)
				{
					nYPos += 10;
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos,true );
					}
					else
					{
						//g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pD3DDevice,nXPos,nYPos );
					}
					if( wDoubleIndex < wFinallyIndex && m_cbCardData[wCardIndex+1] != 255 && m_cbCardData[wCardIndex+2] != 255 )
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleVShadow.GetWidth();
						int nShadowHeight = g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-g_CardResource.m_ImageHeapSingleV.GetHeight();
						/*g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos,nShadowWidth,nShadowHeight,
							0,g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-nShadowHeight );*/
					}
				}
			}
			
			break;
		}
	case Direction_North:	//北向
		{
			//绘画扑克
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//变量定义
				int nXPos=0,nYPos=0;

				//头部扑克
				int nXExcusion = g_CardResource.m_ImageHeapDoubleH.GetWidth();
				int nYExcusion = 9;
				nYPos = m_ControlPoint.y;
				nXPos = m_ControlPoint.x+m_wMinusHeadCount/2*nXExcusion;
				WORD wCardIndex = m_wFullCount-(m_wMinusHeadCount/2)*2-2;
				if (m_wMinusHeadCount%2!=0)
				{
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableTop.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+nYExcusion,true );
					}
					else
					{
						g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pD3DDevice,nXPos,nYPos+nYExcusion );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos+nYExcusion );
					}
					wCardIndex -= 2;
					nXPos += nXExcusion;
				}

				//中间扑克
				WORD wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;
				WORD wDoubleIndex = (m_wMinusHeadCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					if( m_cbCardData[wCardIndex+1] != 255 || m_cbCardData[wCardIndex] != 255 )
						//g_CardResource.m_ImageHeapDoubleHShadow.DrawImage( pDC,nXPos,nYPos );
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 || m_cbCardData[wCardIndex] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
						{
							if( m_cbCardData[wCardIndex+1] != 255 )
								g_CardResource.m_ImageTableTop.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+nYExcusion,false );
						}
						else g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos+nYExcusion );
						if( m_cbCardData[wCardIndex] != 0 )
						{
							if( m_cbCardData[wCardIndex] != 255 )
								g_CardResource.m_ImageTableTop.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						}
						else g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos );
					}
					else g_CardResource.m_ImageHeapDoubleH.DrawImage( pD3DDevice,nXPos,nYPos );
					wCardIndex -= 2;
					nXPos += nXExcusion;
				}

				//尾部扑克
				if (m_wMinusLastCount%2!=0)
				{
					//如果有翻牌
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 255 )
							g_CardResource.m_ImageTableTop.DrawCardItem( pD3DDevice,m_cbCardData[wCardIndex+1],nXPos,nYPos+nYExcusion,true );
					}
					else
					{
						//g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos+nYExcusion );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pD3DDevice,nXPos,nYPos+nYExcusion );
					}
					if( wDoubleIndex < wFinallyIndex && m_cbCardData[wCardIndex+1] != 255 && m_cbCardData[wCardIndex+2] != 255 )
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nXExcusion;
						int nShadowHeight = g_CardResource.m_ImageHeapSingleHShadow.GetHeight();
						/*g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos,nShadowWidth,nShadowHeight,
							g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nShadowWidth,0 );*/
					}
				}
			}
			
			break;
		}
	}

	return;
}

//设置扑克
bool CHeapCard::SetHeapCardInfo(WORD wMinusHeadCount, WORD wMinusLastCount)
{
	ASSERT( wMinusHeadCount+wMinusLastCount<=m_wFullCount );
	if( wMinusHeadCount + wMinusLastCount > m_wFullCount ) return false;

	//设置变量
	m_wMinusHeadCount=wMinusHeadCount;
	m_wMinusLastCount=wMinusLastCount;

	return true;
}

//设置翻牌,wIndex从牌尾的上面那只牌算起
void CHeapCard::SetCardData( WORD wIndex, BYTE cbCardData )
{
	ASSERT( wIndex < m_wFullCount );
	if( wIndex >= m_wFullCount ) return;

	m_cbCardData[wIndex] = cbCardData;
}

//重置翻牌
void CHeapCard::ResetCardData()
{
	ZeroMemory( m_cbCardData,sizeof(m_cbCardData) );
	return ;
}

//设置牌为空,wIndex从牌尾的上面那只算起
void CHeapCard::SetCardEmpty( WORD wIndex, BYTE cbEmptyCount )
{
	ASSERT( wIndex+1 >= cbEmptyCount );
	if( wIndex+1 < cbEmptyCount ) return;

	for( int i = wIndex; i >= wIndex+1-cbEmptyCount; i-- )
		m_cbCardData[i] = 255;
}

//获取出牌位置
CPoint CHeapCard::GetDispatchCardPos( bool bTail )
{
	CPoint ptHead = m_ControlPoint;

	switch( m_CardDirection )
	{
	case Direction_East:	//东
		{
			int nCardHeight = g_CardResource.m_ImageHeapDoubleV.GetHeight()-Y_HEAP_DOUBLE_V_EXCUSION;
			int nYExcusion = 0;
			if( !bTail )
			{
				nYExcusion = (m_wMinusHeadCount+2)/2;
				ptHead.y -= nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION+nCardHeight;
				if( m_wMinusHeadCount%2 != 0 )	ptHead.y += 9;
			}
			else
			{
				nYExcusion = (m_wFullCount-m_wMinusLastCount+1)/2;
				ptHead.y -= nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION+nCardHeight;
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 9;
			}
		}
		break;
	case Direction_South:	//南
		{
			int nXExcusion = 0;
			if( !bTail )
			{
				nXExcusion = (m_wMinusHeadCount+2)/2;
				ptHead.x -= nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusHeadCount%2 != 0 ) ptHead.y += 9;
			}
			else
			{
				nXExcusion = (m_wFullCount-m_wMinusLastCount+1)/2;
				ptHead.x -= nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 9;
			}
		}
		break;
	case Direction_West:	//西
		{
			int nYExcusion = 0;
			if( !bTail )
			{
				nYExcusion = m_wMinusHeadCount/2;
				ptHead.y += nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION;
				if( m_wMinusHeadCount%2 != 0 ) ptHead.y += 10;
			}
			else
			{
				nYExcusion = (m_wFullCount-m_wMinusLastCount)/2;
				ptHead.y += nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION;
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 10;
			}
		}
		break;
	case Direction_North:	//北
		{
			int nXExcusion = 0;
			if( !bTail )
			{
				//nXExcusion = m_wMinusHeadCount/2;
				//ptHead.x += nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusHeadCount%2 != 0 ) ptHead.y += 9;
			}
			else
			{
				//nXExcusion = (m_wFullCount-m_wMinusLastCount)/2;
				//ptHead.x += nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 9;
			}
		}
		break;
	}

	return ptHead;
}

//获取出牌位置,参数wIndex表示从牌尾上面那只牌算起
CPoint CHeapCard::GetDispatchCardPos( WORD wIndex )
{
	ASSERT( wIndex < m_wFullCount );
	CPoint ptHead = m_ControlPoint;

	switch( m_CardDirection )
	{
	case Direction_East:	//东
		{
			int nCardHeight = g_CardResource.m_ImageHeapDoubleV.GetHeight()-Y_HEAP_DOUBLE_V_EXCUSION;
			int nYExcusion = (m_wFullCount-wIndex+1)/2*Y_HEAP_DOUBLE_V_EXCUSION+nCardHeight;
			ptHead.y -= nYExcusion;
			if( wIndex%2 != 0 ) ptHead.y += 9;
		}
		break;
	case Direction_South:	//南
		{
			int nXExcusion = (m_wFullCount-wIndex+1)/2*g_CardResource.m_ImageHeapDoubleH.GetWidth();
			ptHead.x -= nXExcusion;
			if( wIndex%2 != 0 ) ptHead.y += 9;
		}
		break;
	case Direction_West:	//西
		{
			int nYExcusion = (m_wFullCount/2-(wIndex+2)/2)*Y_HEAP_DOUBLE_V_EXCUSION;
			ptHead.y += nYExcusion;
			if( wIndex%2 != 0 ) ptHead.y += 10;
		}
		break;
	case Direction_North:	//北
		{
			//int nXExcusion = (m_wFullCount/2-(wIndex+2)/2)*g_CardResource.m_ImageHeapDoubleH.GetWidth();
			//ptHead.x += nXExcusion;
			if( wIndex%2 != 0 ) ptHead.y += 9;
		}
		break;
	}

	return ptHead;
}

//从牌堆移除牌,bTail指是否从牌尾移除
bool CHeapCard::DeductionHeapCard( bool bTail )
{
	if( m_wMinusHeadCount+m_wMinusLastCount >= m_wFullCount ) return false;

	if( !bTail )
	{
		m_wMinusHeadCount++;
		WORD wCardIndex = 0;
		if( (m_wFullCount-m_wMinusHeadCount)%2 != 0 )
			wCardIndex = m_wFullCount-m_wMinusHeadCount-1;
		else wCardIndex = m_wFullCount-m_wMinusHeadCount+1;
		while( m_cbCardData[wCardIndex] != 0 && m_wMinusLastCount+m_wMinusHeadCount<m_wFullCount )
		{
			m_wMinusHeadCount++;
			if( wCardIndex%2 != 0 )
			{
				if( wCardIndex < 3 ) break;
				wCardIndex -= 3;
			}
			else wCardIndex++;
		}
	}
	else 
	{
		m_wMinusLastCount++;
		while( m_cbCardData[m_wMinusLastCount-1] != 0 && m_wMinusLastCount+m_wMinusHeadCount<m_wFullCount )
			m_wMinusLastCount++;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CWeaveCard::CWeaveCard()
{
	//状态变量
	m_bDisplayItem=false;
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;
	m_cbDirectionCardPos = 4;

	//扑克数据
	m_wCardCount=0;
	ZeroMemory(&m_cbCardData,sizeof(m_cbCardData));



	return;
}

//析构函数
CWeaveCard::~CWeaveCard()
{
}

bool CWeaveCard::LoadResource(CD3DDevice* pD3DDevice)
{
	m_ImageDirectionPos.LoadImage(pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/DIRECTION_OPERATE.png"),FALSE);
	m_sizeImageDirection.cx = m_ImageDirectionPos.GetWidth()/4;
	m_sizeImageDirection.cy = m_ImageDirectionPos.GetHeight();
	return true;
}

//绘画扑克
void CWeaveCard::DrawCardControlEx(CD3DDevice* pD3DDevice, int nXPos, int nYPos)
{
	//设置位置
	SetControlPoint(nXPos,nYPos);
	//显示判断
	if (m_wCardCount==0) 
		return;

	//变量定义
	int nXScreenPos=0,nYScreenPos=0;

	//绘画扑克
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//变量定义
			int nYExcusion = Y_TABLE_LEFT_EXCUSION;

			nXScreenPos = m_ControlPoint.x;
			nYScreenPos = m_ControlPoint.y;

			//第一扑克
			g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,false);
			nYScreenPos += nYExcusion;

			//第二扑克
			g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,false);
			nYScreenPos += nYExcusion;

			//第三扑克
			g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,false);

			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,GetCardData(3),nXScreenPos,nYScreenPos-nYExcusion-12,false );			

			break;
		}
	case Direction_South:	//南向
		{
			//变量定义
			int nXExcusion = g_CardResource.m_ImageDisCard.GetViewWidth();

			nXScreenPos = m_ControlPoint.x;
			nYScreenPos = m_ControlPoint.y;

			//第一扑克
			g_CardResource.m_ImageDisCard.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,false);
			nXScreenPos += nXExcusion;

			//第二扑克
			g_CardResource.m_ImageDisCard.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,false);
			nXScreenPos += nXExcusion;

			//第三扑克
			g_CardResource.m_ImageDisCard.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,false);

			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageDisCard.DrawCardItem( pD3DDevice,GetCardData(3),nXScreenPos-nXExcusion,nYScreenPos-12,false );			

			break;
		}
	case Direction_West:	//西向
		{
			//变量定义
			int nYExcusion = Y_TABLE_RIGHT_EXCUSION;

			nXScreenPos = m_ControlPoint.x;
			nYScreenPos = m_ControlPoint.y-nYExcusion*2-g_CardResource.m_ImageTableRight.GetViewHeight();

			//第三扑克
			g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,false);
			nYScreenPos += nYExcusion;

			//第二扑克
			g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,false);
			nYScreenPos += nYExcusion;

			//第一扑克
			g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,false);

			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageTableRight.DrawCardItem( pD3DDevice,GetCardData(3),nXScreenPos,nYScreenPos-nYExcusion-12,false );			

			break;
		}
	case Direction_North:	//北向
		{
			//变量定义
			int nXExcusion = g_CardResource.m_ImageTableTop.GetViewWidth();

			nXScreenPos = m_ControlPoint.x-nXExcusion*3;
			nYScreenPos = m_ControlPoint.y;

			//第三扑克
			g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,false);
			nXScreenPos += nXExcusion;

			//第二扑克
			g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,false);
			nXScreenPos += nXExcusion;

			//第一扑克
			g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,false);

			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(3),nXScreenPos-nXExcusion,nYScreenPos-12,false);			

			break;
		}
	}

	return;
}


//绘画扑克
void CWeaveCard::DrawCardControl(CD3DDevice * pD3DDevice, bool bShadow)
{
	//显示判断
	if (m_wCardCount==0) 
		return;

	//变量定义
	int nXScreenPos=0,nYScreenPos=0;
	int nOffset = 10;

	//绘画扑克
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//变量定义
			int nYExcusion = Y_TABLE_LEFT_EXCUSION;

			nXScreenPos = m_ControlPoint.x;
			nYScreenPos = m_ControlPoint.y;

			//第一扑克
			g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos -= nOffset;
			nYScreenPos += nYExcusion;

			//第二扑克
			g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos -= nOffset;
			nYScreenPos += nYExcusion;

			//第三扑克
			g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);
			
			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageTableLeft.DrawCardItem( pD3DDevice,GetCardData(3),nXScreenPos+nOffset,nYScreenPos-nYExcusion-12,bShadow );

			//绘画指向
			/*if( m_cbDirectionCardPos < 4 && m_cbDirectionCardPos-m_CardDirection != 0 )
			{
				nYScreenPos -= nYExcusion+(m_wCardCount==4?12:0);

				nXScreenPos += (g_CardResource.m_ImageTableLeft.GetViewWidth()-m_sizeImageDirection.cx)/2;
				nYScreenPos += (nYExcusion-m_sizeImageDirection.cy)/2;

				m_ImageDirectionPos.DrawImage( pDC,nXScreenPos,nYScreenPos,m_sizeImageDirection.cx,m_sizeImageDirection.cy,
					m_cbDirectionCardPos*m_sizeImageDirection.cx,0 );
			}*/

			break;
		}
	case Direction_South:	//南向
		{
			//变量定义
			int nXExcusion = g_CardResource.m_ImageTableBottom.GetViewWidth();

			nXScreenPos = m_ControlPoint.x;
			nYScreenPos = m_ControlPoint.y;

			//第一扑克
			g_CardResource.m_ImageTableBottom.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos += nXExcusion;

			//第二扑克
			g_CardResource.m_ImageTableBottom.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos += nXExcusion;

			//第三扑克
			g_CardResource.m_ImageTableBottom.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);

			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageTableBottom.DrawCardItem( pD3DDevice,GetCardData(3),nXScreenPos-nXExcusion,nYScreenPos-12,bShadow );

			//绘画指向
			/*if( m_cbDirectionCardPos < 4 && m_cbDirectionCardPos-m_CardDirection != 0 )
			{
				nXScreenPos -= nXExcusion;
				nYScreenPos -= (m_wCardCount==4?12:0);

				nXScreenPos += (nXExcusion-m_sizeImageDirection.cx)/2;
				nYScreenPos += (Y_TABLE_BOTTOM_EXCUSION-m_sizeImageDirection.cy)/2;

				m_ImageDirectionPos.DrawImage( pDC,nXScreenPos,nYScreenPos,m_sizeImageDirection.cx,m_sizeImageDirection.cy,
					m_cbDirectionCardPos*m_sizeImageDirection.cx,0 );
			}*/

			break;
		}
	case Direction_West:	//西向
		{
			//变量定义
			int nYExcusion = Y_TABLE_RIGHT_EXCUSION;

			nXScreenPos = m_ControlPoint.x;
			nYScreenPos = m_ControlPoint.y-nYExcusion*2-g_CardResource.m_ImageTableRight.GetViewHeight();

			//第三扑克
			g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos += nOffset;
			nYScreenPos += nYExcusion;

			//第二扑克
			g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos += nOffset;
			nYScreenPos += nYExcusion;

			//第一扑克
			g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);			

			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageTableRight.DrawCardItem( pD3DDevice,GetCardData(3),nXScreenPos-nOffset,nYScreenPos-nYExcusion-12,bShadow );

			//绘画指向
			/*if( m_cbDirectionCardPos < 4 && m_cbDirectionCardPos-m_CardDirection != 0 )
			{
				nYScreenPos -= nYExcusion+(m_wCardCount==4?12:0);

				nXScreenPos += (g_CardResource.m_ImageTableRight.GetViewWidth()-m_sizeImageDirection.cx)/2;
				nYScreenPos += (nYExcusion-m_sizeImageDirection.cy)/2;

				m_ImageDirectionPos.DrawImage( pDC,nXScreenPos,nYScreenPos,m_sizeImageDirection.cx,m_sizeImageDirection.cy,
					m_cbDirectionCardPos*m_sizeImageDirection.cx,0 );
			}*/

			break;
		}
	case Direction_North:	//北向
		{
			//变量定义
			int nXExcusion = g_CardResource.m_ImageTableTop.GetViewWidth();

			nXScreenPos = m_ControlPoint.x-nXExcusion*3;
			nYScreenPos = m_ControlPoint.y;

			//第三扑克
			g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos += nXExcusion;

			//第二扑克
			g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
			nXScreenPos += nXExcusion;

			//第一扑克
			g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);

			//第四扑克
			if( m_wCardCount == 4 )
				g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,GetCardData(3),nXScreenPos-nXExcusion,nYScreenPos-12,bShadow);

			//绘画指向
			/*if( m_cbDirectionCardPos < 4 && m_cbDirectionCardPos-m_CardDirection != 0 )
			{
				nXScreenPos -= nXExcusion;
				nYScreenPos -= (m_wCardCount==4?12:0);

				nXScreenPos += (nXExcusion-m_sizeImageDirection.cx)/2;
				nYScreenPos += (Y_TABLE_TOP_EXCUSION-m_sizeImageDirection.cy)/2;

				m_ImageDirectionPos.DrawImage( pDC,nXScreenPos,nYScreenPos,m_sizeImageDirection.cx,m_sizeImageDirection.cy,
					m_cbDirectionCardPos*m_sizeImageDirection.cx,0 );
			}*/

			break;
		}
	}

	return;
}

//设置扑克
bool CWeaveCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//设置扑克
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(BYTE)*wCardCount);

	if( m_wCardCount == 0 )
		m_cbDirectionCardPos = 4;

	return true;
}

//获取扑克
BYTE CWeaveCard::GetCardData(WORD wIndex)
{
	ASSERT(wIndex<CountArray(m_cbCardData));
	if (wIndex==3 && !m_bDisplayItem)
	{
		return m_cbCardData[wIndex];
	}
	return (m_bDisplayItem==true)?m_cbCardData[wIndex]:0;
}

//绘画扑克
void CWeaveCard::DrawCardControl(CD3DDevice * pD3DDevice, int nXPos, int nYPos, bool bShadow)
{
	//设置位置
	SetControlPoint(nXPos,nYPos);

	DrawCardControl( pD3DDevice,bShadow );

	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CUserCard::CUserCard()
{
	//扑克数据
	m_wCardCount=0;
	m_bCurrentCard=false;

	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//析构函数
CUserCard::~CUserCard()
{
}

//绘画扑克
void CUserCard::DrawCardControl(CD3DDevice* pD3DDevice)
{
	int nOffset = 9;
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			int nYExcusion = Y_USER_LEFT_EXCUSION;

			int nXPos = m_ControlPoint.x-(MAX_COUNT-m_wCardCount)*nOffset;
			int nYPos = m_ControlPoint.y-nYExcusion*m_wCardCount-g_CardResource.m_ImageUserLeft.GetHeight()-8;

			//正常扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				//g_CardResource.m_ImageUserLeftShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserLeft.DrawImage( pD3DDevice,nXPos,nYPos );
				nXPos -= nOffset;
				nYPos += nYExcusion;
			}

			//当前扑克
			if( m_bCurrentCard )
			{
				//g_CardResource.m_ImageUserLeftShadow.DrawImage( pDC,nXPos,nYPos+8 );
				g_CardResource.m_ImageUserLeft.DrawImage( pD3DDevice,nXPos-5,nYPos+8 );
			}

			break;
		}
	case Direction_West:	//西向
		{
			int nYExcusion = Y_USER_RIGHT_EXCUSION;

			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y;

			//当前扑克
			if (m_bCurrentCard==true)
			{
				//g_CardResource.m_ImageUserRightShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserRight.DrawImage( pD3DDevice,nXPos-14,nYPos+3 );
			}
			nYPos += nYExcusion+10;

			//正常扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				//g_CardResource.m_ImageUserRightShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserRight.DrawImage( pD3DDevice,nXPos,nYPos );
				nXPos += nOffset;
				nYPos += nYExcusion;
			}

			break;
		}
	case Direction_North:	//北向
		{
			int nXExcusion = g_CardResource.m_ImageUserTop.GetWidth();

			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y;

			//当前扑克
			if (m_bCurrentCard==true)
			{
				//g_CardResource.m_ImageUserTopShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserTop.DrawImage( pD3DDevice,nXPos,nYPos );
			}
			nXPos += nXExcusion+12;

			//正常扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				//g_CardResource.m_ImageUserTopShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserTop.DrawImage( pD3DDevice,nXPos,nYPos );
				nXPos += nXExcusion;
			}

			break;
		}
	}

	return;
}

//设置扑克
bool CUserCard::SetCurrentCard(bool bCurrentCard)
{
	//设置变量
	m_bCurrentCard=bCurrentCard;

	return true;
}

//设置扑克
bool CUserCard::SetCardData(WORD wCardCount, bool bCurrentCard)
{
	//设置变量
	m_wCardCount=wCardCount;
	m_bCurrentCard=bCurrentCard;

	return true;
}

//添加扑克
bool CUserCard::AddCardData(bool bCurrentCard)
{
	ASSERT( m_wCardCount+(m_bCurrentCard?1:0) < MAX_COUNT );
	if( m_wCardCount+(m_bCurrentCard?1:0) >= MAX_COUNT ) return false;

	if( !bCurrentCard )
		m_wCardCount++;
	else 
	{
		m_bCurrentCard = true;
	}

	return true;
}

//获取出牌位置
CPoint CUserCard::GetDispatchCardPos()
{
	CPoint ptTail = m_ControlPoint;
	if( m_CardDirection == Direction_East )
		ptTail.y -= g_CardResource.m_ImageUserLeft.GetHeight();
	return ptTail;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDiscardCard::CDiscardCard()
{
	//扑克数据
	m_cbCurrentCount = 0;
	m_cbCurrentMaxCount = 12;

	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//析构函数
CDiscardCard::~CDiscardCard()
{
}

//绘画扑克
void CDiscardCard::DrawCardControl(CD3DDevice* pD3DDevice)
{
	//绘画控制
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//绘画扑克
			for( int i = 0; i < m_ItemArray.GetCount(); i++ )
			{
				tagDiscardItem &di = m_ItemArray[i];
				int nXPos=m_ControlPoint.x+di.ptExcusion.x;
				int nYPos=m_ControlPoint.y+di.ptExcusion.y;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,di.cbCardData,nXPos,nYPos,false);
			}

			break;
		}
	case Direction_South:	//南向
		{
			if( m_ItemArray.GetCount() == 0 ) break;

			//绘画扑克
			int nDrawCount = 0;
			int nMaxCount = 12;
			while( nDrawCount+nMaxCount < m_ItemArray.GetCount() )
			{
				nDrawCount += nMaxCount;
				nMaxCount -= 2;
			}
			int nBeginIndex = nDrawCount;
			int nEndIndex = (int)m_ItemArray.GetCount()-1;
			while( nDrawCount >= 0 )
			{
				//绘画列
				for( ; nBeginIndex <= nEndIndex; nBeginIndex++ )
				{
					tagDiscardItem &di = m_ItemArray[nBeginIndex];
					int nXPos=m_ControlPoint.x+di.ptExcusion.x;
					int nYPos=m_ControlPoint.y+di.ptExcusion.y;
					g_CardResource.m_ImageDisCard.DrawCardItem(pD3DDevice,di.cbCardData,nXPos,nYPos,false);
				}
				nEndIndex = nDrawCount-1;
				nMaxCount += 2;
				nBeginIndex = nDrawCount-nMaxCount;
				nDrawCount -= nMaxCount;
			};

			break;
		}
	case Direction_West:	//西向
		{
			//绘画扑克
			/*for( int i = (int)m_ItemArray.GetCount()-1; i >= 0; i-- )
			{
				tagDiscardItem &di = m_ItemArray[i];
				int nXPos=m_ControlPoint.x+di.ptExcusion.x;
				int nYPos=m_ControlPoint.y+di.ptExcusion.y;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,di.cbCardData,nXPos,nYPos,false);
			}*/

			if( m_ItemArray.GetCount() == 0 ) break;

			//绘画扑克
			int nMaxCount = 10;
			if( nMaxCount > m_ItemArray.GetCount() )
				nMaxCount = (int)m_ItemArray.GetCount();
			int nBeginIndex = nMaxCount-1;
			int nEndIndex = 0;
			int nDrawCount = 0;
			while( nDrawCount < m_ItemArray.GetCount() )
			{
				nDrawCount += nBeginIndex-nEndIndex+1;
				//绘画列
				for( ; nBeginIndex >= nEndIndex; nBeginIndex-- )
				{
					tagDiscardItem &di = m_ItemArray[nBeginIndex];
					int nXPos=m_ControlPoint.x+di.ptExcusion.x;
					int nYPos=m_ControlPoint.y+di.ptExcusion.y;
					g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,di.cbCardData,nXPos,nYPos,false);
				}
				nEndIndex = nDrawCount;
				nMaxCount -= 2;
				nBeginIndex = nDrawCount+nMaxCount-1;
				if( nDrawCount + nMaxCount > m_ItemArray.GetCount() )
					nBeginIndex = (int)m_ItemArray.GetCount()-1;
			}

			break;
		}
	case Direction_North:	//北向
		{
			if( m_ItemArray.GetCount() == 0 ) break;

			//绘画扑克
			int nMaxCount = 12;
			if( nMaxCount > m_ItemArray.GetCount() )
				nMaxCount = (int)m_ItemArray.GetCount();
			int nBeginIndex = nMaxCount-1;
			int nEndIndex = 0;
			int nDrawCount = 0;
			while( nDrawCount < m_ItemArray.GetCount() )
			{
				nDrawCount += nBeginIndex-nEndIndex+1;
				//绘画列
				for( ; nBeginIndex >= nEndIndex; nBeginIndex-- )
				{
					tagDiscardItem &di = m_ItemArray[nBeginIndex];
					int nXPos=m_ControlPoint.x+di.ptExcusion.x;
					int nYPos=m_ControlPoint.y+di.ptExcusion.y;
					g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,di.cbCardData,nXPos,nYPos,false);
				}
				nEndIndex = nDrawCount;
				nMaxCount -= 2;
				nBeginIndex = nDrawCount+nMaxCount-1;
				if( nDrawCount + nMaxCount > m_ItemArray.GetCount() )
					nBeginIndex = (int)m_ItemArray.GetCount()-1;
			}
			break;
		}
	}

	return;
}

//增加扑克
bool CDiscardCard::AddCardItem(BYTE cbCardData)
{
	//定义变量
	tagDiscardItem di;
	di.cbCardData = cbCardData;
	int nOffset = 11;

	//设置坐标
	//如果超过当前列最大数
	if( ++m_cbCurrentCount > m_cbCurrentMaxCount )
	{
		//如果已经排到最小列
		if( m_cbCurrentMaxCount == 2 )
		{
			//替换最后一个牌
			m_ItemArray[m_ItemArray.GetCount()-1].cbCardData = cbCardData;
			return true;
		}
		//重新开始一列
		m_cbCurrentMaxCount -= 2;
		m_cbCurrentCount = 1;
	}

	//计算偏移
	CPoint pt;
	switch( m_CardDirection )
	{
	case Direction_East:	//东
		{
			int nItemWidth = g_CardResource.m_ImageTableLeft.GetViewWidth();
			int nItemHeight = Y_TABLE_LEFT_EXCUSION;
			int nRowIndex = (10-m_cbCurrentMaxCount)/2;
			int nCardCount = (int)m_ItemArray.GetCount()+1;
			if (nCardCount > 28) nCardCount -= 28;
			else if (nCardCount > 24) nCardCount -= 24;
			else if (nCardCount > 18) nCardCount -= 18;
			else if (nCardCount > 10) nCardCount -= 10;
			pt.x = nItemWidth*nRowIndex-nCardCount*nOffset-nRowIndex*20;
			pt.y = (m_cbCurrentCount-1+nRowIndex)*nItemHeight;
		}
		break;
	case Direction_South:	//南
		{
			int nItemWidth = g_CardResource.m_ImageDisCard.GetViewWidth();
			int nItemHeight = Y_TABLE_BOTTOM_EXCUSION;
			int nRowIndex = (12-m_cbCurrentMaxCount)/2;
			pt.x = (m_cbCurrentCount-1+nRowIndex)*nItemWidth;
			pt.y = -nItemHeight*nRowIndex;
		}
		break;
	case Direction_West:	//西
		{
			int nItemWidth = g_CardResource.m_ImageTableRight.GetViewWidth();
			int nItemHeight = Y_TABLE_RIGHT_EXCUSION;
			int nRowIndex = (10-m_cbCurrentMaxCount)/2;
			int nCardCount = (int)m_ItemArray.GetCount()+1;
			if (nCardCount > 28) nCardCount -= 28;
			else if (nCardCount > 24) nCardCount -= 24;
			else if (nCardCount > 18) nCardCount -= 18;
			else if (nCardCount > 10) nCardCount -= 10;
			pt.x = -nItemWidth*nRowIndex-nCardCount*nOffset-nRowIndex;
			pt.y = -(m_cbCurrentCount-1+nRowIndex)*nItemHeight - g_CardResource.m_ImageTableRight.GetViewHeight();
		}
		break;
	case Direction_North:	//北
		{
			int nItemWidth = g_CardResource.m_ImageTableTop.GetViewWidth();
			int nItemHeight = Y_TABLE_TOP_EXCUSION;
			int nRowIndex = (12-m_cbCurrentMaxCount)/2;
			pt.x = -(m_cbCurrentCount+nRowIndex)*nItemWidth;
			pt.y = nItemHeight*nRowIndex;
		}
		break;
	}

	//设置偏移
	di.ptExcusion = pt;

	//添加
	m_ItemArray.Add( di );

	return true;
}

//设置方向
void CDiscardCard::SetDirection( enDirection Direction )
{
	m_CardDirection = Direction;

	//设置当前列最大显示数目,水平方向第一列为12,往后逐步减2,垂直方向第一列为10,往后逐步减2
	if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
		m_cbCurrentMaxCount = 10;
	else m_cbCurrentMaxCount = 12;
}

//设置扑克
bool CDiscardCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	m_cbCurrentCount = 0;

	//设置当前列最大显示数目,水平方向第一列为12,往后逐步减2,垂直方向第一列为10,往后逐步减2
	if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
		m_cbCurrentMaxCount = 10;
	else m_cbCurrentMaxCount = 12;

	m_ItemArray.RemoveAll();
	for( WORD i = 0; i < wCardCount; i++ )
		AddCardItem( cbCardData[i] );

	return true;
}

//获取位置
CPoint CDiscardCard::GetLastCardPosition()
{
	if( m_ItemArray.GetCount() == 0 ) return CPoint(0,0);

	CPoint pt = m_ItemArray[m_ItemArray.GetCount()-1].ptExcusion;
	pt.x += m_ControlPoint.x;
	pt.y += m_ControlPoint.y;

	//居中
	switch( m_CardDirection )
	{
	case Direction_East:
	case Direction_West:
		{
			pt.x += g_CardResource.m_ImageTableLeft.GetViewWidth()/2;
			pt.y += Y_TABLE_LEFT_EXCUSION/2;
		}
		break;
	case Direction_North:
	case Direction_South:
		{
			pt.x += g_CardResource.m_ImageTableTop.GetViewWidth()/2;
			pt.y += Y_TABLE_TOP_EXCUSION/2;
		}
	}

	return pt;
}

//获取出牌位置
CPoint CDiscardCard::GetDispatchCardPos()
{
	CPoint pt;

	//如果超过当前列最大数
	BYTE cbCurrentCount = m_cbCurrentCount;
	BYTE cbCurrentMaxCount = m_cbCurrentMaxCount;
	if( ++cbCurrentCount > cbCurrentMaxCount )
	{
		//如果已经排到最小列
		if( cbCurrentMaxCount == 2 )
		{
			//替换最后一个牌
			pt = m_ItemArray[m_ItemArray.GetCount()-1].ptExcusion;
			pt += m_ControlPoint;
			return pt;
		}
		//重新开始一列
		cbCurrentMaxCount -= 2;
		cbCurrentCount = 1;
	}

	//计算偏移
	switch( m_CardDirection )
	{
	case Direction_East:	//东
		{
			int nItemWidth = g_CardResource.m_ImageTableLeft.GetViewWidth();
			int nItemHeight = Y_TABLE_LEFT_EXCUSION;
			int nRowIndex = (10-cbCurrentMaxCount)/2;
			pt.x = nItemWidth*nRowIndex;
			pt.y = (cbCurrentCount-1+nRowIndex)*nItemHeight;
		}
		break;
	case Direction_South:	//南
		{
			int nItemWidth = g_CardResource.m_ImageDisCard.GetViewWidth();
			int nItemHeight = Y_TABLE_BOTTOM_EXCUSION;
			int nRowIndex = (12-cbCurrentMaxCount)/2;
			pt.x = (cbCurrentCount-1+nRowIndex)*nItemWidth;
			pt.y = -nItemHeight*nRowIndex;
		}
		break;
	case Direction_West:	//西
		{
			int nItemWidth = g_CardResource.m_ImageTableRight.GetViewWidth();
			int nItemHeight = Y_TABLE_RIGHT_EXCUSION;
			int nRowIndex = (10-cbCurrentMaxCount)/2;
			pt.x = -nItemWidth*nRowIndex;
			pt.y = -(cbCurrentCount-1+nRowIndex)*nItemHeight - g_CardResource.m_ImageTableRight.GetViewHeight();
		}
		break;
	case Direction_North:	//北
		{
			int nItemWidth = g_CardResource.m_ImageTableTop.GetViewWidth();
			int nItemHeight = Y_TABLE_TOP_EXCUSION;
			int nRowIndex = (12-cbCurrentMaxCount)/2;
			pt.x = -(cbCurrentCount+nRowIndex)*nItemWidth;
			pt.y = nItemHeight*nRowIndex;
		}
		break;
	}

	//加基准位置
	pt += m_ControlPoint;

	return pt;
}

//移除牌
void CDiscardCard::RemoveCard( BYTE cbRemoveCount )
{
	ASSERT( cbRemoveCount <= m_ItemArray.GetCount() );
	if( cbRemoveCount > m_ItemArray.GetCount() ) return;

	INT nCount = (INT)m_ItemArray.GetCount();
	m_ItemArray.RemoveAt( nCount-cbRemoveCount,cbRemoveCount );

	//设置变量
	if( m_cbCurrentCount < cbRemoveCount )
	{
		cbRemoveCount -= m_cbCurrentCount;
		if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
			m_cbCurrentMaxCount = __min( 10,m_cbCurrentMaxCount+2 );
		else m_cbCurrentMaxCount = __min( 12,m_cbCurrentMaxCount+2 );
		while( cbRemoveCount > m_cbCurrentMaxCount )
		{
			cbRemoveCount -= m_cbCurrentMaxCount;
			if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
				m_cbCurrentMaxCount = __min( 10,m_cbCurrentMaxCount+2 );
			else m_cbCurrentMaxCount = __min( 12,m_cbCurrentMaxCount+2 );
		}
		m_cbCurrentCount = m_cbCurrentMaxCount-cbRemoveCount;
	}
	else m_cbCurrentCount -= cbRemoveCount;

	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableCard::CTableCard()
{
	//扑克数据
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//析构函数
CTableCard::~CTableCard()
{
}

//绘画扑克
void CTableCard::DrawCardControl(CD3DDevice* pD3DDevice)
{
	int nOffset = 11;
	//绘画控制
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			int nYExcusion = Y_TABLE_LEFT_EXCUSION;
			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y+(CountArray(m_cbCardData)-m_wCardCount)*nYExcusion;

			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pD3DDevice,m_cbCardData[i],nXPos,nYPos,true);
				nXPos -= nOffset;
				nYPos += nYExcusion;
			}

			break;
		}
	case Direction_South:	//南向
		{
			int nXExcusion = g_CardResource.m_ImageTableBottom.GetViewWidth();
			int nXPos = m_ControlPoint.x+(CountArray(m_cbCardData)-m_wCardCount)*nXExcusion;
			int nYPos = m_ControlPoint.y;

			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(pD3DDevice,m_cbCardData[i],nXPos,nYPos,true);
				nXPos += nXExcusion;
			}

			break;
		}
	case Direction_West:	//西向
		{
			int nYExcusion = Y_TABLE_RIGHT_EXCUSION;
			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y-(CountArray(m_cbCardData)-1)*nYExcusion-g_CardResource.m_ImageTableRight.GetViewHeight();

			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pD3DDevice,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos,true);
				nXPos += nOffset;
				nYPos += nYExcusion;
			}

			break;
		}
	case Direction_North:	//北向
		{
			int nItemWidth = g_CardResource.m_ImageTableTop.GetViewWidth();
			int nXPos = m_ControlPoint.x-CountArray(m_cbCardData)*nItemWidth;
			int nYPos = m_ControlPoint.y;

			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pD3DDevice,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos,true);
				nXPos += nItemWidth;
			}

			break;
		}
	}

	return;
}

//设置扑克
bool CTableCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData))
		return false;

	//设置扑克
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CCardControl::CCardControl()
{
	//状态变量
	m_bPositively=false;
	m_bDisplayItem=false;
	m_bSelectMode = false;
	m_cbSelectInfoCount = 0;
	m_cbCurSelectIndex = CountArray(m_SelectCardInfo);
	ZeroMemory( m_SelectCardInfo,sizeof(m_SelectCardInfo) );
	for( BYTE i = 0; i < CountArray(m_rcSelectCard); i++ )
		m_rcSelectCard[i].SetRectEmpty();

	//位置变量
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//扑克数据
	m_wCardCount=0;
	m_wHoverItem=INVALID_ITEM;
	ZeroMemory(&m_CurrentCard,sizeof(m_CurrentCard));
	ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

	//加载设置
	m_ControlPoint.SetPoint(0,0);
	m_ControlSize.cy = CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=(CountArray(m_CardItemArray)+1)*CARD_WIDTH+POS_SPACE;



	return;
}

//析构函数
CCardControl::~CCardControl()
{
}

//加载资源
bool CCardControl::LoadResource(CD3DDevice* pD3DDevice)
{
	//阴影图片
	m_ImageCardShadow.LoadImage(pD3DDevice, TEXT("SubGame/SparrowDZ/clientview/USER_BOTTOM_SHADOW.png"),FALSE);
	return true;
}
//基准位置
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//横向位置
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//竖向位置
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//基准位置
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//横向位置
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//竖向位置
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//获取弹起扑克
BYTE CCardControl::GetHoverCard()
{
	//获取扑克
	if (m_wHoverItem!=INVALID_ITEM)
	{
		if (m_wHoverItem==CountArray(m_CardItemArray))
			return m_CurrentCard.cbCardData;
		else return m_CardItemArray[m_wHoverItem].cbCardData;
	}

	return 0;
}

//设置扑克
bool CCardControl::SetCurrentCard(BYTE cbCardData)
{
	//设置变量
	m_CurrentCard.bShoot=false;
	m_CurrentCard.bGrayed = false;
	m_CurrentCard.cbCardData=cbCardData;

	return true;
}

//设置扑克
bool CCardControl::SetCurrentCard(tagCardItem CardItem)
{
	//设置变量
	m_CurrentCard = CardItem;

	return true;
}

//设置扑克
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard)
{
	////效验大小
	//ASSERT(wCardCount<=CountArray(m_CardItemArray));
	//if (wCardCount>CountArray(m_CardItemArray)) 
	//	return false;

	//当前扑克
	m_CurrentCard.bShoot=false;
	m_CurrentCard.bGrayed = false;
	m_CurrentCard.cbCardData=cbCurrentCard;

	//设置扑克
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].bGrayed = false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//添加扑克
bool CCardControl::AddCardData(const BYTE cbCardData[], WORD wCardCount)
{
	ASSERT( m_wCardCount+(m_CurrentCard.cbCardData!=0?1:0)+wCardCount <= MAX_COUNT );
	if( m_wCardCount+(m_CurrentCard.cbCardData!=0?1:0)+wCardCount > MAX_COUNT ) return false;

	for( WORD i = 0; i < wCardCount; i++ )
	{
		if( m_wCardCount < CountArray(m_CardItemArray) )
		{
			m_CardItemArray[m_wCardCount].bShoot = false;
			m_CardItemArray[m_wCardCount].bGrayed = false;
			m_CardItemArray[m_wCardCount++].cbCardData = cbCardData[i];
		}
		else
		{
			ASSERT( i == wCardCount-1 );
			m_CurrentCard.bShoot = false;
			m_CurrentCard.bGrayed = false;
			m_CurrentCard.cbCardData = cbCardData[i];
		}
	}
	return true;
}

//设置扑克
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray))
		return false;

	//设置扑克
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i] = CardItemArray[i];
	}

	return true;
}

//绘画扑克
void CCardControl::DrawCardControl(CD3DDevice* pD3DDevice)
{
	//绘画准备
	int nXExcursion=m_ControlPoint.x+(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH;

	//绘画扑克
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//计算位置
		int nXScreenPos=nXExcursion+CARD_WIDTH*i;
		int nYScreenPos=m_ControlPoint.y+(m_CardItemArray[i].bShoot==false?POS_SHOOT:0);

		//绘画阴影
		/*m_ImageCardShadow.DrawImage( pDC,nXScreenPos+((m_CurrentCard.bShoot||m_wHoverItem==i)?3:0),
			m_ControlPoint.y+POS_SHOOT );*/

		//绘画扑克
		BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
		if( !m_CardItemArray[i].bGrayed )
			g_CardResource.m_ImageUserBottom.DrawCardItem(pD3DDevice,cbCardData,nXScreenPos,nYScreenPos,false);
		else
			g_CardResource.m_ImageUserBtGrayed.DrawCardItem( pD3DDevice,cbCardData,nXScreenPos,nYScreenPos,false );
	}

	//当前扑克
	if (m_CurrentCard.cbCardData!=0)
	{
		//计算位置
		int nXScreenPos=m_ControlPoint.x+CountArray(m_CardItemArray)*CARD_WIDTH+POS_SPACE;
		int nYScreenPos=m_ControlPoint.y+(m_CurrentCard.bShoot==false?POS_SHOOT:0);
		
		//绘画阴影
		//m_ImageCardShadow.DrawImage( pDC,nXScreenPos+(m_CurrentCard.bShoot?3:0),m_ControlPoint.y+POS_SHOOT );

		//绘画扑克
		BYTE cbCardData=(m_bDisplayItem==true)?m_CurrentCard.cbCardData:0;
		if( !m_CurrentCard.bGrayed )
			g_CardResource.m_ImageUserBottom.DrawCardItem(pD3DDevice,cbCardData,nXScreenPos,nYScreenPos,false);
		else 
			g_CardResource.m_ImageUserBtGrayed.DrawCardItem( pD3DDevice,cbCardData,nXScreenPos,nYScreenPos,false );
	}

	return;
}

//索引切换
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//基准位置
	int nXPos=MousePoint.x-m_ControlPoint.x;
	int nYPos=MousePoint.y-m_ControlPoint.y;

	//范围判断
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) 
		return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) 
		return INVALID_ITEM;

	//牌列子项
	if (nXPos<CARD_WIDTH*(int)CountArray(m_CardItemArray))
	{
		WORD wViewIndex=(WORD)(nXPos/CARD_WIDTH)+m_wCardCount;
		if (wViewIndex>=CountArray(m_CardItemArray))
		{
			wViewIndex -= CountArray(m_CardItemArray);
			//如果该牌不是灰色状态,则返回该索引
			if( !m_CardItemArray[wViewIndex].bGrayed )
				return wViewIndex;
		}
		return INVALID_ITEM;
	}

	//当前子项
	if ((m_CurrentCard.cbCardData!=0)&&(nXPos>=(m_ControlSize.cx-CARD_WIDTH))&&!m_CurrentCard.bGrayed) 
		return CountArray(m_CardItemArray);

	return INVALID_ITEM;
}

//光标消息
bool CCardControl::OnEventSetCursor(CPoint Point, CRect &rcRePaint)
{
	rcRePaint.SetRectEmpty();

	if( !m_bSelectMode && m_bPositively==true )
	{
		//获取索引
		WORD wHoverItem=SwitchCardPoint(Point);
		//响应判断
		if ((m_bPositively==false)&&(m_wHoverItem!=INVALID_ITEM))
		{
			m_wHoverItem=INVALID_ITEM;
		}
		if( wHoverItem != m_wHoverItem )
		{
			//计算区域
			CRect rcOld = GetCardRect(m_wHoverItem);
			CRect rcNew = GetCardRect(wHoverItem);
			rcRePaint.UnionRect( rcOld,rcNew );
			rcRePaint.InflateRect( 0,0,40,0 );
			rcRePaint.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );

			if( m_wHoverItem != INVALID_ITEM )
			{
				if( m_wHoverItem == CountArray(m_CardItemArray) )
					m_CurrentCard.bShoot = false;
				else m_CardItemArray[m_wHoverItem].bShoot = false;
			}
			m_wHoverItem=wHoverItem;
			if( m_wHoverItem != INVALID_ITEM )
			{
				if( m_wHoverItem == CountArray(m_CardItemArray) )
					m_CurrentCard.bShoot = true;
				else m_CardItemArray[m_wHoverItem].bShoot = true;
			}
		}

		if( wHoverItem!=INVALID_ITEM )
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
		else return false;
	}
	//如果是选择模式下,则弹起当前选择的牌
	else if( m_bSelectMode && m_bPositively )
	{
		//设置重绘区
		if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
		{
			CRect rc = m_rcSelectCard[m_cbCurSelectIndex];
			rc.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );
			rcRePaint = rc;
		}

		//获取当前选择位置
		BYTE cbCurIndex = CountArray(m_SelectCardInfo);
		for( BYTE i = 0; i < m_cbSelectInfoCount; i++ )
		{
			CRect rcValid = m_rcSelectCard[i];
			rcValid.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );
			if( rcValid.PtInRect(Point) )
			{
				cbCurIndex = i;
				break;
			}
		}

		//设置当前弹起
		if( m_cbCurSelectIndex != cbCurIndex )
		{
			//设置所有牌非弹起
			m_CurrentCard.bShoot = false;
			for( WORD i = 0; i < m_wCardCount; i++ )
				m_CardItemArray[i].bShoot = false;
			//设置当前选择位置
			m_cbCurSelectIndex = cbCurIndex;
			//设置当前弹起
			if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
			{
				for( BYTE i = 0; i < m_SelectCardInfo[cbCurIndex].cbCardCount; i++ )
				{
					WORD j = 0;
					for( ; j < m_wCardCount; j++ )
					{
						if( m_CardItemArray[j].cbCardData == m_SelectCardInfo[cbCurIndex].cbCardData[i] &&
							!m_CardItemArray[j].bShoot)
						{
							m_CardItemArray[j].bShoot = true;
							break;
						}
					}
					if( j == m_wCardCount && 
						m_CurrentCard.cbCardData == m_SelectCardInfo[cbCurIndex].cbCardData[i] )
					{
						m_CurrentCard.bShoot = true;
					}
				}
				//加入重绘区域
				CRect rc = m_rcSelectCard[cbCurIndex];
				rc.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );
				rcRePaint |= rc;
			}
		}

		if( m_cbCurSelectIndex!=CountArray(m_SelectCardInfo) )
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
		else return false;
	}

	return false;
}

//设置所有牌是否灰色
void CCardControl::SetAllCardGray( bool bGray )
{
	for( WORD i = 0; i < m_wCardCount; i++ )
		m_CardItemArray[i].bGrayed = bGray;
	
	if( m_CurrentCard.cbCardData != 0 )
		m_CurrentCard.bGrayed = bGray;
}

//设置牌灰色
void CCardControl::SetCardGray( BYTE cbCardData[], BYTE cbCardCount, bool bGray )
{
	ASSERT( cbCardCount <= m_wCardCount+1 );
	if( cbCardCount > m_wCardCount+1 ) return;

	for( WORD i = 0; i < cbCardCount; i++ )
	{
		//查找相应牌
		WORD j = 0;
		for( ; j < m_wCardCount; j++ )
		{
			if( m_CardItemArray[j].cbCardData == cbCardData[i] )
			{
				m_CardItemArray[j].bGrayed = bGray;
				break;
			}
		}
		if( j == m_wCardCount && m_CurrentCard.cbCardData == cbCardData[i] )
			m_CurrentCard.bGrayed = bGray;
	}
}

//获取灰色牌
BYTE CCardControl::GetGrayCard( BYTE cbCardData[], BYTE cbMaxCount, bool bGetGray )
{
	//清0
	ZeroMemory( cbCardData,sizeof(BYTE)*cbMaxCount );

	BYTE cbGetCount = 0;
	//查找符合条件的牌
	for( WORD i = 0; i < m_wCardCount; i++ )
	{
		if( m_CardItemArray[i].bGrayed == bGetGray )
		{
			cbCardData[cbGetCount++] = m_CardItemArray[i].cbCardData;
			if( cbGetCount == cbMaxCount ) return cbGetCount;
		}
	}
	if( m_CurrentCard.bGrayed == bGetGray )
		cbCardData[cbGetCount++] = m_CurrentCard.cbCardData;

	return cbGetCount;
}

//设置操作模式
void CCardControl::SetSelectMode( bool bSelectMode )
{
	if( m_bSelectMode != bSelectMode )
	{
		m_bSelectMode = bSelectMode;
		if( !m_bSelectMode )
			SetAllCardGray(false);
	}
}

//获取出牌位置
CPoint CCardControl::GetDispatchCardPos()
{
	CPoint pt = m_ControlPoint;
	pt.x += CountArray(m_CardItemArray)*CARD_WIDTH+POS_SPACE;
	return pt;
}

//鼠标按下
bool CCardControl::OnEventLeftHitCard()
{
	if( !m_bSelectMode )
	{
		BYTE cbHoverCard = GetHoverCard();
		if( cbHoverCard != 0 )
		{
			CGameFrameView *pWnd=CGameFrameView::GetInstance();
			if( pWnd )
				pWnd->PostEngineMessage(IDM_OUT_CARD,cbHoverCard,0);
		}
		return cbHoverCard!=0;
	}
	//选择模式下
	else
	{
		//当前选择
		if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
		{
			CGameFrameView *pWnd=CGameFrameView::GetInstance();
			if( pWnd )
				pWnd->PostEngineMessage(IDM_CARD_OPERATE,0,0);
		}
		else return false;
	}
	return false;
}

//获取区域
CRect CCardControl::GetCardRect( WORD wItemIndex )
{
	//变量定义
	CRect rc;
	rc.SetRectEmpty();

	if( wItemIndex == INVALID_ITEM ) return rc;

	//当前牌
	if( wItemIndex == CountArray(m_CardItemArray) )
	{
		CPoint ptTopLeft( CountArray(m_CardItemArray)*CARD_WIDTH+POS_SPACE,0 );
		CPoint ptRightBottom( ptTopLeft.x+CARD_WIDTH,ptTopLeft.y+CARD_HEIGHT );
		rc.SetRect( ptTopLeft,ptRightBottom );
	}
	else
	{
		WORD wViewIndex = CountArray(m_CardItemArray)-m_wCardCount+wItemIndex;
		CPoint ptTopLeft( wViewIndex*CARD_WIDTH,0 );
		CPoint ptRightBottom( ptTopLeft.x+CARD_WIDTH,ptTopLeft.y+CARD_HEIGHT );
		rc.SetRect( ptTopLeft,ptRightBottom );
	}

	//扩大像素
	rc.InflateRect( 0,0,0,POS_SHOOT );
	return rc;
}

//玩家吃牌
bool CCardControl::OnEventUserAction( const tagSelectCardInfo SelectInfo[MAX_WEAVE], BYTE cbInfoCount )
{
	//初始化
	m_cbSelectInfoCount = cbInfoCount;
	CopyMemory( m_SelectCardInfo,SelectInfo,sizeof(tagSelectCardInfo)*cbInfoCount );

	//设置所有牌灰色
	SetAllCardGray( true );

	//设置变量
	m_bSelectMode = true;
	m_cbCurSelectIndex = CountArray(m_SelectCardInfo);
	
	//根据组合信息，搜索并设置牌
	for( BYTE i = 0; i < m_cbSelectInfoCount; i++ )
	{
		//设置区域
		m_rcSelectCard[i].SetRectEmpty();

		for( BYTE j = 0; j < m_SelectCardInfo[i].cbCardCount; j++ )
		{
			WORD k = 0;
			for( ; k < m_wCardCount; k++ )
			{
				if( m_CardItemArray[k].cbCardData == m_SelectCardInfo[i].cbCardData[j] )
				{
					CRect rcCard = GetCardRect(k);

					if( rcCard != (m_rcSelectCard[i]&rcCard) )
					{
						//设置不灰
						m_CardItemArray[k].bGrayed = false;

						//增加区域
						m_rcSelectCard[i] |= rcCard;

						break;
					}
				}
			}
			if( k == m_wCardCount && m_CurrentCard.cbCardData == m_SelectCardInfo[i].cbCardData[j] )
			{
				//设置不灰
				m_CurrentCard.bGrayed = false;

				//增加区域
				m_rcSelectCard[i] |= GetCardRect(CountArray(m_CardItemArray));
			}
		}
	}

	return m_cbSelectInfoCount==1;
}

//获取玩家操作结果
void CCardControl::GetUserSelectResult( tagSelectCardInfo &SelectInfo )
{
	if( m_bSelectMode )
	{
		ASSERT( m_cbSelectInfoCount > 0 );
		if( m_cbSelectInfoCount == 0 ) return;
		if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
			SelectInfo = m_SelectCardInfo[m_cbCurSelectIndex];
		else SelectInfo = m_SelectCardInfo[0];
	}
	else SelectInfo.cbCardCount = 0;
	return;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//变量声明
CCardResource						g_CardResource;						//扑克资源

//////////////////////////////////////////////////////////////////////////
