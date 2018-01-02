#include "Stdafx.h"
#include "GameScore.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScore::CGameScore()
{
	//��ʼ������
	m_bShow=false;
	m_pParent=NULL;
	m_cbCurrentIndex = 0;
	ZeroMemory(m_cbWeaveCount, sizeof(m_cbWeaveCount));
	ZeroMemory(&m_ScoreInfo, sizeof(m_ScoreInfo));
}

//��������
CGameScore::~CGameScore()
{
}

//��λ����
void CGameScore::RestorationData()
{
	//���ñ���
	m_bShow=false;
	m_cbCurrentIndex = 0;
	ZeroMemory(m_cbWeaveCount, sizeof(m_cbWeaveCount));
	ZeroMemory(&m_ScoreInfo, sizeof(m_ScoreInfo));

	for( BYTE i = 0; i < GAME_PLAYER; i++ )
	{
		m_cbWeaveCount[i] = 0;
		for( BYTE j = 0; j < CountArray(m_WeaveCard[0]); j++ )
			m_WeaveCard[i][j].SetCardData(NULL,0);

		m_ChiHuRight[i].SetEmpty();
	}

	return;
}

//��ʼ������
void CGameScore::Initialization(CWnd* pParent,CD3DDevice* pD3DDevice,HINSTANCE hInstance)
{
	m_pParent = pParent;

	//��������
	m_InfoFont.CreateFont(pD3DDevice,12,200,TEXT("����"));

	m_ImageZiMo.LoadImage(pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/ZI_MO.png"),FALSE);
	m_PngBanker.LoadImage(pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/ZHUANG.png"),FALSE);
	m_ImageFangPao.LoadImage(pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/FANG_PAO.png"),FALSE);
	m_ImageBack.LoadImage(pD3DDevice,TEXT("SubGame/SparrowDZ/clientview/GAME_SCORE.png"),FALSE);
	return;
}

//���û���
void CGameScore::SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo WeaveInfo[GAME_PLAYER])
{
	//���ñ���
	m_bShow=true;
	m_ScoreInfo=ScoreInfo;

	BYTE cbChiHuCount = 0;
	for( BYTE j = 0; j < GAME_PLAYER; j++ )
	{
		m_cbWeaveCount[j]=WeaveInfo[j].cbWeaveCount;

		for (BYTE i=0;i<m_cbWeaveCount[j];i++)
		{
			bool bPublicWeave=(WeaveInfo[j].cbPublicWeave[i]==TRUE);
			m_WeaveCard[j][i].SetCardData(WeaveInfo[j].cbCardData[i],WeaveInfo[j].cbCardCount[i]);
			m_WeaveCard[j][i].SetDisplayItem(true);
		}

		m_ChiHuRight[j].SetRightData( m_ScoreInfo.dwChiHuRight[j],MAX_RIGHT_COUNT );

		if (!m_ChiHuRight[j].IsEmpty()) cbChiHuCount++;
	}

	//ȷ����ʾ����
	for( BYTE j = 0; j < GAME_PLAYER; j++ )
	{
		if( !m_ChiHuRight[j].IsEmpty() )
		{
			m_cbCurrentIndex = j;
			break;
		}
	}

	return;
}

//�ػ�����
void CGameScore::DrawGameScore(CD3DDevice* pD3DDevice, INT nWidth, INT nHeight)
{
	if (!m_bShow) return;

	//�������
	INT nWndPosX = (nWidth-m_ImageBack.GetWidth())/2;
	INT nWndPosY = (nHeight-m_ImageBack.GetHeight())/2;

	//�滭����
	m_ImageBack.DrawImage(pD3DDevice, nWndPosX, nWndPosY);

	//λ�ñ���
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageDisCard.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount[m_cbCurrentIndex]*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount[m_cbCurrentIndex]+nCardSpace;

	//����λ��
	int nXCardPos=nWndPosX+(m_ImageBack.GetWidth()-nTotalWidth)/2;
	int nYCardPos=nWndPosY+g_CardResource.m_ImageDisCard.GetViewHeight()+35;

	//�滭���
	for (BYTE i=0;i<m_cbWeaveCount[m_cbCurrentIndex];i++)
	{
		//�滭�˿�
		m_WeaveCard[m_cbCurrentIndex][i].DrawCardControlEx(pD3DDevice, nXCardPos, nYCardPos);

		//����λ��
		nXCardPos+=(nCardSpace+nItemWidth*3);
	}

	//�滭�˿�
	for (BYTE i=0;i<m_ScoreInfo.cbCardCount[m_cbCurrentIndex];i++)
	{
		//�滭�˿�
		g_CardResource.m_ImageDisCard.DrawCardItem(pD3DDevice, m_ScoreInfo.cbCardData[m_cbCurrentIndex][i], nXCardPos, nYCardPos, false);

		//����λ��
		nXCardPos+=nItemWidth;
		if ((i+2)==m_ScoreInfo.cbCardCount[m_cbCurrentIndex]) nXCardPos+=nCardSpace;
	}

	TCHAR szBuffer[256] = TEXT("");
	//������Ϣ
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
		//������
		/*DrawTextString(pDC, m_ScoreInfo.szUserName[m_cbCurrentIndex], RGB(255,255,255), 
			RGB(0,0,0), CRect(nWndPosX+25,nWndPosY+20,nWndPosX+105,nWndPosY+34), 14, 400, DT_END_ELLIPSIS);*/

		//����
		if( m_ScoreInfo.wProvideUser != m_cbCurrentIndex )
		{
			/*DrawTextString(pDC, m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser], RGB(255,255,255), 
				RGB(0,0,0), CRect(nWndPosX+120, nWndPosY+108, nWndPosX+200, nWndPosY+122), 14, 400, DT_END_ELLIPSIS);*/
			m_ImageFangPao.DrawImage(pD3DDevice, nWndPosX+355, nWndPosY+200);
			/*DrawTextString(pDC, m_ScoreInfo.szUserName[m_cbCurrentIndex], RGB(255,255,255), 
				RGB(0,0,0), CRect(nWndPosX+300, nWndPosY+108, nWndPosX+380, nWndPosY+122), 14, 400, DT_END_ELLIPSIS);*/
		}
		//����
		else
		{
			/*DrawTextString(pDC, m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser], RGB(255,255,255), 
				RGB(0,0,0), CRect(nWndPosX+120, nWndPosY+108, nWndPosX+200, nWndPosY+122), 14, 400, DT_END_ELLIPSIS);*/
			m_ImageZiMo.DrawImage(pD3DDevice, nWndPosX+355, nWndPosY+200);
		}
	}

	//�÷���Ϣ
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//ׯ��
		if (i == m_ScoreInfo.wBankerUser)
			m_PngBanker.DrawImage(pD3DDevice, nWndPosX+70-m_PngBanker.GetWidth(), nWndPosY+190+i*26);

		if (i==m_cbCurrentIndex)
		{
			//���
			CRect rcDrawPlayer(nWndPosX+72,nWndPosY+194+i*26,nWndPosX+170,nWndPosY+218+i*26);
			DrawTextString(pD3DDevice, m_ScoreInfo.szUserName[i], D3DCOLOR_XRGB(255,255,135), D3DCOLOR_XRGB(0,0,0), rcDrawPlayer, 14, 400, DT_END_ELLIPSIS|DT_CENTER);

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			//����
			CRect rcGangScore(nWndPosX+185,nWndPosY+194+i*26,nWndPosX+260,nWndPosY+218+i*26);
			//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+I64d"),m_ScoreInfo.lGangScore[i] );
			double dbUserScore = m_ScoreInfo.lGangScore[i];
			SwitchScoreFormat(dbUserScore/100,0,TEXT("%0.2f"),szBuffer,CountArray(szBuffer));
			DrawTextString(pD3DDevice, szBuffer, D3DCOLOR_XRGB(255,255,135), D3DCOLOR_XRGB(0,0,0), rcGangScore, 14, 400, DT_END_ELLIPSIS|DT_CENTER);
#endif

			//����
			CRect rcDrawScore(nWndPosX+260,nWndPosY+194+i*26,nWndPosX+350,nWndPosY+218+i*26);
			//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+I64d"),m_ScoreInfo.lGameScore[i] );
			double dbUserScore = m_ScoreInfo.lGameScore[i];
			SwitchScoreFormat(dbUserScore/100,0,TEXT("%0.2f"),szBuffer,CountArray(szBuffer));
			DrawTextString(pD3DDevice, szBuffer, D3DCOLOR_XRGB(255,255,135), D3DCOLOR_XRGB(0,0,0), rcDrawScore, 14, 400, DT_END_ELLIPSIS|DT_CENTER);
		}
		else
		{
			//���
			CRect rcDrawPlayer(nWndPosX+72,nWndPosY+194+i*26,nWndPosX+170,nWndPosY+218+i*26);
			DrawTextString(pD3DDevice, m_ScoreInfo.szUserName[i], D3DCOLOR_XRGB(255,255,255), D3DCOLOR_XRGB(0,0,0), rcDrawPlayer, 14, 400, DT_END_ELLIPSIS|DT_CENTER);

#if GLOBAL_MODIFICATION_SWITCH_FLAG_ER_REN_MA_JIANG
#else
			//����
			CRect rcGangScore(nWndPosX+185,nWndPosY+194+i*26,nWndPosX+260,nWndPosY+218+i*26);
			//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+I64d"),m_ScoreInfo.lGameScore[i] );
			double dbUserScore = m_ScoreInfo.lGangScore[i];
			SwitchScoreFormat(dbUserScore/100,0,TEXT("%0.2f"),szBuffer,CountArray(szBuffer));
			DrawTextString(pD3DDevice, szBuffer, D3DCOLOR_XRGB(255,255,135), D3DCOLOR_XRGB(0,0,0), rcGangScore, 14, 400, DT_END_ELLIPSIS|DT_CENTER);
#endif

			//����
			CRect rcDrawScore(nWndPosX+260,nWndPosY+194+i*26,nWndPosX+350,nWndPosY+218+i*26);
			//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%+I64d"),m_ScoreInfo.lGameScore[i] );
			double dbUserScore = m_ScoreInfo.lGameScore[i];
			SwitchScoreFormat(dbUserScore/100,0,TEXT("%0.2f"),szBuffer,CountArray(szBuffer));
			DrawTextString(pD3DDevice, szBuffer, D3DCOLOR_XRGB(255,255,255), D3DCOLOR_XRGB(0,0,0), rcDrawScore, 14, 400, DT_END_ELLIPSIS|DT_CENTER);
		}

		CRect rcDrawFen(nWndPosX+360,nWndPosY+147+i*26,nWndPosX+512,nWndPosY+171+i*26);
		//DrawTextString(pDC, TEXT("��"), RGB(255,255,255), RGB(0,0,0), rcDrawFen, 14, 400, DT_END_ELLIPSIS);
	}

	return;
}

