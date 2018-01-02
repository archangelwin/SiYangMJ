#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
//////////////////////////////////////////////////////////////////////////

//���ڲ����ת����������ƿ��Դ����������ʹ�ã�����Ϊ����������������ΪMAX_INDEX. ע:����滻����������,������.
#define	INDEX_REPLACE_CARD					MAX_INDEX

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

//��������
#define WIK_GANERAL					0x00								//��ͨ����
#define WIK_MING_GANG				0x01								//���ܣ������ٸܣ�
#define WIK_FANG_GANG				0x02								//�Ÿ�
#define WIK_AN_GANG					0x03								//����

//������־
#define WIK_NULL					0x00								//û������
#define WIK_LEFT					0x01								//�������
#define WIK_CENTER					0x02								//�г�����
#define WIK_RIGHT					0x04								//�ҳ�����
#define WIK_PENG					0x08								//��������
#define WIK_GANG					0x10								//��������
#define WIK_LISTEN					0x20								//��������
#define WIK_CHI_HU					0x40								//�Ժ�����
#define WIK_FANG_PAO			0x80								//����

//////////////////////////////////////////////////////////////////////////
//���ƶ���

#define CHR_PING_HU					0x00000001							//ƽ��
#define CHR_PENG_PENG				0x00000002							//������
#define CHR_HUN_YI_SE		        0x00000004                          //��һɫ
#define CHR_QING_YI_SE		        0x00000008                          //��һɫ
#define CHR_XIAO_MENG_QING		    0x00000010                          //С����  ����Ĭ����С���壬���жϿ��Ժ���ʱ���ж��ǲ��Ǵ�����
#define CHR_DA_MENG_QING		    0x00000020                          //����   
#define CHR_DA_JIN_DIAO				0x00000040                          //����  

#define CHR_GANG_SHANG_HUA          0x00000080                          //���ϻ�
#define CHR_QIANG_GANG_HU           0x00000100                          //���ܺ�
#define CHR_TIAN_HU					0x00000200                          //���
#define CHR_CHI_HU					0x00000400							//����
#define CHR_ZI_MO					0x00000800							//����

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagKindItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbValidIndex[3];					//ʵ���˿�����
	BYTE							cbMagicCount;						//��������
};

//���ƽ��
struct tagGangCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_WEAVE];				//�˿�����
};


//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����˿�
	bool                            bMagicEye;                          //�����Ƿ�������
	BYTE							cbWeaveKind[MAX_WEAVE];				//�������
	BYTE							cbCenterCard[MAX_WEAVE];			//�����˿�
	BYTE                            cbCardData[MAX_WEAVE][4];           //ʵ���˿�
};


//////////////////////////////////////////////////////////////////////////


#define MASK_CHI_HU_RIGHT			0x0fffffff

