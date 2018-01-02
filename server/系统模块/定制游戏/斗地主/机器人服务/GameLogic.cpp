#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;

//�˿�����
const BYTE  CGameLogic::m_cbCardData[FULL_COUNT]=
{
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D, //���� A - K
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D, //÷�� A - K
  0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D, //���� A - K
  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D, //���� A - K
  0x4E,0x4F,
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
  //������
  switch(cbCardCount)
  {
    case 0: //����
    {
      return CT_ERROR;
    }
    case 1: //����
    {
      return CT_SINGLE;
    }
    case 2: //���ƻ��
    {
      //�����ж�
      if((cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E))
      {
        return CT_MISSILE_CARD;
      }
      if(GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1]))
      {
        return CT_DOUBLE;
      }

      return CT_ERROR;
    }
  }

  //�����˿�
  tagAnalyseResult AnalyseResult;
  AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

  //�����ж�
  if(AnalyseResult.cbBlockCount[3]>0)
  {
    //�����ж�
    if((AnalyseResult.cbBlockCount[3]==1)&&(cbCardCount==4))
    {
      return CT_BOMB_CARD;
    }
    if((AnalyseResult.cbBlockCount[3]==1)&&(cbCardCount==6))
    {
      return CT_FOUR_TAKE_ONE;
    }
    if((AnalyseResult.cbBlockCount[3]==1)&&(cbCardCount==8)&&(AnalyseResult.cbBlockCount[1]==2))
    {
      return CT_FOUR_TAKE_TWO;
    }

    return CT_ERROR;
  }

  //�����ж�
  if(AnalyseResult.cbBlockCount[2]>0)
  {
    //�����ж�
    if(AnalyseResult.cbBlockCount[2]>1)
    {
      //��������
      BYTE cbCardData=AnalyseResult.cbCardData[2][0];
      BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

      //�������
      if(cbFirstLogicValue>=15)
      {
        return CT_ERROR;
      }

      //�����ж�
      for(BYTE i=1; i<AnalyseResult.cbBlockCount[2]; i++)
      {
        BYTE cbCardData=AnalyseResult.cbCardData[2][i*3];
        if(cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i))
        {
          return CT_ERROR;
        }
      }
    }
    else if(cbCardCount == 3)
    {
      return CT_THREE;
    }

    //�����ж�
    if(AnalyseResult.cbBlockCount[2]*3==cbCardCount)
    {
      return CT_THREE_LINE;
    }
    if(AnalyseResult.cbBlockCount[2]*4==cbCardCount)
    {
      return CT_THREE_TAKE_ONE;
    }
    if((AnalyseResult.cbBlockCount[2]*5==cbCardCount)&&(AnalyseResult.cbBlockCount[1]==AnalyseResult.cbBlockCount[2]))
    {
      return CT_THREE_TAKE_TWO;
    }

    return CT_ERROR;
  }

  //��������
  if(AnalyseResult.cbBlockCount[1]>=3)
  {
    //��������
    BYTE cbCardData=AnalyseResult.cbCardData[1][0];
    BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

    //�������
    if(cbFirstLogicValue>=15)
    {
      return CT_ERROR;
    }

    //�����ж�
    for(BYTE i=1; i<AnalyseResult.cbBlockCount[1]; i++)
    {
      BYTE cbCardData=AnalyseResult.cbCardData[1][i*2];
      if(cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i))
      {
        return CT_ERROR;
      }
    }

    //�����ж�
    if((AnalyseResult.cbBlockCount[1]*2)==cbCardCount)
    {
      return CT_DOUBLE_LINE;
    }

    return CT_ERROR;
  }

  //�����ж�
  if((AnalyseResult.cbBlockCount[0]>=5)&&(AnalyseResult.cbBlockCount[0]==cbCardCount))
  {
    //��������
    BYTE cbCardData=AnalyseResult.cbCardData[0][0];
    BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

    //�������
    if(cbFirstLogicValue>=15)
    {
      return CT_ERROR;
    }

    //�����ж�
    for(BYTE i=1; i<AnalyseResult.cbBlockCount[0]; i++)
    {
      BYTE cbCardData=AnalyseResult.cbCardData[0][i];
      if(cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i))
      {
        return CT_ERROR;
      }
    }

    return CT_SINGLE_LINE;
  }

  return CT_ERROR;
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
  //��Ŀ����
  if(cbCardCount==0)
  {
    return;
  }
  if(cbSortType==ST_CUSTOM)
  {
    return;
  }

  //ת����ֵ
  BYTE cbSortValue[MAX_COUNT];
  for(BYTE i=0; i<cbCardCount; i++)
  {
    cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
  }

  //�������
  bool bSorted=true;
  BYTE cbSwitchData=0,cbLast=cbCardCount-1;
  do
  {
    bSorted=true;
    for(BYTE i=0; i<cbLast; i++)
    {
      if((cbSortValue[i]<cbSortValue[i+1])||
         ((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
      {
        //���ñ�־
        bSorted=false;

        //�˿�����
        cbSwitchData=cbCardData[i];
        cbCardData[i]=cbCardData[i+1];
        cbCardData[i+1]=cbSwitchData;

        //����Ȩλ
        cbSwitchData=cbSortValue[i];
        cbSortValue[i]=cbSortValue[i+1];
        cbSortValue[i+1]=cbSwitchData;
      }
    }
    cbLast--;
  }
  while(bSorted==false);

  //��Ŀ����
  if(cbSortType==ST_COUNT)
  {
    //��������
    BYTE cbCardIndex=0;

    //�����˿�
    tagAnalyseResult AnalyseResult;
    AnalysebCardData(&cbCardData[cbCardIndex],cbCardCount-cbCardIndex,AnalyseResult);

    //��ȡ�˿�
    for(BYTE i=0; i<CountArray(AnalyseResult.cbBlockCount); i++)
    {
      //�����˿�
      BYTE cbIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;
      CopyMemory(&cbCardData[cbCardIndex],AnalyseResult.cbCardData[cbIndex],AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE));

      //��������
      cbCardIndex+=AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE);
    }
  }

  return;
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  //����׼��
  BYTE cbCardData[CountArray(m_cbCardData)];
  CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

  //�����˿�
  BYTE cbRandCount=0,cbPosition=0;
  do
  {
    cbPosition=rand()%(cbBufferCount-cbRandCount);
    cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
    cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
  }
  while(cbRandCount<cbBufferCount);

  return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
  //��������
  ASSERT(cbRemoveCount<=cbCardCount);

  //�������
  BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
  if(cbCardCount>CountArray(cbTempCardData))
  {
    return false;
  }
  CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

  //�����˿�
  for(BYTE i=0; i<cbRemoveCount; i++)
  {
    for(BYTE j=0; j<cbCardCount; j++)
    {
      if(cbRemoveCard[i]==cbTempCardData[j])
      {
        cbDeleteCount++;
        cbTempCardData[j]=0;
        break;
      }
    }
  }
  if(cbDeleteCount!=cbRemoveCount)
  {
    return false;
  }

  //�����˿�
  BYTE cbCardPos=0;
  for(BYTE i=0; i<cbCardCount; i++)
  {
    if(cbTempCardData[i]!=0)
    {
      cbCardData[cbCardPos++]=cbTempCardData[i];
    }
  }

  return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
  //��ȡ����
  BYTE cbCardColor=GetCardColor(cbCardData);
  BYTE cbCardValue=GetCardValue(cbCardData);

  //��Ч�ж�
  if((cbCardData==0x4E)||(cbCardData==0x4F))
  {
    return true;
  }
  if((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D))
  {
    return true;
  }

  return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
  //�˿�����
  BYTE cbCardColor=GetCardColor(cbCardData);
  BYTE cbCardValue=GetCardValue(cbCardData);

  //ת����ֵ
  if(cbCardColor==0x40)
  {
    return cbCardValue+2;
  }
  return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
  //��ȡ����
  BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
  BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

  //�����ж�
  if(cbNextType==CT_ERROR)
  {
    return false;
  }
  if(cbNextType==CT_MISSILE_CARD)
  {
    return true;
  }

  //ը���ж�
  if((cbFirstType!=CT_BOMB_CARD)&&(cbNextType==CT_BOMB_CARD))
  {
    return true;
  }
  if((cbFirstType==CT_BOMB_CARD)&&(cbNextType!=CT_BOMB_CARD))
  {
    return false;
  }

  //�����ж�
  if((cbFirstType!=cbNextType)||(cbFirstCount!=cbNextCount))
  {
    return false;
  }

  //��ʼ�Ա�
  switch(cbNextType)
  {
    case CT_SINGLE:
    case CT_DOUBLE:
    case CT_THREE:
    case CT_SINGLE_LINE:
    case CT_DOUBLE_LINE:
    case CT_THREE_LINE:
    case CT_BOMB_CARD:
    {
      //��ȡ��ֵ
      BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[0]);
      BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[0]);

      //�Ա��˿�
      return cbNextLogicValue>cbFirstLogicValue;
    }
    case CT_THREE_TAKE_ONE:
    case CT_THREE_TAKE_TWO:
    {
      //�����˿�
      tagAnalyseResult NextResult;
      tagAnalyseResult FirstResult;
      AnalysebCardData(cbNextCard,cbNextCount,NextResult);
      AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

      //��ȡ��ֵ
      BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbCardData[2][0]);
      BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbCardData[2][0]);

      //�Ա��˿�
      return cbNextLogicValue>cbFirstLogicValue;
    }
    case CT_FOUR_TAKE_ONE:
    case CT_FOUR_TAKE_TWO:
    {
      //�����˿�
      tagAnalyseResult NextResult;
      tagAnalyseResult FirstResult;
      AnalysebCardData(cbNextCard,cbNextCount,NextResult);
      AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

      //��ȡ��ֵ
      BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbCardData[3][0]);
      BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbCardData[3][0]);

      //�Ա��˿�
      return cbNextLogicValue>cbFirstLogicValue;
    }
  }

  return false;
}

//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
  //���ý��
  ZeroMemory(&OutCardResult,sizeof(OutCardResult));

  //�����˿�
  BYTE cbCardData[MAX_COUNT];
  BYTE cbCardCount=cbHandCardCount;
  CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

  //�����˿�
  SortCardList(cbCardData,cbCardCount,ST_ORDER);

  //��ȡ����
  BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount);

  //���Ʒ���
  switch(cbTurnOutType)
  {
    case CT_ERROR:          //��������
    {
      //��ȡ��ֵ
      BYTE cbLogicValue=GetCardLogicValue(cbCardData[cbCardCount-1]);

      //�����ж�
      BYTE cbSameCount=1;
      for(BYTE i=1; i<cbCardCount; i++)
      {
        if(GetCardLogicValue(cbCardData[cbCardCount-i-1])==cbLogicValue)
        {
          cbSameCount++;
        }
        else
        {
          break;
        }
      }

      //��ɴ���
      if(cbSameCount>1)
      {
        OutCardResult.cbCardCount=cbSameCount;
        for(BYTE j=0; j<cbSameCount; j++)
        {
          OutCardResult.cbResultCard[j]=cbCardData[cbCardCount-1-j];
        }
        return true;
      }

      //���ƴ���
      OutCardResult.cbCardCount=1;
      OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-1];

      return true;
    }
    case CT_SINGLE:         //��������
    case CT_DOUBLE:         //��������
    case CT_THREE:          //��������
    {
      //��ȡ��ֵ
      BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

      //�����˿�
      tagAnalyseResult AnalyseResult;
      AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

      //Ѱ�ҵ���
      if(cbTurnCardCount<=1)
      {
        for(BYTE i=0; i<AnalyseResult.cbBlockCount[0]; i++)
        {
          BYTE cbIndex=AnalyseResult.cbBlockCount[0]-i-1;
          if(GetCardLogicValue(AnalyseResult.cbCardData[0][cbIndex])>cbLogicValue)
          {
            //���ý��
            OutCardResult.cbCardCount=cbTurnCardCount;
            CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[0][cbIndex],sizeof(BYTE)*cbTurnCardCount);

            return true;
          }
        }
      }

      //Ѱ�Ҷ���
      if(cbTurnCardCount<=2)
      {
        for(BYTE i=0; i<AnalyseResult.cbBlockCount[1]; i++)
        {
          BYTE cbIndex=(AnalyseResult.cbBlockCount[1]-i-1)*2;
          if(GetCardLogicValue(AnalyseResult.cbCardData[1][cbIndex])>cbLogicValue)
          {
            //���ý��
            OutCardResult.cbCardCount=cbTurnCardCount;
            CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[1][cbIndex],sizeof(BYTE)*cbTurnCardCount);

            return true;
          }
        }
      }

      //Ѱ������
      if(cbTurnCardCount<=3)
      {
        for(BYTE i=0; i<AnalyseResult.cbBlockCount[2]; i++)
        {
          BYTE cbIndex=(AnalyseResult.cbBlockCount[2]-i-1)*3;
          if(GetCardLogicValue(AnalyseResult.cbCardData[2][cbIndex])>cbLogicValue)
          {
            //���ý��
            OutCardResult.cbCardCount=cbTurnCardCount;
            CopyMemory(OutCardResult.cbResultCard,&AnalyseResult.cbCardData[2][cbIndex],sizeof(BYTE)*cbTurnCardCount);

            return true;
          }
        }
      }

      break;
    }
    case CT_SINGLE_LINE:    //��������
    {
      //�����ж�
      if(cbCardCount<cbTurnCardCount)
      {
        break;
      }

      //��ȡ��ֵ
      BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

      //��������
      for(BYTE i=(cbTurnCardCount-1); i<cbCardCount; i++)
      {
        //��ȡ��ֵ
        BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

        //�����ж�
        if(cbHandLogicValue>=15)
        {
          break;
        }
        if(cbHandLogicValue<=cbLogicValue)
        {
          continue;
        }

        //��������
        BYTE cbLineCount=0;
        for(BYTE j=(cbCardCount-i-1); j<cbCardCount; j++)
        {
          if((GetCardLogicValue(cbCardData[j])+cbLineCount)==cbHandLogicValue)
          {
            //��������
            OutCardResult.cbResultCard[cbLineCount++]=cbCardData[j];

            //����ж�
            if(cbLineCount==cbTurnCardCount)
            {
              OutCardResult.cbCardCount=cbTurnCardCount;
              return true;
            }
          }
        }
      }

      break;
    }
    case CT_DOUBLE_LINE:    //��������
    {
      //�����ж�
      if(cbCardCount<cbTurnCardCount)
      {
        break;
      }

      //��ȡ��ֵ
      BYTE cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);

      //��������
      for(BYTE i=(cbTurnCardCount-1); i<cbCardCount; i++)
      {
        //��ȡ��ֵ
        BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

        //�����ж�
        if(cbHandLogicValue<=cbLogicValue)
        {
          continue;
        }
        if((cbTurnCardCount>1)&&(cbHandLogicValue>=15))
        {
          break;
        }

        //��������
        BYTE cbLineCount=0;
        for(BYTE j=(cbCardCount-i-1); j<(cbCardCount-1); j++)
        {
          if(((GetCardLogicValue(cbCardData[j])+cbLineCount)==cbHandLogicValue)
             &&((GetCardLogicValue(cbCardData[j+1])+cbLineCount)==cbHandLogicValue))
          {
            //��������
            OutCardResult.cbResultCard[cbLineCount*2]=cbCardData[j];
            OutCardResult.cbResultCard[(cbLineCount++)*2+1]=cbCardData[j+1];

            //����ж�
            if(cbLineCount*2==cbTurnCardCount)
            {
              OutCardResult.cbCardCount=cbTurnCardCount;
              return true;
            }
          }
        }
      }

      break;
    }
    case CT_THREE_LINE:       //��������
    case CT_THREE_TAKE_ONE: //����һ��
    case CT_THREE_TAKE_TWO: //����һ��
    {
      //�����ж�
      if(cbCardCount<cbTurnCardCount)
      {
        break;
      }

      //��ȡ��ֵ
      BYTE cbLogicValue=0;
      for(BYTE i=0; i<cbTurnCardCount-2; i++)
      {
        cbLogicValue=GetCardLogicValue(cbTurnCardData[i]);
        if(GetCardLogicValue(cbTurnCardData[i+1])!=cbLogicValue)
        {
          continue;
        }
        if(GetCardLogicValue(cbTurnCardData[i+2])!=cbLogicValue)
        {
          continue;
        }
        break;
      }

      //������ֵ
      BYTE cbTurnLineCount=0;
      if(cbTurnOutType==CT_THREE_TAKE_ONE)
      {
        cbTurnLineCount=cbTurnCardCount/4;
      }
      else if(cbTurnOutType==CT_THREE_TAKE_TWO)
      {
        cbTurnLineCount=cbTurnCardCount/5;
      }
      else
      {
        cbTurnLineCount=cbTurnCardCount/3;
      }

      //��������
      for(BYTE i=cbTurnLineCount*3-1; i<cbCardCount; i++)
      {
        //��ȡ��ֵ
        BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

        //�����ж�
        if(cbHandLogicValue<=cbLogicValue)
        {
          continue;
        }
        if((cbTurnLineCount>1)&&(cbHandLogicValue>=15))
        {
          break;
        }

        //��������
        BYTE cbLineCount=0;
        for(BYTE j=(cbCardCount-i-1); j<(cbCardCount-2); j++)
        {
          //���ñ���
          OutCardResult.cbCardCount=0;

          //�����ж�
          if((GetCardLogicValue(cbCardData[j])+cbLineCount)!=cbHandLogicValue)
          {
            continue;
          }
          if((GetCardLogicValue(cbCardData[j+1])+cbLineCount)!=cbHandLogicValue)
          {
            continue;
          }
          if((GetCardLogicValue(cbCardData[j+2])+cbLineCount)!=cbHandLogicValue)
          {
            continue;
          }

          //��������
          OutCardResult.cbResultCard[cbLineCount*3]=cbCardData[j];
          OutCardResult.cbResultCard[cbLineCount*3+1]=cbCardData[j+1];
          OutCardResult.cbResultCard[(cbLineCount++)*3+2]=cbCardData[j+2];

          //����ж�
          if(cbLineCount==cbTurnLineCount)
          {
            //��������
            OutCardResult.cbCardCount=cbLineCount*3;

            //�����˿�
            BYTE cbLeftCardData[MAX_COUNT];
            BYTE cbLeftCount=cbCardCount-OutCardResult.cbCardCount;
            CopyMemory(cbLeftCardData,cbCardData,sizeof(BYTE)*cbCardCount);
            RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,cbLeftCardData,cbCardCount);

            //�����˿�
            tagAnalyseResult AnalyseResultLeft;
            AnalysebCardData(cbLeftCardData,cbLeftCount,AnalyseResultLeft);

            //���ƴ���
            if(cbTurnOutType==CT_THREE_TAKE_ONE)
            {
              //��ȡ����
              for(BYTE k=0; k<AnalyseResultLeft.cbBlockCount[0]; k++)
              {
                //��ֹ�ж�
                if(OutCardResult.cbCardCount==cbTurnCardCount)
                {
                  break;
                }

                //�����˿�
                BYTE cbIndex=AnalyseResultLeft.cbBlockCount[0]-k-1;
                BYTE cbSignedCard=AnalyseResultLeft.cbCardData[0][cbIndex];
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
              }

              //��ȡ����
              for(BYTE k=0; k<AnalyseResultLeft.cbBlockCount[1]*2; k++)
              {
                //��ֹ�ж�
                if(OutCardResult.cbCardCount==cbTurnCardCount)
                {
                  break;
                }

                //�����˿�
                BYTE cbIndex=(AnalyseResultLeft.cbBlockCount[1]*2-k-1);
                BYTE cbSignedCard=AnalyseResultLeft.cbCardData[1][cbIndex];
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
              }

              //��ȡ����
              for(BYTE k=0; k<AnalyseResultLeft.cbBlockCount[2]*3; k++)
              {
                //��ֹ�ж�
                if(OutCardResult.cbCardCount==cbTurnCardCount)
                {
                  break;
                }

                //�����˿�
                BYTE cbIndex=(AnalyseResultLeft.cbBlockCount[2]*3-k-1);
                BYTE cbSignedCard=AnalyseResultLeft.cbCardData[2][cbIndex];
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
              }

              //��ȡ����
              for(BYTE k=0; k<AnalyseResultLeft.cbBlockCount[3]*4; k++)
              {
                //��ֹ�ж�
                if(OutCardResult.cbCardCount==cbTurnCardCount)
                {
                  break;
                }

                //�����˿�
                BYTE cbIndex=(AnalyseResultLeft.cbBlockCount[3]*4-k-1);
                BYTE cbSignedCard=AnalyseResultLeft.cbCardData[3][cbIndex];
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbSignedCard;
              }
            }

            //���ƴ���
            if(cbTurnOutType==CT_THREE_TAKE_TWO)
            {
              //��ȡ����
              for(BYTE k=0; k<AnalyseResultLeft.cbBlockCount[1]; k++)
              {
                //��ֹ�ж�
                if(OutCardResult.cbCardCount==cbTurnCardCount)
                {
                  break;
                }

                //�����˿�
                BYTE cbIndex=(AnalyseResultLeft.cbBlockCount[1]-k-1)*2;
                BYTE cbCardData1=AnalyseResultLeft.cbCardData[1][cbIndex];
                BYTE cbCardData2=AnalyseResultLeft.cbCardData[1][cbIndex+1];
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
              }

              //��ȡ����
              for(BYTE k=0; k<AnalyseResultLeft.cbBlockCount[2]; k++)
              {
                //��ֹ�ж�
                if(OutCardResult.cbCardCount==cbTurnCardCount)
                {
                  break;
                }

                //�����˿�
                BYTE cbIndex=(AnalyseResultLeft.cbBlockCount[2]-k-1)*3;
                BYTE cbCardData1=AnalyseResultLeft.cbCardData[2][cbIndex];
                BYTE cbCardData2=AnalyseResultLeft.cbCardData[2][cbIndex+1];
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
              }

              //��ȡ����
              for(BYTE k=0; k<AnalyseResultLeft.cbBlockCount[3]; k++)
              {
                //��ֹ�ж�
                if(OutCardResult.cbCardCount==cbTurnCardCount)
                {
                  break;
                }

                //�����˿�
                BYTE cbIndex=(AnalyseResultLeft.cbBlockCount[3]-k-1)*4;
                BYTE cbCardData1=AnalyseResultLeft.cbCardData[3][cbIndex];
                BYTE cbCardData2=AnalyseResultLeft.cbCardData[3][cbIndex+1];
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData1;
                OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbCardData2;
              }
            }

            //����ж�
            if(OutCardResult.cbCardCount==cbTurnCardCount)
            {
              return true;
            }
          }
        }
      }

      break;
    }
  }

  //����ը��
  if((cbCardCount>=4)&&(cbTurnOutType!=CT_MISSILE_CARD))
  {
    //��������
    BYTE cbLogicValue=0;
    if(cbTurnOutType==CT_BOMB_CARD)
    {
      cbLogicValue=GetCardLogicValue(cbTurnCardData[0]);
    }

    //����ը��
    for(BYTE i=3; i<cbCardCount; i++)
    {
      //��ȡ��ֵ
      BYTE cbHandLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);

      //�����ж�
      if(cbHandLogicValue<=cbLogicValue)
      {
        continue;
      }

      //ը���ж�
      BYTE cbTempLogicValue=GetCardLogicValue(cbCardData[cbCardCount-i-1]);
	  BYTE j=1;
      for(; j<4; j++)
      {
        if(GetCardLogicValue(cbCardData[cbCardCount+j-i-1])!=cbTempLogicValue)
        {
          break;
        }
      }
      if(j!=4)
      {
        continue;
      }

      //���ý��
      OutCardResult.cbCardCount=4;
      OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-i-1];
      OutCardResult.cbResultCard[1]=cbCardData[cbCardCount-i];
      OutCardResult.cbResultCard[2]=cbCardData[cbCardCount-i+1];
      OutCardResult.cbResultCard[3]=cbCardData[cbCardCount-i+2];

      return true;
    }
  }

  //�������
  if((cbCardCount>=2)&&(cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E))
  {
    //���ý��
    OutCardResult.cbCardCount=2;
    OutCardResult.cbResultCard[0]=cbCardData[0];
    OutCardResult.cbResultCard[1]=cbCardData[1];

    return true;
  }

  return false;
}

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
  return (cbColorIndex<<4)|(cbValueIndex+1);
}

