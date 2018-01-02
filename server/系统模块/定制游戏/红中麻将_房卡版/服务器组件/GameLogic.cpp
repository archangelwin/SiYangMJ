#include "StdAfx.h"
#include "GameLogic.h"
#include "math.h"
#include <algorithm>
#include "tbl/hulib.h"


//////////////////////////////////////////////////////////////////////////

//��̬����
bool		CChiHuRight::m_bInit = false;
DWORD		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//���캯��
CChiHuRight::CChiHuRight()
{
	ZeroMemory(m_dwRight,sizeof(m_dwRight));

	if(!m_bInit)
	{
		m_bInit = true;
		for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
		{
			if(0 == i)
				m_dwRightMask[i] = 0;
			else
				m_dwRightMask[i] = (DWORD(pow(2,i-1)))<<28;
		}
	}
}

//��ֵ������
CChiHuRight & CChiHuRight::operator = (DWORD dwRight)
{
	DWORD dwOtherRight = 0;
	//��֤Ȩλ
	if(!IsValidRight(dwRight))
	{
		//��֤ȡ��Ȩλ
		ASSERT(IsValidRight(~dwRight));
		if(!IsValidRight(~dwRight)) return *this;
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if((dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000))
			m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
		else m_dwRight[i] = dwOtherRight;
	}

	return *this;
}

//�����
CChiHuRight & CChiHuRight::operator &= (DWORD dwRight)
{
	bool bNavigate = false;
	//��֤Ȩλ
	if(!IsValidRight(dwRight))
	{
		//��֤ȡ��Ȩλ
		ASSERT(IsValidRight(~dwRight));
		if(!IsValidRight(~dwRight)) return *this;
		//����Ȩλ
		DWORD dwHeadRight = (~dwRight)&0xF0000000;
		DWORD dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight|dwTailRight;
		bNavigate = true;
	}

	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if((dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000))
		{
			m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if(!bNavigate)
			m_dwRight[i] = 0;
	}

	return *this;
}

//�����
CChiHuRight & CChiHuRight::operator |= (DWORD dwRight)
{
	//��֤Ȩλ
	if(!IsValidRight(dwRight)) return *this;

	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if((dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000))
		{
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
			break;
		}
	}

	return *this;
}

//��
CChiHuRight CChiHuRight::operator & (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator & (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator | (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//��
CChiHuRight CChiHuRight::operator | (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//���
bool CChiHuRight::operator == (DWORD dwRight) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)==chr;
}

//���
bool CChiHuRight::operator == (const CChiHuRight chr) const
{
	for(WORD i = 0; i < CountArray(m_dwRight); i++)
	{
		if(m_dwRight[i] != chr.m_dwRight[i]) return false;
	}
	return true;
}

//�����
bool CChiHuRight::operator != (DWORD dwRight) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)!=chr;
}

//�����
bool CChiHuRight::operator != (const CChiHuRight chr) const
{
	return !((*this)==chr);
}

//�Ƿ�ȨλΪ��
bool CChiHuRight::IsEmpty()
{
	for(BYTE i = 0; i < CountArray(m_dwRight); i++)
		if(m_dwRight[i]) return false;
	return true;
}

//����ȨλΪ��
void CChiHuRight::SetEmpty()
{
	ZeroMemory(m_dwRight,sizeof(m_dwRight));
	return;
}

//��ȡȨλ��ֵ
BYTE CChiHuRight::GetRightData(DWORD dwRight[], BYTE cbMaxCount)
{
	ASSERT(cbMaxCount >= CountArray(m_dwRight));
	if(cbMaxCount < CountArray(m_dwRight)) return 0;

	CopyMemory(dwRight,m_dwRight,sizeof(DWORD)*CountArray(m_dwRight));
	return CountArray(m_dwRight);
}

//����Ȩλ��ֵ
bool CChiHuRight::SetRightData(const DWORD dwRight[], BYTE cbRightCount)
{
	ASSERT(cbRightCount <= CountArray(m_dwRight));
	if(cbRightCount > CountArray(m_dwRight)) return false;

	ZeroMemory(m_dwRight,sizeof(m_dwRight));
	CopyMemory(m_dwRight,dwRight,sizeof(DWORD)*cbRightCount);

	return true;
}

//����λ�Ƿ���ȷ
bool CChiHuRight::IsValidRight(DWORD dwRight)
{
	DWORD dwRightHead = dwRight & 0xF0000000;
	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
		if(m_dwRightMask[i] == dwRightHead) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////

//��̬����

// //�齫����
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 	0x42,0x43,
// 		0x02,0x04,0x05,0x06,0x08,0x09,						//����
// 		0x02,0x04,0x05,0x06,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x08,0x09,						//����
// 		0x01,0x02,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		
// 		0x44, 0x45,0x46,0x47,0x48,								//�����ﶬ÷������				
// 
// 		0x21,0x22,0x23,0x24,
// 		
// 		
// 		0x23,
// 		0x26,0x27,
// 		0x07,
// 		0x07,
// 		0x07,
// 		0x16,
// 		0x16,
// 		0x03,
// 		0x03,0x01,0x01,0x41,
// 
// 		0x28,0x29,						//ͬ��
// 
// 		0x21,0x22,0x24,0x25,0x26,0x27,0x28,						//ͬ��
// 		0x25,
// 		0x03,0x16,0x29
// 
// 
// };			



//  const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
//  {
// 	 0x03,
// 	 0x01,0x02,0x04,0x06,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x05,0x06,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x05,0x06,0x07,0x08,0x09,						//����
//  
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 
// 		0x41,0x43,0x44, 0x45,0x46,0x47,0x48,
// 
//  		0x11,0x12,0x19,						//����
//  		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
//  		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
//  		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
//  		
//  		0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
//  		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
//  		0x21,0x22,
// 		0x04,
// 		0x04,
// 		
// 		0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
//  		0x21,0x22,0x23,0x24,0x25,0x26,0x04,0x27,0x28,0x29,						//ͬ��
//  					//�����ﶬ÷������
// 
// 		0x05,0x07,0x13,0x14,0x15,0x16,0x17,0x18,
// 	 0x21,0x22,0x23,0x42,0x07
//  };

//�齫���� ������
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
// {
// 	0x24,0x07,0x08,
// 	0x01,		0x05,				//����
// 
// 
// 
// 	0x01,0x02,0x04,0x05,0x06,0x07,0x08,						//����
// 	0x05,0x06,0x07,0x08,						//����
// 
// 
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,						//����
// 
// 
// 	0x31,0x32,0x33,0x34	,
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 
// 	0x21,0x25,0x26,0x27,0x28,	0x28,					//ͬ��
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,						//ͬ��
// 	0x21,0x22,0x23,
// 	0x35,0x36,0x37,									//����
// 
// 	0x41,0x42,0x43,
// 	0x01,0x02,0x03,0x04,
// 							//�����ﶬ÷������	
// 
// 	0x24,0x25,
// 	
// 	0x26,0x27,0x28,0x29,	0x09,0x29,0x29,0x44, 0x45,						//ͬ��
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,						//ͬ��
// 
// 
// 	0x22,0x23,0x06,0x29,
// 	0x03,
// 	0x09,
// 	0x09,
// 	0x09,
// 	0x02,0x03,0x04,0x46,0x47,0x48
// };

// �����
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
// {
// 	0x31,0x05,0x31,
// 		0x01,0x02,0x04,0x07,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 
// 		0x21,0x22,0x23,0x25,0x26,0x27,						//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 
// 		0x32,0x33,0x34	,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,
// 		
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 
// 		0x42,0x43,0x44, 0x45,0x46,0x47,0x48								//�����ﶬ÷������				
// 
// 
// 		,0x03,0x06,0x08,0x09,0x18,0x19,0x24,0x28,0x29
// 	,0x35,0x36,0x41,0x17
// };

// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 
// 		0x41,0x42,0x43,0x44, 0x45,0x46,0x47,0x48,								//�����ﶬ÷������				
// 
// 		
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 
// 		0x21,0x31,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 
// 
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x31,
// 		0x31,
// 		0x31
// 		
// 		
// };

// ��������  һ�ڶ���
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x06,0x07,0x08,
	0x16,

	0x26,
	0x26,
	0x01,0x05,0x09,						//����
		0x01,0x02,0x05,0x07,0x08,0x09,						//����
		0x01,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x18,0x19,						//����
		0x12,0x13,0x17,0x18,0x19,						//����
		0x12,0x13,0x17,0x18,0x19,						//����
		0x12,0x13,0x14,0x16,0x17,0x18,0x19,						//����
		
		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
		
		0x31,0x32,0x33,0x34	,0x35,0x36,									//����
		0x11,0x15,0x11,0x15,
		0x21,0x22,0x23,0x24,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x27,0x28,0x29,						//ͬ��



		
		0x43,0x44,							//�����ﶬ÷������
		0x16,

		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x26,0x27,0x28,0x29,						//ͬ��
		0x16,
		
		0x25,
		0x25,

		0x02,
		0x02,
		0x03,0x04,
		0x03,0x04,
		0x14,0x15,
		0x14,0x15,
		0x45,0x46,0x47,
		0x11,
		0x11,


		0x12,0x13,0x14,0x17,
		0x41,0x42,
		0x48,
		0x06,
		0x25,
		0x37,
		0x03,
		0x02,
		0x04,
};

