#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���ڲ����ת����������ƿ��Դ����������ʹ�ã�����Ϊ����������������ΪMAX_INDEX. ע:����滻����������,������.
#define INDEX_REPLACE_CARD          33

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define MASK_COLOR          0xF0                //��ɫ����
#define MASK_VALUE          0x0F                //��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

//������־
#define WIK_NULL          0x00                //û������
#define WIK_LEFT          0x01                //�������
#define WIK_CENTER          0x02                //�г�����
#define WIK_RIGHT         0x04                //�ҳ�����
#define WIK_PENG          0x08                //��������
#define WIK_GANG          0x10                //��������
#define WIK_LISTEN          0x20                //��������
#define WIK_CHI_HU          0x40                //�Ժ�����
#define WIK_REPLACE         0x80                //�����滻

//////////////////////////////////////////////////////////////////////////
//���ƶ���

/*
//  ���ͷ�
*/
#define CHK_NULL          0x0000                //�Ǻ�
#define CHR_ZI_MO         0x0001                //����
#define CHR_PENG_PENG_HU      0x0002                //������
#define CHR_BA_DUI          0x0004                //�˶�
#define CHR_DI_HU         0x0008                //�غ�
#define CHR_TIAN_HU         0x0010                //���
#define CHR_DAN_DIAO        0x0020                //����
#define CHR_THREE_KING        0x0040                //3��
#define CHR_RUAN_HU         0x0080                //���
#define CHR_YING_HU         0x0100                //Ӳ��
#define CHR_SHUANG_FAN        0x0200                //˫��  
#define CHR_QING_YI_SE        0x0400                //��һɫ
#define CHR_BA_DUI_YING       0x1000                //Ӳ�˶�  

//////////////////////////////////////////////////////////////////////////

//��������
struct tagKindItem
{
  BYTE              cbWeaveKind;            //�������
  BYTE              cbCenterCard;           //�����˿�
  BYTE              cbValidIndex[3];          //ʵ���˿�����
};

//�������
struct tagWeaveItem
{
  BYTE              cbWeaveKind;            //�������
  BYTE              cbCenterCard;           //�����˿�
  BYTE              cbPublicCard;           //������־
  WORD              wProvideUser;           //��Ӧ�û�
  BYTE              cbCardData[4];            //
};

//���ƽ��
struct tagGangCardResult
{
  BYTE              cbCardCount;            //�˿���Ŀ
  BYTE              cbCardData[MAX_WEAVE];        //�˿�����
};

//��������
struct tagAnalyseItem
{
  BYTE              cbCardEye;              //�����˿�
  bool                            bMagicEye;                          //�����Ƿ�������
  BYTE              cbWeaveKind[MAX_WEAVE];       //�������
  BYTE              cbCenterCard[MAX_WEAVE];      //�����˿�
  BYTE                            cbCardData[MAX_WEAVE][4];           //ʵ���˿�
};

//////////////////////////////////////////////////////////////////////////


#define MASK_CHI_HU_RIGHT     0x0fffffff

/*
//  Ȩλ�ࡣ
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
  static bool           m_bInit;
  static DWORD          m_dwRightMask[MAX_RIGHT_COUNT];

  //Ȩλ����
private:
  DWORD             m_dwRight[MAX_RIGHT_COUNT];

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

//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CWHArray<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//��Ϸ�߼���
class CGameLogic
{
  //��������
protected:
  static const BYTE       m_cbCardDataArray[MAX_REPERTORY]; //�˿�����
  BYTE              m_cbMagicIndex;           //��������

  //��������
public:
  //���캯��
  CGameLogic();
  //��������
  virtual ~CGameLogic();

  //���ƺ���
public:
  //�����˿�
  void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
  //�����˿�
  void RandCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbRandData[]);
  //ɾ���˿�
  bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
  //ɾ���˿�
  bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
  //ɾ���˿�
  bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
  //���ò���
  void SetMagicIndex(BYTE cbMagicIndex)
  {
    m_cbMagicIndex = cbMagicIndex;
  }
  //�����ж�
  inline bool IsMagicCard(BYTE cbCardData, bool bIsIndex/* = false */)
  {
    BYTE cbCardIndex = cbCardData;
    if(!bIsIndex)
    {
      cbCardIndex = SwitchToCardIndex(cbCardData);
    }

    if(m_cbMagicIndex != MAX_INDEX)
    {
      if(m_cbMagicIndex >= 34 && m_cbMagicIndex <= 37)
      {
        return (cbCardIndex >= 34 && (cbCardIndex <= 37));
      }
      else if(m_cbMagicIndex >= 38 && m_cbMagicIndex <= 41)
      {
        return ((cbCardIndex >= 38) && (cbCardIndex <= 41));
      }
      else
      {
        return (cbCardIndex==m_cbMagicIndex);
      }
    }
    return false;
  }
  //�����ж�
  bool IsHuaCard(BYTE cbCardData);
  //�����ж�
  bool IsTuoPai(BYTE cbOutCard, tagWeaveItem * pWeaveItem);

  //��������
public:
  //��Ч�ж�
  bool IsValidCard(BYTE cbCardData);
  //�˿���Ŀ
  BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);
  //����˿�
  BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);
  //������Ŀ
  BYTE GetMagicCardCount(const BYTE cbCardIndex[MAX_INDEX]);

  //�ȼ�����
public:
  //�����ȼ�
  BYTE GetUserActionRank(BYTE cbUserAction);
  //���Ƶȼ�
  BYTE GetChiHuActionRank(const CChiHuRight & ChiHuRight, bool bCheckFan = false);

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
  //�Ժ�����
  BYTE AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
  //���Ʒ���
  BYTE AnalyseTingCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);

  //ת������
public:
  //�˿�ת��
  BYTE SwitchToCardData(BYTE cbCardIndex);
  //�˿�ת��
  BYTE SwitchToCardIndex(BYTE cbCardData);
  //�˿�ת��
  BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
  //�˿�ת��
  BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

  //��������
protected:
  //����
  bool IsDanDiaoJiang(const tagAnalyseItem *pAnalyseItem, BYTE cbCurrentCard);
  //�˶�
  bool IsBaDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, bool & bYingBaDui);
  //������
  bool IsPengPeng(const tagAnalyseItem *pAnalyseItem);
  //Ӳ��
  bool IsYingHu(const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount);
  //����
  bool IsSanJinDao(const BYTE cbCardIndex[MAX_INDEX]);

  //�ڲ�����
private:
  //�����˿�
  bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
  //����,������ֵ����
  bool SortCardList(BYTE cbCardData[MAX_COUNT], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