//�����˿�
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
  //���ý��
  ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

  //�˿˷���
  for(BYTE i=0; i<cbCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1,cbCardValueTemp=0;
    BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

    //����ͬ��
    for(BYTE j=i+1; j<cbCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }

    //���ý��
    BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
    for(BYTE j=0; j<cbSameCount; j++)
    {
      AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];
    }

    //��������
    i+=cbSameCount-1;
  }

  return;
}

//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
  //���ñ���
  ZeroMemory(&Distributing,sizeof(Distributing));

  //���ñ���
  for(BYTE i=0; i<cbCardCount; i++)
  {
    if(cbCardData[i]==0)
    {
      continue;
    }

    //��ȡ����
    BYTE cbCardColor=GetCardColor(cbCardData[i]);
    BYTE cbCardValue=GetCardValue(cbCardData[i]);

    //�ֲ���Ϣ
    Distributing.cbCardCount++;
    Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;
    Distributing.cbDistributing[cbCardValue-1][cbCardColor>>4]++;
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////����ΪAI����

//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, WORD wOutCardUser, WORD wMeChairID, tagOutCardResult & OutCardResult)
{
  //����ж�
  WORD wUndersideOfBanker = (m_wBankerUser+1)%GAME_PLAYER ; //�����¼�
  WORD wUpsideOfBanker = (wUndersideOfBanker+1)%GAME_PLAYER ; //�����ϼ�

  //��ʼ����
  ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;

  //�ȳ���
  if(cbTurnCardCount==0)
  {
    //��������
    if(wMeChairID==m_wBankerUser)
    {
      BankerOutCard(cbHandCardData, cbHandCardCount, OutCardResult) ;
    }
    //�����¼�
    else if(wMeChairID==wUndersideOfBanker)
    {
      UndersideOfBankerOutCard(cbHandCardData, cbHandCardCount,wMeChairID,  OutCardResult) ;
    }
    //�����ϼ�
    else if(wMeChairID==wUpsideOfBanker)
    {
      UpsideOfBankerOutCard(cbHandCardData, cbHandCardCount, wMeChairID, OutCardResult) ;
    }
    //���� ID
    else
    {
      ASSERT(false) ;
    }
  }
  //ѹ��
  else
  {
    //��������
    if(wMeChairID==m_wBankerUser)
    {
      BankerOutCard(cbHandCardData, cbHandCardCount, wOutCardUser, cbTurnCardData, cbTurnCardCount, OutCardResult) ;
    }
    //�����¼�
    else if(wMeChairID==wUndersideOfBanker)
    {
      UndersideOfBankerOutCard(cbHandCardData, cbHandCardCount, wOutCardUser, cbTurnCardData, cbTurnCardCount, OutCardResult) ;
    }
    //�����ϼ�
    else if(wMeChairID==wUpsideOfBanker)
    {
      UpsideOfBankerOutCard(cbHandCardData, cbHandCardCount, wOutCardUser, cbTurnCardData, cbTurnCardCount, OutCardResult) ;
    }
    //���� ID
    else
    {
      ASSERT(false) ;
    }

  }
  return true ;
}

//����ը��
VOID CGameLogic::GetAllBomCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbBomCardData[], BYTE &cbBomCardCount)
{
  BYTE cbTmpCardData[MAX_COUNT] ;
  CopyMemory(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

  //��С����
  SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

  cbBomCardCount = 0 ;

  if(cbHandCardCount<2)
  {
    return ;
  }

  //˫��ը��
  if(0x4F==cbTmpCardData[0] && 0x4E==cbTmpCardData[1])
  {
    cbBomCardData[cbBomCardCount++] = cbTmpCardData[0] ;
    cbBomCardData[cbBomCardCount++] = cbTmpCardData[1] ;
  }

  //�˿˷���
  for(BYTE i=0; i<cbHandCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1;
    BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

    //����ͬ��
    for(BYTE j=i+1; j<cbHandCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }

    if(4==cbSameCount)
    {
      cbBomCardData[cbBomCardCount++] = cbTmpCardData[i] ;
      cbBomCardData[cbBomCardCount++] = cbTmpCardData[i+1] ;
      cbBomCardData[cbBomCardCount++] = cbTmpCardData[i+2] ;
      cbBomCardData[cbBomCardCount++] = cbTmpCardData[i+3] ;
    }

    //��������
    i+=cbSameCount-1;
  }
}

//����˳��
VOID CGameLogic::GetAllLineCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbLineCardData[], BYTE &cbLineCardCount)
{
  BYTE cbTmpCard[MAX_COUNT] ;
  CopyMemory(cbTmpCard, cbHandCardData, cbHandCardCount) ;
  //��С����
  SortCardList(cbTmpCard, cbHandCardCount, ST_ORDER) ;

  cbLineCardCount = 0 ;

  //����У��
  if(cbHandCardCount<5)
  {
    return ;
  }

  BYTE cbFirstCard = 0 ;
  //ȥ��2����
  for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardLogicValue(cbTmpCard[i])<15)
    {
      cbFirstCard = i ;
      break ;
    }

  BYTE cbSingleLineCard[12] ;
  BYTE cbSingleLineCount=0 ;
  BYTE cbLeftCardCount = cbHandCardCount ;
  bool bFindSingleLine = true ;

  //�����ж�
  while(cbLeftCardCount>=5 && bFindSingleLine)
  {
    cbSingleLineCount=1 ;
    bFindSingleLine = false ;
    BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
    cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[cbFirstCard] ;
    for(BYTE i=cbFirstCard+1; i<cbLeftCardCount; i++)
    {
      BYTE cbCardData=cbTmpCard[i];

      //�����ж�
      if(1!=(GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbCardData)) && GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
      {
        cbLastCard = cbTmpCard[i] ;
        if(cbSingleLineCount<5)
        {
          cbSingleLineCount = 1 ;
          cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[i] ;
          continue ;
        }
        else
        {
          break ;
        }
      }
      //ͬ���ж�
      else if(GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
      {
        cbLastCard = cbCardData ;
        cbSingleLineCard[cbSingleLineCount] = cbCardData ;
        ++cbSingleLineCount ;
      }
    }

    //��������
    if(cbSingleLineCount>=5)
    {
      RemoveCard(cbSingleLineCard, cbSingleLineCount, cbTmpCard, cbLeftCardCount) ;
      memcpy(cbLineCardData+cbLineCardCount , cbSingleLineCard, sizeof(BYTE)*cbSingleLineCount) ;
      cbLineCardCount += cbSingleLineCount ;
      cbLeftCardCount -= cbSingleLineCount ;
      bFindSingleLine = true ;
    }
  }
}