// 2�������Ʋ��ԣ����Դ����Ҳ����
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
// {
// 	0x01,
// 	0x03,0x06,0x07,0x08,0x09,						//����
// 	0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 	0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 
// 	0x13,0x14,0x15,0x16,0x18,0x19,						//����
// 	0x13,0x14,0x15,0x16,0x18,0x19,						//����
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x18,0x19,						//����
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 
// 
// 	0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 	0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 	0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 
// 	0x41,0x42,0x43,0x44, 0x45,0x47,0x48,								//�����ﶬ÷������				
// 
// 
// 	0x23,0x24,0x25,0x29,						//ͬ��
// 	0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 
// 	0x02,0x03,
// 	0x01,0x02,
// 	0x21,0x22,
// 	0x21,0x22,
// 	0x11,0x12,
// 	0x11,0x12,
// 	0x31,
// 
// 
// 	0x31,
// 	0x31,
// 	0x26,0x27,0x28,
// 	0x17,
// 	0x17,
// 	0x17,
// 	0x04,0x05,
// 	0x02,
// 	0x01,
// 	
// 	0x46,
// };


// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x41,0x42,0x43,0x44, 0x45,0x46,0x47,0x48,								//�����ﶬ÷������				
// 
// };


// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 	0x05,0x06,0x07,
// 
// 		0x01,0x08,0x09,						//����
// 		0x06,0x07,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x09,						//����
// 
// 
// 		0x32,0x33,0x35,0x36,0x37,									//����
// 		0x32,0x33,0x35,0x36,0x37,									//����
// 		0x32,0x35,0x36,0x37,									//����
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x44, 0x45,0x46,0x47,0x48,								//�����ﶬ÷������			
// 
// 		0x33,
// 		0x14,0x19,						//����
// 					        0x14,0x17,0x18,0x19,						//����
// 					   0x14,0x15,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//����
// 
// 	
// 
// 		0x29,						//ͬ��
// 		0x22,0x23,0x24,0x25,0x27,0x28,0x29,						//ͬ��
// 		0x22,0x23,0x24,0x25,0x27,0x28,					//ͬ��
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,					//ͬ��
// 
// 		0x08,
// 		0x26,
// 		0x08,
// 		0x08,
// 		0x16,
// 		0x16,
// 		0x16,
// 		0x21,
// 		0x21,
// 		0x21,
// 		0x13,
// 		0x13,
// 		0x13,
// 
// 		0x31,
// 		0x31,
// 		0x22,0x23,0x24,
// 		0x16,0x17,0x18,
// 		0x01,0x02,0x03,0x04,0x05,
// 
// 		0x31,
// 		0x31,
// 		0x29,
// 		0x29,
// 		0x26,
// 		0x25,0x26,
// 		0x11,0x12,
// 		0x11,0x12,
// 		0x11,0x12,
// 
// 		0x34,
// 		0x34,
// 		0x34,
// 		0x27,0x28,
// 		0x15,
// 		0x15,
// 		0x02,0x03,0x04,
// 		0x41,0x42,0x43,
// };

// ������
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 	0x05,0x06,0x07,
// 		0x01,0x02,0x03,0x04,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
// 
// 	0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//����
// 		0x41,0x42,0x43,0x44, 0x45,								//�����ﶬ÷������				
// 
// 
// 		0x21,0x22,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x24,0x25,0x26,0x27,						//ͬ��
// 		0x21,0x22,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
// 		0x21,0x22,0x24,0x25,
// 		0x23,
// 		0x26,0x27,0x28,0x29,						//ͬ��
// 
// 
// 
// 		0x28,0x29,
// 		0x24,0x25,
// 		0x11,0x12,0x13,
// 		0x23,
// 		0x23,
// 		0x23,
// 		0x46,0x47,0x48,
// };

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	m_cbMagicIndex = MAX_INDEX;
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

#ifdef DEBUG
	CopyMemory(cbCardBuffer, m_cbCardDataArray, sizeof(m_cbCardDataArray));
#else

	//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=rand()%(cbBufferCount-cbRandCount);
			cbCardBuffer[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbBufferCount-cbRandCount];
		} while (cbRandCount<cbBufferCount);

#endif
	return;
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardData[], BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardDataTemp[MAX_HAND_COUNT] = {0};
	CopyMemory(cbCardDataTemp,cbCardData,sizeof(BYTE) * cbBufferCount);

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//ɾ���˿�
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//�������
			ASSERT(FALSE);

			//��ԭɾ��
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���˿�
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)] > 0);

	//ɾ���˿�
	BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex] > 0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	ASSERT(FALSE);

	return false;
}

