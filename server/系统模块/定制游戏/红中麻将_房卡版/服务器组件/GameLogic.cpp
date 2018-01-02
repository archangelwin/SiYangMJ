#include "StdAfx.h"
#include "GameLogic.h"
#include "math.h"
#include <algorithm>
#include "tbl/hulib.h"


//////////////////////////////////////////////////////////////////////////

//静态变量
bool		CChiHuRight::m_bInit = false;
DWORD		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//构造函数
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

//赋值符重载
CChiHuRight & CChiHuRight::operator = (DWORD dwRight)
{
	DWORD dwOtherRight = 0;
	//验证权位
	if(!IsValidRight(dwRight))
	{
		//验证取反权位
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

//与等于
CChiHuRight & CChiHuRight::operator &= (DWORD dwRight)
{
	bool bNavigate = false;
	//验证权位
	if(!IsValidRight(dwRight))
	{
		//验证取反权位
		ASSERT(IsValidRight(~dwRight));
		if(!IsValidRight(~dwRight)) return *this;
		//调整权位
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

//或等于
CChiHuRight & CChiHuRight::operator |= (DWORD dwRight)
{
	//验证权位
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

//与
CChiHuRight CChiHuRight::operator & (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//与
CChiHuRight CChiHuRight::operator & (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//或
CChiHuRight CChiHuRight::operator | (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//或
CChiHuRight CChiHuRight::operator | (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//相等
bool CChiHuRight::operator == (DWORD dwRight) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)==chr;
}

//相等
bool CChiHuRight::operator == (const CChiHuRight chr) const
{
	for(WORD i = 0; i < CountArray(m_dwRight); i++)
	{
		if(m_dwRight[i] != chr.m_dwRight[i]) return false;
	}
	return true;
}

//不相等
bool CChiHuRight::operator != (DWORD dwRight) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)!=chr;
}

//不相等
bool CChiHuRight::operator != (const CChiHuRight chr) const
{
	return !((*this)==chr);
}

//是否权位为空
bool CChiHuRight::IsEmpty()
{
	for(BYTE i = 0; i < CountArray(m_dwRight); i++)
		if(m_dwRight[i]) return false;
	return true;
}

//设置权位为空
void CChiHuRight::SetEmpty()
{
	ZeroMemory(m_dwRight,sizeof(m_dwRight));
	return;
}

//获取权位数值
BYTE CChiHuRight::GetRightData(DWORD dwRight[], BYTE cbMaxCount)
{
	ASSERT(cbMaxCount >= CountArray(m_dwRight));
	if(cbMaxCount < CountArray(m_dwRight)) return 0;

	CopyMemory(dwRight,m_dwRight,sizeof(DWORD)*CountArray(m_dwRight));
	return CountArray(m_dwRight);
}

//设置权位数值
bool CChiHuRight::SetRightData(const DWORD dwRight[], BYTE cbRightCount)
{
	ASSERT(cbRightCount <= CountArray(m_dwRight));
	if(cbRightCount > CountArray(m_dwRight)) return false;

	ZeroMemory(m_dwRight,sizeof(m_dwRight));
	CopyMemory(m_dwRight,dwRight,sizeof(DWORD)*cbRightCount);

	return true;
}

//检查仅位是否正确
bool CChiHuRight::IsValidRight(DWORD dwRight)
{
	DWORD dwRightHead = dwRight & 0xF0000000;
	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
		if(m_dwRightMask[i] == dwRightHead) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////

//静态变量

// //麻将数据
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 	0x42,0x43,
// 		0x02,0x04,0x05,0x06,0x08,0x09,						//万子
// 		0x02,0x04,0x05,0x06,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x08,0x09,						//万子
// 		0x01,0x02,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		
// 		0x44, 0x45,0x46,0x47,0x48,								//春夏秋冬梅兰菊竹				
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
// 		0x28,0x29,						//同子
// 
// 		0x21,0x22,0x24,0x25,0x26,0x27,0x28,						//同子
// 		0x25,
// 		0x03,0x16,0x29
// 
// 
// };			



//  const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
//  {
// 	 0x03,
// 	 0x01,0x02,0x04,0x06,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x05,0x06,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x05,0x06,0x07,0x08,0x09,						//万子
//  
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 
// 		0x41,0x43,0x44, 0x45,0x46,0x47,0x48,
// 
//  		0x11,0x12,0x19,						//索子
//  		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
//  		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
//  		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
//  		
//  		0x24,0x25,0x26,0x27,0x28,0x29,						//同子
//  		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
//  		0x21,0x22,
// 		0x04,
// 		0x04,
// 		
// 		0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
//  		0x21,0x22,0x23,0x24,0x25,0x26,0x04,0x27,0x28,0x29,						//同子
//  					//春夏秋冬梅兰菊竹
// 
// 		0x05,0x07,0x13,0x14,0x15,0x16,0x17,0x18,
// 	 0x21,0x22,0x23,0x42,0x07
//  };

//麻将数据 上来听
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
// {
// 	0x24,0x07,0x08,
// 	0x01,		0x05,				//万子
// 
// 
// 
// 	0x01,0x02,0x04,0x05,0x06,0x07,0x08,						//万子
// 	0x05,0x06,0x07,0x08,						//万子
// 
// 
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,						//万子
// 
// 
// 	0x31,0x32,0x33,0x34	,
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 
// 	0x21,0x25,0x26,0x27,0x28,	0x28,					//同子
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,						//同子
// 	0x21,0x22,0x23,
// 	0x35,0x36,0x37,									//番子
// 
// 	0x41,0x42,0x43,
// 	0x01,0x02,0x03,0x04,
// 							//春夏秋冬梅兰菊竹	
// 
// 	0x24,0x25,
// 	
// 	0x26,0x27,0x28,0x29,	0x09,0x29,0x29,0x44, 0x45,						//同子
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,						//同子
// 
// 
// 	0x22,0x23,0x06,0x29,
// 	0x03,
// 	0x09,
// 	0x09,
// 	0x09,
// 	0x02,0x03,0x04,0x46,0x47,0x48
// };

// 碰后杠
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
// {
// 	0x31,0x05,0x31,
// 		0x01,0x02,0x04,0x07,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 
// 		0x21,0x22,0x23,0x25,0x26,0x27,						//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 
// 		0x32,0x33,0x34	,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,
// 		
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 
// 		0x42,0x43,0x44, 0x45,0x46,0x47,0x48								//春夏秋冬梅兰菊竹				
// 
// 
// 		,0x03,0x06,0x08,0x09,0x18,0x19,0x24,0x28,0x29
// 	,0x35,0x36,0x41,0x17
// };

// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 
// 		0x41,0x42,0x43,0x44, 0x45,0x46,0x47,0x48,								//春夏秋冬梅兰菊竹				
// 
// 		
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 
// 		0x21,0x31,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 
// 
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x31,
// 		0x31,
// 		0x31
// 		
// 		
// };

// 听不了牌  一炮多响
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x06,0x07,0x08,
	0x16,

	0x26,
	0x26,
	0x01,0x05,0x09,						//万子
		0x01,0x02,0x05,0x07,0x08,0x09,						//万子
		0x01,0x05,0x06,0x07,0x08,0x09,						//万子
		0x01,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x18,0x19,						//索子
		0x12,0x13,0x17,0x18,0x19,						//索子
		0x12,0x13,0x17,0x18,0x19,						//索子
		0x12,0x13,0x14,0x16,0x17,0x18,0x19,						//索子
		
		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
		
		0x31,0x32,0x33,0x34	,0x35,0x36,									//番子
		0x11,0x15,0x11,0x15,
		0x21,0x22,0x23,0x24,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x27,0x28,0x29,						//同子



		
		0x43,0x44,							//春夏秋冬梅兰菊竹
		0x16,

		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x26,0x27,0x28,0x29,						//同子
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

// 2花能听牌不对，可以打的牌也不对
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY] =
// {
// 	0x01,
// 	0x03,0x06,0x07,0x08,0x09,						//万子
// 	0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 	0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 
// 	0x13,0x14,0x15,0x16,0x18,0x19,						//索子
// 	0x13,0x14,0x15,0x16,0x18,0x19,						//索子
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x18,0x19,						//索子
// 	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 
// 
// 	0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 	0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 	0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 	0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 
// 	0x41,0x42,0x43,0x44, 0x45,0x47,0x48,								//春夏秋冬梅兰菊竹				
// 
// 
// 	0x23,0x24,0x25,0x29,						//同子
// 	0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
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
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x41,0x42,0x43,0x44, 0x45,0x46,0x47,0x48,								//春夏秋冬梅兰菊竹				
// 
// };


// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 	0x05,0x06,0x07,
// 
// 		0x01,0x08,0x09,						//万子
// 		0x06,0x07,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x09,						//万子
// 
// 
// 		0x32,0x33,0x35,0x36,0x37,									//番子
// 		0x32,0x33,0x35,0x36,0x37,									//番子
// 		0x32,0x35,0x36,0x37,									//番子
// 		0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x44, 0x45,0x46,0x47,0x48,								//春夏秋冬梅兰菊竹			
// 
// 		0x33,
// 		0x14,0x19,						//索子
// 					        0x14,0x17,0x18,0x19,						//索子
// 					   0x14,0x15,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x19,						//索子
// 
// 	
// 
// 		0x29,						//同子
// 		0x22,0x23,0x24,0x25,0x27,0x28,0x29,						//同子
// 		0x22,0x23,0x24,0x25,0x27,0x28,					//同子
// 		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,					//同子
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

// 不能听
// const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
// {
// 	0x05,0x06,0x07,
// 		0x01,0x02,0x03,0x04,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
// 
// 	0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
// 
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x31,0x32,0x33,0x34	,0x35,0x36,0x37,									//番子
// 		0x41,0x42,0x43,0x44, 0x45,								//春夏秋冬梅兰菊竹				
// 
// 
// 		0x21,0x22,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x24,0x25,0x26,0x27,						//同子
// 		0x21,0x22,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
// 		0x21,0x22,0x24,0x25,
// 		0x23,
// 		0x26,0x27,0x28,0x29,						//同子
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

//构造函数
CGameLogic::CGameLogic()
{
	m_cbMagicIndex = MAX_INDEX;
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

#ifdef DEBUG
	CopyMemory(cbCardBuffer, m_cbCardDataArray, sizeof(m_cbCardDataArray));
#else

	//混乱扑克
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

//混乱扑克
VOID CGameLogic::RandCardList(BYTE cbCardData[], BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//混乱准备
	BYTE cbCardDataTemp[MAX_HAND_COUNT] = {0};
	CopyMemory(cbCardDataTemp,cbCardData,sizeof(BYTE) * cbBufferCount);

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//删除扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//删除扑克
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//错误断言
			ASSERT(FALSE);

			//还原删除
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)] > 0);

	//删除扑克
	BYTE cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex] > 0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	ASSERT(FALSE);

	return false;
}

//财神判断
bool CGameLogic::IsMagicCard(BYTE cbCardData)
{
	if(m_cbMagicIndex != MAX_INDEX)
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

//花牌判断
bool CGameLogic::IsHuaCard(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return SwitchToCardIndex(cbCardData) >= FLOWER_INDEX;
}

//花牌判断
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

//排序,根据牌值排序
bool CGameLogic::SortCardList(BYTE cbCardData[MAX_HAND_COUNT], BYTE cbCardCount)
{
	//数目过虑
	if (cbCardCount==0||cbCardCount>MAX_HAND_COUNT) return false;

	//排序操作
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
			//如果财神有代替牌，财神与代替牌转换
			if(INDEX_REPLACE_CARD != MAX_INDEX  && m_cbMagicIndex != INDEX_REPLACE_CARD)
			{
				if(SwitchToCardIndex(cbCard1) == INDEX_REPLACE_CARD) cbCard1 = SwitchToCardData(m_cbMagicIndex);
				if(SwitchToCardIndex(cbCard2) == INDEX_REPLACE_CARD) cbCard2 = SwitchToCardData(m_cbMagicIndex);
			}
			if (cbCard1>cbCard2)
			{
				//设置标志
				bSorted=false;

				//扑克数据
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//检验数据
	ASSERT(cbCardCount<=MAX_HAND_COUNT);
	ASSERT(cbRemoveCount<=cbCardCount);

	//定义变量
	BYTE cbDeleteCount=0,cbTempCardData[MAX_HAND_COUNT];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//置零扑克
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

	//成功判断
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//清理扑克
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) 
			cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//动作等级
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//胡牌等级
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//杠牌等级
	if (cbUserAction&WIK_GANG) { return 3; }

	//碰牌等级
	if (cbUserAction&WIK_PENG) { return 2; }

	//上牌等级
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//胡牌等级
WORD CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
{

	return 1;
}

//胡牌后花数统计
WORD CGameLogic::GetChiHuScore(const CChiHuRight & ChiHuRight, const BYTE cbCardIndex[MAX_INDEX], BYTE flowerCnt)
{
	WORD maxFlowerCnt = 40;

	logChiHuRight(ChiHuRight, "GetChiHuScore");

	if (!(ChiHuRight&CHR_DA_JIN_DIAO).IsEmpty())//杠上开花
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

//胡牌倍数
WORD CGameLogic::GetBaseScore(const CChiHuRight & ChiHuRight)
{
	WORD baseScore = 2;//平胡一倍
	if (!(ChiHuRight&CHR_DA_MENG_QING).IsEmpty())//
		baseScore = 20;

	if (!(ChiHuRight&CHR_XIAO_MENG_QING).IsEmpty())//
		baseScore = 7;
	return baseScore;
}

WORD CGameLogic::GetExtralBaseScore(const BYTE cbCardIndex[MAX_INDEX])
{
	WORD extralBaseScore = 0;//平胡一倍
	for (int i = FENG_INDEX; i < FLOWER_INDEX; ++i)
	{
		if (cbCardIndex[i] > 3)
		{
			++extralBaseScore;
		}
	}
	return extralBaseScore;
}

//胡牌倍数
WORD CGameLogic::GetChiHuTimes(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 0;//平胡一倍

	if (!(ChiHuRight&CHR_GANG_SHANG_HUA).IsEmpty())//杠上开花
		wFanShu += 2;
	
	if (!(ChiHuRight&CHR_QIANG_GANG_HU).IsEmpty())//抢杠
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

//胡牌倍数
WORD CGameLogic::GetZuiTimes(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 1;//平胡一倍

	if (!(ChiHuRight&CHR_GANG_SHANG_HUA).IsEmpty())//杠上开花
		wFanShu = 2;

	if (!(ChiHuRight&CHR_QIANG_GANG_HU).IsEmpty())//抢杠
	{
		wFanShu = 3;
	}

	return wFanShu;
}

//自动出牌
BYTE CGameLogic::AutomatismOutCard(const BYTE cbCardIndex[MAX_INDEX], const BYTE cbEnjoinOutCard[MAX_HAND_COUNT], BYTE cbEnjoinOutCardCount)
{
	// 先打财神
	if(m_cbMagicIndex != MAX_INDEX)
	{
		if(cbCardIndex[m_cbMagicIndex] > 0)
		{
			return SwitchToCardData(m_cbMagicIndex);
		}
	}

	//而后打字牌，字牌打自己多的，数目一样就按东南西北中发白的顺序
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

	//没有字牌就打边张，1或9，顺序为万筒索，2，8，而后3，7，而后4，6，而后5
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

//吃牌判断
BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);

	//过滤判断
	if (cbCurrentIndex == m_cbMagicIndex) return WIK_NULL;
	if (cbCurrentIndex == INDEX_REPLACE_CARD && m_cbMagicIndex>=27) return WIK_NULL;
	if (cbCurrentCard >= 0x31 && cbCurrentIndex != INDEX_REPLACE_CARD) return WIK_NULL;

	//变量定义
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//拆分分析
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));

	//如果有财神
	BYTE cbMagicCardCount = 0;
	if(m_cbMagicIndex != MAX_INDEX)
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		//如果财神有代替牌，财神与代替牌转换
		if(INDEX_REPLACE_CARD != MAX_INDEX)
		{
			cbMagicCardIndex[m_cbMagicIndex] = cbMagicCardIndex[INDEX_REPLACE_CARD];
			cbMagicCardIndex[INDEX_REPLACE_CARD] = cbMagicCardCount;
		}
	}

	//吃牌判断
	BYTE cbEatKind=0,cbFirstIndex=0;
	if (cbCurrentIndex == INDEX_REPLACE_CARD) cbCurrentIndex = m_cbMagicIndex;
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//吃牌判断
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//吃牌不能包含有财神
			if(m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2) continue;

			if ((cbCurrentIndex!=cbFirstIndex)&&(cbMagicCardIndex[cbFirstIndex]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbMagicCardIndex[cbFirstIndex+1]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbMagicCardIndex[cbFirstIndex+2]==0))
				continue;

			//设置类型
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}

//碰牌判断
BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if (IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard))
	{
		return WIK_NULL;
	}

	//碰牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//杠牌判断
BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if (IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard)) 
		return WIK_NULL;


	//杠牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