//��������
VOID CGameLogic::GetAllThreeCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbThreeCardData[], BYTE &cbThreeCardCount)
{
  BYTE cbTmpCardData[MAX_COUNT] ;
  CopyMemory(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

  //��С����
  SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

  cbThreeCardCount = 0 ;

  //�˿˷���
  for(BYTE i=0; i<cbHandCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1;
    BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

    //����ͬ��
    for(BYTE j=i+1; j<cbHandCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }

    if(cbSameCount>=3)
    {
      cbThreeCardData[cbThreeCardCount++] = cbTmpCardData[i] ;
      cbThreeCardData[cbThreeCardCount++] = cbTmpCardData[i+1] ;
      cbThreeCardData[cbThreeCardCount++] = cbTmpCardData[i+2] ;
    }

    //��������
    i+=cbSameCount-1;
  }
}

//��������
VOID CGameLogic::GetAllDoubleCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbDoubleCardData[], BYTE &cbDoubleCardCount)
{
  BYTE cbTmpCardData[MAX_COUNT] ;
  CopyMemory(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

  //��С����
  SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

  cbDoubleCardCount = 0 ;

  //�˿˷���
  for(BYTE i=0; i<cbHandCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1;
    BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

    //����ͬ��
    for(BYTE j=i+1; j<cbHandCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }

    if(cbSameCount>=2)
    {
      cbDoubleCardData[cbDoubleCardCount++] = cbTmpCardData[i] ;
      cbDoubleCardData[cbDoubleCardCount++] = cbTmpCardData[i+1] ;
    }

    //��������
    i+=cbSameCount-1;
  }
}

//��������
VOID CGameLogic::GetAllSingleCard(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE cbSingleCardData[], BYTE &cbSingleCardCount)
{
  cbSingleCardCount =0 ;

  BYTE cbTmpCardData[MAX_COUNT] ;
  CopyMemory(cbTmpCardData, cbHandCardData, cbHandCardCount) ;

  //��С����
  SortCardList(cbTmpCardData, cbHandCardCount, ST_ORDER);

  //�˿˷���
  for(BYTE i=0; i<cbHandCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1;
    BYTE cbLogicValue=GetCardLogicValue(cbTmpCardData[i]);

    //����ͬ��
    for(BYTE j=i+1; j<cbHandCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbTmpCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }

    if(cbSameCount==1)
    {
      cbSingleCardData[cbSingleCardCount++] = cbTmpCardData[i] ;
    }

    //��������
    i+=cbSameCount-1;
  }
}

//��������
VOID CGameLogic::AnalyseOutCardType(BYTE const cbHandCardData[], BYTE const cbHandCardCount, tagOutCardTypeResult CardTypeResult[12+1])
{
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult[0])*12) ;
  BYTE cbTmpCardData[MAX_COUNT] ;
  //�����˿ˣ���ֹ����ʱ�ı��˿�
  BYTE cbReserveCardData[MAX_COUNT] ;
  CopyMemory(cbReserveCardData, cbHandCardData, cbHandCardCount) ;
  SortCardList(cbReserveCardData, cbHandCardCount, ST_ORDER) ;
  CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

  //��������
  for(BYTE i=0; i<cbHandCardCount; ++i)
  {
    BYTE Index = CardTypeResult[CT_SINGLE].cbCardTypeCount ;
    CardTypeResult[CT_SINGLE].cbCardType = CT_SINGLE ;
    CardTypeResult[CT_SINGLE].cbCardData[Index][0] = cbTmpCardData[i] ;
    CardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] = 1 ;
    CardTypeResult[CT_SINGLE].cbCardTypeCount++ ;

    ASSERT(CardTypeResult[CT_SINGLE].cbCardTypeCount<MAX_TYPE_COUNT) ;
  }

  //��������
  {
    BYTE cbDoubleCardData[MAX_COUNT] ;
    BYTE cbDoubleCardcount=0;
    GetAllDoubleCard(cbTmpCardData, cbHandCardCount, cbDoubleCardData, cbDoubleCardcount) ;
    for(BYTE i=0; i<cbDoubleCardcount; i+=2)
    {
      BYTE Index = CardTypeResult[CT_DOUBLE].cbCardTypeCount ;
      CardTypeResult[CT_DOUBLE].cbCardType = CT_DOUBLE ;
      CardTypeResult[CT_DOUBLE].cbCardData[Index][0] = cbDoubleCardData[i] ;
      CardTypeResult[CT_DOUBLE].cbCardData[Index][1] = cbDoubleCardData[i+1] ;
      CardTypeResult[CT_DOUBLE].cbEachHandCardCount[Index] = 2 ;
      CardTypeResult[CT_DOUBLE].cbCardTypeCount++ ;

      ASSERT(CardTypeResult[CT_DOUBLE].cbCardTypeCount<MAX_TYPE_COUNT) ;
    }
  }

  //��������
  {
    BYTE cbThreeCardData[MAX_COUNT];
    BYTE cbThreeCardCount=0 ;
    GetAllThreeCard(cbTmpCardData, cbHandCardCount, cbThreeCardData, cbThreeCardCount) ;
    for(BYTE i=0; i<cbThreeCardCount; i+=3)
    {
      BYTE Index = CardTypeResult[CT_THREE].cbCardTypeCount ;
      CardTypeResult[CT_THREE].cbCardType = CT_THREE ;
      CardTypeResult[CT_THREE].cbCardData[Index][0] = cbThreeCardData[i] ;
      CardTypeResult[CT_THREE].cbCardData[Index][1] = cbThreeCardData[i+1] ;
      CardTypeResult[CT_THREE].cbCardData[Index][2] = cbThreeCardData[i+2] ;
      CardTypeResult[CT_THREE].cbEachHandCardCount[Index] = 3 ;
      CardTypeResult[CT_THREE].cbCardTypeCount++ ;

      ASSERT(CardTypeResult[CT_THREE].cbCardTypeCount<MAX_TYPE_COUNT) ;
    }
  }

  //ը������
  {
    BYTE cbFourCardData[MAX_COUNT];
    BYTE cbFourCardCount=0 ;
    if(cbHandCardCount>=2 && 0x4F==cbTmpCardData[0] && 0x4E==cbTmpCardData[1])
    {
      BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
      CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = cbTmpCardData[0] ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = cbTmpCardData[1] ;
      CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 2 ;
      CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;
      GetAllBomCard(cbTmpCardData+2, cbHandCardCount-2, cbFourCardData, cbFourCardCount) ;
    }
    else
    {
      GetAllBomCard(cbTmpCardData, cbHandCardCount, cbFourCardData, cbFourCardCount) ;
    }
    for(BYTE i=0; i<cbFourCardCount; i+=4)
    {
      BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
      CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = cbFourCardData[i] ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = cbFourCardData[i+1] ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][2] = cbFourCardData[i+2] ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][3] = cbFourCardData[i+3] ;
      CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4 ;
      CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;

      ASSERT(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<MAX_TYPE_COUNT) ;
    }
  }
  //��������
  {
    //�ָ��˿ˣ���ֹ����ʱ�ı��˿�
    CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

    BYTE cbFirstCard = 0 ;
    //ȥ��2����
    for(BYTE i=0 ; i<cbHandCardCount ; ++i)
    {
      if(GetCardLogicValue(cbTmpCardData[i])<15)
      {
        cbFirstCard = i ;
        break ;
      }
    }

    BYTE cbSingleLineCard[12] ;
    BYTE cbSingleLineCount=1 ;
    BYTE cbLeftCardCount = cbHandCardCount ;
    bool bFindSingleLine = true ;

    //�����ж�
    while(cbLeftCardCount>=5 && bFindSingleLine)
    {
      cbSingleLineCount=1 ;
      bFindSingleLine = false ;
      BYTE cbLastCard = cbTmpCardData[cbFirstCard] ;
      cbSingleLineCard[cbSingleLineCount-1] = cbTmpCardData[cbFirstCard] ;
      for(BYTE i=cbFirstCard+1; i<cbLeftCardCount; i++)
      {
        BYTE cbCardData=cbTmpCardData[i];

        //�����ж�
        if(1!=(GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbCardData)) && GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
        {
          cbLastCard = cbTmpCardData[i] ;
          //�Ƿ�Ϸ�
          if(cbSingleLineCount<5)
          {
            cbSingleLineCount = 1 ;
            cbSingleLineCard[cbSingleLineCount-1] = cbTmpCardData[i] ;
            continue ;
          }
          else
          {
            break ;
          }
        }
        //ͬ���ж�
        else if(GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
        {
          cbLastCard = cbCardData ;
          cbSingleLineCard[cbSingleLineCount] = cbCardData ;
          ++cbSingleLineCount ;
        }
      }

      //��������
      if(cbSingleLineCount>=5)
      {
        BYTE Index ;
        //��������
        BYTE cbStart=0 ;
        //�Ӵ�С
        while(cbSingleLineCount-cbStart>=5)
        {
          Index = CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount ;
          BYTE cbThisLineCount = cbSingleLineCount-cbStart ;
          CardTypeResult[CT_SINGLE_LINE].cbCardType = CT_SINGLE_LINE ;
          CopyMemory(CardTypeResult[CT_SINGLE_LINE].cbCardData[Index], cbSingleLineCard+cbStart, sizeof(BYTE)*(cbThisLineCount));
          CardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[Index] = cbThisLineCount;
          CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;
          cbStart++ ;
        }
        //��С����
        cbStart=1 ;
        while(cbSingleLineCount-cbStart>=5)
        {
          Index = CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount ;
          BYTE cbThisLineCount = cbSingleLineCount-cbStart ;
          CardTypeResult[CT_SINGLE_LINE].cbCardType = CT_SINGLE_LINE ;
          CopyMemory(CardTypeResult[CT_SINGLE_LINE].cbCardData[Index], cbSingleLineCard, sizeof(BYTE)*(cbThisLineCount));
          CardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[Index] = cbThisLineCount;
          CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;
          cbStart++ ;
        }

        RemoveCard(cbSingleLineCard, cbSingleLineCount, cbTmpCardData, cbLeftCardCount) ;
        cbLeftCardCount -= cbSingleLineCount ;
        bFindSingleLine = true ;
      }
    }

  }

  //��������
  {
    //�ָ��˿ˣ���ֹ����ʱ�ı��˿�
    CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

    //�����ж�
    BYTE cbFirstCard = 0 ;
    //ȥ��2����
    for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardLogicValue(cbTmpCardData[i])<15)
      {
        cbFirstCard = i ;
        break ;
      }

    BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
    bool bFindDoubleLine = true ;
    BYTE cbDoubleLineCount = 0 ;
    BYTE cbDoubleLineCard[24] ;
    //��ʼ�ж�
    while(cbLeftCardCount>=6 && bFindDoubleLine)
    {
      BYTE cbLastCard = cbTmpCardData[cbFirstCard] ;
      BYTE cbSameCount = 1 ;
      cbDoubleLineCount = 0 ;
      bFindDoubleLine=false ;
      for(BYTE i=cbFirstCard+1 ; i<cbLeftCardCount+cbFirstCard ; ++i)
      {
        //����ͬ��
        while(GetCardLogicValue(cbLastCard)==GetCardLogicValue(cbTmpCardData[i]) && i<cbLeftCardCount+cbFirstCard)
        {
          ++cbSameCount;
          ++i ;
        }

        BYTE cbLastDoubleCardValue ;
        if(cbDoubleLineCount>0)
        {
          cbLastDoubleCardValue = GetCardLogicValue(cbDoubleLineCard[cbDoubleLineCount-1]) ;
        }
        //���¿�ʼ
        if((cbSameCount<2 || (cbDoubleLineCount>0 && (cbLastDoubleCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
        {
          if(cbDoubleLineCount>=6)
          {
            break ;
          }
          //����
          if(cbSameCount>=2)
          {
            i-=cbSameCount ;
          }
          cbLastCard = cbTmpCardData[i] ;
          cbDoubleLineCount = 0 ;
        }
        //��������
        else if(cbSameCount>=2)
        {
          cbDoubleLineCard[cbDoubleLineCount] = cbTmpCardData[i-cbSameCount] ;
          cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCardData[i-cbSameCount+1] ;
          cbDoubleLineCount += 2 ;

          //��β�ж�
          if(i==(cbLeftCardCount+cbFirstCard-2))
            if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCardData[i]))==1 && (GetCardLogicValue(cbTmpCardData[i])==GetCardLogicValue(cbTmpCardData[i+1])))
            {
              cbDoubleLineCard[cbDoubleLineCount] = cbTmpCardData[i] ;
              cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCardData[i+1] ;
              cbDoubleLineCount += 2 ;
              break ;
            }

        }

        cbLastCard = cbTmpCardData[i] ;
        cbSameCount = 1 ;
      }

      //��������
      if(cbDoubleLineCount>=6)
      {
        BYTE Index ;

        Index = CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount ;
        CardTypeResult[CT_DOUBLE_LINE].cbCardType = CT_DOUBLE_LINE ;
        CopyMemory(CardTypeResult[CT_DOUBLE_LINE].cbCardData[Index], cbDoubleLineCard, sizeof(BYTE)*cbDoubleLineCount);
        CardTypeResult[CT_DOUBLE_LINE].cbEachHandCardCount[Index] = cbDoubleLineCount;
        CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount++ ;

        ASSERT(CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;

        RemoveCard(cbDoubleLineCard, cbDoubleLineCount, cbTmpCardData, cbFirstCard+cbLeftCardCount) ;
        bFindDoubleLine=true ;
        cbLeftCardCount -= cbDoubleLineCount ;
      }
    }
  }

  //��������
  {
    //�ָ��˿ˣ���ֹ����ʱ�ı��˿�
    CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

    //�����ж�
    BYTE cbFirstCard = 0 ;
    //ȥ��2����
    for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardLogicValue(cbTmpCardData[i])<15)
      {
        cbFirstCard = i ;
        break ;
      }

    BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
    bool bFindThreeLine = true ;
    BYTE cbThreeLineCount = 0 ;
    BYTE cbThreeLineCard[20] ;
    //��ʼ�ж�
    while(cbLeftCardCount>=6 && bFindThreeLine)
    {
      BYTE cbLastCard = cbTmpCardData[cbFirstCard] ;
      BYTE cbSameCount = 1 ;
      cbThreeLineCount = 0 ;
      bFindThreeLine = false ;
      for(BYTE i=cbFirstCard+1 ; i<cbLeftCardCount+cbFirstCard ; ++i)
      {
        //����ͬ��
        while(GetCardLogicValue(cbLastCard)==GetCardLogicValue(cbTmpCardData[i]) && i<cbLeftCardCount+cbFirstCard)
        {
          ++cbSameCount;
          ++i ;
        }

        BYTE cbLastThreeCardValue ;
        if(cbThreeLineCount>0)
        {
          cbLastThreeCardValue = GetCardLogicValue(cbThreeLineCard[cbThreeLineCount-1]) ;
        }

        //���¿�ʼ
        if((cbSameCount<3 || (cbThreeLineCount>0&&(cbLastThreeCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
        {
          if(cbThreeLineCount>=6)
          {
            break ;
          }

          if(cbSameCount>=3)
          {
            i-=cbSameCount ;
          }
          cbLastCard = cbTmpCardData[i] ;
          cbThreeLineCount = 0 ;
        }
        //��������
        else if(cbSameCount>=3)
        {
          cbThreeLineCard[cbThreeLineCount] = cbTmpCardData[i-cbSameCount] ;
          cbThreeLineCard[cbThreeLineCount+1] = cbTmpCardData[i-cbSameCount+1] ;
          cbThreeLineCard[cbThreeLineCount+2] = cbTmpCardData[i-cbSameCount+2] ;
          cbThreeLineCount += 3 ;

          //��β�ж�
          if(i==(cbLeftCardCount+cbFirstCard-3))
            if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCardData[i]))==1 && (GetCardLogicValue(cbTmpCardData[i])==GetCardLogicValue(cbTmpCardData[i+1])) && (GetCardLogicValue(cbTmpCardData[i])==GetCardLogicValue(cbTmpCardData[i+2])))
            {
              cbThreeLineCard[cbThreeLineCount] = cbTmpCardData[i] ;
              cbThreeLineCard[cbThreeLineCount+1] = cbTmpCardData[i+1] ;
              cbThreeLineCard[cbThreeLineCount+2] = cbTmpCardData[i+2] ;
              cbThreeLineCount += 3 ;
              break ;
            }

        }

        cbLastCard = cbTmpCardData[i] ;
        cbSameCount = 1 ;
      }

      //��������
      if(cbThreeLineCount>=6)
      {
        BYTE Index ;

        Index = CardTypeResult[CT_THREE_LINE].cbCardTypeCount ;
        CardTypeResult[CT_THREE_LINE].cbCardType = CT_THREE_LINE ;
        CopyMemory(CardTypeResult[CT_THREE_LINE].cbCardData[Index], cbThreeLineCard, sizeof(BYTE)*cbThreeLineCount);
        CardTypeResult[CT_THREE_LINE].cbEachHandCardCount[Index] = cbThreeLineCount;
        CardTypeResult[CT_THREE_LINE].cbCardTypeCount++ ;

        ASSERT(CardTypeResult[CT_THREE_LINE].cbCardTypeCount<MAX_TYPE_COUNT) ;

        RemoveCard(cbThreeLineCard, cbThreeLineCount, cbTmpCardData, cbFirstCard+cbLeftCardCount) ;
        bFindThreeLine=true ;
        cbLeftCardCount -= cbThreeLineCount ;
      }
    }

  }
  //����һ��
  {
    //�ָ��˿ˣ���ֹ����ʱ�ı��˿�
    CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

    BYTE cbHandThreeCard[MAX_COUNT];
    BYTE cbHandThreeCount=0 ;

    //�Ƴ�ը��
    BYTE cbAllBomCardData[MAX_COUNT] ;
    BYTE cbAllBomCardCount=0 ;
    GetAllBomCard(cbTmpCardData, cbHandCardCount, cbAllBomCardData, cbAllBomCardCount) ;
    RemoveCard(cbAllBomCardData, cbAllBomCardCount, cbTmpCardData, cbHandCardCount) ;

    GetAllThreeCard(cbTmpCardData, cbHandCardCount-cbAllBomCardCount, cbHandThreeCard, cbHandThreeCount) ;

    {
      BYTE Index ;
      //ȥ������
      BYTE cbRemainCardData[MAX_COUNT] ;
      CopyMemory(cbRemainCardData, cbTmpCardData, cbHandCardCount-cbAllBomCardCount) ;
      BYTE cbRemainCardCount=cbHandCardCount-cbAllBomCardCount-cbHandThreeCount ;
      RemoveCard(cbHandThreeCard, cbHandThreeCount, cbRemainCardData, cbHandCardCount-cbAllBomCardCount) ;
      //������һ��
      for(BYTE i=0; i<cbHandThreeCount; i+=3)
      {
        //������һ��
        for(BYTE j=0; j<cbRemainCardCount; ++j)
        {
          Index = CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount ;
          CardTypeResult[CT_THREE_TAKE_ONE].cbCardType = CT_THREE_TAKE_ONE ;
          CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index][0] = cbHandThreeCard[i] ;
          CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index][1] = cbHandThreeCard[i+1] ;
          CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index][2] = cbHandThreeCard[i+2] ;
          CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index][3] = cbRemainCardData[j] ;
          CardTypeResult[CT_THREE_TAKE_ONE].cbEachHandCardCount[Index] = 4 ;
          CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount++ ;
        }
      }
    }

    //��������
    BYTE cbLeftThreeCardCount=cbHandThreeCount ;
    bool bFindThreeLine=true ;
    BYTE cbLastIndex=0 ;
    if(GetCardLogicValue(cbHandThreeCard[0])==15)
    {
      cbLastIndex=3 ;
    }
    while(cbLeftThreeCardCount>=6 && bFindThreeLine)
    {
      BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[cbLastIndex]);
      BYTE cbThreeLineCard[MAX_COUNT];
      BYTE cbThreeLineCardCount=3;
      cbThreeLineCard[0]=cbHandThreeCard[cbLastIndex];
      cbThreeLineCard[1]=cbHandThreeCard[cbLastIndex+1];
      cbThreeLineCard[2]=cbHandThreeCard[cbLastIndex+2];

      bFindThreeLine = false ;
      for(BYTE j=3+cbLastIndex; j<cbLeftThreeCardCount; j+=3)
      {
        //�����ж�
        if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))))
        {
          cbLastIndex = j ;
          if(cbLeftThreeCardCount-j>=6)
          {
            bFindThreeLine = true ;
          }

          break;
        }

        cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
        cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
        cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
        cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
        cbThreeLineCardCount += 3;
      }
      if(cbThreeLineCardCount>3)
      {
        BYTE Index ;

        BYTE cbRemainCard[MAX_COUNT];
        BYTE cbRemainCardCount=cbHandCardCount-cbAllBomCardCount-cbHandThreeCount ;


        //�Ƴ���������Ӧ���Ƴ�ը�����ȣ�
        CopyMemory(cbRemainCard, cbTmpCardData, (cbHandCardCount-cbAllBomCardCount)*sizeof(BYTE));
        RemoveCard(cbHandThreeCard, cbHandThreeCount, cbRemainCard, cbHandCardCount-cbAllBomCardCount) ;

        for(BYTE start=0; start<cbThreeLineCardCount-3; start+=3)
        {
          //��˳��Ŀ
          BYTE cbThisTreeLineCardCount = cbThreeLineCardCount-start ;
          //���Ƹ���
          BYTE cbSingleCardCount=(cbThisTreeLineCardCount)/3;

          //���Ʋ���
          if(cbRemainCardCount<cbSingleCardCount)
          {
            continue ;
          }

          //�������
          BYTE cbComCard[5];
          BYTE cbComResCard[254][5] ;
          BYTE cbComResLen=0 ;

          Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, cbSingleCardCount, cbRemainCardCount, cbSingleCardCount);
          for(BYTE i=0; i<cbComResLen; ++i)
          {
            Index = CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount ;
            CardTypeResult[CT_THREE_TAKE_ONE].cbCardType = CT_THREE_TAKE_ONE ;
            //��������
            CopyMemory(CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index], cbThreeLineCard+start, sizeof(BYTE)*cbThisTreeLineCardCount);
            //���浥��
            CopyMemory(CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index]+cbThisTreeLineCardCount, cbComResCard[i], cbSingleCardCount) ;


            CardTypeResult[CT_THREE_TAKE_ONE].cbEachHandCardCount[Index] = cbThisTreeLineCardCount+cbSingleCardCount ;
            CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount++ ;

            ASSERT(CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount<MAX_TYPE_COUNT) ;
          }

        }

        //�Ƴ�����
        bFindThreeLine = true ;
        RemoveCard(cbThreeLineCard, cbThreeLineCardCount, cbHandThreeCard, cbLeftThreeCardCount) ;
        cbLeftThreeCardCount -= cbThreeLineCardCount ;
      }
    }
  }

  //����һ��
  {
    //�ָ��˿ˣ���ֹ����ʱ�ı��˿�
    CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

    BYTE cbHandThreeCard[MAX_COUNT];
    BYTE cbHandThreeCount=0 ;
    BYTE cbRemainCarData[MAX_COUNT] ;
    BYTE cbRemainCardCount=0 ;

    //��ȡ����
    GetAllThreeCard(cbTmpCardData, cbHandCardCount, cbHandThreeCard, cbHandThreeCount) ;

    //�Ƴ���������Ӧ���Ƴ�ը�����ȣ�
    CopyMemory(cbRemainCarData, cbTmpCardData, cbHandCardCount) ;
    RemoveCard(cbHandThreeCard, cbHandThreeCount, cbRemainCarData, cbHandCardCount) ;
    cbRemainCardCount = cbHandCardCount-cbHandThreeCount ;

    //��ȡ����
    BYTE cbAllDoubleCardData[MAX_COUNT] ;
    BYTE cbAllDoubleCardCount=0 ;
    GetAllDoubleCard(cbRemainCarData, cbRemainCardCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;

    //������һ��
    for(BYTE i=0; i<cbHandThreeCount; i+=3)
    {
      BYTE Index ;

      //������һ��
      for(BYTE j=0; j<cbAllDoubleCardCount; j+=2)
      {
        Index = CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbCardType = CT_THREE_TAKE_TWO ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][0] = cbHandThreeCard[i] ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][1] = cbHandThreeCard[i+1] ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][2] = cbHandThreeCard[i+2] ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][3] = cbAllDoubleCardData[j] ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][4] = cbAllDoubleCardData[j+1] ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbEachHandCardCount[Index] = 5 ;
        CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount++ ;
      }
    }

    //��������
    BYTE cbLeftThreeCardCount=cbHandThreeCount ;
    bool bFindThreeLine=true ;
    BYTE cbLastIndex=0 ;
    if(GetCardLogicValue(cbHandThreeCard[0])==15)
    {
      cbLastIndex=3 ;
    }
    while(cbLeftThreeCardCount>=6 && bFindThreeLine)
    {
      BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[cbLastIndex]);
      BYTE cbThreeLineCard[MAX_COUNT];
      BYTE cbThreeLineCardCount=3;
      cbThreeLineCard[0]=cbHandThreeCard[cbLastIndex];
      cbThreeLineCard[1]=cbHandThreeCard[cbLastIndex+1];
      cbThreeLineCard[2]=cbHandThreeCard[cbLastIndex+2];

      bFindThreeLine=false ;
      for(BYTE j=3+cbLastIndex; j<cbLeftThreeCardCount; j+=3)
      {
        //�����ж�
        if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))))
        {
          cbLastIndex = j ;
          if(cbLeftThreeCardCount-j>=6)
          {
            bFindThreeLine = true ;
          }

          break;
        }

        cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
        cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
        cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
        cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
        cbThreeLineCardCount += 3;
      }
      if(cbThreeLineCardCount>3)
      {
        BYTE Index ;

        for(BYTE start=0; start<cbThreeLineCardCount-3; start+=3)
        {
          //��˳��Ŀ
          BYTE cbThisTreeLineCardCount = cbThreeLineCardCount-start ;
          //��������
          BYTE cbDoubleCardCount=((cbThisTreeLineCardCount)/3);

          //���Ʋ���
          if(cbRemainCardCount<cbDoubleCardCount)
          {
            continue ;
          }

          BYTE cbDoubleCardIndex[10]; //�����±�
          for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
          {
            cbDoubleCardIndex[j]=i ;
          }

          //�������
          BYTE cbComCard[5];
          BYTE cbComResCard[254][5] ;
          BYTE cbComResLen=0 ;

          //���ö��Ƶ��±�����ϣ��ٸ����±���ȡ������
          Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, cbDoubleCardCount, cbAllDoubleCardCount/2, cbDoubleCardCount);

          ASSERT(cbComResLen<=254) ;

          for(BYTE i=0; i<cbComResLen; ++i)
          {
            Index = CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount ;
            CardTypeResult[CT_THREE_TAKE_TWO].cbCardType = CT_THREE_TAKE_TWO ;
            //��������
            CopyMemory(CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index], cbThreeLineCard+start, sizeof(BYTE)*cbThisTreeLineCardCount);
            //�������
            for(BYTE j=0, k=0; j<cbDoubleCardCount; ++j, k+=2)
            {
              CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][cbThisTreeLineCardCount+k] = cbAllDoubleCardData[cbComResCard[i][j]];
              CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][cbThisTreeLineCardCount+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
            }

            CardTypeResult[CT_THREE_TAKE_TWO].cbEachHandCardCount[Index] = cbThisTreeLineCardCount+2*cbDoubleCardCount ;
            CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount++ ;

            ASSERT(CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount<MAX_TYPE_COUNT) ;
          }

        }
        //�Ƴ�����
        bFindThreeLine = true ;
        RemoveCard(cbThreeLineCard, cbThreeLineCardCount, cbHandThreeCard, cbLeftThreeCardCount) ;
        cbLeftThreeCardCount -= cbThreeLineCardCount ;
      }
    }
  }
  //�Ĵ�����
  /*
  {
  //�ָ��˿ˣ���ֹ����ʱ�ı��˿�
  CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

  BYTE cbFirstCard = 0 ;
  //ȥ������
  for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardColor(cbTmpCardData[i])!=0x40)  {cbFirstCard = i ; break ;}

  BYTE cbHandAllFourCardData[MAX_COUNT] ;
  BYTE cbHandAllFourCardCount=0;
  //��ȡ����
  GetAllBomCard(cbTmpCardData+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;

  //�Ƴ�����
  BYTE cbRemainCard[MAX_COUNT];
  BYTE cbRemainCardCount=cbHandCardCount-cbHandAllFourCardCount ;
  CopyMemory(cbRemainCard, cbTmpCardData, cbHandCardCount*sizeof(BYTE));
  RemoveCard(cbHandAllFourCardData, cbHandAllFourCardCount, cbRemainCard, cbHandCardCount) ;

  for(BYTE Start=0; Start<cbHandAllFourCardCount; Start += 4)
  {
  BYTE Index ;
  //�������
  BYTE cbComCard[5];
  BYTE cbComResCard[254][5] ;
  BYTE cbComResLen=0 ;
  //�������
  Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, 2, cbRemainCardCount, 2);
  for(BYTE i=0; i<cbComResLen; ++i)
  {
  //���ܴ���
  if(GetCardValue(cbComResCard[i][0])==GetCardValue(cbComResCard[i][1])) continue ;

  Index=CardTypeResult[CT_FOUR_TAKE_ONE].cbCardTypeCount ;
  CardTypeResult[CT_FOUR_TAKE_ONE].cbCardType = CT_FOUR_TAKE_ONE ;
  CopyMemory(CardTypeResult[CT_FOUR_TAKE_ONE].cbCardData[Index], cbHandAllFourCardData+Start, 4) ;
  CardTypeResult[CT_FOUR_TAKE_ONE].cbCardData[Index][4] = cbComResCard[i][0] ;
  CardTypeResult[CT_FOUR_TAKE_ONE].cbCardData[Index][4+1] = cbComResCard[i][1] ;
  CardTypeResult[CT_FOUR_TAKE_ONE].cbEachHandCardCount[Index] = 6 ;
  CardTypeResult[CT_FOUR_TAKE_ONE].cbCardTypeCount++ ;

  ASSERT(CardTypeResult[CT_FOUR_TAKE_ONE].cbCardTypeCount<MAX_TYPE_COUNT) ;
  }
  }
  }*/


  //�Ĵ�����
  /*
  {
  //�ָ��˿ˣ���ֹ����ʱ�ı��˿�
  CopyMemory(cbTmpCardData, cbReserveCardData, cbHandCardCount) ;

  BYTE cbFirstCard = 0 ;
  //ȥ������
  for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardColor(cbTmpCardData[i])!=0x40)  {cbFirstCard = i ; break ;}

  BYTE cbHandAllFourCardData[MAX_COUNT] ;
  BYTE cbHandAllFourCardCount=0;

  //��ȡ����
  GetAllBomCard(cbTmpCardData+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;

  //�Ƴ�����
  BYTE cbRemainCard[MAX_COUNT];
  BYTE cbRemainCardCount=cbHandCardCount-cbHandAllFourCardCount ;
  CopyMemory(cbRemainCard, cbTmpCardData, cbHandCardCount*sizeof(BYTE));
  RemoveCard(cbHandAllFourCardData, cbHandAllFourCardCount, cbRemainCard, cbHandCardCount) ;

  for(BYTE Start=0; Start<cbHandAllFourCardCount; Start += 4)
  {
  //��ȡ����
  BYTE cbAllDoubleCardData[MAX_COUNT] ;
  BYTE cbAllDoubleCardCount=0 ;
  GetAllDoubleCard(cbRemainCard, cbRemainCardCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;

  BYTE cbDoubleCardIndex[10]; //�����±�
  for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
  cbDoubleCardIndex[j]=i ;

  //�������
  BYTE cbComCard[5];
  BYTE cbComResCard[255][5] ;
  BYTE cbComResLen=0 ;

  //���ö��Ƶ��±�����ϣ��ٸ����±���ȡ������
  Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, 2, cbAllDoubleCardCount/2, 2);
  for(BYTE i=0; i<cbComResLen; ++i)
  {
  BYTE Index = CardTypeResult[CT_FOUR_TAKE_TWO].cbCardTypeCount ;
  CardTypeResult[CT_FOUR_TAKE_TWO].cbCardType = CT_FOUR_TAKE_TWO ;
  CopyMemory(CardTypeResult[CT_FOUR_TAKE_TWO].cbCardData[Index], cbHandAllFourCardData+Start, 4) ;

  //�������
  for(BYTE j=0, k=0; j<4; ++j, k+=2)
  {
  CardTypeResult[CT_FOUR_TAKE_TWO].cbCardData[Index][4+k] = cbAllDoubleCardData[cbComResCard[i][j]];
  CardTypeResult[CT_FOUR_TAKE_TWO].cbCardData[Index][4+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
  }

  CardTypeResult[CT_FOUR_TAKE_TWO].cbEachHandCardCount[Index] = 8 ;
  CardTypeResult[CT_FOUR_TAKE_TWO].cbCardTypeCount++ ;

  ASSERT(CardTypeResult[CT_FOUR_TAKE_TWO].cbCardTypeCount<MAX_TYPE_COUNT) ;
  }
  }
  }*/

}