//�����ж�
bool CGameLogic::IsMagicCard(BYTE cbCardData)
{
	if(m_cbMagicIndex != MAX_INDEX)
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

//�����ж�
bool CGameLogic::IsHuaCard(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return SwitchToCardIndex(cbCardData) >= FLOWER_INDEX;
}

//�����ж�
BYTE CGameLogic::IsHuaCard(BYTE cbCardIndex[MAX_INDEX])
{
	BYTE cbHuaCardCount = 0;
	for(int i = MAX_INDEX - FLOWER_INDEX; i < MAX_INDEX; i++)
	{
		if(cbCardIndex[i] > 0)
		{
			cbHuaCardCount += cbCardIndex[i];
		}
	}

	return cbHuaCardCount;
}

//����,������ֵ����
bool CGameLogic::SortCardList(BYTE cbCardData[MAX_HAND_COUNT], BYTE cbCardCount)
{
	//��Ŀ����
	if (cbCardCount==0||cbCardCount>MAX_HAND_COUNT) return false;

	//�������
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	BYTE cbCard1, cbCard2;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			cbCard1 = cbCardData[i];
			cbCard2 = cbCardData[i+1];
			//��������д����ƣ������������ת��
			if(INDEX_REPLACE_CARD != MAX_INDEX  && m_cbMagicIndex != INDEX_REPLACE_CARD)
			{
				if(SwitchToCardIndex(cbCard1) == INDEX_REPLACE_CARD) cbCard1 = SwitchToCardData(m_cbMagicIndex);
				if(SwitchToCardIndex(cbCard2) == INDEX_REPLACE_CARD) cbCard2 = SwitchToCardData(m_cbMagicIndex);
			}
			if (cbCard1>cbCard2)
			{
				//���ñ�־
				bSorted=false;

				//�˿�����
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return true;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//��������
	ASSERT(cbCardCount<=MAX_HAND_COUNT);
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[MAX_HAND_COUNT];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//�ɹ��ж�
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) 
			cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//�����ȼ�
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//���Ƶȼ�
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (cbUserAction&WIK_GANG) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&WIK_PENG) { return 2; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//���Ƶȼ�
WORD CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
{

	return 1;
}

//���ƺ���ͳ��
WORD CGameLogic::GetChiHuScore(const CChiHuRight & ChiHuRight, const BYTE cbCardIndex[MAX_INDEX], BYTE flowerCnt)
{
	WORD maxFlowerCnt = 40;

	logChiHuRight(ChiHuRight, "GetChiHuScore");

	if (!(ChiHuRight&CHR_DA_JIN_DIAO).IsEmpty())//���Ͽ���
	{
		return maxFlowerCnt;
	}

	WORD baseScore = GetBaseScore(ChiHuRight) + GetExtralBaseScore(cbCardIndex);

	WORD times = GetChiHuTimes(ChiHuRight);

	WORD score = (baseScore + flowerCnt) * times;

#ifdef DEBUG
	string str = "GetChiHuScore";
	str += " baseScore: " + std::to_string(baseScore);
	str += " times: " + std::to_string(times);
	str += " flowerCnt: " + std::to_string(flowerCnt);
	str += " score: " + std::to_string(score);

	_bstr_t bstrTmp(str.c_str());
	LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
	// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);

	CString strInfo(pszDescribeString);
	// 		CString strInfo;
	// 		strInfo.Format(TEXT("[%s]"), str.c_str());
	CGameLogic::NcaTextOut(strInfo);

#endif

	return score > maxFlowerCnt ? maxFlowerCnt : score;
}

//���Ʊ���
WORD CGameLogic::GetBaseScore(const CChiHuRight & ChiHuRight)
{
	WORD baseScore = 2;//ƽ��һ��
	if (!(ChiHuRight&CHR_DA_MENG_QING).IsEmpty())//
		baseScore = 20;

	if (!(ChiHuRight&CHR_XIAO_MENG_QING).IsEmpty())//
		baseScore = 7;
	return baseScore;
}

WORD CGameLogic::GetExtralBaseScore(const BYTE cbCardIndex[MAX_INDEX])
{
	WORD extralBaseScore = 0;//ƽ��һ��
	for (int i = FENG_INDEX; i < FLOWER_INDEX; ++i)
	{
		if (cbCardIndex[i] > 3)
		{
			++extralBaseScore;
		}
	}
	return extralBaseScore;
}

//���Ʊ���
WORD CGameLogic::GetChiHuTimes(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 0;//ƽ��һ��

	if (!(ChiHuRight&CHR_GANG_SHANG_HUA).IsEmpty())//���Ͽ���
		wFanShu += 2;
	
	if (!(ChiHuRight&CHR_QIANG_GANG_HU).IsEmpty())//����
	{
		wFanShu += 3;
	}

	if (!(ChiHuRight&CHR_PENG_PENG).IsEmpty())//
		wFanShu += 4;

	if (!(ChiHuRight&CHR_HUN_YI_SE).IsEmpty())//
		wFanShu += 4;

	if (!(ChiHuRight&CHR_QING_YI_SE).IsEmpty())//
		wFanShu += 8;

	return wFanShu > 0 ? wFanShu : 1;
}

//���Ʊ���
WORD CGameLogic::GetZuiTimes(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 1;//ƽ��һ��

	if (!(ChiHuRight&CHR_GANG_SHANG_HUA).IsEmpty())//���Ͽ���
		wFanShu = 2;

	if (!(ChiHuRight&CHR_QIANG_GANG_HU).IsEmpty())//����
	{
		wFanShu = 3;
	}

	return wFanShu;
}

//�Զ�����
BYTE CGameLogic::AutomatismOutCard(const BYTE cbCardIndex[MAX_INDEX], const BYTE cbEnjoinOutCard[MAX_HAND_COUNT], BYTE cbEnjoinOutCardCount)
{
	// �ȴ����
	if(m_cbMagicIndex != MAX_INDEX)
	{
		if(cbCardIndex[m_cbMagicIndex] > 0)
		{
			return SwitchToCardData(m_cbMagicIndex);
		}
	}

	//��������ƣ����ƴ��Լ���ģ���Ŀһ���Ͱ����������з��׵�˳��
	BYTE cbCardData = 0;
	BYTE cbOutCardIndex  = MAX_INDEX;
	BYTE cbOutCardIndexCount = 0;
	for(int i = MAX_INDEX - 7; i < MAX_INDEX - 1; i++)
	{
		if(cbCardIndex[i] > cbOutCardIndexCount)
		{
			cbOutCardIndexCount = cbCardIndex[i];
			cbOutCardIndex = i;
		}
	}

	if(cbOutCardIndex != MAX_INDEX)
	{
		cbCardData = SwitchToCardData(cbOutCardIndex);
		bool bEnjoinCard = false;
		for(int k = 0; k < cbEnjoinOutCardCount; k++)
		{
			if(cbCardData == cbEnjoinOutCard[k])
			{
				bEnjoinCard = true;
			}
		}
		if(!bEnjoinCard)
		{
			return cbCardData;
		}		
	}

	//û�����ƾʹ���ţ�1��9��˳��Ϊ��Ͳ����2��8������3��7������4��6������5
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			cbOutCardIndex  = MAX_INDEX;
			if(cbCardIndex[j * 9 + i] > 0)
			{
				cbOutCardIndex = j * 9 + i;
			}
			else if(cbCardIndex[j * 9 + (9 - i - 1)] > 0)
			{
				cbOutCardIndex = j * 9 + (9 - i - 1);
			}

			if(cbOutCardIndex != MAX_INDEX)
			{
				BYTE cbCardDataTemp = SwitchToCardData(cbOutCardIndex);
				bool bEnjoinCard = false;
				for(int k = 0; k < cbEnjoinOutCardCount; k++)
				{
					if(cbCardDataTemp == cbEnjoinOutCard[k])
					{
						bEnjoinCard = true;
					}
				}
				if(!bEnjoinCard)
				{
					return cbCardDataTemp;
				}
				else
				{
					if(cbCardData == 0)
					{
						cbCardData = cbCardDataTemp;
					}
				}
			}
		}
	}
	return cbCardData;
}

//�����ж�
BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//�����ж�
	if (cbCurrentIndex == m_cbMagicIndex) return WIK_NULL;
	if (cbCurrentIndex == INDEX_REPLACE_CARD && m_cbMagicIndex>=27) return WIK_NULL;
	if (cbCurrentCard >= 0x31 && cbCurrentIndex != INDEX_REPLACE_CARD) return WIK_NULL;

	//��������
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//��ַ���
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));

	//����в���
	BYTE cbMagicCardCount = 0;
	if(m_cbMagicIndex != MAX_INDEX)
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		//��������д����ƣ������������ת��
		if(INDEX_REPLACE_CARD != MAX_INDEX)
		{
			cbMagicCardIndex[m_cbMagicIndex] = cbMagicCardIndex[INDEX_REPLACE_CARD];
			cbMagicCardIndex[INDEX_REPLACE_CARD] = cbMagicCardCount;
		}
	}

	//�����ж�
	BYTE cbEatKind=0,cbFirstIndex=0;
	if (cbCurrentIndex == INDEX_REPLACE_CARD) cbCurrentIndex = m_cbMagicIndex;
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//�����ж�
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//���Ʋ��ܰ����в���
			if(m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2) continue;

			if ((cbCurrentIndex!=cbFirstIndex)&&(cbMagicCardIndex[cbFirstIndex]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbMagicCardIndex[cbFirstIndex+1]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbMagicCardIndex[cbFirstIndex+2]==0))
				continue;

			//��������
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}

//�����ж�
BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard))
	{
		return WIK_NULL;
	}

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//�����ж�
BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard)) 
		return WIK_NULL;


	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

//���Ʒ���
BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//���ñ���
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//���ϸ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i == m_cbMagicIndex) continue;
		if (cbCardIndex[i]==4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//��ϸ���
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}