/*
//	Ȩλ�ࡣ
//  ע�⣬�ڲ�����λʱ���ֻ��������Ȩλ.����
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)������������޶���ġ�
//  ֻ�ܵ�������:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/
class CChiHuRight
{	
	//��̬����
private:
	static bool						m_bInit;
	static DWORD					m_dwRightMask[MAX_RIGHT_COUNT];

	//Ȩλ����
private:
	DWORD							m_dwRight[MAX_RIGHT_COUNT];

public:
	//���캯��
	CChiHuRight();

	//���������
public:
	//��ֵ��
	CChiHuRight & operator = (DWORD dwRight);

	//�����
	CChiHuRight & operator &= (DWORD dwRight);
	//�����
	CChiHuRight & operator |= (DWORD dwRight);

	//��
	CChiHuRight operator & (DWORD dwRight);
	CChiHuRight operator & (DWORD dwRight) const;

	//��
	CChiHuRight operator | (DWORD dwRight);
	CChiHuRight operator | (DWORD dwRight) const;

	//���
	bool operator == (DWORD dwRight) const;
	bool operator == (const CChiHuRight chr) const;

	//�����
	bool operator != (DWORD dwRight) const;
	bool operator != (const CChiHuRight chr) const;

	//���ܺ���
public:
	//�Ƿ�ȨλΪ��
	bool IsEmpty();

	//����ȨλΪ��
	void SetEmpty();

	//��ȡȨλ��ֵ
	BYTE GetRightData(DWORD dwRight[], BYTE cbMaxCount);

	//����Ȩλ��ֵ
	bool SetRightData(const DWORD dwRight[], BYTE cbRightCount);

private:
	//���Ȩλ�Ƿ���ȷ
	bool IsValidRight(DWORD dwRight);
};

//////////////////////////////////////////////////////////////////////////////////

struct  Card_Info
{
	int		nLen;
	int		nColor;
	std::vector<BYTE> kCardList;

	bool   operator <  (const   Card_Info&   rhs)  const   //��������ʱ����д�ĺ���
	{
		return   nLen < rhs.nLen;
	}
};


//����˵��
typedef CWHArray<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//��Ϸ�߼���
class CGameLogic
{
public:
	static CString GetFileDialogPath()
	{
		CString strFileDlgPath;
		TCHAR szModuleDirectory[MAX_PATH];	//ģ��Ŀ¼
		GetModuleFileName(AfxGetInstanceHandle(), szModuleDirectory, sizeof(szModuleDirectory));
		int nModuleLen = lstrlen(szModuleDirectory);
		int nProcessLen = lstrlen(AfxGetApp()->m_pszExeName) + lstrlen(TEXT(".EXE")) + 1;
		if (nModuleLen <= nProcessLen)
			return TEXT("");
		szModuleDirectory[nModuleLen - nProcessLen] = 0;
		strFileDlgPath = szModuleDirectory;
		return strFileDlgPath;
	}

	static void NcaTextOut(CString strInfo)
	{

#ifdef DEBUG

		CString strName = GetFileDialogPath() + "\\SYMJ.log";

		CTime time = CTime::GetCurrentTime();
		CString strTime;
		strTime.Format(TEXT("%d-%d-%d %d:%d:%d"),
			time.GetYear(),
			time.GetMonth(),
			time.GetDay(),
			time.GetHour(),
			time.GetMinute(),
			time.GetSecond()
		);
		CString strMsg;
		strMsg.Format(TEXT("%s,%s\r\n"), strTime, strInfo);

		CFile fLog;
		if (fLog.Open(strName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
		{

			fLog.SeekToEnd();
			int strLength = strMsg.GetLength();
#ifdef _UNICODE
			BYTE bom[2] = { 0xff, 0xfe };
			fLog.Write(bom, sizeof(BYTE) * 2);
			strLength *= 2;
#endif
			fLog.Write((LPCTSTR)strMsg, strLength);
			fLog.Close();
		}
#endif
	}

	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//�˿�����
	BYTE							m_cbMagicIndex;						//��������

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	VOID RandCardList(BYTE cbCardData[], BYTE cbCardBuffer[], BYTE cbBufferCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//����,������ֵ����
	bool SortCardList(BYTE cbCardData[MAX_HAND_COUNT], BYTE cbCardCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//���ò���
	void SetMagicIndex(BYTE cbMagicIndex) { m_cbMagicIndex = cbMagicIndex; }
	//�����ж�
	bool IsMagicCard(BYTE cbCardData);
	//�����ж�
	bool IsHuaCard(BYTE cbCardData);
	//�����ж�
	BYTE IsHuaCard(BYTE cbCardIndex[MAX_INDEX]);

	//�߼�����
public:

	//�ȼ�����
public:
	//�����ȼ�
	BYTE GetUserActionRank(BYTE cbUserAction);
	//���Ƶȼ�
	WORD GetChiHuActionRank(const CChiHuRight & ChiHuRight);
	WORD GetChiHuScore(const CChiHuRight & ChiHuRight, const BYTE cbCardIndex[MAX_INDEX], BYTE flowerCnt);
	WORD GetBaseScore(const CChiHuRight & ChiHuRight);
	WORD GetExtralBaseScore(const BYTE cbCardIndex[MAX_INDEX]);
	//���Ʊ���
	WORD GetChiHuTimes(const CChiHuRight & ChiHuRight);
	WORD GetZuiTimes(const CChiHuRight & ChiHuRight);
	//�Զ�����
	BYTE AutomatismOutCard(const BYTE cbCardIndex[MAX_INDEX], const BYTE cbEnjoinOutCard[MAX_HAND_COUNT], BYTE cbEnjoinOutCardCount);

	//�����ж�
public:
	//�����ж�
	BYTE EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//�����ж�
public:
	//���Ʒ���
	BYTE AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
	//���Ʒ���
	BYTE AnalyseGangCardEx(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE cbProvideCard, bool isSend, tagGangCardResult & GangCardResult);
	BYTE AnalyseCanHuForTing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, BYTE flowerCnt, bool isGui /*= false*/);
	//�Ժ�����
	BYTE AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, BYTE flowerCnt, bool isCheckTing = false);
	void logChiHuRight(const CChiHuRight &ChiHuRight, string str);
	//���Ʒ���
	BYTE AnalyseTingCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE flowerCnt);
	//���Ʒ�������ȡ����Щ���������Լ��ܺ���Щ��
	BYTE AnalyseTingCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE flowerCnt, bool isGuoZhuang, BYTE& cbOutCardCount,BYTE cbOutCardData[],BYTE cbHuCardCount[],BYTE cbHuCardData[][28]);
	//��ȡ�������ݣ����ƺ����
	BYTE GetHuCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE cbHuCardData[], BYTE flowerCnt);

	//�Ƿ��ǻ���
	bool IsFlowerCard(BYTE cbCardData);

	bool IsHasFlowerCard(const BYTE cbCardIndex[MAX_INDEX]);
	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_HAND_COUNT]);
	//�˿�ת��
	BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);
	

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�˿���Ŀ
	BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);	
	//����˿�
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//�ڲ�����
private:
	bool AddKindItem(tagKindItem &TempKindItem, tagKindItem KindItem[], BYTE &cbKindItemCount, bool &bMagicThree);
	//�����˿�
	bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveICount, CAnalyseItemArray & AnalyseItemArray);

	//��������
protected:

	//ƽ��
	bool IsPingHu(const tagAnalyseItem *pAnalyseItem);

	//������
	bool IsPengPeng(const tagAnalyseItem *pAnalyseItem);

	bool IsPengPeng(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard);

	bool IsDaJinDiao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard);

	//��һɫ
	BYTE IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard);

	//��һɫ
	bool IsHunYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard);

	bool IsMenQing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard);

	BYTE AnalyseIsHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	bool CGameLogic::IsJiaHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard);
	bool IsKanHu(const tagAnalyseItem *pAnalyseItem, BYTE cbCurrentCard);
	//�����
public:

	BYTE GetTingCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, BYTE curCard);

	BYTE GetKanCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount);

	BYTE GetWenQianCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount);

	// 
	BYTE GetQueColorCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount);

	BYTE GetDuiCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount);

	BYTE GetBaCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount);

	BYTE GetExtralFlowerCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount);

	bool IsCanHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, BYTE flowerCnt, CChiHuRight &ChiHuRight);
};

//////////////////////////////////////////////////////////////////////////////////

#endif