//��������
VOID CGameLogic::AnalyseOutCardType(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE const cbTurnCardData[], BYTE const cbTurnCardCount,tagOutCardTypeResult CardTypeResult[12+1])
{
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult[0])*12) ;
  //����У��
  if(cbHandCardCount<cbTurnCardCount)
  {
    return ;
  }

  BYTE cbTmpCard[MAX_COUNT];
  CopyMemory(cbTmpCard, cbHandCardData, cbHandCardCount) ;
  SortCardList(cbTmpCard, cbHandCardCount, ST_ORDER) ;
  //  SortCardList(cbTurnCardData, cbTurnCardCount, ST_ORDER) ;

  BYTE cbTurnCardType = GetCardType(cbTurnCardData, cbTurnCardCount) ;
  ASSERT(cbTurnCardType!=CT_ERROR) ;
  if(cbTurnCardType==CT_ERROR)
  {
    return ;
  }

  if(cbTurnCardType!=CT_MISSILE_CARD && cbTurnCardType!=CT_BOMB_CARD)
  {
    //˫��ը��
    if(cbHandCardCount>=2 && 0x4F==cbTmpCard[0] && 0x4E==cbTmpCard[1])
    {
      BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount;
      CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = cbTmpCard[0] ;
      CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = cbTmpCard[1] ;
      CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 2 ;
      CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++;

      BYTE cbBomCardData[MAX_COUNT];
      BYTE cbBomCardCount=0;
      GetAllBomCard(cbTmpCard+2, cbHandCardCount-2, cbBomCardData, cbBomCardCount) ;
      for(BYTE i=0; i<cbBomCardCount/4; ++i)
      {
        Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount;
        CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
        CopyMemory(CardTypeResult[CT_BOMB_CARD].cbCardData[Index], cbBomCardData+4*i, 4) ;
        CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4;
        CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++;

        ASSERT(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
      }
    }
    //ը������
    else
    {
      BYTE cbBomCardData[MAX_COUNT];
      BYTE cbBomCardCount=0;
      GetAllBomCard(cbTmpCard, cbHandCardCount, cbBomCardData, cbBomCardCount) ;
      for(BYTE i=0; i<cbBomCardCount/4; ++i)
      {
        BYTE Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount;
        CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
        CopyMemory(CardTypeResult[CT_BOMB_CARD].cbCardData[Index], cbBomCardData+4*i, 4) ;
        CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4;
        CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++;

        ASSERT(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
      }
    }
  }

  switch(cbTurnCardType)
  {
    case CT_SINGLE:       //��������
    {
      for(BYTE i=0; i<cbHandCardCount; ++i)
        if(GetCardLogicValue(cbTmpCard[i])>GetCardLogicValue(cbTurnCardData[0]))
        {
          BYTE Index = CardTypeResult[CT_SINGLE].cbCardTypeCount ;
          CardTypeResult[CT_SINGLE].cbCardType = CT_SINGLE ;
          CardTypeResult[CT_SINGLE].cbCardData[Index][0] = cbTmpCard[i];
          CardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] = 1;
          CardTypeResult[CT_SINGLE].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_SINGLE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
        }
      break ;
    }
    case CT_DOUBLE:       //��������
    {
      //�˿˷���
      for(BYTE i=0; i<cbHandCardCount; i++)
      {
        //��������
        BYTE cbSameCount=1;
        BYTE cbLogicValue=GetCardLogicValue(cbTmpCard[i]);

        //����ͬ��
        for(BYTE j=i+1; j<cbHandCardCount; j++)
        {
          //��ȡ�˿�
          if(GetCardLogicValue(cbTmpCard[j])!=cbLogicValue)
          {
            break;
          }

          //���ñ���
          cbSameCount++;
        }

        if(cbSameCount>=2 && GetCardLogicValue(cbTmpCard[i])>GetCardLogicValue(cbTurnCardData[0]))
        {
          BYTE Index = CardTypeResult[CT_DOUBLE].cbCardTypeCount ;
          CardTypeResult[CT_DOUBLE].cbCardType = CT_DOUBLE ;
          CardTypeResult[CT_DOUBLE].cbCardData[Index][0] = cbTmpCard[i];
          CardTypeResult[CT_DOUBLE].cbCardData[Index][1] = cbTmpCard[i+1];
          CardTypeResult[CT_DOUBLE].cbEachHandCardCount[Index] = 2;
          CardTypeResult[CT_DOUBLE].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_DOUBLE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
        }
        //��������
        i+=cbSameCount-1;
      }
      break ;
    }
    case CT_THREE:        //��������
    {
      //�˿˷���
      for(BYTE i=0; i<cbHandCardCount; i++)
      {
        //��������
        BYTE cbSameCount=1;
        BYTE cbLogicValue=GetCardLogicValue(cbTmpCard[i]);

        //����ͬ��
        for(BYTE j=i+1; j<cbHandCardCount; j++)
        {
          //��ȡ�˿�
          if(GetCardLogicValue(cbTmpCard[j])!=cbLogicValue)
          {
            break;
          }

          //���ñ���
          cbSameCount++;
        }

        if(cbSameCount>=3 && GetCardLogicValue(cbTmpCard[i])>GetCardLogicValue(cbTurnCardData[0]))
        {
          BYTE Index = CardTypeResult[CT_THREE].cbCardTypeCount ;
          CardTypeResult[CT_THREE].cbCardType = CT_THREE ;
          CardTypeResult[CT_THREE].cbCardData[Index][0] = cbTmpCard[i];
          CardTypeResult[CT_THREE].cbCardData[Index][1] = cbTmpCard[i+1];
          CardTypeResult[CT_THREE].cbCardData[Index][2] = cbTmpCard[i+2];
          CardTypeResult[CT_THREE].cbEachHandCardCount[Index] = 3;
          CardTypeResult[CT_THREE].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_THREE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
        }
        //��������
        i+=cbSameCount-1;
      }
      break ;
    }
    case CT_SINGLE_LINE:    //��������
    {
      BYTE cbFirstCard = 0 ;
      //ȥ��2����
      for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardLogicValue(cbTmpCard[i])<15)
        {
          cbFirstCard = i ;
          break ;
        }

      BYTE cbSingleLineCard[12] ;
      BYTE cbSingleLineCount=1 ;
      BYTE cbLeftCardCount = cbHandCardCount ;
      bool bFindSingleLine = true ;

      //�����ж�
      while(cbLeftCardCount>=cbTurnCardCount && bFindSingleLine)
      {
        cbSingleLineCount=1 ;
        bFindSingleLine = false ;
        BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
        cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[cbFirstCard] ;
        for(BYTE i=cbFirstCard+1; i<cbLeftCardCount; i++)
        {
          BYTE cbCardData=cbTmpCard[i];

          //�����ж�
          if(1!=(GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbCardData)) && GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
          {
            cbLastCard = cbTmpCard[i] ;
            //�Ƿ�Ϸ�
            if(cbSingleLineCount<cbTurnCardCount)
            {
              cbSingleLineCount = 1 ;
              cbSingleLineCard[cbSingleLineCount-1] = cbTmpCard[i] ;
              continue ;
            }
            else
            {
              break ;
            }
          }
          //ͬ���ж�
          else if(GetCardValue(cbLastCard)!=GetCardValue(cbCardData))
          {
            cbLastCard = cbCardData ;
            cbSingleLineCard[cbSingleLineCount] = cbCardData ;
            ++cbSingleLineCount ;
          }
        }

        //��������
        if(cbSingleLineCount>=cbTurnCardCount && GetCardLogicValue(cbSingleLineCard[0])>GetCardLogicValue(cbTurnCardData[0]))
        {
          BYTE Index ;
          BYTE cbStart=0 ;
          //��������
          while(GetCardLogicValue(cbSingleLineCard[cbStart])>GetCardLogicValue(cbTurnCardData[0]) && ((cbSingleLineCount-cbStart)>=cbTurnCardCount))
          {
            Index = CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount ;
            CardTypeResult[CT_SINGLE_LINE].cbCardType = CT_SINGLE_LINE ;
            CopyMemory(CardTypeResult[CT_SINGLE_LINE].cbCardData[Index], cbSingleLineCard+cbStart, sizeof(BYTE)*cbTurnCardCount);
            CardTypeResult[CT_SINGLE_LINE].cbEachHandCardCount[Index] = cbTurnCardCount;
            CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount++ ;
            cbStart++;

            ASSERT(CardTypeResult[CT_SINGLE_LINE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
          }

          RemoveCard(cbSingleLineCard, cbSingleLineCount, cbTmpCard, cbLeftCardCount) ;
          cbLeftCardCount -= cbSingleLineCount ;
          bFindSingleLine = true ;
        }
      }

      break ;
    }
    case CT_DOUBLE_LINE:    //��������
    {
      //�����ж�
      BYTE cbFirstCard = 0 ;
      //ȥ��2����
      for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardLogicValue(cbTmpCard[i])<15)
        {
          cbFirstCard = i ;
          break ;
        }

      BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
      bool bFindDoubleLine = true ;
      BYTE cbDoubleLineCount = 0 ;
      BYTE cbDoubleLineCard[24] ;
      //��ʼ�ж�
      while(cbLeftCardCount>=cbTurnCardCount && bFindDoubleLine)
      {
        BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
        BYTE cbSameCount = 1 ;
        cbDoubleLineCount = 0 ;
        bFindDoubleLine=false ;
        for(BYTE i=cbFirstCard+1 ; i<cbLeftCardCount+cbFirstCard ; ++i)
        {
          //����ͬ��
          while(GetCardValue(cbLastCard)==GetCardValue(cbTmpCard[i]) && i<cbLeftCardCount+cbFirstCard)
          {
            ++cbSameCount;
            ++i ;
          }

          BYTE cbLastDoubleCardValue ;
          if(cbDoubleLineCount>0)
          {
            cbLastDoubleCardValue = GetCardLogicValue(cbDoubleLineCard[cbDoubleLineCount-1]) ;
          }
          //���¿�ʼ
          if((cbSameCount<2 || (cbDoubleLineCount>0 && (cbLastDoubleCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
          {
            if(cbDoubleLineCount>=cbTurnCardCount)
            {
              break ;
            }

            if(cbSameCount>=2)
            {
              i-=cbSameCount ;
            }

            cbLastCard = cbTmpCard[i] ;
            cbDoubleLineCount = 0 ;
          }
          //��������
          else if(cbSameCount>=2)
          {
            cbDoubleLineCard[cbDoubleLineCount] = cbTmpCard[i-cbSameCount] ;
            cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCard[i-cbSameCount+1] ;
            cbDoubleLineCount += 2 ;

            //��β�ж�
            if(i==(cbLeftCardCount+cbFirstCard-2))
              if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCard[i]))==1 && (GetCardLogicValue(cbTmpCard[i])==GetCardLogicValue(cbTmpCard[i+1])))
              {
                cbDoubleLineCard[cbDoubleLineCount] = cbTmpCard[i] ;
                cbDoubleLineCard[cbDoubleLineCount+1] = cbTmpCard[i+1] ;
                cbDoubleLineCount += 2 ;
                break ;
              }

          }

          cbLastCard = cbTmpCard[i] ;
          cbSameCount = 1 ;
        }

        //��������
        if(cbDoubleLineCount>=cbTurnCardCount)
        {
          BYTE Index ;
          BYTE cbStart=0 ;
          //��������
          while(GetCardLogicValue(cbDoubleLineCard[cbStart])>GetCardLogicValue(cbTurnCardData[0]) && ((cbDoubleLineCount-cbStart)>=cbTurnCardCount))
          {
            Index = CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount ;
            CardTypeResult[CT_DOUBLE_LINE].cbCardType = CT_DOUBLE_LINE ;
            CopyMemory(CardTypeResult[CT_DOUBLE_LINE].cbCardData[Index], cbDoubleLineCard+cbStart, sizeof(BYTE)*cbTurnCardCount);
            CardTypeResult[CT_DOUBLE_LINE].cbEachHandCardCount[Index] = cbTurnCardCount;
            CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount++ ;
            cbStart += 2;

            ASSERT(CardTypeResult[CT_DOUBLE_LINE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
          }
          RemoveCard(cbDoubleLineCard, cbDoubleLineCount, cbTmpCard, cbFirstCard+cbLeftCardCount) ;
          bFindDoubleLine=true ;
          cbLeftCardCount -= cbDoubleLineCount ;
        }
      }

      break;
    }
    case CT_THREE_LINE:     //��������
    {
      //�����ж�
      BYTE cbFirstCard = 0 ;
      //ȥ��2����
      for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardLogicValue(cbTmpCard[i])<15)
        {
          cbFirstCard = i ;
          break ;
        }

      BYTE cbLeftCardCount = cbHandCardCount-cbFirstCard ;
      bool bFindThreeLine = true ;
      BYTE cbThreeLineCount = 0 ;
      BYTE cbThreeLineCard[20] ;
      //��ʼ�ж�
      while(cbLeftCardCount>=cbTurnCardCount && bFindThreeLine)
      {
        BYTE cbLastCard = cbTmpCard[cbFirstCard] ;
        BYTE cbSameCount = 1 ;
        cbThreeLineCount = 0 ;
        bFindThreeLine = false ;
        for(BYTE i=cbFirstCard+1 ; i<cbLeftCardCount+cbFirstCard ; ++i)
        {
          //����ͬ��
          while(GetCardValue(cbLastCard)==GetCardValue(cbTmpCard[i]) && i<cbLeftCardCount+cbFirstCard)
          {
            ++cbSameCount;
            ++i ;
          }

          BYTE cbLastThreeCardValue ;
          if(cbThreeLineCount>0)
          {
            cbLastThreeCardValue = GetCardLogicValue(cbThreeLineCard[cbThreeLineCount-1]) ;
          }

          //���¿�ʼ
          if((cbSameCount<3 || (cbThreeLineCount>0&&(cbLastThreeCardValue-GetCardLogicValue(cbLastCard))!=1)) && i<=cbLeftCardCount+cbFirstCard)
          {
            if(cbThreeLineCount>=cbTurnCardCount)
            {
              break ;
            }

            if(cbSameCount>=3)
            {
              i-= 3 ;
            }
            cbLastCard = cbTmpCard[i] ;
            cbThreeLineCount = 0 ;
          }
          //��������
          else if(cbSameCount>=3)
          {
            cbThreeLineCard[cbThreeLineCount] = cbTmpCard[i-cbSameCount] ;
            cbThreeLineCard[cbThreeLineCount+1] = cbTmpCard[i-cbSameCount+1] ;
            cbThreeLineCard[cbThreeLineCount+2] = cbTmpCard[i-cbSameCount+2] ;
            cbThreeLineCount += 3 ;

            //��β�ж�
            if(i==(cbLeftCardCount+cbFirstCard-3))
              if((GetCardLogicValue(cbLastCard)-GetCardLogicValue(cbTmpCard[i]))==1 && (GetCardLogicValue(cbTmpCard[i])==GetCardLogicValue(cbTmpCard[i+1])) && (GetCardLogicValue(cbTmpCard[i])==GetCardLogicValue(cbTmpCard[i+2])))
              {
                cbThreeLineCard[cbThreeLineCount] = cbTmpCard[i] ;
                cbThreeLineCard[cbThreeLineCount+1] = cbTmpCard[i+1] ;
                cbThreeLineCard[cbThreeLineCount+2] = cbTmpCard[i+2] ;
                cbThreeLineCount += 3 ;
                break ;
              }

          }

          cbLastCard = cbTmpCard[i] ;
          cbSameCount = 1 ;
        }

        //��������
        if(cbThreeLineCount>=cbTurnCardCount)
        {
          BYTE Index ;
          BYTE cbStart=0 ;
          //��������
          while(GetCardLogicValue(cbThreeLineCard[cbStart])>GetCardLogicValue(cbTurnCardData[0]) && ((cbThreeLineCount-cbStart)>=cbTurnCardCount))
          {
            Index = CardTypeResult[CT_THREE_LINE].cbCardTypeCount ;
            CardTypeResult[CT_THREE_LINE].cbCardType = CT_THREE_LINE ;
            CopyMemory(CardTypeResult[CT_THREE_LINE].cbCardData[Index], cbThreeLineCard+cbStart, sizeof(BYTE)*cbTurnCardCount);
            CardTypeResult[CT_THREE_LINE].cbEachHandCardCount[Index] = cbTurnCardCount;
            CardTypeResult[CT_THREE_LINE].cbCardTypeCount++ ;
            cbStart += 3;

            ASSERT(CardTypeResult[CT_THREE_LINE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
          }

          RemoveCard(cbThreeLineCard, cbThreeLineCount, cbTmpCard, cbFirstCard+cbLeftCardCount) ;
          bFindThreeLine=true ;
          cbLeftCardCount -= cbThreeLineCount ;
        }
      }

      break;
    }
    case CT_THREE_TAKE_ONE://����һ��
    {
      BYTE cbTurnThreeCard[MAX_COUNT];
      BYTE cbTurnThreeCount=0;
      BYTE cbHandThreeCard[MAX_COUNT];
      BYTE cbHandThreeCount=0 ;
      BYTE cbSingleCardCount=cbTurnCardCount/4;

      //�Ƴ�ը��
      BYTE cbAllBomCardData[MAX_COUNT] ;
      BYTE cbAllBomCardCount=0 ;
      GetAllBomCard(cbTmpCard, cbHandCardCount, cbAllBomCardData, cbAllBomCardCount) ;
      RemoveCard(cbAllBomCardData, cbAllBomCardCount, cbTmpCard, cbHandCardCount) ;

      //�����˿�
      GetAllThreeCard(cbTurnCardData, cbTurnCardCount, cbTurnThreeCard, cbTurnThreeCount) ;

      BYTE cbFirstCard = 0 ;

      //ȥ��2����
      if(cbTurnThreeCount>3)
        for(BYTE i=0 ; i<cbHandCardCount-cbAllBomCardCount ; ++i)
          if(GetCardLogicValue(cbTmpCard[i])<15)
          {
            cbFirstCard = i ;
            break ;
          }

      GetAllThreeCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard-cbAllBomCardCount, cbHandThreeCard, cbHandThreeCount) ;

      if(cbHandThreeCount<cbTurnThreeCount || (cbHandThreeCount>0&&GetCardLogicValue(cbHandThreeCard[0])<GetCardLogicValue(cbTurnThreeCard[0])))
      {
        return ;
      }

      for(BYTE i=0; i<cbHandThreeCount; i+=3)
      {
        BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[i]);
        BYTE cbThreeLineCard[MAX_COUNT];
        BYTE cbThreeLineCardCount=3;
        cbThreeLineCard[0]=cbHandThreeCard[i];
        cbThreeLineCard[1]=cbHandThreeCard[i+1];
        cbThreeLineCard[2]=cbHandThreeCard[i+2];
        for(BYTE j=i+3; j<cbHandThreeCount; j+=3)
        {
          //�����ж�
          if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))) || cbThreeLineCardCount==cbTurnThreeCount)
          {
            break;
          }

          cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
          cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
          cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
          cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
          cbThreeLineCardCount += 3;
        }
        if(cbThreeLineCardCount==cbTurnThreeCount && GetCardLogicValue(cbThreeLineCard[0])>GetCardLogicValue(cbTurnThreeCard[0]))
        {
          BYTE Index ;

          BYTE cbRemainCard[MAX_COUNT];
          CopyMemory(cbRemainCard, cbTmpCard, (cbHandCardCount-cbAllBomCardCount)*sizeof(BYTE));
          RemoveCard(cbThreeLineCard, cbTurnThreeCount, cbRemainCard, (cbHandCardCount-cbAllBomCardCount)) ;

          //�������
          BYTE cbComCard[5];
          BYTE cbComResCard[254][5] ;
          BYTE cbComResLen=0 ;
          Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, cbSingleCardCount, (cbHandCardCount-cbAllBomCardCount)-cbTurnThreeCount, cbSingleCardCount);
          for(BYTE i=0; i<cbComResLen; ++i)
          {
            Index = CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount ;
            CardTypeResult[CT_THREE_TAKE_ONE].cbCardType = CT_THREE_TAKE_ONE;
            //��������
            CopyMemory(CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index], cbThreeLineCard, sizeof(BYTE)*cbTurnThreeCount);
            //���浥��
            CopyMemory(CardTypeResult[CT_THREE_TAKE_ONE].cbCardData[Index]+cbTurnThreeCount, cbComResCard[i], cbSingleCardCount) ;

            ASSERT(cbTurnThreeCount+cbSingleCardCount==cbTurnCardCount) ;
            CardTypeResult[CT_THREE_TAKE_ONE].cbEachHandCardCount[Index] = cbTurnCardCount ;
            CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount++ ;

            ASSERT(CardTypeResult[CT_THREE_TAKE_ONE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
          }

        }
      }

      break;
    }
    case CT_THREE_TAKE_TWO://����һ��
    {
      BYTE cbTurnThreeCard[MAX_COUNT];
      BYTE cbTurnThreeCount=0;
      BYTE cbHandThreeCard[MAX_COUNT];
      BYTE cbHandThreeCount=0 ;
      BYTE cbDoubleCardCount=cbTurnCardCount/5;

      //�����˿�
      GetAllThreeCard(cbTurnCardData, cbTurnCardCount, cbTurnThreeCard, cbTurnThreeCount) ;

      BYTE cbFirstCard = 0 ;

      //ȥ��2����
      if(cbTurnThreeCount>3)
        for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardLogicValue(cbTmpCard[i])<15)
          {
            cbFirstCard = i ;
            break ;
          }

      GetAllThreeCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandThreeCard, cbHandThreeCount) ;

      if(cbHandThreeCount<cbTurnThreeCount || (cbHandThreeCount>0&&GetCardLogicValue(cbHandThreeCard[0])<GetCardLogicValue(cbTurnThreeCard[0])))
      {
        return ;
      }

      for(BYTE i=0; i<cbHandThreeCount; i+=3)
      {
        BYTE cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[i]);
        BYTE cbThreeLineCard[MAX_COUNT];
        BYTE cbThreeLineCardCount=3;
        cbThreeLineCard[0]=cbHandThreeCard[i];
        cbThreeLineCard[1]=cbHandThreeCard[i+1];
        cbThreeLineCard[2]=cbHandThreeCard[i+2];
        for(BYTE j=i+3; j<cbHandThreeCount; j+=3)
        {
          //�����ж�
          if(1!=(cbLastLogicCard-(GetCardLogicValue(cbHandThreeCard[j]))) || cbThreeLineCardCount==cbTurnThreeCount)
          {
            break;
          }

          cbLastLogicCard=GetCardLogicValue(cbHandThreeCard[j]);
          cbThreeLineCard[cbThreeLineCardCount]=cbHandThreeCard[j];
          cbThreeLineCard[cbThreeLineCardCount+1]=cbHandThreeCard[j+1];
          cbThreeLineCard[cbThreeLineCardCount+2]=cbHandThreeCard[j+2];
          cbThreeLineCardCount += 3;
        }
        if(cbThreeLineCardCount==cbTurnThreeCount && GetCardLogicValue(cbThreeLineCard[0])>GetCardLogicValue(cbTurnThreeCard[0]))
        {
          BYTE Index ;

          BYTE cbRemainCard[MAX_COUNT];
          CopyMemory(cbRemainCard, cbTmpCard, cbHandCardCount*sizeof(BYTE));
          RemoveCard(cbThreeLineCard, cbTurnThreeCount, cbRemainCard, cbHandCardCount) ;

          BYTE cbAllDoubleCardData[MAX_COUNT] ;
          BYTE cbAllDoubleCardCount=0 ;
          GetAllDoubleCard(cbRemainCard, cbHandCardCount-cbTurnThreeCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;


          BYTE cbDoubleCardIndex[10]; //�����±�
          for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
          {
            cbDoubleCardIndex[j]=i ;
          }

          //�������
          BYTE cbComCard[5];
          BYTE cbComResCard[254][5] ;
          BYTE cbComResLen=0 ;

          //���ö��Ƶ��±�����ϣ��ٸ����±���ȡ������
          Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, cbDoubleCardCount, cbAllDoubleCardCount/2, cbDoubleCardCount);
          for(BYTE i=0; i<cbComResLen; ++i)
          {
            Index = CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount ;
            CardTypeResult[CT_THREE_TAKE_TWO].cbCardType = CT_THREE_TAKE_TWO ;
            //��������
            CopyMemory(CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index], cbThreeLineCard, sizeof(BYTE)*cbTurnThreeCount);
            //�������
            for(BYTE j=0, k=0; j<cbDoubleCardCount; ++j, k+=2)
            {
              CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][cbTurnThreeCount+k] = cbAllDoubleCardData[cbComResCard[i][j]];
              CardTypeResult[CT_THREE_TAKE_TWO].cbCardData[Index][cbTurnThreeCount+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
            }

            ASSERT(cbTurnThreeCount+cbDoubleCardCount*2==cbTurnCardCount) ;
            CardTypeResult[CT_THREE_TAKE_TWO].cbEachHandCardCount[Index] = cbTurnCardCount ;

            CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount++ ;

            ASSERT(CardTypeResult[CT_THREE_TAKE_TWO].cbCardTypeCount<=MAX_TYPE_COUNT) ;
          }

        }
      }

      break;
    }
    case CT_FOUR_TAKE_ONE://�Ĵ�����
    {
      BYTE cbFirstCard = 0 ;
      //ȥ������
      for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardColor(cbTmpCard[i])!=0x40)
        {
          cbFirstCard = i ;
          break ;
        }

      BYTE cbHandAllFourCardData[MAX_COUNT] ;
      BYTE cbHandAllFourCardCount=0;
      BYTE cbTurnAllFourCardData[MAX_COUNT];
      BYTE cbTurnAllFourCardCount=0;
      //��ȡ����
      GetAllBomCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;
      GetAllBomCard(cbTurnCardData, cbTurnCardCount, cbTurnAllFourCardData, cbTurnAllFourCardCount) ;

      if(cbHandAllFourCardCount>0 && GetCardLogicValue(cbHandAllFourCardData[0])<GetCardLogicValue(cbTurnAllFourCardData[0]))
      {
        return ;
      }


      BYTE cbCanOutFourCardData[MAX_COUNT] ;
      BYTE cbCanOutFourCardCount=0 ;
      //�ɳ�����
      for(BYTE i=0; i<cbHandAllFourCardCount; i+=4)
      {
        if(GetCardLogicValue(cbHandAllFourCardData[i])>GetCardLogicValue(cbTurnAllFourCardData[0]))
        {
          cbCanOutFourCardData[cbCanOutFourCardCount] = cbHandAllFourCardData[i] ;
          cbCanOutFourCardData[cbCanOutFourCardCount+1] = cbHandAllFourCardData[i+1] ;
          cbCanOutFourCardData[cbCanOutFourCardCount+2] = cbHandAllFourCardData[i+2] ;
          cbCanOutFourCardData[cbCanOutFourCardCount+3] = cbHandAllFourCardData[i+3] ;
          cbCanOutFourCardCount += 4 ;
        }
      }

      if((cbHandCardCount-cbCanOutFourCardCount) < (cbTurnCardCount-cbTurnAllFourCardCount))
      {
        return ;
      }

      BYTE cbRemainCard[MAX_COUNT];
      CopyMemory(cbRemainCard, cbTmpCard, cbHandCardCount*sizeof(BYTE));
      RemoveCard(cbCanOutFourCardData, cbCanOutFourCardCount, cbRemainCard, cbHandCardCount) ;
      for(BYTE Start=0; Start<cbCanOutFourCardCount; Start += 4)
      {
        BYTE Index ;
        //�������
        BYTE cbComCard[5];
        BYTE cbComResCard[254][5] ;
        BYTE cbComResLen=0 ;
        //�������
        Combination(cbComCard, 0, cbComResCard, cbComResLen, cbRemainCard, 2, cbHandCardCount-cbCanOutFourCardCount, 2);
        for(BYTE i=0; i<cbComResLen; ++i)
        {
          //���ܴ���
          if(GetCardValue(cbComResCard[i][0])==GetCardValue(cbComResCard[i][1]))
          {
            continue ;
          }

          Index=CardTypeResult[CT_FOUR_TAKE_ONE].cbCardTypeCount ;
          CardTypeResult[CT_FOUR_TAKE_ONE].cbCardType = CT_FOUR_TAKE_ONE ;
          CopyMemory(CardTypeResult[CT_FOUR_TAKE_ONE].cbCardData[Index], cbCanOutFourCardData+Start, 4) ;
          CardTypeResult[CT_FOUR_TAKE_ONE].cbCardData[Index][4] = cbComResCard[i][0] ;
          CardTypeResult[CT_FOUR_TAKE_ONE].cbCardData[Index][4+1] = cbComResCard[i][1] ;
          CardTypeResult[CT_FOUR_TAKE_ONE].cbEachHandCardCount[Index] = 6 ;
          CardTypeResult[CT_FOUR_TAKE_ONE].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_FOUR_TAKE_ONE].cbCardTypeCount<=MAX_TYPE_COUNT) ;
        }
      }


      break;
    }
    case CT_FOUR_TAKE_TWO://�Ĵ�����
    {
      BYTE cbFirstCard = 0 ;
      //ȥ������
      for(BYTE i=0 ; i<cbHandCardCount ; ++i) if(GetCardColor(cbTmpCard[i])!=0x40)
        {
          cbFirstCard = i ;
          break ;
        }

      BYTE cbHandAllFourCardData[MAX_COUNT] ;
      BYTE cbHandAllFourCardCount=0;
      BYTE cbTurnAllFourCardData[MAX_COUNT];
      BYTE cbTurnAllFourCardCount=0;
      //��ȡ����
      GetAllBomCard(cbTmpCard+cbFirstCard, cbHandCardCount-cbFirstCard, cbHandAllFourCardData, cbHandAllFourCardCount) ;
      GetAllBomCard(cbTurnCardData, cbTurnCardCount, cbTurnAllFourCardData, cbTurnAllFourCardCount) ;

      if(cbHandAllFourCardCount>0 && GetCardLogicValue(cbHandAllFourCardData[0])<GetCardLogicValue(cbTurnAllFourCardData[0]))
      {
        return ;
      }


      BYTE cbCanOutFourCardData[MAX_COUNT] ;
      BYTE cbCanOutFourCardCount=0 ;
      //�ɳ�����
      for(BYTE i=0; i<cbHandAllFourCardCount; i+=4)
      {
        if(GetCardLogicValue(cbHandAllFourCardData[i])>GetCardLogicValue(cbTurnAllFourCardData[0]))
        {
          cbCanOutFourCardData[cbCanOutFourCardCount] = cbHandAllFourCardData[i] ;
          cbCanOutFourCardData[cbCanOutFourCardCount+1] = cbHandAllFourCardData[i+1] ;
          cbCanOutFourCardData[cbCanOutFourCardCount+2] = cbHandAllFourCardData[i+2] ;
          cbCanOutFourCardData[cbCanOutFourCardCount+3] = cbHandAllFourCardData[i+3] ;
          cbCanOutFourCardCount += 4 ;
        }
      }

      if((cbHandCardCount-cbCanOutFourCardCount) < (cbTurnCardCount-cbTurnAllFourCardCount))
      {
        return ;
      }

      BYTE cbRemainCard[MAX_COUNT];
      CopyMemory(cbRemainCard, cbTmpCard, cbHandCardCount*sizeof(BYTE));
      RemoveCard(cbCanOutFourCardData, cbCanOutFourCardCount, cbRemainCard, cbHandCardCount) ;
      for(BYTE Start=0; Start<cbCanOutFourCardCount; Start += 4)
      {
        BYTE cbAllDoubleCardData[MAX_COUNT] ;
        BYTE cbAllDoubleCardCount=0 ;
        GetAllDoubleCard(cbRemainCard, cbHandCardCount-cbCanOutFourCardCount, cbAllDoubleCardData, cbAllDoubleCardCount) ;


        BYTE cbDoubleCardIndex[10]; //�����±�
        for(BYTE i=0, j=0; i<cbAllDoubleCardCount; i+=2, ++j)
        {
          cbDoubleCardIndex[j]=i ;
        }

        //�������
        BYTE cbComCard[5];
        BYTE cbComResCard[254][5] ;
        BYTE cbComResLen=0 ;

        //���ö��Ƶ��±�����ϣ��ٸ����±���ȡ������
        Combination(cbComCard, 0, cbComResCard, cbComResLen, cbDoubleCardIndex, 2, cbAllDoubleCardCount/2, 2);
        for(BYTE i=0; i<cbComResLen; ++i)
        {
          BYTE Index = CardTypeResult[CT_FOUR_TAKE_TWO].cbCardTypeCount ;
          CardTypeResult[CT_FOUR_TAKE_TWO].cbCardType = CT_FOUR_TAKE_TWO ;
          CopyMemory(CardTypeResult[CT_FOUR_TAKE_TWO].cbCardData[Index], cbCanOutFourCardData+Start, 4) ;

          //�������
          for(BYTE j=0, k=0; j<4; ++j, k+=2)
          {
            CardTypeResult[CT_FOUR_TAKE_TWO].cbCardData[Index][4+k] = cbAllDoubleCardData[cbComResCard[i][j]];
            CardTypeResult[CT_FOUR_TAKE_TWO].cbCardData[Index][4+k+1] = cbAllDoubleCardData[cbComResCard[i][j]+1];
          }

          CardTypeResult[CT_FOUR_TAKE_TWO].cbEachHandCardCount[Index] = 8 ;
          CardTypeResult[CT_FOUR_TAKE_TWO].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_FOUR_TAKE_TWO].cbCardTypeCount<=MAX_TYPE_COUNT) ;
        }
      }
      break;
    }
    case CT_BOMB_CARD:      //ը������
    {
      BYTE cbAllBomCardData[MAX_COUNT] ;
      BYTE cbAllBomCardCount=0 ;
      GetAllBomCard(cbTmpCard, cbHandCardCount, cbAllBomCardData, cbAllBomCardCount) ;
      BYTE cbFirstBom=0 ;
      BYTE Index ;
      if(cbAllBomCardCount>0 && cbAllBomCardData[0]==0x4F)
      {
        Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
        CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
        CardTypeResult[CT_BOMB_CARD].cbCardData[Index][0] = 0x4F ;
        CardTypeResult[CT_BOMB_CARD].cbCardData[Index][1] = 0x4E ;
        CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 2 ;
        CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;

        ASSERT(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
        cbFirstBom=2;
      }
      for(BYTE i=cbFirstBom; i<cbAllBomCardCount; i+=4)
      {
        if(GetCardLogicValue(cbAllBomCardData[i])>GetCardLogicValue(cbTurnCardData[0]))
        {
          Index = CardTypeResult[CT_BOMB_CARD].cbCardTypeCount ;
          CardTypeResult[CT_BOMB_CARD].cbCardType = CT_BOMB_CARD ;
          CopyMemory(CardTypeResult[CT_BOMB_CARD].cbCardData[Index], cbAllBomCardData+i, 4) ;
          CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index] = 4 ;
          CardTypeResult[CT_BOMB_CARD].cbCardTypeCount++ ;

          ASSERT(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount<=MAX_TYPE_COUNT) ;
        }
      }
      break;
    }
    case CT_MISSILE_CARD:   //�������
    {
      //û�бȻ�����������
      break;
    }
    default:
      ASSERT(false) ;
      break;
  }

}