BYTE CGameLogic::AnalyseGangCardEx(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE cbProvideCard, bool isSend, tagGangCardResult & GangCardResult)
{
	//���ñ���
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	
	if (!isSend)
	{
		return EstimateGangCard(cbCardIndex, cbProvideCard);
	}

	//���ϸ���
	for (BYTE i=0;i<FLOWER_INDEX;i++)
	{
		if(i == m_cbMagicIndex) continue;
		if (cbCardIndexTemp[i]>=4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//��ϸ���
	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind == WIK_PENG)
		{
			if (WeaveItem[i].cbCenterCard == cbProvideCard)//֮��ץ���ĵ��Ʋ��ܺ�����ɸ�
			{
				cbActionMask |= WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++] = WeaveItem[i].cbCenterCard;
			}
			else
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)] == 1)
				{
					cbActionMask |= WIK_GANG;
					GangCardResult.cbCardData[GangCardResult.cbCardCount++] = WeaveItem[i].cbCenterCard;
				}
			}
		}
	}

	return cbActionMask;
}

//�Ժ�����
BYTE CGameLogic::AnalyseCanHuForTing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, BYTE flowerCnt, bool isGui /*= false*/)
{
	// �л��ز��ܺ�
	if (IsHasFlowerCard(cbCardIndex))
	{
		return WIK_NULL;
	}

	//��������
	BYTE cbChiHuKind = WIK_NULL;
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//cbCurrentCardһ����Ϊ0			!!!!!!!!!
	ASSERT(cbCurrentCard != 0);
	if (cbCurrentCard == 0) return WIK_NULL;

	//�����˿�
	if (cbCurrentCard != 0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//�����˿�
	BYTE magicIndex = m_cbMagicIndex;
	if (isGui)
	{
		magicIndex = SwitchToCardIndex(cbCurrentCard);
	}

	if (HuLib::get_hu_info((char* const)cbCardIndexTemp, NULL, 0, 0, magicIndex, false))
	{
		if (IsMenQing(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard))
		{
			ChiHuRight |= CHR_XIAO_MENG_QING;
		}

		//���ͷ���
		if (!IsCanHu(cbCardIndexTemp, WeaveItem, cbWeaveCount, flowerCnt, ChiHuRight))
		{
			return cbChiHuKind;
		}

		cbChiHuKind = WIK_CHI_HU;
	}

	return cbChiHuKind;
}

//�Ժ�����
BYTE CGameLogic::AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, BYTE flowerCnt, bool isCheckTing /*= false*/)
{
	// �л��ز��ܺ�
	if (IsHasFlowerCard(cbCardIndex))
	{
		return WIK_NULL;
	}

	//��������
	BYTE cbChiHuKind=WIK_NULL;
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCardһ����Ϊ0			!!!!!!!!!
	ASSERT(cbCurrentCard != 0);
	if(cbCurrentCard == 0) return WIK_NULL;

	//�����˿�
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	

	//�����˿�
	if (HuLib::get_hu_info((char* const)cbCardIndexTemp, NULL, 0, 0, m_cbMagicIndex, false))
	{
		if (IsMenQing(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard))
		{
			ChiHuRight |= CHR_XIAO_MENG_QING;
		}

		//���ͷ���
		if (!IsCanHu(cbCardIndexTemp, WeaveItem, cbWeaveCount, flowerCnt, ChiHuRight))
		{
			return cbChiHuKind;
		}

		cbChiHuKind = WIK_CHI_HU;

		//������
		if (IsPengPeng(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard))
		{
			//����
			if (IsDaJinDiao(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard))
			{
				ChiHuRight |= CHR_DA_JIN_DIAO;
			}
			else
			{
				ChiHuRight |= CHR_PENG_PENG;
			}
		}

		BYTE huKind = IsQingYiSe(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard);
		if (huKind == 1)
		{
			ChiHuRight |= CHR_QING_YI_SE;
		}
		else if (huKind == 2)
		{
			ChiHuRight |= CHR_HUN_YI_SE;
		}

		if (ChiHuRight.IsEmpty())
		{
			ChiHuRight |= CHR_PING_HU;
		}

	}
// 	else
// 	{
// 		string str = "CenterCard: " + std::to_string(cbCurrentCard) + " cardData: ";
// 		for (int cnt = 0; cnt < FLOWER_INDEX; ++cnt)
// 		{
// 			if (cnt % 9 == 0)
// 			{
// 				str += "\n";
// 			}
// 
// 			str += " " + to_string(cbCardIndex[cnt]);
// 		}
// 
// 		str += " get_hu_info not hu xxxxxxxxxxxxxxx";
// 		_bstr_t bstrTmp(str.c_str());
// 		LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
// 		CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);
// 
// 		CString strInfo(pszDescribeString);
// 		// 		CString strInfo;
// 		// 		strInfo.Format(TEXT("[%s]"), str.c_str());
// 		CGameLogic::NcaTextOut(strInfo);
// 	}

	if (!ChiHuRight.IsEmpty())
	{
		cbChiHuKind = WIK_CHI_HU;
	}

	return cbChiHuKind;
}

void CGameLogic::logChiHuRight(const CChiHuRight &ChiHuRight, string strIn)
{

	string str = strIn + " logChiHuRight";

	if (!(ChiHuRight&CHR_PENG_PENG).IsEmpty())
	{
		str += " CHR_PENG_PENG";
	}

	if (!(ChiHuRight&CHR_HUN_YI_SE).IsEmpty())
	{
		str += " CHR_HUN_YI_SE";
	}

	if (!(ChiHuRight&CHR_QING_YI_SE).IsEmpty())
	{
		str += " CHR_QING_YI_SE";
	}

	if (!(ChiHuRight&CHR_XIAO_MENG_QING).IsEmpty())
	{
		str += " CHR_XIAO_MENG_QING";
	}
	
	if (!(ChiHuRight&CHR_DA_MENG_QING).IsEmpty())
	{
		str += " CHR_DA_MENG_QING";
	}

	if (!(ChiHuRight&CHR_GANG_SHANG_HUA).IsEmpty())
	{
		str += " CHR_GANG_SHANG_HUA";
	}

	if (!(ChiHuRight&CHR_QIANG_GANG_HU).IsEmpty())
	{
		str += " CHR_QIANG_GANG_HU";
	}


	_bstr_t bstrTmp(str.c_str());
	LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
	// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);

	CString strInfo(pszDescribeString);
	// 		CString strInfo;
	// 		strInfo.Format(TEXT("[%s]"), str.c_str());
	CGameLogic::NcaTextOut(strInfo);
}

bool CGameLogic::IsCanHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, BYTE flowerCnt, CChiHuRight &ChiHuRight)
{
	if (!(ChiHuRight&CHR_XIAO_MENG_QING).IsEmpty())
	{
		if (flowerCnt <= 0)
		{
			ChiHuRight &= (~CHR_XIAO_MENG_QING);
			ChiHuRight |= CHR_DA_MENG_QING;
		}

		return true;
	}

	if (flowerCnt < 3)
	{
		return false;
	}

	return true;
}

//���Ʒ���
BYTE CGameLogic::AnalyseTingCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE flowerCnt)
{
	//��������
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if((cbCardCount + 1) % 3 == 0)
	{
		for(BYTE i = 0; i < MAX_INDEX - FLOWER_INDEX; i++)
		{
			if(cbCardIndexTemp[i] == 0) continue;
			cbCardIndexTemp[i]--;

			for(BYTE j = 0; j < FLOWER_INDEX; j++)
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				if(WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp, WeaveItem, cbWeaveCount, cbCurrentCard, chr, flowerCnt))
					return WIK_LISTEN;
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		for(BYTE j = 0; j < FLOWER_INDEX; j++)
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if(WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, flowerCnt))
				return WIK_LISTEN;
		}
	}

	return WIK_NULL;
}