//杠牌分析
BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//设置变量
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//手上杠牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i == m_cbMagicIndex) continue;
		if (cbCardIndex[i]==4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//组合杠牌
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
	//设置变量
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	
	if (!isSend)
	{
		return EstimateGangCard(cbCardIndex, cbProvideCard);
	}

	//手上杠牌
	for (BYTE i=0;i<FLOWER_INDEX;i++)
	{
		if(i == m_cbMagicIndex) continue;
		if (cbCardIndexTemp[i]>=4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//组合杠牌
	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind == WIK_PENG)
		{
			if (WeaveItem[i].cbCenterCard == cbProvideCard)//之后抓来的的牌才能和碰组成杠
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

//吃胡分析
BYTE CGameLogic::AnalyseCanHuForTing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, BYTE flowerCnt, bool isGui /*= false*/)
{
	// 有花必不能胡
	if (IsHasFlowerCard(cbCardIndex))
	{
		return WIK_NULL;
	}

	//变量定义
	BYTE cbChiHuKind = WIK_NULL;
	ChiHuRight.SetEmpty();

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//cbCurrentCard一定不为0			!!!!!!!!!
	ASSERT(cbCurrentCard != 0);
	if (cbCurrentCard == 0) return WIK_NULL;

	//插入扑克
	if (cbCurrentCard != 0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//分析扑克
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

		//牌型分析
		if (!IsCanHu(cbCardIndexTemp, WeaveItem, cbWeaveCount, flowerCnt, ChiHuRight))
		{
			return cbChiHuKind;
		}

		cbChiHuKind = WIK_CHI_HU;
	}

	return cbChiHuKind;
}

//吃胡分析
BYTE CGameLogic::AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, BYTE flowerCnt, bool isCheckTing /*= false*/)
{
	// 有花必不能胡
	if (IsHasFlowerCard(cbCardIndex))
	{
		return WIK_NULL;
	}

	//变量定义
	BYTE cbChiHuKind=WIK_NULL;
	ChiHuRight.SetEmpty();

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCard一定不为0			!!!!!!!!!
	ASSERT(cbCurrentCard != 0);
	if(cbCurrentCard == 0) return WIK_NULL;

	//插入扑克
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	

	//分析扑克
	if (HuLib::get_hu_info((char* const)cbCardIndexTemp, NULL, 0, 0, m_cbMagicIndex, false))
	{
		if (IsMenQing(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard))
		{
			ChiHuRight |= CHR_XIAO_MENG_QING;
		}

		//牌型分析
		if (!IsCanHu(cbCardIndexTemp, WeaveItem, cbWeaveCount, flowerCnt, ChiHuRight))
		{
			return cbChiHuKind;
		}

		cbChiHuKind = WIK_CHI_HU;

		//碰碰和
		if (IsPengPeng(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard))
		{
			//大金吊
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

//听牌分析
BYTE CGameLogic::AnalyseTingCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE flowerCnt)
{
	//复制数据
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
	//复制数据
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
				// 再算可胡哪些牌
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
	//复制数据
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

//是否是花牌
bool CGameLogic::IsFlowerCard(BYTE cbCardData)
{
	auto cardIndex = SwitchToCardIndex(cbCardData);
	if (cardIndex >= FLOWER_INDEX)
	{
		return true;
	}

	return false;
}

//是否有花牌
bool CGameLogic::IsHasFlowerCard(const BYTE cbCardIndex[MAX_INDEX])
{
	// 有花必不能胡
	for (int i = FLOWER_INDEX; i < MAX_INDEX; ++i)
	{
		if (cbCardIndex[i] > 0)
		{
			return true;
		}
	}

	return false;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	if (cbCardIndex < FENG_INDEX)
		return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
	if (cbCardIndex < 34)
		return (0x30 | (cbCardIndex - FENG_INDEX + 1));
	return (0x40 | (cbCardIndex - 34 + 1));
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	if (cbCardData >= 0x41)
		return 33 + (cbCardData&MASK_VALUE);
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_HAND_COUNT])
{
	//转换扑克
	BYTE cbPosition=0;
	//财神
	if(m_cbMagicIndex != MAX_INDEX)
	{
		for(BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++)
			cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i == m_cbMagicIndex && m_cbMagicIndex != INDEX_REPLACE_CARD) 
		{
			//如果财神有代替牌，则代替牌代替财神原来的位置
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

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//转换扑克
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


//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue = (cbCardData&MASK_VALUE);
	BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)) || ((cbValue >= 1) && (cbValue <= 8) && (cbColor == 4)));
}
//扑克数目
BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	//数目统计
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//获取组合
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_RIGHT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//碰牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_GANG:		//杠牌操作
		{
			//设置变量
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

//分析扑克
bool CGameLogic::AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//计算数目
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//效验数目
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_HAND_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_HAND_COUNT)||((cbCardCount-2)%3!=0))
		return false;

	//变量定义
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+28+16];
	ZeroMemory(KindItem,sizeof(KindItem));
	tagKindItem TempKindItem;
	ZeroMemory(&TempKindItem,sizeof(TempKindItem));
	bool bMagicThree=false;

	//需求判断
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==MAX_WEAVE);

	//单吊判断
	if (cbLessKindItem==0)
	{
		//效验参数
		ASSERT((cbCardCount==2)&&(cbWeaveCount==MAX_WEAVE));

		//牌眼判断
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2 || (m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2))
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//设置结果
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

				//插入结果
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//拆分分析
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));

	//如果有财神
	BYTE cbMagicCardCount = 0;
	BYTE cbTempMagicCount = 0;

	if(m_cbMagicIndex != MAX_INDEX)
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		//如果财神有代替牌，财神与代替牌转换
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
			//同牌判断
			//如果是财神,并且财神数小于3,则不进行组合
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
					//如果是财神且财神有代替牌,则换成代替牌
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


					//当前索引牌未与财神组合 且财神个数不为0 
					if(nTempIndex>=3 && cbMagicCardCount >0)
					{
						--nTempIndex;
						//1个财神与之组合
						TempKindItem.cbWeaveKind=WIK_PENG;
						TempKindItem.cbCenterCard=cbCenterCard;
						TempKindItem.cbValidIndex[0] = nTempIndex>0?cbIndex:m_cbMagicIndex;
						TempKindItem.cbValidIndex[1] = nTempIndex>1?cbIndex:m_cbMagicIndex;
						TempKindItem.cbValidIndex[2] = nTempIndex>2?cbIndex:m_cbMagicIndex;
						AddKindItem(TempKindItem, KindItem, cbKindItemCount, bMagicThree);

						//两个财神与之组合
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

					//如果是财神,则退出
					if(i == INDEX_REPLACE_CARD || ((i == m_cbMagicIndex) && INDEX_REPLACE_CARD == MAX_INDEX))
						break;

					nTempIndex -= 3;
					//如果刚好搭配全部，则退出
					if(nTempIndex == 0) break;

				}while(nTempIndex+cbMagicCardCount >= 3);
			}

			//连牌判断
			if ((i<(MAX_INDEX - MAX_HUA_INDEX -5))&&((i%9)<7))
			{
				//只要财神牌数加上3个顺序索引的牌数大于等于3,则进行组合
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

	//组合分析
	if (cbKindItemCount>=cbLessKindItem)
	{
		ASSERT(27*2+28+16 >= cbKindItemCount);
		//变量定义
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//变量定义
		BYTE cbIndex[MAX_WEAVE];
		for(BYTE i = 0; i < CountArray(cbIndex); i++)
			cbIndex[i] = i;

		tagKindItem * pKindItem[MAX_WEAVE];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		tagKindItem KindItemTemp[CountArray(KindItem)];

		//开始组合
		do
		{
			//如果四个组合中的混牌大于手上的混牌个数则重置索引
			cbTempMagicCount = 0;
			for(int i=0;i<cbLessKindItem;i++) cbTempMagicCount +=  KindItem[cbIndex[i]].cbMagicCount; 
			if(cbTempMagicCount <= cbMagicCardCount)
			{
				//设置变量
				CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
				CopyMemory(KindItemTemp,KindItem,sizeof(KindItem));
	
				for (BYTE i=0;i<cbLessKindItem;i++)
					pKindItem[i]=&KindItemTemp[cbIndex[i]];


				//数量判断
				bool bEnoughCard=true;

				for (BYTE i=0;i<cbLessKindItem*3;i++)
				{
					//存在判断
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

				//胡牌判断
				if (bEnoughCard==true)
				{
					//牌眼判断
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

					//组合类型
					if (cbCardEye!=0)
					{
						//变量定义
						tagAnalyseItem AnalyseItem;
						ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

						//设置组合
						for (BYTE i=0;i<cbWeaveCount;i++)
						{
							AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
							AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
							GetWeaveCard(WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,AnalyseItem.cbCardData[i]);
						}

						//设置牌型
						for (BYTE i=0;i<cbLessKindItem;i++) 
						{
							AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
							AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
							AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
							AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
							AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
						}

						//设置牌眼
						AnalyseItem.cbCardEye=cbCardEye;
						AnalyseItem.bMagicEye = bMagicEye;

						//插入结果
						AnalyseItemArray.Add(AnalyseItem);
					}
				}
			}

			//设置索引
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
// 胡法分析函数
*/

//平胡
bool CGameLogic::IsPingHu(const tagAnalyseItem *pAnalyseItem)
{
	//检查组合
	for (BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if (pAnalyseItem->cbWeaveKind[i] & (WIK_PENG | WIK_GANG)) return false;
	}

	return true;
}

//碰碰和
bool CGameLogic::IsPengPeng(const tagAnalyseItem *pAnalyseItem)
{
	for(BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
			return false;
	}
	return true;
}

//碰碰和
bool CGameLogic::IsPengPeng(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard)
{
	//胡牌判断
	BYTE cbCardColor = 0xFF;

	bool isFoundEye = false;
	BYTE eyeIndex = MAX_INDEX;
	//构造扑克
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

//大金吊
bool CGameLogic::IsDaJinDiao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount, const BYTE cbCurrentCard)
{
	BYTE cardCnt = GetCardCount(cbCardIndex);
	if (cardCnt > 1)
	{
		return false;
	}

	return true;
}

//清一色牌
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

//混一色
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

// 门清 保证13张牌再胡牌
bool CGameLogic::IsMenQing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE cbCurrentCard)
{
	if (cbWeaveCount > 0)
	{
		return false;
	}
	//计算数目
	BYTE cbCardCount = GetCardCount(cbCardIndex);

	return cbCardCount + 1 == MAX_HAND_COUNT;
}

//吃胡分析
BYTE CGameLogic::AnalyseIsHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//变量定义
	BYTE cbChiHuKind = WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//设置变量
	AnalyseItemArray.RemoveAll();

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//分析扑克
	if (HuLib::get_hu_info((char* const)cbCardIndexTemp, NULL, 0, 0, m_cbMagicIndex, false))
	{
		cbChiHuKind = WIK_CHI_HU;
	}

	return cbChiHuKind;
}

bool CGameLogic::IsJiaHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, const BYTE huCard)
{
	//构造扑克
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
	// 单吊
	if (cbCurrentCard == pAnalyseItem->cbCardEye)
	{
		return true;
	}

	for (BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		// 1. 必须顺子
		// 2. 顺子中的牌要有当前牌

		//过滤刻子
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

// 砍：暗杠和明杠都是砍，明杠为一砍，暗杠为两砍
BYTE CGameLogic::GetTingCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, BYTE curCard)
{
	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	if (IsJiaHu(cbCardIndexTemp, WeaveItem, cbWeaveCount, curCard))
	{
		return 1;
	}

	return 0;
}

// 砍：暗杠和明杠都是砍，明杠为一砍，暗杠为两砍
BYTE CGameLogic::GetKanCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount)
{
	BYTE kanCnt = 0;
	//构造扑克
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

//文钱：筒子牌123；456；789才算文钱  这里要包括发的那张牌
BYTE CGameLogic::GetWenQianCnt(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount)
{
	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	tagWeaveItem tmpWeaveItem[MAX_WEAVE] = { 0 };
	CopyMemory(tmpWeaveItem, WeaveItem, sizeof(tagWeaveItem) * cbItemCount);
	BYTE cbTempWeaveItemCount = cbItemCount;

	BYTE wenQianCnt = 0;
	for (int i = WEN_QIAN_INDEX; i < WEN_QIAN_INDEX + 9; i = i + 3)
	{
		bool isKe = true; // 一刻子 三张连牌
		for (int maxCnt = 0; maxCnt < MAX_WEAVE; ++maxCnt) // 最多四张碰
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

// 撇：缺一门算一嘴，缺两门算两嘴
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

// 对：碰一个算一嘴（明杠也算一个嘴，暗杠不算） 
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

// 八：八张或八张以上的 万/筒/条，包含碰或杠的牌
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

// 另外加花 1. 风牌的碰加1  
//		   2. 非风牌明杠加1 暗杠加2
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