/********************************************************************
��������Combination
������
cbCombineCardData���洢��������Ͻ��
cbResComLen���ѵõ�����ϳ��ȣ���ʼ����ʱ�˲���Ϊ0
cbResultCardData���洢���е���Ͻ��
cbResCardLen��cbResultCardData�ĵ�һ�±�ĳ��ȣ���Ͻ���ĸ���
cbSrcCardData����Ҫ����ϵ�����
cbSrcLen��cbSrcCardData��������Ŀ
cbCombineLen2��cbCombineLen1����ϵĳ��ȣ���ʼ����ʱ������ȡ�
*********************************************************************/
//����㷨
VOID CGameLogic::Combination(BYTE cbCombineCardData[], BYTE cbResComLen,  BYTE cbResultCardData[100][5], BYTE &cbResCardLen,BYTE cbSrcCardData[] , BYTE cbCombineLen1, BYTE cbSrcLen, const BYTE cbCombineLen2)
{

  if(cbResComLen == cbCombineLen2)
  {
    CopyMemory(&cbResultCardData[cbResCardLen], cbCombineCardData, cbResComLen) ;
    ++cbResCardLen ;

    ASSERT(cbResCardLen<255) ;

  }
  else
  {
    if(cbCombineLen1 >= 1 && cbSrcLen > 0 && (cbSrcLen+1) >= 0)
    {
      cbCombineCardData[cbCombineLen2-cbCombineLen1] =  cbSrcCardData[0];
      ++cbResComLen ;
      Combination(cbCombineCardData,cbResComLen, cbResultCardData, cbResCardLen, cbSrcCardData+1,cbCombineLen1-1, cbSrcLen-1, cbCombineLen2);

      --cbResComLen;
      Combination(cbCombineCardData,cbResComLen, cbResultCardData, cbResCardLen, cbSrcCardData+1,cbCombineLen1, cbSrcLen-1, cbCombineLen2);
    }
  }
}

//�����㷨
VOID CGameLogic::Permutation(BYTE *list, INT m, INT n, BYTE result[][4], BYTE &len)
{
  INT j,temp;
  if(m == n)
  {
    for(j = 0; j < n; j++)
    {
      result[len][j]=list[j];
    }
    len++ ;
  }
  else
  {
    for(j = m; j < n; j++)
    {
      temp = list[m] ;
      list[m] = list[j];
      list[j] = temp ;
      Permutation(list,m+1,n,result,len);
      temp = list[m] ;
      list[m] = list[j];
      list[j] = temp ;
    }
  }
}

//���Ƹ���
BYTE CGameLogic::AnalyseSinleCardCount(BYTE const cbHandCardData[], BYTE const cbHandCardCount, BYTE const cbWantOutCardData[], BYTE const cbWantOutCardCount, BYTE cbSingleCardData[])
{
  BYTE cbRemainCard[MAX_COUNT] ;
  BYTE cbRemainCardCount=0 ;
  CopyMemory(cbRemainCard, cbHandCardData, cbHandCardCount) ;
  SortCardList(cbRemainCard, cbHandCardCount, ST_ORDER) ;

  if(cbWantOutCardCount!=0)
  {
    RemoveCard(cbWantOutCardData, cbWantOutCardCount, cbRemainCard, cbHandCardCount) ;
  }
  cbRemainCardCount = cbHandCardCount-cbWantOutCardCount ;

  //����ָ��
  typedef VOID (CGameLogic::*pGetAllCardFun)(BYTE const [], BYTE const , BYTE[], BYTE &);

  //ָ������
  pGetAllCardFun GetAllCardFunArray[4] ;
  GetAllCardFunArray[0] = &CGameLogic::GetAllBomCard ;		//ը������
  GetAllCardFunArray[1] = &CGameLogic::GetAllLineCard ;		//˳�Ӻ���
  GetAllCardFunArray[2] = &CGameLogic::GetAllThreeCard ;	//��������
  GetAllCardFunArray[3] = &CGameLogic::GetAllDoubleCard ;	//���Ӻ���

  //ָ�������±�
  BYTE cbIndexArray[4] = {0,1,2,3} ;
  //���н��
  BYTE cbPermutationRes[24][4] ;
  BYTE cbLen=0 ;
  //��������
  Permutation(cbIndexArray, 0, 4, cbPermutationRes, cbLen) ;
  ASSERT(cbLen==24) ;
  if(cbLen!=24)
  {
    return MAX_COUNT ;
  }

  //������Ŀ
  BYTE cbMinSingleCardCount = MAX_COUNT ;
  //������Сֵ
  for(BYTE i=0; i<24; ++i)
  {
    //��������
    BYTE cbTmpCardData[MAX_COUNT] ;
    BYTE cbTmpCardCount = cbRemainCardCount ;
    CopyMemory(cbTmpCardData, cbRemainCard, cbRemainCardCount) ;

    for(BYTE j=0; j<4; ++j)
    {
      BYTE Index = cbPermutationRes[i][j] ;

      //У���±�
      ASSERT(Index>=0 && Index<4) ;
      if(Index<0 || Index>=4)
      {
        return MAX_COUNT ;
      }

      pGetAllCardFun pTmpGetAllCardFun = GetAllCardFunArray[Index] ;

      //��ȡ�˿�
      BYTE cbGetCardData[MAX_COUNT] ;
      BYTE cbGetCardCount=0 ;
      //��Ա����
      ((*this).*pTmpGetAllCardFun)(cbTmpCardData, cbTmpCardCount, cbGetCardData, cbGetCardCount) ;

      //ɾ���˿�
      if(cbGetCardCount!=0)
      {
        RemoveCard(cbGetCardData, cbGetCardCount, cbTmpCardData, cbTmpCardCount) ;
      }
      cbTmpCardCount -= cbGetCardCount ;
    }

    //���㵥��
    BYTE cbSingleCard[MAX_COUNT] ;
    BYTE cbSingleCardCount=0 ;
    GetAllSingleCard(cbTmpCardData, cbTmpCardCount, cbSingleCard, cbSingleCardCount) ;
    cbMinSingleCardCount = cbMinSingleCardCount > cbSingleCardCount ? cbSingleCardCount : cbMinSingleCardCount ;
  }

  return cbMinSingleCardCount ;
}

//�����˿�
VOID CGameLogic::SetUserCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
  CopyMemory(m_cbAllCardData[wChairID], cbCardData, cbCardCount*sizeof(BYTE)) ;
  m_cbUserCardCount[wChairID] = cbCardCount ;

  //�����˿�
  SortCardList(m_cbAllCardData[wChairID], cbCardCount, ST_ORDER) ;
}

//���õ���
VOID CGameLogic::SetBackCard(WORD wChairID, BYTE cbBackCardData[], BYTE cbCardCount)
{
  BYTE cbTmpCount = m_cbUserCardCount[wChairID] ;
  CopyMemory(m_cbAllCardData[wChairID]+cbTmpCount, cbBackCardData, cbCardCount*sizeof(BYTE)) ;
  m_cbUserCardCount[wChairID] += cbCardCount ;

  //�����˿�
  SortCardList(m_cbAllCardData[wChairID], m_cbUserCardCount[wChairID], ST_ORDER) ;
}

//����ׯ��
VOID CGameLogic::SetBanker(WORD wBanker)
{
  m_wBankerUser = wBanker ;
}
//�����˿�
VOID CGameLogic::SetLandScoreCardData(BYTE cbCardData[], BYTE cbCardCount)
{
  ASSERT(cbCardCount==MAX_COUNT) ;
  if(cbCardCount!=MAX_COUNT)
  {
    return ;
  }

  CopyMemory(m_cbLandScoreCardData, cbCardData, cbCardCount*sizeof(BYTE)) ;
  //�����˿�
  SortCardList(m_cbLandScoreCardData, cbCardCount, ST_ORDER) ;
}
//ɾ���˿�
VOID CGameLogic::RemoveUserCardData(WORD wChairID, BYTE cbRemoveCardData[], BYTE cbRemoveCardCount)
{
  bool bSuccess = RemoveCard(cbRemoveCardData, cbRemoveCardCount, m_cbAllCardData[wChairID], m_cbUserCardCount[wChairID]) ;
  ASSERT(bSuccess) ;
  m_cbUserCardCount[wChairID] -= cbRemoveCardCount ;

}

//�������ƣ��ȳ��ƣ�
VOID CGameLogic::BankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, tagOutCardResult & OutCardResult)
{
  //���±�û��
  tagOutCardTypeResult CardTypeResult[12+1] ;
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult)) ;

  //��ʼ����
  ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;

  BYTE cbLineCard[MAX_COUNT] ;
  BYTE cbThreeLineCard[MAX_COUNT] ;
  BYTE cbDoubleLineCard[MAX_COUNT] ;
  BYTE cbLineCardCount;
  BYTE cbThreeLineCardCount ;
  BYTE cbDoubleLineCount ;
  GetAllLineCard(cbHandCardData, cbHandCardCount, cbLineCard, cbLineCardCount) ;
  GetAllThreeCard(cbHandCardData, cbHandCardCount, cbThreeLineCard, cbThreeLineCardCount) ;
  GetAllDoubleCard(cbHandCardData, cbHandCardCount, cbDoubleLineCard, cbDoubleLineCount) ;

  WORD wUndersideOfBanker = (m_wBankerUser+1)%GAME_PLAYER ; //�����¼�
  WORD wUpsideOfBanker = (wUndersideOfBanker+1)%GAME_PLAYER ; //�����ϼ�

  //���ֻʣ˳�ƺ͵�ֻ�����ȳ�˳
  {
    if(cbLineCardCount+1==cbHandCardCount && CT_SINGLE==GetCardType(cbLineCard, cbLineCardCount))
    {
      OutCardResult.cbCardCount = cbLineCardCount ;
      CopyMemory(OutCardResult.cbResultCard, cbLineCard, cbLineCardCount) ;
    }
    else if(cbThreeLineCardCount+1==cbHandCardCount && CT_THREE_LINE==GetCardType(cbThreeLineCard, cbThreeLineCardCount))
    {
      OutCardResult.cbCardCount = cbThreeLineCardCount ;
      CopyMemory(OutCardResult.cbResultCard, cbThreeLineCard, cbThreeLineCardCount) ;
    }
    else if(cbDoubleLineCount+1==cbHandCardCount && CT_DOUBLE_LINE==GetCardType(cbDoubleLineCard, cbDoubleLineCount))
    {
      OutCardResult.cbCardCount = cbDoubleLineCount ;
      CopyMemory(OutCardResult.cbResultCard, cbDoubleLineCard, cbDoubleLineCount) ;
    }
    //˫��ը����һ��
    else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2))
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
    }

    if(OutCardResult.cbCardCount>0)
    {
      return ;
    }
  }

  //������һֻ
  if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }
  //����
  else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }
  //ֻʣһ����
  else if(CT_ERROR!=GetCardType(cbHandCardData, cbHandCardCount))
  {
    OutCardResult.cbCardCount = cbHandCardCount ;
    CopyMemory(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;
    return ;
  }

  //ֻʣһ�ź�һ��
  if(cbHandCardCount>=2)
  {
    //�ϼ��˿�
    tagOutCardTypeResult UpsideCanOutCardType1[13] ;
    ZeroMemory(UpsideCanOutCardType1, sizeof(UpsideCanOutCardType1)) ;
    tagOutCardTypeResult UpsideCanOutCardType2[13] ;
    ZeroMemory(UpsideCanOutCardType2, sizeof(UpsideCanOutCardType2)) ;

    //�¼��˿�
    tagOutCardTypeResult UndersideCanOutCardType1[13] ;
    ZeroMemory(UndersideCanOutCardType1, sizeof(UndersideCanOutCardType1)) ;
    tagOutCardTypeResult UndersideCanOutCardType2[13] ;
    ZeroMemory(UndersideCanOutCardType2, sizeof(UndersideCanOutCardType2)) ;

    BYTE cbFirstHandCardType = GetCardType(cbHandCardData, cbHandCardCount-1) ;
    BYTE cbSecondHandCardType = GetCardType(cbHandCardData+1, cbHandCardCount-1) ;

    if(CT_ERROR!=cbFirstHandCardType && cbFirstHandCardType!=CT_FOUR_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_TAKE_TWO)
    {
      AnalyseOutCardType(m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker], cbHandCardData, cbHandCardCount-1, UpsideCanOutCardType1) ;
      AnalyseOutCardType(m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker], cbHandCardData, cbHandCardCount-1, UndersideCanOutCardType1) ;
    }
    if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO)
    {
      AnalyseOutCardType(m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker], cbHandCardData+1, cbHandCardCount-1, UpsideCanOutCardType2) ;
      AnalyseOutCardType(m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker], cbHandCardData+1, cbHandCardCount-1, UndersideCanOutCardType2) ;
    }

    if(cbSecondHandCardType!=CT_ERROR && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO && UpsideCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 && UndersideCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 &&
       UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
      return ;
    }

    if(cbFirstHandCardType!=CT_ERROR && cbFirstHandCardType!=CT_FOUR_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_TAKE_TWO && UpsideCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 && UndersideCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 &&
       UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount-1) ;
      return ;
    }

    if(GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[wUpsideOfBanker][0]) &&
       GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[wUndersideOfBanker][0]) &&
       CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = 1 ;
      OutCardResult.cbResultCard[0] = cbHandCardData[0] ;
      return ;
    }

    if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       UpsideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0 && UndersideCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
      return ;
    }
  }



  {
    {
      //�����˿�
      tagOutCardTypeResult MeOutCardTypeResult[13] ;
      ZeroMemory(MeOutCardTypeResult, sizeof(MeOutCardTypeResult)) ;
      AnalyseOutCardType(cbHandCardData, cbHandCardCount, MeOutCardTypeResult) ;

      //���㵥��
      BYTE cbMinSingleCardCount[4] ;
      cbMinSingleCardCount[0]=MAX_COUNT ;
      cbMinSingleCardCount[1]=MAX_COUNT ;
      cbMinSingleCardCount[2]=MAX_COUNT ;
      cbMinSingleCardCount[3]=MAX_COUNT ;
      BYTE cbIndex[4]= {0} ;
      BYTE cbOutcardType[4]= {CT_ERROR} ;
      BYTE cbMinValue=MAX_COUNT ;
      BYTE cbMinSingleCountInFour=MAX_COUNT ;
      BYTE cbMinCardType=CT_ERROR ;
      BYTE cbMinIndex=0 ;

      //��ը�������
      for(BYTE cbCardType=CT_DOUBLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
      {

        tagOutCardTypeResult const &tmpCardResult = MeOutCardTypeResult[cbCardType] ;

        //��ͬ���ͣ���ͬ���ȣ���������������ͬ���Ϳ��ܳ��Ȳ�һ��
        BYTE cbThisHandCardCount = MAX_COUNT ;

        //�ϼ��˿�
        tagOutCardTypeResult UpsideOutCardTypeResult[13] ;
        ZeroMemory(UpsideOutCardTypeResult, sizeof(UpsideOutCardTypeResult)) ;

        //�¼��˿�
        tagOutCardTypeResult UndersideOutCardTypeResult[13] ;
        ZeroMemory(UndersideOutCardTypeResult, sizeof(UndersideOutCardTypeResult)) ;


        for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
        {
          BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

          //���·���
          if(tmpCardResult.cbEachHandCardCount[i]!=cbThisHandCardCount)
          {
            cbThisHandCardCount = tmpCardResult.cbEachHandCardCount[i] ;
            AnalyseOutCardType(m_cbAllCardData[wUpsideOfBanker], m_cbUserCardCount[wUpsideOfBanker],
                               tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,UpsideOutCardTypeResult) ;
            AnalyseOutCardType(m_cbAllCardData[wUndersideOfBanker], m_cbUserCardCount[wUndersideOfBanker],
                               tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,UndersideOutCardTypeResult) ;
          }
          BYTE cbMaxValue=0 ;
          BYTE Index = 0 ;

          //�з�����ѹס��
          if(UpsideOutCardTypeResult[cbCardType].cbCardTypeCount>0 || UndersideOutCardTypeResult[cbCardType].cbCardTypeCount>0)
          {
            continue ;
          }
          //�Ƿ��д���
          if(tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount)
          {
            bool bHaveLargeCard=false ;
            for(BYTE j=0; j<tmpCardResult.cbEachHandCardCount[i]; ++j)
            {
              if(GetCardLogicValue(tmpCardResult.cbCardData[i][j])>=15)
              {
                bHaveLargeCard=true ;
              }
              if(cbCardType!=CT_SINGLE_LINE && cbCardType!=CT_DOUBLE_LINE  && GetCardLogicValue(tmpCardResult.cbCardData[i][0])==14)
              {
                bHaveLargeCard=true ;
              }
            }

            if(bHaveLargeCard)
            {
              continue ;
            }
          }

          //����cbMinSingleCardCount[4]�����ֵ
          for(BYTE j=0; j<4; ++j)
          {
            if(cbMinSingleCardCount[j]>=cbTmpCount)
            {
              cbMinSingleCardCount[j] = cbTmpCount ;
              cbIndex[j] = i ;
              cbOutcardType[j] = cbCardType ;
              break ;
            }
          }

          //������Сֵ
          if(cbMinSingleCountInFour>=cbTmpCount)
          {
            //��С����
            cbMinCardType = cbCardType ;
            //��С�����е���С����
            cbMinSingleCountInFour=cbTmpCount ;
            //��С�����е���С��
            cbMinIndex=i ;
          }
        }
      }

      if(cbMinSingleCountInFour>=AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0)+3 &&
         (m_cbUserCardCount[wUndersideOfBanker]>=4 && m_cbUserCardCount[wUpsideOfBanker]>=4))
      {
        cbMinSingleCountInFour=MAX_COUNT ;
      }

      if(cbMinSingleCountInFour!=MAX_COUNT)
      {
        BYTE Index = cbMinIndex ;

        //ѡ����С��
        for(BYTE i=0; i<4; ++i)
        {
          if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
             GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[Index][0]))
          {
            Index = cbIndex[i] ;
          }
        }

        //������һֻ
        if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
        {
          OutCardResult.cbCardCount = 2 ;
          OutCardResult.cbResultCard[0] = 0x4f ;
          OutCardResult.cbResultCard[1] = 0x4e ;
          return ;
        }
        //����
        else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
        {
          OutCardResult.cbCardCount = 2 ;
          OutCardResult.cbResultCard[0] = 0x4f ;
          OutCardResult.cbResultCard[1] = 0x4e ;
          return ;
        }
        else
        {
          //���ñ���
          OutCardResult.cbCardCount=MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
          CopyMemory(OutCardResult.cbResultCard,MeOutCardTypeResult[cbMinCardType].cbCardData[Index],MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
          return ;
        }

        ASSERT(OutCardResult.cbCardCount>0) ;

        return ;
      }

      //��������˿�����5����û���ҵ��ʺϵ�����Ӵ����С
      if(OutCardResult.cbCardCount<=0 && (m_cbUserCardCount[wUndersideOfBanker]>=4 || m_cbUserCardCount[wUpsideOfBanker]>=4))
      {
        //ֻ��һ����ʱ���ܷŵ�����
        if(m_cbUserCardCount[m_wBankerUser]==1 && MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount>0)
        {
          BYTE Index=MAX_COUNT ;
          for(BYTE i=0; i<MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount; ++i)
          {
            if(GetCardLogicValue(MeOutCardTypeResult[CT_SINGLE].cbCardData[i][0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]))
            {
              Index=i ;
            }
            else
            {
              break ;
            }
          }

          if(MAX_COUNT!=Index)
          {
            OutCardResult.cbCardCount = MeOutCardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] ;
            CopyMemory(OutCardResult.cbResultCard, MeOutCardTypeResult[CT_SINGLE].cbCardData[Index], OutCardResult.cbCardCount) ;
            return ;
          }
        }
      }
    }
  }
  BYTE cbFirstCard=0 ;
  //��������2
  for(BYTE i=0; i<cbHandCardCount; ++i)
    if(GetCardLogicValue(cbHandCardData[i])<15)
    {
      cbFirstCard = i ;
      break ;
    }

  if(cbFirstCard<cbHandCardCount-1)
  {
    AnalyseOutCardType(cbHandCardData+cbFirstCard, cbHandCardCount-cbFirstCard, CardTypeResult) ;
  }
  else
  {
    AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;
  }

  //���㵥��
  BYTE cbMinSingleCardCount[4] ;
  cbMinSingleCardCount[0]=MAX_COUNT ;
  cbMinSingleCardCount[1]=MAX_COUNT ;
  cbMinSingleCardCount[2]=MAX_COUNT ;
  cbMinSingleCardCount[3]=MAX_COUNT ;
  BYTE cbIndex[4]= {0} ;
  BYTE cbOutcardType[4]= {CT_ERROR} ;
  BYTE cbMinValue=MAX_COUNT ;
  BYTE cbMinSingleCountInFour=MAX_COUNT ;
  BYTE cbMinCardType=CT_ERROR ;
  BYTE cbMinIndex=0 ;

  //��ը�������
  for(BYTE cbCardType=CT_SINGLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
  {
    tagOutCardTypeResult const &tmpCardResult = CardTypeResult[cbCardType] ;
    for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
    {
      BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

      BYTE cbMaxValue=0 ;
      BYTE Index = 0 ;

      //����cbMinSingleCardCount[4]�����ֵ
      for(BYTE j=0; j<4; ++j)
      {
        if(cbMinSingleCardCount[j]>=cbTmpCount)
        {
          cbMinSingleCardCount[j] = cbTmpCount ;
          cbIndex[j] = i ;
          cbOutcardType[j] = cbCardType ;
          break ;
        }
      }

      //������Сֵ
      if(cbMinSingleCountInFour>=cbTmpCount)
      {
        //��С����
        cbMinCardType = cbCardType ;
        //��С�����е���С����
        cbMinSingleCountInFour=cbTmpCount ;
        //��С�����е���С��
        cbMinIndex=i ;
      }
    }
  }

  if(cbMinSingleCountInFour!=MAX_COUNT)
  {
    BYTE Index = cbMinIndex ;

    //ѡ����С��
    for(BYTE i=0; i<4; ++i)
    {
      if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
         GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[Index][0]))
      {
        Index = cbIndex[i] ;
      }
    }

    //������һֻ
    if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
      return ;
    }
    //����
    else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
      return ;
    }
    else
    {
      //���ñ���
      OutCardResult.cbCardCount=CardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
      CopyMemory(OutCardResult.cbResultCard,CardTypeResult[cbMinCardType].cbCardData[Index],CardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
      return ;
    }

    ASSERT(OutCardResult.cbCardCount>0) ;

    return ;
  }
  //���ֻʣը��
  else
  {
    BYTE Index=0 ;
    BYTE cbLogicCardValue = GetCardLogicValue(0x4F)+1 ;
    //��Сը��
    for(BYTE i=0; i<CardTypeResult[CT_BOMB_CARD].cbCardTypeCount; ++i)
      if(cbLogicCardValue>GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]))
      {
        cbLogicCardValue = GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]) ;
        Index = i ;
      }

    //���ñ���
    OutCardResult.cbCardCount=CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index];
    CopyMemory(OutCardResult.cbResultCard,CardTypeResult[CT_BOMB_CARD].cbCardData[Index],CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index]*sizeof(BYTE));

    return ;
  }

  //�����û���������ͳ���С��һ��
  OutCardResult.cbCardCount = 1 ;
  OutCardResult.cbResultCard[0] = cbHandCardData[cbHandCardCount-1] ;

  return ;
}