BYTE CGameLogic::AnalyseTingCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE flowerCnt, bool isGuoZhuang, BYTE& cbOutCardCount,BYTE cbOutCardData[],BYTE cbHuCardCount[],BYTE cbHuCardData[][28])
{
	// return 0;
	//��������
	BYTE cbOutCount = 0;
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	BYTE magicCard = 0;
	for (BYTE i = 0; i < FLOWER_INDEX; i++)
	{
		if (cbCardIndex[i] == 0)
		{
			magicCard = SwitchToCardData(i);
			break;
		}
	}

	if((cbCardCount-2)%3==0)
	{
		for(BYTE i = 0; i < FLOWER_INDEX; i++)
		{
			if (!isGuoZhuang && i >= FENG_INDEX)
			{
				break;
			}

			if (cbCardIndexTemp[i] == 0)
			{
				continue;
			}

			cbCardIndexTemp[i]--;

			bool bAdd = false;
			BYTE nCount = 0;
			if (WIK_CHI_HU == AnalyseCanHuForTing(cbCardIndexTemp, WeaveItem, cbWeaveCount, magicCard, chr, flowerCnt, true))
			{
				// ����ɺ���Щ��
				for (BYTE j = 0; j < FLOWER_INDEX; j++)
				{
					BYTE cbCurrentCard = SwitchToCardData(j);
					if (WIK_CHI_HU == AnalyseCanHuForTing(cbCardIndexTemp, WeaveItem, cbWeaveCount, cbCurrentCard, chr, flowerCnt, false))
					{
						if (bAdd == FALSE)
						{
							bAdd = true;
							cbOutCardData[cbOutCount++] = SwitchToCardData(i);
						}
						cbHuCardData[cbOutCount - 1][nCount++] = SwitchToCardData(j);
					}
				}
			}
			if (bAdd)
				cbHuCardCount[cbOutCount - 1] = nCount;

			cbCardIndexTemp[i]++;


// 			bool bAdd=false;
// 			BYTE nCount=0;
// 			for(BYTE j = 0; j < FLOWER_INDEX; j++)
// 			{
// 				BYTE cbCurrentCard = SwitchToCardData(j);
// 				if(WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, flowerCnt, false))
// 				{
// 					if(bAdd==FALSE)
// 					{
// 						bAdd=true;
// 						cbOutCardData[cbOutCount++] = SwitchToCardData(i);
// 					}
// 					cbHuCardData[cbOutCount-1][nCount++]=SwitchToCardData(j);
// 
// 					// break;
// 				}
// 			}
// 			if(bAdd)
// 				cbHuCardCount[cbOutCount-1]=nCount;
		}
	}
	else
	{
		BYTE cbCount=0;
		for( BYTE j = 0; j < FLOWER_INDEX; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, flowerCnt, true) )
			{
				cbHuCardData[0][cbCount++] = cbCurrentCard;
			}
		}
		cbHuCardCount[0]=cbCount;
	}

	cbOutCardCount = cbOutCount;
	return cbOutCount;
}

BYTE CGameLogic::GetHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,BYTE cbHuCardData[], BYTE flowerCnt)
{
	//��������
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCount = 0;
	ZeroMemory(cbHuCardData,sizeof(cbHuCardData));

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3 !=0 )
	{
		for( BYTE j = 0; j < MAX_INDEX; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, flowerCnt))
			{
				cbHuCardData[cbCount++] = cbCurrentCard;
			}
		}
		if(cbCount>0)
			return cbCount;
	}

	return 0;
}

//�Ƿ��ǻ���
bool CGameLogic::IsFlowerCard(BYTE cbCardData)
{
	auto cardIndex = SwitchToCardIndex(cbCardData);
	if (cardIndex >= FLOWER_INDEX)
	{
		return true;
	}

	return false;
}

//�Ƿ��л���
bool CGameLogic::IsHasFlowerCard(const BYTE cbCardIndex[MAX_INDEX])
{
	// �л��ز��ܺ�
	for (int i = FLOWER_INDEX; i < MAX_INDEX; ++i)
	{
		if (cbCardIndex[i] > 0)
		{
			return true;
		}
	}

	return false;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	if (cbCardIndex < FENG_INDEX)
		return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
	if (cbCardIndex < 34)
		return (0x30 | (cbCardIndex - FENG_INDEX + 1));
	return (0x40 | (cbCardIndex - 34 + 1));
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	if (cbCardData >= 0x41)
		return 33 + (cbCardData&MASK_VALUE);
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_HAND_COUNT])
{
	//ת���˿�
	BYTE cbPosition=0;
	//����
	if(m_cbMagicIndex != MAX_INDEX)
	{
		for(BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++)
			cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i == m_cbMagicIndex && m_cbMagicIndex != INDEX_REPLACE_CARD) 
		{
			//��������д����ƣ�������ƴ������ԭ����λ��
			if(INDEX_REPLACE_CARD != MAX_INDEX)
			{
				for(BYTE j = 0; j < cbCardIndex[INDEX_REPLACE_CARD]; j++)
					cbCardData[cbPosition++] = SwitchToCardData(INDEX_REPLACE_CARD);
			}
			continue;
		}
		if(i == INDEX_REPLACE_CARD) continue;
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_HAND_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���˿�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (!IsValidCard(cbCardData[i]))
		{
			ASSERT(IsValidCard(cbCardData[i]));
		}
		
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}


//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue = (cbCardData&MASK_VALUE);
	BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)) || ((cbValue >= 1) && (cbValue <= 8) && (cbColor == 4)));
}
//�˿���Ŀ
BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_GANG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

bool CGameLogic::AddKindItem(tagKindItem &TempKindItem, tagKindItem KindItem[], BYTE &cbKindItemCount, bool &bMagicThree)
{
	TempKindItem.cbMagicCount =  
		(m_cbMagicIndex == TempKindItem.cbValidIndex[0] ? 1 : 0) + 
		(m_cbMagicIndex == TempKindItem.cbValidIndex[1] ? 1 : 0) +
		(m_cbMagicIndex == TempKindItem.cbValidIndex[2] ? 1 : 0) ;

	if(TempKindItem.cbMagicCount>=3) 
	{
		if(!bMagicThree)
		{
			bMagicThree = true;
			CopyMemory(&KindItem[cbKindItemCount++],&TempKindItem,sizeof(TempKindItem));
			return true;
		}
		return false;
	}
	else if(TempKindItem.cbMagicCount == 2)
	{
		BYTE cbNoMagicIndex = 0;
		BYTE cbNoTempMagicIndex = 0;
		for(int i = 0; i < 3; i++)
		{
			if(TempKindItem.cbValidIndex[i] != m_cbMagicIndex)
			{
				cbNoTempMagicIndex = TempKindItem.cbValidIndex[i];
				break;
			}
		}
		bool bFind = false;
		for(int j = 0; j < cbKindItemCount; j++)
		{
			for(int i = 0; i < 3; i++)
			{
				if(KindItem[j].cbValidIndex[i] != m_cbMagicIndex)
				{
					cbNoMagicIndex = KindItem[j].cbValidIndex[i];
					break;
				}
			}
			if(cbNoMagicIndex == cbNoTempMagicIndex && cbNoMagicIndex != 0)
			{
				bFind = true;
			}
		}

		if(!bFind)
		{
			CopyMemory(&KindItem[cbKindItemCount++],&TempKindItem,sizeof(TempKindItem));
			return true;
		}
		return false;
	}
	else if(TempKindItem.cbMagicCount == 1)
	{
		BYTE cbTempCardIndex[2] = {0};
		BYTE cbCardIndex[2] = {0};
		BYTE cbCardCount = 0;
		for(int i = 0; i < 3; i++)
		{
			if(TempKindItem.cbValidIndex[i] != m_cbMagicIndex)
			{
				cbTempCardIndex[cbCardCount++] = TempKindItem.cbValidIndex[i];
			}
		}
		ASSERT(cbCardCount == 2);

		for(int j = 0; j < cbKindItemCount; j++)
		{
			if(1 == KindItem[j].cbMagicCount)
			{
				cbCardCount = 0;
				for(int i = 0; i < 3; i++)
				{
					if(KindItem[j].cbValidIndex[i] != m_cbMagicIndex)
					{
						cbCardIndex[cbCardCount++] = KindItem[j].cbValidIndex[i];
					}
				}
				ASSERT(cbCardCount == 2);

				if(cbTempCardIndex[0] == cbCardIndex[0] && cbTempCardIndex[1] == cbCardIndex[1])
				{
					return false;
				}
			}
		}


		CopyMemory(&KindItem[cbKindItemCount++],&TempKindItem,sizeof(TempKindItem));
		return true;
	}
	else
	{
		CopyMemory(&KindItem[cbKindItemCount++],&TempKindItem,sizeof(TempKindItem));
		return true;
	}
}