//��һλ���
void CGameScore::OnBnNext()
{
	BYTE cbIndexTemp = m_cbCurrentIndex;
	cbIndexTemp = (cbIndexTemp+1)%GAME_PLAYER;

	while( cbIndexTemp != m_cbCurrentIndex )
	{
		if( !m_ChiHuRight[cbIndexTemp].IsEmpty() ) break;
		cbIndexTemp = (cbIndexTemp+1)%GAME_PLAYER;
	}

	if( cbIndexTemp != m_cbCurrentIndex )
	{
		m_cbCurrentIndex = cbIndexTemp;
	}

	return;
}

//��������
void CGameScore::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat)
{
	//��������
	//int nStringLength=lstrlen(pszString);
	//int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	//int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	////�滭�߿�
	//for (int i=0;i<CountArray(nXExcursion);i++)
	//{
	//	m_InfoFont.DrawText( pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nFormat);
	//}

	//�滭����
	m_InfoFont.DrawText(pszString,nXPos,nYPos, crText, nFormat);

	return;
}

//��������
void CGameScore::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	//CRect rcDraw;
	//for (int i=0;i<CountArray(nXExcursion);i++)
	//{
	//	rcDraw.CopyRect(lpRect);
	//	rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
	//	m_InfoFont.DrawText(pszString,&rcDraw, crFrame, nFormat);
	//}

	//�滭����
	//rcDraw.CopyRect(lpRect);
	m_InfoFont.DrawText(pszString,lpRect, crText, nFormat);

	return;
}

//////////////////////////////////////////////////////////////////////////