//�������ƣ�����ƣ�
VOID CGameLogic::BankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
  //��ʼ����
  ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;

  //���±�û��
  tagOutCardTypeResult CardTypeResult[12+1] ;
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult)) ;

  //��������
  BYTE cbOutCardType = GetCardType(cbTurnCardData,cbTurnCardCount) ;
  AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

  WORD wUndersideUser = (m_wBankerUser+1)%GAME_PLAYER ;
  WORD wUpsideUser = (wUndersideUser+1)%GAME_PLAYER ;

  //ֻʣը��
  if(cbHandCardCount==CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0])
  {
    OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] ;
    CopyMemory(OutCardResult.cbResultCard,  CardTypeResult[CT_BOMB_CARD].cbCardData, OutCardResult.cbCardCount) ;

    return ;
  }
  //˫��ը����һ��
  else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2))
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return  ;
  }

  //ȡ���ĸ���С����
  BYTE cbMinSingleCardCount[4] ;
  cbMinSingleCardCount[0]=MAX_COUNT ;
  cbMinSingleCardCount[1]=MAX_COUNT ;
  cbMinSingleCardCount[2]=MAX_COUNT ;
  cbMinSingleCardCount[3]=MAX_COUNT ;
  BYTE cbIndex[4]= {0} ;
  BYTE cbMinSingleCountInFour=MAX_COUNT ;

  //�ɳ��˿ˣ������Ѿ����˵�ը���ˣ�
  tagOutCardTypeResult const &CanOutCard = CardTypeResult[cbOutCardType] ;

  for(BYTE i=0; i<CanOutCard.cbCardTypeCount; ++i)
  {
    //��С����
    BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,CanOutCard.cbCardData[i], CanOutCard.cbEachHandCardCount[i]) ;
    BYTE cbMaxValue=0 ;
    BYTE Index = 0 ;

    //����cbMinSingleCardCount[4]�����ֵ
    for(BYTE j=0; j<4; ++j)
    {
      if(cbMinSingleCardCount[j]>=cbTmpCount)
      {
        cbMinSingleCardCount[j] = cbTmpCount ;
        cbIndex[j] = i ;
        break ;
      }
    }

  }

  for(BYTE i=0; i<4; ++i)
    if(cbMinSingleCountInFour>cbMinSingleCardCount[i])
    {
      cbMinSingleCountInFour = cbMinSingleCardCount[i] ;
    }


  //ԭʼ������
  BYTE cbOriginSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,NULL,0) ;

  if(CanOutCard.cbCardTypeCount>0)
  {
    BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
    bool bFindCard = false ;
    BYTE cbCanOutIndex=0 ;
    for(BYTE i=0; i<4; ++i)
    {
      BYTE Index = cbIndex[i] ;

      if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+3)  && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
         cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
      {
        //��Դ���
        bool bNoLargeCard = true ;

        //���������������������4�����ҳ�����С��K���ƶ��Ҳ��ǳ������ϵ������ʱ�����ܳ�2ȥ��
        if(m_cbUserCardCount[wOutCardUser]>=4 && cbHandCardCount>=5  && CanOutCard.cbEachHandCardCount[Index]>=2 &&
           GetCardLogicValue(CanOutCard.cbCardData[Index][0])>=15 &&
           GetCardLogicValue(cbTurnCardData[0])<13 &&
           (wOutCardUser==wUndersideUser&&GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) || wOutCardUser==wUpsideUser&&GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[wUpsideUser][0])) &&
           CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
        {
          bNoLargeCard=false ;
        }

        //������û�д��ƣ���Էɻ���������Ĵ��ƣ�
        for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
        {
          if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 &&
             CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
          {
            bNoLargeCard = false ;
          }
        }
        if(bNoLargeCard)
        {
          bFindCard = true ;
          cbCanOutIndex = Index ;
          cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
        }
      }
    }

    if(bFindCard)
    {
      //�����
      BYTE cbLargestLogicCard ;
      if(wOutCardUser==wUndersideUser)
      {
        cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) ;
      }
      else if(wOutCardUser==wUpsideUser)
      {
        cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUpsideUser][0]) ;
      }
      bool bCanOut=true ;

      //��ֻѹ2
      if(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard)
      {
        if(GetCardColor(CanOutCard.cbCardData[cbCanOutIndex][0])==0x40 && GetCardLogicValue(cbTurnCardData[0])<=14 && cbHandCardCount>5)
        {
          bCanOut = false ;
        }
      }

      if(bCanOut)
      {
        //���ñ���
        OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
        CopyMemory(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

        return ;
      }
    }

    if(cbOutCardType==CT_SINGLE)
    {
      //�мҵ������
      BYTE cbLargestLogicCard ;
      if(wOutCardUser==wUndersideUser)
      {
        cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUndersideUser][0]) ;
      }
      else if(wOutCardUser==wUpsideUser)
      {
        cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[wUpsideUser][0]) ;
      }

      if(GetCardLogicValue(cbTurnCardData[0])==14 ||
         GetCardLogicValue(cbTurnCardData[0])>=cbLargestLogicCard ||
         (GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard-1) ||
         (wOutCardUser==wUndersideUser&&m_cbUserCardCount[wUndersideUser]<=5 || wOutCardUser==wUpsideUser&&m_cbUserCardCount[wUpsideUser]<=5))
      {
        //ȡһ�Ŵ��ڵ���2����Ҫ���мҳ����ƴ���ƣ�
        BYTE cbIndex=MAX_COUNT ;
        for(BYTE i=0; i<cbHandCardCount; ++i)
          if(GetCardLogicValue(cbHandCardData[i])>GetCardLogicValue(cbTurnCardData[0]) &&
             GetCardLogicValue(cbHandCardData[i])>=15)
          {
            cbIndex = i ;
          }
        if(cbIndex!=MAX_COUNT)
        {
          //���ñ���
          OutCardResult.cbCardCount=1;
          OutCardResult.cbResultCard[0] = cbHandCardData[cbIndex] ;

          return ;
        }
      }
    }


    BYTE cbMinSingleCount=MAX_COUNT ;
    BYTE Index=0 ;
    for(BYTE i=0; i<CardTypeResult[cbOutCardType].cbCardTypeCount; ++i)
    {
      BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[i], CardTypeResult[cbOutCardType].cbEachHandCardCount[i]) ;
      if(cbMinSingleCount>=cbTmpCount)
      {
        cbMinSingleCount = cbTmpCount ;
        Index = i ;
      }
    }
    //���ñ���
    OutCardResult.cbCardCount=CardTypeResult[cbOutCardType].cbEachHandCardCount[Index];
    CopyMemory(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[Index], OutCardResult.cbCardCount) ;

    return ;
  }

  //��Ҫ����ը��
  if(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount>0 && cbHandCardCount<=10)
  {
    tagOutCardTypeResult const &BomCard = CardTypeResult[CT_BOMB_CARD] ;
    BYTE cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[0][0]) ;
    BYTE Index = 0 ;
    for(BYTE i=0; i<BomCard.cbCardTypeCount; ++i)
    {
      if(cbMinLogicValue>GetCardLogicValue(BomCard.cbCardData[i][0]))
      {
        cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[i][0]) ;
        Index = i ;
      }
    }

    //�жϳ���ը����ĵ�����
    BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]) ;
    if(cbSingleCardCount>=3 || (cbOutCardType==CT_SINGLE && GetCardLogicValue(cbTurnCardData[0])<15))
    {
      return ;
    }

    //���ñ���
    OutCardResult.cbCardCount=BomCard.cbEachHandCardCount[Index];
    CopyMemory(OutCardResult.cbResultCard,BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]*sizeof(BYTE));

    return ;
  }

  return ;
}

//�����ϼң��ȳ��ƣ�
VOID CGameLogic::UpsideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wMeChairID, tagOutCardResult & OutCardResult)
{
  //���±�û��
  tagOutCardTypeResult CardTypeResult[12+1] ;
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult)) ;

  //��ʼ����
  ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;

  BYTE cbLineCard[MAX_COUNT] ;
  BYTE cbThreeLineCard[MAX_COUNT] ;
  BYTE cbDoubleLineCard[MAX_COUNT] ;
  BYTE cbLineCardCount;
  BYTE cbThreeLineCardCount ;
  BYTE cbDoubleLineCount ;
  GetAllLineCard(cbHandCardData, cbHandCardCount, cbLineCard, cbLineCardCount) ;
  GetAllThreeCard(cbHandCardData, cbHandCardCount, cbThreeLineCard, cbThreeLineCardCount) ;
  GetAllDoubleCard(cbHandCardData, cbHandCardCount, cbDoubleLineCard, cbDoubleLineCount) ;

  //�����˳�ƺ͵�ֻ��һ�ԣ����ҵ�ֻ��Աȵ�����С�����ȳ�˳
  {
    if(cbLineCardCount+1==cbHandCardCount && CT_SINGLE==GetCardType(cbLineCard, cbLineCardCount))
    {
      OutCardResult.cbCardCount = cbLineCardCount ;
      CopyMemory(OutCardResult.cbResultCard, cbLineCard, cbLineCardCount) ;
    }
    else if(cbThreeLineCardCount+1==cbHandCardCount && CT_THREE_LINE==GetCardType(cbThreeLineCard, cbThreeLineCardCount))
    {
      OutCardResult.cbCardCount = cbThreeLineCardCount ;
      CopyMemory(OutCardResult.cbResultCard, cbThreeLineCard, cbThreeLineCardCount) ;
    }
    else if(cbDoubleLineCount+1==cbHandCardCount && CT_DOUBLE_LINE==GetCardType(cbDoubleLineCard, cbDoubleLineCount))
    {
      OutCardResult.cbCardCount = cbDoubleLineCount ;
      CopyMemory(OutCardResult.cbResultCard, cbDoubleLineCard, cbDoubleLineCount) ;
    }
    //˫��ը����һ��
    else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2))
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
    }

    if(OutCardResult.cbCardCount>0)
    {
      return ;
    }
  }
  //������һֻ
  if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }
  //����
  else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }
  //ֻʣһ����
  else if(CT_ERROR!=GetCardType(cbHandCardData, cbHandCardCount))
  {
    OutCardResult.cbCardCount = cbHandCardCount ;
    CopyMemory(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;
    return ;
  }

  //ֻʣһ�ź�һ��
  if(cbHandCardCount>=2)
  {
    //�����˿�
    tagOutCardTypeResult BankerCanOutCardType1[13] ;
    ZeroMemory(BankerCanOutCardType1, sizeof(BankerCanOutCardType1)) ;
    tagOutCardTypeResult BankerCanOutCardType2[13] ;
    ZeroMemory(BankerCanOutCardType2, sizeof(BankerCanOutCardType2)) ;

    BYTE cbFirstHandCardType = GetCardType(cbHandCardData, cbHandCardCount-1) ;
    BYTE cbSecondHandCardType = GetCardType(cbHandCardData+1, cbHandCardCount-1) ;

    //�������Գ�����
    if(cbFirstHandCardType!=CT_ERROR)
    {
      AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData, cbHandCardCount-1, BankerCanOutCardType1) ;
    }
    if(cbSecondHandCardType!=CT_ERROR)
    {
      AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData+1, cbHandCardCount-1, BankerCanOutCardType2) ;
    }

    if(cbSecondHandCardType!=CT_ERROR && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
      return ;
    }

    if(cbFirstHandCardType!=CT_ERROR && cbFirstHandCardType!=CT_FOUR_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount-1) ;
      return ;
    }

    if(GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) &&
       CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = 1 ;
      OutCardResult.cbResultCard[0] = cbHandCardData[0] ;
      return ;
    }

    if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
      return ;
    }
  }


  //�¼�Ϊ���������ҵ����˿�����5��
  //  if(m_cbUserCardCount[m_wBankerUser]<=5)
  {
    //�����˿�
    tagOutCardTypeResult MeOutCardTypeResult[13] ;
    ZeroMemory(MeOutCardTypeResult, sizeof(MeOutCardTypeResult)) ;
    AnalyseOutCardType(cbHandCardData, cbHandCardCount, MeOutCardTypeResult) ;

    //�Լ��˿�
    WORD wFriendID ;
    for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
      if(wChairID!=m_wBankerUser && wMeChairID!=wChairID)
      {
        wFriendID = wChairID ;
      }

    //���㵥��
    BYTE cbMinSingleCardCount[4] ;
    cbMinSingleCardCount[0]=MAX_COUNT ;
    cbMinSingleCardCount[1]=MAX_COUNT ;
    cbMinSingleCardCount[2]=MAX_COUNT ;
    cbMinSingleCardCount[3]=MAX_COUNT ;
    BYTE cbIndex[4]= {0} ;
    BYTE cbOutcardType[4]= {CT_ERROR} ;
    BYTE cbMinValue=MAX_COUNT ;
    BYTE cbMinSingleCountInFour=MAX_COUNT ;
    BYTE cbMinCardType=CT_ERROR ;
    BYTE cbMinIndex=0 ;

    //��ը�������
    for(BYTE cbCardType=CT_DOUBLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
    {
      tagOutCardTypeResult const &tmpCardResult = MeOutCardTypeResult[cbCardType] ;

      //��ͬ���ͣ���ͬ���ȣ���������������ͬ���Ϳ��ܳ��Ȳ�һ��
      BYTE cbThisHandCardCount = MAX_COUNT ;

      //�����˿�
      tagOutCardTypeResult BankerCanOutCard[13] ;
      ZeroMemory(BankerCanOutCard, sizeof(BankerCanOutCard)) ;

      tagOutCardTypeResult FriendOutCardTypeResult[13] ;
      ZeroMemory(FriendOutCardTypeResult, sizeof(FriendOutCardTypeResult)) ;

      for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
      {
        BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

        //���·���
        if(tmpCardResult.cbEachHandCardCount[i]!=cbThisHandCardCount)
        {
          AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser],
                             tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,BankerCanOutCard) ;
          AnalyseOutCardType(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID],
                             tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,FriendOutCardTypeResult) ;
        }

        BYTE cbMaxValue=0 ;
        BYTE Index = 0 ;

        //��������ѹ�ƣ����Ҷ��Ѳ�����ѹ����
        if((BankerCanOutCard[cbCardType].cbCardTypeCount>0&&FriendOutCardTypeResult[cbCardType].cbCardTypeCount==0) || (BankerCanOutCard[cbCardType].cbCardTypeCount>0 && FriendOutCardTypeResult[cbCardType].cbCardTypeCount>0 &&
            GetCardLogicValue(FriendOutCardTypeResult[cbCardType].cbCardData[0][0])<=GetCardLogicValue(BankerCanOutCard[cbCardType].cbCardData[0][0])))
        {
          continue ;
        }
        //�Ƿ��д���
        if(tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount)
        {
          bool bHaveLargeCard=false ;
          for(BYTE j=0; j<tmpCardResult.cbEachHandCardCount[i]; ++j)
            if(GetCardLogicValue(tmpCardResult.cbCardData[i][j])>=15)
            {
              bHaveLargeCard=true ;
            }
          if(cbCardType!=CT_SINGLE_LINE && cbCardType!=CT_DOUBLE_LINE  && GetCardLogicValue(tmpCardResult.cbCardData[i][0])==14)
          {
            bHaveLargeCard=true ;
          }

          if(bHaveLargeCard)
          {
            continue ;
          }
        }

        //�����Ƿ�����ߵ������ﶼû�п���ը��
        if(tmpCardResult.cbEachHandCardCount[i]==m_cbUserCardCount[m_wBankerUser] &&
           GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])>GetCardLogicValue(tmpCardResult.cbCardData[i][0]))
        {
          continue ;
        }

        //����cbMinSingleCardCount[4]�����ֵ
        for(BYTE j=0; j<4; ++j)
        {
          if(cbMinSingleCardCount[j]>=cbTmpCount)
          {
            cbMinSingleCardCount[j] = cbTmpCount ;
            cbIndex[j] = i ;
            cbOutcardType[j] = cbCardType ;
            break ;
          }
        }

        //������Сֵ
        if(cbMinSingleCountInFour>=cbTmpCount)
        {
          //��С����
          cbMinCardType = cbCardType ;
          //��С�����е���С����
          cbMinSingleCountInFour=cbTmpCount ;
          //��С�����е���С��
          cbMinIndex=i ;
        }
      }
    }

    if(cbMinSingleCountInFour>=AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0)+3 &&
       m_cbUserCardCount[m_wBankerUser]>4)
    {
      cbMinSingleCountInFour=MAX_COUNT ;
    }

    if(cbMinSingleCountInFour!=MAX_COUNT)
    {
      BYTE Index = cbMinIndex ;

      //ѡ����С��
      for(BYTE i=0; i<4; ++i)
      {
        if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
           GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[Index][0]))
        {
          Index = cbIndex[i] ;
        }
      }

      //������һֻ
      if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
      {
        OutCardResult.cbCardCount = 2 ;
        OutCardResult.cbResultCard[0] = 0x4f ;
        OutCardResult.cbResultCard[1] = 0x4e ;
        return ;
      }
      //����
      else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
      {
        OutCardResult.cbCardCount = 2 ;
        OutCardResult.cbResultCard[0] = 0x4f ;
        OutCardResult.cbResultCard[1] = 0x4e ;
        return ;
      }
      else
      {
        //���ñ���
        OutCardResult.cbCardCount=MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
        CopyMemory(OutCardResult.cbResultCard,MeOutCardTypeResult[cbMinCardType].cbCardData[Index],MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
        return ;
      }

      ASSERT(OutCardResult.cbCardCount>0) ;

      return ;
    }

    //��������˿�����5����û���ҵ��ʺϵ�����Ӵ����С
    if(OutCardResult.cbCardCount<=0 && m_cbUserCardCount[m_wBankerUser]<=5)
    {
      //ֻ��һ����ʱ���ܷŵ�����
      if(m_cbUserCardCount[m_wBankerUser]==1 && MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount>0)
      {
        BYTE Index=MAX_COUNT ;
        for(BYTE i=0; i<MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount; ++i)
        {
          if(GetCardLogicValue(MeOutCardTypeResult[CT_SINGLE].cbCardData[i][0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]))
          {
            Index=i ;
          }
          else
          {
            break ;
          }
        }

        if(MAX_COUNT!=Index)
        {
          OutCardResult.cbCardCount = MeOutCardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] ;
          CopyMemory(OutCardResult.cbResultCard, MeOutCardTypeResult[CT_SINGLE].cbCardData[Index], OutCardResult.cbCardCount) ;
          return ;
        }
      }
    }
  }

  BYTE cbFirstCard=0 ;
  //��������2
  for(BYTE i=0; i<cbHandCardCount; ++i)
    if(GetCardLogicValue(cbHandCardData[i])<15)
    {
      cbFirstCard = i ;
      break ;
    }

  if(cbFirstCard<cbHandCardCount-1)
  {
    AnalyseOutCardType(cbHandCardData+cbFirstCard, cbHandCardCount-cbFirstCard, CardTypeResult) ;
  }
  else
  {
    AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;
  }

  //���㵥��
  BYTE cbMinSingleCardCount[4] ;
  cbMinSingleCardCount[0]=MAX_COUNT ;
  cbMinSingleCardCount[1]=MAX_COUNT ;
  cbMinSingleCardCount[2]=MAX_COUNT ;
  cbMinSingleCardCount[3]=MAX_COUNT ;
  BYTE cbIndex[4]= {0} ;
  BYTE cbOutcardType[4]= {CT_ERROR} ;
  BYTE cbMinValue=MAX_COUNT ;
  BYTE cbMinSingleCountInFour=MAX_COUNT ;
  BYTE cbMinCardType=CT_ERROR ;
  BYTE cbMinIndex=0 ;

  //������������
  BYTE cbBankerSingleCardData[MAX_COUNT] ;
  BYTE cbBankerSingleCardCount=AnalyseSinleCardCount(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], NULL, 0, cbBankerSingleCardData) ;
  BYTE cbBankerSingleCardLogic = 0 ;
  if(cbBankerSingleCardCount>2 && GetCardLogicValue(cbBankerSingleCardData[1])<=10)
  {
    cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[1]) ;
  }
  else if(cbBankerSingleCardCount>0 && GetCardLogicValue(cbBankerSingleCardData[0])<=10)
  {
    cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[0]) ;
  }

  //��ը�������
  for(BYTE cbCardType=CT_SINGLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
  {
    tagOutCardTypeResult const &tmpCardResult = CardTypeResult[cbCardType] ;
    for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
    {
      //���ܷ��ߵ���С��
      if(cbCardType==CT_SINGLE && GetCardLogicValue(tmpCardResult.cbCardData[i][0])<cbBankerSingleCardLogic)
      {
        continue ;
      }

      BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

      BYTE cbMaxValue=0 ;
      BYTE Index = 0 ;

      //����cbMinSingleCardCount[4]�����ֵ
      for(BYTE j=0; j<4; ++j)
      {
        if(cbMinSingleCardCount[j]>=cbTmpCount)
        {
          cbMinSingleCardCount[j] = cbTmpCount ;
          cbIndex[j] = i ;
          cbOutcardType[j] = cbCardType ;
          break ;
        }
      }

      //������Сֵ
      if(cbMinSingleCountInFour>=cbTmpCount)
      {
        //��С����
        cbMinCardType = cbCardType ;
        //��С�����е���С����
        cbMinSingleCountInFour=cbTmpCount ;
        //��С�����е���С��
        cbMinIndex=i ;
      }
    }
  }

  if(cbMinSingleCountInFour!=MAX_COUNT)
  {
    BYTE Index = cbMinIndex ;

    //ѡ����С��
    for(BYTE i=0; i<4; ++i)
    {
      if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
         GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[Index][0]))
      {
        Index = cbIndex[i] ;
      }
    }

    //������һֻ
    if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
      return ;
    }
    //����
    else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
      return ;
    }
    else
    {
      //���ñ���
      OutCardResult.cbCardCount=CardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
      CopyMemory(OutCardResult.cbResultCard,CardTypeResult[cbMinCardType].cbCardData[Index],CardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
      return ;
    }

    ASSERT(OutCardResult.cbCardCount>0) ;

    return ;
  }
  //���ֻʣը��
  else
  {
    BYTE Index=0 ;
    BYTE cbLogicCardValue = GetCardLogicValue(0x4F)+1 ;
    //��Сը��
    for(BYTE i=0; i<CardTypeResult[CT_BOMB_CARD].cbCardTypeCount; ++i)
      if(cbLogicCardValue>GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]))
      {
        cbLogicCardValue = GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]) ;
        Index = i ;
      }

    //���ñ���
    OutCardResult.cbCardCount=CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index];
    CopyMemory(OutCardResult.cbResultCard,CardTypeResult[CT_BOMB_CARD].cbCardData[Index],CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index]*sizeof(BYTE));

    return ;
  }

  //�����û���������ͳ���С��һ��
  OutCardResult.cbCardCount = 1 ;
  OutCardResult.cbResultCard[0] = cbHandCardData[cbHandCardCount-1] ;
  return ;
}