//�����˿�
bool CGameLogic::AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//������Ŀ
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_HAND_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_HAND_COUNT)||((cbCardCount-2)%3!=0))
		return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+28+16];
	ZeroMemory(KindItem,sizeof(KindItem));
	tagKindItem TempKindItem;
	ZeroMemory(&TempKindItem,sizeof(TempKindItem));
	bool bMagicThree=false;

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==MAX_WEAVE);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		ASSERT((cbCardCount==2)&&(cbWeaveCount==MAX_WEAVE));

		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2 || (m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2))
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
					CopyMemory(AnalyseItem.cbCardData[j],WeaveItem[j].cbCardData,sizeof(WeaveItem[j].cbCardData));
				}
				if(cbCardIndex[i] < 2 || i == m_cbMagicIndex)
					AnalyseItem.bMagicEye = true;
				else AnalyseItem.bMagicEye = false;
				AnalyseItem.cbCardEye=cbCardIndex[i]==0?SwitchToCardData(cbCardIndex[m_cbMagicIndex]):SwitchToCardData(i);

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//��ַ���
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));

	//����в���
	BYTE cbMagicCardCount = 0;
	BYTE cbTempMagicCount = 0;

	if(m_cbMagicIndex != MAX_INDEX)
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		//��������д����ƣ������������ת��
		if(INDEX_REPLACE_CARD != MAX_INDEX)
		{
			cbMagicCardIndex[m_cbMagicIndex] = cbMagicCardIndex[INDEX_REPLACE_CARD];
			cbMagicCardIndex[INDEX_REPLACE_CARD] = cbMagicCardCount;
		}
	}

	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<FLOWER_INDEX;i++)
		{
			//ͬ���ж�
			//����ǲ���,���Ҳ�����С��3,�򲻽������
			if(cbMagicCardIndex[i] >= 3 || (cbMagicCardIndex[i]+cbMagicCardCount >= 3 &&
				((INDEX_REPLACE_CARD!=MAX_INDEX && i != INDEX_REPLACE_CARD) || (INDEX_REPLACE_CARD==MAX_INDEX && i != m_cbMagicIndex)))
				)
			{
				int nTempIndex = cbMagicCardIndex[i];
				do
				{
					ASSERT(cbKindItemCount < CountArray(KindItem));
					BYTE cbIndex = i;
					BYTE cbCenterCard = SwitchToCardData(i);
					//����ǲ����Ҳ����д�����,�򻻳ɴ�����
					if(i == m_cbMagicIndex && INDEX_REPLACE_CARD != MAX_INDEX)
					{
						cbIndex = INDEX_REPLACE_CARD;
						cbCenterCard = SwitchToCardData(INDEX_REPLACE_CARD);
					}
					TempKindItem.cbWeaveKind=WIK_PENG;
					TempKindItem.cbCenterCard=cbCenterCard;
					TempKindItem.cbValidIndex[0] = nTempIndex>0?cbIndex:m_cbMagicIndex;
					TempKindItem.cbValidIndex[1] = nTempIndex>1?cbIndex:m_cbMagicIndex;
					TempKindItem.cbValidIndex[2] = nTempIndex>2?cbIndex:m_cbMagicIndex;
					AddKindItem(TempKindItem, KindItem, cbKindItemCount, bMagicThree);


					//��ǰ������δ�������� �Ҳ��������Ϊ0 
					if(nTempIndex>=3 && cbMagicCardCount >0)
					{
						--nTempIndex;
						//1��������֮���
						TempKindItem.cbWeaveKind=WIK_PENG;
						TempKindItem.cbCenterCard=cbCenterCard;
						TempKindItem.cbValidIndex[0] = nTempIndex>0?cbIndex:m_cbMagicIndex;
						TempKindItem.cbValidIndex[1] = nTempIndex>1?cbIndex:m_cbMagicIndex;
						TempKindItem.cbValidIndex[2] = nTempIndex>2?cbIndex:m_cbMagicIndex;
						AddKindItem(TempKindItem, KindItem, cbKindItemCount, bMagicThree);

						//����������֮���
						if(cbMagicCardCount>1)
						{
							TempKindItem.cbWeaveKind=WIK_PENG;
							TempKindItem.cbCenterCard=cbCenterCard;
							TempKindItem.cbValidIndex[0] = nTempIndex>0?cbIndex:m_cbMagicIndex;
							TempKindItem.cbValidIndex[1] = nTempIndex>1?cbIndex:m_cbMagicIndex;
							TempKindItem.cbValidIndex[2] = nTempIndex>2?cbIndex:m_cbMagicIndex;
							AddKindItem(TempKindItem, KindItem, cbKindItemCount, bMagicThree);
						}

						++nTempIndex;
					}

					//����ǲ���,���˳�
					if(i == INDEX_REPLACE_CARD || ((i == m_cbMagicIndex) && INDEX_REPLACE_CARD == MAX_INDEX))
						break;

					nTempIndex -= 3;
					//����պô���ȫ�������˳�
					if(nTempIndex == 0) break;

				}while(nTempIndex+cbMagicCardCount >= 3);
			}

			//�����ж�
			if ((i<(MAX_INDEX - MAX_HUA_INDEX -5))&&((i%9)<7))
			{
				//ֻҪ������������3��˳���������������ڵ���3,��������
				if(cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3)
				{
					BYTE cbIndex[3] = { cbMagicCardIndex[i],cbMagicCardIndex[i+1],cbMagicCardIndex[i+2] };

					if(cbIndex[0]+cbIndex[1]+cbIndex[2]==0) continue;

					int nMagicCountTemp;
					nMagicCountTemp = cbMagicCardCount;

					BYTE cbValidIndex[3];
					while(nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3)
					{
						for(BYTE j = 0; j < CountArray(cbIndex); j++)
						{
							if(cbIndex[j] > 0) 
							{
								cbIndex[j]--;
								cbValidIndex[j] = ((i+j==m_cbMagicIndex) && INDEX_REPLACE_CARD!=MAX_INDEX)?INDEX_REPLACE_CARD:i+j;
							}
							else 
							{
								nMagicCountTemp--;
								cbValidIndex[j] = m_cbMagicIndex;														
							}
						}
						if(nMagicCountTemp >= 0)
						{
							ASSERT(cbKindItemCount < CountArray(KindItem));
							TempKindItem.cbWeaveKind=WIK_LEFT;
							TempKindItem.cbCenterCard=SwitchToCardData(i);
							CopyMemory(TempKindItem.cbValidIndex,cbValidIndex,sizeof(cbValidIndex));
							AddKindItem(TempKindItem, KindItem, cbKindItemCount, bMagicThree);
						}
						else break;
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		ASSERT(27*2+28+16 >= cbKindItemCount);
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[MAX_WEAVE];
		for(BYTE i = 0; i < CountArray(cbIndex); i++)
			cbIndex[i] = i;

		tagKindItem * pKindItem[MAX_WEAVE];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		tagKindItem KindItemTemp[CountArray(KindItem)];

		//��ʼ���
		do
		{
			//����ĸ�����еĻ��ƴ������ϵĻ��Ƹ�������������
			cbTempMagicCount = 0;
			for(int i=0;i<cbLessKindItem;i++) cbTempMagicCount +=  KindItem[cbIndex[i]].cbMagicCount; 
			if(cbTempMagicCount <= cbMagicCardCount)
			{
				//���ñ���
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
				CopyMemory(KindItemTemp,KindItem,sizeof(KindItem));
	
				for (BYTE i=0;i<cbLessKindItem;i++)
					pKindItem[i]=&KindItemTemp[cbIndex[i]];


				//�����ж�
				bool bEnoughCard=true;

				for (BYTE i=0;i<cbLessKindItem*3;i++)
				{
					//�����ж�
					BYTE cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3]; 
					if (cbCardIndexTemp[cbCardIndex]==0)
					{
						if(m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[m_cbMagicIndex] > 0)
						{											
							pKindItem[i/3]->cbValidIndex[i%3] = m_cbMagicIndex;
							cbCardIndexTemp[m_cbMagicIndex]--;
						}
						else
						{
							bEnoughCard=false;
							break;
						}
					}
					else cbCardIndexTemp[cbCardIndex]--;
				}

				//�����ж�
				if (bEnoughCard==true)
				{
					//�����ж�
					BYTE cbCardEye=0;
					bool bMagicEye = false;
					if(GetCardCount(cbCardIndexTemp) == 2)
					{
						if(m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[m_cbMagicIndex]==2)
						{
							cbCardEye = SwitchToCardData(m_cbMagicIndex);
							bMagicEye = true;
						}
						else
						{
							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								if (cbCardIndexTemp[i]==2)
								{
									cbCardEye=SwitchToCardData(i);
									if(m_cbMagicIndex != MAX_INDEX && i == m_cbMagicIndex) 
									{
										bMagicEye = true;
									}
									break;
								}
								else if(i!=m_cbMagicIndex && m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+cbCardIndexTemp[m_cbMagicIndex]==2)
								{
									cbCardEye = SwitchToCardData(i);
									bMagicEye = true;
									break;
								}
							}
						}
					}

					//�������
					if (cbCardEye!=0)
					{
						//��������
						tagAnalyseItem AnalyseItem;
						ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

						//�������
						for (BYTE i=0;i<cbWeaveCount;i++)
						{
							AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
							AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
							GetWeaveCard(WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,AnalyseItem.cbCardData[i]);
						}

						//��������
						for (BYTE i=0;i<cbLessKindItem;i++) 
						{
							AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
							AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
							AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
							AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
							AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
						}

						//��������
						AnalyseItem.cbCardEye=cbCardEye;
						AnalyseItem.bMagicEye = bMagicEye;

						//������
						AnalyseItemArray.Add(AnalyseItem);
					}
				}
			}

			//��������
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				BYTE i;
				for (i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
							cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0)
					break;
			}
			else
				cbIndex[cbLessKindItem-1]++;

		} while (true);
	}

	if (AnalyseItemArray.GetCount()>0)
	{
		string str = "fen xi hu!!!!!!!!!!!!!!!!!";
		_bstr_t bstrTmp(str.c_str());
		LPCTSTR pszDescribeString = (LPCTSTR)bstrTmp;
		// CTraceService::TraceString(pszDescribeString, TraceLevel_Warning);

		CString strInfo(pszDescribeString);
		// 		CString strInfo;
		// 		strInfo.Format(TEXT("[%s]"), str.c_str());
		CGameLogic::NcaTextOut(strInfo);
	}

	return (AnalyseItemArray.GetCount()>0);
}