//�����ϼң�����ƣ�
VOID CGameLogic::UpsideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{

  //���±�û��
  tagOutCardTypeResult CardTypeResult[12+1] ;
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult)) ;

  //��ʼ����
  ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;

  //��������
  BYTE cbOutCardType = GetCardType(cbTurnCardData, cbTurnCardCount) ;

  //�����ɳ���
  tagOutCardTypeResult BankerOutCardTypeResult[13] ;
  ZeroMemory(BankerOutCardTypeResult, sizeof(BankerOutCardTypeResult)) ;

  AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], BankerOutCardTypeResult) ;
  AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

  //ֻʣը��
  if(cbHandCardCount==CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0])
  {
    OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] ;
    CopyMemory(OutCardResult.cbResultCard,  CardTypeResult[CT_BOMB_CARD].cbCardData, OutCardResult.cbCardCount) ;

    return ;
  }
  //˫��ը����һ��
  else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2))
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }

  //���ׯ��û�д���������ѹ�Լ���
  if(m_cbUserCardCount[m_wBankerUser]<=5 && wOutCardUser!=m_wBankerUser &&
     (BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount==0 ||
      GetCardLogicValue(BankerOutCardTypeResult[cbOutCardType].cbCardData[0][0])<=GetCardLogicValue(cbTurnCardData[0])) &&
     CardTypeResult[cbOutCardType].cbEachHandCardCount[0]!=cbHandCardCount)//����һ�γ���
  {
    //��������
    return ;
  }

  //�¼�Ϊ���������ҵ����˿�����5��
  if(m_cbUserCardCount[m_wBankerUser]<=5 && CardTypeResult[cbOutCardType].cbCardTypeCount>0 && cbOutCardType!=CT_BOMB_CARD &&
     ((GetCardLogicValue(cbTurnCardData[0])<12 && wOutCardUser!=m_wBankerUser && BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount>0) ||//�Լҳ���
      (wOutCardUser==m_wBankerUser)))//��������
  {
    BYTE Index=0;
    //Ѱ�ҿ���ѹס��������Сһ����
    BYTE cbThisOutTypeMinSingleCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[0], CardTypeResult[cbOutCardType].cbEachHandCardCount[0]) ;
    BYTE cbBestIndex = 255 ;
    for(BYTE i=0; i<CardTypeResult[cbOutCardType].cbCardTypeCount; ++i)
    {
      BYTE cbTmpSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[i], CardTypeResult[cbOutCardType].cbEachHandCardCount[i]) ;
      if((BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount>0 &&
          GetCardLogicValue(CardTypeResult[cbOutCardType].cbCardData[i][0])>=GetCardLogicValue(BankerOutCardTypeResult[cbOutCardType].cbCardData[0][0]) ||
          BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount==0) &&
         cbTmpSingleCardCount<=cbThisOutTypeMinSingleCount)
      {
        cbBestIndex = i ;
        cbThisOutTypeMinSingleCount = cbTmpSingleCardCount ;
      }

      if((BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount>0 &&
          GetCardLogicValue(CardTypeResult[cbOutCardType].cbCardData[i][0])>=GetCardLogicValue(BankerOutCardTypeResult[cbOutCardType].cbCardData[0][0]) ||
          BankerOutCardTypeResult[cbOutCardType].cbCardTypeCount==0))
      {
        Index = i ;
      }
      else
      {
        break ;
      }
    }

    if(cbBestIndex!=255)
    {
      OutCardResult.cbCardCount = CardTypeResult[cbOutCardType].cbEachHandCardCount[cbBestIndex] ;
      CopyMemory(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[cbBestIndex], OutCardResult.cbCardCount) ;
    }
    else
    {
      OutCardResult.cbCardCount = CardTypeResult[cbOutCardType].cbEachHandCardCount[Index] ;
      CopyMemory(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[Index], OutCardResult.cbCardCount) ;
    }

    return ;
  }

  //ȡ���ĸ���С����
  BYTE cbMinSingleCardCount[4] ;
  cbMinSingleCardCount[0]=MAX_COUNT ;
  cbMinSingleCardCount[1]=MAX_COUNT ;
  cbMinSingleCardCount[2]=MAX_COUNT ;
  cbMinSingleCardCount[3]=MAX_COUNT ;
  BYTE cbIndex[4]= {0} ;
  BYTE cbMinSingleCountInFour=MAX_COUNT ;

  //�ɳ��˿ˣ������Ѿ����˵�ը���ˣ�
  tagOutCardTypeResult const &CanOutCard = CardTypeResult[cbOutCardType] ;

  for(BYTE i=0; i<CanOutCard.cbCardTypeCount; ++i)
  {
    //��С����
    BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,CanOutCard.cbCardData[i], CanOutCard.cbEachHandCardCount[i]) ;
    BYTE cbMaxValue=0 ;
    BYTE Index = 0 ;

    //����cbMinSingleCardCount[4]�����ֵ
    for(BYTE j=0; j<4; ++j)
    {
      if(cbMinSingleCardCount[j]>=cbTmpCount)
      {
        cbMinSingleCardCount[j] = cbTmpCount ;
        cbIndex[j] = i ;
        break ;
      }
    }

  }

  for(BYTE i=0; i<4; ++i)
    if(cbMinSingleCountInFour>cbMinSingleCardCount[i])
    {
      cbMinSingleCountInFour = cbMinSingleCardCount[i] ;
    }


  //ԭʼ������
  BYTE cbOriginSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,NULL,0) ;

  //���ѳ���
  bool bFriendOut = m_wBankerUser!=wOutCardUser ;
  if(bFriendOut)
  {
    if(CanOutCard.cbCardTypeCount>0)
    {
      //������������
      BYTE cbBankerSingleCardData[MAX_COUNT] ;
      BYTE cbBankerSingleCardCount=AnalyseSinleCardCount(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], NULL, 0, cbBankerSingleCardData) ;
      BYTE cbBankerSingleCardLogic = 0 ;
      if(cbBankerSingleCardCount>2 && GetCardLogicValue(cbBankerSingleCardData[1])<=10)
      {
        cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[1]) ;
      }
      else if(cbBankerSingleCardCount>0 && GetCardLogicValue(cbBankerSingleCardData[0])<=10)
      {
        cbBankerSingleCardLogic = GetCardLogicValue(cbBankerSingleCardData[0]) ;
      }

      BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
      bool bFindCard = false ;
      BYTE cbCanOutIndex=0 ;
      for(BYTE i=0; i<4; ++i)
      {
        BYTE Index = cbIndex[i] ;

        bool bCanOut = false ;
        if(cbOutCardType==CT_SINGLE && GetCardLogicValue(cbTurnCardData[0])<cbBankerSingleCardLogic &&
           GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=14 && cbMinSingleCardCount[i]<cbOriginSingleCardCount+2)
        {
          bCanOut = true ;
        }

        //С��J���ƣ�����С��K������ɢ��
        if(bCanOut ||
           ((cbMinSingleCardCount[i]<cbOriginSingleCardCount+4 && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
             (GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=11 || (cbMinSingleCardCount[i]<cbOriginSingleCardCount)&&GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=13)) &&
            cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]) && cbHandCardCount>5))
        {
          //������û�д��ƣ���Էɻ���������Ĵ��ƣ�
          bool bNoLargeCard = true ;
          for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
          {
            //�д��ƶ��Ҳ���һ�γ���
            if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 &&
               CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
            {
              bNoLargeCard = false ;
            }
          }
          if(bNoLargeCard)
          {
            bFindCard = true ;
            cbCanOutIndex = Index ;
            cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
          }
        }
        else if(cbHandCardCount<5 && cbMinSingleCardCount[i]<cbOriginSingleCardCount+4 && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
                cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
        {
          bFindCard = true ;
          cbCanOutIndex = Index ;
          cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
        }
      }

      if(bFindCard)
      {

        //���ñ���
        OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
        CopyMemory(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

        return ;
      }
      //��������������
      else if(cbHandCardCount<=5)
      {
        BYTE cbMinLogicCard = GetCardLogicValue(0x4f)+1 ;
        BYTE cbCanOutIndex = 0 ;
        for(BYTE i=0; i<4; ++i)
          if(cbMinSingleCardCount[i]<MAX_COUNT && cbMinSingleCardCount[i]<=cbMinSingleCountInFour && cbMinLogicCard>GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) &&
             GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0])<=14)
          {
            cbMinLogicCard = GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) ;
            cbCanOutIndex = cbIndex[i] ;
          }

        if(cbMinLogicCard != (GetCardLogicValue(0x4f)+1))
        {
          //���ñ���
          OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
          CopyMemory(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

          return ;
        }
      }

      return ;
    }
    else
    {
      return ;
    }

  }
  //��������
  else
  {
    if(CanOutCard.cbCardTypeCount>0)
    {
      BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
      bool bFindCard = false ;
      BYTE cbCanOutIndex=0 ;
      for(BYTE i=0; i<4; ++i)
      {
        BYTE Index = cbIndex[i] ;

        if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+4)  && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
           cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
        {
          //��Դ���
          bool bNoLargeCard = true ;

          //������������������4�����ҵ���������С��K���ƶ��Ҳ��ǵ������ϵ������ʱ�����ܳ�2ȥ��
          if(m_cbUserCardCount[m_wBankerUser]>=4 && cbHandCardCount>=5  && CanOutCard.cbEachHandCardCount[Index]>=2 &&
             GetCardLogicValue(CanOutCard.cbCardData[Index][0])>=15 &&
             GetCardLogicValue(cbTurnCardData[0])<13 &&
             GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) &&
             CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
          {
            bNoLargeCard=false ;
          }

          //������û�д��ƣ���Էɻ���������Ĵ��ƣ�
          for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
          {
            if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 &&
               CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
            {
              bNoLargeCard = false ;
            }
          }
          if(bNoLargeCard)
          {
            bFindCard = true ;
            cbCanOutIndex = Index ;
            cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
          }
        }
      }

      if(bFindCard)
      {
        //�����������
        BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;
        bool bCanOut=true ;

        //��ֻѹ2
        if(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard)
        {
          if(GetCardColor(CanOutCard.cbCardData[cbCanOutIndex][0])==0x40 && GetCardLogicValue(cbTurnCardData[0])<=14 && cbHandCardCount>5)
          {
            bCanOut = false ;
          }
        }

        if(bCanOut)
        {
          //���ñ���
          OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
          CopyMemory(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

          return ;
        }
      }

      if(cbOutCardType==CT_SINGLE)
      {
        //�����������
        BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;

        if(GetCardLogicValue(cbTurnCardData[0])==14 ||
           GetCardLogicValue(cbTurnCardData[0])>=cbLargestLogicCard ||
           (GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard-1) ||
           m_cbUserCardCount[m_wBankerUser]<=5)
        {
          //ȡһ�Ŵ��ڵ���2����Ҫ�ȵ��������ƴ���ƣ�
          BYTE cbIndex=MAX_COUNT ;
          for(BYTE i=0; i<cbHandCardCount; ++i)
            if(GetCardLogicValue(cbHandCardData[i])>GetCardLogicValue(cbTurnCardData[0]) && GetCardLogicValue(cbHandCardData[i])>=15)
            {
              cbIndex = i ;
            }
          if(cbIndex!=MAX_COUNT)
          {
            //���ñ���
            OutCardResult.cbCardCount=1;
            OutCardResult.cbResultCard[0] = cbHandCardData[cbIndex] ;

            return ;
          }
        }
      }
    }

    //��Ҫ����ը��
    if(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount>0 && cbHandCardCount<=10)
    {
      tagOutCardTypeResult const &BomCard = CardTypeResult[CT_BOMB_CARD] ;
      BYTE cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[0][0]) ;
      BYTE Index = 0 ;
      for(BYTE i=0; i<BomCard.cbCardTypeCount; ++i)
      {
        if(cbMinLogicValue>GetCardLogicValue(BomCard.cbCardData[i][0]))
        {
          cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[i][0]) ;
          Index = i ;
        }
      }

      //�жϳ���ը����ĵ�����
      BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]) ;
      if(cbSingleCardCount>=3 || (cbOutCardType==CT_SINGLE && GetCardLogicValue(cbTurnCardData[0])<15))
      {
        return ;
      }

      //���ñ���
      OutCardResult.cbCardCount=BomCard.cbEachHandCardCount[Index];
      CopyMemory(OutCardResult.cbResultCard,BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]*sizeof(BYTE));

      return ;
    }

    return ;
  }
  return ;
}