/*
// ������������
*/

//ƽ��
bool CGameLogic::IsPingHu(const tagAnalyseItem *pAnalyseItem)
{
	//������
	for (BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if (pAnalyseItem->cbWeaveKind[i] & (WIK_PENG | WIK_GANG)) return false;
	}

	return true;
}

//������
bool CGameLogic::IsPengPeng(const tagAnalyseItem *pAnalyseItem)
{
	for(BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
			return false;
	}
	return true;
}

//������
bool CGameLogic::IsPengPeng(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard)
{
	//�����ж�
	BYTE cbCardColor = 0xFF;

	bool isFoundEye = false;
	BYTE eyeIndex = MAX_INDEX;
	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	for (BYTE i = 0; i < FLOWER_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;

		if (cbCardIndexTemp[i] == 0)
		{
			continue;
		}

		if (cbCardIndexTemp[i] == 2)
		{
			isFoundEye = true;
			cbCardIndexTemp[i] = 0;
			eyeIndex = i;
			break;
		}
	}

	if (!isFoundEye)
	{
		return false;
	}

	for (BYTE i = 0; i < FLOWER_INDEX; i++)
	{
		if (i == m_cbMagicIndex) continue;

		if (i == eyeIndex)
		{
			continue;
		}

		if (cbCardIndexTemp[i] == 0)
		{
			continue;
		}

		if (cbCardIndexTemp[i] != 3)
		{
			return false;
		}
	}

	return true;
}

//����
bool CGameLogic::IsDaJinDiao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard)
{
	BYTE cardCnt = GetCardCount(cbCardIndex);
	if (cardCnt > 1)
	{
		return false;
	}

	return true;
}

//��һɫ��
BYTE CGameLogic::IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard)
{
	BYTE cbColorCount[MAX_COLOR] = { 0 };
	BYTE  cbCardColor = ((cbCurrentCard)&MASK_COLOR) >> 4;
	cbColorCount[cbCardColor] = 1;

	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndex[i] <= 0)
		{
			continue;
		}

		cbCardColor = (SwitchToCardData(i)&MASK_COLOR) >> 4;
		ASSERT(cbCardColor >= 0 && cbCardColor <= MAX_COLOR);
		if (0 == cbColorCount[cbCardColor])
		{
			cbColorCount[cbCardColor] = 1;
		}
	}

	for (BYTE i = 0; i < cbItemCount; i++)
	{
		BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
		cbCardColor = (cbCenterCard&MASK_COLOR) >> 4;
		ASSERT(cbCardColor >= 0 && cbCardColor <= MAX_COLOR);
		if (0 == cbColorCount[cbCardColor])
		{
			cbColorCount[cbCardColor] = 1;
		}
	}

	if (cbColorCount[0] + cbColorCount[1] + cbColorCount[2] == 1)
	{
		if (cbColorCount[3] == 0)
		{
			return 1;
		}
		
		if (cbColorCount[3] == 1)
		{
			return 2;
		}
	}

	return 0;
}

//��һɫ
bool CGameLogic::IsHunYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard)
{
	BYTE cbColorCount[MAX_COLOR] = { 0 };
	BYTE  cbCardColor = (SwitchToCardData(cbCurrentCard)&MASK_COLOR) >> 4;
	cbColorCount[cbCardColor] = 1;

	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		cbCardColor = (SwitchToCardData(i)&MASK_COLOR) >> 4;
		ASSERT(cbCardColor >= 0 && cbCardColor <= MAX_COLOR);
		if (0 == cbColorCount[cbCardColor])
		{
			cbColorCount[cbCardColor] = 1;
		}
	}

	for (BYTE i = 0; i < cbItemCount; i++)
	{
		BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
		cbCardColor = (cbCenterCard&MASK_COLOR) >> 4;
		ASSERT(cbCardColor >= 0 && cbCardColor <= MAX_COLOR);
		if (0 == cbColorCount[cbCardColor])
		{
			cbColorCount[cbCardColor] = 1;
		}
	}

	if (cbColorCount[0] + cbColorCount[1] + cbColorCount[2] == 1 && cbColorCount[3] == 1)
	{
		return true;
	}

	return false;
}

// ���� ��֤13�����ٺ���
bool CGameLogic::IsMenQing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard)
{
	if (cbWeaveCount > 0)
	{
		return false;
	}
	//������Ŀ
	BYTE cbCardCount = GetCardCount(cbCardIndex);

	return cbCardCount + 1 == MAX_HAND_COUNT;
}

//�Ժ�����
BYTE CGameLogic::AnalyseIsHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//��������
	BYTE cbChiHuKind = WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//�����˿�
	if (HuLib::get_hu_info((char* const)cbCardIndexTemp, NULL, 0, 0, m_cbMagicIndex, false))
	{
		cbChiHuKind = WIK_CHI_HU;
	}

	return cbChiHuKind;
}

bool CGameLogic::IsJiaHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE huCard)
{
	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	for (BYTE j = 0; j < MAX_INDEX; j++)
	{
		cbCardIndexTemp[j]++;

		if (WIK_CHI_HU == AnalyseIsHu(cbCardIndexTemp, WeaveItem, cbWeaveCount))
		{
			BYTE card = SwitchToCardData(j);
			if (card != huCard)
			{
				return false;
			}
		}
		cbCardIndexTemp[j]--;
	}

	return true;
}

bool CGameLogic::IsKanHu(const tagAnalyseItem *pAnalyseItem, BYTE cbCurrentCard)
{
	// ����
	if (cbCurrentCard == pAnalyseItem->cbCardEye)
	{
		return true;
	}

	for (BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		// 1. ����˳��
		// 2. ˳���е���Ҫ�е�ǰ��

		//���˿���
		if (!(pAnalyseItem->cbWeaveKind[i] & (WIK_LEFT | WIK_CENTER | WIK_RIGHT)))
			continue;

		for (BYTE j = 0; j < 3; j++)
		{
			if (pAnalyseItem->cbCardData[i][j] == cbCurrentCard)
			{
				if (j == 1 && pAnalyseItem->cbCardData[i][j] == cbCurrentCard)
				{
					return true;
				}
			}

			if (pAnalyseItem->cbCardData[i][j] == cbCurrentCard)
			{
				if (j == 0 && (pAnalyseItem->cbCenterCard[i] & MASK_VALUE) == 7 ||
					j == 2 && (pAnalyseItem->cbCenterCard[i] & MASK_VALUE) == 1)
				{
					return true;
				}
			}
		}
	}

	return false;
}

// �������ܺ����ܶ��ǿ�������Ϊһ��������Ϊ����
BYTE CGameLogic::GetTingCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, BYTE curCard)
{
	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	if (IsJiaHu(cbCardIndexTemp, WeaveItem, cbWeaveCount, curCard))
	{
		return 1;
	}

	return 0;
}

// �������ܺ����ܶ��ǿ�������Ϊһ��������Ϊ����
BYTE CGameLogic::GetKanCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount)
{
	BYTE kanCnt = 0;
	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] >= 3)
		{
			cbCardIndexTemp[i] -= 3;
			if (HuLib::get_hu_info((char* const)cbCardIndexTemp, NULL, 0, 0, m_cbMagicIndex, false))
			{
				++kanCnt;
			}
		}
	}

	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind != WIK_GANG)
		{
			continue;
		}

		kanCnt += 1;
		
		if (WeaveItem[i].cbParam == WIK_AN_GANG)
		{
			kanCnt += 1;
		}
	}

	return kanCnt;
}

//��Ǯ��Ͳ����123��456��789������Ǯ  ����Ҫ��������������
BYTE CGameLogic::GetWenQianCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount)
{
	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	tagWeaveItem tmpWeaveItem[MAX_WEAVE] = { 0 };
	CopyMemory(tmpWeaveItem, WeaveItem, sizeof(tagWeaveItem) * cbItemCount);
	BYTE cbTempWeaveItemCount = cbItemCount;

	BYTE wenQianCnt = 0;
	for (int i = WEN_QIAN_INDEX; i < WEN_QIAN_INDEX + 9; i = i + 3)
	{
		bool isKe = true; // һ���� ��������
		for (int maxCnt = 0; maxCnt < MAX_WEAVE; ++maxCnt) // ���������
		{
			for (int j = 0; j < 3; ++j)
			{
				if (cbCardIndexTemp[i + j] <= 0)
				{
					isKe = false;
					break;
				}
			}

			if (!isKe)
			{
				break;
			}

			BYTE cbCenterCard = SwitchToCardData(i);
			tmpWeaveItem[cbTempWeaveItemCount].cbCenterCard = cbCenterCard;
			tmpWeaveItem[cbTempWeaveItemCount].cbWeaveKind = WIK_PENG;
			tmpWeaveItem[cbTempWeaveItemCount].cbParam = WIK_GANERAL;
			tmpWeaveItem[cbTempWeaveItemCount].wProvideUser = 0;
			cbTempWeaveItemCount++;

			for (int j = 0; j < 3; ++j)
			{
				cbCardIndexTemp[i + j]--;
			}

			if (HuLib::get_hu_info((char* const)cbCardIndexTemp, NULL, 0, 0, m_cbMagicIndex, false))
			{
				++wenQianCnt;
			}
		}
	}

	return wenQianCnt > 4 ? 4 : wenQianCnt;
}

// Ʋ��ȱһ����һ�죬ȱ����������
BYTE CGameLogic::GetQueColorCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount)
{
	BYTE cbColorCount[MAX_COLOR] = { 0 };
	BYTE  cbCardColor = 0;

	for (BYTE i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndex[i] <= 0)
		{
			continue;
		}

		cbCardColor = (SwitchToCardData(i)&MASK_COLOR) >> 4;
		ASSERT(cbCardColor >= 0 && cbCardColor <= MAX_COLOR);
		if (0 == cbColorCount[cbCardColor])
		{
			cbColorCount[cbCardColor] = 1;
		}
	}

	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
		cbCardColor = (cbCenterCard&MASK_COLOR) >> 4;
		ASSERT(cbCardColor >= 0 && cbCardColor <= MAX_COLOR);
		if (0 == cbColorCount[cbCardColor])
		{
			cbColorCount[cbCardColor] = 1;
		}
	}

	return 3 - (cbColorCount[0] + cbColorCount[1] + cbColorCount[2]);
}

// �ԣ���һ����һ�죨����Ҳ��һ���죬���ܲ��㣩 
BYTE CGameLogic::GetDuiCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount)
{
	BYTE duiCnt = 0;
	for (BYTE i = 0; i < cbItemCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind == WIK_AN_GANG)
		{
			continue;
		}

		duiCnt += 1;
	}

	return duiCnt;
}

// �ˣ����Ż�������ϵ� ��/Ͳ/������������ܵ���
BYTE CGameLogic::GetBaCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount)
{
	BYTE cbColorCount[3] = { 0 };
	for (BYTE i = 0; i < FENG_INDEX; i = i + 9)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (cbCardIndex[i + j] > 0)
			{
				BYTE  cbCardColor = (SwitchToCardData(i + j)&MASK_COLOR) >> 4;
				cbColorCount[cbCardColor] += cbCardIndex[i + j];
			}
		}
	}

	for (BYTE i = 0; i < cbItemCount; i++)
	{
		BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
		auto cardIndex = SwitchToCardIndex(cbCenterCard);
		if (cardIndex >= FENG_INDEX)
		{
			continue;
		}

		BYTE  cbCardColor = ((cbCenterCard)&MASK_COLOR) >> 4;
		if (WeaveItem[i].cbWeaveKind == WIK_PENG)
		{
			
			cbColorCount[cbCardColor] += 3;
		}
		else if (WeaveItem[i].cbWeaveKind == WIK_GANG)
		{
			cbColorCount[cbCardColor] += 4;
		}
	}

	BYTE baCnt = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (cbColorCount[i] >= 8)
		{
			baCnt++;
		}
	}
	return baCnt;
}

// ����ӻ� 1. ���Ƶ�����1  
//		   2. �Ƿ������ܼ�1 ���ܼ�2
BYTE CGameLogic::GetExtralFlowerCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount)
{
	BYTE baCnt = 0;

	for (BYTE i = 0; i < cbItemCount; i++)
	{
		BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
		auto cardIndex = SwitchToCardIndex(cbCenterCard);
		
		if (WeaveItem[i].cbWeaveKind == WIK_PENG)
		{
			if (cardIndex >= FENG_INDEX && cardIndex < FLOWER_INDEX)
			{
				baCnt += 1;
			}
		}
		else if (WeaveItem[i].cbWeaveKind == WIK_GANG)
		{
			if (WeaveItem[i].cbParam == WIK_AN_GANG)
			{
				baCnt += 2;
			}
			else
			{
				baCnt += 1;
			}

			if (cardIndex >= FENG_INDEX && cardIndex < FLOWER_INDEX)
			{
				baCnt += 1;
			}
		}
	}

	return baCnt;
}


//////////////////////////////////////////////////////////////////////////////////