//�����¼ң��ȳ��ƣ�
VOID CGameLogic::UndersideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wMeChairID,tagOutCardResult & OutCardResult)
{
  //���±�û��
  tagOutCardTypeResult CardTypeResult[12+1] ;
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult)) ;

  //��ʼ����
  ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;

  BYTE cbLineCard[MAX_COUNT] ;
  BYTE cbThreeLineCard[MAX_COUNT] ;
  BYTE cbDoubleLineCard[MAX_COUNT] ;
  BYTE cbLineCardCount;
  BYTE cbThreeLineCardCount ;
  BYTE cbDoubleLineCount ;
  GetAllLineCard(cbHandCardData, cbHandCardCount, cbLineCard, cbLineCardCount) ;
  GetAllThreeCard(cbHandCardData, cbHandCardCount, cbThreeLineCard, cbThreeLineCardCount) ;
  GetAllDoubleCard(cbHandCardData, cbHandCardCount, cbDoubleLineCard, cbDoubleLineCount) ;

  //�����˳�ƺ͵�ֻ��һ�ԣ����ҵ�ֻ��Աȵ�����С�����ȳ�˳
  {
    if(cbLineCardCount+1==cbHandCardCount && CT_SINGLE==GetCardType(cbLineCard, cbLineCardCount))
    {
      OutCardResult.cbCardCount = cbLineCardCount ;
      CopyMemory(OutCardResult.cbResultCard, cbLineCard, cbLineCardCount) ;
    }
    else if(cbThreeLineCardCount+1==cbHandCardCount && CT_THREE_LINE==GetCardType(cbThreeLineCard, cbThreeLineCardCount))
    {
      OutCardResult.cbCardCount = cbThreeLineCardCount ;
      CopyMemory(OutCardResult.cbResultCard, cbThreeLineCard, cbThreeLineCardCount) ;
    }
    else if(cbDoubleLineCount+1==cbHandCardCount && CT_DOUBLE_LINE==GetCardType(cbDoubleLineCard, cbDoubleLineCount))
    {
      OutCardResult.cbCardCount = cbDoubleLineCount ;
      CopyMemory(OutCardResult.cbResultCard, cbDoubleLineCard, cbDoubleLineCount) ;
    }
    //˫��ը����һ��
    else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2))
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
    }

    if(OutCardResult.cbCardCount>0)
    {
      return ;
    }
  }
  //������һֻ
  if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }
  //����
  else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }
  //ֻʣһ����
  else if(CT_ERROR!=GetCardType(cbHandCardData, cbHandCardCount))
  {
    OutCardResult.cbCardCount = cbHandCardCount ;
    CopyMemory(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount) ;
    return ;
  }

  //ֻʣһ�ź�һ��
  if(cbHandCardCount>=2)
  {
    //�����˿�
    tagOutCardTypeResult BankerCanOutCardType1[13] ;
    ZeroMemory(BankerCanOutCardType1, sizeof(BankerCanOutCardType1)) ;
    tagOutCardTypeResult BankerCanOutCardType2[13] ;
    ZeroMemory(BankerCanOutCardType2, sizeof(BankerCanOutCardType2)) ;

    BYTE cbFirstHandCardType = GetCardType(cbHandCardData, cbHandCardCount-1) ;
    BYTE cbSecondHandCardType = GetCardType(cbHandCardData+1, cbHandCardCount-1) ;

    //�������Գ�����
    if(cbFirstHandCardType!=CT_ERROR)
    {
      AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData, cbHandCardCount-1, BankerCanOutCardType1) ;
    }
    if(cbSecondHandCardType!=CT_ERROR)
    {
      AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], cbHandCardData+1, cbHandCardCount-1, BankerCanOutCardType2) ;
    }

    if(cbSecondHandCardType!=CT_ERROR && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType2[cbSecondHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
      return ;
    }

    if(cbFirstHandCardType!=CT_ERROR && cbFirstHandCardType!=CT_FOUR_TAKE_ONE && cbFirstHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType1[cbFirstHandCardType].cbCardTypeCount==0 && BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData, cbHandCardCount-1) ;
      return ;
    }

    if(GetCardLogicValue(cbHandCardData[0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) &&
       CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = 1 ;
      OutCardResult.cbResultCard[0] = cbHandCardData[0] ;
      return ;
    }

    if(CT_ERROR!=cbSecondHandCardType && cbSecondHandCardType!=CT_FOUR_TAKE_ONE && cbSecondHandCardType!= CT_FOUR_TAKE_TWO &&
       BankerCanOutCardType2[CT_BOMB_CARD].cbCardTypeCount==0)
    {
      OutCardResult.cbCardCount = cbHandCardCount-1 ;
      CopyMemory(OutCardResult.cbResultCard, cbHandCardData+1, cbHandCardCount-1) ;
      return ;
    }
  }


  //�¼�Ϊ���������ҵ����˿�����5��
  //  if(m_cbUserCardCount[m_wBankerUser]<=5)
  {
    //�����˿�
    tagOutCardTypeResult MeOutCardTypeResult[13] ;
    ZeroMemory(MeOutCardTypeResult, sizeof(MeOutCardTypeResult)) ;
    AnalyseOutCardType(cbHandCardData, cbHandCardCount, MeOutCardTypeResult) ;

    //�Լ��˿�
    WORD wFriendID ;
    for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
      if(wChairID!=m_wBankerUser && wMeChairID!=wChairID)
      {
        wFriendID = wChairID ;
      }

    /*
    BYTE cbFirstCard=0 ;
    //��������2
    for(BYTE i=0; i<cbHandCardCount; ++i)
    if(GetCardLogicValue(cbHandCardData[i])<15)
    {
    cbFirstCard = i ;
    break ;
    }


    if(cbFirstCard<cbHandCardCount-1)
    AnalyseOutCardType(cbHandCardData+cbFirstCard, cbHandCardCount-cbFirstCard, MeOutCardTypeResult) ;
    else
    AnalyseOutCardType(cbHandCardData, cbHandCardCount, MeOutCardTypeResult) ;*/


    //���㵥��
    BYTE cbMinSingleCardCount[4] ;
    cbMinSingleCardCount[0]=MAX_COUNT ;
    cbMinSingleCardCount[1]=MAX_COUNT ;
    cbMinSingleCardCount[2]=MAX_COUNT ;
    cbMinSingleCardCount[3]=MAX_COUNT ;
    BYTE cbIndex[4]= {0} ;
    BYTE cbOutcardType[4]= {CT_ERROR} ;
    BYTE cbMinValue=MAX_COUNT ;
    BYTE cbMinSingleCountInFour=MAX_COUNT ;
    BYTE cbMinCardType=CT_ERROR ;
    BYTE cbMinIndex=0 ;

    //��ը�������
    for(BYTE cbCardType=CT_DOUBLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
    {
      tagOutCardTypeResult const &tmpCardResult = MeOutCardTypeResult[cbCardType] ;

      //��ͬ���ͣ���ͬ���ȣ���������������ͬ���Ϳ��ܳ��Ȳ�һ��
      BYTE cbThisHandCardCount = MAX_COUNT ;

      //�����˿�
      tagOutCardTypeResult BankerCanOutCard[13] ;
      ZeroMemory(BankerCanOutCard, sizeof(BankerCanOutCard)) ;

      tagOutCardTypeResult FriendOutCardTypeResult[13] ;
      ZeroMemory(FriendOutCardTypeResult, sizeof(FriendOutCardTypeResult)) ;

      for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
      {
        BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

        //���·���
        if(tmpCardResult.cbEachHandCardCount[i]!=cbThisHandCardCount)
        {
          AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser],
                             tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,BankerCanOutCard) ;
          AnalyseOutCardType(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID],
                             tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i] ,FriendOutCardTypeResult) ;
        }

        BYTE cbMaxValue=0 ;
        BYTE Index = 0 ;

        //��������ѹ�ƣ����Ҷ��Ѳ�����ѹ����
        if((BankerCanOutCard[cbCardType].cbCardTypeCount>0&&FriendOutCardTypeResult[cbCardType].cbCardTypeCount==0) || (BankerCanOutCard[cbCardType].cbCardTypeCount>0 && FriendOutCardTypeResult[cbCardType].cbCardTypeCount>0 &&
            GetCardLogicValue(FriendOutCardTypeResult[cbCardType].cbCardData[0][0])<=GetCardLogicValue(BankerCanOutCard[cbCardType].cbCardData[0][0])))
        {
          continue ;
        }
        //�Ƿ��д���
        if(tmpCardResult.cbEachHandCardCount[i] != cbHandCardCount)
        {
          bool bHaveLargeCard=false ;
          for(BYTE j=0; j<tmpCardResult.cbEachHandCardCount[i]; ++j)
            if(GetCardLogicValue(tmpCardResult.cbCardData[i][j])>=15)
            {
              bHaveLargeCard=true ;
            }

          if(cbCardType!=CT_SINGLE_LINE && cbCardType!=CT_DOUBLE_LINE && GetCardLogicValue(tmpCardResult.cbCardData[i][0])==14)
          {
            bHaveLargeCard=true ;
          }

          if(bHaveLargeCard)
          {
            continue ;
          }
        }

        //�����Ƿ�����ߵ������ﶼû�п���ը��
        if(tmpCardResult.cbEachHandCardCount[i]==m_cbUserCardCount[m_wBankerUser] &&
           GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0])>GetCardLogicValue(tmpCardResult.cbCardData[i][0]))
        {
          continue ;
        }

        //����cbMinSingleCardCount[4]�����ֵ
        for(BYTE j=0; j<4; ++j)
        {
          if(cbMinSingleCardCount[j]>=cbTmpCount)
          {
            cbMinSingleCardCount[j] = cbTmpCount ;
            cbIndex[j] = i ;
            cbOutcardType[j] = cbCardType ;
            break ;
          }
        }

        //������Сֵ
        if(cbMinSingleCountInFour>=cbTmpCount)
        {
          //��С����
          cbMinCardType = cbCardType ;
          //��С�����е���С����
          cbMinSingleCountInFour=cbTmpCount ;
          //��С�����е���С��
          cbMinIndex=i ;
        }
      }
    }

    if(cbMinSingleCountInFour>=AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, NULL, 0)+3 &&
       m_cbUserCardCount[m_wBankerUser]>4)
    {
      cbMinSingleCountInFour=MAX_COUNT ;
    }

    if(cbMinSingleCountInFour!=MAX_COUNT)
    {
      BYTE Index = cbMinIndex ;

      //ѡ����С��
      for(BYTE i=0; i<4; ++i)
      {
        if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
           GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(MeOutCardTypeResult[cbMinCardType].cbCardData[Index][0]))
        {
          Index = cbIndex[i] ;
        }
      }

      //������һֻ
      if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
      {
        OutCardResult.cbCardCount = 2 ;
        OutCardResult.cbResultCard[0] = 0x4f ;
        OutCardResult.cbResultCard[1] = 0x4e ;
        return ;
      }
      //����
      else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
      {
        OutCardResult.cbCardCount = 2 ;
        OutCardResult.cbResultCard[0] = 0x4f ;
        OutCardResult.cbResultCard[1] = 0x4e ;
        return ;
      }
      else
      {
        //���ñ���
        OutCardResult.cbCardCount=MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
        CopyMemory(OutCardResult.cbResultCard,MeOutCardTypeResult[cbMinCardType].cbCardData[Index],MeOutCardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
        return ;
      }

      ASSERT(OutCardResult.cbCardCount>0) ;

      return ;
    }

    //��������˿�����5����û���ҵ��ʺϵ�����Ӵ����С
    if(OutCardResult.cbCardCount<=0 && m_cbUserCardCount[m_wBankerUser]<=5)
    {
      //ֻ��һ����ʱ���ܷŵ�����
      if(m_cbUserCardCount[m_wBankerUser]==1 && MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount>0)
      {
        BYTE Index=MAX_COUNT ;
        for(BYTE i=0; i<MeOutCardTypeResult[CT_SINGLE].cbCardTypeCount; ++i)
        {
          if(GetCardLogicValue(MeOutCardTypeResult[CT_SINGLE].cbCardData[i][0])>=GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]))
          {
            Index=i ;
          }
          else
          {
            break ;
          }
        }

        if(MAX_COUNT!=Index)
        {
          OutCardResult.cbCardCount = MeOutCardTypeResult[CT_SINGLE].cbEachHandCardCount[Index] ;
          CopyMemory(OutCardResult.cbResultCard, MeOutCardTypeResult[CT_SINGLE].cbCardData[Index], OutCardResult.cbCardCount) ;
          return ;
        }
      }
    }
  }

  BYTE cbFirstCard=0 ;
  //��������2
  for(BYTE i=0; i<cbHandCardCount; ++i)
    if(GetCardLogicValue(cbHandCardData[i])<15)
    {
      cbFirstCard = i ;
      break ;
    }

  if(cbFirstCard<cbHandCardCount-1)
  {
    AnalyseOutCardType(cbHandCardData+cbFirstCard, cbHandCardCount-cbFirstCard, CardTypeResult) ;
  }
  else
  {
    AnalyseOutCardType(cbHandCardData, cbHandCardCount, CardTypeResult) ;
  }

  //���㵥��
  BYTE cbMinSingleCardCount[4] ;
  cbMinSingleCardCount[0]=MAX_COUNT ;
  cbMinSingleCardCount[1]=MAX_COUNT ;
  cbMinSingleCardCount[2]=MAX_COUNT ;
  cbMinSingleCardCount[3]=MAX_COUNT ;
  BYTE cbIndex[4]= {0} ;
  BYTE cbOutcardType[4]= {CT_ERROR} ;
  BYTE cbMinValue=MAX_COUNT ;
  BYTE cbMinSingleCountInFour=MAX_COUNT ;
  BYTE cbMinCardType=CT_ERROR ;
  BYTE cbMinIndex=0 ;

  //��ը�������
  for(BYTE cbCardType=CT_SINGLE; cbCardType<CT_BOMB_CARD; ++cbCardType)
  {
    tagOutCardTypeResult const &tmpCardResult = CardTypeResult[cbCardType] ;
    for(BYTE i=0; i<tmpCardResult.cbCardTypeCount; ++i)
    {
      BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, tmpCardResult.cbCardData[i], tmpCardResult.cbEachHandCardCount[i]) ;

      BYTE cbMaxValue=0 ;
      BYTE Index = 0 ;

      //����cbMinSingleCardCount[4]�����ֵ
      for(BYTE j=0; j<4; ++j)
      {
        if(cbMinSingleCardCount[j]>=cbTmpCount)
        {
          cbMinSingleCardCount[j] = cbTmpCount ;
          cbIndex[j] = i ;
          cbOutcardType[j] = cbCardType ;
          break ;
        }
      }

      //������Сֵ
      if(cbMinSingleCountInFour>=cbTmpCount)
      {
        //��С����
        cbMinCardType = cbCardType ;
        //��С�����е���С����
        cbMinSingleCountInFour=cbTmpCount ;
        //��С�����е���С��
        cbMinIndex=i ;
      }
    }
  }

  if(cbMinSingleCountInFour!=MAX_COUNT)
  {
    BYTE Index = cbMinIndex ;

    //ѡ����С��
    for(BYTE i=0; i<4; ++i)
    {
      if(cbOutcardType[i]==cbMinCardType && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
         GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[cbIndex[i]][0])<GetCardLogicValue(CardTypeResult[cbMinCardType].cbCardData[Index][0]))
      {
        Index = cbIndex[i] ;
      }
    }

    //������һֻ
    if(cbHandCardCount==3 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
      return ;
    }
    //����
    else if(cbHandCardCount==2 && GetCardColor(cbHandCardData[0])==0x40 && GetCardColor(cbHandCardData[1])==0x40)
    {
      OutCardResult.cbCardCount = 2 ;
      OutCardResult.cbResultCard[0] = 0x4f ;
      OutCardResult.cbResultCard[1] = 0x4e ;
      return ;
    }
    else
    {
      //���ñ���
      OutCardResult.cbCardCount=CardTypeResult[cbMinCardType].cbEachHandCardCount[Index];
      CopyMemory(OutCardResult.cbResultCard,CardTypeResult[cbMinCardType].cbCardData[Index],CardTypeResult[cbMinCardType].cbEachHandCardCount[Index]*sizeof(BYTE));
      return ;
    }

    ASSERT(OutCardResult.cbCardCount>0) ;

    return ;
  }
  //���ֻʣը��
  else
  {
    BYTE Index=0 ;
    BYTE cbLogicCardValue = GetCardLogicValue(0x4F)+1 ;
    //��Сը��
    for(BYTE i=0; i<CardTypeResult[CT_BOMB_CARD].cbCardTypeCount; ++i)
      if(cbLogicCardValue>GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]))
      {
        cbLogicCardValue = GetCardLogicValue(CardTypeResult[CT_BOMB_CARD].cbCardData[i][0]) ;
        Index = i ;
      }

    //���ñ���
    OutCardResult.cbCardCount=CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index];
    CopyMemory(OutCardResult.cbResultCard,CardTypeResult[CT_BOMB_CARD].cbCardData[Index],CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[Index]*sizeof(BYTE));

    return ;
  }

  //�����û���������ͳ���С��һ��
  OutCardResult.cbCardCount = 1 ;
  OutCardResult.cbResultCard[0] = cbHandCardData[cbHandCardCount-1] ;
  return ;
}
//�����¼ң�����ƣ�
VOID CGameLogic::UndersideOfBankerOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, WORD wOutCardUser, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
  //��ʼ����
  ZeroMemory(&OutCardResult, sizeof(OutCardResult)) ;

  //���±�û��
  tagOutCardTypeResult CardTypeResult[12+1] ;
  ZeroMemory(CardTypeResult, sizeof(CardTypeResult)) ;

  //��������
  BYTE cbOutCardType = GetCardType(cbTurnCardData,cbTurnCardCount) ;

  //�����ɳ���
  tagOutCardTypeResult BankerOutCardTypeResult[13] ;
  ZeroMemory(BankerOutCardTypeResult, sizeof(BankerOutCardTypeResult)) ;

  AnalyseOutCardType(m_cbAllCardData[m_wBankerUser], m_cbUserCardCount[m_wBankerUser], BankerOutCardTypeResult) ;
  AnalyseOutCardType(cbHandCardData,cbHandCardCount,cbTurnCardData,cbTurnCardCount, CardTypeResult) ;

  //ֻʣը��
  if(cbHandCardCount==CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0])
  {
    OutCardResult.cbCardCount = CardTypeResult[CT_BOMB_CARD].cbEachHandCardCount[0] ;
    CopyMemory(OutCardResult.cbResultCard,  CardTypeResult[CT_BOMB_CARD].cbCardData, OutCardResult.cbCardCount) ;

    return ;
  }
  //˫��ը����һ��
  else if(cbHandCardCount>2 && cbHandCardData[0]==0x4f && cbHandCardData[1]==0x4e && CT_ERROR!=GetCardType(cbHandCardData+2, cbHandCardCount-2))
  {
    OutCardResult.cbCardCount = 2 ;
    OutCardResult.cbResultCard[0] = 0x4f ;
    OutCardResult.cbResultCard[1] = 0x4e ;
    return ;
  }

  //ȡ���ĸ���С����
  BYTE cbMinSingleCardCount[4] ;
  cbMinSingleCardCount[0]=MAX_COUNT ;
  cbMinSingleCardCount[1]=MAX_COUNT ;
  cbMinSingleCardCount[2]=MAX_COUNT ;
  cbMinSingleCardCount[3]=MAX_COUNT ;
  BYTE cbIndex[4]= {0} ;
  BYTE cbMinSingleCountInFour=MAX_COUNT ;

  //�ɳ��˿ˣ������Ѿ����˵�ը���ˣ�
  tagOutCardTypeResult const &CanOutCard = CardTypeResult[cbOutCardType] ;

  for(BYTE i=0; i<CanOutCard.cbCardTypeCount; ++i)
  {
    //��С����
    BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,CanOutCard.cbCardData[i], CanOutCard.cbEachHandCardCount[i]) ;
    BYTE cbMaxValue=0 ;
    BYTE Index = 0 ;

    //����cbMinSingleCardCount[4]�����ֵ
    for(BYTE j=0; j<4; ++j)
    {
      if(cbMinSingleCardCount[j]>=cbTmpCount)
      {
        cbMinSingleCardCount[j] = cbTmpCount ;
        cbIndex[j] = i ;
        break ;
      }
    }

  }

  for(BYTE i=0; i<4; ++i)
    if(cbMinSingleCountInFour>cbMinSingleCardCount[i])
    {
      cbMinSingleCountInFour = cbMinSingleCardCount[i] ;
    }


  //ԭʼ������
  BYTE cbOriginSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount,NULL,0) ;

  //���ѳ���
  bool bFriendOut = m_wBankerUser!=wOutCardUser ;
  if(bFriendOut)
  {
    if(CanOutCard.cbCardTypeCount>0)
    {
      BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
      bool bFindCard = false ;
      BYTE cbCanOutIndex=0 ;
      for(BYTE i=0; i<4; ++i)
      {
        BYTE Index = cbIndex[i] ;
        //С��J���ƣ�����С��K������ɢ��
        if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+4 && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
            (GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=11 || (cbMinSingleCardCount[i]<cbOriginSingleCardCount)&&GetCardLogicValue(CanOutCard.cbCardData[Index][0])<=13)) &&
           cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]) && cbHandCardCount>5)
        {
          //������û�д��ƣ���Էɻ���������Ĵ��ƣ�
          bool bNoLargeCard = true ;
          for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
          {
            //�д��ƶ��Ҳ���һ�γ���
            if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 &&
               CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
            {
              bNoLargeCard = false ;
            }
          }
          if(bNoLargeCard)
          {
            bFindCard = true ;
            cbCanOutIndex = Index ;
            cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
          }
        }
        else if(cbHandCardCount<5 && cbMinSingleCardCount[i]<cbOriginSingleCardCount+4 && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
                cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
        {
          bFindCard = true ;
          cbCanOutIndex = Index ;
          cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
        }
      }

      if(bFindCard)
      {

        //���ñ���
        OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
        CopyMemory(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

        return ;
      }
      //��������������
      else if(cbHandCardCount<=5)
      {
        BYTE cbMinLogicCard = GetCardLogicValue(0x4f)+1 ;
        BYTE cbCanOutIndex = 0 ;
        for(BYTE i=0; i<4; ++i)
          if(cbMinSingleCardCount[i]<MAX_COUNT && cbMinSingleCardCount[i]<=cbMinSingleCountInFour && cbMinLogicCard>GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) &&
             GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0])<=14)
          {
            cbMinLogicCard = GetCardLogicValue(CanOutCard.cbCardData[cbIndex[i]][0]) ;
            cbCanOutIndex = cbIndex[i] ;
          }

        if(cbMinLogicCard != (GetCardLogicValue(0x4f)+1))
        {
          //���ñ���
          OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
          CopyMemory(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

          return ;
        }
      }

      return ;
    }
    else
    {
      return ;
    }

  }
  //��������
  else
  {
    if(CanOutCard.cbCardTypeCount>0)
    {
      BYTE cbMinLogicCardValue = GetCardLogicValue(0x4F)+1 ;
      bool bFindCard = false ;
      BYTE cbCanOutIndex=0 ;
      for(BYTE i=0; i<4; ++i)
      {
        BYTE Index = cbIndex[i] ;

        if((cbMinSingleCardCount[i]<cbOriginSingleCardCount+4)  && cbMinSingleCardCount[i]<=cbMinSingleCountInFour &&
           cbMinLogicCardValue>GetCardLogicValue(CanOutCard.cbCardData[Index][0]))
        {
          //��Դ���
          bool bNoLargeCard = true ;

          //������������������4�����ҵ���������С��K���ƶ��Ҳ��ǵ������ϵ������ʱ�����ܳ�2ȥ��
          if(m_cbUserCardCount[m_wBankerUser]>=4 && cbHandCardCount>=5 && CanOutCard.cbEachHandCardCount[Index]>=2 &&
             GetCardLogicValue(CanOutCard.cbCardData[Index][0])>=15 &&
             GetCardLogicValue(cbTurnCardData[0])<13 &&
             GetCardLogicValue(cbTurnCardData[0])<GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) &&
             CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
          {
            bNoLargeCard=false ;
          }

          //������û�д��ƣ���Էɻ���������Ĵ��ƣ�
          for(BYTE k=3; k<CanOutCard.cbEachHandCardCount[Index]; ++k)
          {
            if(GetCardLogicValue(CanOutCard.cbCardData[Index][k])>=15 &&
               CanOutCard.cbEachHandCardCount[Index]!=cbHandCardCount)
            {
              bNoLargeCard = false ;
            }
          }
          if(bNoLargeCard)
          {
            bFindCard = true ;
            cbCanOutIndex = Index ;
            cbMinLogicCardValue = GetCardLogicValue(CanOutCard.cbCardData[Index][0]) ;
          }
        }
      }

      if(bFindCard)
      {
        //�����������
        BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;
        bool bCanOut=true ;

        //��ֻѹ2
        if(GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard)
        {
          if(GetCardColor(CanOutCard.cbCardData[cbCanOutIndex][0])==0x40 && GetCardLogicValue(cbTurnCardData[0])<=14 && cbHandCardCount>5)
          {
            bCanOut = false ;
          }
        }

        if(bCanOut)
        {
          //���ñ���
          OutCardResult.cbCardCount=CanOutCard.cbEachHandCardCount[cbCanOutIndex];
          CopyMemory(OutCardResult.cbResultCard,CanOutCard.cbCardData[cbCanOutIndex],CanOutCard.cbEachHandCardCount[cbCanOutIndex]*sizeof(BYTE));

          return ;
        }
      }

      if(cbOutCardType==CT_SINGLE)
      {
        //�����������
        BYTE cbLargestLogicCard = GetCardLogicValue(m_cbAllCardData[m_wBankerUser][0]) ;

        if(GetCardLogicValue(cbTurnCardData[0])==14 ||
           GetCardLogicValue(cbTurnCardData[0])>=cbLargestLogicCard ||
           (GetCardLogicValue(cbTurnCardData[0])<cbLargestLogicCard-1) ||
           m_cbUserCardCount[m_wBankerUser]<=5)
        {
          //ȡһ�Ŵ��ڵ���2����Ҫ�ȵ��������ƴ���ƣ�
          BYTE cbIndex=MAX_COUNT ;
          for(BYTE i=0; i<cbHandCardCount; ++i)
            if(GetCardLogicValue(cbHandCardData[i])>GetCardLogicValue(cbTurnCardData[0]) &&
               GetCardLogicValue(cbHandCardData[i])>=15)
            {
              cbIndex = i ;
            }
          if(cbIndex!=MAX_COUNT)
          {
            //���ñ���
            OutCardResult.cbCardCount=1;
            OutCardResult.cbResultCard[0] = cbHandCardData[cbIndex] ;

            return ;
          }
        }
      }

      //�����Ѳ������ص���ʱ
      WORD wMeChairID = (m_wBankerUser+1)%GAME_PLAYER ;
      WORD wFriendID = (wMeChairID+1)%GAME_PLAYER ;

      tagOutCardTypeResult FriendCardTypeResult[13] ;
      ZeroMemory(FriendCardTypeResult, sizeof(FriendCardTypeResult)) ;
      AnalyseOutCardType(m_cbAllCardData[wFriendID], m_cbUserCardCount[wFriendID], cbTurnCardData, cbTurnCardCount, FriendCardTypeResult) ;

      //�����Ѳ������ص���ʱ
      if(m_cbUserCardCount[m_wBankerUser]<=4 && FriendCardTypeResult[cbOutCardType].cbCardTypeCount==0 && CardTypeResult[cbOutCardType].cbCardTypeCount>0)
      {
        BYTE cbMinSingleCount=MAX_COUNT ;
        BYTE Index=0 ;
        for(BYTE i=0; i<CardTypeResult[cbOutCardType].cbCardTypeCount; ++i)
        {
          BYTE cbTmpCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, CardTypeResult[cbOutCardType].cbCardData[i], CardTypeResult[cbOutCardType].cbEachHandCardCount[i]) ;
          if(cbMinSingleCount>=cbTmpCount)
          {
            cbMinSingleCount = cbTmpCount ;
            Index = i ;
          }
        }
        //���ñ���
        OutCardResult.cbCardCount=CardTypeResult[cbOutCardType].cbEachHandCardCount[Index];
        CopyMemory(OutCardResult.cbResultCard, CardTypeResult[cbOutCardType].cbCardData[Index], OutCardResult.cbCardCount) ;

        return ;
      }
    }

    //��Ҫ����ը��
    if(CardTypeResult[CT_BOMB_CARD].cbCardTypeCount>0 && cbHandCardCount<=10)
    {
      tagOutCardTypeResult const &BomCard = CardTypeResult[CT_BOMB_CARD] ;
      BYTE cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[0][0]) ;
      BYTE Index = 0 ;
      for(BYTE i=0; i<BomCard.cbCardTypeCount; ++i)
      {
        if(cbMinLogicValue>GetCardLogicValue(BomCard.cbCardData[i][0]))
        {
          cbMinLogicValue = GetCardLogicValue(BomCard.cbCardData[i][0]) ;
          Index = i ;
        }
      }

      //�жϳ���ը����ĵ�����
      BYTE cbSingleCardCount = AnalyseSinleCardCount(cbHandCardData, cbHandCardCount, BomCard.cbCardData[Index], BomCard.cbEachHandCardCount[Index]) ;
      if(cbSingleCardCount>=3 || (cbOutCardType==CT_SINGLE && GetCardLogicValue(cbTurnCardData[0])<15))
      {
        return ;
      }

      //���ñ���
      OutCardResult.cbCardCount=BomCard.cbEachHandCardCount[Index];
      CopyMemory(OutCardResult.cbResultCard,BomCard.cbCardData[Index],BomCard.cbEachHandCardCount[Index]*sizeof(BYTE));

      return ;
    }

    return ;
  }
  return ;
}

//�з��ж�
BYTE CGameLogic::LandScore(WORD wMeChairID, BYTE cbCurrentLandScore)
{
  //������Ŀ
  BYTE cbLargeCardCount = 0 ;
  BYTE Index=0 ;
  while(GetCardLogicValue(m_cbLandScoreCardData[Index++])>=15)
  {
    ++cbLargeCardCount ;
  }

  //���Ƹ���
  BYTE cbSingleCardCount=AnalyseSinleCardCount(m_cbLandScoreCardData, sizeof(m_cbLandScoreCardData), NULL, 0) ;

  //������
  if(cbLargeCardCount>=4 && cbSingleCardCount<=4)
  {
    return 2 ;
  }

  //�����з�
  if(cbLargeCardCount<=2 || cbCurrentLandScore==1)
  {
    //�����з�
    return 255;
  }

  //��������
  BYTE cbMinSingleCardCount=MAX_COUNT ;
  for(WORD wChairID=0 , i=0; wChairID<GAME_PLAYER; ++wChairID)
  {
    BYTE cbTmpSingleCardCount = AnalyseSinleCardCount(m_cbAllCardData[wChairID], NORMAL_COUNT, NULL, 0) ;
    if(wChairID!=wMeChairID && cbTmpSingleCardCount<cbMinSingleCardCount)
    {
      cbTmpSingleCardCount=cbMinSingleCardCount ;
    }
  }

  //��һ��
  if(cbLargeCardCount>=3 && cbSingleCardCount<cbMinSingleCardCount-3)
  {
    return 1 ;
  }

  //�����з�
  return 255;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
